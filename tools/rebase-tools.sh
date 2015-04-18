#!/bin/bash

files='*.sh README.md tools/'
declare -a branches=('generic-4.0.x' 'ubuntu-15.04' 'ubuntu-14.10' 'generic-3.19.x' 'mint-17.1' 'generic-3.18.x' 'generic-3.17.x' 'generic-3.13.x' 'mint-17' 'generic-3.16.x' 'ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'fedora-20' 'arch' 'generic-3.14.x' 'fedora-19')

restore='\033[0m'
red='\033[0;31m'
blue='\033[0;34m'
yellow='\033[1;33m'

die ()
{
    echo -e "${red}[ERROR]: $1${restore}"
    exit 1
}

hasChanges ()
{
    git status
    git status | grep "hanges not staged for commit" > /dev/null
}

[ -f README.md ] || die "Did not find README.md. Are you in the right directory?"

git pull

# Copy to temp dir the files we care about
DIR_NAME="/tmp/temp-for-rtl8188ce-driver-rebase"
[ -d "$DIR_NAME" ] && rm -rf "$DIR_NAME"
mkdir -p "$DIR_NAME"
echo -e ${blue}Copying files: $files${restore}
cp -r $files "$DIR_NAME"

for br in ${branches[@]}; do
    git checkout -f $br || continue
    git rebase          || continue

    echo -e "${blue}Copying files onto ${br}${restore}"
    cp -r $DIR_NAME/* ./

    if hasChanges; then
        echo -e "${yellow}There are updates to commit, committing...${br}${restore}"
        git add .
        git commit -m "Update tools to latest from master

    updated by the update script $0"
    else
        echo -e "${blue}There are no updates to commit on ${br}${restore}"
    fi
    
    if [ "$?" != 0 ]; then
        echo -e "${red}Error committing files onto ${br}${restore}"
        continue
    fi

    git push                       
done

[ -d "$DIR_NAME" ] && rm -rf "$DIR_NAME"

