#/bin/sh

echo "Verifying a sane branch for your kernel version..."
if $(uname -r | grep "3.11" > /dev/null); then
    # recommend branch ubuntu-13.10
    if $(git st | grep "ubuntu-13.10" > /dev/null); then
        echo "Yes"
    else
        read -p "No.  Should I switch it for you?  (y/n)" input
        if [ "$input" == "y" ]; then
            git checkout -f ubuntu-13.10
        else
            echo "OK, but if the build fails come back here and try again."
        fi
    fi
elif $(uname -r | grep "3.8" > /dev/null); then
    # recommend branch ubuntu-13.04
    if $(git st | grep "ubuntu-13.04" > /dev/null); then
        echo "Yes"
    else
        read -p "No.  Should I switch it for you?  (y/n)" input
        if [ "$input" == "y" ]; then
            git checkout -f ubuntu-13.04
        else
            echo "OK, but if the build fails come back here and try again."
        fi
    fi
elif $(uname -r | grep "3.2" > /dev/null); then
    # recommend branch ubuntu-12.04
    if $(git st | grep "ubuntu-12.04" > /dev/null); then
        echo "Yes"
    else
        read -p "No.  Should I switch it for you?  (y/n)" input
        if [ "$input" == "y" ]; then
            git checkout -f ubuntu-12.04
        else
            echo "OK, but if the build fails come back here and try again."
        fi
    fi
fi
