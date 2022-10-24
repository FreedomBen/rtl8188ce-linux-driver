#!/usr/bin/env bash

PREV='RT_TRACE'
REPL='rtl_dbg'

for f in $(grep -r --color=none "${PREV}" . | awk -F : '{ print $1 }' | sort | uniq); do
  sed -i -e "s/${PREV}/${REPL}/g" "${f}"
done
