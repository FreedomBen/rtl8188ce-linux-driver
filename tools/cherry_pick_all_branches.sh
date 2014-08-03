#!/bin/bash

declare -a branches=('ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'fedora-20' 'arch' 'master')


if [ -z "$1" ]; then
    echo "Error: No SHAs to cherry-pick"
fi


git pull

for br in ${branches[@]}; do
    echo "Cherrypicking $@ onto $br"
    git checkout -f $br                 && \
    git rebase                          && \
    git cherry-pick $@                  && \
    git push                       
done

