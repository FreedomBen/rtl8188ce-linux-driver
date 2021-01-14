#!/bin/bash

LOCATION="$HOME/gitclone"

GENERIC='git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git'
UBUNTU1404='git://kernel.ubuntu.com/ubuntu/ubuntu-trusty.git'
UBUNTU1410='git://kernel.ubuntu.com/ubuntu/ubuntu-utopic.git'
UBUNTU1504='git://kernel.ubuntu.com/ubuntu/ubuntu-vivid.git'
UBUNTU1510='git://kernel.ubuntu.com/ubuntu/ubuntu-wily.git'
UBUNTU1604='git://kernel.ubuntu.com/ubuntu/ubuntu-xenial.git'
UBUNTU1610='git://kernel.ubuntu.com/ubuntu/ubuntu-yakkety.git'
UBUNTU1704='git://kernel.ubuntu.com/ubuntu/ubuntu-zesty.git'
UBUNTU1710='git://kernel.ubuntu.com/ubuntu/ubuntu-artful.git'
UBUNTU1804='git://kernel.ubuntu.com/ubuntu/ubuntu-bionic.git'
UBUNTU2004='git://kernel.ubuntu.com/ubuntu/ubuntu-focal.git'

starting_dir="$(pwd)"

die ()
{
    echo "[DIE]: $1" 2>&1
    exit 1
}

is_generic ()
{
    echo "$1" | grep "generic" >/dev/null 2>&1
}

is_ubuntu ()
{
    echo "$1" | grep "ubuntu" >/dev/null 2>&1
}

tag ()
{
    if $(is_generic $1); then
        if $(echo "$1" | grep "5\.10" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.10" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.9" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.9" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.8" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.8" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.7" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.7" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.6" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.6" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.5" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.5" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.4" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.4" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.3" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.3" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.2" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.2" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.1" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.1" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "5\.0" >/dev/null 2>&1); then
            echo "$(git tag | grep "v5\.0" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.19" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.19" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.18" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.18" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.17" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.17" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.16" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.16" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.15" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.15" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.14" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.14" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.13" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.13" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.12" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.12" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.11" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.11" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.10" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.10" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.9" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.9" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.8" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.8" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.7" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.7" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.6" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.6" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.5" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.5" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.4" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.4" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.3" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.3" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.2" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.2" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.1" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.1" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "4\.0" >/dev/null 2>&1); then
            echo "$(git tag | grep "v4\.0" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "3\.19\." >/dev/null 2>&1); then
            echo "$(git tag | grep "v3\.19" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "3\.18\." >/dev/null 2>&1); then
            echo "$(git tag | grep "v3\.18" | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "3\.17\." >/dev/null 2>&1); then
            echo "$(git tag | grep "v3\.17" | sort -n | ~/bin/extract.rb)"
            return
        fi
    elif $(is_ubuntu $1); then
        if $(echo "$1" | grep "14.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-3.13 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "14.10" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-3.16 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "15.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-3.19 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "15.10" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.2 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "16.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.4 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "16.10" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.8 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "17.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.10 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "17.10" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.13 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "18.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-4.15 | sort -n | ~/bin/extract.rb)"
            return
        elif $(echo "$1" | grep "20.04" >/dev/null 2>&1); then
            echo "$(git tag | grep Ubuntu-5.4 | sort -n | ~/bin/extract.rb)"
            return
        fi
    fi
    die "unknown tag for \"$1\""
}

repo_dir ()
{
    echo $1 | sed -e 's/.*\///g' | sed -e 's/\.git//g'
}

branch_exists ()
{
    git branch --remotes | grep "$1" >/dev/null 2>&1
}

if [ "$1" = "ubuntu-14.04" ]; then
    remote="$UBUNTU1404"
elif [ "$1" = "ubuntu-14.10" ]; then
    remote="$UBUNTU1410"
elif [ "$1" = "ubuntu-15.04" ]; then
    remote="$UBUNTU1504"
elif [ "$1" = "ubuntu-15.10" ]; then
    remote="$UBUNTU1510"
elif [ "$1" = "ubuntu-16.04" ]; then
    remote="$UBUNTU1604"
elif [ "$1" = "ubuntu-16.10" ]; then
    remote="$UBUNTU1610"
elif [ "$1" = "ubuntu-17.04" ]; then
    remote="$UBUNTU1704"
elif [ "$1" = "ubuntu-17.10" ]; then
    remote="$UBUNTU1710"
elif [ "$1" = "ubuntu-18.04" ]; then
    remote="$UBUNTU1804"
elif [ "$1" = "ubuntu-20.04" ]; then
    remote="$UBUNTU2004"
elif [[ $1 =~ generic ]]; then
    remote="$GENERIC"
fi

[ -f "regd.c" ] || die "Need to start script from root of dir"

[ -n "$1" ] || die "Need branch to rebase specified"

[ -f "tools/extract.rb" ] && cp tools/extract.rb $HOME/bin/

mkdir -p "$LOCATION"
cd $LOCATION

[ -n "$remote" ] || die "Unsupported branch \"$1\""

[ -d "$(repo_dir $remote)" ] || git clone "$remote"
[ -d "$(repo_dir $remote)" ] || die "could not clone $remote and the dir does not exist"

cd "$(repo_dir $remote)"
git checkout --force master
git pull --rebase || die "Problem running git pull"
echo "Tag in use is $(tag $1)"
git checkout --force "$(tag $1)" || die "Problem checking out $(tag $1)"
temp="$(pwd)"
cd "$starting_dir"

if branch_exists "$1"; then
    git checkout --force "$1"
else
    git checkout --force master
fi

cd "$temp"

if [ -d "drivers/net/wireless/rtlwifi" ]; then
    cp -r drivers/net/wireless/rtlwifi/* "${starting_dir}/"
else
    cp -r drivers/net/wireless/realtek/rtlwifi/* "${starting_dir}/"
fi

cd "$starting_dir"

git checkout Kconfig
find . -name "Makefile" | xargs git checkout -f
~/bin/fixParens.py *.[ch] **/*.[ch]
./tools/addNameToCommentHeader.py *.[ch] **/*.[ch]

echo "Ok, ready for manual merging"
