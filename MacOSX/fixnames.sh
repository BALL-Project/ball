#!/bin/bash

tofix="libBALL.dylib libVIEW.dylib"

# find the currently installed name
 cur_name=$(otool -L $1 | awk '{if (NR == 2) print $1}')
 target=$1
 a=$(readlink $1);
 if test $a; then	
	target=$a
 fi;
 install_name_tool -id @executable_path/../Frameworks/$target $1

 for i in tofix; do
  install_name_tool -change $i @executable_path/../Frameworks/$i $1; 
 done
