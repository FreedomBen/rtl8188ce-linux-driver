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

#ifndef	__RTL8723E_DM_H__
#define __RTL8723E_DM_H__

#define HAL_DM_DIG_DISABLE			BIT(0)
#define HAL_DM_HIPWR_DISABLE		BIT(1)

#define OFDM_TABLE_LENGTH 			37
#define CCK_TABLE_LENGTH 			33

#define OFDM_TABLE_SIZE 			37
#define CCK_TABLE_SIZE				33

#define BW_AUTO_SWITCH_HIGH_LOW		25
#define BW_AUTO_SWITCH_LOW_HIGH		30

#define DM_DIG_THRESH_HIGH			40
#define DM_DIG_THRESH_LOW			35

#define DM_FALSEALARM_THRESH_LOW	400
#define DM_FALSEALARM_THRESH_HIGH	1000

#define DM_DIG_MAX					0x3e
#define DM_DIG_MIN					0x1e

#define DM_DIG_FA_UPPER				0x32
#define DM_DIG_FA_LOWER				0x20
#define DM_DIG_FA_TH0				0x20
#define DM_DIG_FA_TH1				0x100
#define DM_DIG_FA_TH2				0x200

#define DM_DIG_BACKOFF_MAX			12
#define DM_DIG_BACKOFF_MIN			-4
#define DM_DIG_BACKOFF_DEFAULT		10

#define RXPATHSELECTION_SS_TH_lOW	30
#define RXPATHSELECTION_DIFF_TH		18

#define DM_RATR_STA_INIT			0
#define DM_RATR_STA_HIGH			1
#define DM_RATR_STA_MIDDLE			2
#define DM_RATR_STA_LOW				3

#define CTS2SELF_THVAL				30
#define REGC38_TH					20

#define WAIOTTHVal					25

#define TXHIGHPWRLEVEL_NORMAL		0
#define TXHIGHPWRLEVEL_LEVEL1		1
#define TXHIGHPWRLEVEL_LEVEL2		2
#define TXHIGHPWRLEVEL_BT1			3
#define TXHIGHPWRLEVEL_BT2			4

#define DM_TYPE_BYFW				0
#define DM_TYPE_BYDRIVER			1

#define TX_POWER_NEAR_FIELD_THRESH_LVL2	74
#define TX_POWER_NEAR_FIELD_THRESH_LVL1	67

struct ps_t {
	u8 pre_ccastate;
	u8 cur_ccasate;
	u8 pre_rfstate;
	u8 cur_rfstate;
	long rssi_val_min;

};

struct dig_t {
	u8 dig_enable_flag;
	u8 dig_ext_port_stage;
	u32 rssi_lowthresh;
	u32 rssi_highthresh;
	u32 fa_lowthresh;
	u32 fa_highthresh;
	u8 cursta_connectctate;
	u8 presta_connectstate;
	u8 curmultista_connectstate;
	u8 pre_igvalue;
	u8 cur_igvalue;
	char backoff_val;
	char backoff_val_range_max;
	char backoff_val_range_min;
	u8 rx_gain_range_max;
	u8 rx_gain_range_min;
	u8 rssi_val_min;
	u8 pre_cck_pd_state;
	u8 cur_cck_pd_state;
	u8 pre_cck_fa_state;
	u8 cur_cck_fa_state;
	u8 pre_ccastate;
	u8 cur_ccasate;
};

struct swat_t {
	u8 failure_cnt;
	u8 try_flag;
	u8 stop_trying;
	long pre_rssi;
	long trying_threshold;
	u8 cur_antenna;
	u8 pre_antenna;

};

enum tag_dynamic_init_gain_operation_type_definition {
	DIG_TYPE_THRESH_HIGH = 0,
	DIG_TYPE_THRESH_LOW = 1,
	DIG_TYPE_BACKOFF = 2,
	DIG_TYPE_RX_GAIN_MIN = 3,
	DIG_TYPE_RX_GAIN_MAX = 4,
	DIG_TYPE_ENABLE = 5,
	DIG_TYPE_DISABLE = 6,
	DIG_OP_TYPE_MAX
};

enum tag_cck_packet_detection_threshold_type_definition {
	CCK_PD_STAGE_LowRssi = 0,
	CCK_PD_STAGE_HighRssi = 1,
	CCK_FA_STAGE_Low = 2,
	CCK_FA_STAGE_High = 3,
	CCK_PD_STAGE_MAX = 4,
};

enum dm_1r_cca_e {
	CCA_1R = 0,
	CCA_2R = 1,
	CCA_MAX = 2,
};

enum dm_rf_e {
	RF_SAVE = 0,
	RF_NORMAL = 1,
	RF_MAX = 2,
};

enum dm_sw_ant_switch_e {
	ANS_ANTENNA_B = 1,
	ANS_ANTENNA_A = 2,
	ANS_ANTENNA_MAX = 3,
};

enum dm_dig_ext_port_alg_e {
	DIG_EXT_PORT_STAGE_0 = 0,
	DIG_EXT_PORT_STAGE_1 = 1,
	DIG_EXT_PORT_STAGE_2 = 2,
	DIG_EXT_PORT_STAGE_3 = 3,
	DIG_EXT_PORT_STAGE_MAX = 4,
};

enum dm_dig_connect_e {
	DIG_STA_DISCONNECT = 0,
	DIG_STA_CONNECT = 1,
	DIG_STA_BEFORE_CONNECT = 2,
	DIG_MULTISTA_DISCONNECT = 3,
	DIG_MULTISTA_CONNECT = 4,
	DIG_CONNECT_MAX
};

#define BT_RSSI_STATE_NORMAL_POWER      BIT_OFFSET_LEN_MASK_32(0, 1)
#define BT_RSSI_STATE_AMDPU_OFF         BIT_OFFSET_LEN_MASK_32(1, 1)
#define BT_RSSI_STATE_SPECIAL_LOW       BIT_OFFSET_LEN_MASK_32(2, 1)
#define BT_RSSI_STATE_BG_EDCA_LOW       BIT_OFFSET_LEN_MASK_32(3, 1)
#define BT_RSSI_STATE_TXPOWER_LOW       BIT_OFFSET_LEN_MASK_32(4, 1)
#define GET_UNDECORATED_AVERAGE_RSSI(_priv)     \
        (((struct rtl_priv *)(_priv))->mac80211.opmode == NL80211_IFTYPE_ADHOC)?  \
        (((struct rtl_priv *)(_priv))->dm.entry_min_undecoratedsmoothed_pwdb):  \
        (((struct rtl_priv *)(_priv))->dm.undecorated_smoothed_pwdb)
extern struct dig_t dm_digtable;
void rtl8723e_dm_init(struct ieee80211_hw *hw);
void rtl8723e_dm_watchdog(struct ieee80211_hw *hw);
void rtl8723e_dm_write_dig(struct ieee80211_hw *hw);
void rtl8723e_dm_init_edca_turbo(struct ieee80211_hw *hw);
void rtl8723e_dm_check_txpower_tracking(struct ieee80211_hw *hw);
void rtl8723e_dm_init_rate_adaptive_mask(struct ieee80211_hw *hw);
void rtl8723e_dm_rf_saving(struct ieee80211_hw *hw, u8 bforce_in_normal);
void rtl8723e_dm_bt_coexist(struct ieee80211_hw *hw);
#endif
