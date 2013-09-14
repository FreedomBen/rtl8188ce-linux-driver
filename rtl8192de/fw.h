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

#ifndef __RTL92D__FW__H__
#define __RTL92D__FW__H__

#define FW_8192D_START_ADDRESS			0x1000
#define FW_8192D_PAGE_SIZE				4096
#define FW_8192D_POLLING_TIMEOUT_COUNT	1000

#define IS_FW_HEADER_EXIST(_pfwhdr)	\
		((GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFF0) == 0x92C0 ||\
		(GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFF0) == 0x88C0 ||\
		(GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFFF) == 0x92D0 ||\
		(GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFFF) == 0x92D1 ||\
		(GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFFF) == 0x92D2 ||\
		(GET_FIRMWARE_HDR_SIGNATURE(_pfwhdr) & 0xFFFF) == 0x92D3)

/* Firmware Header(8-byte alinment required) */
/* --- LONG WORD 0 ---- */
#define GET_FIRMWARE_HDR_SIGNATURE(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr, 0, 16)
#define GET_FIRMWARE_HDR_CATEGORY(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr, 16, 8)
#define GET_FIRMWARE_HDR_FUNCTION(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr, 24, 8)
#define GET_FIRMWARE_HDR_VERSION(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr + 4, 0, 16)
#define GET_FIRMWARE_HDR_SUB_VER(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr + 4, 16, 8)
#define GET_FIRMWARE_HDR_RSVD1(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 4, 24, 8)

/* --- LONG WORD 1 ---- */
#define GET_FIRMWARE_HDR_MONTH(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 8, 0, 8)
#define GET_FIRMWARE_HDR_DATE(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 8, 8, 8)
#define GET_FIRMWARE_HDR_HOUR(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 8, 16, 8)
#define GET_FIRMWARE_HDR_MINUTE(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr + 8, 24, 8)
#define GET_FIRMWARE_HDR_ROMCODE_SIZE(__fwhdr)	\
	LE_BITS_TO_4BYTE(__fwhdr + 12, 0, 16)
#define GET_FIRMWARE_HDR_RSVD2(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 12, 16, 16)

/* --- LONG WORD 2 ---- */
#define GET_FIRMWARE_HDR_SVN_IDX(__fwhdr)		\
	LE_BITS_TO_4BYTE(__fwhdr + 16, 0, 32)
#define GET_FIRMWARE_HDR_RSVD3(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 20, 0, 32)

/* --- LONG WORD 3 ---- */
#define GET_FIRMWARE_HDR_RSVD4(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 24, 0, 32)
#define GET_FIRMWARE_HDR_RSVD5(__fwhdr)			\
	LE_BITS_TO_4BYTE(__fwhdr + 28, 0, 32)

#define pagenum_128(_len) \
	(u32)(((_len) >> 7) + ((_len) & 0x7F ? 1 : 0))

#define SET_H2CCMD_PWRMODE_PARM_MODE(__ph2ccmd, __val)			\
	SET_BITS_TO_LE_1BYTE(__ph2ccmd, 0, 8, __val)
#define SET_H2CCMD_PWRMODE_PARM_SMART_PS(__ph2ccmd, __val)		\
	SET_BITS_TO_LE_1BYTE((__ph2ccmd) + 1, 0, 8, __val)
#define SET_H2CCMD_PWRMODE_PARM_BCN_PASS_TIME(__ph2ccmd, __val)	\
	SET_BITS_TO_LE_1BYTE((__ph2ccmd) + 2, 0, 8, __val)
#define SET_H2CCMD_JOINBSSRPT_PARM_OPMODE(__ph2ccmd, __val)		\
	SET_BITS_TO_LE_1BYTE(__ph2ccmd, 0, 8, __val)
#define SET_H2CCMD_RSVDPAGE_LOC_PROBE_RSP(__ph2ccmd, __val)		\
	SET_BITS_TO_LE_1BYTE(__ph2ccmd, 0, 8, __val)
#define SET_H2CCMD_RSVDPAGE_LOC_PSPOLL(__ph2ccmd, __val)		\
	SET_BITS_TO_LE_1BYTE((__ph2ccmd) + 1, 0, 8, __val)
#define SET_H2CCMD_RSVDPAGE_LOC_NULL_DATA(__ph2ccmd, __val)		\
	SET_BITS_TO_LE_1BYTE((__ph2ccmd) + 2, 0, 8, __val)

struct rtl92d_firmware_header {
	u16 signature;
	u8 category;
	u8 function;
	u16 version;
	u8 subversion;
	u8 rsvd1;

	u8 month;
	u8 date;
	u8 hour;
	u8 minute;
	u16 ramcodeSize;
	u16 rsvd2;

	u32 svnindex;
	u32 rsvd3;

	u32 rsvd4;
	u32 rsvd5;
};

enum rtl8192d_h2c_cmd {
	H2C_AP_OFFLOAD = 0,
	H2C_SETPWRMODE = 1,
	H2C_JOINBSSRPT = 2,
	H2C_RSVDPAGE = 3,
	H2C_RSSI_REPORT = 5,
	H2C_RA_MASK = 6,
	H2C_P2P_PS_OFFLOAD = 8,
	H2C_MAC_MODE_SEL = 9,
	H2C_PWRM = 15,
	H2C_P2P_PS_CTW_CMD = 24,
	MAX_H2CCMD
};

int rtl92d_download_fw(struct ieee80211_hw *hw);
void rtl92d_fill_h2c_cmd(struct ieee80211_hw *hw, u8 element_id,
			 u32 cmd_len, u8 *p_cmdbuffer);
void rtl92d_firmware_selfreset(struct ieee80211_hw *hw);
void rtl92d_set_fw_pwrmode_cmd(struct ieee80211_hw *hw, u8 mode);
void rtl92d_set_fw_rsvdpagepkt(struct ieee80211_hw *hw, bool b_dl_finished);
void rtl92d_set_fw_joinbss_report_cmd(struct ieee80211_hw *hw, u8 mstatus);
void rtl92d_set_p2p_ps_offload_cmd(struct ieee80211_hw *hw, u8 p2p_ps_state);
#endif
