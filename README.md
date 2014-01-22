rtl8188ce-linux-driver
======================


This slightly modified version of the driver fixes some issues with RealTek cards on Linux.  It also fixes a bug with ignoring CRDA restrictions


Instructions to install:

Note:  These instructions were verified on Ubuntu 13.04 running kernel 3.8.0-27-generic #40 and on Ubuntu 13.10 running kernel 3.11

Some instructions can be found here:  http://www.perseosblog.com/en/posts/solving-connection-problem-with-realtek-wifi-card-rtl8188ce-rtl8192ce-rtl8191se-and-rtl8192de-on-debian-ubuntu-and-derivatives/

These commands should be typed in an open terminal.  I recommend you start in your home directory by typing "cd"

0\.  Clone this git repository:

Install git if necessary:

    sudo apt-get install git

Clone the repo (Basically it makes a copy of the current source code)

    git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git

1\.  Install build dependencies (pay attention to the backticks!):

    apt-get install gcc build-essential linux-headers-generic linux-headers-`uname -r`

2\. Make sure you are on the correct branch for your kernel version.  The branches are named after Ubuntu releases to which they commonly apply, but the most important factor is not the version of Ubuntu, it is the kernel version.  There is a script called "verify_branch" that will help you find the right branch based on your kernel version.  

You can switch to the correct branch with:

    git checkout <branch>
    
Ex: "git checkout ubuntu-13.04"

    Release ver. | *Kernel ver.   |  Branch Name
    --------------------------------------------
    Ubuntu 12.04 | Kernel 3.2.x:  |  ubuntu-12.04
    Ubuntu 13.04 | Kernel 3.8.x:  |  ubuntu-13.04
    Ubuntu 13.10 | Kernel 3.11.x: |  ubuntu-13.10

    * Note, if the Ubuntu release ver and your kernel ver conflict, go with the branch corresponding to your *kernel version*

master is currently compiling on 13.10 but for the latest and greatest I recommend you use the specific branch for your OS

3\. Compile:

    make

4\. Remove existing kernel modules.  You may want to write these down before moving just in case.  Record the output of "lsmod | grep ^rtl" (pay attention to the backticks!):

    sudo modprobe -r `lsmod | grep ^rtl`

5\. Install:

    sudo make install

6\. Modprobe in the new driver:

    sudo modprobe rtl8192ce 

(This is the driver for rtl8188ce also)


7\. You may need to modprobe back in the other modules too (I did).  Mine were: 

    rtl8192ce, rtlwifi, and rtl8192c_common

8\. Make persistent by adding this to the end of /etc/modules (for Ubuntu), or /etc/rc.modules (for Fedora) (make sure /etc/rc.modules is exectuable):

    rtl8192ce.ko

You may want to verify your CRDA domain.  For example if you were in Bolivia it would be: "iw reg set BO"
There is more information about CRDA available at: http://ttys1.wordpress.com/2012/04/12/fixing-regulatory-domain-crda-of-realtec-wireless-device-drivers/
