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

#ifndef __RTL_WIFI_H__
#define __RTL_WIFI_H__

#include <linux/interrupt.h>
#include <linux/sched.h>
#include <linux/firmware.h>
#include <linux/version.h>
#include <linux/etherdevice.h>
#include <net/mac80211.h>
#include "debug.h"

#define RF_CHANGE_BY_INIT			0
#define RF_CHANGE_BY_IPS 			BIT(28)
#define RF_CHANGE_BY_PS 			BIT(29)
#define RF_CHANGE_BY_HW 			BIT(30)
#define RF_CHANGE_BY_SW 			BIT(31)

#define IQK_ADDA_REG_NUM			16
#define IQK_MAC_REG_NUM				4

#define MAX_KEY_LEN					61
#define KEY_BUF_SIZE				5

/* QoS related. */
/*aci: 0x00	Best Effort*/
/*aci: 0x01	Background*/
/*aci: 0x10	Video*/
/*aci: 0x11	Voice*/
/*Max: define total number.*/
#define AC0_BE						0
#define AC1_BK						1
#define AC2_VI						2
#define AC3_VO						3
#define AC_MAX						4
#define QOS_QUEUE_NUM				4
#define RTL_MAC80211_NUM_QUEUE		5

#define QBSS_LOAD_SIZE				5
#define MAX_WMMELE_LENGTH			64

#define TOTAL_CAM_ENTRY 				32

/*slot time for 11g. */
#define RTL_SLOT_TIME_9				9
#define RTL_SLOT_TIME_20			20

/*related with tcp/ip. */
/*if_ehther.h*/
#define ETH_P_PAE 					0x888E	/*Port Access Entity (IEEE 802.1X) */
#define ETH_P_IP        			0x0800	/*Internet Protocol packet */
#define ETH_P_ARP       			0x0806	/*Address Resolution packet */
#define SNAP_SIZE 					6
#define PROTOC_TYPE_SIZE			2

/*related with 802.11 frame*/
#define MAC80211_3ADDR_LEN 			24
#define MAC80211_4ADDR_LEN 			30

#define CHANNEL_MAX_NUMBER		(14 + 24 + 21)	/*  14 is the max channel number */
#define CHANNEL_GROUP_MAX		(3 + 9)	/*  ch1~3, ch4~9, ch10~14 total three groups */
#define MAX_PG_GROUP 			13
#define	CHANNEL_GROUP_MAX_2G		3
#define	CHANNEL_GROUP_IDX_5GL		3
#define	CHANNEL_GROUP_IDX_5GM		6
#define	CHANNEL_GROUP_IDX_5GH		9
#define	CHANNEL_GROUP_MAX_5G		9
#define CHANNEL_MAX_NUMBER_2G		14
#define AVG_THERMAL_NUM			8
#define AVG_THERMAL_NUM_88E		4
#define MAX_TID_COUNT			9
#define MAX_NUM_RATES			264

/*for 88E use*/
/*It must always set to 4, otherwise read efuse table secquence will be wrong.*/
#define 	MAX_TX_COUNT				4
#define	MAX_RF_PATH	4
#define	MAX_CHNL_GROUP_24G		6 
#define	MAX_CHNL_GROUP_5G		14 


struct txpower_info_2g {
	u8 index_cck_base[MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	u8 index_bw40_base[MAX_RF_PATH][MAX_CHNL_GROUP_24G];
	/*If only one tx, only BW20 and OFDM are used.*/
	u8 cck_diff[MAX_RF_PATH][MAX_TX_COUNT];	
	u8 ofdm_diff[MAX_RF_PATH][MAX_TX_COUNT];
	u8 bw20_diff[MAX_RF_PATH][MAX_TX_COUNT];
	u8 bw40_diff[MAX_RF_PATH][MAX_TX_COUNT];
};

struct txpower_info_5g {
	u8 index_bw40_base[MAX_RF_PATH][MAX_CHNL_GROUP_5G];
	/*If only one tx, only BW20, OFDM, BW80 and BW160 are used.*/
	u8 ofdm_diff[MAX_RF_PATH][MAX_TX_COUNT];
	u8 bw20_diff[MAX_RF_PATH][MAX_TX_COUNT];
	u8 bw40_diff[MAX_RF_PATH][MAX_TX_COUNT];
};


/* for early mode */
#define EM_HDR_LEN				8
#define FCS_LEN				4

#define MAX_VIRTUAL_MAC			1

enum intf_type {
	INTF_PCI = 0,
	INTF_USB = 1,
};

enum radio_path {
	RF90_PATH_A = 0,
	RF90_PATH_B = 1,
	RF90_PATH_C = 2,
	RF90_PATH_D = 3,
};

enum rt_eeprom_type {
	EEPROM_93C46,
	EEPROM_93C56,
	EEPROM_BOOT_EFUSE,
};

enum rtl_status {
	RTL_STATUS_INTERFACE_START = 0,
};

enum hardware_type {
	HARDWARE_TYPE_RTL8192E,
	HARDWARE_TYPE_RTL8192U,
	HARDWARE_TYPE_RTL8192SE,
	HARDWARE_TYPE_RTL8192SU,
	HARDWARE_TYPE_RTL8192CE,
	HARDWARE_TYPE_RTL8192CU,
	HARDWARE_TYPE_RTL8192DE,
	HARDWARE_TYPE_RTL8192DU,
	HARDWARE_TYPE_RTL8723AE,
	HARDWARE_TYPE_RTL8188EE,
	/* keep it last */
	HARDWARE_TYPE_NUM
};

enum scan_operation_backup_opt {
	SCAN_OPT_BACKUP = 0,
	SCAN_OPT_RESTORE,
	SCAN_OPT_MAX
};

/*RF state.*/
enum rf_pwrstate {
	ERFON,
	ERFSLEEP,
	ERFOFF
};

struct bb_reg_def {
	u32 rfintfs;
	u32 rfintfi;
	u32 rfintfo;
	u32 rfintfe;
	u32 rf3wire_offset;
	u32 rflssi_select;
	u32 rftxgain_stage;
	u32 rfhssi_para1;
	u32 rfhssi_para2;
	u32 rfswitch_control;
	u32 rfagc_control1;
	u32 rfagc_control2;
	u32 rfrxiq_imbalance;
	u32 rfrx_afe;
	u32 rftxiq_imbalance;
	u32 rftx_afe;
	u32 rflssi_readback;
	u32 rflssi_readbackpi;
};

enum io_type {
	IO_CMD_PAUSE_DM_BY_SCAN = 0,
	IO_CMD_RESUME_DM_BY_SCAN = 1,
};

enum hw_variables {
	HW_VAR_ETHER_ADDR,
	HW_VAR_MULTICAST_REG,
	HW_VAR_BASIC_RATE,
	HW_VAR_BSSID,
	HW_VAR_MEDIA_STATUS,
	HW_VAR_SECURITY_CONF,
	HW_VAR_BEACON_INTERVAL,
	HW_VAR_ATIM_WINDOW,
	HW_VAR_LISTEN_INTERVAL,
	HW_VAR_CS_COUNTER,
	HW_VAR_DEFAULTKEY0,
	HW_VAR_DEFAULTKEY1,
	HW_VAR_DEFAULTKEY2,
	HW_VAR_DEFAULTKEY3,
	HW_VAR_SIFS,
	HW_VAR_DIFS,
	HW_VAR_EIFS,
	HW_VAR_SLOT_TIME,
	HW_VAR_ACK_PREAMBLE,
	HW_VAR_CW_CONFIG,
	HW_VAR_CW_VALUES,
	HW_VAR_RATE_FALLBACK_CONTROL,
	HW_VAR_CONTENTION_WINDOW,
	HW_VAR_RETRY_COUNT,
	HW_VAR_TR_SWITCH,
	HW_VAR_COMMAND,
	HW_VAR_WPA_CONFIG,
	HW_VAR_AMPDU_MIN_SPACE,
	HW_VAR_SHORTGI_DENSITY,
	HW_VAR_AMPDU_FACTOR,
	HW_VAR_MCS_RATE_AVAILABLE,
	HW_VAR_AC_PARAM,
	HW_VAR_ACM_CTRL,
	HW_VAR_DIS_Req_Qsize,
	HW_VAR_CCX_CHNL_LOAD,
	HW_VAR_CCX_NOISE_HISTOGRAM,
	HW_VAR_CCX_CLM_NHM,
	HW_VAR_TxOPLimit,
	HW_VAR_TURBO_MODE,
	HW_VAR_RF_STATE,
	HW_VAR_RF_OFF_BY_HW,
	HW_VAR_BUS_SPEED,
	HW_VAR_SET_DEV_POWER,

	HW_VAR_RCR,
	HW_VAR_RATR_0,
	HW_VAR_RRSR,
	HW_VAR_CPU_RST,
	HW_VAR_CECHK_BSSID,
	HW_VAR_LBK_MODE,
	HW_VAR_AES_11N_FIX,
	HW_VAR_USB_RX_AGGR,
	HW_VAR_USER_CONTROL_TURBO_MODE,
	HW_VAR_RETRY_LIMIT,
	HW_VAR_INIT_TX_RATE,
	HW_VAR_TX_RATE_REG,
	HW_VAR_EFUSE_USAGE,
	HW_VAR_EFUSE_BYTES,
	HW_VAR_AUTOLOAD_STATUS,
	HW_VAR_RF_2R_DISABLE,
	HW_VAR_SET_RPWM,
	HW_VAR_H2C_FW_PWRMODE,
	HW_VAR_H2C_FW_JOINBSSRPT,
	HW_VAR_H2C_FW_P2P_PS_OFFLOAD,
	HW_VAR_FW_PSMODE_STATUS,
	HW_VAR_RESUME_CLK_ON,
	HW_VAR_FW_LPS_ACTION,
	HW_VAR_1X1_RECV_COMBINE,
	HW_VAR_STOP_SEND_BEACON,
	HW_VAR_TSF_TIMER,
	HW_VAR_IO_CMD,

	HW_VAR_RF_RECOVERY,
	HW_VAR_H2C_FW_UPDATE_GTK,
	HW_VAR_WF_MASK,
	HW_VAR_WF_CRC,
	HW_VAR_WF_IS_MAC_ADDR,
	HW_VAR_H2C_FW_OFFLOAD,
	HW_VAR_RESET_WFCRC,

	HW_VAR_HANDLE_FW_C2H,
	HW_VAR_DL_FW_RSVD_PAGE,
	HW_VAR_AID,
	HW_VAR_HW_SEQ_ENABLE,
	HW_VAR_CORRECT_TSF,
	HW_VAR_BCN_VALID,
	HW_VAR_FWLPS_RF_ON,
	HW_VAR_DUAL_TSF_RST,
	HW_VAR_SWITCH_EPHY_WoWLAN,
	HW_VAR_INT_MIGRATION,
	HW_VAR_INT_AC,
	HW_VAR_RF_TIMING,
	
	HAL_DEF_WOWLAN,
	HW_VAR_MRC,
};

enum rt_media_status {
	RT_MEDIA_DISCONNECT = 0,
	RT_MEDIA_CONNECT = 1
};

enum rt_oem_id {
	RT_CID_DEFAULT = 0,
	RT_CID_8187_ALPHA0 = 1,
	RT_CID_8187_SERCOMM_PS = 2,
	RT_CID_8187_HW_LED = 3,
	RT_CID_8187_NETGEAR = 4,
	RT_CID_WHQL = 5,
	RT_CID_819x_CAMEO = 6,
	RT_CID_819x_RUNTOP = 7,
	RT_CID_819x_Senao = 8,
	RT_CID_TOSHIBA = 9,
	RT_CID_819x_Netcore = 10,
	RT_CID_Nettronix = 11,
	RT_CID_DLINK = 12,
	RT_CID_PRONET = 13,
	RT_CID_COREGA = 14,
	RT_CID_819x_ALPHA = 15,
	RT_CID_819x_Sitecom = 16,
	RT_CID_CCX = 17,
	RT_CID_819x_Lenovo = 18,
	RT_CID_819x_QMI = 19,
	RT_CID_819x_Edimax_Belkin = 20,
	RT_CID_819x_Sercomm_Belkin = 21,
	RT_CID_819x_CAMEO1 = 22,
	RT_CID_819x_MSI = 23,
	RT_CID_819x_Acer = 24,
	RT_CID_819x_HP = 27,
	RT_CID_819x_CLEVO = 28,
	RT_CID_819x_Arcadyan_Belkin = 29,
	RT_CID_819x_SAMSUNG = 30,
	RT_CID_819x_WNC_COREGA = 31,
	RT_CID_819x_Foxcoon = 32,
	RT_CID_819x_DELL = 33,
	RT_CID_819x_PRONETS = 34,
	RT_CID_819x_Edimax_ASUS = 35,
	RT_CID_NETGEAR = 36,
	RT_CID_PLANEX = 37,
	RT_CID_CC_C = 38,
};

enum hw_descs {
	HW_DESC_OWN,
	HW_DESC_RXOWN,
	HW_DESC_TX_NEXTDESC_ADDR,
	HW_DESC_TXBUFF_ADDR,
	HW_DESC_RXBUFF_ADDR,
	HW_DESC_RXPKT_LEN,
	HW_DESC_RXERO,
};

enum prime_sc {
	PRIME_CHNL_OFFSET_DONT_CARE = 0,
	PRIME_CHNL_OFFSET_LOWER = 1,
	PRIME_CHNL_OFFSET_UPPER = 2,
};

enum rf_type {
	RF_1T1R = 0,
	RF_1T2R = 1,
	RF_2T2R = 2,
	RF_2T2R_GREEN = 3,
};

enum ht_channel_width {
	HT_CHANNEL_WIDTH_20 = 0,
	HT_CHANNEL_WIDTH_20_40 = 1,
};

/* Ref: 802.11i sepc D10.0 7.3.2.25.1
Cipher Suites Encryption Algorithms */
enum rt_enc_alg {
	NO_ENCRYPTION = 0,
	WEP40_ENCRYPTION = 1,
	TKIP_ENCRYPTION = 2,
	RSERVED_ENCRYPTION = 3,
	AESCCMP_ENCRYPTION = 4,
	WEP104_ENCRYPTION = 5,
	AESCMAC_ENCRYPTION = 6,	/*IEEE802.11w */
};

enum rtl_hal_state {
	_HAL_STATE_STOP = 0,
	_HAL_STATE_START = 1,
};

enum rtl_var_map {
	/*reg map */
	SYS_ISO_CTRL = 0,
	SYS_FUNC_EN,
	SYS_CLK,
	MAC_RCR_AM,
	MAC_RCR_AB,
	MAC_RCR_ACRC32,
	MAC_RCR_ACF,
	MAC_RCR_AAP,

	/*efuse map */
	EFUSE_TEST,
	EFUSE_CTRL,
	EFUSE_CLK,
	EFUSE_CLK_CTRL,
	EFUSE_PWC_EV12V,
	EFUSE_FEN_ELDR,
	EFUSE_LOADER_CLK_EN,
	EFUSE_ANA8M,
	EFUSE_HWSET_MAX_SIZE,
	EFUSE_MAX_SECTION_MAP,
	EFUSE_REAL_CONTENT_SIZE,
	EFUSE_OOB_PROTECT_BYTES_LEN,
	EFUSE_ACCESS,	
	/*CAM map */
	RWCAM,
	WCAMI,
	RCAMO,
	CAMDBG,
	SECR,
	SEC_CAM_NONE,
	SEC_CAM_WEP40,
	SEC_CAM_TKIP,
	SEC_CAM_AES,
	SEC_CAM_WEP104,

	/*IMR map */
	RTL_IMR_BCNDMAINT6,	/*Beacon DMA Interrupt 6 */
	RTL_IMR_BCNDMAINT5,	/*Beacon DMA Interrupt 5 */
	RTL_IMR_BCNDMAINT4,	/*Beacon DMA Interrupt 4 */
	RTL_IMR_BCNDMAINT3,	/*Beacon DMA Interrupt 3 */
	RTL_IMR_BCNDMAINT2,	/*Beacon DMA Interrupt 2 */
	RTL_IMR_BCNDMAINT1,	/*Beacon DMA Interrupt 1 */
	RTL_IMR_BCNDOK8,	/*Beacon Queue DMA OK Interrup 8 */
	RTL_IMR_BCNDOK7,	/*Beacon Queue DMA OK Interrup 7 */
	RTL_IMR_BCNDOK6,	/*Beacon Queue DMA OK Interrup 6 */
	RTL_IMR_BCNDOK5,	/*Beacon Queue DMA OK Interrup 5 */
	RTL_IMR_BCNDOK4,	/*Beacon Queue DMA OK Interrup 4 */
	RTL_IMR_BCNDOK3,	/*Beacon Queue DMA OK Interrup 3 */
	RTL_IMR_BCNDOK2,	/*Beacon Queue DMA OK Interrup 2 */
	RTL_IMR_BCNDOK1,	/*Beacon Queue DMA OK Interrup 1 */
	RTL_IMR_TIMEOUT2,	/*Timeout interrupt 2 */
	RTL_IMR_TIMEOUT1,	/*Timeout interrupt 1 */
	RTL_IMR_TXFOVW,		/*Transmit FIFO Overflow */
	RTL_IMR_PSTIMEOUT,	/*Power save time out interrupt */
	RTL_IMR_BcnInt,		/*Beacon DMA Interrupt 0 */
	RTL_IMR_RXFOVW,		/*Receive FIFO Overflow */
	RTL_IMR_RDU,		/*Receive Descriptor Unavailable */
	RTL_IMR_ATIMEND,	/*For 92C,ATIM Window End Interrupt */
	RTL_IMR_BDOK,		/*Beacon Queue DMA OK Interrup */
	RTL_IMR_HIGHDOK,	/*High Queue DMA OK Interrupt */
	RTL_IMR_COMDOK,		/*Command Queue DMA OK Interrupt*/
	RTL_IMR_TBDOK,		/*Transmit Beacon OK interrup */
	RTL_IMR_MGNTDOK,	/*Management Queue DMA OK Interrupt */
	RTL_IMR_TBDER,		/*For 92C,Transmit Beacon Error Interrupt */
	RTL_IMR_BKDOK,		/*AC_BK DMA OK Interrupt */
	RTL_IMR_BEDOK,		/*AC_BE DMA OK Interrupt */
	RTL_IMR_VIDOK,		/*AC_VI DMA OK Interrupt */
	RTL_IMR_VODOK,		/*AC_VO DMA Interrupt */
	RTL_IMR_ROK,		/*Receive DMA OK Interrupt */
	RTL_IBSS_INT_MASKS,	/*(RTL_IMR_BcnInt | RTL_IMR_TBDOK | RTL_IMR_TBDER) */
	RTL_IMR_C2HCMD,		/*fw interrupt*/

	/*CCK Rates, TxHT = 0 */
	RTL_RC_CCK_RATE1M,
	RTL_RC_CCK_RATE2M,
	RTL_RC_CCK_RATE5_5M,
	RTL_RC_CCK_RATE11M,

	/*OFDM Rates, TxHT = 0 */
	RTL_RC_OFDM_RATE6M,
	RTL_RC_OFDM_RATE9M,
	RTL_RC_OFDM_RATE12M,
	RTL_RC_OFDM_RATE18M,
	RTL_RC_OFDM_RATE24M,
	RTL_RC_OFDM_RATE36M,
	RTL_RC_OFDM_RATE48M,
	RTL_RC_OFDM_RATE54M,

	RTL_RC_HT_RATEMCS7,
	RTL_RC_HT_RATEMCS15,

	/*keep it last */
	RTL_VAR_MAP_MAX,
};

/*Firmware PS mode for control LPS.*/
enum _fw_ps_mode {
	FW_PS_ACTIVE_MODE = 0,
	FW_PS_MIN_MODE = 1,
	FW_PS_MAX_MODE = 2,
	FW_PS_DTIM_MODE = 3,
	FW_PS_VOIP_MODE = 4,
	FW_PS_UAPSD_WMM_MODE = 5,
	FW_PS_UAPSD_MODE = 6,
	FW_PS_IBSS_MODE = 7,
	FW_PS_WWLAN_MODE = 8,
	FW_PS_PM_Radio_Off = 9,
	FW_PS_PM_Card_Disable = 10,
};

enum rt_psmode {
	EACTIVE,		/*Active/Continuous access. */
	EMAXPS,			/*Max power save mode. */
	EFASTPS,		/*Fast power save mode. */
	EAUTOPS,		/*Auto power save mode. */
};

/*LED related.*/
enum led_ctl_mode {
	LED_CTL_POWER_ON = 1,
	LED_CTL_LINK = 2,
	LED_CTL_NO_LINK = 3,
	LED_CTL_TX = 4,
	LED_CTL_RX = 5,
	LED_CTL_SITE_SURVEY = 6,
	LED_CTL_POWER_OFF = 7,
	LED_CTL_START_TO_LINK = 8,
	LED_CTL_START_WPS = 9,
	LED_CTL_STOP_WPS = 10,
};

enum rtl_led_pin {
	LED_PIN_GPIO0,
	LED_PIN_LED0,
	LED_PIN_LED1,
	LED_PIN_LED2
};

/*QoS related.*/
/*acm implementation method.*/
enum acm_method {
	eAcmWay0_SwAndHw = 0,
	eAcmWay1_HW = 1,
	eAcmWay2_SW = 2,
};

enum macphy_mode {
	SINGLEMAC_SINGLEPHY = 0,
	DUALMAC_DUALPHY,
	DUALMAC_SINGLEPHY,
};

enum band_type {
	BAND_ON_2_4G = 0,
	BAND_ON_5G,
	BAND_ON_BOTH,
	BANDMAX
};

/*aci/aifsn Field.
Ref: WMM spec 2.2.2: WME Parameter Element, p.12.*/
union aci_aifsn {
	u8 char_data;

	struct {
		u8 aifsn:4;
		u8 acm:1;
		u8 aci:2;
		u8 reserved:1;
	} f;			/* Field */
};

/*mlme related.*/
enum wireless_mode {
	WIRELESS_MODE_UNKNOWN = 0x00,
	WIRELESS_MODE_A = 0x01,
	WIRELESS_MODE_B = 0x02,
	WIRELESS_MODE_G = 0x04,
	WIRELESS_MODE_AUTO = 0x08,
	WIRELESS_MODE_N_24G = 0x10,
	WIRELESS_MODE_N_5G = 0x20
};

enum ratr_table_mode {
	RATR_INX_WIRELESS_NGB = 0,
	RATR_INX_WIRELESS_NG = 1,
	RATR_INX_WIRELESS_NB = 2,
	RATR_INX_WIRELESS_N = 3,
	RATR_INX_WIRELESS_GB = 4,
	RATR_INX_WIRELESS_G = 5,
	RATR_INX_WIRELESS_B = 6,
	RATR_INX_WIRELESS_MC = 7,
	RATR_INX_WIRELESS_A = 8,
};

enum rtl_link_state {
	MAC80211_NOLINK = 0,
	MAC80211_LINKING = 1,
	MAC80211_LINKED = 2,
	MAC80211_LINKED_SCANNING = 3,
};

enum act_category {
	ACT_CAT_QOS = 1,
	ACT_CAT_DLS = 2,
	ACT_CAT_BA = 3,
	ACT_CAT_HT = 7,
	ACT_CAT_WMM = 17,
};

enum ba_action {
	ACT_ADDBAREQ = 0,
	ACT_ADDBARSP = 1,
	ACT_DELBA = 2,
};

enum rt_polarity_ctl {
	RT_POLARITY_LOW_ACT = 0,
	RT_POLARITY_HIGH_ACT = 1,	
};


struct octet_string {
	u8 *octet;
	u16 length;
};

struct rtl_hdr_3addr {
	__le16 frame_ctl;
	__le16 duration_id;
	u8 addr1[ETH_ALEN];
	u8 addr2[ETH_ALEN];
	u8 addr3[ETH_ALEN];
	__le16 seq_ctl;
	u8 payload[0];
} __packed;

struct rtl_info_element {
	u8 id;
	u8 len;
	u8 data[0];
} __packed;

struct rtl_probe_rsp {
	struct rtl_hdr_3addr header;
	u32 time_stamp[2];
	__le16 beacon_interval;
	__le16 capability;
	/*SSID, supported rates, FH params, DS params,
	   CF params, IBSS params, TIM (if beacon), RSN */
	struct rtl_info_element info_element[0];
} __packed;

/*LED related.*/
/*ledpin Identify how to implement this SW led.*/
struct rtl_led {
	void *hw;
	enum rtl_led_pin ledpin;
	bool b_ledon;
};

struct rtl_led_ctl {
	bool bled_opendrain;
	struct rtl_led sw_led0;
	struct rtl_led sw_led1;
};

struct rtl_qos_parameters {
	__le16 cw_min;
	__le16 cw_max;
	u8 aifs;
	u8 flag;
	__le16 tx_op;
} __packed;

struct rt_smooth_data {
	u32 elements[100];	/*array to store values */
	u32 index;		/*index to current array to store */
	u32 total_num;		/*num of valid elements */
	u32 total_val;		/*sum of valid elements */
};

struct rtl_ht_agg {
	u16 txq_id;
	u16 wait_for_ba;
	u16 start_idx;
	u64 bitmap;
	u32 rate_n_flags;
	u8 agg_state;
	u8 rx_agg_state;
};

struct rtl_tid_data {
	u16 seq_number;
	struct rtl_ht_agg agg;
};

struct rssi_sta{
	long undecorated_smoothed_pwdb;
};

struct rtl_sta_info {
	struct list_head list;
	u8 ratr_index;
	u8 wireless_mode;
	u8 mimo_ps;
	u8 mac_addr[6];
	struct rtl_tid_data tids[MAX_TID_COUNT];

	/* just used for ap adhoc or mesh*/
	struct rssi_sta rssi_stat;
} __packed;

#ifdef VIF_TODO
struct rtl_vif {
	unsigned int id;
	/* struct ieee80211_vif __rcu *vif; */
	struct ieee80211_vif *vif;
};

struct rtl_vif_info {
	struct list_head list;
	bool active;
	unsigned int id;
	struct sk_buff *beacon;
	bool enable_beacon;
};

struct vif_priv {
	struct list_head vif_list;

	/* interface mode settings */
	unsigned long vif_bitmap;
	unsigned int vifs;
	struct rtl_vif vif[MAX_VIRTUAL_MAC];

	/* beaconing */
	spinlock_t beacon_lock;
	unsigned int global_pretbtt;
	unsigned int global_beacon_int;
	/* struct rtl_vif_info __rcu *beacon_iter; */
	struct rtl_vif_info *beacon_iter;
	unsigned int beacon_enabled;
};
#endif

struct false_alarm_statistics {
	u32 cnt_parity_fail;
	u32 cnt_rate_illegal;
	u32 cnt_crc8_fail;
	u32 cnt_mcs_fail;
	u32 cnt_fast_fsync_fail;
	u32 cnt_sb_search_fail;
	u32 cnt_ofdm_fail;
	u32 cnt_cck_fail;
	u32 cnt_all;
	u32 cnt_ofdm_cca;
	u32 cnt_cck_cca;
	u32 cnt_cca_all;
	u32 cnt_bw_usc;
	u32 cnt_bw_lsc;
};

struct init_gain {
	u8 xaagccore1;
	u8 xbagccore1;
	u8 xcagccore1;
	u8 xdagccore1;
	u8 cca;

};

struct wireless_stats {
	unsigned long txbytesunicast;
	unsigned long txbytesmulticast;
	unsigned long txbytesbroadcast;
	unsigned long rxbytesunicast;

	long rx_snr_db[4];
	/*Correct smoothed ss in Dbm, only used
	   in driver to report real power now. */
	long recv_signal_power;
	long signal_quality;
	long last_sigstrength_inpercent;

	u32 rssi_calculate_cnt;

	/*Transformed, in dbm. Beautified signal
	   strength for UI, not correct. */
	long signal_strength;

	u8 rx_rssi_percentage[4];
	u8 rx_evm_percentage[2];

	struct rt_smooth_data ui_rssi;
	struct rt_smooth_data ui_link_quality;
};

struct rate_adaptive {
	u8 rate_adaptive_disabled;
	u8 ratr_state;
	u16 reserve;

	u32 high_rssi_thresh_for_ra;
	u32 high2low_rssi_thresh_for_ra;
	u8 low2high_rssi_thresh_for_ra40m;
	u32 low_rssi_thresh_for_ra40M;
	u8 low2high_rssi_thresh_for_ra20m;
	u32 low_rssi_thresh_for_ra20M;
	u32 upper_rssi_threshold_ratr;
	u32 middleupper_rssi_threshold_ratr;
	u32 middle_rssi_threshold_ratr;
	u32 middlelow_rssi_threshold_ratr;
	u32 low_rssi_threshold_ratr;
	u32 ultralow_rssi_threshold_ratr;
	u32 low_rssi_threshold_ratr_40m;
	u32 low_rssi_threshold_ratr_20m;
	u8 ping_rssi_enable;
	u32 ping_rssi_ratr;
	u32 ping_rssi_thresh_for_ra;
	u32 last_ratr;
	u8 pre_ratr_state;
};

struct regd_pair_mapping {
	u16 reg_dmnenum;
	u16 reg_5ghz_ctl;
	u16 reg_2ghz_ctl;
};

struct rtl_regulatory {
	char alpha2[2];
	u16 country_code;
	u16 max_power_level;
	u32 tp_scale;
	u16 current_rd;
	u16 current_rd_ext;
	int16_t power_limit;
	struct regd_pair_mapping *regpair;
};

struct rtl_rfkill {
	bool rfkill_state;	/*0 is off, 1 is on */
};

/*for P2P PS**/
#define	P2P_MAX_NOA_NUM		2

enum p2p_role {
	P2P_ROLE_DISABLE = 0,
	P2P_ROLE_DEVICE = 1,
	P2P_ROLE_CLIENT = 2,
	P2P_ROLE_GO = 3	
};

enum p2p_ps_state {
	P2P_PS_DISABLE = 0,
	P2P_PS_ENABLE = 1,
	P2P_PS_SCAN = 2,
	P2P_PS_SCAN_DONE = 3,
	P2P_PS_ALLSTASLEEP = 4, // for P2P GO
};

enum p2p_ps_mode {
	P2P_PS_NONE = 0,
	P2P_PS_CTWINDOW = 1,
	P2P_PS_NOA	 = 2,
	P2P_PS_MIX = 3, // CTWindow and NoA
};

struct rtl_p2p_ps_info {
	enum p2p_ps_mode p2p_ps_mode; /* indicate p2p ps mode */
	enum p2p_ps_state p2p_ps_state; /*  indicate p2p ps state */
	u8 noa_index; /*  Identifies and instance of Notice of Absence timing. */
	u8 ctwindow; /*  Client traffic window. A period of time in TU after TBTT. */
	u8 opp_ps; /*  opportunistic power save. */
	u8 noa_num; /*  number of NoA descriptor in P2P IE. */
	u8 noa_count_type[P2P_MAX_NOA_NUM]; /*  Count for owner, Type of client. */
	u32 noa_duration[P2P_MAX_NOA_NUM]; /*  Max duration for owner, preferred or min acceptable duration for client. */
	u32 noa_interval[P2P_MAX_NOA_NUM]; /*  Length of interval for owner, preferred or max acceptable interval of client. */
	u32 noa_start_time[P2P_MAX_NOA_NUM]; /*  schedule expressed in terms of the lower 4 bytes of the TSF timer. */
};

 struct p2p_ps_offload_t {
	u8 Offload_En:1;
	u8 role:1; /* 1: Owner, 0: Client */
	u8 CTWindow_En:1;
	u8 NoA0_En:1;
	u8 NoA1_En:1;
	u8 AllStaSleep:1;
	u8 discovery:1;
	u8 reserved:1;
};
 
#define IQK_MATRIX_REG_NUM	8
#define IQK_MATRIX_SETTINGS_NUM	(1 + 24 + 21)
struct iqk_matrix_regs {
	bool b_iqk_done;
	long value[1][IQK_MATRIX_REG_NUM];
};

struct rtl_phy {
	struct bb_reg_def phyreg_def[4];	/*Radio A/B/C/D */
	struct init_gain initgain_backup;
	enum io_type current_io_type;

	u8 rf_mode;
	u8 rf_type;
	u8 current_chan_bw;
	u8 set_bwmode_inprogress;
	u8 sw_chnl_inprogress;
	u8 sw_chnl_stage;
	u8 sw_chnl_step;
	u8 current_channel;
	u8 h2c_box_num;
	u8 set_io_inprogress;
	u8 lck_inprogress;

	/* record for power tracking */
	s32 reg_e94;
	s32 reg_e9c;
	s32 reg_ea4;
	s32 reg_eac;
	s32 reg_eb4;
	s32 reg_ebc;
	s32 reg_ec4;
	s32 reg_ecc;
	u8 rfpienable;
	u8 reserve_0;
	u16 reserve_1;
	u32 reg_c04, reg_c08, reg_874;
	u32 adda_backup[16];
	u32 iqk_mac_backup[IQK_MAC_REG_NUM];
	u32 iqk_bb_backup[10];
	bool iqk_initialized;

	/* Dul mac */
	bool b_need_iqk;
	struct iqk_matrix_regs iqk_matrix_regsetting[IQK_MATRIX_SETTINGS_NUM];

	bool b_rfpi_enable;

	u8 pwrgroup_cnt;
	u8 bcck_high_power;
	/* 3 gropus of pwr diff by rates */
	u32 mcs_txpwrlevel_origoffset[MAX_PG_GROUP][16];
	u8 default_initialgain[4];

	/* the current Tx power level */
	u8 cur_cck_txpwridx;
	u8 cur_ofdm24g_txpwridx;
	u8 cur_bw20_txpwridx;
	u8 cur_bw40_txpwridx;
	
	u32 rfreg_chnlval[2];
	bool b_apk_done;
	u32 reg_rf3c[2];	/* pathA / pathB  */

	/* bfsync */
	u8 framesync;
	u32 framesync_c34;

	u8 num_total_rfpath;
	u16 rf_pathmap;
	
	enum rt_polarity_ctl polarity_ctl;
};

#define RTL_AGG_STOP 						0
#define RTL_AGG_PROGRESS					1
#define RTL_AGG_START 						2
#define RTL_AGG_OPERATIONAL					3
#define RTL_RX_AGG_START 					1	
#define RTL_RX_AGG_STOP 					0

struct rtl_priv;
struct rtl_io {
	struct device *dev;

	/*PCI MEM map */
	unsigned long pci_mem_end;	/*shared mem end        */
	unsigned long pci_mem_start;	/*shared mem start */

	/*PCI IO map */
	unsigned long pci_base_addr;	/*device I/O address */

	void (*write8_async) (struct rtl_priv * rtlpriv, u32 addr, u8 val);
	void (*write16_async) (struct rtl_priv * rtlpriv, u32 addr, u16 val);
	void (*write32_async) (struct rtl_priv * rtlpriv, u32 addr, u32 val);

	u8(*read8_sync) (struct rtl_priv * rtlpriv, u32 addr);
	u16(*read16_sync) (struct rtl_priv * rtlpriv, u32 addr);
	u32(*read32_sync) (struct rtl_priv * rtlpriv, u32 addr);

};

struct rtl_mac {
	u8 mac_addr[ETH_ALEN];
	u8 mac80211_registered;
	u8 beacon_enabled;

	u32 tx_ss_num;
	u32 rx_ss_num;

	struct ieee80211_supported_band bands[IEEE80211_NUM_BANDS];
	struct ieee80211_hw *hw;
	struct ieee80211_vif *vif;
	enum nl80211_iftype opmode;

	/*Probe Beacon management */
	enum rtl_link_state link_state;

	int n_channels;
	int n_bitrates;

	bool offchan_deley;
	u8 p2p;	/*using p2p role*/
	bool p2p_in_use;

	/*filters */
	u32 rx_conf;

	bool act_scanning;
	u8 cnt_after_linked;
	bool skip_scan;
	
	/* early mode */
	/* skb wait queue */
	struct sk_buff_head skb_waitq[MAX_TID_COUNT];

	/*RDG*/
	bool rdg_en;

	/*AP*/
	u8 bssid[6];
	u32 vendor;
	u32 basic_rates; /* b/g rates */
	u8 ht_enable;
	u8 bw_40;
	u8 mode;		/* wireless mode */
	u8 slot_time;
	u8 short_preamble;
	u8 use_cts_protect;
	u8 cur_40_prime_sc;
	u8 cur_40_prime_sc_bk;
	u64 tsf;
	u8 retry_short;
	u8 retry_long;
	u16 assoc_id;
	bool bhiddenssid;
	
	/*IBSS*/
	int beacon_interval;

	/*AMPDU*/
	u8 min_space_cfg;	/*For Min spacing configurations */
	u8 max_mss_density;
	u8 current_ampdu_factor;
	u8 current_ampdu_density;

	/*QOS & EDCA */
	struct ieee80211_tx_queue_params edca_param[RTL_MAC80211_NUM_QUEUE];
	struct rtl_qos_parameters ac[AC_MAX];
};

struct rtl_hal {
	struct ieee80211_hw *hw;

	bool driver_is_goingto_unload;
	bool up_first_time;
	bool bfirst_init;
	bool being_init_adapter;
	bool b_bbrf_ready;
	bool b_mac_func_enable;

	enum intf_type interface;
	u16 hw_type;		/*92c or 92d or 92s and so on */
	u8 ic_class;
	u8 oem_id;
	u32 version;		/*version of chip */
	u8 state;		/*stop 0, start 1 */
	u8 boad_type;

	/*firmware */
	u32 fwsize;
	u8 *pfirmware;
	u16 fw_version;
	u16 fw_subversion;
	bool b_h2c_setinprogress;
	u8 last_hmeboxnum;
	bool bfw_ready;

	/*Reserve page start offset except beacon in TxQ. */
	u8 fw_rsvdpage_startoffset;
	u8 h2c_txcmd_seq;

	/* FW Cmd IO related */
	u16 fwcmd_iomap;
	u32 fwcmd_ioparam;
	bool set_fwcmd_inprogress;
	u8 current_fwcmd_io;

	bool bfw_clk_change_in_progress;
	bool ballow_sw_to_change_hwclc;
	u8 fw_ps_state;
	 struct p2p_ps_offload_t p2p_ps_offload;
	/**/
	bool driver_going2unload;

	/*AMPDU init min space*/
	u8 minspace_cfg;	/*For Min spacing configurations */

	/* Dul mac */
	enum macphy_mode macphymode;
	enum band_type current_bandtype;	/* 0:2.4G, 1:5G */
	enum band_type current_bandtypebackup;
	enum band_type bandset;
	/* dual MAC 0--Mac0 1--Mac1 */
	u32 interfaceindex;
	/* just for DulMac S3S4 */
	u8 macphyctl_reg;
	bool b_earlymode_enable;
	u8 max_earlymode_num;
	/* Dul mac*/
	bool during_mac0init_radiob;
	bool during_mac1init_radioa;
	bool reloadtxpowerindex;
	/* True if IMR or IQK  have done
	for 2.4G in scan progress */
	bool b_load_imrandiqk_setting_for2g;

	bool disable_amsdu_8k;
	bool bmaster_of_dmsp;
	bool bslave_of_dmsp;
};

struct rtl_security {
	/*default 0 */
	bool use_sw_sec;

	bool being_setkey;
	bool use_defaultkey;
	/*Encryption Algorithm for Unicast Packet */
	enum rt_enc_alg pairwise_enc_algorithm;
	/*Encryption Algorithm for Brocast/Multicast */
	enum rt_enc_alg group_enc_algorithm;
	/*Cam Entry Bitmap */
	u32 hwsec_cam_bitmap;
	u8 hwsec_cam_sta_addr[TOTAL_CAM_ENTRY][ETH_ALEN];
	/*local Key buffer, indx 0 is for
	   pairwise key 1-4 is for agoup key. */
	u8 key_buf[KEY_BUF_SIZE][MAX_KEY_LEN];
	u8 key_len[KEY_BUF_SIZE];

	/*The pointer of Pairwise Key,
	   it always points to KeyBuf[4] */
	u8 *pairwise_key;
};

struct rtl_dig {
	u8 dig_enable_flag;
	u8 dig_ext_port_stage;

	u32 rssi_lowthresh;
	u32 rssi_highthresh;

	u32 fa_lowthresh;
	u32 fa_highthresh;

	u8 cursta_connectstate;
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
	u8 min_undecorated_pwdb_for_dm;
	long last_min_undecorated_pwdb_for_dm;

	u8 pre_cck_pd_state;
	u8 cur_cck_pd_state;

	u8 large_fa_hit;
	u8 forbidden_igi;
	u32 recover_cnt;

};

struct rtl_pstbl {
	u8 pre_ccastate;
	u8 cur_ccasate;

	u8 pre_rfstate;
	u8 cur_rfstate;

	long rssi_val_min;

};

#define ASSOCIATE_ENTRY_NUM	32+1

struct fast_ant_trainning{
	u8	bssid[6];
	u8	antsel_rx_keep_0;
	u8	antsel_rx_keep_1;
	u8	antsel_rx_keep_2;
	u32	ant_sum_rssi[7];
	u32	ant_rssi_cnt[7];
	u32	ant_ave_rssi[7];
	u8	fat_state;
	u32	train_idx;
	u8	antsel_a[ASSOCIATE_ENTRY_NUM];
	u8	antsel_b[ASSOCIATE_ENTRY_NUM];
	u8	antsel_c[ASSOCIATE_ENTRY_NUM];
	u32	main_ant_sum[ASSOCIATE_ENTRY_NUM];
	u32	aux_ant_sum[ASSOCIATE_ENTRY_NUM];
	u32	main_ant_cnt[ASSOCIATE_ENTRY_NUM];
	u32	aux_ant_cnt[ASSOCIATE_ENTRY_NUM];
	u8	rx_idle_ant;
	bool	b_becomelinked;
};

struct rtl_dm {
	/*PHY status for DM */
	long entry_min_undecoratedsmoothed_pwdb;
	long undecorated_smoothed_pwdb;	/*out dm */
	long entry_max_undecoratedsmoothed_pwdb;
	bool b_dm_initialgain_enable;
	bool bdynamic_txpower_enable;
	bool bcurrent_turbo_edca;
	bool bis_any_nonbepkts;	/*out dm */
	bool bis_cur_rdlstate;
	bool btxpower_trackinginit;
	bool b_disable_framebursting;
	bool b_cck_inch14;
	bool btxpower_tracking;
	bool b_useramask;
	bool brfpath_rxenable[4];
	bool binform_fw_driverctrldm;
	bool bcurrent_mrc_switch;
	u8 txpowercount;

	u8 thermalvalue_rxgain;
	u8 thermalvalue_iqk;
	u8 thermalvalue_lck;
	u8 thermalvalue;
	u8 thermalvalue_avg[AVG_THERMAL_NUM];
	u8 thermalvalue_avg_index;
	bool bdone_txpower;
	u8 last_dtp_lvl;
	u8 dynamic_txhighpower_lvl;	/*Tx high power level */
	u8 dm_flag;		/*Indicate if each dynamic mechanism's status. */
	u8 dm_type;
	u8 txpower_track_control;
	bool binterrupt_migration;
	bool bdisable_tx_int;
	char ofdm_index[2];
	char cck_index;
	char	delta_power_index;
	char	delta_power_index_last;
	char power_index_offset;
	
	/*88e tx power tracking*/
	u8	bb_swing_idx_ofdm[2];
	u8	bb_swing_idx_ofdm_current;
	u8	bb_swing_idx_ofdm_base;
	bool	bb_swing_flag_Ofdm;
	u8	bb_swing_idx_cck;
	u8	bb_swing_idx_cck_current;
	u8	bb_swing_idx_cck_base;
	bool	bb_swing_flag_cck;

	
	/* DMSP */
	bool supp_phymode_switch;

	/* DulMac */
	struct rtl_dig dm_digtable;
	struct rtl_pstbl dm_pstable;
	struct fast_ant_trainning fat_table;
};

#define	EFUSE_MAX_LOGICAL_SIZE		256

struct rtl_efuse {
	bool bautoLoad_ok;
	bool bootfromefuse;
	u16 max_physical_size;

	u8 efuse_map[2][EFUSE_MAX_LOGICAL_SIZE];
	u16 efuse_usedbytes;
	u8 efuse_usedpercentage;
#ifdef EFUSE_REPG_WORKAROUND
	bool efuse_re_pg_sec1flag;
	u8 efuse_re_pg_data[8];
#endif

	u8 autoload_failflag;
	u8 autoload_status;

	short epromtype;
	u16 eeprom_vid;
	u16 eeprom_did;
	u16 eeprom_svid;
	u16 eeprom_smid;
	u8 eeprom_oemid;
	u16 eeprom_channelplan;
	u8 eeprom_version;

	u8 dev_addr[6];
	u8 board_type;
	u8 wowlan_enable;
	u8 antenna_div_cfg;
	u8 antenna_div_type;
	
	bool b_txpwr_fromeprom;
	u8 eeprom_crystalcap;
	u8 eeprom_tssi[2];
	u8 eeprom_tssi_5g[3][2]; /* for 5GL/5GM/5GH band. */
	u8 eeprom_pwrlimit_ht20[CHANNEL_GROUP_MAX];
	u8 eeprom_pwrlimit_ht40[CHANNEL_GROUP_MAX];
	u8 eeprom_chnlarea_txpwr_cck[2][CHANNEL_GROUP_MAX_2G];
	u8 eeprom_chnlarea_txpwr_ht40_1s[2][CHANNEL_GROUP_MAX];
	u8 eeprom_chnlarea_txpwr_ht40_2sdiif[2][CHANNEL_GROUP_MAX];
	u8 txpwrlevel_cck[2][CHANNEL_MAX_NUMBER_2G];
	u8 txpwrlevel_ht40_1s[2][CHANNEL_MAX_NUMBER];	/*For HT 40MHZ pwr */
	u8 txpwrlevel_ht40_2s[2][CHANNEL_MAX_NUMBER];	/*For HT 40MHZ pwr */

	u8 internal_pa_5g[2];	/* pathA / pathB */
	u8 eeprom_c9;
	u8 eeprom_cc;

	/*For power group */
	u8 eeprom_pwrgroup[2][3];
	u8 pwrgroup_ht20[2][CHANNEL_MAX_NUMBER];
	u8 pwrgroup_ht40[2][CHANNEL_MAX_NUMBER];

	char txpwr_ht20diff[2][CHANNEL_MAX_NUMBER];	/*HT 20<->40 Pwr diff */
	u8 txpwr_legacyhtdiff[2][CHANNEL_MAX_NUMBER];	/*For HT<->legacy pwr diff */
	u8 txpwr_safetyflag;			/* Band edge enable flag */
	u16 eeprom_txpowerdiff;
	u8 legacy_httxpowerdiff;			/* Legacy to HT rate power diff */
	u8 antenna_txpwdiff[3];

	u8 eeprom_regulatory;
	u8 eeprom_thermalmeter;
	u8 thermalmeter[2];	/*ThermalMeter, index 0 for RFIC0, and 1 for RFIC1 */
	u16 tssi_13dbm;
	u8 crystalcap;		/* CrystalCap. */
	u8 delta_iqk;
	u8 delta_lck;

	u8 legacy_ht_txpowerdiff;	/*Legacy to HT rate power diff */
	bool b_apk_thermalmeterignore;

	bool b1x1_recvcombine;
	bool b1ss_support;

	/*channel plan */
	u8 channel_plan;
};

struct rtl_ps_ctl {
	bool pwrdomain_protect;
	bool b_in_powersavemode;
	bool rfchange_inprogress;
	bool b_swrf_processing;
	bool b_hwradiooff;
	/*
	 * just for PCIE ASPM
	 * If it supports ASPM, Offset[560h] = 0x40,
	 * otherwise Offset[560h] = 0x00.
	 * */
	bool b_support_aspm;
	bool b_support_backdoor;

	/*for LPS */
	enum rt_psmode dot11_psmode;	/*Power save mode configured. */
	bool b_swctrl_lps;
	bool b_fwctrl_lps;
	u8 fwctrl_psmode;
	/*For Fw control LPS mode */
	u8 b_reg_fwctrl_lps;
	/*Record Fw PS mode status. */
	bool b_fw_current_inpsmode;
	u8 reg_max_lps_awakeintvl;
	bool report_linked;
	bool b_low_power_enable;/*for 32k*/
	
	/*for IPS */
	bool b_inactiveps;

	u32 rfoff_reason;

	/*RF OFF Level */
	u32 cur_ps_level;
	u32 reg_rfps_level;

	/*just for PCIE ASPM */
	u8 const_amdpci_aspm;

	enum rf_pwrstate inactive_pwrstate;
	enum rf_pwrstate rfpwr_state;	/*cur power state */

	/* for SW LPS*/
	bool sw_ps_enabled;
	bool state;
	bool state_inap;
	bool multi_buffered;
	u16 nullfunc_seq;
	unsigned int dtim_counter;
	unsigned int sleep_ms;
	unsigned long last_sleep_jiffies;
	unsigned long last_awake_jiffies;
	unsigned long last_delaylps_stamp_jiffies;
	unsigned long last_dtim;
	unsigned long last_beacon;
	unsigned long last_action;
	unsigned long last_slept;

	/*For P2P PS */
	struct rtl_p2p_ps_info p2p_ps_info;
	u8 pwr_mode;
	u8 smart_ps;
};

struct rtl_stats {
	u8 psaddr[ETH_ALEN];
	u32 mac_time[2];
	s8 rssi;
	u8 signal;
	u8 noise;
	u8 rate;		/* hw desc rate */
	u8 received_channel;
	u8 control;
	u8 mask;
	u8 freq;
	u16 len;
	u64 tsf;
	u32 beacon_time;
	u8 nic_type;
	u16 length;
	u8 signalquality;	/*in 0-100 index. */
	/*
	 * Real power in dBm for this packet,
	 * no beautification and aggregation.
	 * */
	s32 recvsignalpower;
	s8 rxpower;		/*in dBm Translate from PWdB */
	u8 signalstrength;	/*in 0-100 index. */
	u16 b_hwerror:1;
	u16 b_crc:1;
	u16 b_icv:1;
	u16 b_shortpreamble:1;
	u16 antenna:1;
	u16 decrypted:1;
	u16 wakeup:1;
	u32 timestamp_low;
	u32 timestamp_high;

	u8 rx_drvinfo_size;
	u8 rx_bufshift;
	bool b_isampdu;
	bool b_isfirst_ampdu;
	bool rx_is40Mhzpacket;
	u32 rx_pwdb_all;
	u8 rx_mimo_signalstrength[4];	/*in 0~100 index */
	s8 rx_mimo_signalquality[2];
	bool b_packet_matchbssid;
	bool b_is_cck;
	bool b_is_ht;
	bool b_packet_toself;
	bool b_packet_beacon;	/*for rssi */
	char cck_adc_pwdb[4];	/*for rx path selection */

	u8 packet_report_type;

	u32 macid;
	u8 wake_match;
	u32 bt_rx_rssi_percentage;
	u32 macid_valid_entry[2];
};

struct rt_link_detect {
	/* count for raoming */
	u32 bcn_rx_inperiod;
	u32 roam_times;
	
	u32 num_tx_in4period[4];
	u32 num_rx_in4period[4];

	u32 num_tx_inperiod;
	u32 num_rx_inperiod;

	bool b_busytraffic;
	bool b_tx_busy_traffic;
	bool b_rx_busy_traffic;
	bool b_higher_busytraffic;
	bool b_higher_busyrxtraffic;

	u32 tidtx_in4period[MAX_TID_COUNT][4];
	u32 tidtx_inperiod[MAX_TID_COUNT];
	bool higher_busytxtraffic[MAX_TID_COUNT];
};

struct rtl_tcb_desc {
	u8 b_packet_bw:1;
	u8 b_multicast:1;
	u8 b_broadcast:1;

	u8 b_rts_stbc:1;
	u8 b_rts_enable:1;
	u8 b_cts_enable:1;
	u8 b_rts_use_shortpreamble:1;
	u8 b_rts_use_shortgi:1;
	u8 rts_sc:1;
	u8 b_rts_bw:1;
	u8 rts_rate;

	u8 use_shortgi:1;
	u8 use_shortpreamble:1;
	u8 use_driver_rate:1;
	u8 disable_ratefallback:1;

	u8 ratr_index;
	u8 mac_id;
	u8 hw_rate;

	u8 b_last_inipkt:1;
	u8 b_cmd_or_init:1;
	u8 queue_index;

	/* early mode */
	u8 empkt_num;
	/* The max value by HW */
	u32 empkt_len[10];
	bool btx_enable_sw_calc_duration;
	/* used for hal construct pkt,
	 * we may set desc when tx */
	u8 self_desc;
};

struct proxim {
	bool proxim_on;

	void *proximity_priv;
	int (*proxim_rx)(struct ieee80211_hw *hw, struct rtl_stats *status,
		struct sk_buff *skb);
	u8  (*proxim_get_var)(struct ieee80211_hw *hw, u8 type);
};

struct rtl_hal_ops {
	int (*init_sw_vars) (struct ieee80211_hw * hw);
	void (*deinit_sw_vars) (struct ieee80211_hw * hw);
	void (*read_eeprom_info) (struct ieee80211_hw * hw);
	void (*interrupt_recognized) (struct ieee80211_hw * hw,
				      u32 * p_inta, u32 * p_intb);
	int (*hw_init) (struct ieee80211_hw * hw);
	void (*hw_disable) (struct ieee80211_hw * hw);
	void (*hw_suspend) (struct ieee80211_hw * hw);
	void (*hw_resume) (struct ieee80211_hw * hw);
	void (*enable_interrupt) (struct ieee80211_hw * hw);
	void (*disable_interrupt) (struct ieee80211_hw * hw);
	int (*set_network_type) (struct ieee80211_hw * hw,
				 enum nl80211_iftype type);
	void (*set_chk_bssid)(struct ieee80211_hw *hw,
				bool check_bssid);
	void (*set_bw_mode) (struct ieee80211_hw * hw,
			     enum nl80211_channel_type ch_type);
	 u8(*switch_channel) (struct ieee80211_hw * hw);
	void (*set_qos) (struct ieee80211_hw * hw, int aci);
	void (*set_bcn_reg) (struct ieee80211_hw * hw);
	void (*set_bcn_intv) (struct ieee80211_hw * hw);
	void (*update_interrupt_mask) (struct ieee80211_hw * hw,
				       u32 add_msr, u32 rm_msr);
	void (*get_hw_reg) (struct ieee80211_hw * hw, u8 variable, u8 * val);
	void (*set_hw_reg) (struct ieee80211_hw * hw, u8 variable, u8 * val);
	void (*update_rate_tbl) (struct ieee80211_hw * hw,
			      struct ieee80211_sta *sta, u8 rssi_level);
	void (*fill_tx_desc) (struct ieee80211_hw * hw,
			      struct ieee80211_hdr * hdr, u8 * pdesc_tx,
			      struct ieee80211_tx_info * info,
/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE >= KERNEL_VERSION(3,7,0))
/*<delete in kernel end>*/
			      struct ieee80211_sta *sta,
/*<delete in kernel start>*/
#endif
/*<delete in kernel end>*/
			      struct sk_buff * skb, u8 hw_queue,
			      struct rtl_tcb_desc *ptcb_desc);
	void (*fill_tx_cmddesc) (struct ieee80211_hw * hw, u8 * pdesc,
				 bool b_firstseg, bool b_lastseg,
				 struct sk_buff * skb);
	 bool(*query_rx_desc) (struct ieee80211_hw * hw,
			       struct rtl_stats * status,
			       struct ieee80211_rx_status * rx_status,
			       u8 * pdesc, struct sk_buff * skb);
	void (*set_channel_access) (struct ieee80211_hw * hw);
	 bool(*radio_onoff_checking) (struct ieee80211_hw * hw, u8 * valid);
	void (*dm_watchdog) (struct ieee80211_hw * hw);
	void (*scan_operation_backup) (struct ieee80211_hw * hw, u8 operation);
	 bool(*set_rf_power_state) (struct ieee80211_hw * hw,
				    enum rf_pwrstate rfpwr_state);
	void (*led_control) (struct ieee80211_hw * hw,
			     enum led_ctl_mode ledaction);
	void (*set_desc) (u8 * pdesc, bool istx, u8 desc_name, u8 * val);
	 u32(*get_desc) (u8 * pdesc, bool istx, u8 desc_name);
	void (*tx_polling) (struct ieee80211_hw * hw, u8 hw_queue);
	void (*enable_hw_sec) (struct ieee80211_hw * hw);
	void (*set_key) (struct ieee80211_hw * hw, u32 key_index,
			 u8 * p_macaddr, bool is_group, u8 enc_algo,
			 bool is_wepkey, bool clear_all);
	void (*init_sw_leds) (struct ieee80211_hw * hw);
	 u32(*get_bbreg) (struct ieee80211_hw * hw, u32 regaddr, u32 bitmask);
	void (*set_bbreg) (struct ieee80211_hw * hw, u32 regaddr, u32 bitmask,
			   u32 data);
	 u32(*get_rfreg) (struct ieee80211_hw * hw, enum radio_path rfpath,
			  u32 regaddr, u32 bitmask);
	void (*set_rfreg) (struct ieee80211_hw * hw, enum radio_path rfpath,
			   u32 regaddr, u32 bitmask, u32 data);
	void (*allow_all_destaddr)(struct ieee80211_hw *hw,
		bool allow_all_da, bool write_into_reg);
	void (*linked_set_reg) (struct ieee80211_hw * hw);
	void (*check_switch_to_dmdp) (struct ieee80211_hw * hw);
	void (*dualmac_easy_concurrent) (struct ieee80211_hw *hw);
	void (*dualmac_switch_to_dmdp) (struct ieee80211_hw *hw);
	void (*c2h_command_handle) (struct ieee80211_hw *hw);
	void (*bt_wifi_media_status_notify) (struct ieee80211_hw *hw, bool mstate);
	void (*bt_turn_off_bt_coexist_before_enter_lps) (struct ieee80211_hw *hw);
};

struct rtl_intf_ops {
	/*com */
	void (*read_efuse_byte)(struct ieee80211_hw *hw, u16 _offset, u8 *pbuf);
	int (*adapter_start) (struct ieee80211_hw * hw);
	void (*adapter_stop) (struct ieee80211_hw * hw);
	bool (*check_buddy_priv)(struct ieee80211_hw *hw,
			struct rtl_priv **buddy_priv);

/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	int (*adapter_tx) (struct ieee80211_hw * hw, struct sk_buff * skb,
			struct rtl_tcb_desc *ptcb_desc);
#else
/*<delete in kernel end>*/
	int (*adapter_tx) (struct ieee80211_hw *hw,
			   struct ieee80211_sta *sta,
			   struct sk_buff *skb,
			   struct rtl_tcb_desc *ptcb_desc);
/*<delete in kernel start>*/
#endif
/*<delete in kernel end>*/
	void (*flush)(struct ieee80211_hw *hw, bool drop);
	int (*reset_trx_ring) (struct ieee80211_hw * hw);
/*<delete in kernel start>*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION(3,7,0))
	bool (*waitq_insert) (struct ieee80211_hw *hw, struct sk_buff *skb);
#else
/*<delete in kernel end>*/
	bool (*waitq_insert) (struct ieee80211_hw *hw,
			      struct ieee80211_sta *sta,
			      struct sk_buff *skb);
/*<delete in kernel start>*/
#endif
/*<delete in kernel end>*/

	/*pci */
	void (*disable_aspm) (struct ieee80211_hw * hw);
	void (*enable_aspm) (struct ieee80211_hw * hw);

	/*usb */
};

struct rtl_mod_params {
	/* default: 0 = using hardware encryption */
	bool sw_crypto;

	/* default: 1 = using no linked power save */
	bool b_inactiveps;

	/* default: 1 = using linked sw power save */
	bool b_swctrl_lps;

	/* default: 1 = using linked fw power save */
	bool b_fwctrl_lps;
};

struct rtl_hal_cfg {
	u8 bar_id;
	bool write_readback;
	char *name;
	char *fw_name;
	struct rtl_hal_ops *ops;
	struct rtl_mod_params *mod_params;

	/*this map used for some registers or vars
	   defined int HAL but used in MAIN */
	u32 maps[RTL_VAR_MAP_MAX];

};

struct rtl_locks {
	/* mutex */
	struct mutex conf_mutex;

	/*spin lock */
	spinlock_t ips_lock;
	spinlock_t irq_th_lock;
	spinlock_t h2c_lock;
	spinlock_t rf_ps_lock;
	spinlock_t rf_lock;
	spinlock_t lps_lock;
	spinlock_t waitq_lock;
	spinlock_t entry_list_lock;
	
	/*FW clock change */
	spinlock_t fw_ps_lock;
	
	/*Dul mac*/
	spinlock_t cck_and_rw_pagea_lock;

	/*Easy concurrent*/
	spinlock_t check_sendpkt_lock;
};

struct rtl_works {
	struct ieee80211_hw *hw;

	/*timer */
	struct timer_list watchdog_timer;
	struct timer_list dualmac_easyconcurrent_retrytimer;
	struct timer_list fw_clockoff_timer;
	struct timer_list fast_antenna_trainning_timer;
	/*task */
	struct tasklet_struct irq_tasklet;
	struct tasklet_struct irq_prepare_bcn_tasklet;

	/*work queue */
	struct workqueue_struct *rtl_wq;
	struct delayed_work watchdog_wq;
	struct delayed_work ips_nic_off_wq;

	/* For SW LPS */
	struct delayed_work ps_work;
	struct delayed_work ps_rfon_wq;
	struct delayed_work fwevt_wq;
};

struct rtl_debug {
	u32 dbgp_type[DBGP_TYPE_MAX];
	u32 global_debuglevel;
	u64 global_debugcomponents;

	/* add for proc debug */
	struct proc_dir_entry *proc_dir;
	char proc_name[20];
};

#define MIMO_PS_STATIC			0
#define MIMO_PS_DYNAMIC			1
#define MIMO_PS_NOLIMIT			3

struct rtl_dualmac_easy_concurrent_ctl {
	enum band_type currentbandtype_backfordmdp;
	bool bclose_bbandrf_for_dmsp;
	bool bchange_to_dmdp;
	bool bchange_to_dmsp;
	bool bswitch_in_process;
};

struct rtl_dmsp_ctl {
	bool bactivescan_for_slaveofdmsp;
	bool bscan_for_anothermac_fordmsp;
	bool bscan_for_itself_fordmsp;
	bool bwritedig_for_anothermacofdmsp;
	u32 curdigvalue_for_anothermacofdmsp;
	bool bchangecckpdstate_for_anothermacofdmsp;
	u8 curcckpdstate_for_anothermacofdmsp;
	bool bchangetxhighpowerlvl_for_anothermacofdmsp;
	u8 curtxhighlvl_for_anothermacofdmsp;
	long rssivalmin_for_anothermacofdmsp;
};

struct rtl_global_var {
	/* from this list we can get
	 * other adapter's rtl_priv */
	struct list_head glb_priv_list;
	spinlock_t glb_list_lock;	
};

struct rtl_priv {
	struct list_head list;
#ifdef VIF_TODO
	struct vif_priv vif_priv;
#endif
	struct rtl_priv *buddy_priv;
	struct rtl_global_var *glb_var;
	struct rtl_dualmac_easy_concurrent_ctl easy_concurrent_ctl;
	struct rtl_dmsp_ctl dmsp_ctl;
	struct rtl_locks locks;
	struct rtl_works works;
	struct rtl_mac mac80211;
	struct rtl_hal rtlhal;
	struct rtl_regulatory regd;
	struct rtl_rfkill rfkill;
	struct rtl_io io;
	struct rtl_phy phy;
	struct rtl_dm dm;
	struct rtl_security sec;
	struct rtl_efuse efuse;

	struct rtl_ps_ctl psc;
	struct rate_adaptive ra;
	struct wireless_stats stats;
	struct rt_link_detect link_info;
	struct false_alarm_statistics falsealm_cnt;

	struct rtl_rate_priv *rate_priv;

	struct rtl_debug dbg;

	/* sta entry list for ap adhoc or mesh */
	struct list_head entry_list;

	/*
	 *hal_cfg : for diff cards
	 *intf_ops : for diff interrface usb/pcie
	 */
	struct rtl_hal_cfg *cfg;
	struct rtl_intf_ops *intf_ops;

	/*this var will be set by set_bit,
	   and was used to indicate status of
	   interface or hardware */
	unsigned long status;

	/* intel Proximity, should be alloc mem
	 * in intel Proximity module and can only 
	 * be used in intel Proximity mode */
	struct proxim proximity;

	/*This must be the last item so
	   that it points to the data allocated
	   beyond  this structure like:
	   rtl_pci_priv or rtl_usb_priv */
	u8 priv[0];
};

#define rtl_priv(hw)		(((struct rtl_priv *)(hw)->priv))
#define rtl_mac(rtlpriv)	(&((rtlpriv)->mac80211))
#define rtl_hal(rtlpriv)	(&((rtlpriv)->rtlhal))
#define rtl_efuse(rtlpriv)	(&((rtlpriv)->efuse))
#define rtl_psc(rtlpriv)	(&((rtlpriv)->psc))
#define rtl_sec(rtlpriv) 	(&((rtlpriv)->sec))
#define rtl_dm(rtlpriv)	(&((rtlpriv)->dm))
/***************************************
    Bluetooth Co-existance Related
****************************************/

enum bt_ant_num {
        ANT_X2 = 0,
        ANT_X1 = 1,
};

enum bt_co_type {
        BT_2WIRE = 0,
        BT_ISSC_3WIRE = 1,
        BT_ACCEL = 2,
        BT_CSR_BC4 = 3,
        BT_CSR_BC8 = 4,
        BT_RTL8756 = 5,
        BT_RTL8723A = 6,
};

enum bt_cur_state {
        BT_OFF = 0,
        BT_ON = 1,
};

enum bt_service_type {
        BT_SCO = 0,
        BT_A2DP = 1,
        BT_HID = 2,
        BT_HID_IDLE = 3,
        BT_SCAN = 4,
        BT_IDLE = 5,
        BT_OTHER_ACTION = 6,
        BT_BUSY = 7,
        BT_OTHERBUSY = 8,
        BT_PAN = 9,
};

enum bt_radio_shared {
        BT_RADIO_SHARED = 0,
        BT_RADIO_INDIVIDUAL = 1,
};

struct bt_coexist_info {

	/* EEPROM BT info. */
	u8 eeprom_bt_coexist;
	u8 eeprom_bt_type;
	u8 eeprom_bt_ant_num;
	u8 eeprom_bt_ant_isolation;
	u8 eeprom_bt_radio_shared;

	u8 bt_coexistence;
	u8 bt_ant_num;
	u8 bt_coexist_type;
	u8 bt_state;
	u8 bt_cur_state;	/* 0:on, 1:off */
	u8 bt_ant_isolation;	/* 0:good, 1:bad */
	u8 bt_pape_ctrl;	/* 0:SW, 1:SW/HW dynamic */
	u8 bt_service;
	u8 bt_radio_shared_type;
	u8 bt_rfreg_origin_1e;
	u8 bt_rfreg_origin_1f;
	u8 bt_rssi_state;
	u32 ratio_tx;
	u32 ratio_pri;
	u32 bt_edca_ul;
	u32 bt_edca_dl;

	bool b_init_set;
	bool b_bt_busy_traffic;
	bool b_bt_traffic_mode_set;
	bool b_bt_non_traffic_mode_set;

	bool b_fw_coexist_all_off;
	bool b_sw_coexist_all_off;
	bool b_hw_coexist_all_off;
	u32 current_state;
	u32 previous_state;
	u32 current_state_h;
	u32 previous_state_h;
	
	u8 bt_pre_rssi_state;
	u8 bt_pre_rssi_state1;

	u8 b_reg_bt_iso;
	u8 b_reg_bt_sco;
	bool b_balance_on;
	u8 bt_active_zero_cnt;
	bool b_cur_bt_disabled;
	bool b_pre_bt_disabled;

	u8 bt_profile_case;
	u8 bt_profile_action;
	bool b_bt_busy;
	bool b_hold_for_bt_operation;
	u8 lps_counter;
};


/****************************************
	mem access macro define start
	Call endian free function when
	1. Read/write packet content.
	2. Before write integer to IO.
	3. After read integer from IO.
****************************************/
/* Convert little data endian to host */
#define EF1BYTE(_val)		\
	((u8)(_val))
#define EF2BYTE(_val)		\
	(le16_to_cpu(_val))
#define EF4BYTE(_val)		\
	(le32_to_cpu(_val))

/* Read data from memory */
#define READEF1BYTE(_ptr)	\
	EF1BYTE(*((u8 *)(_ptr)))
#define READEF2BYTE(_ptr)	\
	EF2BYTE(*((u16 *)(_ptr)))
#define READEF4BYTE(_ptr)	\
	EF4BYTE(*((u32 *)(_ptr)))

/* Write data to memory */
#define WRITEEF1BYTE(_ptr, _val)	\
	(*((u8 *)(_ptr)))=EF1BYTE(_val)
#define WRITEEF2BYTE(_ptr, _val)	\
	(*((u16 *)(_ptr)))=EF2BYTE(_val)
#define WRITEEF4BYTE(_ptr, _val)	\
	(*((u32 *)(_ptr)))=EF4BYTE(_val)

/*Example:
BIT_LEN_MASK_32(0) => 0x00000000
BIT_LEN_MASK_32(1) => 0x00000001
BIT_LEN_MASK_32(2) => 0x00000003
BIT_LEN_MASK_32(32) => 0xFFFFFFFF*/
#define BIT_LEN_MASK_32(__bitlen)	 \
	(0xFFFFFFFF >> (32 - (__bitlen)))
#define BIT_LEN_MASK_16(__bitlen)	 \
	(0xFFFF >> (16 - (__bitlen)))
#define BIT_LEN_MASK_8(__bitlen) \
	(0xFF >> (8 - (__bitlen)))

/*Example:
BIT_OFFSET_LEN_MASK_32(0, 2) => 0x00000003
BIT_OFFSET_LEN_MASK_32(16, 2) => 0x00030000*/
#define BIT_OFFSET_LEN_MASK_32(__bitoffset, __bitlen) \
	(BIT_LEN_MASK_32(__bitlen) << (__bitoffset))
#define BIT_OFFSET_LEN_MASK_16(__bitoffset, __bitlen) \
	(BIT_LEN_MASK_16(__bitlen) << (__bitoffset))
#define BIT_OFFSET_LEN_MASK_8(__bitoffset, __bitlen) \
	(BIT_LEN_MASK_8(__bitlen) << (__bitoffset))

/*Description:
Return 4-byte value in host byte ordering from
4-byte pointer in little-endian system.*/
#define LE_P4BYTE_TO_HOST_4BYTE(__pstart) \
	(EF4BYTE(*((u32 *)(__pstart))))
#define LE_P2BYTE_TO_HOST_2BYTE(__pstart) \
	(EF2BYTE(*((u16 *)(__pstart))))
#define LE_P1BYTE_TO_HOST_1BYTE(__pstart) \
	(EF1BYTE(*((u8 *)(__pstart))))

/*Description:
Translate subfield (continuous bits in little-endian) of 4-byte
value to host byte ordering.*/
#define LE_BITS_TO_4BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		( LE_P4BYTE_TO_HOST_4BYTE(__pstart) >> (__bitoffset) )  & \
		BIT_LEN_MASK_32(__bitlen) \
	)
#define LE_BITS_TO_2BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		( LE_P2BYTE_TO_HOST_2BYTE(__pstart) >> (__bitoffset) ) & \
		BIT_LEN_MASK_16(__bitlen) \
	)
#define LE_BITS_TO_1BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		( LE_P1BYTE_TO_HOST_1BYTE(__pstart) >> (__bitoffset) ) & \
		BIT_LEN_MASK_8(__bitlen) \
	)

/*Description:
Mask subfield (continuous bits in little-endian) of 4-byte value
and return the result in 4-byte value in host byte ordering.*/
#define LE_BITS_CLEARED_TO_4BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		LE_P4BYTE_TO_HOST_4BYTE(__pstart)  & \
		( ~BIT_OFFSET_LEN_MASK_32(__bitoffset, __bitlen) ) \
	)
#define LE_BITS_CLEARED_TO_2BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		LE_P2BYTE_TO_HOST_2BYTE(__pstart) & \
		( ~BIT_OFFSET_LEN_MASK_16(__bitoffset, __bitlen) ) \
	)
#define LE_BITS_CLEARED_TO_1BYTE(__pstart, __bitoffset, __bitlen) \
	( \
		LE_P1BYTE_TO_HOST_1BYTE(__pstart) & \
		( ~BIT_OFFSET_LEN_MASK_8(__bitoffset, __bitlen) ) \
	)

/*Description:
Set subfield of little-endian 4-byte value to specified value.	*/
#define SET_BITS_TO_LE_4BYTE(__pstart, __bitoffset, __bitlen, __val) \
	*((u32 *)(__pstart)) = EF4BYTE \
	( \
		LE_BITS_CLEARED_TO_4BYTE(__pstart, __bitoffset, __bitlen) | \
		( (((u32)__val) & BIT_LEN_MASK_32(__bitlen)) << (__bitoffset) ) \
       );
#define SET_BITS_TO_LE_2BYTE(__pstart, __bitoffset, __bitlen, __val) \
	*((u16 *)(__pstart)) = EF2BYTE \
	( \
		LE_BITS_CLEARED_TO_2BYTE(__pstart, __bitoffset, __bitlen) | \
		( (((u16)__val) & BIT_LEN_MASK_16(__bitlen)) << (__bitoffset) ) \
       );
#define SET_BITS_TO_LE_1BYTE(__pstart, __bitoffset, __bitlen, __val) \
	*((u8 *)(__pstart)) = EF1BYTE \
	( \
		LE_BITS_CLEARED_TO_1BYTE(__pstart, __bitoffset, __bitlen) | \
		( (((u8)__val) & BIT_LEN_MASK_8(__bitlen)) << (__bitoffset) ) \
       );

#define	N_BYTE_ALIGMENT(__value, __aligment) ((__aligment == 1) ? \
	(__value) : (((__value + __aligment - 1) / __aligment) * __aligment))

/****************************************
	mem access macro define end
****************************************/

#define byte(x,n) ((x >> (8 * n)) & 0xff)

#define packet_get_type(_packet) (EF1BYTE((_packet).octet[0]) & 0xFC)
#define RTL_WATCH_DOG_TIME    	2000
#define MSECS(t)		msecs_to_jiffies(t)
#define WLAN_FC_GET_VERS(fc) 	((fc) & IEEE80211_FCTL_VERS)
#define WLAN_FC_GET_TYPE(fc) 	((fc) & IEEE80211_FCTL_FTYPE)
#define WLAN_FC_GET_STYPE(fc) 	((fc) & IEEE80211_FCTL_STYPE)
#define WLAN_FC_MORE_DATA(fc) 	((fc) & IEEE80211_FCTL_MOREDATA)
#define SEQ_TO_SN(seq) 		(((seq) & IEEE80211_SCTL_SEQ) >> 4)
#define SN_TO_SEQ(ssn) 		(((ssn) << 4) & IEEE80211_SCTL_SEQ)
#define MAX_SN 			((IEEE80211_SCTL_SEQ) >> 4)

#define	RT_RF_OFF_LEVL_ASPM		BIT(0)	/*PCI ASPM */
#define	RT_RF_OFF_LEVL_CLK_REQ		BIT(1)	/*PCI clock request */
#define	RT_RF_OFF_LEVL_PCI_D3		BIT(2)	/*PCI D3 mode */
/*NIC halt, re-initialize hw parameters*/
#define	RT_RF_OFF_LEVL_HALT_NIC		BIT(3)
#define	RT_RF_OFF_LEVL_FREE_FW		BIT(4)	/*FW free, re-download the FW */
#define	RT_RF_OFF_LEVL_FW_32K		BIT(5)	/*FW in 32k */
/*Always enable ASPM and Clock Req in initialization.*/
#define	RT_RF_PS_LEVEL_ALWAYS_ASPM	BIT(6)
/* no matter RFOFF or SLEEP we set PS_ASPM_LEVL*/
#define	RT_PS_LEVEL_ASPM		BIT(7)
/*When LPS is on, disable 2R if no packet is received or transmittd.*/
#define	RT_RF_LPS_DISALBE_2R		BIT(30)
#define	RT_RF_LPS_LEVEL_ASPM		BIT(31)	/*LPS with ASPM */
#define	RT_IN_PS_LEVEL(ppsc, _ps_flg)		\
	((ppsc->cur_ps_level & _ps_flg) ? true : false)
#define	RT_CLEAR_PS_LEVEL(ppsc, _ps_flg)	\
	(ppsc->cur_ps_level &= (~(_ps_flg)))
#define	RT_SET_PS_LEVEL(ppsc, _ps_flg)		\
	(ppsc->cur_ps_level |= _ps_flg)

#define container_of_dwork_rtl(x,y,z) \
	container_of(container_of(x, struct delayed_work, work), y, z)

#define FILL_OCTET_STRING(_os,_octet,_len)	\
		(_os).octet=(u8*)(_octet);		\
		(_os).length=(_len);

#define CP_MACADDR(des,src)	\
	((des)[0]=(src)[0],(des)[1]=(src)[1],\
	(des)[2]=(src)[2],(des)[3]=(src)[3],\
	(des)[4]=(src)[4],(des)[5]=(src)[5])

static inline u8 rtl_read_byte(struct rtl_priv *rtlpriv, u32 addr)
{
	return rtlpriv->io.read8_sync(rtlpriv, addr);
}

static inline u16 rtl_read_word(struct rtl_priv *rtlpriv, u32 addr)
{
	return rtlpriv->io.read16_sync(rtlpriv, addr);
}

static inline u32 rtl_read_dword(struct rtl_priv *rtlpriv, u32 addr)
{
	return rtlpriv->io.read32_sync(rtlpriv, addr);
}

static inline void rtl_write_byte(struct rtl_priv *rtlpriv, u32 addr, u8 val8)
{
	rtlpriv->io.write8_async(rtlpriv, addr, val8);

	if (rtlpriv->cfg->write_readback)
		rtlpriv->io.read8_sync(rtlpriv, addr);
}

static inline void rtl_write_word(struct rtl_priv *rtlpriv, u32 addr, u16 val16)
{
	rtlpriv->io.write16_async(rtlpriv, addr, val16);

	if (rtlpriv->cfg->write_readback)
		rtlpriv->io.read16_sync(rtlpriv, addr);
}

static inline void rtl_write_dword(struct rtl_priv *rtlpriv,
				   u32 addr, u32 val32)
{
	rtlpriv->io.write32_async(rtlpriv, addr, val32);

	if (rtlpriv->cfg->write_readback)
		rtlpriv->io.read32_sync(rtlpriv, addr);
}

static inline u32 rtl_get_bbreg(struct ieee80211_hw *hw,
				u32 regaddr, u32 bitmask)
{
	return ((struct rtl_priv *)(hw)->priv)->cfg->ops->get_bbreg(hw,
								    regaddr,
								    bitmask);
}

static inline void rtl_set_bbreg(struct ieee80211_hw *hw, u32 regaddr,
				 u32 bitmask, u32 data)
{
	((struct rtl_priv *)(hw)->priv)->cfg->ops->set_bbreg(hw,
							     regaddr, bitmask,
							     data);

}

static inline u32 rtl_get_rfreg(struct ieee80211_hw *hw,
				enum radio_path rfpath, u32 regaddr,
				u32 bitmask)
{
	return ((struct rtl_priv *)(hw)->priv)->cfg->ops->get_rfreg(hw,
								    rfpath,
								    regaddr,
								    bitmask);
}

static inline void rtl_set_rfreg(struct ieee80211_hw *hw,
				 enum radio_path rfpath, u32 regaddr,
				 u32 bitmask, u32 data)
{
	((struct rtl_priv *)(hw)->priv)->cfg->ops->set_rfreg(hw,
							     rfpath, regaddr,
							     bitmask, data);
}

static inline bool is_hal_stop(struct rtl_hal *rtlhal)
{
	return (_HAL_STATE_STOP == rtlhal->state);
}

static inline void set_hal_start(struct rtl_hal *rtlhal)
{
	rtlhal->state = _HAL_STATE_START;
}

static inline void set_hal_stop(struct rtl_hal *rtlhal)
{
	rtlhal->state = _HAL_STATE_STOP;
}

static inline u8 get_rf_type(struct rtl_phy *rtlphy)
{
	return rtlphy->rf_type;
}

static inline struct ieee80211_hdr *rtl_get_hdr(struct sk_buff *skb)
{
	return (struct ieee80211_hdr *)(skb->data);
}

static inline u16 rtl_get_fc(struct sk_buff *skb)
{
	return le16_to_cpu(rtl_get_hdr(skb)->frame_control);
}

static inline u16 rtl_get_tid_h(struct ieee80211_hdr *hdr)
{
	return (ieee80211_get_qos_ctl(hdr))[0] & IEEE80211_QOS_CTL_TID_MASK;
}

static inline u16 rtl_get_tid(struct sk_buff *skb)
{
	return rtl_get_tid_h(rtl_get_hdr(skb));
}

static inline struct ieee80211_sta *rtl_find_sta(struct ieee80211_hw *hw,
		u8 *mac_addr)
{
	struct rtl_mac *mac = rtl_mac(rtl_priv(hw));
	return ieee80211_find_sta(mac->vif, mac_addr);
}

struct ieee80211_hw *rtl_pci_get_hw_pointer(void);
#endif
