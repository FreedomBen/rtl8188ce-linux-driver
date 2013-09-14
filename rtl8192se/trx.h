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
#ifndef __REALTEK_PCI92SE_TRX_H__
#define __REALTEK_PCI92SE_TRX_H__

/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
void rtl92se_tx_fill_desc(struct ieee80211_hw *hw, struct ieee80211_hdr *hdr,
	u8 *pdesc, struct ieee80211_tx_info *info, struct sk_buff* skb,
	u8 hw_queue, struct rtl_tcb_desc *ptcb_desc);
#else
void rtl92se_tx_fill_desc(struct ieee80211_hw *hw, struct ieee80211_hdr *hdr,
			  u8 *pdesc, struct ieee80211_tx_info *info,
			  struct ieee80211_sta *sta,
			  struct sk_buff *skb, u8 hw_queue,
			  struct rtl_tcb_desc *ptcb_desc);
/*<delete in kernel start>*/
#endif
/*<delete in kernel end>*/
void rtl92se_tx_fill_cmddesc(struct ieee80211_hw *hw, u8 *pdesc, bool b_firstseg,
	bool b_lastseg, struct sk_buff *skb);
bool rtl92se_rx_query_desc(struct ieee80211_hw *hw, struct rtl_stats *status,
	struct ieee80211_rx_status *rx_status, u8 *pdesc, struct sk_buff* skb);
void rtl92se_set_desc(u8* pdesc, bool istx,	u8 desc_name, u8 *val);
u32 rtl92se_get_desc(u8* pdesc, bool istx, u8 desc_name);
void rtl92se_tx_polling(struct ieee80211_hw *hw, u8 hw_queue);
#endif
