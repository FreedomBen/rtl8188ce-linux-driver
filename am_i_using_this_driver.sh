#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo "Required file \"functions.sh\" not found!"
    exit
fi

. "$(readlink -f functions.sh)"

runningNew ()
{
    echo -e "${green}[*] You are running the new ${1}${restore}"
}

runningStock ()
{

    echo -e "${yellow}[*] Not running the new $1 (running stock driver)${restore}"
}

runningNone ()
{
    echo -e "${red}[*] Not running any ${1}${restore}"
}

if runningAnyRtlwifi; then
    if runningOurRtlwifi; then
        runningNew "rtlwifi"
    else
        runningStock "rtlwifi"
    fi
else
    runningNone "rtlwifi"
fi

if pciDetectsRtl8188ce || pciDetectsRtl8192ce; then
    if runningAnyRtl8192c_common; then
        if runningOurRtl8192c_common; then
            runningNew "rtl8192c_common"
        else
            runningStock "rtl8192c_common"
        fi
    else
        runningNone "rtl8192c_common"
    fi

    if runningAnyRtl8192ce; then
        if runningOurRtl8192ce; then
            runningNew "rtl8192ce"
        else
            runningStock "rtl8192ce"
        fi
    else
        runningNone "rtl8192ce"
    fi
elif runningAnyRtl8188ee; then
    if runningOurRtl8188ee; then
        runningNew "rtl8188ee"
    else
        runningStock "rtl8188ee"
    fi
elif runningAnyRtl8192cu; then
    if runningOurRtl8192cu; then
        runningNew "rtl8192cu"
    else
        runningStock "rtl8192cu"
    fi
elif runningAnyRtl8192de; then
    if runningOurRtl8192de; then
        runningNew "rtl8192de"
    else
        runningStock "rtl8192de"
    fi
elif runningAnyRtl8192ee; then
    if runningOurRtl8192ee; then
        runningNew "rtl8192ee"
    else
        runningStock "rtl8192ee"
    fi
elif runningAnyRtl8192se; then
    if runningOurRtl8192se; then
        runningNew "rtl8192se"
    else
        runningStock "rtl8192se"
    fi
elif runningAnyRtl8723ae; then
    if runningOurRtl8723ae; then
        runningNew "rtl8723ae"
    else
        runningStock "rtl8723ae"
    fi
elif runningAnyRtl8723be; then
    if runningOurRtl8723be; then
        runningNew "rtl8723be"
    else
        runningStock "rtl8723be"
    fi
elif runningAnyRtl8821ae; then
    if runningOurRtl8821ae; then
        runningNew "rtl8821ae"
    else
        runningStock "rtl8821ae"
    fi
fi
