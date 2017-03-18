#!/usr/bin/env bash

git add .
git reset HEAD regd.c
for i in $(findref MODULE_AUTHOR | cut -d ':' -f 1 | sort | uniq); do
  git reset HEAD "$i"
done
