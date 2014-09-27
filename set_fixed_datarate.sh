#!/bin/bash

RATE=24M
INTF=''

wireless_interfaces ()
{
    echo $(iwconfig 2>&1 | grep 'IEEE' | awk '{print $1}' | xargs)
}

wi_count ()
{
    echo $(wireless_interfaces | wc -w)
}

set_fixed_rate ()
{
    sudo iwconfig $1 rate fixed
    sudo iwconfig $1 rate $RATE
    sudo iwconfig $1 bit fixed
    sudo iwconfig $1 bit $RATE
    echo "$1 rate set to $RATE"
}

if [ -n "$1" ]; then
    RATE="$1"
fi

if [ "$(wi_count)" = "0" ]; then
    echo "Sorry, I couldn't find any wireless interfaces" >&2
elif [ "$(wi_count)" != "1" ]; then
    echo "You have $(wi_count) wireless interfaces."
    read -p "Use which I/F? (leave blank to use all): " INF
    if [ -n "$INF" ]; then
        INTF="$INF"
    fi
fi

echo "Setting fixed rate to $RATE"

if [ -z "$INTF" ]; then
    for i in "$(wireless_interfaces)"; do
        set_fixed_rate "$i"
    done
else
    set_fixed_rate "$INTF"
fi

