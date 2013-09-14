
#!/bin/bash

#replace LINUX_VERSION_CODE with (KERNEL_VERSION(2,6,39)) for all the files of the directory "rtlwifi"
find ../ -name '*.h' -print0 |xargs -0 sed -i 's/LINUX_VERSION_CODE/(KERNEL_VERSION(3,2,5))/g'
find ../ -name '*.c' -print0 |xargs -0 sed -i 's/LINUX_VERSION_CODE/(KERNEL_VERSION(3,2,5))/g'

src="./3.2.5-1"
des="../../../../.."

files="config.mk Makefile scripts/driver-select scripts/unload.sh scripts/wlunload.sh"

rtlwifi_dir=drivers/net/wireless/rtlwifi
rtlwifi_files="Makefile Kconfig rtl8188ee/Makefile rtl8192ce/Makefile rtl8192de/Makefile rtl8192se/Makefile"

#backup files
echo -e "\n------------------backup files-----------------"
for i in $files;
do
  if cp $des/$i $des/$i.old
  then echo "success backup $i "
  else echo "can not copy $des/$i to $des/$i.old"
  fi
done

#copy files
echo "-----------------copy files---------------------"
for i in $files;
do
  if cp $src/$i $des/$i 
  then echo "success copy $i destination"
  else echo "can not copy $src/$i to $des/$i" 
  fi
done 


for i in $rtlwifi_files;
do
  if cp $src/$rtlwifi_dir/$i $des/$rtlwifi_dir/$i
  then echo "success copy $rtlwifi_dir/$i to destination"
  else echo "can not copy $src/$i to $des/$rtlwifi_dir/$i"
  fi
done
