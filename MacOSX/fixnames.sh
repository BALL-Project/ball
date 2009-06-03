#!/bin/bash

# search for all occurences of a certain path in the dependent libraries and replace them

fix_from_path=$2
tofix=$(otool -L $1 | grep $fix_from_path | awk '{print $1}')

# find the currently installed name
 cur_name=$(otool -L $1 | awk '{if (NR == 2) print $1}')
 target=$1
 a=$(readlink $1);
 if test $a; then	
	target=$a
 fi;
echo $target
 install_name_tool -id @executable_path/../Frameworks/$target $1

 for i in $tofix; do
  newpath=@executable_path/../Frameworks/$(echo $i | sed s,$fix_from_path,,g);
  newpath=$(echo $newpath | sed s_//_/_g)
  install_name_tool -change $i $newpath $1; 
 done
