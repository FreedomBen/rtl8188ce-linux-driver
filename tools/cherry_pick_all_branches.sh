#!/bin/bash

declare -a branches=('ubuntu-17.10' 'generic-4.14.x' 'generic-4.13.x' 'generic-4.12.x' 'ubuntu-17.04' 'generic-4.11.x' 'generic-4.10.x' 'generic-4.9.x' 'ubuntu-16.10' 'generic-4.8.x' 'generic-4.7.x' 'generic-4.6.x' 'ubuntu-16.04' 'generic-4.5.x' 'generic-4.4.x' 'generic-4.3.x' 'ubuntu-15.10' 'generic-4.2.x' 'generic-4.1.x' 'generic-4.0.x' 'ubuntu-15.04' 'ubuntu-14.10' 'generic-3.19.x' 'mint-17.1' 'generic-3.18.x' 'generic-3.17.x' 'generic-3.13.x' 'mint-17' 'generic-3.16.x' 'ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'generic-3.14.x' 'master')

restore='\033[0m'
red='\033[0;31m'
blue='\033[0;34m'
yellow='\033[1;33m'

if [ -z "$1" ]; then
    echo "Error: No SHAs to cherry-pick"
    exit 1
fi


git pull

for br in ${branches[@]}; do
    # Don't cherry-pick a commit that is already on the branch
    git checkout -f $br || (echo -e "${red}Error checking out $br!${restore}" && continue)
    git rebase          || (echo -e "${red}Error rebasing $br!${restore}" && continue)

    for sha in "$@"; do
        if $(git log | grep "$sha" >/dev/null); then
            echo -e "${yellow}Skipping commit $sha on branch $br because it already exists here${restore}"
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

