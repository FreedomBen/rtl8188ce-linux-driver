#!/bin/sh

FILE="/etc/modprobe.d/modprobe.conf"
COMMENT="# disable power save options (added by FreedomBen\'s driver)"
OPTION1="options rtl8192ce ips=0"
OPTION2="options rtl8192ce fwlps=0"

if [ "$(id -u)" = "0" ]; then
    echo -e "\n\nIt is recommended to disable power save features for improved performance.\nThis is done by writing some options to ${FILE}."  
    read -p "Do this now? (y/n): " PROCEED

    if [ "$PROCEED" == "y" -o "$PROCEED" == "Y" ]; then
        CONTAINS=0
        while read line; do
            if [ "$line" = "$OPTION1" -o "$line" = "$OPTION2" ]; then
                CONTAINS=1
            fi
        done < $FILE

        if [ "$CONTAINS" = "0" ]; then
            echo "$COMMENT" >> $FILE
            echo "$OPTION1" >> $FILE
            echo "$OPTION2" >> $FILE
            echo "Done.  Power save disabled in ${FILE}"
        else
            echo "Power save already disabled in ${FILE}. Doing nothing"
        fi
    else
        echo "Did not disable power save options"
    fi
else
    echo "Must be root to disable power save options. Re-run with sudo"
fi
