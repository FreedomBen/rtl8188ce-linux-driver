#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo -e "${red}[*] Error: Required file functions.sh not present${restore}" >&2
    exit
else
    . "$(readlink -f functions.sh)"
fi


if pciDetectsRtl8188ce || pciDetectsRtl8192ce; then

    sudo modprobe -r rtl8192ce
    sudo modprobe -r rtlwifi
    sudo modprobe rtlwifi
    sudo modprobe rtl8192ce

    if usingSystemd; then
        sudo systemctl restart wpa_supplicant
    fi

    ./am_i_using_this_driver.sh
else
    echo -e "${blue}[*] Sorry, I haven't yet added support to this script for cards that are not either RTL8188CE or RTL8192CE\"${restore}"
fi

exit 0
