/******************************************************************************
 *
 * Copyright(c) 2009-2010  Realtek Corporation.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * The full GNU General Public License is included in this distribution in the
 * file called LICENSE.
 *
 * Contact Information:
 * wlanfae <wlanfae@realtek.com>
 * Realtek Corporation, No. 2, Innovation Road II, Hsinchu Science Park,
 * Hsinchu 300, Taiwan.
 *
 * Larry Finger <Larry.Finger@lwfinger.net>
 *
 *****************************************************************************/

#include "wifi.h"
#include "base.h"
#include "ps.h"
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,2,0))
#include <linux/export.h>
#endif

bool rtl_ps_enable_nic(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	bool init_status = true;
	
	/*<1> reset trx ring */
	if (rtlhal->interface == INTF_PCI)
		rtlpriv->intf_ops->reset_trx_ring(hw);

	if (is_hal_stop(rtlhal))
		RT_TRACE(COMP_ERR, DBG_WARNING,
			 ("Driver is already down!\n"));

	/*<2> Enable Adapter */
	rtlpriv->cfg->ops->hw_init(hw);
	RT_CLEAR_PS_LEVEL(ppsc, RT_RF_OFF_LEVL_HALT_NIC);
	/*init_status = false; */

	/*<3> Enable Interrupt */
	rtlpriv->cfg->ops->enable_interrupt(hw);

	/*<enable timer> */
	rtl_watch_dog_timer_callback((unsigned long)hw);

	return init_status;
}
EXPORT_SYMBOL(rtl_ps_enable_nic);

bool rtl_ps_disable_nic(struct ieee80211_hw *hw)
{
	bool status = true;
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	/*<1> Stop all timer */
	rtl_deinit_deferred_work(hw);

	/*<2> Disable Interrupt */
	rtlpriv->cfg->ops->disable_interrupt(hw);

	/*<3> Disable Adapter */
	rtlpriv->cfg->ops->hw_disable(hw);

	return status;
}
EXPORT_SYMBOL(rtl_ps_disable_nic);

bool rtl_ps_set_rf_state(struct ieee80211_hw *hw,
			 enum rf_pwrstate state_toset,
			 u32 changesource, bool protect_or_not)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	enum rf_pwrstate rtstate;
	bool b_actionallowed = false;
	u16 rfwait_cnt = 0;

	/*protect_or_not = true; */

	if (protect_or_not)
		goto no_protect;

	/*
	 *Only one thread can change
	 *the RF state at one time, and others
	 *should wait to be executed.
	 */
	while (true) {
		spin_lock(&rtlpriv->locks.rf_ps_lock);
		if (ppsc->rfchange_inprogress) {
			spin_unlock(&rtlpriv->locks.rf_ps_lock);

			RT_TRACE(COMP_ERR, DBG_WARNING,
				 ("RF Change in progress!"
				  "Wait to set..state_toset(%d).\n",
				  state_toset));

			/* Set RF after the previous action is done.  */
			while (ppsc->rfchange_inprogress) {
				rfwait_cnt++;
				mdelay(1);
				/*
				 *Wait too long, return false to avoid
				 *to be stuck here.
				 */
				if (rfwait_cnt > 100)
					return false;
			}
		} else {
			ppsc->rfchange_inprogress = true;
			spin_unlock(&rtlpriv->locks.rf_ps_lock);
			break;
		}
	}

no_protect:
	rtstate = ppsc->rfpwr_state;

	switch (state_toset) {
	case ERFON:
		ppsc->rfoff_reason &= (~changesource);

		if ((changesource == RF_CHANGE_BY_HW) &&
		    (ppsc->b_hwradiooff == true)) {
			ppsc->b_hwradiooff = false;
		}

		if (!ppsc->rfoff_reason) {
			ppsc->rfoff_reason = 0;
			b_actionallowed = true;
		}

		break;

	case ERFOFF:

		if ((changesource == RF_CHANGE_BY_HW)
		    && (ppsc->b_hwradiooff == false)) {
			ppsc->b_hwradiooff = true;
		}

		ppsc->rfoff_reason |= changesource;
		b_actionallowed = true;
		break;

	case ERFSLEEP:
		ppsc->rfoff_reason |= changesource;
		b_actionallowed = true;
		break;

	default:
		RT_TRACE(COMP_ERR, DBG_EMERG,
			 ("switch case not process \n"));
		break;
	}

	if (b_actionallowed)
		rtlpriv->cfg->ops->set_rf_power_state(hw, state_toset);

	if (!protect_or_not) {
		spin_lock(&rtlpriv->locks.rf_ps_lock);
		ppsc->rfchange_inprogress = false;
		spin_unlock(&rtlpriv->locks.rf_ps_lock);
	}

	return b_actionallowed;
}
EXPORT_SYMBOL(rtl_ps_set_rf_state);

static void _rtl_ps_inactive_ps(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));

	ppsc->b_swrf_processing = true;

	if (ppsc->inactive_pwrstate == ERFON && rtlhal->interface == INTF_PCI) {
		if ((ppsc->reg_rfps_level & RT_RF_OFF_LEVL_ASPM) &&
		    RT_IN_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM) &&
		    rtlhal->interface == INTF_PCI) {
			rtlpriv->intf_ops->disable_aspm(hw);
			RT_CLEAR_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM);
		}
	}

	rtl_ps_set_rf_state(hw, ppsc->inactive_pwrstate,
			    RF_CHANGE_BY_IPS, false);

	if (ppsc->inactive_pwrstate == ERFOFF && rtlhal->interface == INTF_PCI) {
		if (ppsc->reg_rfps_level & RT_RF_OFF_LEVL_ASPM && 
			!RT_IN_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM)) {
			rtlpriv->intf_ops->enable_aspm(hw);
			RT_SET_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM);
		}
	}

	ppsc->b_swrf_processing = false;
}

void rtl_ips_nic_off_wq_callback(void *data)
{
	struct rtl_works *rtlworks =
	    container_of_dwork_rtl(data, struct rtl_works, ips_nic_off_wq);
	struct ieee80211_hw *hw = rtlworks->hw;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	enum rf_pwrstate rtstate;

	if (mac->opmode != NL80211_IFTYPE_STATION) {
		RT_TRACE(COMP_ERR, DBG_WARNING,
			 ("not station return\n"));
		return;
	}
	
	if (mac->p2p_in_use)
		return;

	if (mac->link_state > MAC80211_NOLINK)
		return;

	if (is_hal_stop(rtlhal))
		return;

	if (rtlpriv->sec.being_setkey)
		return;

	if(rtlpriv->cfg->ops->bt_turn_off_bt_coexist_before_enter_lps)
		rtlpriv->cfg->ops->bt_turn_off_bt_coexist_before_enter_lps(hw);

	if (ppsc->b_inactiveps) {
		rtstate = ppsc->rfpwr_state;

		/*
		 *Do not enter IPS in the following conditions:
		 *(1) RF is already OFF or Sleep
		 *(2) b_swrf_processing (indicates the IPS is still under going)
		 *(3) Connectted (only disconnected can trigger IPS)
		 *(4) IBSS (send Beacon)
		 *(5) AP mode (send Beacon)
		 *(6) monitor mode (rcv packet)
		 */

		if (rtstate == ERFON &&
		    !ppsc->b_swrf_processing &&
		    (mac->link_state == MAC80211_NOLINK) &&
		    !mac->act_scanning) {
			RT_TRACE(COMP_RF, DBG_LOUD,
				 ("IPSEnter(): Turn off RF.\n"));

			ppsc->inactive_pwrstate = ERFOFF;
			ppsc->b_in_powersavemode = true;

			/*rtl_pci_reset_trx_ring(hw); */
			_rtl_ps_inactive_ps(hw);
		}
	}
}

void rtl_ips_nic_off(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	/*
	 *because when link with ap, mac80211 will ask us
	 *to disable nic quickly after scan before linking,
	 *this will cause link failed, so we delay 100ms here
	 */
	queue_delayed_work(rtlpriv->works.rtl_wq,
			   &rtlpriv->works.ips_nic_off_wq, MSECS(100));
}

/* NOTICE: any opmode should exc nic_on, or disable without
 * nic_on may something wrong, like adhoc TP*/
void rtl_ips_nic_on(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	enum rf_pwrstate rtstate;

	spin_lock(&rtlpriv->locks.ips_lock);
	if (ppsc->b_inactiveps) {
		rtstate = ppsc->rfpwr_state;

		if (rtstate != ERFON &&
		    !ppsc->b_swrf_processing &&
		    ppsc->rfoff_reason <= RF_CHANGE_BY_IPS) {

			ppsc->inactive_pwrstate = ERFON;
			ppsc->b_in_powersavemode = false;
			_rtl_ps_inactive_ps(hw);
		}
	}
	spin_unlock(&rtlpriv->locks.ips_lock);
}

/*for FW LPS*/

/*
 *Determine if we can set Fw into PS mode
 *in current condition.Return TRUE if it
 *can enter PS mode.
 */
static bool rtl_get_fwlps_doze(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	u32 ps_timediff;

	ps_timediff = jiffies_to_msecs(jiffies -
			ppsc->last_delaylps_stamp_jiffies);

	if (ps_timediff < 2000) {
		RT_TRACE(COMP_POWER, DBG_LOUD,
			 ("Delay enter Fw LPS for DHCP, ARP,"
			  " or EAPOL exchanging state.\n"));
		return false;
	}

	if (mac->link_state != MAC80211_LINKED)
		return false;

	if (mac->opmode == NL80211_IFTYPE_ADHOC)
		return false;

	return true;
}

/* Change current and default preamble mode.*/
static void rtl_lps_set_psmode(struct ieee80211_hw *hw, u8 rt_psmode)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	bool enter_fwlps;

	if (mac->opmode == NL80211_IFTYPE_ADHOC)
		return;

	if (mac->link_state != MAC80211_LINKED)
		return;

	if (ppsc->dot11_psmode == rt_psmode)
		return;

	/* Update power save mode configured. */
	ppsc->dot11_psmode = rt_psmode;

	/*
	 *<FW control LPS>
	 *1. Enter PS mode
	 *   Set RPWM to Fw to turn RF off and send H2C fw_pwrmode
	 *   cmd to set Fw into PS mode.
	 *2. Leave PS mode
	 *   Send H2C fw_pwrmode cmd to Fw to set Fw into Active
	 *   mode and set RPWM to turn RF on.
	 */

	if ((ppsc->b_fwctrl_lps) && ppsc->report_linked) {
		if (ppsc->dot11_psmode == EACTIVE) {
			RT_TRACE(COMP_RF, DBG_DMESG,
				 ("FW LPS leave ps_mode:%x\n",
				  FW_PS_ACTIVE_MODE));
				enter_fwlps = false;
				ppsc->pwr_mode = FW_PS_ACTIVE_MODE;
				ppsc->smart_ps = 0;
				rtlpriv->cfg->ops->set_hw_reg(hw,HW_VAR_FW_LPS_ACTION,
							(u8 *)(&enter_fwlps));
				if (ppsc->p2p_ps_info.opp_ps)
					rtl_p2p_ps_cmd(hw,P2P_PS_ENABLE);

		} else {
			if (rtl_get_fwlps_doze(hw)) {
				RT_TRACE(COMP_RF, DBG_DMESG,
						("FW LPS enter ps_mode:%x\n",
						 ppsc->fwctrl_psmode));
				enter_fwlps = true;
				ppsc->pwr_mode = ppsc->fwctrl_psmode;
				ppsc->smart_ps = 2;
				rtlpriv->cfg->ops->set_hw_reg(hw,HW_VAR_FW_LPS_ACTION,
							(u8 *)(&enter_fwlps));

			} else {
				/* Reset the power save related parameters. */
				ppsc->dot11_psmode = EACTIVE;
			}
		}
	}
}

/*Enter the leisure power save mode.*/
void rtl_lps_enter(struct ieee80211_hw *hw)
{
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	unsigned long flag;

	if (!ppsc->b_fwctrl_lps)
		return;

	if (rtlpriv->sec.being_setkey)
		return;

	if (rtlpriv->link_info.b_busytraffic)
		return;

	/*sleep after linked 10s, to let DHCP and 4-way handshake ok enough!! */
	if (mac->cnt_after_linked < 5)
		return;

	if (mac->opmode == NL80211_IFTYPE_ADHOC)
		return;

	if (mac->link_state != MAC80211_LINKED)
		return;

	spin_lock_irqsave(&rtlpriv->locks.lps_lock, flag);

	/* Idle for a while if we connect to AP a while ago. */
	if (mac->cnt_after_linked >= 2) {
		if (ppsc->dot11_psmode == EACTIVE) {
			RT_TRACE(COMP_POWER, DBG_LOUD,
					("Enter 802.11 power save mode...\n"));

			rtl_lps_set_psmode(hw, EAUTOPS);
		}
	}
	
	spin_unlock_irqrestore(&rtlpriv->locks.lps_lock, flag);
}

/*Leave the leisure power save mode.*/
void rtl_lps_leave(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	unsigned long flag;

	spin_lock_irqsave(&rtlpriv->locks.lps_lock, flag);

	if (ppsc->b_fwctrl_lps) {
		if (ppsc->dot11_psmode != EACTIVE) {

			/*FIX ME */
			rtlpriv->cfg->ops->enable_interrupt(hw);

			if (ppsc->reg_rfps_level & RT_RF_LPS_LEVEL_ASPM &&
			    RT_IN_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM) &&
			    rtlhal->interface == INTF_PCI) {
				rtlpriv->intf_ops->disable_aspm(hw);
				RT_CLEAR_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM);
			}

			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("Busy Traffic,Leave 802.11 power save..\n"));

			rtl_lps_set_psmode(hw, EACTIVE);
		}
	}
	spin_unlock_irqrestore(&rtlpriv->locks.lps_lock, flag);
}

/* For sw LPS*/
void rtl_swlps_beacon(struct ieee80211_hw *hw, void *data, unsigned int len)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct ieee80211_hdr *hdr = (void *) data;
	struct ieee80211_tim_ie *tim_ie;
	u8 *tim;
	u8 tim_len;
	bool u_buffed;
	bool m_buffed;

	if (mac->opmode != NL80211_IFTYPE_STATION)
		return;

	if (!rtlpriv->psc.b_swctrl_lps)
		return;
	
	if (rtlpriv->mac80211.link_state != MAC80211_LINKED)
		return;

	if (!rtlpriv->psc.sw_ps_enabled)
		return;

	if (rtlpriv->psc.b_fwctrl_lps)
		return;

	if (likely(!(hw->conf.flags & IEEE80211_CONF_PS)))
		return;

	/* check if this really is a beacon */
	if (!ieee80211_is_beacon(hdr->frame_control))
		return;

	/* min. beacon length + FCS_LEN */
	if (len <= 40 + FCS_LEN)
		return;

	/* and only beacons from the associated BSSID, please */
	if (compare_ether_addr(hdr->addr3, rtlpriv->mac80211.bssid))
		return;

	rtlpriv->psc.last_beacon = jiffies;

	tim = rtl_find_ie(data, len - FCS_LEN, WLAN_EID_TIM);
	if (!tim)
		return;

	if (tim[1] < sizeof(*tim_ie))
		return;

	tim_len = tim[1];
	tim_ie = (struct ieee80211_tim_ie *) &tim[2];

/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
/*<delete in kernel end>*/
	if (!WARN_ON_ONCE(!hw->conf.ps_dtim_period))
		rtlpriv->psc.dtim_counter = tim_ie->dtim_count;
/*<delete in kernel start>*/
#else
	if (!WARN_ON_ONCE(!mac->vif->bss_conf.dtim_period))
		rtlpriv->psc.dtim_counter = tim_ie->dtim_count;
#endif
/*<delete in kernel end>*/

	/* Check whenever the PHY can be turned off again. */

	/* 1. What about buffered unicast traffic for our AID? */
	u_buffed = ieee80211_check_tim(tim_ie, tim_len, rtlpriv->mac80211.assoc_id);

	/* 2. Maybe the AP wants to send multicast/broadcast data? */
	m_buffed = tim_ie->bitmap_ctrl & 0x01;
	rtlpriv->psc.multi_buffered = m_buffed;

	/* unicast will process by mac80211 through
	 * set ~IEEE80211_CONF_PS, So we just check 
	 * multicast frames here */
	if (!m_buffed ) {//&&) {// !rtlpriv->psc.tx_doing) {
		/* back to low-power land. and delay is 
		 * prevent null power save frame tx fail */
		queue_delayed_work(rtlpriv->works.rtl_wq,
				&rtlpriv->works.ps_work, MSECS(5));
	} else {
		RT_TRACE(COMP_POWER, DBG_DMESG, ("u_bufferd: %x, "
				"m_buffered: %x\n", u_buffed, m_buffed));
	}
}

void rtl_swlps_rf_awake(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	unsigned long flag;

	if (!rtlpriv->psc.b_swctrl_lps)
		return;
	if (mac->link_state != MAC80211_LINKED)
		return;
	
	if (ppsc->reg_rfps_level & RT_RF_LPS_LEVEL_ASPM && 
		RT_IN_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM)) {
		rtlpriv->intf_ops->disable_aspm(hw);
		RT_CLEAR_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM);
	}
	
	spin_lock_irqsave(&rtlpriv->locks.lps_lock, flag);
	rtl_ps_set_rf_state(hw, ERFON, RF_CHANGE_BY_PS, false);
	spin_unlock_irqrestore(&rtlpriv->locks.lps_lock, flag);
}

void rtl_swlps_rfon_wq_callback(void *data)
{
	struct rtl_works *rtlworks =
	    container_of_dwork_rtl(data, struct rtl_works, ps_rfon_wq);
	struct ieee80211_hw *hw = rtlworks->hw;

	rtl_swlps_rf_awake(hw);
}

void rtl_swlps_rf_sleep(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	unsigned long flag;
	u8 sleep_intv;

	if (!rtlpriv->psc.sw_ps_enabled)
		return;
	
	if ((rtlpriv->sec.being_setkey) || (mac->opmode == NL80211_IFTYPE_ADHOC))
		return;

	/*sleep after linked 10s, to let DHCP and 4-way handshake ok enough!! */
	if ((mac->link_state != MAC80211_LINKED) || (mac->cnt_after_linked < 5))
		return;

	if (rtlpriv->link_info.b_busytraffic)
		return;

	spin_lock(&rtlpriv->locks.rf_ps_lock);
	if (rtlpriv->psc.rfchange_inprogress) {
		spin_unlock(&rtlpriv->locks.rf_ps_lock);
		return;
	}	
	spin_unlock(&rtlpriv->locks.rf_ps_lock);

	spin_lock_irqsave(&rtlpriv->locks.lps_lock, flag);
	rtl_ps_set_rf_state(hw, ERFSLEEP, RF_CHANGE_BY_PS,false);
	spin_unlock_irqrestore(&rtlpriv->locks.lps_lock, flag);

	if (ppsc->reg_rfps_level & RT_RF_OFF_LEVL_ASPM && 
		!RT_IN_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM)) {
		rtlpriv->intf_ops->enable_aspm(hw);
		RT_SET_PS_LEVEL(ppsc, RT_PS_LEVEL_ASPM);
	}

	/* here is power save alg, when this beacon is DTIM
	 * we will set sleep time to dtim_period * n;
	 * when this beacon is not DTIM, we will set sleep
	 * time to sleep_intv = rtlpriv->psc.dtim_counter or
	 * MAX_SW_LPS_SLEEP_INTV(default set to 5) */
	 
/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,35))
/*<delete in kernel end>*/
	if (rtlpriv->psc.dtim_counter == 0) {
		if(hw->conf.ps_dtim_period == 1) 
			sleep_intv = hw->conf.ps_dtim_period * 2;
		else
			sleep_intv = hw->conf.ps_dtim_period;
	} else {
		sleep_intv = rtlpriv->psc.dtim_counter;
	}
/*<delete in kernel start>*/
#else
	if (rtlpriv->psc.dtim_counter == 0) {
		if(mac->vif->bss_conf.dtim_period == 1) 
			sleep_intv = mac->vif->bss_conf.dtim_period * 2;
		else
			sleep_intv = mac->vif->bss_conf.dtim_period;
	} else {
		sleep_intv = rtlpriv->psc.dtim_counter;
	}
#endif
/*<delete in kernel end>*/

	if(sleep_intv > MAX_SW_LPS_SLEEP_INTV) 
		sleep_intv = MAX_SW_LPS_SLEEP_INTV;

	/* this print should always be dtim_conter = 0 &
	 * sleep  = dtim_period, that meaons, we should 
	 * awake before every dtim */
	RT_TRACE(COMP_POWER, DBG_DMESG, ("dtim_counter:%x will sleep :%d"
			" beacon_intv\n", rtlpriv->psc.dtim_counter, sleep_intv));

	/* we tested that 40ms is enough for sw & hw sw delay */
	queue_delayed_work(rtlpriv->works.rtl_wq, &rtlpriv->works.ps_rfon_wq,
			MSECS(sleep_intv * mac->vif->bss_conf.beacon_int - 40));
}


void rtl_swlps_wq_callback(void *data)
{
	struct rtl_works *rtlworks = container_of_dwork_rtl(data, struct rtl_works,
			ps_work);
	struct ieee80211_hw *hw = rtlworks->hw;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	bool ps = false;

	ps = (hw->conf.flags & IEEE80211_CONF_PS);

	/* we can sleep after ps null send ok */
	if (rtlpriv->psc.state_inap) {
		rtl_swlps_rf_sleep(hw);

		if (rtlpriv->psc.state && !ps) {
			rtlpriv->psc.sleep_ms = jiffies_to_msecs(jiffies -
					rtlpriv->psc.last_action);
		}

		if (ps)
			rtlpriv->psc.last_slept = jiffies;

		rtlpriv->psc.last_action = jiffies;
		rtlpriv->psc.state = ps;
	}
}


void rtl_p2p_noa_ie(struct ieee80211_hw *hw, void *data, unsigned int len)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct ieee80211_mgmt *mgmt = (void *)data;
	struct rtl_p2p_ps_info *p2pinfo = &(rtlpriv->psc.p2p_ps_info);
	u8 *pos, *end, *ie;
	u16 noa_len;
	static u8 p2p_oui_ie_type[4] = {0x50,0x6f,0x9a,0x09};
	u8 noa_num, index,i, noa_index = 0;
	bool find_p2p_ie = false , find_p2p_ps_ie = false;
	pos = (u8 *)mgmt->u.beacon.variable;
	end = data + len;
	ie = NULL;
		
	while (pos + 1 < end) {
		
		if (pos + 2 + pos[1] > end)
			return;	
		
		if (pos[0] == 221 && pos[1] > 4) {
			if (memcmp(&pos[2], p2p_oui_ie_type, 4) == 0) {
				ie = pos + 2+4;
				break;
			}
		}		
		pos += 2 + pos[1];		
	}

	if (ie == NULL)
		return;
	find_p2p_ie = true;
	/*to find noa ie*/
	while (ie + 1 < end) {	
		noa_len = READEF2BYTE(&ie[1]);
		if (ie + 3 + ie[1] > end)
			return;	
		
		if (ie[0] == 12) {
			find_p2p_ps_ie = true;
			if ( (noa_len - 2) % 13 != 0){
				RT_TRACE(COMP_INIT, DBG_LOUD, ("P2P notice of absence: invalid length.%d\n",noa_len));
				return;
			} else {
				noa_num = (noa_len - 2) / 13;
			}
			noa_index = ie[3];
			if (rtlpriv->psc.p2p_ps_info.p2p_ps_mode == P2P_PS_NONE ||
				noa_index != p2pinfo->noa_index) {
				RT_TRACE(COMP_FW, DBG_LOUD, ("update NOA ie.\n"));
				p2pinfo->noa_index = noa_index;
				p2pinfo->opp_ps= (ie[4] >> 7);
				p2pinfo->ctwindow = ie[4] & 0x7F;
				p2pinfo->noa_num = noa_num;
				index = 5;
				for (i = 0; i< noa_num; i++){
					p2pinfo->noa_count_type[i] = READEF1BYTE(ie+index);
					index += 1;
					p2pinfo->noa_duration[i] = READEF4BYTE(ie+index);
					index += 4;
					p2pinfo->noa_interval[i] = READEF4BYTE(ie+index);
					index += 4;
					p2pinfo->noa_start_time[i] = READEF4BYTE(ie+index);
					index += 4;				
				}

				if (p2pinfo->opp_ps == 1) {
					p2pinfo->p2p_ps_mode = P2P_PS_CTWINDOW;
					/*Driver should wait LPS entering CTWindow*/
					if (rtlpriv->psc.b_fw_current_inpsmode){
						rtl_p2p_ps_cmd(hw, P2P_PS_ENABLE);
					}
				} else if (p2pinfo->noa_num > 0) {
					p2pinfo->p2p_ps_mode = P2P_PS_NOA;
					rtl_p2p_ps_cmd(hw, P2P_PS_ENABLE);
				} else if (p2pinfo->p2p_ps_mode > P2P_PS_NONE){
					rtl_p2p_ps_cmd(hw, P2P_PS_DISABLE);
				}
			}

		break;
		} 
		ie += 3 + noa_len;		
	}

	if (find_p2p_ie == true) {
		if ((p2pinfo->p2p_ps_mode > P2P_PS_NONE) && (find_p2p_ps_ie == false))
			rtl_p2p_ps_cmd(hw, P2P_PS_DISABLE);
	}	
}

void rtl_p2p_action_ie(struct ieee80211_hw *hw, void *data, unsigned int len)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct ieee80211_mgmt *mgmt = (void *)data;
	struct rtl_p2p_ps_info *p2pinfo = &(rtlpriv->psc.p2p_ps_info);
	bool find_p2p_ie = false , find_p2p_ps_ie = false;
	u8 noa_num, index,i, noa_index = 0;	
	u8 *pos, *end, *ie;
	u16 noa_len;
	static u8 p2p_oui_ie_type[4] = {0x50,0x6f,0x9a,0x09};

	pos = (u8 *) &mgmt->u.action.category;
	end = data + len;
	ie = NULL;
	
	if (pos[0] == 0x7f ) {
		if (memcmp(&pos[1], p2p_oui_ie_type, 4) == 0) {
			ie = pos + 3+4;			
		}
	} 

	if (ie == NULL)
		return;
	find_p2p_ie = true;

	RT_TRACE(COMP_FW, DBG_LOUD, ("action frame find P2P IE.\n"));
	/*to find noa ie*/
	while (ie + 1 < end) {	
		noa_len = READEF2BYTE(&ie[1]);
		if (ie + 3 + ie[1] > end)
			return;	
		
		if (ie[0] == 12) {
			RT_TRACE(COMP_FW, DBG_LOUD, ("find NOA IE.\n"));
			RT_PRINT_DATA(rtlpriv, COMP_FW, DBG_LOUD, ("noa ie "), ie, noa_len);
			find_p2p_ps_ie = true;
			if ( (noa_len - 2) % 13 != 0){
				RT_TRACE(COMP_FW, DBG_LOUD, ("P2P notice of absence: invalid length.%d\n",noa_len));
				return;
			} else {
				noa_num = (noa_len - 2) / 13;
			}
			noa_index = ie[3];
			if (rtlpriv->psc.p2p_ps_info.p2p_ps_mode == P2P_PS_NONE ||
				noa_index != p2pinfo->noa_index) {
				p2pinfo->noa_index = noa_index;
				p2pinfo->opp_ps= (ie[4] >> 7);
				p2pinfo->ctwindow = ie[4] & 0x7F;
				p2pinfo->noa_num = noa_num;
				index = 5;
				for (i = 0; i< noa_num; i++){
					p2pinfo->noa_count_type[i] = READEF1BYTE(ie+index);
					index += 1;
					p2pinfo->noa_duration[i] = READEF4BYTE(ie+index);
					index += 4;
					p2pinfo->noa_interval[i] = READEF4BYTE(ie+index);
					index += 4;
					p2pinfo->noa_start_time[i] = READEF4BYTE(ie+index);
					index += 4;				
				}

				if (p2pinfo->opp_ps == 1) {
					p2pinfo->p2p_ps_mode = P2P_PS_CTWINDOW;
					/*Driver should wait LPS entering CTWindow*/
					if (rtlpriv->psc.b_fw_current_inpsmode){
						rtl_p2p_ps_cmd(hw, P2P_PS_ENABLE);
					}
				} else if (p2pinfo->noa_num > 0) {
					p2pinfo->p2p_ps_mode = P2P_PS_NOA;
					rtl_p2p_ps_cmd(hw, P2P_PS_ENABLE);
				} else if (p2pinfo->p2p_ps_mode > P2P_PS_NONE){
					rtl_p2p_ps_cmd(hw, P2P_PS_DISABLE);
				}
			}

		break;
		} 
		ie += 3 + noa_len;		
	}
	

}


void rtl_p2p_ps_cmd(struct ieee80211_hw *hw,u8 p2p_ps_state)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *rtlps = rtl_psc(rtl_priv(hw));
	struct rtl_p2p_ps_info  *p2pinfo = &(rtlpriv->psc.p2p_ps_info); 
	
	RT_TRACE(COMP_FW, DBG_LOUD, (" p2p state %x\n",p2p_ps_state));
	switch (p2p_ps_state) {
		case P2P_PS_DISABLE:
			p2pinfo->p2p_ps_state = p2p_ps_state;
			rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_H2C_FW_P2P_PS_OFFLOAD, (u8 *)(&p2p_ps_state));

			p2pinfo->noa_index = 0;
			p2pinfo->ctwindow = 0;
			p2pinfo->opp_ps = 0;
			p2pinfo->noa_num = 0;
			p2pinfo->p2p_ps_mode = P2P_PS_NONE;
			if (rtlps->b_fw_current_inpsmode == true) {
				if (rtlps->smart_ps == 0) {
					rtlps->smart_ps = 2;
					rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_H2C_FW_PWRMODE, (u8 *)(&rtlps->pwr_mode));
				}

			}
			break;
		case P2P_PS_ENABLE:
			if (p2pinfo->p2p_ps_mode > P2P_PS_NONE) {
				p2pinfo->p2p_ps_state = p2p_ps_state;

				if (p2pinfo->ctwindow > 0) {
					if (rtlps->smart_ps != 0){
						rtlps->smart_ps = 0;
						rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_H2C_FW_PWRMODE, (u8 *)(&rtlps->pwr_mode));
					}
				}
				rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_H2C_FW_P2P_PS_OFFLOAD, (u8 *)(&p2p_ps_state));

			}
			break;
		case P2P_PS_SCAN:
		case P2P_PS_SCAN_DONE:
		case P2P_PS_ALLSTASLEEP:
			if (p2pinfo->p2p_ps_mode > P2P_PS_NONE) {
				p2pinfo->p2p_ps_state = p2p_ps_state;
				rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_H2C_FW_P2P_PS_OFFLOAD, (u8 *)(&p2p_ps_state));
			}
			break;
		default:
			break;

	}
	RT_TRACE(COMP_FW, DBG_LOUD, (" ctwindow %x oppps %x \n", p2pinfo->ctwindow,p2pinfo->opp_ps));	
	RT_TRACE(COMP_FW, DBG_LOUD, ("count %x duration %x index %x interval %x start time %x noa num %x\n",p2pinfo->noa_count_type[0],p2pinfo->noa_duration[0],
					p2pinfo->noa_index,p2pinfo->noa_interval[0],p2pinfo->noa_start_time[0],p2pinfo->noa_num));	
	RT_TRACE(COMP_FW, DBG_LOUD, ("end\n"));	
}

void rtl_p2p_info(struct ieee80211_hw *hw, void *data, unsigned int len)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct ieee80211_hdr *hdr = (void *) data;

	if (!mac->p2p)
		return;
	if (mac->link_state != MAC80211_LINKED)
		return;
	/* min. beacon length + FCS_LEN */
	if (len <= 40 + FCS_LEN)
		return;

	/* and only beacons from the associated BSSID, please */
	if (compare_ether_addr(hdr->addr3, rtlpriv->mac80211.bssid))
		return;
	
	/* check if this really is a beacon */
	if (!(ieee80211_is_beacon(hdr->frame_control) || ieee80211_is_probe_resp(hdr->frame_control)
		||ieee80211_is_action(hdr->frame_control)))
		return;	

	if (ieee80211_is_action(hdr->frame_control)){
		rtl_p2p_action_ie(hw,data,len - FCS_LEN);
	} else {
		rtl_p2p_noa_ie(hw,data,len - FCS_LEN);
	}
	
}
