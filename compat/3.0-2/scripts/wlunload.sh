#!/bin/bash

# The old stack drivers and the mac80211 rc80211_simple modules
# which is no longer on recent kernels (its internal)
OLD_MODULES="iwlwifi_mac80211 rc80211_simple zd1211rw-mac80211"
OLD_MODULES="$OLD_MODULES ieee80211_crypt_tkip ieee80211_crypt_ccmp"
OLD_MODULES="$OLD_MODULES ieee80211softmac ieee80211_crypt ieee80211"
OLD_MODULES="$OLD_MODULES bcm43xx rndis_wext iwl4965"
MODULES="$OLD_MODULES"
MODULES="$MODULES ipw2100 ipw2200 libipw"
MODULES="$MODULES wl1251 wl12xx iwmc3200wifi"
MODULES="$MODULES libertas_cs usb8xxx libertas libertas_sdio libertas_spi"
MODULES="$MODULES libertas_tf libertas_tf_usb"
MODULES="$MODULES adm8211 zd1211rw"
MODULES="$MODULES orinoco_cs orinoco_nortel orinoco_pci orinoco_plx"
MODULES="$MODULES orinoco_tld orinoco_usb spectrum_cs orinoco"
MODULES="$MODULES b43 b44 b43legacy brcm80211 ssb"
MODULES="$MODULES iwl3945 iwlagn iwlcore"
MODULES="$MODULES ath9k ath9k_htc ath9k_common ath9k_hw "
MODULES="$MODULES ath5k ath ath6kl ar9170usb carl9170"
MODULES="$MODULES p54pci p54usb p54spi p54common"
MODULES="$MODULES rt2400pci rt2500pci rt61pci"
MODULES="$MODULES rt2500usb rt73usb"
MODULES="$MODULES rt2800usb rt2800lib"
MODULES="$MODULES rt2x00usb rt2x00lib"
MODULES="$MODULES rtl8180 rtl8187 rtl8192ce rtl8192de rtl8192se rtlwifi"
MODULES="$MODULES mwl8k mac80211_hwsim"
MODULES="$MODULES at76c50x_usb at76_usb"
MODULES="$MODULES rndis_wlan rndis_host cdc_ether usbnet"
# eeprom_93cx6 is used by rt2x00 (rt61pci, rt2500pci, rt2400pci)
# and Realtek drivers ( rtl8187, rtl8180)
MODULES="$MODULES eeprom_93cx6"
MODULES="$MODULES lib80211_crypt_ccmp lib80211_crypt_tkip lib80211_crypt_wep"
MODULES="$MODULES mac80211 cfg80211 lib80211"
MADWIFI_MODULES="ath_pci ath_rate_sample wlan_scan_sta wlan ath_hal"
IPW3945D="/sbin/ipw3945d-`uname -r`"

if [ -f $IPW3945D ]; then
	$IPW3945D --isrunning
	if [ ! $? ]; then
		echo -n "Detected ipw3945 daemon loaded we're going to "
		echo "shut the daemon down now and remove the module."
		modprobe -r --ignore-remove ipw3945
	fi
fi

grep ath_pci /proc/modules 2>&1 > /dev/null
if [ $? -eq 0 ]; then
	echo "MadWifi driver is loaded, going to try to unload it..."
	./scripts/madwifi-unload
fi

for i in $MODULES; do
	grep ^$i /proc/modules 2>&1 > /dev/null
	if [ $? -eq 0 ]; then
		echo Unloading $i...
		modprobe -r --ignore-remove $i
	fi
done
