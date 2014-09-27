#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo "Error: Required file functions.sh not present" >&2
    exit 1
else
    . $(readlink -f functions.sh)
fi

askbackup

