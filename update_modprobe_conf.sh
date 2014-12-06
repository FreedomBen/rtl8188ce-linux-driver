#!/bin/bash

if [ ! -f "functions.sh" ]; then
    echo -e "${red}[*] Error: Required file functions.sh not present${restore}" >&2
    exit
else
    . "$(readlink -f functions.sh)"
fi

FILE="/etc/modprobe.d/modprobe.conf"
COMMENT="# disable power save and enable sw encryption (added by FreedomBen's driver)"
OPTION1="options rtl8192ce ips="
OPTION1_VAL="0"
OPTION2="options rtl8192ce fwlps="
OPTION2_VAL="0"
OPTION3="options rtl8192ce swenc="
OPTION3_VAL="1"

opt1 () {
    echo "${OPTION1}${OPTION1_VAL}"
}

opt2 () {
    echo "${OPTION2}${OPTION2_VAL}"
}

opt3 () {
    echo "${OPTION3}${OPTION3_VAL}"
}

if [ "$(id -u)" = "0" ]; then
    if pciDetectsRtl8192ce || pciDetectsRtl8188ce; then
        CONTAINS_O1=0
        CONTAINS_O2=0
        CONTAINS_O3=0
        WRITE=0

        if [ -f "$FILE" ]; then
            while read line; do
                if [ "$line" = "$(opt1)" ]; then
                    CONTAINS_01=1
                elif [ "$line" = "$(opt2)" ]; then
                    CONTAINS_O2=1
                elif [ "$line" = "$(opt3)" ]; then
                    CONTAINS_O3=1
                fi
            done < $FILE
        fi

        if [ "$CONTAINS_01" = "1" ] && [ "$CONTAINS_O2" = "1" ] && [ "$CONTAINS_O3" = "1" ]; then
            echo "Power save already disabled in ${FILE}. Doing nothing"
        else
            echo -e "\n\nIt is recommended to disable power save features for improved performance.\nThis is done by writing some options to ${FILE}."
            read -p "Do this now? (y/n): " PROCEED

            if [ "$PROCEED" = "y" -o "$PROCEED" = "Y" ]; then
                if [ -f $FILE ]; then
                    # Remove all lines matching our options
                    TFILE=".mp.tmp"
                    touch $TFILE
                    while read line; do
                        if [[ ! $line =~ $OPTION1 ]] && [[ ! $line =~ $OPTION2 ]] && [[ ! $line =~ $OPTION3 ]] && [[ ! $line =~ FreedomBen ]]; then
                            echo $line >> $TFILE
                        fi
                    done < $FILE
                    mv $TFILE $FILE
                else
                    touch $FILE
                fi

                if (( $(cat $FILE | wc -l) )); then
                    echo -e "\n\n" >> $FILE
                fi

                echo "$COMMENT" >> $FILE
                echo "$(opt1)" >> $FILE
                echo "$(opt2)" >> $FILE
                echo "$(opt3)" >> $FILE
                echo "Done.  Power save disabled in ${FILE}"
            else
                echo "Did not disable power save options"
            fi
        fi
    else
        echo "Power save disable only support rtl8188ce and rtl8192ce currently"
    fi
else
    echo "Must be root to disable power save options. Re-run with sudo"
fi
