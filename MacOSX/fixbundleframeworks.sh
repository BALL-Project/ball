#!/bin/bash

## This script finds all .dylibs in a given directory, lists their dynamic load dependencies,
## tries to detect them in the same directory hierarchy, and, if found, prepares their load
## dependencies for bundling

search_path=$1

for lib in $(find "$search_path" -name \*.dylib -o -name \*.so); do
	for cur_name in $(otool -L $lib | awk '{if (NR >= 2) print $1}' | grep -v @executable_path); do
		cur_base_name=$(basename $cur_name)	
		in_bundle_name=$(find "$search_path" -name $cur_base_name)
		in_bundle_base_name=$(echo "$in_bundle_name" | sed "s_^.*/Contents/\(.*\)_\1_g")
		if [ -n "$in_bundle_name" -a -f "$in_bundle_name" ]; then
			target=$in_bundle_name
			a=$(readlink $target);
			if test $a; then	
				target=$a
			fi;

			if [ "$cur_base_name" == "$(basename $lib)" ]; then
				install_name_tool -id @executable_path/../Frameworks/$(basename $target) $lib
			else
				install_name_tool -change $cur_name @executable_path/../$in_bundle_base_name $lib
 			fi
		fi
	done;
done
