#!/bin/bash

dir=$(ls |grep compat-wireless)
for i in $dir
do
	if [[ "$i" != *.bz2 ]] 
	then 
		cd $i
		make uninstall
		cd -

		rm -r $i 
	fi 
done

rm -fr /lib/firmware/rtlwifi/	
