#!/bin/bash

declare -a branches=('ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'fedora-20' 'arch' 'master' 'generic-3.14.x' 'fedora-19')

restore='\033[0m'
red='\033[0;31m'
blue='\033[0;34m'
yellow='\033[1;33m'

if [ -z "$1" ]; then
    echo "Error: No SHAs to cherry-pick"
fi


git pull

for br in ${branches[@]}; do
    # Don't cherry-pick a commit that is already on the branch
    git checkout -f $br || continue
    git rebase          || continue

    for sha in "$@"; do
        if $(git log | grep "$sha" >/dev/null); then
            echo -e "${yellow}Skipping commit $sha because it already exists on this branch${restore}"
        else
            echo -e "${blue}Cherrypicking $sha onto $br${restore}"
            git cherry-pick $sha

            if [ "$?" != 0 ]; then
                echo -e "${red}Error picking $sha onto $br${restore}"
                continue
            fi
        fi
    done

    git push                       
done

