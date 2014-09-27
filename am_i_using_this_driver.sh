#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo "Required file \"functions.sh\" not found!"
    exit
fi

. "$(readlink -f functions.sh)"

if runningAnyRtlwifi; then
    if runningOurRtlwifi; then
        echo -e "${green}[*] You are running the new rtlwifi${restore}"
    else
        echo -e "${yellow}[*] Not running the new rtlwifi (running stock driver)${restore}"
    fi
else
    echo -e "${red}[*] Not running any rtlwifi${restore}"
fi

if runningAnyRtl8192c_common; then
    if runningOurRtl8192c_common; then
        echo -e "${green}[*] You are running the new rtl8192c_common${restore}"
    else
        echo -e "${yellow}[*] Not running the new rtl8192c_common (running stock driver)${restore}"
    fi
else
    echo -e "${red}[*] Not running any rtl8192c_common${restore}"
fi

if runningAnyRtl8192ce; then
    if runningOurRtl8192ce; then
        echo -e "${green}[*] You are running the new rtl8192ce${restore}"
    else
        echo -e "${yellow}[*] Not running the new rtl8192ce (running stock driver)${restore}"
    fi
else
    echo -e "${red}[*] Not running any rtl8192ce${restore}"
fi

