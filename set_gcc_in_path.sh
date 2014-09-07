
if [ -x "$0" ]; then
    echo "Did you source me?  I need to be sourced..."
fi

if [ "$(which gcc)" != "/usr/bin/gcc" ]; then
    if [ -f "/usr/bin/gcc" ]; then
        echo "updated gcc in path"
        export PATH="/usr/bin:$PATH"
    else
        echo "/usr/bin/gcc does not exist"
    fi
else
    echo "gcc is already set properly"
fi
