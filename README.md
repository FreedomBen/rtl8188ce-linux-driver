rtl8188ce-linux-driver
======================

This modified version of the RealTek WiFi driver fixes some issues with RealTek cards on Linux.

**Why use this driver?**

1.  It has been modified to allow you to transmit at up to 33 dBm instead of the stock driver hard limit of 20 dBm, regardless of your CRDA regulatory domain.  This is a substantial increase in capability because every 3 dB increase is equivalent to a doubling of the power.  IOW, you can pump out 4x more Tx power than before.  This is subject to CRDA restrictions however (though you can set that to whatever you want, just make sure you're staying legal)
2.  It has a few default settings that generally increase stability
3.  Some helpful fixes are backported from newer kernels so that they can be enjoyed without upgrading the entire kernel (very useful for staying on your distros current kernel while benefitting from fixes relating to this driver)

Well supported RealTek cards:

        RTL8188CE
        RTL8192CE
        
Well supported kernel releases:

        3.2.x
        3.8.x
        3.11.x
        3.13.x
        3.14.x
        3.15.x
        3.16.x
        3.17.x
        3.18.x
        3.19.x
        4.0.x
        4.1.x
        4.2.x
        4.3.x
        4.4.x
        4.5.x
        4.6.x
        4.7.x
        4.8.x
        4.9.x
        4.10.x
        4.11.x
        4.12.x
        4.13.x
        4.14.x

Well supported Ubuntu (and Ubuntu-based) releases:

        Ubuntu 12.04 (LTS)
        Ubuntu 13.04
        Ubuntu 13.10
        Ubuntu 14.04 (LTS)
        Ubuntu 14.10
        Ubuntu 15.04
        Ubuntu 15.10
        Ubuntu 16.04 (LTS)
        Ubuntu 16.10
        Ubuntu 17.04

        Elementary OS Luna
        Elementary OS Isis

        Linux Mint 13   Maya (LTS)
        Linux Mint 15   Olivia
        Linux Mint 16   Petra
        Linux Mint 17   Qiana (LTS)
        Linux Mint 17.1 Rebecca (LTS)

Well supported Fedora (and Fedora based) releases:

        Fedora 19
        Fedora 20
        Fedora 21
        Fedora 22
        Fedora 23
        Fedora 24
        Fedora 25
        Fedora 26

        Red Hat Enterprise Linux 7.0 (LTS)
        Red Hat Enterprise Linux 7.1 (LTS)
        Red Hat Enterprise Linux 7.2 (LTS)
        Red Hat Enterprise Linux 7.3 (LTS)

        CentOS 7.0 (LTS)
        CentOS 7.1 (LTS)
        CentOS 7.2 (LTS)
        CentOS 7.3 (LTS)

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
        

Please note that I have only tested the driver on Ubuntu and Fedora (the well supported releases).  If you are NOT using Ubuntu, pay close attention to your kernel version when selecting branches.  You also need to substitute your package manager whenever you see `apt-get install`, so Fedora would be `dnf install xxx`.  Most package names are the same but there may be some difference.


Automatic Installation:
-----------------------

There is an install script called install.sh which attempts to automate the manual steps.  If the script fails to build and install the driver, you should follow the steps for manual installation.
    
After cloning this repo, if you want to run the install script, simply run:

    ./install.sh

If you already have `git` installed, you can run this one-liner to clone the repo and kick of the automatic install script:

    git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git && cd rtl8188ce-linux-driver && ./install.sh

You may wish to register the modules with DKMS for automatic rebuild when your kernel changes.  For that, see the steps for Semi-Automatic Installation (DKMS).


Semi-Automatic Installation (DKMS):
-----------------------------------

*Thanks to saiarcot895 for the initial effort getting this working*

You can alternatively use DKMS, so that when a new patch-release of the kernel is released by your distro, you don't have to manually recompile the driver. However, there are a few drawbacks with this method:

1. You need to know what kernel version you are on.

2. You need to change the branch when you change the minor version of the kernel (e.g. 3.13.x to 3.15.x).

To use DKMS:

1. Follow steps 0-2 in the "Manual Installation" section below.

1. Copy the repo `cp -rv . /usr/src/rtlwifi-1.0.0`


2. Run the following command to register the sources with DKMS (the `1.0.0` can be any numbers): (note the period at the end!)

    sudo dkms add rtlwifi/1.0.0 .

3. Run the following command to build and install the modules:

    sudo dkms install rtlwifi/1.0.0

4. DKMS should automatically switch you onto the new modules. To verify this, run `./am_i_using_this_driver.sh`.

To uninstall and remove the modules from DKMS, run the following command:

    sudo dkms remove rtlwifi/1.0.0 --all

Manual Installation:
--------------------

To check your kernel version:  

    uname -r

These commands should be typed in an open terminal.  I recommend you start in your home directory by typing `cd`

0\.  Clone this git repository:

Install git if necessary:

    sudo apt-get install git
    
    or

    dnf install git

Clone the repo (Basically it makes a copy of the current source code)

    git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git


1\.  Install build dependencies:

    Ubuntu:

    apt-get install gcc build-essential linux-headers-generic linux-headers-$(uname -r)

    Fedora:

    dnf install kernel-devel kernel-headers
    dnf groupinstall "Development Tools"
    dnf groupinstall "C Development Tools and Libraries"

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
    Ubuntu 14.04 | Kernel 3.13.x | ubuntu-14.04
    Ubuntu 14.10 | Kernel 3.16.x | ubuntu-14.10
    Ubuntu 15.04 | Kernel 3.19.x | ubuntu-15.04
    Ubuntu 15.10 | Kernel 4.2.x  | ubuntu-15.10
    Ubuntu 16.04 | Kernel 4.4.x  | ubuntu-16.04
    Ubuntu 16.10 | Kernel 4.8.x  | ubuntu-16.10
    Ubuntu 17.04 | Kernel 4.10.x | ubuntu-17.04
    Mint 17      | Kernel 3.13.x | mint-17
    --------------------------------------------
    Any 3.13.x   | Kernel 3.13.x | generic-3.13.x
    Any 3.14.x   | Kernel 3.14.x | generic-3.14.x
    Any 3.15.x   | Kernel 3.14.x | generic-3.14.x
    Any 3.16.x   | Kernel 3.16.x | generic-3.16.x
    Any 3.17.x   | Kernel 3.17.x | generic-3.17.x
    Any 3.18.x   | Kernel 3.18.x | generic-3.18.x
    Any 3.19.x   | Kernel 3.19.x | generic-3.19.x
    Any 4.0.x    | Kernel 4.0.x  | generic-4.0.x
    Any 4.1.x    | Kernel 4.1.x  | generic-4.1.x
    Any 4.2.x    | Kernel 4.2.x  | generic-4.2.x
    Any 4.3.x    | Kernel 4.3.x  | generic-4.3.x
    Any 4.4.x    | Kernel 4.4.x  | generic-4.4.x
    Any 4.5.x    | Kernel 4.5.x  | generic-4.5.x
    Any 4.6.x    | Kernel 4.6.x  | generic-4.6.x
    Any 4.7.x    | Kernel 4.7.x  | generic-4.7.x
    Any 4.8.x    | Kernel 4.8.x  | generic-4.8.x
    Any 4.9.x    | Kernel 4.9.x  | generic-4.9.x
    Any 4.10.x   | Kernel 4.10.x | generic-4.10.x
    Any 4.11.x   | Kernel 4.11.x | generic-4.11.x
    Any 4.12.x   | Kernel 4.12.x | generic-4.12.x
    Any 4.13.x   | Kernel 4.13.x | generic-4.13.x
    Any 4.14.x   | Kernel 4.14.x | generic-4.14.x

    * Note, if the Ubuntu/Mint release version and your kernel version conflict,
      go with the branch corresponding to your *kernel version* as
      that is what really matters!

3\. Compile:

    make

4\. Unload the existing rtl kernel modules.

    lsmod | grep ^rtl | awk '{print $1}'                             // to list the modules
    sudo modprobe -r $(lsmod | grep ^rtl | awk '{print $1}' | xargs) // To unload them

4.5\. (Optional) Backup stock drivers

You may wish to make a backup of your stock drivers in case you need to restore them.  This is done for you automatically by the install script but the truly paranoid may wish to do it themselves as well.  

    cp -r /lib/modules/$(uname -r)/kernel/drivers/net/wireless/rtlwifi ~/

Or tarball it up:

    tar -czf "~/rtlwifi.tar.gz" -C "/lib/modules/$(uname -r)/kernel/drivers/net/wireless/rtlwifi" .

**Important**: When restoring this backup manually, make absolutely sure you are putting it back in to the exact same kernel version.  Failure to do this properly may result in an unbootable system.  I suggest you let the script do your backups automatically and use this as a last resort.

5\. Install:
    
    sudo make install

6\. Modprobe in the new driver (if you're not using an RTL8188CE or RTL8192CE, adjust accordingly):

    sudo modprobe rtl8192ce 

*(rtl8192ce is the driver for the RTL8188CE card also)*


7\. The rtl8192ce should load all modules it depends on (`rtlwifi`, `rtl8192c_common`, `mac80211`, `cfg80211`, etc.), but if it doesn't work you may need to modprobe back in the other modules too.  Common modules:

    rtl8192ce, rtlwifi, rtl8192c_common, mac80211, cfg80211

NOTE: Unlike the stock driver, `rtl8192c_common` is only required with kernel >= 3.14


8\. Make persistent by adding this to the end of `/etc/modules` (for Ubuntu), or `/etc/rc.modules` (for Fedora) (if Fedora make sure `/etc/rc.modules` is executable), or `/etc/modules-load.d/rtlwifi.conf` (for Arch). If you don't have an RTL8188CE or RTL8192CE, then substitute the correct kernel module in place of `rtl8192ce`:

    rtl8192ce

NOTE:  By "make persistent", I mean making the loading of the RLT8192CE kernel modules happen automatically at boot time so you don't have to modprobe them in yourself.  If `udev` is seeing your Realtek card (which is usually the case), then it will load the kernel modules for you without this, but putting this in hurts nothing.

You may want to verify your CRDA domain.  For example if you were in Bolivia it would be: `iw reg set BO`
There is more information about CRDA available at: http://ttys1.wordpress.com/2012/04/12/fixing-regulatory-domain-crda-of-realtec-wireless-device-drivers/


Troubleshooting:
----------------

**0\. To check whether you are running this driver or not, run the script:**

    ./am_i_using_this_driver.sh

**1\. If you have loaded the kernel modules (with `modprobe`) and you are using this driver (check with the script in step 0), and you still have no connection, try restarting `wpa_supplicant`.  If you use `systemd`:**

    sudo systemctl restart wpa_supplicant

**2\. If you get the following error:**
    
    FATAL: Error inserting rtl8192ce (/lib/modules/3.8.0-34-generic/kernel/drivers/net/wireless/rtlwifi/rtl8192ce/rtl8192ce.ko): Invalid argument

After running: 

    modprobe rtl8192ce 
    
You may have invalid configuration options in `/etc/modprobe.d/rtl8192ce.conf`.  You can either remove the file or remove any options that are not supported.

**3\. If you're connection seems unstable:**

This driver has been modified to allow up to 33 dBm Tx power (instead of the stock driver limit of 20).  This allows you to crank up the Tx power, which *vastly* improves performance for me. 

To do this, your CRDA regulatory domain must allow it.  You need to be aware of the laws that govern your local area so you are not in violation.  If you do this, you assume all legal liability for your actions.  Also, don't crank your Tx power up to 33 dBm when you're sitting right next to your router.  Almost all wireless routers have AGC built-in that will save them from hot transmitters, but it is better to try not to fry the receiver.  Keep in mind that you're hacking your wi-fi card to bypass the normal safety net(s) that prevent us from doing cool stuff.

A very lenient CRDA domain is Bolivia (BO) that will let you up to 33 dBm.  To set Bolivia, run the following:

    sudo iw reg set BO

To turn up the Tx power, run this (substitute your wireless interface name for <if-name> and desired power level for <value>):

    sudo iwconfig <if-name> txpower <value>

    E.g.:

    sudo iwconfig wlan0 txpower 33

To check the current Tx power, run:

    iwconfig <if-name> | grep "Tx-Power"

The current Tx power will be listed as `Tx-Power=xx dBm`


**4\. If you're getting power drops\*:**

You may have better luck fixing your data rate.  The best rate will vary depending on your Tx power, Rx power, and distance from the router.  You may want to set the rate to be fixed at a speed similar to your internet connection speed unless you're doing other stuff on your LAN.  Basgoosen found his sweet spot to be 24M.  You can set the fixed rate as follows (substitute your wireless interface for \<wlan\>, so for example wlan0):

    sudo iwconfig <wlan> rate fixed
    sudo iwconfig <wlan> rate 24M
    sudo iwconfig <wlan> bit fixed
    sudo iwconfig <wlan> bit 24M

To make this persistent, create a file in `/etc/network/if-up.d` containing the following (substitute your wireless interface for \<wlan\>!):

    #!/bin/sh

    if [ "$IFACE" = "<wlan>" ]; then
        sudo iwconfig <wlan> rate fixed
        sudo iwconfig <wlan> rate 24M
        sudo iwconfig <wlan> bit fixed
        sudo iwconfig <wlan> bit 24M
    fi

\*Thanks to basgoosen for this suggestion

**5\. My kernel package was updated by my distro and now I'm stuck back on the stock driver!**

Yeah unfortunately this does currently happen anytime your kernel package is updated by your distro.  The solution is to rebuild and reinstall this driver.  On the plus side that gives you a chance to pull down the latest changes so that you're up to date.

Please don't save a copy of the drivers compiled under a different version of the kernel to copy back in after the kernel update.  While a clever solution, this could cause undefined and potentially disastrous results if the ABI changes (which it does frequently).  The drivers need to be rebuilt using the new kernel headers to ensure they are compiled correctly.  It may work fine for a few upgrades, but eventually it will leave your system unbootable.

You can run this command to automatically clone this repo and kick off the installer (git must be installed already):

    git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git && cd rtl8188ce-linux-driver && ./install.sh

You might also consider using DKMS if your kernel changes often.  There are instructions near the top of this README.

**6\. I think this driver broke something.  How do I get back to the stock driver?**

You basically have two choices.  Either will get the job done.  When you run `sudo make uninstall`, the make script will try to restore your backup from when you installed.  The system works like this:

* When you run `sudo make install`, the existing drivers are backed up to `~/.rtlwifi-backups` as a precaution (this is new as of 29-May-2014, so if you installed before then, you have no backups.  Sorry)
* When you run `sudo make uninstall`, this location is checked for a backup that matches your current kernel version.  If one cannot be found, then you are stuck with choice #2.  If a suitable backup is found, the script will offer to restore it for you. 

Choice #1 - Use the backup that the install script made for you:\*

    sudo make uninstall

or

    ./restore_backup.sh

* Note that this will look in the home directory of whichever user you are when you run it, so if it doesn't see a backup, try it again with `sudo` so that it will check root's home directory, or as a regular user so it will check that user's directory.*

Choice #2 - Reinstall your distro's kernel package:

This obviously varies by distro.  For Ubuntu, try:

    sudo apt-get install --reinstall linux-image-extra-$(uname -r)

Please consider reporting the failure (or even better, send a pull request), so I can try to fix it.
