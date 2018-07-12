#!/usr/bin/env bash

git add .
git reset HEAD regd.c
for i in $(grep -r -E '(MODULE_AUTHOR|REG_RULE)' | cut -d ':' -f 1 | sort | uniq); do
  git reset HEAD "$i"
done
