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
 *****************************************************************************/

#include "../wifi.h"
#include "../base.h"
#include "reg.h"
#include "def.h"
#include "phy.h"
#include "dm.h"
#include "fw.h"

static const u32 ofdmswing_table[OFDM_TABLE_SIZE_92D] = {
	0x7f8001fe,		/* 0, +6.0dB */
	0x788001e2,		/* 1, +5.5dB */
	0x71c001c7,		/* 2, +5.0dB */
	0x6b8001ae,		/* 3, +4.5dB */
	0x65400195,		/* 4, +4.0dB */
	0x5fc0017f,		/* 5, +3.5dB */
	0x5a400169,		/* 6, +3.0dB */
	0x55400155,		/* 7, +2.5dB */
	0x50800142,		/* 8, +2.0dB */
	0x4c000130,		/* 9, +1.5dB */
	0x47c0011f,		/* 10, +1.0dB */
	0x43c0010f,		/* 11, +0.5dB */
	0x40000100,		/* 12, +0dB */
	0x3c8000f2,		/* 13, -0.5dB */
	0x390000e4,		/* 14, -1.0dB */
	0x35c000d7,		/* 15, -1.5dB */
	0x32c000cb,		/* 16, -2.0dB */
	0x300000c0,		/* 17, -2.5dB */
	0x2d4000b5,		/* 18, -3.0dB */
	0x2ac000ab,		/* 19, -3.5dB */
	0x288000a2,		/* 20, -4.0dB */
	0x26000098,		/* 21, -4.5dB */
	0x24000090,		/* 22, -5.0dB */
	0x22000088,		/* 23, -5.5dB */
	0x20000080,		/* 24, -6.0dB */
	0x1e400079,		/* 25, -6.5dB */
	0x1c800072,		/* 26, -7.0dB */
	0x1b00006c,		/* 27. -7.5dB */
	0x19800066,		/* 28, -8.0dB */
	0x18000060,		/* 29, -8.5dB */
	0x16c0005b,		/* 30, -9.0dB */
	0x15800056,		/* 31, -9.5dB */
	0x14400051,		/* 32, -10.0dB */
	0x1300004c,		/* 33, -10.5dB */
	0x12000048,		/* 34, -11.0dB */
	0x11000044,		/* 35, -11.5dB */
	0x10000040,		/* 36, -12.0dB */
	0x0f00003c,		/* 37, -12.5dB */
	0x0e400039,		/* 38, -13.0dB */
	0x0d800036,		/* 39, -13.5dB */
	0x0cc00033,		/* 40, -14.0dB */
	0x0c000030,		/* 41, -14.5dB */
	0x0b40002d,		/* 42, -15.0dB */
};

static const u8 cckswing_table_ch1ch13[CCK_TABLE_SIZE][8] = {
	{0x36, 0x35, 0x2e, 0x25, 0x1c, 0x12, 0x09, 0x04},	/* 0, +0dB */
	{0x33, 0x32, 0x2b, 0x23, 0x1a, 0x11, 0x08, 0x04},	/* 1, -0.5dB */
	{0x30, 0x2f, 0x29, 0x21, 0x19, 0x10, 0x08, 0x03},	/* 2, -1.0dB */
	{0x2d, 0x2d, 0x27, 0x1f, 0x18, 0x0f, 0x08, 0x03},	/* 3, -1.5dB */
	{0x2b, 0x2a, 0x25, 0x1e, 0x16, 0x0e, 0x07, 0x03},	/* 4, -2.0dB */
	{0x28, 0x28, 0x22, 0x1c, 0x15, 0x0d, 0x07, 0x03},	/* 5, -2.5dB */
	{0x26, 0x25, 0x21, 0x1b, 0x14, 0x0d, 0x06, 0x03},	/* 6, -3.0dB */
	{0x24, 0x23, 0x1f, 0x19, 0x13, 0x0c, 0x06, 0x03},	/* 7, -3.5dB */
	{0x22, 0x21, 0x1d, 0x18, 0x11, 0x0b, 0x06, 0x02},	/* 8, -4.0dB */
	{0x20, 0x20, 0x1b, 0x16, 0x11, 0x08, 0x05, 0x02},	/* 9, -4.5dB */
	{0x1f, 0x1e, 0x1a, 0x15, 0x10, 0x0a, 0x05, 0x02},	/* 10, -5.0dB */
	{0x1d, 0x1c, 0x18, 0x14, 0x0f, 0x0a, 0x05, 0x02},	/* 11, -5.5dB */
	{0x1b, 0x1a, 0x17, 0x13, 0x0e, 0x09, 0x04, 0x02},	/* 12, -6.0dB */
	{0x1a, 0x19, 0x16, 0x12, 0x0d, 0x09, 0x04, 0x02},	/* 13, -6.5dB */
	{0x18, 0x17, 0x15, 0x11, 0x0c, 0x08, 0x04, 0x02},	/* 14, -7.0dB */
	{0x17, 0x16, 0x13, 0x10, 0x0c, 0x08, 0x04, 0x02},	/* 15, -7.5dB */
	{0x16, 0x15, 0x12, 0x0f, 0x0b, 0x07, 0x04, 0x01},	/* 16, -8.0dB */
	{0x14, 0x14, 0x11, 0x0e, 0x0b, 0x07, 0x03, 0x02},	/* 17, -8.5dB */
	{0x13, 0x13, 0x10, 0x0d, 0x0a, 0x06, 0x03, 0x01},	/* 18, -9.0dB */
	{0x12, 0x12, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},	/* 19, -9.5dB */
	{0x11, 0x11, 0x0f, 0x0c, 0x09, 0x06, 0x03, 0x01},	/* 20, -10.0dB */
	{0x10, 0x10, 0x0e, 0x0b, 0x08, 0x05, 0x03, 0x01},	/* 21, -10.5dB */
	{0x0f, 0x0f, 0x0d, 0x0b, 0x08, 0x05, 0x03, 0x01},	/* 22, -11.0dB */
	{0x0e, 0x0e, 0x0c, 0x0a, 0x08, 0x05, 0x02, 0x01},	/* 23, -11.5dB */
	{0x0d, 0x0d, 0x0c, 0x0a, 0x07, 0x05, 0x02, 0x01},	/* 24, -12.0dB */
	{0x0d, 0x0c, 0x0b, 0x09, 0x07, 0x04, 0x02, 0x01},	/* 25, -12.5dB */
	{0x0c, 0x0c, 0x0a, 0x09, 0x06, 0x04, 0x02, 0x01},	/* 26, -13.0dB */
	{0x0b, 0x0b, 0x0a, 0x08, 0x06, 0x04, 0x02, 0x01},	/* 27, -13.5dB */
	{0x0b, 0x0a, 0x09, 0x08, 0x06, 0x04, 0x02, 0x01},	/* 28, -14.0dB */
	{0x0a, 0x0a, 0x09, 0x07, 0x05, 0x03, 0x02, 0x01},	/* 29, -14.5dB */
	{0x0a, 0x09, 0x08, 0x07, 0x05, 0x03, 0x02, 0x01},	/* 30, -15.0dB */
	{0x09, 0x09, 0x08, 0x06, 0x05, 0x03, 0x01, 0x01},	/* 31, -15.5dB */
	{0x09, 0x08, 0x07, 0x06, 0x04, 0x03, 0x01, 0x01}	/* 32, -16.0dB */
};

static const u8 cckswing_table_ch14[CCK_TABLE_SIZE][8] = {
	{0x36, 0x35, 0x2e, 0x1b, 0x00, 0x00, 0x00, 0x00},	/* 0, +0dB */
	{0x33, 0x32, 0x2b, 0x19, 0x00, 0x00, 0x00, 0x00},	/* 1, -0.5dB */
	{0x30, 0x2f, 0x29, 0x18, 0x00, 0x00, 0x00, 0x00},	/* 2, -1.0dB */
	{0x2d, 0x2d, 0x17, 0x17, 0x00, 0x00, 0x00, 0x00},	/* 3, -1.5dB */
	{0x2b, 0x2a, 0x25, 0x15, 0x00, 0x00, 0x00, 0x00},	/* 4, -2.0dB */
	{0x28, 0x28, 0x24, 0x14, 0x00, 0x00, 0x00, 0x00},	/* 5, -2.5dB */
	{0x26, 0x25, 0x21, 0x13, 0x00, 0x00, 0x00, 0x00},	/* 6, -3.0dB */
	{0x24, 0x23, 0x1f, 0x12, 0x00, 0x00, 0x00, 0x00},	/* 7, -3.5dB */
	{0x22, 0x21, 0x1d, 0x11, 0x00, 0x00, 0x00, 0x00},	/* 8, -4.0dB */
	{0x20, 0x20, 0x1b, 0x10, 0x00, 0x00, 0x00, 0x00},	/* 9, -4.5dB */
	{0x1f, 0x1e, 0x1a, 0x0f, 0x00, 0x00, 0x00, 0x00},	/* 10, -5.0dB */
	{0x1d, 0x1c, 0x18, 0x0e, 0x00, 0x00, 0x00, 0x00},	/* 11, -5.5dB */
	{0x1b, 0x1a, 0x17, 0x0e, 0x00, 0x00, 0x00, 0x00},	/* 12, -6.0dB */
	{0x1a, 0x19, 0x16, 0x0d, 0x00, 0x00, 0x00, 0x00},	/* 13, -6.5dB */
	{0x18, 0x17, 0x15, 0x0c, 0x00, 0x00, 0x00, 0x00},	/* 14, -7.0dB */
	{0x17, 0x16, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00},	/* 15, -7.5dB */
	{0x16, 0x15, 0x12, 0x0b, 0x00, 0x00, 0x00, 0x00},	/* 16, -8.0dB */
	{0x14, 0x14, 0x11, 0x0a, 0x00, 0x00, 0x00, 0x00},	/* 17, -8.5dB */
	{0x13, 0x13, 0x10, 0x0a, 0x00, 0x00, 0x00, 0x00},	/* 18, -9.0dB */
	{0x12, 0x12, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},	/* 19, -9.5dB */
	{0x11, 0x11, 0x0f, 0x09, 0x00, 0x00, 0x00, 0x00},	/* 20, -10.0dB */
	{0x10, 0x10, 0x0e, 0x08, 0x00, 0x00, 0x00, 0x00},	/* 21, -10.5dB */
	{0x0f, 0x0f, 0x0d, 0x08, 0x00, 0x00, 0x00, 0x00},	/* 22, -11.0dB */
	{0x0e, 0x0e, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00},	/* 23, -11.5dB */
	{0x0d, 0x0d, 0x0c, 0x07, 0x00, 0x00, 0x00, 0x00},	/* 24, -12.0dB */
	{0x0d, 0x0c, 0x0b, 0x06, 0x00, 0x00, 0x00, 0x00},	/* 25, -12.5dB */
	{0x0c, 0x0c, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00},	/* 26, -13.0dB */
	{0x0b, 0x0b, 0x0a, 0x06, 0x00, 0x00, 0x00, 0x00},	/* 27, -13.5dB */
	{0x0b, 0x0a, 0x09, 0x05, 0x00, 0x00, 0x00, 0x00},	/* 28, -14.0dB */
	{0x0a, 0x0a, 0x09, 0x05, 0x00, 0x00, 0x00, 0x00},	/* 29, -14.5dB */
	{0x0a, 0x09, 0x08, 0x05, 0x00, 0x00, 0x00, 0x00},	/* 30, -15.0dB */
	{0x09, 0x09, 0x08, 0x05, 0x00, 0x00, 0x00, 0x00},	/* 31, -15.5dB */
	{0x09, 0x08, 0x07, 0x04, 0x00, 0x00, 0x00, 0x00}	/* 32, -16.0dB */
};

void rtl92d_dm_diginit(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);

	rtl_dm_dig->dig_enable_flag = true;
	rtl_dm_dig->dig_ext_port_stage = DIG_EXT_PORT_STAGE_MAX;
	rtl_dm_dig->cur_igvalue = 0x20;
	rtl_dm_dig->pre_igvalue = 0x0;
	rtl_dm_dig->cursta_connectstate = DIG_STA_DISCONNECT;
	rtl_dm_dig->presta_connectstate = DIG_STA_DISCONNECT;
	rtl_dm_dig->curmultista_connectstate = DIG_MULTISTA_DISCONNECT;
	rtl_dm_dig->rssi_lowthresh = DM_DIG_THRESH_LOW;
	rtl_dm_dig->rssi_highthresh = DM_DIG_THRESH_HIGH;
	rtl_dm_dig->fa_lowthresh = DM_FALSEALARM_THRESH_LOW;
	rtl_dm_dig->fa_highthresh = DM_FALSEALARM_THRESH_HIGH;
	rtl_dm_dig->rx_gain_range_max = DM_DIG_FA_UPPER;
	rtl_dm_dig->rx_gain_range_min = DM_DIG_FA_LOWER;
	rtl_dm_dig->backoff_val = DM_DIG_BACKOFF_DEFAULT;
	rtl_dm_dig->backoff_val_range_max = DM_DIG_BACKOFF_MAX;
	rtl_dm_dig->backoff_val_range_min = DM_DIG_BACKOFF_MIN;
	rtl_dm_dig->pre_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
	rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_MAX;
	rtl_dm_dig->large_fa_hit = 0;
	rtl_dm_dig->recover_cnt = 0;
	rtl_dm_dig->forbidden_igi = DM_DIG_FA_LOWER;
}

bool _rtl92d_dualmac_getparameter_from_buddy(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;

	if (rtlpriv->rtlhal.macphymode != DUALMAC_SINGLEPHY)
		return false;

	if (buddy_priv == NULL)
		return false;

	if (rtlpriv->rtlhal.bslave_of_dmsp)
		return false;

	if ((buddy_priv->mac80211.link_state == MAC80211_LINKED) &&
		(rtlpriv->mac80211.link_state != MAC80211_LINKED))
		return true;
	else
		return false;
}

void rtl92d_dm_false_alarm_counter_statistics(struct ieee80211_hw *hw)
{
	u32 ret_value;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct false_alarm_statistics *falsealm_cnt = &(rtlpriv->falsealm_cnt);
	unsigned long flag;

	/* hold ofdm counter */
	rtl_set_bbreg(hw, ROFDM0_LSTF, BIT(31), 1);	/* hold page C counter */
	rtl_set_bbreg(hw, ROFDM1_LSTF, BIT(31), 1);	/*hold page D counter */

	ret_value = rtl_get_bbreg(hw, ROFDM0_FRAMESYNC, BMASKDWORD);
	falsealm_cnt->cnt_fast_fsync_fail = (ret_value & 0xffff);
	falsealm_cnt->cnt_sb_search_fail = ((ret_value & 0xffff0000) >> 16);
	ret_value = rtl_get_bbreg(hw, ROFDM_PHYCOUNTER1, BMASKDWORD);
	falsealm_cnt->cnt_parity_fail = ((ret_value & 0xffff0000) >> 16);
	ret_value = rtl_get_bbreg(hw, ROFDM_PHYCOUNTER2, BMASKDWORD);
	falsealm_cnt->cnt_rate_illegal = (ret_value & 0xffff);
	falsealm_cnt->cnt_crc8_fail = ((ret_value & 0xffff0000) >> 16);
	ret_value = rtl_get_bbreg(hw, ROFDM_PHYCOUNTER3, BMASKDWORD);
	falsealm_cnt->cnt_mcs_fail = (ret_value & 0xffff);
	falsealm_cnt->cnt_ofdm_fail =
	    falsealm_cnt->cnt_parity_fail +
	    falsealm_cnt->cnt_rate_illegal +
	    falsealm_cnt->cnt_crc8_fail +
	    falsealm_cnt->cnt_mcs_fail +
	    falsealm_cnt->cnt_fast_fsync_fail +
	    falsealm_cnt->cnt_sb_search_fail;

	if (rtlpriv->rtlhal.current_bandtype != BAND_ON_5G) {
		/* hold cck counter */
		rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
		ret_value = rtl_get_bbreg(hw, RCCK0_FACOUNTERLOWER, BMASKBYTE0);
		falsealm_cnt->cnt_cck_fail = ret_value;
		ret_value = rtl_get_bbreg(hw, RCCK0_FACOUNTERUPPER, BMASKBYTE3);
		falsealm_cnt->cnt_cck_fail += (ret_value & 0xff) << 8;
		rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
	} else {
		falsealm_cnt->cnt_cck_fail = 0;
	}

	falsealm_cnt->cnt_all = (falsealm_cnt->cnt_fast_fsync_fail +
				 falsealm_cnt->cnt_sb_search_fail +
				 falsealm_cnt->cnt_parity_fail +
				 falsealm_cnt->cnt_rate_illegal +
				 falsealm_cnt->cnt_crc8_fail +
				 falsealm_cnt->cnt_mcs_fail +
				 falsealm_cnt->cnt_cck_fail);

	/* reset false alarm counter registers */
	rtl_set_bbreg(hw, ROFDM1_LSTF, 0x08000000, 1);
	rtl_set_bbreg(hw, ROFDM1_LSTF, 0x08000000, 0);
	/* update ofdm counter */
	rtl_set_bbreg(hw, ROFDM0_LSTF, BIT(31), 0);	/* update page C counter */
	rtl_set_bbreg(hw, ROFDM1_LSTF, BIT(31), 0);	/* update page D counter */
	if (rtlpriv->rtlhal.current_bandtype != BAND_ON_5G) {
		/* reset cck counter */
		rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
		rtl_set_bbreg(hw, RCCK0_FALSEALARMREPORT, 0x0000c000, 0);
		/* enable cck counter */
		rtl_set_bbreg(hw, RCCK0_FALSEALARMREPORT, 0x0000c000, 2);
		rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
	}
	RT_TRACE(COMP_DIG, DBG_LOUD, ("Cnt_Fast_Fsync_fail = %x, "
			"Cnt_SB_Search_fail = %x\n",
			falsealm_cnt->cnt_fast_fsync_fail,
			falsealm_cnt->cnt_sb_search_fail));
	RT_TRACE(COMP_DIG, DBG_LOUD, ("Cnt_Parity_Fail = %x, "
			"Cnt_Rate_Illegal = %x, Cnt_Crc8_fail = %x, Cnt_Mcs_fail = %x\n",
			falsealm_cnt->cnt_parity_fail, falsealm_cnt->cnt_rate_illegal,
			falsealm_cnt->cnt_crc8_fail, falsealm_cnt->cnt_mcs_fail));
	RT_TRACE(COMP_DIG, DBG_LOUD,
			("Cnt_Ofdm_fail = %x, " "Cnt_Cck_fail = %x, Cnt_all = %x\n",
			falsealm_cnt->cnt_ofdm_fail, falsealm_cnt->cnt_cck_fail,
			falsealm_cnt->cnt_all));
}

void rtl92d_dm_false_alarm_counter_statistics_for_slaveofdmsp(
	struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	struct false_alarm_statistics *falsealm_cnt = &(rtlpriv->falsealm_cnt);
	struct false_alarm_statistics *buddy_flasealm_cnt;

	if (buddy_priv == NULL)
		return;

	buddy_flasealm_cnt = &(buddy_priv->falsealm_cnt);

	falsealm_cnt->cnt_fast_fsync_fail =buddy_flasealm_cnt->cnt_fast_fsync_fail;
	falsealm_cnt->cnt_sb_search_fail =buddy_flasealm_cnt->cnt_sb_search_fail;
    	falsealm_cnt->cnt_parity_fail = buddy_flasealm_cnt->cnt_parity_fail;
	falsealm_cnt->cnt_rate_illegal = buddy_flasealm_cnt->cnt_rate_illegal;
	falsealm_cnt->cnt_crc8_fail = buddy_flasealm_cnt->cnt_crc8_fail;
	falsealm_cnt->cnt_mcs_fail = buddy_flasealm_cnt->cnt_mcs_fail;

	falsealm_cnt->cnt_ofdm_fail =
		falsealm_cnt->cnt_parity_fail +
		falsealm_cnt->cnt_rate_illegal +
		falsealm_cnt->cnt_crc8_fail +
		falsealm_cnt->cnt_mcs_fail +
		falsealm_cnt->cnt_fast_fsync_fail +
		falsealm_cnt->cnt_sb_search_fail;


	/*hold cck counter */
	falsealm_cnt->cnt_cck_fail = buddy_flasealm_cnt->cnt_cck_fail;

	falsealm_cnt->cnt_all = (falsealm_cnt->cnt_fast_fsync_fail +
						falsealm_cnt->cnt_sb_search_fail +
						falsealm_cnt->cnt_parity_fail +
						falsealm_cnt->cnt_rate_illegal +
						falsealm_cnt->cnt_crc8_fail +
						falsealm_cnt->cnt_mcs_fail +
						falsealm_cnt->cnt_cck_fail);


	RT_TRACE(COMP_DIG, DBG_LOUD,
		("cnt_fast_fsync_fail = %x, cnt_sb_search_fail = %x\n",
		falsealm_cnt->cnt_fast_fsync_fail, falsealm_cnt->cnt_sb_search_fail));
	RT_TRACE(COMP_DIG, DBG_LOUD,
		("cnt_parity_fail = %x, cnt_rate_illegal = %x,"
		"cnt_crc8_fail = %x, Cnt_Mcs_fail = %x\n",
		falsealm_cnt->cnt_parity_fail, falsealm_cnt->cnt_rate_illegal,
		falsealm_cnt->cnt_crc8_fail, falsealm_cnt->cnt_mcs_fail));
	RT_TRACE(COMP_DIG, DBG_LOUD,
		("cnt_ofdm_fail = %x, cnt_cck_fail = %x, cnt_all = %x\n",
		falsealm_cnt->cnt_ofdm_fail, falsealm_cnt->cnt_cck_fail,
		falsealm_cnt->cnt_all));
}


void rtl92d_dm_find_minimum_rssi(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);
	struct rtl_mac *mac = rtl_mac(rtlpriv);
	long rssi_val_min_back_for_mac0 = 0;
	bool b_getvalue_from_buddy = _rtl92d_dualmac_getparameter_from_buddy(hw);
	bool brestore_rssi = false;
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;

	/* Determine the minimum RSSI  */
	if ((mac->link_state < MAC80211_LINKED) &&
	    (rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb == 0)) {
		rtl_dm_dig->min_undecorated_pwdb_for_dm = 0;
		RT_TRACE(COMP_BB_POWERSAVING, DBG_LOUD,
			 ("Not connected to any \n"));
	}
	if (mac->link_state >= MAC80211_LINKED) {
		if (mac->opmode == NL80211_IFTYPE_AP ||
			mac->opmode == NL80211_IFTYPE_ADHOC) {
			rtl_dm_dig->min_undecorated_pwdb_for_dm =
			    rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;
			RT_TRACE(COMP_BB_POWERSAVING, DBG_LOUD,
				 ("AP Client PWDB = 0x%lx \n",
				  rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb));
		} else {
			rtl_dm_dig->min_undecorated_pwdb_for_dm =
			    rtlpriv->dm.undecorated_smoothed_pwdb;
			RT_TRACE(COMP_BB_POWERSAVING, DBG_LOUD,
				 ("STA Default Port PWDB = 0x%x \n",
				  rtl_dm_dig->min_undecorated_pwdb_for_dm));
		}
	} else {
		rtl_dm_dig->min_undecorated_pwdb_for_dm =
		    rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;
		RT_TRACE(COMP_BB_POWERSAVING, DBG_LOUD,
			 ("AP Ext Port or disconnet PWDB = 0x%x \n",
			  rtl_dm_dig->min_undecorated_pwdb_for_dm));
	}

	if (rtlpriv->dm.supp_phymode_switch) {
		if (rtlpriv->rtlhal.macphymode == DUALMAC_SINGLEPHY) {
			if (buddy_priv) {
				if (rtlpriv->rtlhal.bslave_of_dmsp) {
					RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
						("Slave case of dmsp\n"));
					buddy_priv->dmsp_ctl.rssivalmin_for_anothermacofdmsp =
						rtl_dm_dig->min_undecorated_pwdb_for_dm;
				} else {
					if (b_getvalue_from_buddy) {
						RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
							("get new RSSI\n"));
						brestore_rssi = true;
						rssi_val_min_back_for_mac0 =
							rtl_dm_dig->min_undecorated_pwdb_for_dm;
						rtl_dm_dig->min_undecorated_pwdb_for_dm =
							rtlpriv->dmsp_ctl.rssivalmin_for_anothermacofdmsp;
					}
					/*dm_1P_CCA()*/
					if (brestore_rssi) {
						brestore_rssi = false;
						rtl_dm_dig->min_undecorated_pwdb_for_dm =
							rssi_val_min_back_for_mac0;
					}
				}
			}

		}
	}
	RT_TRACE(COMP_DIG, DBG_LOUD, ("MinUndecoratedPWDBForDM =%d\n",
			rtl_dm_dig->min_undecorated_pwdb_for_dm));
}

u8 _rtl92d_dm_initial_gain_minpwdb(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	long rssi_val_min = 0;
	struct rtl_dig *digtable = &(rtlpriv->dm.dm_digtable);

	digtable->curmultista_connectstate = MULTISTA_CONNECT(rtlpriv);
	if ((digtable->curmultista_connectstate == DIG_MULTISTA_CONNECT) &&
		(digtable->cursta_connectstate == DIG_STA_CONNECT)) {
		if(rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb != 0) {
			rssi_val_min = (rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb >
				rtlpriv->dm.undecorated_smoothed_pwdb)?
				rtlpriv->dm.undecorated_smoothed_pwdb:
				rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;
		} else {
			rssi_val_min = rtlpriv->dm.undecorated_smoothed_pwdb;
		}
	} else if (digtable->cursta_connectstate == DIG_STA_CONNECT ||
			digtable->cursta_connectstate == DIG_STA_BEFORE_CONNECT) {
		rssi_val_min = rtlpriv->dm.undecorated_smoothed_pwdb;
	} else if(digtable->curmultista_connectstate == DIG_MULTISTA_CONNECT) {
		rssi_val_min = rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;
	}

	return (u8)rssi_val_min;
}

void rtl92d_dm_cck_packet_detection_thresh_dmsp(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	bool bgetvalue_from_buddy = _rtl92d_dualmac_getparameter_from_buddy(hw);
	unsigned long flag = 0;

	if (rtl_dm_dig->cursta_connectstate == DIG_STA_CONNECT) {
		rtl_dm_dig->rssi_val_min =
			_rtl92d_dm_initial_gain_minpwdb(hw);
		if (rtl_dm_dig->pre_cck_pd_state == CCK_PD_STAGE_LOWRSSI) {
			if (rtl_dm_dig->rssi_val_min <= 25)
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
			else
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_HIGHRSSI;
		} else {
			if (rtl_dm_dig->rssi_val_min <= 20)
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
			else
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_HIGHRSSI;
		}
	} else {
		rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_MAX;
	}

	if (bgetvalue_from_buddy) {
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
			("mac 1 connect,mac 0 disconnect case  \n"));
		if (rtlpriv->dmsp_ctl.bchangecckpdstate_for_anothermacofdmsp) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_EMERG,
				("mac 0 set for mac1 \n"));
			if (rtlpriv->dmsp_ctl.curcckpdstate_for_anothermacofdmsp ==
				CCK_PD_STAGE_LOWRSSI) {
				rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
				rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0x83);
				rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
			} else {
				rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
				rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0xcd);
				rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
			}
			rtlpriv->dmsp_ctl.bchangecckpdstate_for_anothermacofdmsp = false;
		}
	}

	if (rtl_dm_dig->pre_cck_pd_state != rtl_dm_dig->cur_cck_pd_state) {

		if (NULL == buddy_priv) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("buddy == NULL case \n"));
			if (rtlpriv->rtlhal.bslave_of_dmsp) {
				rtl_dm_dig->pre_cck_pd_state = rtl_dm_dig->cur_cck_pd_state;
			} else {
				if (rtl_dm_dig->cur_cck_pd_state == CCK_PD_STAGE_LOWRSSI) {
					rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
					rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0x83);
					rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
				} else {
					rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
					rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0xcd);
					rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
				}
				rtl_dm_dig->pre_cck_pd_state = rtl_dm_dig->cur_cck_pd_state;
			}
			return;
		}

		if (rtlpriv->rtlhal.bslave_of_dmsp) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("bslave case \n"));
			buddy_priv->dmsp_ctl.bchangecckpdstate_for_anothermacofdmsp = true;
			buddy_priv->dmsp_ctl.curcckpdstate_for_anothermacofdmsp =
				rtl_dm_dig->cur_cck_pd_state;
		} else {
			if (!bgetvalue_from_buddy) {
				RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
					("mac 0 set for mac0 \n"));
				if (rtl_dm_dig->cur_cck_pd_state == CCK_PD_STAGE_LOWRSSI) {
					rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
					rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0x83);
					rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
				} else {
					rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
					rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0xcd);
					rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
				}
			}
		}
		rtl_dm_dig->pre_cck_pd_state = rtl_dm_dig->cur_cck_pd_state;
	}
	RT_TRACE(COMP_DIG, DBG_LOUD,
		("CCKPDStage=%x\n",rtl_dm_dig->cur_cck_pd_state));
}

void rtl92d_dm_cck_packet_detection_thresh(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);
	unsigned long flag;

	if (rtlpriv->dm.supp_phymode_switch) {
		if (rtlpriv->rtlhal.macphymode == DUALMAC_SINGLEPHY) {
			rtl92d_dm_cck_packet_detection_thresh_dmsp(hw);
			return;
		}
	}

	if (rtl_dm_dig->cursta_connectstate == DIG_STA_CONNECT) {
		if (rtl_dm_dig->pre_cck_pd_state == CCK_PD_STAGE_LOWRSSI) {
			if (rtl_dm_dig->min_undecorated_pwdb_for_dm <= 25)
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
			else
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_HIGHRSSI;
		} else {
			if (rtl_dm_dig->min_undecorated_pwdb_for_dm <= 20)
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
			else
				rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_HIGHRSSI;
		}
	} else {
		rtl_dm_dig->cur_cck_pd_state = CCK_PD_STAGE_LOWRSSI;
	}
	if (rtl_dm_dig->pre_cck_pd_state != rtl_dm_dig->cur_cck_pd_state) {
		if (rtl_dm_dig->cur_cck_pd_state == CCK_PD_STAGE_LOWRSSI) {
			rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
			rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0x83);
			rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
		} else {
			rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
			rtl_set_bbreg(hw, RCCK0_CCA, BMASKBYTE2, 0xcd);
			rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
		}
		rtl_dm_dig->pre_cck_pd_state = rtl_dm_dig->cur_cck_pd_state;
	}
	RT_TRACE(COMP_DIG, DBG_LOUD, ("CurSTAConnectState=%s\n",
			(rtl_dm_dig->cursta_connectstate == DIG_STA_CONNECT ?
			"DIG_STA_CONNECT " : "DIG_STA_DISCONNECT")));
	RT_TRACE(COMP_DIG, DBG_LOUD, ("CCKPDStage=%s\n",
			(rtl_dm_dig->cur_cck_pd_state == CCK_PD_STAGE_LOWRSSI ?
			"Low RSSI " : "High RSSI ")));
	RT_TRACE(COMP_DIG, DBG_LOUD, ("is92d single phy =%x\n",
			IS_92D_SINGLEPHY(rtlpriv->rtlhal.version)));

}

void rtl92d_dm_write_dig_dmsp(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	bool bgetvalue_from_othermac = _rtl92d_dualmac_getparameter_from_buddy(hw);

	if (buddy_priv == NULL) {
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
			("not find buddy\n"));
		if (rtlpriv->rtlhal.bmaster_of_dmsp) {
			rtl_set_bbreg(hw, ROFDM0_XAAGCCORE1, 0x7f,
				rtl_dm_dig->cur_igvalue);
			rtl_set_bbreg(hw, ROFDM0_XBAGCCORE1, 0x7f,
				rtl_dm_dig->cur_igvalue);
			rtl_dm_dig->pre_igvalue = rtl_dm_dig->cur_igvalue;
		} else {
			rtl_dm_dig->pre_igvalue = rtl_dm_dig->cur_igvalue;
		}
		return;
	}

	RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
		(("bgetvalue_from_othermac %d \n"),bgetvalue_from_othermac));

	if (bgetvalue_from_othermac) {
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
			("mac 0 set mac 1 value \n"));
		if (rtlpriv->dmsp_ctl.bwritedig_for_anothermacofdmsp) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_EMERG,
				("mac 0 set mac 1 value change value\n"));
			rtlpriv->dmsp_ctl.bwritedig_for_anothermacofdmsp = false;
			rtl_set_bbreg(hw, ROFDM0_XAAGCCORE1, 0x7f,
				rtlpriv->dmsp_ctl.curdigvalue_for_anothermacofdmsp);
			rtl_set_bbreg(hw, ROFDM0_XBAGCCORE1, 0x7f,
				rtlpriv->dmsp_ctl.curdigvalue_for_anothermacofdmsp);
		}
	}

	if (rtl_dm_dig->pre_igvalue != rtl_dm_dig->cur_igvalue) {
		/* Set initial gain.
		Set only BIT0~BIT6 for DIG. BIT7 is for Antenna diversity.
		Just not to modified it for SD3 testing.
		*/
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
			("need to change initial gain\n"));
		 if (rtlpriv->rtlhal.bslave_of_dmsp) {
		 	RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("slave case \n"));
		 	buddy_priv->dmsp_ctl.bwritedig_for_anothermacofdmsp = true;
			buddy_priv->dmsp_ctl.curdigvalue_for_anothermacofdmsp =
				rtl_dm_dig->cur_igvalue;
		} else {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("master case \n"));
			if (!bgetvalue_from_othermac) {
				RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
					("mac 0 set mac 0 value \n"));
				rtl_set_bbreg(hw, ROFDM0_XAAGCCORE1, 0x7f,
					rtl_dm_dig->cur_igvalue);
				rtl_set_bbreg(hw, ROFDM0_XBAGCCORE1, 0x7f,
					rtl_dm_dig->cur_igvalue);
			}
		}
		rtl_dm_dig->pre_igvalue = rtl_dm_dig->cur_igvalue;
	}
}

void rtl92d_dm_write_dig(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);

	if (rtlpriv->dm.supp_phymode_switch) {
		if (rtlpriv->rtlhal.macphymode == DUALMAC_SINGLEPHY) {
			rtl92d_dm_write_dig_dmsp(hw);
			return;
		}
	}
	RT_TRACE(COMP_DIG, DBG_LOUD, ("cur_igvalue = 0x%x, "
		  "pre_igvalue = 0x%x, backoff_val = %d\n",
		  rtl_dm_dig->cur_igvalue, rtl_dm_dig->pre_igvalue,
		  rtl_dm_dig->backoff_val));
	if (rtl_dm_dig->dig_enable_flag == false) {
		RT_TRACE(COMP_DIG, DBG_LOUD, ("DIG is disabled\n"));
		rtl_dm_dig->pre_igvalue = 0x17;
		return;
	}
	if (rtl_dm_dig->pre_igvalue != rtl_dm_dig->cur_igvalue) {
		rtl_set_bbreg(hw, ROFDM0_XAAGCCORE1, 0x7f,
			      rtl_dm_dig->cur_igvalue);
		rtl_set_bbreg(hw, ROFDM0_XBAGCCORE1, 0x7f,
			      rtl_dm_dig->cur_igvalue);
		rtl_dm_dig->pre_igvalue = rtl_dm_dig->cur_igvalue;
	}
}

void rtl92d_dm_dig(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_dig *rtl_dm_dig = &(rtlpriv->dm.dm_digtable);
	u8 value_igi = rtl_dm_dig->cur_igvalue;
	struct false_alarm_statistics *falsealm_cnt = &(rtlpriv->falsealm_cnt);

	RT_TRACE(COMP_DIG, DBG_LOUD, (" \n"));
	if (rtlpriv->rtlhal.b_earlymode_enable) {
		if ((rtlpriv->mac80211.link_state >= MAC80211_LINKED) &&
		    (rtlpriv->mac80211.vendor == PEER_CISCO)) {
			RT_TRACE(COMP_DIG, DBG_LOUD, ("IOT_PEER = CISCO \n"));
			if (rtl_dm_dig->last_min_undecorated_pwdb_for_dm >= 50 &&
				rtl_dm_dig->min_undecorated_pwdb_for_dm < 50) {
				rtl_write_byte(rtlpriv, REG_EARLY_MODE_CONTROL, 0x00);
				RT_TRACE(COMP_DIG, DBG_LOUD, ("Early Mode Off \n"));
			} else if (rtl_dm_dig->last_min_undecorated_pwdb_for_dm <= 55 &&
				rtl_dm_dig->min_undecorated_pwdb_for_dm > 55) {
				rtl_write_byte(rtlpriv, REG_EARLY_MODE_CONTROL, 0x0f);
				RT_TRACE(COMP_DIG, DBG_LOUD, ("Early Mode On \n"));
			}
		} else if (!(rtl_read_byte(rtlpriv, REG_EARLY_MODE_CONTROL) & 0xf)) {
			rtl_write_byte(rtlpriv, REG_EARLY_MODE_CONTROL, 0x0f);
			RT_TRACE(COMP_DIG, DBG_LOUD, ("Early Mode On \n"));
		}
		rtl_dm_dig->last_min_undecorated_pwdb_for_dm = rtl_dm_dig->min_undecorated_pwdb_for_dm;
	}
	if (rtlpriv->dm.b_dm_initialgain_enable == false)
		return;

	/* because we will send data pkt when scanning
	 * this will cause some ap like gear-3700 wep TP
	 * lower if we retrun here, this is the diff of
	 * mac80211 driver vs ieee80211 driver */
	/* if (rtlpriv->mac80211.act_scanning)
	 *      return; */

	RT_TRACE(COMP_DIG, DBG_LOUD, ("progress \n"));
	/* Decide the current status and if modify initial gain or not */
	if (rtlpriv->mac80211.link_state >= MAC80211_LINKED)
		rtl_dm_dig->cursta_connectstate = DIG_STA_CONNECT;
	else
		rtl_dm_dig->cursta_connectstate = DIG_STA_DISCONNECT;

	if (rtlpriv->mac80211.opmode == NL80211_IFTYPE_AP ||
		rtlpriv->mac80211.opmode == NL80211_IFTYPE_ADHOC)
		rtl_dm_dig->cursta_connectstate = DIG_STA_DISCONNECT;

	/* adjust initial gain according to false alarm counter */
	if (falsealm_cnt->cnt_all < DM_DIG_FA_TH0)
		value_igi--;
	else if (falsealm_cnt->cnt_all < DM_DIG_FA_TH1)
		value_igi += 0;
	else if (falsealm_cnt->cnt_all < DM_DIG_FA_TH2)
		value_igi++;
	else if (falsealm_cnt->cnt_all >= DM_DIG_FA_TH2)
		value_igi += 2;
	RT_TRACE(COMP_DIG, DBG_LOUD,
		 ("dm_DIG() Before: large_fa_hit=%d, forbidden_igi=%x \n",
		  rtl_dm_dig->large_fa_hit, rtl_dm_dig->forbidden_igi));
	RT_TRACE(COMP_DIG, DBG_LOUD,
		 ("dm_DIG() Before: Recover_cnt=%d, rx_gain_range_min=%x \n",
		  rtl_dm_dig->recover_cnt, rtl_dm_dig->rx_gain_range_min));

	/* deal with abnorally large false alarm */
	if (falsealm_cnt->cnt_all > 10000) {
		RT_TRACE(COMP_DIG, DBG_LOUD,
			 ("dm_DIG(): Abnornally false alarm case. \n"));

		rtl_dm_dig->large_fa_hit++;
		if (rtl_dm_dig->forbidden_igi < rtl_dm_dig->cur_igvalue) {
			rtl_dm_dig->forbidden_igi = rtl_dm_dig->cur_igvalue;
			rtl_dm_dig->large_fa_hit = 1;
		}
		if (rtl_dm_dig->large_fa_hit >= 3) {
			if ((rtl_dm_dig->forbidden_igi + 1) > DM_DIG_MAX)
				rtl_dm_dig->rx_gain_range_min = DM_DIG_MAX;
			else
				rtl_dm_dig->rx_gain_range_min =
				    (rtl_dm_dig->forbidden_igi + 1);
			rtl_dm_dig->recover_cnt = 3600;	/* 3600=2hr */
		}
	} else {
		/* Recovery mechanism for IGI lower bound */
		if (rtl_dm_dig->recover_cnt != 0)
			rtl_dm_dig->recover_cnt--;
		else {
			if (rtl_dm_dig->large_fa_hit == 0) {
				if ((rtl_dm_dig->forbidden_igi - 1) <
				    DM_DIG_FA_LOWER) {
					rtl_dm_dig->forbidden_igi = DM_DIG_FA_LOWER;
					rtl_dm_dig->rx_gain_range_min = DM_DIG_FA_LOWER;

				} else {
					rtl_dm_dig->forbidden_igi--;
					rtl_dm_dig->rx_gain_range_min =
					    (rtl_dm_dig->forbidden_igi + 1);
				}
			} else if (rtl_dm_dig->large_fa_hit == 3) {
				rtl_dm_dig->large_fa_hit = 0;
			}
		}
	}
	RT_TRACE(COMP_DIG, DBG_LOUD,
		 ("dm_DIG() After: large_fa_hit=%d, forbidden_igi=%x \n",
		  rtl_dm_dig->large_fa_hit, rtl_dm_dig->forbidden_igi));
	RT_TRACE(COMP_DIG, DBG_LOUD,
		 ("dm_DIG() After: recover_cnt=%d, rx_gain_range_min=%x \n",
		  rtl_dm_dig->recover_cnt, rtl_dm_dig->rx_gain_range_min));

	if (value_igi > DM_DIG_MAX)
		value_igi = DM_DIG_MAX;
	else if (value_igi < rtl_dm_dig->rx_gain_range_min)
		value_igi = rtl_dm_dig->rx_gain_range_min;
	rtl_dm_dig->cur_igvalue = value_igi;
	rtl92d_dm_write_dig(hw);
	if (rtlpriv->rtlhal.current_bandtype != BAND_ON_5G)
		rtl92d_dm_cck_packet_detection_thresh(hw);

	RT_TRACE(COMP_DIG, DBG_LOUD, ("end\n"));
}

void rtl92d_dm_init_dynamic_txpower(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	rtlpriv->dm.bdynamic_txpower_enable = true;
	rtlpriv->dm.last_dtp_lvl = TXHIGHPWRLEVEL_NORMAL;
	rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
}

void rtl92d_dm_dynamic_txpower(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_phy *rtlphy = &(rtlpriv->phy);
	struct rtl_hal *rtlhal = rtl_hal(rtlpriv);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	long undecorated_smoothed_pwdb;
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	bool bgetvalue_from_buddy = _rtl92d_dualmac_getparameter_from_buddy(hw);
	u8 highpowerlvl_backformac0 = TXHIGHPWRLEVEL_LEVEL1;

	if ((!rtlpriv->dm.bdynamic_txpower_enable)
	    || rtlpriv->dm.dm_flag & HAL_DM_HIPWR_DISABLE) {
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
		return;
	}
	if (!rtlpriv->dm.supp_phymode_switch &&
	    (mac->link_state < MAC80211_LINKED) &&
	    (rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb == 0)) {
		RT_TRACE(COMP_POWER, DBG_TRACE, ("Not connected to any \n"));
		rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
		rtlpriv->dm.last_dtp_lvl = TXHIGHPWRLEVEL_NORMAL;
		return;
	}
	if (mac->link_state >= MAC80211_LINKED) {
		if (mac->opmode == NL80211_IFTYPE_ADHOC) {
			undecorated_smoothed_pwdb =
			    rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;
			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("IBSS Client PWDB = 0x%lx \n",
				  undecorated_smoothed_pwdb));
		} else {
			undecorated_smoothed_pwdb =
			    rtlpriv->dm.undecorated_smoothed_pwdb;
			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("STA Default Port PWDB = 0x%lx \n",
				  undecorated_smoothed_pwdb));
		}
	} else {
		undecorated_smoothed_pwdb =
		    rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb;

		RT_TRACE(COMP_POWER, DBG_LOUD,
			 ("AP Ext Port PWDB = 0x%lx \n",
			  undecorated_smoothed_pwdb));
	}
	if (rtlhal->current_bandtype == BAND_ON_5G) {
		if (undecorated_smoothed_pwdb >= 0x33) {
			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL2;
			RT_TRACE(COMP_HIPWR, DBG_LOUD,
				 ("5G:TxHighPwrLevel_Level2 (TxPwr=0x0)\n"));
		} else if ((undecorated_smoothed_pwdb < 0x33)
			   && (undecorated_smoothed_pwdb >= 0x2b)) {
			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL1;
			RT_TRACE(COMP_HIPWR, DBG_LOUD,
				 ("5G:TxHighPwrLevel_Level1 (TxPwr=0x10)\n"));
		} else if (undecorated_smoothed_pwdb < 0x2b) {
			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
			RT_TRACE(COMP_HIPWR, DBG_LOUD,
				 ("5G:TxHighPwrLevel_Normal\n"));
		}
	} else {
		if (undecorated_smoothed_pwdb >=
		    TX_POWER_NEAR_FIELD_THRESH_LVL2) {
			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL2;
			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x0)\n"));
		} else
		    if ((undecorated_smoothed_pwdb <
			 (TX_POWER_NEAR_FIELD_THRESH_LVL2 - 3))
			&& (undecorated_smoothed_pwdb >=
			    TX_POWER_NEAR_FIELD_THRESH_LVL1)) {

			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_LEVEL1;
			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("TXHIGHPWRLEVEL_LEVEL1 (TxPwr=0x10)\n"));
		} else if (undecorated_smoothed_pwdb <
			   (TX_POWER_NEAR_FIELD_THRESH_LVL1 - 5)) {
			rtlpriv->dm.dynamic_txhighpower_lvl = TXHIGHPWRLEVEL_NORMAL;
			RT_TRACE(COMP_POWER, DBG_LOUD,
				 ("TXHIGHPWRLEVEL_NORMAL\n"));
		}
	}

	if (rtlpriv->dm.supp_phymode_switch && bgetvalue_from_buddy) {
		RT_TRACE(COMP_POWER, DBG_LOUD,
			("mac 0 for mac 1 \n"));
		if (rtlpriv->dmsp_ctl.bchangetxhighpowerlvl_for_anothermacofdmsp) {
			RT_TRACE(COMP_POWER, DBG_EMERG,
				("change value \n"));
			highpowerlvl_backformac0 = rtlpriv->dm.dynamic_txhighpower_lvl;
			rtlpriv->dm.dynamic_txhighpower_lvl =
				rtlpriv->dmsp_ctl.curtxhighlvl_for_anothermacofdmsp;
			rtl92d_phy_set_txpower_level(hw, buddy_priv->phy.current_channel);
			rtlpriv->dm.dynamic_txhighpower_lvl = highpowerlvl_backformac0;
			rtlpriv->dmsp_ctl.bchangetxhighpowerlvl_for_anothermacofdmsp =
					false;
		}
	}
	if ((rtlpriv->dm.dynamic_txhighpower_lvl != rtlpriv->dm.last_dtp_lvl)) {
		RT_TRACE(COMP_POWER, DBG_LOUD,
			 ("Channel = %d \n", rtlphy->current_channel));
		if (rtlpriv->dm.supp_phymode_switch) {
			if (NULL == buddy_priv) {
				RT_TRACE(COMP_POWER, DBG_LOUD,
					("Buddy == NULL case \n"));
				if (!rtlpriv->rtlhal.bslave_of_dmsp)
					rtl92d_phy_set_txpower_level(hw, rtlphy->current_channel);
			} else {
				if (rtlpriv->rtlhal.macphymode == DUALMAC_SINGLEPHY) {
					RT_TRACE(COMP_POWER, DBG_LOUD,
						("Buddy != NULL DMSP \n"));
					if (rtlpriv->rtlhal.bslave_of_dmsp) {
						RT_TRACE(COMP_POWER, DBG_LOUD,
							("bslave case\n"));
						buddy_priv->dmsp_ctl.
							bchangetxhighpowerlvl_for_anothermacofdmsp = true;
						buddy_priv->dmsp_ctl.curtxhighlvl_for_anothermacofdmsp
							= rtlpriv->dm.dynamic_txhighpower_lvl;
					} else {
						RT_TRACE(COMP_POWER, DBG_LOUD,
							("master case  \n"));
						if (!bgetvalue_from_buddy) {
							RT_TRACE(COMP_POWER, DBG_LOUD,
								("mac 0 for mac 0 \n"));
							rtl92d_phy_set_txpower_level(hw, rtlphy->current_channel);
						}
					}
				} else {
					RT_TRACE(COMP_POWER, DBG_LOUD,
						("Buddy != NULL DMDP\n"));
					rtl92d_phy_set_txpower_level(hw, rtlphy->current_channel);
				}
			}
		} else {
			rtl92d_phy_set_txpower_level(hw, rtlphy->current_channel);
		}
	}
	rtlpriv->dm.last_dtp_lvl = rtlpriv->dm.dynamic_txhighpower_lvl;
}

void rtl92d_dm_pwdb_monitor(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_sta_info *drv_priv;
	long tmp_entry_max_pwdb = 0, tmp_entry_min_pwdb = 0xff;

	/* AP & ADHOC & MESH */
	spin_lock_bh(&rtlpriv->locks.entry_list_lock);
	list_for_each_entry(drv_priv, &rtlpriv->entry_list, list) {
		if(drv_priv->rssi_stat.undecorated_smoothed_pwdb < tmp_entry_min_pwdb)
			tmp_entry_min_pwdb = drv_priv->rssi_stat.undecorated_smoothed_pwdb;
		if(drv_priv->rssi_stat.undecorated_smoothed_pwdb > tmp_entry_max_pwdb)
			tmp_entry_max_pwdb = drv_priv->rssi_stat.undecorated_smoothed_pwdb;
	}
	spin_unlock_bh(&rtlpriv->locks.entry_list_lock);

	/* If associated entry is found */
	if(tmp_entry_max_pwdb != 0)	{
		rtlpriv->dm.entry_max_undecoratedsmoothed_pwdb = tmp_entry_max_pwdb;
		RTPRINT(rtlpriv, FDM, DM_PWDB, ("EntryMaxPWDB = 0x%lx(%ld)\n",
			tmp_entry_max_pwdb, tmp_entry_max_pwdb));
	} else {
		rtlpriv->dm.entry_max_undecoratedsmoothed_pwdb = 0;
	}
	/* If associated entry is found */
	if(tmp_entry_min_pwdb != 0xff) {
		rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb = tmp_entry_min_pwdb;
		RTPRINT(rtlpriv, FDM, DM_PWDB, ("EntryMinPWDB = 0x%lx(%ld)\n",
					tmp_entry_min_pwdb, tmp_entry_min_pwdb));
	} else {
		rtlpriv->dm.entry_min_undecoratedsmoothed_pwdb = 0;
	}

	/* Indicate Rx signal strength to FW. */
	if (rtlpriv->dm.b_useramask) {
		u32 temp = 0;
		temp = rtlpriv->dm.undecorated_smoothed_pwdb;
		temp = temp << 16;
		temp = temp | 0x100;
		/* fw v12 cmdid 5:use max macid ,for nic ,
		 * default macid is 0 ,max macid is 1 */
		rtl92d_fill_h2c_cmd(hw, H2C_RSSI_REPORT, 3, (u8 *) (&temp));
	} else {
		rtl_write_byte(rtlpriv, 0x4fe,
				(u8) rtlpriv->dm.undecorated_smoothed_pwdb);
	}
}

void rtl92d_dm_init_edca_turbo(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	rtlpriv->dm.bcurrent_turbo_edca = false;
	rtlpriv->dm.bis_any_nonbepkts = false;
	rtlpriv->dm.bis_cur_rdlstate = false;
}

void rtl92d_dm_interrupt_migration(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtlpriv);
	bool bcurrent_int_mt, bcurrent_ac_int_disable;
	bool int_mt_to_set = false;
	bool ac_int_to_set = false;

	/* Retrieve current interrupt migration
	 * and Tx four ACs IMR settings first. */
	rtlpriv->cfg->ops->get_hw_reg(hw, HW_VAR_INT_MIGRATION,
				      (u8 *) (&bcurrent_int_mt));
	rtlpriv->cfg->ops->get_hw_reg(hw, HW_VAR_INT_AC,
				      (u8 *) (&bcurrent_ac_int_disable));

	/* Currently we use busy traffic for reference
	 * instead of RxIntOK counts to prevent non-linear
	 * Rx statistics when interrupt migration is set before.*/
	if (mac->link_state >= MAC80211_LINKED &&
	    rtlpriv->link_info.b_higher_busytraffic) {
		int_mt_to_set = true;

		/* To check whether we should disable Tx interrupt or not. */
		if (rtlpriv->link_info.b_higher_busyrxtraffic)
			ac_int_to_set = true;
	}
	/* Update current settings. */
	if (bcurrent_int_mt != int_mt_to_set)
		rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_INT_MIGRATION,
				(u8 *) & int_mt_to_set);
	if (bcurrent_ac_int_disable != ac_int_to_set)
		rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_INT_AC,
				(u8 *) & ac_int_to_set);
}

void rtl92d_dm_check_edca_turbo(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));

	static u64 last_txok_cnt = 0;
	static u64 last_rxok_cnt = 0;
	u64 cur_txok_cnt;
	u64 cur_rxok_cnt;
	u32 edca_be_ul = 0x5ea42b;
	u32 edca_be_dl = 0x5ea42b;

	if (mac->link_state != MAC80211_LINKED) {
		rtlpriv->dm.bcurrent_turbo_edca = false;
		goto dm_checkedcaturbo_exit;
	}

	/* Enable BEQ TxOP limit configuration in wireless G-mode. */
	/* To check whether we shall force turn on TXOP configuration. */
	if ((!rtlpriv->dm.b_disable_framebursting) &&
		(rtlpriv->sec.pairwise_enc_algorithm == WEP40_ENCRYPTION ||
		rtlpriv->sec.pairwise_enc_algorithm == WEP104_ENCRYPTION ||
		rtlpriv->sec.pairwise_enc_algorithm == TKIP_ENCRYPTION)) {
		/* Force TxOP limit to 0x005e for UL. */
		if (!(edca_be_ul & 0xffff0000))
			edca_be_ul |= 0x005e0000;
		/* Force TxOP limit to 0x005e for DL. */
		if (!(edca_be_dl & 0xffff0000))
			edca_be_dl |= 0x005e0000;
	}

	if ((!rtlpriv->dm.bis_any_nonbepkts) &&
	    (!rtlpriv->dm.b_disable_framebursting)) {
		cur_txok_cnt = rtlpriv->stats.txbytesunicast - last_txok_cnt;
		cur_rxok_cnt = rtlpriv->stats.rxbytesunicast - last_rxok_cnt;
		if (cur_rxok_cnt > 4 * cur_txok_cnt) {
			if (!rtlpriv->dm.bis_cur_rdlstate ||
			    !rtlpriv->dm.bcurrent_turbo_edca) {
				rtl_write_dword(rtlpriv, REG_EDCA_BE_PARAM,	edca_be_dl);
				rtlpriv->dm.bis_cur_rdlstate = true;
			}
		} else {
			if (rtlpriv->dm.bis_cur_rdlstate ||
			    !rtlpriv->dm.bcurrent_turbo_edca) {
				rtl_write_dword(rtlpriv, REG_EDCA_BE_PARAM, edca_be_ul);
				rtlpriv->dm.bis_cur_rdlstate = false;
			}
		}
		rtlpriv->dm.bcurrent_turbo_edca = true;
	} else {
		if (rtlpriv->dm.bcurrent_turbo_edca) {
			u8 tmp = AC0_BE;
			rtlpriv->cfg->ops->set_hw_reg(hw, HW_VAR_AC_PARAM, (u8 *) (&tmp));
			rtlpriv->dm.bcurrent_turbo_edca = false;
		}
	}

dm_checkedcaturbo_exit:
	rtlpriv->dm.bis_any_nonbepkts = false;
	last_txok_cnt = rtlpriv->stats.txbytesunicast;
	last_rxok_cnt = rtlpriv->stats.rxbytesunicast;
}

static void rtl92d_dm_rxgain_tracking_thermalmeter(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u8 index_mapping[RX_INDEX_MAPPING_NUM] = {
		0x0f, 0x0f, 0x0d, 0x0c, 0x0b,
		0x0a, 0x09, 0x08, 0x07, 0x06,
		0x05, 0x04, 0x04, 0x03, 0x02
	};
	u8 rfpath;
	u32 u4tmp;

	u4tmp = (index_mapping[(rtlpriv->efuse.eeprom_thermalmeter -
				rtlpriv->dm.thermalvalue_rxgain)]) << 12;
	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
			(" Rx Gain %x\n", u4tmp));
	for (rfpath = RF90_PATH_A; rfpath < rtlpriv->phy.num_total_rfpath;
		rfpath++)
		rtl_set_rfreg(hw, rfpath, 0x3C, BRFREGOFFSETMASK,
			      (rtlpriv->phy.reg_rf3c[rfpath] & (~(0xF000))) | u4tmp);
}

static void rtl92d_dm_txpower_tracking_callback_thermalmeter(
			struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	struct rtl_phy *rtlphy = &(rtlpriv->phy);
	struct rtl_efuse *rtlefuse = rtl_efuse(rtl_priv(hw));
	u8 thermalvalue, delta, delta_lck, delta_iqk, delta_rxgain;
	u8 index, offset, thermalvalue_avg_count = 0;
	u32 thermalvalue_avg = 0;
	bool binteral_pa = false;
	long ele_a = 0, ele_d, temp_cck, val_x, value32;
	long val_y, ele_c = 0;
	unsigned long flag;
	char ofdm_index[2], cck_index = 0, ofdm_index_old[2], cck_index_old = 0;
	int i;
	bool is2t = IS_92D_SINGLEPHY(rtlhal->version);
	u8 ofdm_min_index = 6, ofdm_min_index_internal_pa = 3, rf;
	u8 indexforchannel =
	    rtl92d_get_rightchnlplace_for_iqk(rtlphy->current_channel);
	u8 index_mapping[5][INDEX_MAPPING_NUM] = {
		/* 5G, path A/MAC 0, decrease power  */
		{0, 1, 3, 6, 8, 9,	11, 13, 14, 16, 17, 18, 18},
		/* 5G, path A/MAC 0, increase power  */
		{0, 2, 4, 5, 7, 10,	12, 14, 16, 18, 18, 18, 18},
		/* 5G, path B/MAC 1, decrease power */
		{0, 2, 3, 6, 8, 9,	11, 13, 14, 16, 17, 18, 18},
		/* 5G, path B/MAC 1, increase power */
		{0, 2, 4, 5, 7, 10,	13, 16, 16, 18, 18, 18, 18},
		/* 2.4G, for decreas power */
		{0, 1, 2, 3, 4, 5,	6, 7, 7, 8, 9, 10, 10},
	};
	u8 index_mapping_internal_pa[8][INDEX_MAPPING_NUM] = {
		/* 5G, path A/MAC 0, ch36-64, decrease power  */
		{0, 1, 2, 4, 6, 7,	9, 11, 12, 14, 15, 16, 16},
		/* 5G, path A/MAC 0, ch36-64, increase power  */
		{0, 2, 4, 5, 7, 10,	12, 14, 16, 18, 18, 18, 18},
		/* 5G, path A/MAC 0, ch100-165, decrease power  */
		{0, 1, 2, 3, 5, 6,	8, 10, 11, 13, 14, 15, 15},
		/* 5G, path A/MAC 0, ch100-165, increase power  */
		{0, 2, 4, 5, 7, 10,	12, 14, 16, 18, 18, 18, 18},
		/* 5G, path B/MAC 1, ch36-64, decrease power */
		{0, 1, 2, 4, 6, 7,	9, 11, 12, 14, 15, 16, 16},
		/* 5G, path B/MAC 1, ch36-64, increase power */
		{0, 2, 4, 5, 7, 10,	13, 16, 16, 18, 18, 18, 18},
		/* 5G, path B/MAC 1, ch100-165, decrease power */
		{0, 1, 2, 3, 5, 6,	8, 9, 10, 12, 13, 14, 14},
		/* 5G, path B/MAC 1, ch100-165, increase power */
		{0, 2, 4, 5, 7, 10,	13, 16, 16, 18, 18, 18, 18},
	};

	rtlpriv->dm.btxpower_trackinginit = true;
	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("\n"));
	thermalvalue = (u8) rtl_get_rfreg(hw, RF90_PATH_A, RF_T_METER, 0xf800);
	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
		 ("Readback Thermal Meter = 0x%x pre thermal meter 0x%x "
		  "eeprom_thermalmeter 0x%x\n", thermalvalue,
		  rtlpriv->dm.thermalvalue, rtlefuse->eeprom_thermalmeter));
	rtl92d_phy_ap_calibrate(hw, (thermalvalue -
				     rtlefuse->eeprom_thermalmeter));
	if (is2t)
		rf = 2;
	else
		rf = 1;
	if (thermalvalue) {
		ele_d = rtl_get_bbreg(hw, ROFDM0_XATxIQIMBALANCE,
				      BMASKDWORD) & BMASKOFDM_D;
		for (i = 0; i < OFDM_TABLE_SIZE_92D; i++) {
			if (ele_d == (ofdmswing_table[i] & BMASKOFDM_D)) {
				ofdm_index_old[0] = (u8) i;

				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("Initial pathA ele_d reg0x%x = 0x%lx, "
					  "ofdm_index=0x%x\n", ROFDM0_XATxIQIMBALANCE,
					  ele_d, ofdm_index_old[0]));
				break;
			}
		}
		if (is2t) {
			ele_d = rtl_get_bbreg(hw, ROFDM0_XBTxIQIMBALANCE,
					      BMASKDWORD) & BMASKOFDM_D;
			for (i = 0; i < OFDM_TABLE_SIZE_92D; i++) {
				if (ele_d == (ofdmswing_table[i] & BMASKOFDM_D)) {
					ofdm_index_old[1] = (u8) i;
					RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
						 ("Initial pathB ele_d reg0x%x = "
						  "0x%lx, ofdm_index=0x%x\n",
						  ROFDM0_XBTxIQIMBALANCE, ele_d,
						  ofdm_index_old[1]));
					break;
				}
			}
		}
		if (rtlhal->current_bandtype == BAND_ON_2_4G) {
			/* Query CCK default setting From 0xa24 */
			rtl92d_acquire_cckandrw_pagea_ctl(hw, &flag);
			temp_cck = rtl_get_bbreg(hw, RCCK0_TXFILTER2,
					  BMASKDWORD) & BMASKCCK;
			rtl92d_release_cckandrw_pagea_ctl(hw, &flag);
			for (i = 0; i < CCK_TABLE_LENGTH; i++) {
				if (rtlpriv->dm.b_cck_inch14) {
					if (memcmp((void *)&temp_cck,
						   (void *)&cckswing_table_ch14[i][2], 4) == 0) {
						cck_index_old = (u8) i;
						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
							 ("Initial reg0x%x = 0x%lx, "
							  "cck_index=0x%x, ch 14 %d\n",
							  RCCK0_TXFILTER2,
							  temp_cck, cck_index_old,
							  rtlpriv->dm.b_cck_inch14));
						break;
					}
				} else {
					if (memcmp((void *)&temp_cck,
							(void *)&cckswing_table_ch1ch13[i][2], 4) == 0) {
						cck_index_old = (u8) i;
						RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
							 ("Initial reg0x%x = 0x%lx, "
							  "cck_index=0x%x, ch14 %d\n",
							  RCCK0_TXFILTER2,
							  temp_cck, cck_index_old,
							  rtlpriv->dm.b_cck_inch14));
						break;
					}
				}
			}
		} else {
			temp_cck = 0x090e1317;
			cck_index_old = 12;
		}

		if (!rtlpriv->dm.thermalvalue) {
			rtlpriv->dm.thermalvalue = rtlefuse->eeprom_thermalmeter;
			rtlpriv->dm.thermalvalue_lck = thermalvalue;
			rtlpriv->dm.thermalvalue_iqk = thermalvalue;
			rtlpriv->dm.thermalvalue_rxgain = rtlefuse->eeprom_thermalmeter;
			for (i = 0; i < rf; i++)
				rtlpriv->dm.ofdm_index[i] = ofdm_index_old[i];
			rtlpriv->dm.cck_index = cck_index_old;
		}
		if (rtlhal->reloadtxpowerindex) {
			for (i = 0; i < rf; i++)
				rtlpriv->dm.ofdm_index[i] = ofdm_index_old[i];
			rtlpriv->dm.cck_index = cck_index_old;
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
				 ("reload ofdm index for band switch\n"));
		}
		rtlpriv->dm.thermalvalue_avg[rtlpriv->dm.thermalvalue_avg_index] =
			    thermalvalue;
		rtlpriv->dm.thermalvalue_avg_index++;
		if (rtlpriv->dm.thermalvalue_avg_index == AVG_THERMAL_NUM)
			rtlpriv->dm.thermalvalue_avg_index = 0;
		for (i = 0; i < AVG_THERMAL_NUM; i++) {
			if (rtlpriv->dm.thermalvalue_avg[i]) {
				thermalvalue_avg += rtlpriv->dm.thermalvalue_avg[i];
				thermalvalue_avg_count++;
			}
		}
		if (thermalvalue_avg_count)
			thermalvalue = (u8) (thermalvalue_avg / thermalvalue_avg_count);
		if (rtlhal->reloadtxpowerindex) {
			delta = (thermalvalue > rtlefuse->eeprom_thermalmeter) ?
			    (thermalvalue - rtlefuse->eeprom_thermalmeter) :
			    (rtlefuse->eeprom_thermalmeter - thermalvalue);
			rtlhal->reloadtxpowerindex = false;
			rtlpriv->dm.bdone_txpower = false;
		} else if (rtlpriv->dm.bdone_txpower) {
			delta = (thermalvalue > rtlpriv->dm.thermalvalue) ?
			    (thermalvalue - rtlpriv->dm.thermalvalue) :
			    (rtlpriv->dm.thermalvalue - thermalvalue);
		} else {
			delta = (thermalvalue > rtlefuse->eeprom_thermalmeter) ?
			    (thermalvalue - rtlefuse->eeprom_thermalmeter) :
			    (rtlefuse->eeprom_thermalmeter - thermalvalue);
		}
		delta_lck = (thermalvalue > rtlpriv->dm.thermalvalue_lck) ?
		    (thermalvalue - rtlpriv->dm.thermalvalue_lck) :
		    (rtlpriv->dm.thermalvalue_lck - thermalvalue);
		delta_iqk = (thermalvalue > rtlpriv->dm.thermalvalue_iqk) ?
		    (thermalvalue - rtlpriv->dm.thermalvalue_iqk) :
		    (rtlpriv->dm.thermalvalue_iqk - thermalvalue);
		delta_rxgain = (thermalvalue >rtlpriv->dm.thermalvalue_rxgain) ?
			(thermalvalue - rtlpriv->dm.thermalvalue_rxgain) :
			(rtlpriv->dm.thermalvalue_rxgain - thermalvalue);
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
			 ("Readback Thermal Meter = 0x%x pre thermal meter 0x%x "
			  "eeprom_thermalmeter 0x%x delta 0x%x "
			  "delta_lck 0x%x delta_iqk 0x%x\n",
			  thermalvalue, rtlpriv->dm.thermalvalue,
			  rtlefuse->eeprom_thermalmeter, delta, delta_lck,
			  delta_iqk));
		if ((delta_lck > rtlefuse->delta_lck) && (rtlefuse->delta_lck != 0)) {
			rtlpriv->dm.thermalvalue_lck = thermalvalue;
			rtl92d_phy_lc_calibrate(hw);
		}
		if (delta > 0 && rtlpriv->dm.txpower_track_control) {
			rtlpriv->dm.bdone_txpower = true;
			delta = (thermalvalue > rtlefuse->eeprom_thermalmeter) ?
			    (thermalvalue - rtlefuse->eeprom_thermalmeter) :
			    (rtlefuse->eeprom_thermalmeter - thermalvalue);
			if (rtlhal->current_bandtype == BAND_ON_2_4G) {
				offset = 4;
				if (delta > INDEX_MAPPING_NUM - 1)
					index = index_mapping[offset][INDEX_MAPPING_NUM - 1];
				else
					index = index_mapping[offset][delta];
				if (thermalvalue > rtlpriv->dm.thermalvalue) {
					for (i = 0; i < rf; i++)
						ofdm_index[i] -= delta;
					cck_index -= delta;
				} else {
					for (i = 0; i < rf; i++)
						ofdm_index[i] += index;
					cck_index += index;
				}
			} else if (rtlhal->current_bandtype == BAND_ON_5G) {
				for (i = 0; i < rf; i++) {
					if (rtlhal->macphymode == DUALMAC_DUALPHY &&
						rtlhal->interfaceindex == 1)	/* MAC 1 5G */
						binteral_pa = rtlefuse->internal_pa_5g[1];
					else
						binteral_pa = rtlefuse->internal_pa_5g[i];
					if (binteral_pa) {
						if (rtlhal->interfaceindex == 1 || i == rf)
							offset = 4;
						else
							offset = 0;
						if (rtlphy->current_channel >= 100 &&
							rtlphy->current_channel <= 165)
							offset += 2;
					} else {
						if (rtlhal->interfaceindex == 1 || i == rf)
							offset = 2;
						else
							offset = 0;
					}
					if (thermalvalue > rtlefuse->eeprom_thermalmeter)
						offset++;
					if (binteral_pa) {
						if (delta > INDEX_MAPPING_NUM - 1)
							index = index_mapping_internal_pa[offset]
							    [INDEX_MAPPING_NUM - 1];
						else
							index = index_mapping_internal_pa[offset][delta];
					} else {
						if (delta > INDEX_MAPPING_NUM - 1)
							index = index_mapping[offset][INDEX_MAPPING_NUM - 1];
						else
							index = index_mapping[offset][delta];
					}
					if (thermalvalue > rtlefuse->eeprom_thermalmeter) {
						if (binteral_pa && thermalvalue > 0x12) {
							ofdm_index[i] = rtlpriv->dm.ofdm_index[i] -
							    ((delta / 2) * 3 + (delta % 2));
						} else {
							ofdm_index[i] -= index;
						}
					} else {
						ofdm_index[i] += index;
					}
				}
			}
			if (is2t) {
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("temp OFDM_A_index=0x%x, OFDM_B_index=0x%x,"
					  "cck_index=0x%x\n", rtlpriv->dm.ofdm_index[0],
					  rtlpriv->dm.ofdm_index[1], rtlpriv->dm.cck_index));
			} else {
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("temp OFDM_A_index=0x%x,cck_index=0x%x\n",
					  rtlpriv->dm.ofdm_index[0], rtlpriv->dm.cck_index));
			}
			for (i = 0; i < rf; i++) {
				if (ofdm_index[i] > OFDM_TABLE_SIZE_92D - 1)
					ofdm_index[i] = OFDM_TABLE_SIZE_92D - 1;
				else if (ofdm_index[i] < ofdm_min_index)
					ofdm_index[i] = ofdm_min_index;
			}
			if (rtlhal->current_bandtype == BAND_ON_2_4G) {
				if (cck_index > CCK_TABLE_SIZE - 1) {
					cck_index = CCK_TABLE_SIZE - 1;
				} else if (binteral_pa || rtlhal->current_bandtype ==
					   BAND_ON_2_4G) {
					if (ofdm_index[i] < ofdm_min_index_internal_pa)
						ofdm_index[i] = ofdm_min_index_internal_pa;
				} else if (cck_index < 0) {
					cck_index = 0;
				}
			}
			if (is2t) {
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("new OFDM_A_index=0x%x, OFDM_B_index=0x%x,"
					  "cck_index=0x%x\n", ofdm_index[0], ofdm_index[1],
					  cck_index));
			} else {
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("new OFDM_A_index=0x%x,cck_index=0x%x\n",
					  ofdm_index[0], cck_index));
			}
			ele_d = (ofdmswing_table[(u8) ofdm_index[0]] & 0xFFC00000) >> 22;
			val_x = rtlphy->iqk_matrix_regsetting[indexforchannel].value[0][0];
			val_y = rtlphy->iqk_matrix_regsetting[indexforchannel].value[0][1];
			if (val_x != 0) {
				if ((val_x & 0x00000200) != 0)
					val_x = val_x | 0xFFFFFC00;
				ele_a =
				    ((val_x * ele_d) >> 8) & 0x000003FF;

				/* new element C = element D x Y */
				if ((val_y & 0x00000200) != 0)
					val_y = val_y | 0xFFFFFC00;
				ele_c = ((val_y * ele_d) >> 8) & 0x000003FF;

				/* wirte new elements A, C, D to regC80 and
				 * regC94, element B is always 0 */
				value32 = (ele_d << 22) | ((ele_c & 0x3F) << 16) | ele_a;
				rtl_set_bbreg(hw, ROFDM0_XATxIQIMBALANCE, BMASKDWORD, value32);

				value32 = (ele_c & 0x000003C0) >> 6;
				rtl_set_bbreg(hw, ROFDM0_XCTxAFE, BMASKH4BITS, value32);

				value32 = ((val_x * ele_d) >> 7) & 0x01;
				rtl_set_bbreg(hw, ROFDM0_ECCATHRESHOLD, BIT(24), value32);

			} else {
				rtl_set_bbreg(hw, ROFDM0_XATxIQIMBALANCE, BMASKDWORD,
					      ofdmswing_table[(u8)ofdm_index[0]]);
				rtl_set_bbreg(hw, ROFDM0_XCTxAFE, BMASKH4BITS, 0x00);
				rtl_set_bbreg(hw, ROFDM0_ECCATHRESHOLD, BIT(24), 0x00);
			}

			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
				 ("TxPwrTracking for interface %d path A: X = 0x%lx, "
				 "Y = 0x%lx ele_A = 0x%lx ele_C = 0x%lx ele_D = 0x%lx "
				 "0xe94 = 0x%lx 0xe9c = 0x%lx\n", rtlhal->interfaceindex,
				 val_x, val_y, ele_a, ele_c, ele_d, val_x, val_y));

			if (rtlhal->current_bandtype == BAND_ON_2_4G) {
				/* Adjust CCK according to IQK result */
				if (!rtlpriv->dm.b_cck_inch14) {
					rtl_write_byte(rtlpriv, 0xa22, cckswing_table_ch1ch13
						       [(u8)cck_index][0]);
					rtl_write_byte(rtlpriv, 0xa23, cckswing_table_ch1ch13
						       [(u8)cck_index][1]);
					rtl_write_byte(rtlpriv, 0xa24, cckswing_table_ch1ch13
						       [(u8)cck_index][2]);
					rtl_write_byte(rtlpriv, 0xa25, cckswing_table_ch1ch13
						       [(u8)cck_index][3]);
					rtl_write_byte(rtlpriv, 0xa26, cckswing_table_ch1ch13
						       [(u8)cck_index][4]);
					rtl_write_byte(rtlpriv, 0xa27, cckswing_table_ch1ch13
						       [(u8)cck_index][5]);
					rtl_write_byte(rtlpriv, 0xa28, cckswing_table_ch1ch13
						       [(u8)cck_index][6]);
					rtl_write_byte(rtlpriv, 0xa29, cckswing_table_ch1ch13
						       [(u8)cck_index][7]);
				} else {
					rtl_write_byte(rtlpriv, 0xa22, cckswing_table_ch14
						       [(u8)cck_index][0]);
					rtl_write_byte(rtlpriv, 0xa23, cckswing_table_ch14
						       [(u8)cck_index][1]);
					rtl_write_byte(rtlpriv, 0xa24, cckswing_table_ch14
						       [(u8)cck_index][2]);
					rtl_write_byte(rtlpriv, 0xa25, cckswing_table_ch14
						       [(u8)cck_index][3]);
					rtl_write_byte(rtlpriv, 0xa26, cckswing_table_ch14
						       [(u8)cck_index][4]);
					rtl_write_byte(rtlpriv, 0xa27, cckswing_table_ch14
						       [(u8)cck_index][5]);
					rtl_write_byte(rtlpriv, 0xa28, cckswing_table_ch14
						       [(u8)cck_index][6]);
					rtl_write_byte(rtlpriv, 0xa29, cckswing_table_ch14
						       [(u8)cck_index][7]);
				}
			}
			if (is2t) {
				ele_d = (ofdmswing_table[(u8) ofdm_index[1]] &
						0xFFC00000) >> 22;
				val_x = rtlphy->iqk_matrix_regsetting[indexforchannel].value[
							0][4];
				val_y = rtlphy->iqk_matrix_regsetting[indexforchannel].value[
							0][5];
				if (val_x != 0) {
					if ((val_x & 0x00000200) != 0)	/* consider minus */
						val_x = val_x | 0xFFFFFC00;
					ele_a = ((val_x * ele_d) >> 8) & 0x000003FF;
					/* new element C = element D x Y */
					if ((val_y & 0x00000200) != 0)
						val_y =
						    val_y | 0xFFFFFC00;
					ele_c =
					    ((val_y *
					      ele_d) >> 8) & 0x00003FF;
					/* wirte new elements A, C, D to regC88 and regC9C, */
					/* element B is always 0 */
					value32 = (ele_d << 22) | ((ele_c & 0x3F) << 16) | ele_a;
					rtl_set_bbreg(hw, ROFDM0_XBTxIQIMBALANCE,
							BMASKDWORD, value32);
					value32 = (ele_c & 0x000003C0) >> 6;
					rtl_set_bbreg(hw, ROFDM0_XDTxAFE, BMASKH4BITS, value32);
					value32 = ((val_x * ele_d) >> 7) & 0x01;
					rtl_set_bbreg(hw, ROFDM0_ECCATHRESHOLD,
						      BIT(28), value32);
				} else {
					rtl_set_bbreg(hw, ROFDM0_XBTxIQIMBALANCE, BMASKDWORD,
						      ofdmswing_table[(u8) ofdm_index[1]]);
					rtl_set_bbreg(hw, ROFDM0_XDTxAFE, BMASKH4BITS, 0x00);
					rtl_set_bbreg(hw, ROFDM0_ECCATHRESHOLD, BIT(28), 0x00);
				}
				RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
					 ("TxPwrTracking path B: X = 0x%lx, Y = 0x%lx "
					 "ele_A = 0x%lx ele_C = 0x%lx ele_D = 0x%lx "
					 "0xeb4 = 0x%lx 0xebc = 0x%lx\n",
					  val_x, val_y, ele_a, ele_c,
					  ele_d, val_x, val_y));
			}
			RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
				 ("TxPwrTracking 0xc80 = 0x%x, 0xc94 = 0x%x RF 0x24 = 0x%x\n",
				  rtl_get_bbreg(hw, 0xc80, BMASKDWORD),
				  rtl_get_bbreg(hw, 0xc94, BMASKDWORD),
				  rtl_get_rfreg(hw, RF90_PATH_A, 0x24, BRFREGOFFSETMASK)));
		}
		if ((delta_iqk > rtlefuse->delta_iqk) && (rtlefuse->delta_iqk != 0)) {
			rtl92d_phy_reset_iqk_result(hw);
			rtlpriv->dm.thermalvalue_iqk = thermalvalue;
			rtl92d_phy_iq_calibrate(hw);
		}
		if (delta_rxgain > 0 && rtlhal->current_bandtype == BAND_ON_5G
		    && thermalvalue <= rtlefuse->eeprom_thermalmeter) {
			rtlpriv->dm.thermalvalue_rxgain = thermalvalue;
			rtl92d_dm_rxgain_tracking_thermalmeter(hw);
		}
		if (rtlpriv->dm.txpower_track_control)
			rtlpriv->dm.thermalvalue = thermalvalue;
	}

	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD, ("end\n"));
}

static void rtl92d_dm_initialize_txpower_tracking(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	rtlpriv->dm.btxpower_tracking = true;
	rtlpriv->dm.btxpower_trackinginit = false;
	rtlpriv->dm.txpower_track_control = true;
	RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
		 ("pMgntInfo->btxpower_tracking = %d\n",
		  rtlpriv->dm.btxpower_tracking));
}

void rtl92d_dm_check_txpower_tracking_thermal_meter(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	static u8 tm_trigger = 0;

	if (!rtlpriv->dm.btxpower_tracking)
		return;

	if (!tm_trigger) {
		rtl_set_rfreg(hw, RF90_PATH_A, RF_T_METER, BIT(17) | BIT(16), 0x03);
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
			 ("Trigger 92S Thermal Meter!!\n"));
		tm_trigger = 1;
		return;
	} else {
		RT_TRACE(COMP_POWER_TRACKING, DBG_LOUD,
			 ("Schedule TxPowerTracking direct call!!\n"));
		rtl92d_dm_txpower_tracking_callback_thermalmeter(hw);
		tm_trigger = 0;
	}
}

void rtl92c_dm_report_signalstrength(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	rtl_write_byte(rtlpriv, REG_DUMMY,
		       (u8) rtlpriv->dm.undecorated_smoothed_pwdb);
}

void rtl92d_dm_init_rate_adaptive_mask(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rate_adaptive *p_ra = &(rtlpriv->ra);

	p_ra->ratr_state = DM_RATR_STA_INIT;
	p_ra->pre_ratr_state = DM_RATR_STA_INIT;
	if (rtlpriv->dm.dm_type == DM_TYPE_BYDRIVER)
		rtlpriv->dm.b_useramask = true;
	else
		rtlpriv->dm.b_useramask = false;
}

void rtl92d_dm_refresh_rate_adaptive_mask(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rate_adaptive *p_ra = &(rtlpriv->ra);
	u32 low_rssithresh_for_ra, high_rssithresh_for_ra;
	struct ieee80211_sta *sta = NULL;

	if (is_hal_stop(rtlhal)) {
		RT_TRACE(COMP_RATE, DBG_LOUD,
			 (" driver is going to unload\n"));
		return;
	}
	if (!rtlpriv->dm.b_useramask) {
		RT_TRACE(COMP_RATE, DBG_LOUD,
			 (" driver does not control rate adaptive mask\n"));
		return;
	}
	if (mac->opmode != NL80211_IFTYPE_STATION)
		return;

	if (mac->link_state == MAC80211_LINKED) {
		switch (p_ra->pre_ratr_state) {
		case DM_RATR_STA_HIGH:
			high_rssithresh_for_ra = 50;
			low_rssithresh_for_ra = 20;
			break;
		case DM_RATR_STA_MIDDLE:
			high_rssithresh_for_ra = 55;
			low_rssithresh_for_ra = 20;
			break;
		case DM_RATR_STA_LOW:
			high_rssithresh_for_ra = 50;
			low_rssithresh_for_ra = 25;
			break;
		default:
			high_rssithresh_for_ra = 50;
			low_rssithresh_for_ra = 20;
			break;
		}
		if (rtlpriv->dm.undecorated_smoothed_pwdb >
		    (long)high_rssithresh_for_ra)
			p_ra->ratr_state = DM_RATR_STA_HIGH;
		else if (rtlpriv->dm.undecorated_smoothed_pwdb >
			 (long)low_rssithresh_for_ra)
			p_ra->ratr_state = DM_RATR_STA_MIDDLE;
		else
			p_ra->ratr_state = DM_RATR_STA_LOW;

		if (p_ra->pre_ratr_state != p_ra->ratr_state) {
			RT_TRACE(COMP_RATE, DBG_LOUD, ("RSSI = %ld\n",
					rtlpriv->dm.undecorated_smoothed_pwdb));
			RT_TRACE(COMP_RATE, DBG_LOUD, ("RSSI_LEVEL = %d\n",
					p_ra->ratr_state));
			RT_TRACE(COMP_RATE, DBG_LOUD,
					("PreState = %d, CurState = %d\n",
					p_ra->pre_ratr_state, p_ra->ratr_state));

			rcu_read_lock();
			sta = rtl_find_sta(hw, mac->bssid);
			if (sta)
			rtlpriv->cfg->ops->update_rate_tbl(hw, sta, p_ra->ratr_state);
			rcu_read_unlock();

			p_ra->pre_ratr_state = p_ra->ratr_state;
		}
	}
}

bool _rtl92d_dm_dual_mac_check_switch_to_dmsp(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtlpriv);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	struct ieee80211_hw *buddy_hw = NULL;
	bool b_need_to_change = true;

	if (buddy_priv == NULL) {
		/* SMSP */
		b_need_to_change = false;
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("buddy is NULL \n"));
		return b_need_to_change;
	} else {
		buddy_hw = buddy_priv->mac80211.hw;
		if (rtlhal->macphymode == DUALMAC_SINGLEPHY) {
			/* DMSP */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("DMSP \n"));
			return b_need_to_change;
		}

		if (rtlpriv->rtlhal.interfaceindex == 1) {
			/* MAC 1 */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("MAC 1 \n"));
			return b_need_to_change;
		}

		if ((rtlpriv->rtlhal.being_init_adapter) ||
			(buddy_priv->rtlhal.being_init_adapter)) {
			/* hw is not ready */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("Initialization is on going\n"));
			return b_need_to_change;
		}

		if ((!rtlpriv->rtlhal.b_bbrf_ready) ||
			(!buddy_priv->rtlhal.b_bbrf_ready)) {
			/* hw is not ready */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("bb rf not ready \n"));
			return b_need_to_change;
		}

		if((rtlpriv->mac80211.link_state == MAC80211_LINKING) ||
			(buddy_priv->mac80211.link_state == MAC80211_LINKING)) {
			/* Link In process */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("Link has been in Progress \n"));
			return b_need_to_change;
		}

		if ((rtlpriv->mac80211.act_scanning) ||
			(buddy_priv->mac80211.act_scanning)) {
			/* Scan In process */
			b_need_to_change = false;
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("Scan In Progress \n"));
			return b_need_to_change;
		}
	}

	return b_need_to_change;
}

void rtl92d_dm_easy_concurrent_switch_to_dmsp(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_hal *rtlhal = rtl_hal(rtlpriv);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	struct ieee80211_hw *buddy_hw = NULL;
	struct ieee80211_sta *sta = NULL;
	struct rtl_sta_info *sta_entry = NULL;
	bool bschedule_workitem = false;
	bool b_buddy_linked = false;
	bool b_change_to_dmsp = false;
	static int switch_count = 0;
	if (!rtlpriv->dm.supp_phymode_switch)
		return;
	if (rtlpriv->mac80211.opmode == NL80211_IFTYPE_ADHOC)
		return;
	b_change_to_dmsp = _rtl92d_dm_dual_mac_check_switch_to_dmsp(hw);
	RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
		(("rtlhal->macphymode %x \n"),rtlhal->macphymode));
	if (buddy_priv)
		buddy_hw = buddy_priv->mac80211.hw;
	if (!b_change_to_dmsp) {
		RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
			("Skip change To DMSP return\n"));

		if (buddy_priv != NULL) {
			RT_TRACE(COMP_EASY_CONCURRENT,DBG_LOUD,
				(("buddy mode is %d \n"),buddy_priv->mac80211.opmode));
		}
		return;
	} else {
		if ((buddy_priv->mac80211.opmode != NL80211_IFTYPE_AP) &&
			(rtlpriv->mac80211.opmode != NL80211_IFTYPE_AP)) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("STA + STA Case \n"));

			if(((rtlpriv->mac80211.link_state == MAC80211_LINKED) &&
				(buddy_priv->mac80211.link_state != MAC80211_LINKED)) ||
				((rtlpriv->mac80211.link_state != MAC80211_LINKED) &&
				(buddy_priv->mac80211.link_state == MAC80211_LINKED))) {
				bschedule_workitem = true;
				switch_count++;
			} else {
				bschedule_workitem = false;
				switch_count = 0;
			}
			if (bschedule_workitem && (switch_count == 3)) {
				RT_TRACE(COMP_EASY_CONCURRENT, DBG_EMERG,
					("Change to DMSP mode \n"));
				switch_count = 0;
				rcu_read_lock();
				if ((rtlpriv->mac80211.link_state == MAC80211_LINKED) &&
					(buddy_priv->mac80211.link_state != MAC80211_LINKED)) {
					sta = rtl_find_sta(hw, rtlpriv->mac80211.bssid);
				} else {
					sta = rtl_find_sta(buddy_priv->mac80211.hw,
						buddy_priv->mac80211.bssid);
					b_buddy_linked = true;

				}
				if (sta) {
					sta_entry = (struct rtl_sta_info *) sta->drv_priv;
					sta_entry->mimo_ps = IEEE80211_SMPS_OFF;
				}
				rtlpriv->easy_concurrent_ctl.bchange_to_dmsp = true;
				rtl_dualmac_easyconcurrent(hw);
				if (b_buddy_linked) {
					if (sta->ht_cap.ht_supported)
						rtl_send_smps_action(buddy_hw, sta, IEEE80211_SMPS_OFF);
				} else {
					if (sta->ht_cap.ht_supported)
						rtl_send_smps_action(hw, sta, IEEE80211_SMPS_OFF);
				}
				rcu_read_unlock();
			}
		} else if ((buddy_priv->mac80211.opmode == NL80211_IFTYPE_AP) &&
			 (rtlpriv->mac80211.opmode != NL80211_IFTYPE_AP)) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_LOUD,
				("MAC 0 STA and MAC 1 AP \n"));
		} else if((buddy_priv->mac80211.opmode != NL80211_IFTYPE_AP) &&
			(rtlpriv->mac80211.opmode == NL80211_IFTYPE_AP)) {
		}

	}
}

void rtl92d_dm_easy_concurrent_switch_to_dmdp(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	if (!rtlpriv->dm.supp_phymode_switch)
		return;
	if (buddy_priv == NULL)
		return;
	if ((buddy_priv->mac80211.opmode != NL80211_IFTYPE_AP) &&
		(rtlpriv->mac80211.opmode != NL80211_IFTYPE_AP)) {
		if ((rtlpriv->mac80211.link_state == MAC80211_NOLINK) &&
			(buddy_priv->mac80211.link_state == MAC80211_NOLINK)) {
			RT_TRACE(COMP_EASY_CONCURRENT, DBG_EMERG,
					("STA + STA no link Case \n"));
			rtlpriv->easy_concurrent_ctl.bchange_to_dmdp = true;
			buddy_priv->easy_concurrent_ctl.bchange_to_dmdp = true;

			if (rtlpriv->rtlhal.interfaceindex == 0)
				rtlpriv->easy_concurrent_ctl.bclose_bbandrf_for_dmsp
					= true;
			else
				buddy_priv->easy_concurrent_ctl.bclose_bbandrf_for_dmsp
					= true;

			rtl_dualmac_easyconcurrent(hw);
			rtl_dualmac_easyconcurrent(buddy_priv->mac80211.hw);
		}
	}
}

void rtl92d_dm_init(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	rtlpriv->dm.dm_type = DM_TYPE_BYDRIVER;
	rtl92d_dm_diginit(hw);
	rtl92d_dm_init_dynamic_txpower(hw);
	rtl92d_dm_init_edca_turbo(hw);
	rtl92d_dm_init_rate_adaptive_mask(hw);
	rtl92d_dm_initialize_txpower_tracking(hw);
}

void rtl92d_dm_watchdog(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtl_priv(hw));
	struct rtl_priv *buddy_priv = rtlpriv->buddy_priv;
	bool b_fw_current_inpsmode = false;
	bool fwps_awake = true;
	unsigned long flags = 0;

	if (rtlpriv->dm.supp_phymode_switch && buddy_priv == NULL) {
		if (rtlpriv->intf_ops->check_buddy_priv(hw, &buddy_priv)) {
			rtlpriv->buddy_priv = buddy_priv;
		}
	}
	if (ppsc->p2p_ps_info.p2p_ps_mode)
		fwps_awake = false;

	if ((ppsc->rfpwr_state == ERFON) && ((!b_fw_current_inpsmode) &&
		fwps_awake) && (!ppsc->rfchange_inprogress)) {
		rtl92d_dm_pwdb_monitor(hw);
		if (rtlpriv->dm.supp_phymode_switch) {
			spin_lock_irqsave(&rtlpriv->glb_var->glb_list_lock, flags);
			if (rtlpriv->rtlhal.bslave_of_dmsp)
				rtl92d_dm_false_alarm_counter_statistics_for_slaveofdmsp(hw);
			else
				rtl92d_dm_false_alarm_counter_statistics(hw);
			spin_unlock_irqrestore(&rtlpriv->glb_var->glb_list_lock, flags);
		} else {
			rtl92d_dm_false_alarm_counter_statistics(hw);
		}
		rtl92d_dm_find_minimum_rssi(hw);
		rtl92d_dm_dig(hw);
		/* rtl92d_dm_dynamic_bb_powersaving(hw); */
		rtl92d_dm_dynamic_txpower(hw);
		rtl92d_dm_easy_concurrent_switch_to_dmsp(hw);
		rtl92d_dm_easy_concurrent_switch_to_dmdp(hw);
		/* rtl92d_dm_check_txpower_tracking_thermal_meter(hw); */
		/* rtl92d_dm_refresh_rate_adaptive_mask(hw); */
		/* rtl92d_dm_interrupt_migration(hw); */
		rtl92d_dm_check_edca_turbo(hw);
	}
}
