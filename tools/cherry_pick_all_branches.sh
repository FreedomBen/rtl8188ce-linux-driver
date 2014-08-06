#!/bin/bash

declare -a branches=('ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'fedora-20' 'arch' 'master')

restore='\033[0m'
blue='\033[0;34m'
yellow='\033[1;33m'

if [ -z "$1" ]; then
    echo "Error: No SHAs to cherry-pick"
    exit 1
fi


git pull

for br in ${branches[@]}; do
    # Don't cherry-pick a commit that is already on the branch
    git checkout -f $br || continue
    git rebase          || continue

    for sha in "$@"; do
        if $(git log | grep "$sha" >/dev/null); then
            echo -e "${yellow}Skipping commit $sha on branch $br because it already exists here${restore}"
        else
            echo -e "${blue}Cherrypicking $sha onto $br${restore}"
            git cherry-pick $sha || continue
        fi
    done

    git push                       
done

