#/bin/sh

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

runningUbuntu () 
{ 
    uname -a | grep --color=auto "Ubuntu" > /dev/null
}

doSwitch () 
{
    BRANCH="$1"
    CUR_BRANCH="$(git status --branch | head -1 | sed 's/# On branch //g')"
    if $(git status | grep "$BRANCH" > /dev/null); then
        echo "Yes"
    else
        read -p "No (Current branch $CUR_BRANCH).  Should I switch it to $BRANCH for you?  (y/n): " input
        if [ "$input" = "y" ]; then
            git checkout -f $BRANCH
        else
            echo "OK, but if the build fails come back here and try again."
        fi
    fi
}

echo "Verifying a sane branch for your kernel version..."

if inGitRepo; then
    if $(uname -r | grep "3.13" > /dev/null); then
        doSwitch "fedora-20"
    elif $(uname -r | grep "3.12" > /dev/null); then
        doSwitch "fedora-20"
    elif $(uname -r | grep "3.11" > /dev/null); then
        if [ runningFedora ]; then
            doSwitch "fedora-20"
        else
            doSwitch "ubuntu-13.10"
        fi
    elif $(uname -r | grep "3.8" > /dev/null); then
        doSwitch "ubuntu-13.04"
    elif $(uname -r | grep "3.2" > /dev/null); then
        doSwitch "ubuntu-12.04"
    else
        echo "You are running kernel $(uname -r), which is not well supported."
        echo "See the README.md for recommended branch."
        read -p "(<Enter> to continue with build or Ctrl+C to quit): " input
    fi
else
    echo -e "\nYou don't appear to be in a Git checkout.\nThis means branch information is not available.\nYou can still proceed to build, but you might be using unstable code."
    read -p "Would you like me to try and get a git checkout for you? (Y/N): " checkout

    if [ "$checkout" = "y" -o "$checkout" = "Y" ]; then
        if runningFedora; then
            sudo yum -y install kernel-devel kernel-headers
            sudo yum -y groupinstall "Development Tools"
            sudo yum -y groupinstall "C Development Tools and Libraries"
            sudo yum -y install git
        elif runningUbuntu; then
            sudo apt-get -y install gcc build-essential linux-headers-generic linux-headers-$(uname -r)
            sudo apt-get -y install git
        fi

        dirname="rtl8188ce-linux-driver"
        deleteok="NOT_ASKED"

        if [ -d "$dirname" ]; then
            echo ""
            read -p "The target directory already exists.  Is it ok if I delete it? (I'll be replacing it) (Y/N): " deleteok
            if [ "$deleteok" = "Y" -o "$deleteok" = "y" ]; then
                rm -rf "$dirname"
            fi
        fi

        if [ ! -d "$dirname" ]; then
            git clone https://github.com/FreedomBen/rtl8188ce-linux-driver.git
            if [ -d "$dirname" ]; then
                rm -rf *.c *.o *.h *.bz2 *.ko compat/ firmware/ Kconfig Makefile modules.order Module.symvers rtl8188ee/ rtl8188ce/ rtl8192* rtl8723e
                echo -e "\nLooks like the clone was successful (new folder is $(readlink -f $dirname))"
                echo -e "Kicking off the build...\n"
                cd "$dirname" && make
                if (( $? )); then
                    echo -e "\nBuild exited with success status."
                    read -p "Please press <Ctrl+C> to exit (if you press enter you may get a false failure message)" throwaway
                else
                    echo -e "\nBuild exited with failure status (Exit code $?)."
                    read -p "Please press <Ctrl+C> to exit (if you press enter you may get a false failure message)" throwaway
                fi
            else
                echo -e "\nSomething went wrong and I couldn't clone the repo."
                read -p "Press <Enter> to continue with this build, or <Ctrl+C> to cancel" throwaway
            fi
        else
            echo -e "\nDid not clone the git repo because the target already exists"
            read -p "Press <Enter> to continue with this build, or <Ctrl+C> to cancel" throwaway
        fi
    else
        read -p "Press <Enter> to continue with the build, or <Ctrl+C> to cancel" throwaway
    fi
fi
