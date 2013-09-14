rtl8188ce-linux-driver
======================

This slightly modified version of the driver fixes some issues with RealTek cards on Linux.  It also fixes a bug with ignoring CRPA restrictions


Instruction to install:

Note:  These instructions were verified on Ubuntu 13.04 running kernel 3.8.0-27-generic #40.

Some instructions can be found here:  http://www.perseosblog.com/en/posts/solving-connection-problem-with-realtek-wifi-card-rtl8188ce-rtl8192ce-rtl8191se-and-rtl8192de-on-debian-ubuntu-and-derivatives/

1.  Install build dependencies:
    apt-get install gcc build-essential linux-headers-generic linux-headers-`uname -r`

2. Compile:
    make

3. Remove existing kernel modules.  You may want to write these down before moving just in case.  Record the output of "lsmod | grep ^rtl"
    modprobe -r `lsmod | grep ^rtl`

4. Install:
    make install

5. Modprobe in the new driver
    modprobe rtl8192ce (This is the driver for rtl8188ce also)

6. You may need to modprobe back in the other modules too (I did).  Mine were: rtl8192ce, rtlwifi, and rtl8192c_common

7. Make persistent by adding this to the end of /etc/modules:
    rtl8192ce.ko


You may want to verify your CRDA domain.  For example if you were in Bolivia it would be: "iw reg set BO"
There is more information about CRDA available at: http://ttys1.wordpress.com/2012/04/12/fixing-regulatory-domain-crda-of-realtec-wireless-device-drivers/
