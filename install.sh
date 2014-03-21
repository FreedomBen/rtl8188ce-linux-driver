#!/bin/sh

if [ ! -f functions.sh ]; then
    echo "Error: Required file functions.sh not present" >&2
    exit
else
    source functions.sh
fi

echo "So you want to live on the wild side and try a different driver for your RealTek wireless card eh?  Awesome!  I'll help you do it."
echo "We are going to build and install the driver from source code, compiled specifically for your machine."
echo ""
echo "If you want to do the build/install manually, there are instructions in the \"README.md\" file."
echo "Please report any bugs/problems at https://github.com/FreedomBen/rtl8188ce-linux-driver"
echo ""
echo "You will need sudo privileges in order to complete this install."
read -p "Press <Enter> when ready to begin, or <Ctrl+C> to quit" throwaway

echo -e "\nLet's install any dependencies you will need in order to build the driver."
installBuildDependencies

echo -e "\nNow let's compile the driver from source"
make
echo -e "\nNow let's copy the files to the right directories"
sudo make install

read -p "Is your wireless card either the RTL8188CE or RTL8192CE? (Y/N): " input

if [ "$input" = "y" -o "$input" = "Y" ]; then
    echo -e "\nNow let's make sure your kernel loads the new modules at boot time"
    makeModuleLoadPersistent

    echo -e "\nTime to modprobe in your new driver.  If this doesn't work, a reboot should fix it and then you'll be running the new driver"

    sudo modprobe -r rtl8192ce
    sudo modprobe -r rtlwifi
    sudo modprobe rtlwifi
    sudo modprobe rtl8192ce

    echo "You can check if you're running the new driver by running the script \"am_i_using_this_driver.sh\""
    echo "If you aren't running the driver, try a reboot and re-run the script."
else
    echo "OK, reboot and you should be running the new driver.  To check, run the script \"am_i_using_this_driver.sh\""
fi

