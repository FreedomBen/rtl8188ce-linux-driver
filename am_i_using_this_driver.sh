#!/bin/sh

if [ ! -f "functions.sh" ]; then
    echo "Required file \"functions.sh\" not found!"
    exit
fi

. "$(readlink -f functions.sh)"

if runningAnyRtlwifi; then
    if runningOurRtlwifi; then
        echo -e "${green}[*] You are running our rtlwifi${restore}"
    else
        echo -e "${yellow}[*] Not running our rtlwifi (running stock driver)${restore}"
    fi
else
    echo -e "${red}[*] Not running any rtlwifi${restore}"
fi

if runningAnyRtl8192ce; then
    if runningOurRtl8192ce; then
        echo -e "${green}[*] You are running our rtl8192ce${restore}"
    else
        echo -e "${yellow}[*] Not running our rtl8192ce (running stock driver)${restore}"
    fi
else
    echo -e "${red}[*] Not running any rtl8192ce${restore}"
fi

