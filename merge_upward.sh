#!/bin/bash

git checkout -f ubuntu-13.04

git merge origin/ubuntu-12.04
git push

git checkout -f ubuntu-13.10
git merge origin/ubuntu-13.04
git push

git checkout -f fedora-20
git merge origin/ubuntu-13.10
git push

git checkout -f master
git merge origin/fedora-20
git push
