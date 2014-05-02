
# some shared vars and functions used by other scripts

restore='\033[0m'
black='\033[0;30m'
red='\033[0;31m'
green='\033[0;32m'
brown='\033[0;33m'
blue='\033[0;34m'
purple='\033[0;35m'
cyan='\033[0;36m'
light_gray='\033[0;37m'
dark_gray='\033[1;30m'
light_red='\033[1;31m'
light_green='\033[1;32m'
yellow='\033[1;33m'
light_blue='\033[1;34m'
light_purple='\033[1;35m'
light_cyan='\033[1;36m'
white='\033[1;37m'


inGitRepo ()
{
    prevdir="$(pwd)"
    while true; do
        if [ -d ".git" ]; then
            cd "$prevdir"
            return 0
        fi

        if [ "$(pwd)" = "/" ]; then
            cd "$prevdir"
            return 1
        fi

        cd ..
    done
}

runningFedora () 
{ 
    uname -r | grep --color=auto "fc" > /dev/null
}

# also returns true for Linux Mint
runningUbuntu () 
{ 
    uname -a | grep --color=auto "Ubuntu" > /dev/null
}
runningArch ()
{
    uname -a | grep --color=auto "ARCH" > /dev/null
}

installBuildDependencies ()
{
    if ! $(which sudo > /dev/null 2>&1); then
        echo "This script requires sudo to be installed." >&2
        return 2
    fi

    if runningFedora; then
        sudo yum -y install kernel-devel kernel-headers
        sudo yum -y groupinstall "Development Tools"
        sudo yum -y groupinstall "C Development Tools and Libraries"
        sudo yum -y install git
        return $?
    elif runningUbuntu; then
        sudo apt-get -y install gcc build-essential linux-headers-generic linux-headers-$(uname -r)
        sudo apt-get -y install git
        return $?
    elif runningArch; then
        sudo pacman -S git
        sudo pacman -S linux-headers
        sudo pacman -S base-devel
        return $?
    else
        echo "Unknown distro. Please ensure all build dependencies are installed before proceeding (or the compile will fail).  Roughly you need gcc build essentials, linux headers, and git." >&2
        return 1
    fi
}

makeModuleLoadPersistent () 
{
    if runningFedora; then
        file="/etc/rc.modules"
    elif runningUbuntu; then
        file="/etc/modules"
    else
        echo "Cannot make persistent; unknown module file" >&2
        return 1
    fi

    not_present=1
    if [ -f "$file" ]; then
        while read line; do
            if $(echo "$line" | grep "rtl8192ce" > /dev/null); then
                not_present=0
            fi
        done < "$file"
    fi

    if (( $not_present )); then
        echo "rtl8192ce.ko" >> "$file"
    fi
}

usbDetectsRealtekCard ()
{
    lsusb | egrep -i "realtek.*wifi" > /dev/null
}

pciDetectsRealtekCard ()
{
    lspci | egrep -i "realtek.*wifi" > /dev/null
}

pciDetectsRtl8192ce ()
{
    lspci | grep -i "RTL8192CE" > /dev/null
}

pciDetectsRtl8188ce ()
{
    lspci | grep -i "RTL8188CE" > /dev/null
}

runningAnyRtl8192ce ()
{
    lsmod | grep "rtl8192ce" > /dev/null
}

runningAnyRtlwifi ()
{
    lsmod | grep "rtlwifi" > /dev/null
}

runningOurRtlwifi ()
{
    modinfo rtlwifi | grep "Benjamin Porter" > /dev/null
}

runningOurRtl8192ce ()
{
    modinfo rtl8192ce | grep "Benjamin Porter" > /dev/null
}

runningStockRtlwifi ()
{
    runningAnyRtlwifi && ! runningOurRtlwifi
}

runningStockRtl8192ce  ()
{
    runningAnyRtl8192ce && ! runningOurRtl8192ce
}

