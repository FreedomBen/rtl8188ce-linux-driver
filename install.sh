#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo -e "${red}[*] Error: Required file functions.sh not present${restore}" >&2
    exit
else
    . "$(readlink -f functions.sh)"
fi

if ! $(pciDetectsRealtekCard || usbDetectsRealtekCard); then
    echo -en "${yellow}[*] I wasn't able to find a Realtek card on your machine.  Do you want to proceed anyway? (Y/N)${restore}: "
    read PROCEED

    if ! [ "$PROCEED" = "Y" -o "$PROCEED" = "y" ]; then
        exit 1
    fi
fi

echo -e "\n${blue}[*] So you want to live on the wild side and try a different driver for your RealTek wireless card eh?  Awesome!  I'll help you do it.${restore}"
echo -e "\n${blue}[*] We are going to build and install the driver from source code, compiled specifically for your machine.${restore}"
echo ""
echo -e "${purple}[*] If you want to do the build/install manually, there are instructions in the \"README.md\" file.${restore}"
echo -e "\n${cyan}[*] Please report any bugs/problems at https://github.com/FreedomBen/rtl8188ce-linux-driver${restore}"
echo ""

if [ "$(id -u)" != "0" ]; then
    echo -e "${yellow}[*] You will need sudo privileges in order to complete this install.${restore}"
fi
read -p "[*] Press <Enter> when ready to begin, or <Ctrl+C> to quit" throwaway

echo -e "\n${blue}[*] Let's install any dependencies you will need in order to build the driver.${restore}"
if ! installBuildDependencies; then
    read -p "[*] Dependency install reported failure.  Proceed anyway? (Y/N): " PROCEED

    if ! [ "$PROCEED" = "Y" -o "$PROCEED" = "y" ]; then
        exit 1
    fi
fi

echo -e "\n${blue}[*] Now let's compile the driver from source and copy the files to the right directories${restore}"
make && sudo make install

if [ "$?" != "0" ]; then
    echo -e "${red}[*] The build has failed! Please make sure you have all dependencies installed and you are building on the correct branch (kernel version, not linux distro version, is most important for the branch)${restore}"
    exit 1
fi

if pciDetectsRtl8188ce || pciDetectsRtl8192ce; then
    input="y"
    if pciDetectsRtl8188ce; then
        CARD="RTL8188CE"
    elif pciDetectsRtl8192ce; then
        CARD="RTL8192CE"
    fi

    echo -e "${blue}[*] I see you have an $CARD card${restore}"
else
    read -p "[*] Is your wireless card either the RTL8188CE or RTL8192CE? (Y/N): " input
fi

if [ "$input" = "y" -o "$input" = "Y" ]; then
    echo -e "\n${blue}[*] Now let's make sure your kernel loads the new modules at boot time${restore}"
    makeModuleLoadPersistent

    echo -e "\n${blue}[*] Time to modprobe in your new driver...${restore}"

    sudo modprobe -r rtl8192ce
    sudo modprobe -r rtlwifi
    sudo modprobe rtlwifi
    sudo modprobe rtl8192ce

    if usingSystemd; then
        sudo systemctl restart wpa_supplicant
    fi

    ./am_i_using_this_driver.sh

    echo -e "\n${blue}[*] If you aren't running the driver, try a reboot and re-run the script \"am_i_using_this_driver.sh\".${restore}"
    echo -e "\n${blue}[*] NOTE: If your wifi looks dead, it usually comes back after a reboot.  Don't panic yet.\n${restore}"
    echo -e "${blue}[*] OK, ready for a reboot. (only necessary if your wifi is not working)${restore}"
    echo -e "${blue}[*] You can check if you're running the new driver by running the script \"am_i_using_this_driver.sh\"${restore}"
else
    echo -e "${blue}[*] OK, reboot and you should be running the new driver.  To check, run the script \"am_i_using_this_driver.sh\"${restore}"
fi

exit 0
