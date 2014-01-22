export

## NOTE
## Make sure to have each variable declaration start
## in the first column, no whitespace allowed.

ifeq ($(wildcard $(KLIB_BUILD)/.config),)
# These will be ignored by compat autoconf
 CONFIG_PCI=y
 CONFIG_USB=y
 CONFIG_PCMCIA=y
 CONFIG_SSB=m
else
include $(KLIB_BUILD)/.config
endif

# We will warn when you don't have MQ support or NET_SCHED enabled.
#
# We could consider just quiting if MQ and NET_SCHED is disabled
# as I suspect all users of this package want 802.11e (WME) and
# 802.11n (HT) support.
ifneq ($(wildcard $(KLIB_BUILD)/Makefile),)

COMPAT_LATEST_VERSION = 1

KERNEL_VERSION := $(shell $(MAKE) -C $(KLIB_BUILD) kernelversion | sed -n 's/^\([0-9]\)\..*/\1/p')

ifneq ($(KERNEL_VERSION),2)
KERNEL_SUBLEVEL := $(shell $(MAKE) -C $(KLIB_BUILD) kernelversion | sed -n 's/^3\.\([0-9]\+\).*/\1/p')
else
COMPAT_26LATEST_VERSION = 39
KERNEL_26SUBLEVEL := $(shell $(MAKE) -C $(KLIB_BUILD) kernelversion | sed -n 's/^2\.6\.\([0-9]\+\).*/\1/p')
COMPAT_26VERSIONS := $(shell I=$(COMPAT_26LATEST_VERSION); while [ "$$I" -gt $(KERNEL_26SUBLEVEL) ]; do echo $$I; I=$$(($$I - 1)); done)
$(foreach ver,$(COMPAT_26VERSIONS),$(eval CONFIG_COMPAT_KERNEL_2_6_$(ver)=y))
KERNEL_SUBLEVEL := -1
endif

COMPAT_VERSIONS := $(shell I=$(COMPAT_LATEST_VERSION); while [ "$$I" -gt $(KERNEL_SUBLEVEL) ]; do echo $$I; I=$$(($$I - 1)); done)
$(foreach ver,$(COMPAT_VERSIONS),$(eval CONFIG_COMPAT_KERNEL_3_$(ver)=y))

ifdef CONFIG_COMPAT_KERNEL_2_6_24
$(error "ERROR: compat-wireless by default supports kernels >= 2.6.24, try enabling only one driver though")
endif #CONFIG_COMPAT_KERNEL_2_6_24

ifeq ($(CONFIG_CFG80211),y)
$(error "ERROR: your kernel has CONFIG_CFG80211=y, you should have it CONFIG_CFG80211=m if you want to use this thing.")
endif


# 2.6.27 has FTRACE_DYNAMIC borked, so we will complain if
# you have it enabled, otherwise you will very likely run into
# a kernel panic.
ifeq ($(shell test $(KERNEL_VERSION) -eq 2 -a $(KERNEL_SUBLEVEL) -eq 27 && echo yes),yes)
ifeq ($(CONFIG_DYNAMIC_FTRACE),y)
$(error "ERROR: Your 2.6.27 kernel has CONFIG_DYNAMIC_FTRACE, please upgrade your distribution kernel as newer ones should not have this enabled (and if so report a bug) or remove this warning if you know what you are doing")
endif
endif

# This is because with CONFIG_MAC80211 include/linux/skbuff.h will
# enable on 2.6.27 a new attribute:
#
# skb->do_not_encrypt
#
# and on 2.6.28 another new attribute:
#
# skb->requeue
#
# In kernel 2.6.32 both attributes were removed.
#
ifeq ($(shell test $(KERNEL_VERSION) -eq 2 -a $(KERNEL_SUBLEVEL) -ge 27 -a $(KERNEL_SUBLEVEL) -le 31 && echo yes),yes)
ifeq ($(CONFIG_MAC80211),)
$(error "ERROR: Your >=2.6.27 and <= 2.6.31 kernel has CONFIG_MAC80211 disabled, you should have it CONFIG_MAC80211=m if you want to use this thing.")
endif
endif

ifneq ($(KERNELRELEASE),) # This prevents a warning

ifeq ($(CONFIG_NET_SCHED),)
 QOS_REQS_MISSING+=CONFIG_NET_SCHED
endif

ifneq ($(QOS_REQS_MISSING),) # Complain about our missing dependencies
$(warning "WARNING: You are running a kernel >= 2.6.23, you should enable in it $(QOS_REQS_MISSING) for 802.11[ne] support")
endif

endif # build check
endif # kernel Makefile check

# These both are needed by compat-wireless || compat-bluetooth so enable them
 CONFIG_COMPAT_RFKILL=y

ifeq ($(CONFIG_MAC80211),y)
$(error "ERROR: you have MAC80211 compiled into the kernel, CONFIG_MAC80211=y, as such you cannot replace its mac80211 driver. You need this set to CONFIG_MAC80211=m. If you are using Fedora upgrade your kernel as later version should this set as modular. For further information on Fedora see https://bugzilla.redhat.com/show_bug.cgi?id=470143. If you are using your own kernel recompile it and make mac80211 modular")
else
 CONFIG_COMPAT_WIRELESS=y
 CONFIG_COMPAT_WIRELESS_MODULES=m
 CONFIG_COMPAT_VAR_MODULES=m
# We could technically separate these but not yet, we only have b44
# Note that we don't intend on backporting network drivers that
# use Multiqueue as that was a pain to backport to kernels older than
# 2.6.27. But -- we could just disable those drivers from kernels
# older than 2.6.27
 CONFIG_COMPAT_NETWORK_MODULES=m
 CONFIG_COMPAT_NET_USB_MODULES=m
endif

# The Bluetooth compatibility only builds on kernels >= 2.6.27 for now
ifndef CONFIG_COMPAT_KERNEL_2_6_27
ifeq ($(CONFIG_BT),y)
# we'll ignore compiling bluetooth
else
 CONFIG_COMPAT_BLUETOOTH=y
 CONFIG_COMPAT_BLUETOOTH_MODULES=m
endif
endif #CONFIG_COMPAT_KERNEL_2_6_27

ifdef CONFIG_COMPAT_KERNEL_2_6_33
ifdef CONFIG_FW_LOADER
 CONFIG_COMPAT_FIRMWARE_CLASS=m
endif #CONFIG_FW_LOADER
endif #CONFIG_COMPAT_KERNEL_2_6_33

# Wireless subsystem stuff
CONFIG_MAC80211=m

# CONFIG_MAC80211_DEBUGFS=y
# CONFIG_MAC80211_NOINLINE=y
# CONFIG_MAC80211_VERBOSE_DEBUG=y
# CONFIG_MAC80211_HT_DEBUG=y
# CONFIG_MAC80211_TKIP_DEBUG=y
# CONFIG_MAC80211_IBSS_DEBUG=y
# CONFIG_MAC80211_VERBOSE_PS_DEBUG=y
# CONFIG_MAC80211_VERBOSE_MPL_DEBUG=y
# CONFIG_MAC80211_VERBOSE_MHWMP_DEBUG=y
# CONFIG_MAC80211_DEBUG_COUNTERS=y
# CONFIG_MAC80211_DRIVER_API_TRACER=y

# choose between pid and minstrel as default rate control algorithm
CONFIG_MAC80211_RC_DEFAULT=minstrel_ht
CONFIG_MAC80211_RC_DEFAULT_MINSTREL=y
# CONFIG_MAC80211_RC_DEFAULT_PID=y
# This is the one used by our compat-wireless net/mac80211/rate.c
# in case you have and old kernel which is overriding this to pid.
CONFIG_COMPAT_MAC80211_RC_DEFAULT=minstrel_ht
CONFIG_MAC80211_RC_PID=y
CONFIG_MAC80211_RC_MINSTREL=y
CONFIG_MAC80211_RC_MINSTREL_HT=y
ifdef CONFIG_LEDS_TRIGGERS
CONFIG_MAC80211_LEDS=y
endif #CONFIG_LEDS_TRIGGERS

# enable mesh networking too
CONFIG_MAC80211_MESH=y

CONFIG_CFG80211=m
CONFIG_CFG80211_DEFAULT_PS=y
# CONFIG_CFG80211_DEBUGFS=y
# CONFIG_NL80211_TESTMODE=y
# CONFIG_CFG80211_DEVELOPER_WARNINGS=y
# CONFIG_CFG80211_REG_DEBUG=y
# See below for wext stuff

CONFIG_LIB80211=m
CONFIG_LIB80211_CRYPT_WEP=m
CONFIG_LIB80211_CRYPT_CCMP=m
CONFIG_LIB80211_CRYPT_TKIP=m
# CONFIG_LIB80211_DEBUG=y

CONFIG_BT=m
CONFIG_COMPAT_BT_L2CAP=y
CONFIG_COMPAT_BT_SCO=y
CONFIG_BT_RFCOMM=m
CONFIG_BT_RFCOMM_TTY=y
CONFIG_BT_BNEP=m
CONFIG_BT_BNEP_MC_FILTER=y
CONFIG_BT_BNEP_PROTO_FILTER=y
# CONFIG_BT_CMTP depends on ISDN_CAPI
ifdef CONFIG_ISDN_CAPI
CONFIG_BT_CMTP=m
endif #CONFIG_ISDN_CAPI
ifndef CONFIG_COMPAT_KERNEL_2_6_28
CONFIG_COMPAT_BT_HIDP=m
endif #CONFIG_COMPAT_KERNEL_2_6_28

CONFIG_BT_HCIUART=M
CONFIG_BT_HCIUART_H4=y
CONFIG_BT_HCIUART_BCSP=y
CONFIG_BT_HCIUART_ATH3K=y
CONFIG_BT_HCIUART_LL=y

CONFIG_BT_HCIVHCI=m
CONFIG_BT_MRVL=m

ifdef CONFIG_PCMCIA
CONFIG_BT_HCIDTL1=m
CONFIG_BT_HCIBT3C=m
CONFIG_BT_HCIBLUECARD=m
CONFIG_BT_HCIBTUART=m
endif #CONFIG_PCMCIA


# We need CONFIG_WIRELESS_EXT for CONFIG_CFG80211_WEXT for every kernel 
# version. The new way CONFIG_CFG80211_WEXT is called from the kernel 
# does not work with compat-wireless because it calls some callback 
# function on struct wiphy. This struct is shipped with compat-wireless 
# and changes from kernel version to version. We are using the 
# wireless_handlers attribute which will be activated by 
# CONFIG_WIRELESS_EXT. 
ifdef CONFIG_WIRELESS_EXT
CONFIG_CFG80211_WEXT=y
else #CONFIG_CFG80211_WEXT
$(warning "WARNING: CONFIG_CFG80211_WEXT will be deactivated or not working because kernel was compiled with CONFIG_WIRELESS_EXT=n. Tools using wext interface like iwconfig will not work. To activate it build your kernel e.g. with CONFIG_LIBIPW=m.")
endif #CONFIG_WIRELESS_EXT

ifdef CONFIG_STAGING
CONFIG_COMPAT_STAGING=m
endif #CONFIG_STAGING

# mac80211 test driver
CONFIG_MAC80211_HWSIM=m

CONFIG_ATH5K=m
# CONFIG_ATH5K_DEBUG=y
# CONFIG_ATH5K_AHB=y

CONFIG_ATH9K=m
CONFIG_ATH9K_HW=m
CONFIG_ATH9K_COMMON=m
# CONFIG_ATH9K_DEBUGFS=y
# CONFIG_ATH9K_AHB=y
# CONFIG_ATH9K_PKTLOG=y

# Disable this to get minstrel as default, we leave the ath9k
# rate control algorithm as the default for now as that is also
# default upstream on the kernel. We will move this to minstrel
# as default once we get minstrel properly tested and blessed by
# our systems engineering team. CCK rates also need to be used
# for long range considerations.
CONFIG_ATH9K_RATE_CONTROL=y

# PCI Drivers
ifdef CONFIG_PCI

CONFIG_ATH5K_PCI=y
CONFIG_ATH9K_PCI=y

CONFIG_IWLAGN=m
# CONFIG_IWLWIFI_DEBUG=y
# CONFIG_IWLWIFI_DEBUGFS=y
# CONFIG_IWLWIFI_DEVICE_TRACING=y
# CONFIG_IWLWIFI_DEBUG_EXPERIMENTAL_UCODE=y
CONFIG_IWL_P2P=y

CONFIG_IWLWIFI_LEGACY=m
CONFIG_COMPAT_IWL4965=m
CONFIG_IWL3945=m
# CONFIG_IWLWIFI_LEGACY_DEBUG=y
# CONFIG_IWLWIFI_LEGACY_DEBUGFS=y
# CONFIG_IWLWIFI_LEGACY_DEVICE_TRACING=y


CONFIG_B43=m
CONFIG_B43_HWRNG=y
CONFIG_B43_PCI_AUTOSELECT=y
ifdef CONFIG_PCMCIA
CONFIG_B43_PCMCIA=y
endif #CONFIG_PCMCIA
ifdef CONFIG_MAC80211_LEDS
CONFIG_B43_LEDS=y
endif #CONFIG_MAC80211_LEDS
CONFIG_B43_PHY_LP=y
CONFIG_B43_PHY_N=y
# CONFIG_B43_FORCE_PIO=y
# CONFIG_B43_DEBUG=y

CONFIG_B43LEGACY=m
CONFIG_B43LEGACY_HWRNG=y
CONFIG_B43LEGACY_PCI_AUTOSELECT=y
ifdef CONFIG_MAC80211_LEDS
CONFIG_B43LEGACY_LEDS=y
endif #CONFIG_MAC80211_LEDS
# CONFIG_B43LEGACY_DEBUG=y
CONFIG_B43LEGACY_DMA=y
CONFIG_B43LEGACY_PIO=y

ifdef CONFIG_WIRELESS_EXT
# The Intel ipws
CONFIG_LIBIPW=m
# CONFIG_LIBIPW_DEBUG=y

CONFIG_IPW2100=m
CONFIG_IPW2100_MONITOR=y
# CONFIG_IPW2100_DEBUG=y
CONFIG_IPW2200=m
CONFIG_IPW2200_MONITOR=y
CONFIG_IPW2200_RADIOTAP=y
CONFIG_IPW2200_PROMISCUOUS=y
CONFIG_IPW2200_QOS=y
# CONFIG_IPW2200_DEBUG=y
# The above enables use a second interface prefixed 'rtap'.
#           Example usage:
#
# % modprobe ipw2200 rtap_iface=1
# % ifconfig rtap0 up
# % tethereal -i rtap0
#
# If you do not specify 'rtap_iface=1' as a module parameter then
# the rtap interface will not be created and you will need to turn
# it on via sysfs:
#
# % echo 1 > /sys/bus/pci/drivers/ipw2200/*/rtap_iface
endif #CONFIG_WIRELESS_EXT

ifdef CONFIG_SSB
# Sonics Silicon Backplane
CONFIG_SSB_SPROM=y

CONFIG_SSB_BLOCKIO=y
CONFIG_SSB_PCIHOST=y
CONFIG_SSB_B43_PCI_BRIDGE=y
ifdef CONFIG_PCMCIA
CONFIG_SSB_PCMCIAHOST=y
endif #CONFIG_PCMCIA
# CONFIG_SSB_DEBUG=y
CONFIG_SSB_DRIVER_PCICORE=y
endif #CONFIG_SSB

CONFIG_P54_PCI=m

CONFIG_B44=m
CONFIG_B44_PCI=y

CONFIG_RTL8180=m

CONFIG_ADM8211=m

CONFIG_RT2X00_LIB_PCI=m
CONFIG_RT2400PCI=m
CONFIG_RT2500PCI=m
ifdef CONFIG_CRC_CCITT
CONFIG_RT2800PCI=m
CONFIG_RT2800PCI_RT33XX=y
CONFIG_RT2800PCI_RT35XX=y
# CONFIG_RT2800PCI_RT53XX=y
endif #CONFIG_CRC_CCITT
NEED_RT2X00=y

# Two rt2x00 drivers require firmware: rt61pci and rt73usb. They depend on
# CRC to check the firmware. We check here first for the PCI
# driver as we're in the PCI section.
ifdef CONFIG_CRC_ITU_T
CONFIG_RT61PCI=m
endif #CONFIG_CRC_ITU_T

CONFIG_MWL8K=m

# Ethernet drivers go here
CONFIG_ATL1=m
CONFIG_ATL2=m
CONFIG_ATL1E=m
ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_ATL1C=n
else #CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_ATL1C=m
endif #CONFIG_COMPAT_KERNEL_2_6_27

ifdef CONFIG_WIRELESS_EXT
CONFIG_HERMES=m
CONFIG_HERMES_CACHE_FW_ON_INIT=y
ifdef CONFIG_PPC_PMAC
CONFIG_APPLE_AIRPORT=m
endif #CONFIG_PPC_PMAC
CONFIG_PLX_HERMES=m
CONFIG_TMD_HERMES=m
CONFIG_NORTEL_HERMES=m
CONFIG_PCI_HERMES=m
ifdef CONFIG_PCMCIA
CONFIG_PCMCIA_HERMES=m
CONFIG_PCMCIA_SPECTRUM=m
endif #CONFIG_PCMCIA
endif #CONFIG_WIRELESS_EXT

CONFIG_RTL8192CE=m
CONFIG_RTL8192DE=m
CONFIG_RTL8192SE=m
CONFIG_RTL8723E=m

ifdef CONFIG_COMPAT_STAGING
CONFIG_BRCMSMAC=m
endif #CONFIG_COMPAT_STAGING

endif #CONFIG_PCI
## end of PCI

ifdef CONFIG_PCMCIA

ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS=n
CONFIG_LIBERTAS_CS=n
else #CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_CS=m
NEED_LIBERTAS=y
endif #CONFIG_COMPAT_KERNEL_2_6_27

endif #CONFIG_PCMCIA
## end of PCMCIA

# This is required for some cards
CONFIG_EEPROM_93CX6=m

# USB Drivers
ifdef CONFIG_USB
ifndef CONFIG_COMPAT_KERNEL_2_6_29
CONFIG_COMPAT_ZD1211RW=m
# CONFIG_ZD1211RW_DEBUG=y
endif #CONFIG_COMPAT_KERNEL_2_6_29

# Sorry, rndis_wlan uses cancel_work_sync which is new and can't be done in compat...

# Wireless RNDIS USB support (RTL8185 802.11g) A-Link WL54PC
# All of these devices are based on Broadcom 4320 chip which
# is only wireless RNDIS chip known to date.
# Note: this depends on CONFIG_USB_NET_RNDIS_HOST and CONFIG_USB_NET_CDCETHER
# it also requires new RNDIS_HOST and CDC_ETHER modules which we add
ifdef CONFIG_COMPAT_KERNEL_2_6_29
CONFIG_USB_COMPAT_USBNET=n
CONFIG_USB_NET_COMPAT_RNDIS_HOST=n
CONFIG_USB_NET_COMPAT_RNDIS_WLAN=n
CONFIG_USB_NET_COMPAT_CDCETHER=n
else #CONFIG_COMPAT_KERNEL_2_6_29
CONFIG_USB_COMPAT_USBNET=m
ifdef CONFIG_USB_NET_CDCETHER
CONFIG_USB_NET_COMPAT_RNDIS_HOST=m
CONFIG_USB_NET_COMPAT_RNDIS_WLAN=m
endif #CONFIG_USB_NET_CDCETHER
ifdef CONFIG_USB_NET_CDCETHER_MODULE
CONFIG_USB_NET_COMPAT_RNDIS_HOST=m
CONFIG_USB_NET_COMPAT_RNDIS_WLAN=m
endif #CONFIG_USB_NET_CDCETHER
CONFIG_USB_NET_COMPAT_CDCETHER=m
endif #CONFIG_COMPAT_KERNEL_2_6_29


CONFIG_P54_USB=m
CONFIG_RTL8187=m
ifdef CONFIG_MAC80211_LEDS
CONFIG_RTL8187_LEDS=y
endif #CONFIG_MAC80211_LEDS

CONFIG_AT76C50X_USB=m

ifndef CONFIG_COMPAT_KERNEL_2_6_29
CONFIG_CARL9170=m
ifdef CONFIG_MAC80211_LEDS
CONFIG_CARL9170_LEDS=y
endif #CONFIG_MAC80211_LEDS
# CONFIG_CARL9170_DEBUGFS=y
CONFIG_CARL9170_WPC=y
endif #CONFIG_COMPAT_KERNEL_2_6_29

# This activates a threading fix for usb urb.
# this is mainline commit: b3e670443b7fb8a2d29831b62b44a039c283e351
# This fix will be included in some stable releases.
CONFIG_COMPAT_USB_URB_THREAD_FIX=y

CONFIG_ATH9K_HTC=m
# CONFIG_ATH9K_HTC_DEBUGFS=y

# RT2500USB does not require firmware
CONFIG_RT2500USB=m
ifdef CONFIG_CRC_CCITT
CONFIG_RT2800USB=m
CONFIG_RT2800USB_RT33XX=y
CONFIG_RT2800USB_RT35XX=y
# CONFIG_RT2800USB_RT53XX=y
CONFIG_RT2800USB_UNKNOWN=y
endif #CONFIG_CRC_CCITT
CONFIG_RT2X00_LIB_USB=m
NEED_RT2X00=y
# RT73USB requires firmware
ifdef CONFIG_CRC_ITU_T
CONFIG_RT73USB=m
endif #CONFIG_CRC_ITU_T

ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_THINFIRM_USB=n
CONFIG_LIBERTAS_USB=n
NEED_LIBERTAS=n
else #CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_THINFIRM_USB=m
CONFIG_LIBERTAS_USB=m
NEED_LIBERTAS=y
endif #CONFIG_COMPAT_KERNEL_2_6_27

CONFIG_ORINOCO_USB=m

CONFIG_BT_HCIBTUSB=m
CONFIG_BT_HCIBCM203X=m
CONFIG_BT_HCIBPA10X=m
CONFIG_BT_HCIBFUSB=m
CONFIG_BT_ATH3K=m


endif #CONFIG_USB end of USB driver list

ifdef CONFIG_SPI_MASTER
ifndef CONFIG_COMPAT_KERNEL_2_6_25

ifdef CONFIG_CRC7
CONFIG_WL1251_SPI=m
CONFIG_WL12XX_SPI=m
endif #CONFIG_CRC7
CONFIG_P54_SPI=m

ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_SPI=n
NEED_LIBERTAS=n
else #CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_SPI=m
NEED_LIBERTAS=y
endif #CONFIG_COMPAT_KERNEL_2_6_27

endif #CONFIG_COMPAT_KERNEL_2_6_25
endif #CONFIG_SPI_MASTER end of SPI driver list

ifdef CONFIG_MMC

CONFIG_SSB_SDIOHOST=y
CONFIG_B43_SDIO=y

ifdef CONFIG_CRC7
ifdef CONFIG_WL12XX_PLATFORM_DATA
CONFIG_COMPAT_WL1251_SDIO=m
endif #CONFIG_WL12XX_PLATFORM_DATA

ifndef CONFIG_COMPAT_KERNEL_2_6_32
ifdef CONFIG_WL12XX_PLATFORM_DATA
CONFIG_COMPAT_WL12XX_SDIO=m
endif #CONFIG_WL12XX_PLATFORM_DATA
endif #CONFIG_COMPAT_KERNEL_2_6_32

endif #CONFIG_CRC7

CONFIG_MWIFIEX_SDIO=m

ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_SDIO=n
NEED_LIBERTAS=n
else #CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS_SDIO=m
NEED_LIBERTAS=y
endif #CONFIG_COMPAT_KERNEL_2_6_27

CONFIG_IWM=m
# CONFIG_IWM_DEBUG=y

CONFIG_BT_HCIBTSDIO=m
CONFIG_BT_MRVL_SDIO=m

ifdef CONFIG_COMPAT_STAGING
ifdef CONFIG_WIRELESS_EXT
# CONFIG_ATH6KL_DISABLE_TARGET_DBGLOGS is not set
# CONFIG_ATH6KL_ENABLE_COEXISTENCE is not set
# CONFIG_ATH6KL_ENABLE_HOST_DEBUG is not set
CONFIG_ATH6KL_ENABLE_TARGET_DEBUG_PRINTS=y
# CONFIG_ATH6KL_HCI_BRIDGE is not set
# CONFIG_ATH6KL_HTC_RAW_INTERFACE is not set
# CONFIG_ATH6KL_SKIP_ABI_VERSION_CHECK is not set
CONFIG_ATH6KL_VIRTUAL_SCATTER_GATHER=y
CONFIG_ATH6K_LEGACY=m

CONFIG_BRCMFMAC=m

endif #CONFIG_WIRELESS_EXT
endif #CONFIG_COMPAT_STAGING


endif #CONFIG_MMC

CONFIG_RTLWIFI=m

# Common rt2x00 requirements
ifeq ($(NEED_RT2X00),y)
CONFIG_RT2X00=y
CONFIG_RT2X00_LIB=m
CONFIG_RT2800_LIB=m
CONFIG_RT2X00_LIB_FIRMWARE=y
CONFIG_RT2X00_LIB_CRYPTO=y
# CONFIG_RT2X00_LIB_SOC=y
ifdef CONFIG_COMPAT_KERNEL_2_6_25
CONFIG_RT2X00_LIB_LEDS=n
else #CONFIG_COMPAT_KERNEL_2_6_25
ifdef CONFIG_LEDS_CLASS
CONFIG_RT2X00_LIB_LEDS=y
endif #CONFIG_LEDS_CLASS
endif #CONFIG_COMPAT_KERNEL_2_6_25
# CONFIG_RT2X00_DEBUG=y
# CONFIG_RT2X00_LIB_DEBUGFS
endif

# p54
CONFIG_P54_COMMON=m
ifdef CONFIG_MAC80211_LEDS
CONFIG_P54_LEDS=y
endif #CONFIG_MAC80211_LEDS

# Atheros
CONFIG_ATH_COMMON=m
# CONFIG_ATH_DEBUG=y

ifdef CONFIG_COMPAT_STAGING
CONFIG_BRCMUTIL=m
# CONFIG_BRCMDBG=y
endif #CONFIG_COMPAT_STAGING

ifdef CONFIG_CRC7
CONFIG_WL1251=m
CONFIG_WL12XX=m
CONFIG_WL12XX_HT=y
endif #CONFIG_CRC7

CONFIG_MWIFIEX=m

ifdef CONFIG_COMPAT_KERNEL_2_6_27
CONFIG_LIBERTAS=n
else #CONFIG_COMPAT_KERNEL_2_6_27
ifeq ($(NEED_LIBERTAS),y)
CONFIG_LIBERTAS_THINFIRM=m
CONFIG_LIBERTAS=m
CONFIG_LIBERTAS_MESH=y
# CONFIG_LIBERTAS_DEBUG=y
endif
endif #CONFIG_COMPAT_KERNEL_2_6_27

# We need the backported rfkill module on kernel < 2.6.31.
# In more recent kernel versions use the in kernel rfkill module.
ifdef CONFIG_COMPAT_KERNEL_2_6_31
CONFIG_RFKILL_BACKPORT=m
ifdef CONFIG_LEDS_TRIGGERS
CONFIG_RFKILL_BACKPORT_LEDS=y
endif #CONFIG_LEDS_TRIGGERS
CONFIG_RFKILL_BACKPORT_INPUT=y
endif #CONFIG_COMPAT_KERNEL_2_6_31

