rtl8188ce-linux-driver
======================


This slightly modified version of the driver fixes some issues with RealTek cards on Linux.  It also fixes a bug with ignoring CRDA restrictions


Well supported RealTek cards:

        RTL8188CE
        RTL8192CE
        
Well supported kernel releases:

        3.2.x
        3.8.x
        3.11.x
        3.12.x
        3.13.x
        3.14.x

Well supported Ubuntu (and Ubuntu based) releases:

        Ubuntu 12.04
        Ubuntu 13.04
        Ubuntu 13.10
        Ubuntu 14.04

        Elementary OS Luna
        Elementary OS Isis

        Linux Mint 13 Maya
        Linux Mint 15 Olivia
        Linux Mint 16 Petra

Well supported Fedora (and Fedora based) releases:

        Fedora 19
        Fedora 20

        Red Hat Enterprise Linux 7

        CentOS 7

Arch is also supported, but tends to break each time the kernel is bumped up a minor version (like 3.13.x to 3.14.x).  I try to fix the breakage quickly but this is not how I support my family ;-)

Other RealTek cards, kernel releases, and Linux flavors/releases are supported, but not tested by me (since I don't have them/use them).  From the original readme file:

--This driver supports following RealTek PCIE Wireless LAN NICs:

        RTL8188CE/RTL8192CE
        RTL8191SE/RTL8192SE
        RTL8192DE
        RTL8723AE
        RTL8188EE

--This driver supports following Linux OSes:

        Fedora Core
        Debian
        Mandriva
        Open SUSE
        Gentoo
        MeeGo
        android 2.2 (froyo-x86), etc.
        

Please note that I have only tested the driver on Ubuntu and Fedora (the well supported releases).  If you are NOT using Ubuntu, pay close attention to your kernel version when selecting branches.  You also need to substitute your package manager whenever you see `apt-get install`, so Fedora would be `yum install xxx`.  Most package names are the same but there may be some difference.


Automatic Installation:
-----------------------

There is an install script called install.sh which attempts to automate the manual steps.  If the script fails to build and install the driver, you should follow the steps for manual installation.
    
If you want to run the install script, simply run:

    ./install.sh


Manual Installation:
--------------------

To check your kernel version:  

    uname -r


These commands should be typed in an open terminal.  I recommend you start in your home directory by typing `cd`

0\.  Clone this git repository:

Install git if necessary:

    sudo apt-get install git
    
    or

    yum install git

Clone the repo (Basically it makes a copy of the current source code)

    git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git


1\.  Install build dependencies (pay attention to the backticks!):

    Ubuntu:

    apt-get install gcc build-essential linux-headers-generic linux-headers-`uname -r`

    Fedora:

    yum install kernel-devel kernel-headers
    yum groupinstall "Development Tools"
    yum groupinstall "C Development Tools and Libraries"

    Arch:

    sudo pacman -S git 
    sudo pacman -S linux-headers
    sudo pacman -S base-devel

2\. (Skip unless your first build attempt fails) -> Make sure you are on the correct branch for your kernel version.  The branches are named after Ubuntu releases to which they commonly apply, but the most important factor is not the version of Ubuntu, it is the kernel version.  There is a script called `verify_branch` that will help you find the right branch based on your kernel version.  When you run "make", the script `verify_branch` will be automatically called for you.  It will recommend to you the best branch for your kernel version.  If you tried the recommended branch and your build failed, you may need to try a different one.  If so, decline the offer to automatically switch branches for you.

If you need to, you can switch to the correct branch with:

    git checkout <branch>
    
Ex: "git checkout ubuntu-13.04"

    Release ver. | *Kernel ver.  | Branch Name
    --------------------------------------------
    Ubuntu 12.04 | Kernel 3.2.x  | ubuntu-12.04
    Ubuntu 13.04 | Kernel 3.8.x  | ubuntu-13.04
    Ubuntu 13.10 | Kernel 3.11.x | ubuntu-13.10
    Fedora 19/20 | Kernel 3.13.x | fedora-20
    Ubuntu 14.04 | Kernel 3.13.x | ubuntu-14.04 
    Arch         | Kernel 3.14.x | arch

    * Note, if the Ubuntu/Fedora release version and your kernel version conflict, go with the branch corresponding to your *kernel version* as that is what really matters!


3\. Compile:

    make

4\. Remove existing kernel modules.  You may want to write these down before removing just in case you need to add them back manually after the build (rare but possible).  Record the output of "lsmod | grep ^rtl":

    lsmod | grep ^rtl                     // to list the modules
    sudo modprobe -r $(lsmod | grep ^rtl) // To unload them

5\. Install:

    sudo make install

6\. Modprobe in the new driver (if you're not using an RTL8188CE or RTL8192CE, adjust accordingly):

    sudo modprobe rtl8192ce 

*(rtl8192ce is the driver for the RTL8188CE card also)*


7\. The rtl8192ce should load all modules it depends on (`rtlwifi`, `rtl8192c_common`, `mac80211`, `cfg80211`, etc.), but if it doesn't work you may need to modprobe back in the other modules too (I did).  Common modules:

    rtl8192ce, rtlwifi, rtl8192c_common, mac80211, cfg80211


8\. Make persistent by adding this to the end of "/etc/modules" (for Ubuntu), or "/etc/rc.modules" (for Fedora) (if Fedora make sure /etc/rc.modules is exectuable. If you don't have an RTL8188CE or RTL8192CE, then substitute the correct kernel module in place of `rtl8192ce.ko`:

    rtl8192ce.ko

NOTE:  By "make persistent", I mean making the loading of the RLT8192CE kernel modules happen automatically at boot time so you don't have to modprobe them in yourself.  If `udev` is seeing your Realtek card (which is usually the case), then it will load the kernel modules for you without this, but putting this in hurts nothing.

You may want to verify your CRDA domain.  For example if you were in Bolivia it would be: "iw reg set BO"
There is more information about CRDA available at: http://ttys1.wordpress.com/2012/04/12/fixing-regulatory-domain-crda-of-realtec-wireless-device-drivers/


Troubleshooting:
----------------

0\. To check whether you are running this driver or not, run the script:

    ./am_i_using_this_driver.sh

1\. If you have loaded the kernel modules (with `modprobe`) and you are using this driver (check with the script in step 0), and you still have no connection, try restarting `wpa_supplicant`.  If you use `systemd`:

    sudo systemctl restart wpa_supplicant

2\. If you get the following error:
    
    FATAL: Error inserting rtl8192ce                 (/lib/modules/3.8.0-34-generic/kernel/drivers/net/wireless/rtlwifi/rtl8192ce/rtl8192ce.ko): Invalid argument

After running: 

    modprobe rtl8192ce 
    
You may have invalid configuration options in /etc/modprobe.d/rtl8192ce.conf.  You can either remove the file or remove the debug option as it is no long supported.   

