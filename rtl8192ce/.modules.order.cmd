cmd_drivers/net/wireless/realtek/rtlwifi/rtl8192ce/modules.order := {   echo drivers/net/wireless/realtek/rtlwifi/rtl8192ce/rtl8192ce.ko; :; } | awk '!x[$$0]++' - > drivers/net/wireless/realtek/rtlwifi/rtl8192ce/modules.order
