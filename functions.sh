
# some shared sh functions used by other scripts

inGitRepo ()
{
    prevdir="$(pwd)"
    while true; do
        if [ -d ".git" ]; then
            cd "$prevdir"
            return 0
        fi

        if [ "$(pwd)" = "/" ]; then
            cd "$prevdir"
            return 1
        fi

        cd ..
    done
}

runningFedora () 
{ 
    uname -r | grep --color=auto "fc" > /dev/null
}

runningUbuntu () 
{ 
    uname -a | grep --color=auto "Ubuntu" > /dev/null
}

installBuildDependencies ()
{
    if runningFedora; then
        sudo yum -y install kernel-devel kernel-headers
        sudo yum -y groupinstall "Development Tools"
        sudo yum -y groupinstall "C Development Tools and Libraries"
        sudo yum -y install git
    elif runningUbuntu; then
        sudo apt-get -y install gcc build-essential linux-headers-generic linux-headers-$(uname -r)
        sudo apt-get -y install git
    fi
}

makeModuleLoadPersistent () 
{
    if runningFedora; then
        file="/etc/rc.modules"
    elif runningUbuntu; then
        file="/etc/modules"
    else
        echo "Cannot make persistent; unknown module file" >&2
        return 1
    fi

    not_present=1
    if [ -f "$file" ]; then
        while read line; do
            if $(echo "$line" | grep "rtl8192ce" > /dev/null); then
                not_present=0
            fi
        done < "$file"
    fi

    if (( $not_present )); then
        echo "rtl8192ce.ko" >> "$file"
    fi
}
