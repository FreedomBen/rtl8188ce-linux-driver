#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo "Error: Required file functions.sh not present" >&2
    exit 1
else
    . "$(readlink -f functions.sh)"
fi


doSwitch () 
{
    BRANCH="$1"
    CUR_BRANCH="$(git status --branch | head -1 | sed 's/#//g' | sed 's/On branch //g')"
    if $(git status | grep "$BRANCH" > /dev/null); then
        echo "Yes"
    else
        echo "No (Current branch $CUR_BRANCH)"
        echo "Recommended branch is $BRANCH based on your kernel version ($(uname -r))"
        read -p "Should I switch it to $BRANCH for you?  (y/n): " input
        if [ "$input" = "y" ]; then
            git checkout -f $BRANCH
        else
            echo "OK, but if the build fails come back here and try again."
            read -p "Continue with the build? (Y/N): " input
            if ! [ "$input" = "Y" -o "$input" = "y" ]; then
                exit 1
            fi
        fi
    fi
}


echo "Verifying a sane branch for your kernel version..."

if inGitRepo; then
    if $(uname -r | grep "4.14" > /dev/null); then
        doSwitch "generic-4.14.x"
    elif $(uname -r | grep "4.13" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-17.10"
        else
            doSwitch "generic-4.13.x"
        fi
    elif $(uname -r | grep "4.12" > /dev/null); then
        doSwitch "generic-4.12.x"
    elif $(uname -r | grep "4.11" > /dev/null); then
        doSwitch "generic-4.11.x"
    elif $(uname -r | grep "4.10" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-17.04"
        else
            doSwitch "generic-4.10.x"
        fi
    elif $(uname -r | grep "4.9" > /dev/null); then
        doSwitch "generic-4.9.x"
    elif $(uname -r | grep "4.8" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-16.10"
        else
            doSwitch "generic-4.8.x"
        fi
    elif $(uname -r | grep "4.7" > /dev/null); then
        doSwitch "generic-4.7.x"
    elif $(uname -r | grep "4.6" > /dev/null); then
        doSwitch "generic-4.6.x"
    elif $(uname -r | grep "4.5" > /dev/null); then
        doSwitch "generic-4.5.x"
    elif $(uname -r | grep "4.4" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-16.04"
        else
            doSwitch "generic-4.4.x"
        fi
    elif $(uname -r | grep "4.3" > /dev/null); then
        doSwitch "generic-4.3.x"
    elif $(uname -r | grep "4.2" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-15.10"
        else
            doSwitch "generic-4.2.x"
        fi
    elif $(uname -r | grep "4.1" > /dev/null); then
        doSwitch "generic-4.1.x"
    elif $(uname -r | grep "4.0" > /dev/null); then
        doSwitch "generic-4.0.x"
    elif $(uname -r | grep "3.19" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-15.04"
        else
            doSwitch "generic-3.19.x"
        fi
    elif $(uname -r | grep "3.18" > /dev/null); then
        doSwitch "generic-3.18.x"
    elif $(uname -r | grep "3.17" > /dev/null); then
        doSwitch "generic-3.17.x"
    elif $(uname -r | grep "3.16" > /dev/null); then
        if runningUbuntuKernel; then
            doSwitch "ubuntu-14.10"
        else
            doSwitch "generic-3.16.x"
        fi
    elif $(uname -r | grep "3.15" > /dev/null); then
        doSwitch "generic-3.15.x"
    elif $(uname -r | grep "3.14" > /dev/null); then
        doSwitch "generic-3.14.x"
    elif $(uname -r | grep "3.13" > /dev/null); then
        if runningMint171; then
            doSwitch "mint-17.1"
        elif runningMint; then
            doSwitch "mint-17"
        elif runningUbuntuKernel; then
            doSwitch "ubuntu-14.04"
        else
            doSwitch "generic-3.13.x"
        fi
    elif $(uname -r | grep "3.11" > /dev/null); then
        doSwitch "ubuntu-13.10"
    elif $(uname -r | grep "3.8" > /dev/null); then
        doSwitch "ubuntu-13.04"
    elif $(uname -r | grep "3.2" > /dev/null); then
        doSwitch "ubuntu-12.04"
    else
        echo "You are running kernel $(uname -r), which is not well supported."
        echo "See the README.md for recommended branch."
        read -p "Continue with the build? (Y/N): " input
        if ! [ "$input" = "Y" -o "$input" = "y" ]; then
            exit 1
        fi
    fi
else
    base="$(basename $(pwd))"
    if ( $(uname -r | grep "4.14" > /dev/null) && ! $(echo "$base" | grep "generic-4.14.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.13" > /dev/null) && ! $(echo "$base" | grep "generic-4.13.x" > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-17.10"   > /dev/null) || \
       ( $(uname -r | grep "4.12" > /dev/null) && ! $(echo "$base" | grep "generic-4.12.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.11" > /dev/null) && ! $(echo "$base" | grep "generic-4.11.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.10" > /dev/null) && ! $(echo "$base" | grep "generic-4.10.x" > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-17.04"   > /dev/null) || \
       ( $(uname -r | grep "4.9"  > /dev/null) && ! $(echo "$base" | grep "generic-4.9.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.8"  > /dev/null) && ! $(echo "$base" | grep "generic-4.8.x"  > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-16.10"   > /dev/null) || \
       ( $(uname -r | grep "4.7"  > /dev/null) && ! $(echo "$base" | grep "generic-4.7.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.6"  > /dev/null) && ! $(echo "$base" | grep "generic-4.6.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.5"  > /dev/null) && ! $(echo "$base" | grep "generic-4.5.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.4"  > /dev/null) && ! $(echo "$base" | grep "generic-4.4.x"  > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-16.04"   > /dev/null) || \
       ( $(uname -r | grep "4.3"  > /dev/null) && ! $(echo "$base" | grep "generic-4.3.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.2"  > /dev/null) && ! $(echo "$base" | grep "generic-4.2.x"  > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-15.10"   > /dev/null) || \
       ( $(uname -r | grep "4.1"  > /dev/null) && ! $(echo "$base" | grep "generic-4.1.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "4.0"  > /dev/null) && ! $(echo "$base" | grep "generic-4.0.x"  > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "3.19" > /dev/null) && ! $(echo "$base" | grep "generic-3.19.x" > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-15.04"   > /dev/null) || \
       ( $(uname -r | grep "3.18" > /dev/null) && ! $(echo "$base" | grep "generic-3.18.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "3.17" > /dev/null) && ! $(echo "$base" | grep "generic-3.17.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "3.16" > /dev/null) && ! $(echo "$base" | grep "ubuntu-14.10"   > /dev/null) ) && ! $(echo "$base" | grep "generic-3.16.x" > /dev/null) || \
       ( $(uname -r | grep "3.15" > /dev/null) && ! $(echo "$base" | grep "arch"           > /dev/null) ) && ! $(echo "$base" | grep "fedora-20"      > /dev/null) || \
       ( $(uname -r | grep "3.14" > /dev/null) && ! $(echo "$base" | grep "generic-3.14.x" > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "3.13" > /dev/null) && ! $(echo "$base" | grep "ubuntu-14.04"   > /dev/null) ) && ! $(echo "$base" | grep "generic-3.13.x" > /dev/null) || \
       # Add a check for mint-17 and mint-17.1 branches
       ( $(uname -r | grep "3.11" > /dev/null) && ! $(echo "$base" | grep "fedora-20"      > /dev/null) ) && ! $(echo "$base" | grep "ubuntu-13.10"   > /dev/null) || \
       ( $(uname -r | grep "3.8"  > /dev/null) && ! $(echo "$base" | grep "ubuntu-13.04"   > /dev/null) ) ||                                                          \
       ( $(uname -r | grep "3.2"  > /dev/null) && ! $(echo "$base" | grep "ubuntu-12.04"   > /dev/null) )
    then
        echo "No (current branch $base)"
        echo -e "\nYou don't appear to be in a Git checkout.\nThis means branch information is not available.\nYou can still proceed to build, but you might be using unstable code."
        read -p "Would you like me to try and get a git checkout for you? (Y/N): " checkout

        if [ "$checkout" = "y" -o "$checkout" = "Y" ]; then
            installBuildDependencies

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
                    else
                        echo -e "\nBuild exited with failure status (Exit code $?)."
                    fi
                    echo "To install, cd over to $dirname and run \"sudo make install\""
                else
                    echo -e "\nSomething went wrong and I couldn't clone the repo."
                    read -p "Continue with the build? (Y/N): " input
                    if ! [ "$input" = "Y" -o "$input" = "y" ]; then
                        exit 1
                    fi
                fi
            else
                echo -e "\nDid not clone the git repo because the target already exists"
                read -p "Continue with the build? (Y/N): " input
                if ! [ "$input" = "Y" -o "$input" = "y" ]; then
                    exit 1
                fi
            fi
        else
            read -p "Continue with the build? (Y/N): " input
            if ! [ "$input" = "Y" -o "$input" = "y" ]; then
                exit 1
            fi
        fi
    else
        echo "Yes"
    fi
fi
