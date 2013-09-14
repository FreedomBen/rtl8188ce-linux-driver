#!/bin/bash

compat_wireless_version="3.0-2"

tar jxvf compat-wireless-3.0-2-mesh.tar.bz2

driver_fold=compat-wireless-${compat_wireless_version}/drivers/net/wireless/rtlwifi

rm -r $driver_fold
make clean

mkdir $driver_fold
cp Makefile $driver_fold -fr
cp Kconfig $driver_fold -fr
cp readme $driver_fold -fr
cp release_note $driver_fold -fr

cp *.c $driver_fold -fr
cp *.h $driver_fold -fr

cp firmware/rtlwifi /lib/firmware -fr
cp rtl8192se $driver_fold -fr
cp rtl8192ce $driver_fold -fr
cp rtl8192de $driver_fold -fr
cp rtl8723e  $driver_fold -fr
#cp compat $driver_fold -fr

#replace LINUX_VERSION_CODE with compat-wireless kernel version for all the files of the directory "rtlwifi"
find $driver_fold -name '*.h' -print0 |xargs -0 sed -i 's/LINUX_VERSION_CODE/(KERNEL_VERSION(2,6,39))/g'
find $driver_fold -name '*.c' -print0 |xargs -0 sed -i 's/LINUX_VERSION_CODE/(KERNEL_VERSION(2,6,39))/g'

#replace the corresponding files with the files in compat/${compat_wireless_version} 
cd compat/${compat_wireless_version}
files=$(find ./ -not -wholename  '*.svn*' -type f)

for i in $files
do
  if cp $i ../../compat-wireless-${compat_wireless_version}/$i
  then
	  echo "success copy $i to destination"
  else 
	  echo "can not copy $i to compat-wireless-${compat_wireless_version}/$i"
  fi
done

# select rtlwifi driver in compat-wireless
cd ../../compat-wireless-${compat_wireless_version}
./scripts/driver-select rtlwifi
echo
echo "success select driver rtlwifi in compat-wireless, next will auto build driver"
echo

#build driver with compat-wireless
make 
echo
echo "success build driver rtlwifi in compat-wireless, next will auto install driver"
echo

#install driver with compat-wireless
make install
echo
echo "success install driver rtlwifi in compat-wireless, you can use driver after reboot"
echo
cd -
