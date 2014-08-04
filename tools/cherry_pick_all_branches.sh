#!/bin/bash

declare -a branches=('ubuntu-12.04' 'ubuntu-13.04' 'ubuntu-13.10' 'ubuntu-14.04' 'fedora-20' 'arch' 'master')


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
            echo "Skipping commit $sha because it already exists on this branch"
        else
            echo "Cherrypicking $sha onto $br"
            git cherry-pick $sha || continue
        fi
    done

    git push                       
done

