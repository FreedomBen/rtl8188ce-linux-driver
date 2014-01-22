#/bin/sh

doSwitch () 
{
    BRANCH="$1"
    CUR_BRANCH="$(git status --branch | head -1 | sed 's/# On branch //g')"
    if $(git status | grep "$BRANCH" > /dev/null); then
        echo "Yes"
    else
        read -p "No (Current branch $CUR_BRANCH).  Should I switch it to $BRANCH for you?  (y/n): " input
        if [ "$input" = "y" ]; then
            git checkout -f $BRANCH
        else
            echo "OK, but if the build fails come back here and try again."
        fi
    fi
}

echo "Verifying a sane branch for your kernel version..."
if $(uname -r | grep "3.12" > /dev/null); then
    doSwitch "fedora-20"
elif $(uname -r | grep "3.11" > /dev/null); then
    doSwitch "ubuntu-13.10"
elif $(uname -r | grep "3.8" > /dev/null); then
    doSwitch "ubuntu-13.04"
elif $(uname -r | grep "3.2" > /dev/null); then
    doSwitch "ubuntu-12.04"
fi

