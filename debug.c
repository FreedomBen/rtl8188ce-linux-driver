/******************************************************************************
 *
 * Copyright(c) 2009-2010  Realtek Corporation.
 *
 * Tmis program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * Tmis program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * tmis program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 * Tme full GNU General Public License is included in this distribution in the
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
#include "cam.h"

void rtl_dbgp_flag_init(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u8 i;

	rtlpriv->dbg.global_debuglevel = DBG_EMERG;

	rtlpriv->dbg.global_debugcomponents =
		COMP_ERR |
		COMP_FW |
		COMP_INIT |
		COMP_RECV |
		COMP_SEND |
	  	COMP_MLME |
	  	COMP_SCAN |
	  	COMP_INTR |
	  	COMP_LED |
	  	COMP_SEC |
	  	COMP_BEACON |
	  	COMP_RATE |
	  	COMP_RXDESC |
	  	COMP_DIG |
	  	COMP_TXAGC |
		COMP_POWER |
	  	COMP_POWER_TRACKING |
	  	COMP_BB_POWERSAVING |
	  	COMP_SWAS |
	  	COMP_RF |
	  	COMP_TURBO |
	  	COMP_RATR |
	  	COMP_CMD |
	  	COMP_EASY_CONCURRENT |
	  	COMP_EFUSE |
	  	COMP_QOS | COMP_MAC80211 | COMP_REGD |
		COMP_CHAN |
		COMP_BT_COEXIST | 0;

	for (i = 0; i < DBGP_TYPE_MAX; i++)
		rtlpriv->dbg.dbgp_type[i] = 0;

	/*Init Debug flag enable condition */
}

struct proc_dir_entry *proc_topdir;
static int rtl_proc_get_mac_0(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x000;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_mac_1(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x100;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}
static int rtl_proc_get_mac_2(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x200;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len,"\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_mac_3(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x300;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_mac_4(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max=0xff;
	page0 = 0x400;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_mac_5(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x500;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_mac_6(char *page, char **start,
                          off_t offset, int count,
                          int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x600;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}
static int rtl_proc_get_mac_7(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x700;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; i++, n += 4)
			len += snprintf(page + len, count - len,
				"%8.8x    ", rtl_read_dword(rtlpriv, (page0 | n)));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}

static int rtl_proc_get_bb_8(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x800;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
		len += snprintf(page + len, count - len,
			"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;

}
static int rtl_proc_get_bb_9(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0x900;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 |n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_bb_a(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xa00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_bb_b(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xb00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}
static int rtl_proc_get_bb_c(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xc00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}
static int rtl_proc_get_bb_d(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xd00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_bb_e(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xe00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_bb_f(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n, page0;
	int max = 0xff;
	page0 = 0xf00;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n + page0);
		for (i = 0; i < 4 && n <= max; n += 4, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_bbreg(hw, (page0 | n), 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_reg_rf_a(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n;
	int max = 0x40;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n);
		for (i = 0; i < 4 && n <= max; n += 1, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_rfreg(hw, RF90_PATH_A, n, 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_reg_rf_b(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	int len = 0;
	int i, n;
	int max = 0x40;

	for (n = 0; n <= max; ) {
		len += snprintf(page + len, count - len, "\n%#8.8x  ", n);
		for (i = 0; i < 4 && n <= max; n += 1, i++)
			len += snprintf(page + len, count - len,
				"%8.8x     ",rtl_get_rfreg(hw, RF90_PATH_B, n, 0xffffffff));
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_cam_register_1(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u32 target_cmd = 0;
	u32 target_val=0;
	u8 entry_i=0;
	u32 ulstatus;
	int len = 0;
	int i = 100, j = 0;

	/* This dump the current register page */
	len += snprintf(page + len, count - len,
		"\n#################### SECURITY CAM (0-10) ##################\n ");

	for (j = 0; j < 11; j++) {
		len += snprintf(page + len, count - len, "\nD:  %2x > ", j);
	 	for (entry_i = 0; entry_i < CAM_CONTENT_COUNT; entry_i++) {
	   		/* polling bit, and No Write enable, and address  */
			target_cmd = entry_i + CAM_CONTENT_COUNT * j;
			target_cmd = target_cmd | BIT(31);

			/* Check polling bit is clear */
			while ((i--) >= 0) {
				ulstatus = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM]);
				if (ulstatus & BIT(31)) {
					continue;
				} else {
					break;
				}
			}

	  		rtl_write_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM], target_cmd);
	  	 	target_val = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RCAMO]);
			len += snprintf(page + len, count - len, "%8.8x ", target_val);
	 	}
	}

	len += snprintf(page + len, count - len,"\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_cam_register_2(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u32 target_cmd = 0;
	u32 target_val = 0;
	u8 entry_i = 0;
	u32 ulstatus;
	int len = 0;
	int i = 100, j = 0;

	/* This dump the current register page */
	len += snprintf(page + len, count - len,
		"\n#################### SECURITY CAM (11-21) ##################\n ");

	for (j = 11; j < 22; j++) {
		len += snprintf(page + len, count - len, "\nD:  %2x > ", j);
	 	for (entry_i = 0; entry_i < CAM_CONTENT_COUNT; entry_i++) {
			target_cmd = entry_i + CAM_CONTENT_COUNT * j;
			target_cmd = target_cmd | BIT(31);

			while ((i--) >= 0) {
				ulstatus = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM]);
				if (ulstatus & BIT(31)) {
					continue;
				} else {
					break;
				}
			}

	  		rtl_write_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM], target_cmd);
	  	 	target_val = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RCAMO]);
			len += snprintf(page + len, count - len,"%8.8x ", target_val);
	 	}
	}

	len += snprintf(page + len, count - len,"\n");
	*eof = 1;
	return len;
}

static int rtl_proc_get_cam_register_3(char *page, char **start,
		off_t offset, int count, int *eof, void *data)
{
	struct ieee80211_hw *hw = data;
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	u32 target_cmd = 0;
	u32 target_val = 0;
	u8 entry_i = 0;
	u32 ulstatus;
	int len = 0;
	int i = 100, j = 0;

	/* This dump the current register page */
	len += snprintf(page + len, count - len,
		"\n#################### SECURITY CAM (22-31) ##################\n ");

	for (j = 22; j < TOTAL_CAM_ENTRY; j++) {
		len += snprintf(page + len, count - len, "\nD:  %2x > ", j);
	 	for (entry_i = 0; entry_i < CAM_CONTENT_COUNT; entry_i++) {
			target_cmd = entry_i+CAM_CONTENT_COUNT*j;
			target_cmd = target_cmd | BIT(31);

			while ((i--) >= 0) {
				ulstatus = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM]);
				if (ulstatus & BIT(31)) {
					continue;
				} else {
					break;
				}
			}

	  		rtl_write_dword(rtlpriv, rtlpriv->cfg->maps[RWCAM], target_cmd);
	  	 	target_val = rtl_read_dword(rtlpriv, rtlpriv->cfg->maps[RCAMO]);
			len += snprintf(page + len, count - len,"%8.8x ", target_val);
	 	}
	}

	len += snprintf(page + len, count - len, "\n");
	*eof = 1;
	return len;
}

void rtl_proc_add_one(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);
	struct rtl_efuse *rtlefuse = rtl_efuse(rtl_priv(hw));
	struct proc_dir_entry *entry;

	snprintf(rtlpriv->dbg.proc_name, 18, "%x-%x-%x-%x-%x-%x",
		rtlefuse->dev_addr[0], rtlefuse->dev_addr[1],
		rtlefuse->dev_addr[2], rtlefuse->dev_addr[3],
		rtlefuse->dev_addr[4], rtlefuse->dev_addr[5]);

	rtlpriv->dbg.proc_dir = create_proc_entry(rtlpriv->dbg.proc_name,
		S_IFDIR | S_IRUGO | S_IXUGO, proc_topdir);
	if (!rtlpriv->dbg.proc_dir) {
		RT_TRACE(COMP_INIT, DBG_EMERG, ("Unable to init "
			"/proc/net/%s/%s\n", rtlpriv->cfg->name,
			rtlpriv->dbg.proc_name));
		return;
	}

	entry = create_proc_read_entry("mac-0", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_0, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, DBG_EMERG, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-0\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-1", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_1, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-1\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-2", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_2, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-2\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-3", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_3, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-3\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-4", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_4, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-4\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-5", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_5, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-5\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-6", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_6, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-6\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("mac-7", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_mac_7, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/mac-7\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-8", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_8, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-8\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-9", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_9, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-9\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-a", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_a, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-a\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-b", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_b, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-b\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-c", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_c, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-c\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-d", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_d, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-d\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-e", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_e, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-e\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("bb-f", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_bb_f, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/bb-f\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("rf-a", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_reg_rf_a, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/rf-a\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("rf-b", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_reg_rf_b, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/rf-b\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("cam-1", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_cam_register_1, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/cam-1\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("cam-2", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_cam_register_2, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/cam-2\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));

	entry = create_proc_read_entry("cam-3", S_IFREG | S_IRUGO,
				   rtlpriv->dbg.proc_dir, rtl_proc_get_cam_register_3, hw);
	if (!entry)
		RT_TRACE(COMP_INIT, COMP_ERR, ("Unable to initialize "
		      "/proc/net/%s/%s/cam-3\n",
		      rtlpriv->cfg->name, rtlpriv->dbg.proc_name));
}

void rtl_proc_remove_one(struct ieee80211_hw *hw)
{
	struct rtl_priv *rtlpriv = rtl_priv(hw);

	if (rtlpriv->dbg.proc_dir) {
		remove_proc_entry("mac-0", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-1", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-2", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-3", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-4", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-5", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-6", rtlpriv->dbg.proc_dir);
		remove_proc_entry("mac-7", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-8", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-9", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-a", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-b", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-c", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-d", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-e", rtlpriv->dbg.proc_dir);
		remove_proc_entry("bb-f", rtlpriv->dbg.proc_dir);
		remove_proc_entry("rf-a", rtlpriv->dbg.proc_dir);
		remove_proc_entry("rf-b", rtlpriv->dbg.proc_dir);
		remove_proc_entry("cam-1", rtlpriv->dbg.proc_dir);
		remove_proc_entry("cam-2", rtlpriv->dbg.proc_dir);
		remove_proc_entry("cam-3", rtlpriv->dbg.proc_dir);

		remove_proc_entry(rtlpriv->dbg.proc_name, proc_topdir);

		rtlpriv->dbg.proc_dir = NULL;
	}
}

void rtl_proc_add_topdir(void)
{
		proc_topdir = create_proc_entry("rtlwifi",
			S_IFDIR, init_net.proc_net);
}

void rtl_proc_remove_topdir(void)
{
	if (proc_topdir)
		remove_proc_entry("rtlwifi", init_net.proc_net);
}
