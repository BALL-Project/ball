#!/bin/sh

if (test -f .objects);
then
	rm -f .objects;
fi

PREFIX=$1
shift 1

for i in $@
	do 
			echo $PREFIX/$i >> .objects; 
	done
