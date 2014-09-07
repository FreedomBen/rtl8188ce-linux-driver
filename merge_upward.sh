#!/bin/bash

git checkout -f ubuntu-13.04        && \
git rebase                          && \
                                       \
git merge origin/ubuntu-12.04       && \
git push                            && \
                                       \
git checkout -f ubuntu-13.10        && \
git rebase                          && \
git merge origin/ubuntu-13.04       && \
git push                            && \
                                       \
git checkout -f ubuntu-14.04        && \
git rebase                          && \
git merge origin/ubuntu-13.10       && \
git push                       

# We don't want to merge up ubuntu-14.04 anymore due to code changes
git checkout -f fedora-20           && \
git rebase                          && \
git merge origin/ubuntu-13.10       && \
git push                            && \
                                       \
git checkout -f arch                && \
git rebase                          && \
git merge origin/fedora-20          && \
git push                       

git checkout -f master              && \
git rebase                          && \
git merge origin/arch               && \
git push                       

