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
#include "../pci.h"
#include "../base.h"
#include "../stats.h"
#include "reg.h"
#include "def.h"
#include "phy.h"
#include "fw.h"
#include "trx.h"
#include "led.h"

u8 _rtl92se_map_hwqueue_to_fwqueue(struct sk_buff *skb,	u8 skb_queue)
{
	u16 fc = rtl_get_fc(skb);

	if (unlikely(ieee80211_is_beacon(fc)))
		return QSLT_BEACON;
	if (ieee80211_is_mgmt(fc) || ieee80211_is_ctl(fc))
		return QSLT_MGNT;
	if (ieee80211_is_nullfunc(fc))
		return QSLT_HIGH;

	return skb->priority;
}

/* mac80211's rate_idx is like this:
 *
 * 2.4G band:rx_status->band == IEEE80211_BAND_2GHZ
 *
 * B/G rate:
 * (rx_status->flag & RX_FLAG_HT) = 0,
 * DESC92C_RATE1M-->DESC92C_RATE54M ==> idx is 0-->11,
 *
 * N rate:
 * (rx_status->flag & RX_FLAG_HT) = 1,
 * DESC92C_RATEMCS0-->DESC92C_RATEMCS15 ==> idx is 0-->15
 *
 * 5G band:rx_status->band == IEEE80211_BAND_5GHZ
 * A rate:
 * (rx_status->flag & RX_FLAG_HT) = 0,
 * DESC92C_RATE6M-->DESC92C_RATE54M ==> idx is 0-->7,
 *
 * N rate:
 * (rx_status->flag & RX_FLAG_HT) = 1,
 * DESC92C_RATEMCS0-->DESC92C_RATEMCS15 ==> idx is 0-->15
 */
static int _rtl92se_rate_mapping(struct ieee80211_hw *hw,
	bool isht, u8 desc_rate)
{
	int rate_idx;

	if (false == isht) {
		if (IEEE80211_BAND_2GHZ == hw->conf.channel->band) {
			switch (desc_rate) {
			case DESC92S_RATE1M:
				rate_idx = 0;
				break;
			case DESC92S_RATE2M:
				rate_idx = 1;
				break;
			case DESC92S_RATE5_5M:
				rate_idx = 2;
				break;
			case DESC92S_RATE11M:
				rate_idx = 3;
				break;
			case DESC92S_RATE6M:
				rate_idx = 4;
				break;
			case DESC92S_RATE9M:
				rate_idx = 5;
				break;
			case DESC92S_RATE12M:
				rate_idx = 6;
				break;
			case DESC92S_RATE18M:
				rate_idx = 7;
				break;
			case DESC92S_RATE24M:
				rate_idx = 8;
				break;
			case DESC92S_RATE36M:
				rate_idx = 9;
				break;
			case DESC92S_RATE48M:
				rate_idx = 10;
				break;
			case DESC92S_RATE54M:
				rate_idx = 11;
				break;
			default:
				rate_idx = 0;
				break;
			}
		} else {
			switch (desc_rate) {
			case DESC92S_RATE6M:
				rate_idx = 0;
				break;
			case DESC92S_RATE9M:
				rate_idx = 1;
				break;
			case DESC92S_RATE12M:
				rate_idx = 2;
				break;
			case DESC92S_RATE18M:
				rate_idx = 3;
				break;
			case DESC92S_RATE24M:
				rate_idx = 4;
				break;
			case DESC92S_RATE36M:
				rate_idx = 5;
				break;
			case DESC92S_RATE48M:
				rate_idx = 6;
				break;
			case DESC92S_RATE54M:
				rate_idx = 7;
				break;
			default:
				rate_idx = 0;
				break;
			}
		}
	} else {
		switch(desc_rate) {
		case DESC92S_RATEMCS0:
			rate_idx = 0;
			break;
		case DESC92S_RATEMCS1:
			rate_idx = 1;
			break;
		case DESC92S_RATEMCS2:
			rate_idx = 2;
			break;
		case DESC92S_RATEMCS3:
			rate_idx = 3;
			break;
		case DESC92S_RATEMCS4:
			rate_idx = 4;
			break;
		case DESC92S_RATEMCS5:
			rate_idx = 5;
			break;
		case DESC92S_RATEMCS6:
			rate_idx = 6;
			break;
		case DESC92S_RATEMCS7:
			rate_idx = 7;
			break;
		case DESC92S_RATEMCS8:
			rate_idx = 8;
			break;
		case DESC92S_RATEMCS9:
			rate_idx = 9;
			break;
		case DESC92S_RATEMCS10:
			rate_idx = 10;
			break;
		case DESC92S_RATEMCS11:
			rate_idx = 11;
			break;
		case DESC92S_RATEMCS12:
			rate_idx = 12;
			break;
		case DESC92S_RATEMCS13:
			rate_idx = 13;
			break;
		case DESC92S_RATEMCS14:
			rate_idx = 14;
			break;
		case DESC92S_RATEMCS15:
			rate_idx = 15;
			break;
		default:
			rate_idx = 0;
			break;
		}
	}
	return rate_idx;
}

static void _rtl92se_query_rxphystatus(struct ieee80211_hw *hw,
		struct rtl_stats *pstatus, u8 *pdesc,
		struct rx_fwinfo *p_drvinfo, bool bpacket_match_bssid,
		bool bpacket_toself, bool b_packet_beacon)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_ps_ctl *ppsc = rtl_psc(rtlpriv);
	struct phy_sts_cck_8192s_t *cck_buf;
	s8 rx_pwr_all = 0, rx_pwr[4];
	u8 rf_rx_num = 0, evm, pwdb_all;
	u8 i, max_spatial_stream;
	u32 rssi, total_rssi = 0;
	bool b_is_cck = pstatus->b_is_cck;

	pstatus->b_packet_matchbssid = bpacket_match_bssid;
	pstatus->b_packet_toself = bpacket_toself;
	pstatus->b_packet_beacon = b_packet_beacon;
	pstatus->rx_mimo_signalquality[0] = -1;
	pstatus->rx_mimo_signalquality[1] = -1;

	if (b_is_cck) {
		u8 report, cck_highpwr;
		cck_buf = (struct phy_sts_cck_8192s_t *)p_drvinfo;

		if (ppsc->rfpwr_state == ERFON)
			cck_highpwr = (u8) rtl_get_bbreg(hw, RFPGA0_XA_HSSIPARAMETER2,
					0x200);
		else
			cck_highpwr = false;

		if (!cck_highpwr) {
			u8 cck_agc_rpt = cck_buf->cck_agc_rpt;
			report = cck_buf->cck_agc_rpt & 0xc0;
			report = report >> 6;
			switch (report) {
			case 0x3:
				rx_pwr_all = -40 - (cck_agc_rpt & 0x3e);
				break;
			case 0x2:
				rx_pwr_all = -20 - (cck_agc_rpt & 0x3e);
				break;
			case 0x1:
				rx_pwr_all = -2 - (cck_agc_rpt & 0x3e);
				break;
			case 0x0:
				rx_pwr_all = 14 - (cck_agc_rpt & 0x3e);
				break;
			}
		} else {
			u8 cck_agc_rpt = cck_buf->cck_agc_rpt;
			report = p_drvinfo->cfosho[0] & 0x60;
			report = report >> 5;
			switch (report) {
			case 0x3:
				rx_pwr_all = -40 - ((cck_agc_rpt & 0x1f) << 1);
				break;
			case 0x2:
				rx_pwr_all = -20 - ((cck_agc_rpt & 0x1f) << 1);
				break;
			case 0x1:
				rx_pwr_all = -2 - ((cck_agc_rpt & 0x1f) << 1);
				break;
			case 0x0:
				rx_pwr_all = 14 - ((cck_agc_rpt & 0x1f) << 1);
				break;
			}
		}

		pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
		/* CCK gain is smaller than OFDM/MCS gain,  */
		/* so we add gain diff by experiences, the val is 6 */
		pwdb_all += 6;
		if (pwdb_all > 100)
			pwdb_all = 100;
		/* modify the offset to make the same gain index with OFDM. */
		if (pwdb_all > 34 && pwdb_all <= 42)
			pwdb_all -= 2;
		else if (pwdb_all > 26 && pwdb_all <= 34)
			pwdb_all -= 6;
		else if (pwdb_all > 14 && pwdb_all <= 26)
			pwdb_all -= 8;
		else if (pwdb_all > 4 && pwdb_all <= 14)
			pwdb_all -= 4;

		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->recvsignalpower = rx_pwr_all;

		if (bpacket_match_bssid) {
			u8 sq;
			if (pstatus->rx_pwdb_all > 40) {
				sq = 100;
			} else {
				sq = cck_buf->sq_rpt;
				if (sq > 64)
					sq = 0;
				else if (sq < 20)
					sq = 100;
				else
					sq = ((64 - sq) * 100) / 44;
			}

			pstatus->signalquality = sq;
			pstatus->rx_mimo_signalquality[0] = sq;
			pstatus->rx_mimo_signalquality[1] = -1;
		}
	} else {
		rtlpriv->dm.brfpath_rxenable[0] =
		    rtlpriv->dm.brfpath_rxenable[1] = true;
		for (i = RF90_PATH_A; i < RF6052_MAX_PATH; i++) {
			if (rtlpriv->dm.brfpath_rxenable[i])
				rf_rx_num++;

			rx_pwr[i] = ((p_drvinfo->gain_trsw[i] & 0x3f) * 2) - 110;
			rssi = rtl_query_rxpwrpercentage(rx_pwr[i]);
			total_rssi += rssi;
			rtlpriv->stats.rx_snr_db[i] = (long)(p_drvinfo->rxsnr[i] / 2);

			if (bpacket_match_bssid)
				pstatus->rx_mimo_signalstrength[i] = (u8) rssi;
		}

		rx_pwr_all = ((p_drvinfo->pwdb_all >> 1) & 0x7f) - 110;
		pwdb_all = rtl_query_rxpwrpercentage(rx_pwr_all);
		pstatus->rx_pwdb_all = pwdb_all;
		pstatus->rxpower = rx_pwr_all;
		pstatus->recvsignalpower = rx_pwr_all;

		if (pstatus->b_is_ht && pstatus->rate >= DESC92S_RATEMCS8 &&
		    pstatus->rate <= DESC92S_RATEMCS15)
			max_spatial_stream = 2;
		else
			max_spatial_stream = 1;

		for (i = 0; i < max_spatial_stream; i++) {
			evm = rtl_evm_db_to_percentage(p_drvinfo->rxevm[i]);

			if (bpacket_match_bssid) {
				if (i == 0)
					pstatus->signalquality = (u8) (evm & 0xff);
				pstatus->rx_mimo_signalquality[i] = (u8) (evm & 0xff);
			}
		}
	}

	if (b_is_cck)
		pstatus->signalstrength = (u8)(rtl_signal_scale_mapping(hw,
				pwdb_all));
	else if (rf_rx_num != 0)
		pstatus->signalstrength = (u8) (rtl_signal_scale_mapping(hw,
				total_rssi /= rf_rx_num));
}

static void _rtl92se_translate_rx_signal_stuff(struct ieee80211_hw *hw,
		struct sk_buff *skb, struct rtl_stats *pstatus,
		u8 *pdesc, struct rx_fwinfo *p_drvinfo)
{
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_efuse *rtlefuse = rtl_efuse(rtl_priv(hw));
	struct ieee80211_hdr *hdr;
	u8 *tmp_buf;
	u8 *praddr;
	u8 *psaddr;
	u16 fc, type;
	bool b_packet_matchbssid, b_packet_toself, b_packet_beacon;

	tmp_buf = skb->data + pstatus->rx_drvinfo_size + pstatus->rx_bufshift;

	hdr = (struct ieee80211_hdr *)tmp_buf;
	fc = le16_to_cpu(hdr->frame_control);
	type = WLAN_FC_GET_TYPE(fc);
	praddr = hdr->addr1;
	psaddr = ieee80211_get_SA(hdr);

	b_packet_matchbssid = ((IEEE80211_FTYPE_CTL != type) &&
	     (!compare_ether_addr(mac->bssid, (fc & IEEE80211_FCTL_TODS) ?
			hdr->addr1 : (fc & IEEE80211_FCTL_FROMDS) ?
			hdr->addr2 : hdr->addr3)) && (!pstatus->b_hwerror) &&
			(!pstatus->b_crc) && (!pstatus->b_icv));

	b_packet_toself = b_packet_matchbssid &&
	    (!compare_ether_addr(praddr, rtlefuse->dev_addr));

	if (ieee80211_is_beacon(fc))
		b_packet_beacon = true;

	_rtl92se_query_rxphystatus(hw, pstatus, pdesc, p_drvinfo,
			b_packet_matchbssid, b_packet_toself,
			b_packet_beacon);

	rtl_process_phyinfo(hw, tmp_buf, pstatus);
}

bool rtl92se_rx_query_desc(struct ieee80211_hw *hw, struct rtl_stats *status,
		struct ieee80211_rx_status *rx_status, u8 *pdesc, struct sk_buff *skb)
{
	struct rx_fwinfo *p_drvinfo;
	u32 phystatus = (u32)GET_RX_STATUS_DESC_PHY_STATUS(pdesc);
	struct ieee80211_hdr *hdr;

	status->length = (u16)GET_RX_STATUS_DESC_PKT_LEN(pdesc);
	status->rx_drvinfo_size = (u8)GET_RX_STATUS_DESC_DRVINFO_SIZE(pdesc) * 8;
	status->rx_bufshift 	= (u8)(GET_RX_STATUS_DESC_SHIFT(pdesc) & 0x03);
	status->b_icv = (u16)GET_RX_STATUS_DESC_ICV(pdesc);
	status->b_crc = (u16)GET_RX_STATUS_DESC_CRC32(pdesc);
	status->b_hwerror = (u16)(status->b_crc | status->b_icv);
	status->decrypted = !GET_RX_STATUS_DESC_SWDEC(pdesc);

	status->rate = (u8)GET_RX_STATUS_DESC_RX_MCS(pdesc);
	status->b_shortpreamble = (u16)GET_RX_STATUS_DESC_SPLCP(pdesc);
	status->b_isampdu = (bool)(GET_RX_STATUS_DESC_PAGGR(pdesc) == 1);
	status->b_isfirst_ampdu = (bool) ((GET_RX_STATUS_DESC_PAGGR(pdesc) == 1)
			&& (GET_RX_STATUS_DESC_FAGGR(pdesc) == 1));
	status->timestamp_low = GET_RX_STATUS_DESC_TSFL(pdesc);
	status->rx_is40Mhzpacket = (bool)GET_RX_STATUS_DESC_BW(pdesc);
	status->b_is_ht = (bool)GET_RX_STATUS_DESC_RX_HT(pdesc);

	status->b_is_cck = RX_HAL_IS_CCK_RATE(status->rate);

	if (status->b_hwerror)
		return false;

	rx_status->freq = hw->conf.channel->center_freq;
	rx_status->band = hw->conf.channel->band;

	hdr = (struct ieee80211_hdr *)(skb->data + status->rx_drvinfo_size
	                        + status->rx_bufshift);

	if (status->b_crc)
		rx_status->flag |= RX_FLAG_FAILED_FCS_CRC;

	if (status->rx_is40Mhzpacket)
		rx_status->flag |= RX_FLAG_40MHZ;

	if (status->b_is_ht)
		rx_status->flag |= RX_FLAG_HT;

	rx_status->flag |= RX_FLAG_MACTIME_MPDU;

	/* hw will set status->decrypted true, if it finds the
	 * frame is open data frame or mgmt frame. */
	/* So hw will not decryption robust managment frame
	 * for IEEE80211w but still set status->decrypted
	 * true, so here we should set it back to undecrypted
	 * for IEEE80211w frame, and mac80211 sw will help
	 * to decrypt it */
	if (status->decrypted) {
		if ((ieee80211_is_robust_mgmt_frame(hdr)) &&
			(ieee80211_has_protected(hdr->frame_control)))
			rx_status->flag &= ~RX_FLAG_DECRYPTED;
		else
			rx_status->flag |= RX_FLAG_DECRYPTED;
	}

	rx_status->rate_idx = _rtl92se_rate_mapping(hw,
			status->b_is_ht, status->rate);


	rx_status->mactime = status->timestamp_low;
	if (phystatus == true) {
		p_drvinfo = (struct rx_fwinfo *)(skb->data + status->rx_bufshift);
		_rtl92se_translate_rx_signal_stuff(hw, skb, status, pdesc, p_drvinfo);
	}

	/*rx_status->qual = status->signal; */
	rx_status->signal = status->recvsignalpower + 10;
	/*rx_status->noise = -status->noise; */

	return true;
}

/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
void rtl92se_tx_fill_desc(struct ieee80211_hw *hw, struct ieee80211_hdr *hdr,
	u8 *pdesc_tx, struct ieee80211_tx_info *info, struct sk_buff* skb,
		u8 hw_queue, struct rtl_tcb_desc *ptcb_desc)
#else
void rtl92se_tx_fill_desc(struct ieee80211_hw *hw, struct ieee80211_hdr *hdr,
			  u8 *pdesc_tx, struct ieee80211_tx_info *info,
			  struct ieee80211_sta *sta,
			  struct sk_buff *skb, u8 hw_queue,
			  struct rtl_tcb_desc *ptcb_desc)
/*<delete in kernel start>*/
#endif
/*<delete in kernel end>*/
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	struct ieee80211_sta *sta = info->control.sta;
#endif
/*<delete in kernel end>*/
	u8 *pdesc = (u8 *) pdesc_tx;
	u16 seq_number;
	u16 fc = le16_to_cpu(hdr->frame_control);
	u8 reserved_macid = 0;
	u8 fw_qsel = _rtl92se_map_hwqueue_to_fwqueue(skb, hw_queue);
	bool b_firstseg = (!(hdr->seq_ctrl & cpu_to_le16(IEEE80211_SCTL_FRAG)));
	bool b_lastseg = (!(hdr->frame_control &
			cpu_to_le16(IEEE80211_FCTL_MOREFRAGS)));
	dma_addr_t mapping = pci_map_single(rtlpci->pdev, skb->data, skb->len,
		    PCI_DMA_TODEVICE);
	u8 bw_40 = 0;

	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(COMP_SEND, DBG_TRACE,
			 ("DMA mapping error"));
		return;
	}
	if (mac->opmode == NL80211_IFTYPE_STATION) {
		bw_40 = mac->bw_40;
	} else if (mac->opmode == NL80211_IFTYPE_AP ||
		mac->opmode == NL80211_IFTYPE_ADHOC) {
		if (sta)
			bw_40 = sta->ht_cap.cap & IEEE80211_HT_CAP_SUP_WIDTH_20_40;
	}

	seq_number = (le16_to_cpu(hdr->seq_ctrl) & IEEE80211_SCTL_SEQ) >> 4;

	rtl_get_tcb_desc(hw, info, sta, skb, ptcb_desc);

	CLEAR_PCI_TX_DESC_CONTENT(pdesc, TX_DESC_SIZE_RTL8192S);

	if (ieee80211_is_nullfunc(fc) || ieee80211_is_ctl(fc)) {
		b_firstseg = true;
		b_lastseg = true;
	}

	if (b_firstseg) {
		if (rtlpriv->dm.b_useramask) {
			/* set txdesc macId */
			if (ptcb_desc->mac_id < 32) {
				SET_TX_DESC_MACID(pdesc, ptcb_desc->mac_id);
				reserved_macid |= ptcb_desc->mac_id;
			}
		}
		SET_TX_DESC_RSVD_MACID(pdesc, reserved_macid);

		SET_TX_DESC_TXHT(pdesc, ((ptcb_desc->hw_rate >= DESC92S_RATEMCS0) ?
					1 : 0));

		if (rtlhal->version == VERSION_8192S_ACUT) {
			if (ptcb_desc->hw_rate == DESC92S_RATE1M ||
				ptcb_desc->hw_rate  == DESC92S_RATE2M ||
				ptcb_desc->hw_rate == DESC92S_RATE5_5M ||
				ptcb_desc->hw_rate == DESC92S_RATE11M) {
				ptcb_desc->hw_rate = DESC92S_RATE12M;
			}
		}

		SET_TX_DESC_TX_RATE(pdesc, ptcb_desc->hw_rate);

		if (ptcb_desc->use_shortgi || ptcb_desc->use_shortpreamble)
			SET_TX_DESC_TX_SHORT(pdesc, 0);

		/* Aggregation related */
		if (info->flags & IEEE80211_TX_CTL_AMPDU)
			SET_TX_DESC_AGG_ENABLE(pdesc, 1);

		/* For AMPDU, we must insert SSN into TX_DESC */
		SET_TX_DESC_SEQ(pdesc, seq_number);

		/* Protection mode related */
		/* For 92S, if RTS/CTS are set, HW will execute RTS. */
		/* We choose only one protection mode to execute */
		SET_TX_DESC_RTS_ENABLE(pdesc, ((ptcb_desc->b_rts_enable &&
				!ptcb_desc->b_cts_enable) ? 1 : 0));
		SET_TX_DESC_CTS_ENABLE(pdesc, ((ptcb_desc->b_cts_enable) ? 1 : 0));
		SET_TX_DESC_RTS_STBC(pdesc, ((ptcb_desc->b_rts_stbc) ? 1 : 0));
		/* SET_TX_DESC_RTS_HT(pdesc, ((ptcb_desc->rts_rate & 0x80) ? 1 : 0)); */

		SET_TX_DESC_RTS_RATE(pdesc, ptcb_desc->rts_rate);
		SET_TX_DESC_RTS_BANDWIDTH(pdesc, 0);
		SET_TX_DESC_RTS_SUB_CARRIER(pdesc, ptcb_desc->rts_sc);
		SET_TX_DESC_RTS_SHORT(pdesc, ((ptcb_desc->rts_rate <= DESC92S_RATE54M) ?
		       (ptcb_desc->b_rts_use_shortpreamble ? 1 : 0)
		       : (ptcb_desc->b_rts_use_shortgi ? 1 : 0)));


		/* Set Bandwidth and sub-channel settings. */
		if (bw_40) {
			if (ptcb_desc->b_packet_bw) {
				SET_TX_DESC_TX_BANDWIDTH(pdesc, 1);
				/* use duplicated mode */
				SET_TX_DESC_TX_SUB_CARRIER(pdesc, 0);
			} else {
				SET_TX_DESC_TX_BANDWIDTH(pdesc, 0);
				SET_TX_DESC_TX_SUB_CARRIER(pdesc, mac->cur_40_prime_sc);
			}
		} else {
			SET_TX_DESC_TX_BANDWIDTH(pdesc, 0);
			SET_TX_DESC_TX_SUB_CARRIER(pdesc, 0);
		}

		/* 3 Fill necessary field in First Descriptor */
		/*DWORD 0*/
		SET_TX_DESC_LINIP(pdesc, 0);
		SET_TX_DESC_OFFSET(pdesc, 32);
		SET_TX_DESC_PKT_SIZE(pdesc, (u16) skb->len);

		/*DWORD 1*/
		SET_TX_DESC_RA_BRSR_ID(pdesc, ptcb_desc->ratr_index);

		/* Fill security related */
		if (info->control.hw_key) {
			struct ieee80211_key_conf *keyconf = info->control.hw_key;

/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(2,6,37))
/*<delete in kernel end>*/
			switch (keyconf->cipher) {
			case WLAN_CIPHER_SUITE_WEP40:
			case WLAN_CIPHER_SUITE_WEP104:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x1);
				break;
			case WLAN_CIPHER_SUITE_TKIP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x2);
				break;
			case WLAN_CIPHER_SUITE_CCMP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x3);
				break;
			default:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x0);
				break;

			}
/*<delete in kernel start>*/
#else
			switch (keyconf->alg) {
			case ALG_WEP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x1);
				break;
			case ALG_TKIP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x2);
				break;
			case ALG_CCMP:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x3);
				break;
			default:
				SET_TX_DESC_SEC_TYPE(pdesc, 0x0);
				break;

			}
#endif
/*<delete in kernel end>*/
		}

		/* Set Packet ID */
		SET_TX_DESC_PACKET_ID(pdesc, 0);

		/* We will assign magement queue to BK. */
		SET_TX_DESC_QUEUE_SEL(pdesc, fw_qsel);

		/* Alwasy enable all rate fallback range */
		SET_TX_DESC_DATA_RATE_FB_LIMIT(pdesc, 0x1F);

		/* Fix: I don't kown why hw use 6.5M to tx when set it */
		//SET_TX_DESC_DISABLE_FB(pdesc, tcb_desc.disable_ratefallback);
		SET_TX_DESC_USER_RATE(pdesc, ptcb_desc->use_driver_rate ? 1 : 0);

		/* Set NON_QOS bit. */
		if (!ieee80211_is_data_qos(fc)) {
			SET_TX_DESC_NON_QOS(pdesc, 1);
		}

	}

	/* Fill fields that are required to be initialized
	 * in all of the descriptors */
	/*DWORD 0 */
	SET_TX_DESC_FIRST_SEG(pdesc, (b_firstseg ? 1 : 0));
	SET_TX_DESC_LAST_SEG(pdesc, (b_lastseg ? 1 : 0));

	/* DWORD 7 */
	SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16) skb->len);

	/* DOWRD 8 */
	SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, cpu_to_le32(mapping));

	RT_TRACE(COMP_SEND, DBG_TRACE, ("\n"));
}

void rtl92se_tx_fill_cmddesc(struct ieee80211_hw *hw, u8 *pdesc,
	bool b_firstseg, bool b_lastseg, struct sk_buff *skb)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_pci *rtlpci = rtl_pcidev(rtl_pcipriv(hw));
	struct rtl_hal *rtlhal = rtl_hal(rtl_priv(hw));
	struct rtl_tcb_desc *tcb_desc = (struct rtl_tcb_desc *)(skb->cb);

	dma_addr_t mapping = pci_map_single(rtlpci->pdev, skb->data, skb->len,
			PCI_DMA_TODEVICE);

	if (pci_dma_mapping_error(rtlpci->pdev, mapping)) {
		RT_TRACE(COMP_SEND, DBG_TRACE,
			 ("DMA mapping error"));
		return;
	}
    /* Clear all status	*/
	CLEAR_PCI_TX_DESC_CONTENT(pdesc, TX_CMDDESC_SIZE_RTL8192S);

	/* This bit indicate this packet is used for FW download. */
	if (tcb_desc->b_cmd_or_init == DESC_PACKET_TYPE_INIT) {
		/* For firmware downlaod we only need to set LINIP */
		SET_TX_DESC_LINIP(pdesc, tcb_desc->b_last_inipkt);

		/* 92SE must set as 1 for firmware download HW DMA error */
		SET_TX_DESC_FIRST_SEG(pdesc, 1);
		SET_TX_DESC_LAST_SEG(pdesc, 1);

		/* 92SE need not to set TX packet size when firmware download */
		SET_TX_DESC_PKT_SIZE(pdesc, (u16)(skb->len));
		SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16)(skb->len));
		SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, cpu_to_le32(mapping));

		SET_TX_DESC_OWN(pdesc, 1);
	} else { /* H2C Command Desc format (Host TXCMD) */
		/* 92SE must set as 1 for firmware download HW DMA error */
		SET_TX_DESC_FIRST_SEG(pdesc, 1);
		SET_TX_DESC_LAST_SEG(pdesc, 1);

		SET_TX_DESC_OFFSET(pdesc, 0x20);

		/* Buffer size + command header */
		SET_TX_DESC_PKT_SIZE(pdesc, (u16)(skb->len));
		/* Fixed queue of H2C command */
		SET_TX_DESC_QUEUE_SEL(pdesc, 0x13);

		SET_BITS_TO_LE_4BYTE(skb->data, 24, 7, rtlhal->h2c_txcmd_seq);

		SET_TX_DESC_TX_BUFFER_SIZE(pdesc, (u16)(skb->len));
		SET_TX_DESC_TX_BUFFER_ADDRESS(pdesc, cpu_to_le32(mapping));

		SET_TX_DESC_OWN(pdesc, 1);

	}
}

void rtl92se_set_desc(u8 * pdesc, bool istx, u8 desc_name, u8 * val)
{
	if (istx == true) {
		switch (desc_name) {
		case HW_DESC_OWN:
			SET_TX_DESC_OWN(pdesc, 1);
			break;
		case HW_DESC_TX_NEXTDESC_ADDR:
			SET_TX_DESC_NEXT_DESC_ADDRESS(pdesc, *(u32 *) val);
			break;
		default:
			RT_ASSERT(false, ("ERR txdesc :%d not process\n", desc_name));
			break;
		}
	} else {
		switch (desc_name) {
		case HW_DESC_RXOWN:
			SET_RX_STATUS_DESC_OWN(pdesc, 1);
			break;
		case HW_DESC_RXBUFF_ADDR:
			SET_RX_STATUS__DESC_BUFF_ADDR(pdesc, *(u32 *) val);
			break;
		case HW_DESC_RXPKT_LEN:
			SET_RX_STATUS_DESC_PKT_LEN(pdesc, *(u32 *) val);
			break;
		case HW_DESC_RXERO:
			SET_RX_STATUS_DESC_EOR(pdesc, 1);
			break;
		default:
			RT_ASSERT(false, ("ERR rxdesc :%d not process\n", desc_name));
			break;
		}
	}
}

u32 rtl92se_get_desc(u8 * pdesc, bool istx, u8 desc_name)
{
	u32 ret = 0;

	if (istx == true) {
		switch (desc_name) {
		case HW_DESC_OWN:
			ret = GET_TX_DESC_OWN(pdesc);
			break;
		case HW_DESC_TXBUFF_ADDR:
			ret = GET_TX_DESC_TX_BUFFER_ADDRESS(pdesc);
			break;
		default:
			RT_ASSERT(false, ("ERR txdesc :%d not process\n", desc_name));
			break;
		}
	} else {
		switch (desc_name) {
		case HW_DESC_OWN:
			ret = GET_RX_STATUS_DESC_OWN(pdesc);
			break;
		case HW_DESC_RXPKT_LEN:
			ret = GET_RX_STATUS_DESC_PKT_LEN(pdesc);
			break;
		default:
			RT_ASSERT(false, ("ERR rxdesc :%d not process\n", desc_name));
			break;
		}
	}
	return ret;
}

void rtl92se_tx_polling(struct ieee80211_hw *hw, u8 hw_queue)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	rtl_write_word(rtlpriv, TP_POLL, BIT(0) << (hw_queue));
}
