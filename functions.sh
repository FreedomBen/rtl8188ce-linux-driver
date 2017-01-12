
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
    if $(which lsb_release >/dev/null 2>&1); then
        lsb_release -d | grep --color=auto "Fedora" > /dev/null
    else
        uname -r | grep --color=auto "fc" > /dev/null
    fi
}

runningUbuntu ()
{
    if $(which lsb_release >/dev/null 2>&1); then
        lsb_release -d | grep --color=auto "Ubuntu" > /dev/null
    else
        uname -a | grep --color=auto "Ubuntu" > /dev/null
    fi
}

runningArch ()
{
    if $(which lsb_release >/dev/null 2>&1); then
        lsb_release -d | grep --color=auto "Arch" > /dev/null
    else
        uname -a | grep --color=auto "ARCH" > /dev/null
    fi
}

runningMint ()
{
    if $(which lsb_release >/dev/null 2>&1); then
        lsb_release -d | grep --color=auto "Mint" > /dev/null
    else
        uname -a | grep --color=auto "Ubuntu" > /dev/null
    fi
}

runningMint171 ()
{
    lsb_release -d | grep --color=auto "Rebecca" > /dev/null
}

# Derivatives like Mint and Elementary usually run the Ubuntu kernel so this can be an easy way to detect an Ubuntu derivative
runningUbuntuKernel ()
{
    uname -a | grep --color=auto "Ubuntu" > /dev/null
}

runningKernelLibre ()
{
    uname -r | grep --color=auto "libre" > /dev/null
}

hasDnf ()
{
    which dnf > /dev/null 2>&1
}

installBuildDependencies ()
{
    if ! $(which sudo > /dev/null 2>&1); then
        echo "This script requires sudo to be installed." >&2
        return 2
    fi

    if runningFedora; then
        hasDnf && PKGMAN='dnf' || PKGMAN='yum'
        if runningKernelLibre; then
            sudo $PKGMAN -y install kernel-libre-devel kernel-libre-headers
        else
            sudo $PKGMAN -y install kernel-devel kernel-headers
        fi
        sudo $PKGMAN -y groupinstall "Development Tools"
        sudo $PKGMAN -y groupinstall "C Development Tools and Libraries"
        sudo $PKGMAN -y install git
        return $?
    elif runningArch; then
        sudo pacman -S --noconfirm --needed git
        sudo pacman -S --noconfirm --needed linux-headers
        sudo pacman -S --noconfirm --needed base-devel
        return $?
    elif runningUbuntu || runningUbuntuKernel; then
        LHG=''; LHUR=''
        $(sudo dpkg -s linux-headers-generic >/dev/null 2>&1) && LHG="linux-headers-generic"
        $(sudo dpkg -s linux-headers-$(uname -r) >/dev/null 2>&1) && LHUR="linux-headers-$(uname -r)"
        sudo apt-get -y install gcc build-essential $LHG $LHUR
        sudo apt-get -y install git
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
    elif runningUbuntu || runningUbuntuKernel; then
        file="/etc/modules"
    elif runningArch; then
        file="/etc/modules-load.d/rtlwifi.conf"
    else
        echo "Cannot make persistent; unknown module file" >&2
        return 1
    fi

    not_present="1"
    if [ -f "$file" ]; then
        while read line; do
            if $(echo "$line" | grep "rtl8192ce" > /dev/null); then
                not_present="0"
            fi
        done < "$file"
    fi

    if [ "$not_present" = "1" ]; then
        if [ "$(id -u)" = "0" ]; then
            echo "rtl8192ce" >> "$file"
        else
            sudo sh -c "echo 'rtl8192ce' >> $file"
        fi
    fi
}

usbDetectsRealtekCard ()
{
    lsusb | egrep -i "realtek.*wi[rf][ei]" > /dev/null
}

pciDetectsRealtekCard ()
{
    lspci | egrep -i "realtek.*wi[rf][ei]" > /dev/null
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

runningAnyRtl8192c_common ()
{
    lsmod | grep "rtl8192c_common" > /dev/null
}

runningOurRtlwifi ()
{
    modinfo rtlwifi | grep "Benjamin Porter" > /dev/null
}

runningOurRtl8192ce ()
{
    modinfo rtl8192ce | grep "Benjamin Porter" > /dev/null
}

runningOurRtl8192c_common ()
{
    modinfo rtl8192c_common | grep "Benjamin Porter" > /dev/null
}

runningStockRtlwifi ()
{
    runningAnyRtlwifi && ! runningOurRtlwifi
}

runningStockRtl8192c_common ()
{
    runningAnyRtl8192c_common && ! runningOurRtl8192c_common
}

runningStockRtl8192ce  ()
{
    runningAnyRtl8192ce && ! runningOurRtl8192ce
}

usingSystemd ()
{
    which systemctl > /dev/null 2>&1
}

runningAnyRtl8188ee ()
{
    lsmod | grep "rtl8188ee" > /dev/null
}

runningOurRtl8188ee ()
{
    modinfo rtl8188ee | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8188ee ()
{
    runningAnyRtl8188ee && ! runningOurRtl8188ee
}

runningAnyRtl8192cu ()
{
    lsmod | grep "rtl8192cu" > /dev/null
}

runningOurRtl8192cu ()
{
    modinfo rtl8192cu | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8192cu ()
{
    runningAnyRtl8192cu && ! runningOurRtl8192cu
}

runningAnyRtl8192de ()
{
    lsmod | grep "rtl8192de" > /dev/null
}

runningOurRtl8192de ()
{
    modinfo rtl8192de | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8192de ()
{
    runningAnyRtl8192de && ! runningOurRtl8192de
}

runningAnyRtl8192ee ()
{
    lsmod | grep "rtl8192ee" > /dev/null
}

runningOurRtl8192ee ()
{
    modinfo rtl8192ee | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8192ee ()
{
    runningAnyRtl8192ee && ! runningOurRtl8192ee
}

runningAnyRtl8192se ()
{
    lsmod | grep "rtl8192se" > /dev/null
}

runningOurRtl8192se ()
{
    modinfo rtl8192se | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8192se ()
{
    runningAnyRtl8192se && ! runningOurRtl8192se
}

runningAnyRtl8723ae ()
{
    lsmod | grep "rtl8723ae" > /dev/null
}

runningOurRtl8723ae ()
{
    modinfo rtl8723ae | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8723ae ()
{
    runningAnyRtl8723ae && ! runningOurRtl8723ae
}

runningAnyRtl8723be ()
{
    lsmod | grep "rtl8723be" > /dev/null
}

runningOurRtl8723be ()
{
    modinfo rtl8723be | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8723be ()
{
    runningAnyRtl8723be && ! runningOurRtl8723be
}

runningAnyRtl8821ae ()
{
    lsmod | grep "rtl8821ae" > /dev/null
}

runningOurRtl8821ae ()
{
    modinfo rtl8821ae | grep "Benjamin Porter" > /dev/null
}

runningStockRtl8821ae ()
{
    runningAnyRtl8821ae && ! runningOurRtl8821ae
}

readonly rtlwifi_orig="/lib/modules/$(uname -r)/kernel/drivers/net/wireless/rtlwifi"
readonly rtlwifi_backup_dir="$HOME/.rtlwifi-backup"
readonly rtlwifi_backup_outfile="$rtlwifi_backup_dir/$(uname -r).tar.gz"

askbackup ()
{
    if [ -f "$rtlwifi_backup_outfile" ]; then
        echo -e "You already have a backup of a driver from this kernel version (located at ${rtlwifi_backup_outfile}.\n\nYou can back it up again, however if you have installed this driver already, then backing it up again will overwrite the original backup (which contains the stock drivers) with one that contains these drivers, which is most likely NOT what you want.  I recommend you only proceed with backing up the current drivers if you are sure of what you're doing."

        read -p "Make a backup of the existing stock driver before installing? (Y/[N]): " RESP

        if [ "$RESP" = "Y" -o "$RESP" = "y" ]; then
            echo -e "${blue}Making backup...${restore}"
            rm -r "$rtlwifi_backup_outfile"
            backupCurrent -f
        else
            echo -e "${blue}Not making backup...${restore}"
            # We're not exiting with error since the user declined the backup
            return 0
        fi
    else
        backupCurrent
    fi
}

backupCurrent ()
{
    # don't overwrite an already existing backup without a -f, because we may be backuping up ourselves from a previous install
    if [ -f "$rtlwifi_backup_outfile" ] && [ "$1" != "-f" ]; then
        echo "Not overwritting existing backup without -f flag" >&2
    else
        if [ -d "$rtlwifi_orig" ]; then
            mkdir -p "$rtlwifi_backup_dir"
            tar -czf "$rtlwifi_backup_outfile" -C "$rtlwifi_orig" . > /dev/null 2>&1
        else
            echo "Could not backup rtlwifi because it could not be found!  Expected at $rtlwifi_orig" >&2
        fi
    fi
}

askrestore ()
{
    if [ ! -f "$rtlwifi_backup_outfile" ]; then
        echo "Sorry, no backup matching this kernel version found at $rtlwifi_backup_outfile" >&2
    else
        echo "You have a backup copy of the old driver for this kernel version."
        read -p "Do you want to restore it? (Y/N): " RESP

        if [ "$RESP" = "Y" -o "$RESP" = "y" ]; then
            restoreFromBackup
        else
            return
        fi
    fi
}

restoreFromBackup ()
{
    # Only restore same kernel ver to avoid breaking systems
    if [ -f "$rtlwifi_backup_outfile" ]; then
        # double check that we are the same kernel version. We don't want to screw this up and become the next bumblebee ;-)
        if [ $(basename "$rtlwifi_backup_outfile" | sed -e 's/.tar.gz//g') = "$(uname -r)" ]; then
            sudo rm -rf "$rtlwifi_orig"
            sudo mkdir -p "$rtlwifi_orig"
            dir="$(pwd)"
            cd "$rtlwifi_orig"
            sudo cp "$rtlwifi_backup_outfile" ./
            sudo tar -xzf "$rtlwifi_backup_outfile" > /dev/null 2>&1
            sudo rm -r "$(uname -r).tar.gz"
            cd "$dir"
        else
            echo "Sanity check failed. Please report this bug" >&2
        fi
    else
        echo "Sorry, no backup matching this kernel version found at $rtlwifi_backup_outfile" >&2
    fi
}

