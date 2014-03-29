#!/bin/sh

restore='\033[0m'
red='\033[0;31m'
green='\033[0;32m'
yellow='\033[1;33m'

if [ ! -f "functions.sh" ]; then
    echo "Required file \"functions.sh\" not found!"
    exit
fi

source "$(readlink -f functions.sh)"

if runningOurDriver; then
    if runningRtlwifiDriver; then
        echo -e "${green}[*] You are running our rtlwifi${restore}"
    else
        echo -e "${yellow}[*] Not running our rtlwifi${restore}"
    fi
    if runningRtl8192ceDriver; then
        echo -e "${green}[*] You are running our rtl8192ce${restore}"
    else
        echo -e "${yellow}[*] Not running our rtl8192ce${restore}"
    fi
elif ! runningStockDriver; then
    if runningRtlwifiDriver; then
        echo -e "${green}[*] You are running our rtlwifi${restore}"
    else
        echo -e "${red}[*] Not running our rtlwifi${restore}"
    fi
    if runningRtl8192ceDriver; then
        echo -e "${green}[*] You are running our rtl8192ce${restore}"
    else
        echo -e "${red}[*] Not running our rtl8192ce${restore}"
    fi
else
    echo "${red}[*] Not running any rtlwifi or rtl8192ce driver${restore}"
fi

