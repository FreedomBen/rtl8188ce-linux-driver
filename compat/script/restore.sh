
#!/bin/bash

src="./3.2.5-1"
des="../../../../.."

files="config.mk Makefile scripts/driver-select scripts/unload.sh scripts/wlunload.sh"

#restore file
echo -e "------------------restore files-----------------"
for i in $files;
do
  if cp $des/$i.old $des/$i
  then echo "success restore $i "
       rm $des/$i.old
  else echo "can not copy $des/$i to $des/$i.old"
  fi
done



