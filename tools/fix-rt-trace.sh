#!/usr/bin/env bash

PREV='RT_TRACE'
REPL='rtl_dbg'

files()
{
  grep \
    --perl-regexp \
    --color=none \
    --binary-files=without-match \
    --directories=skip \
    --devices=skip \
    --recursive \
    "${PREV}" \
    . \
  | awk -F : '{ print $1 }' \
  | sort \
  | uniq \
  | sed '/fix-rt-trace.sh$/d'
}

for f in $(files); do
  sed -i -e "s/${PREV}/${REPL}/g" "${f}"
done
