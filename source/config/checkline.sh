#!/usr/local/bin/bash
for i in `find . -name \*.[hC] -print`; do
	if [ _`basename $i`_ != _config.h_ ] ; then
		LINE=`head -1 $i`
		if [ "$LINE" != "#include <BALL/config.h>" ] ; then
			echo "modifying $i..."
			mv $i $i.bak
			echo "#include <BALL/config.h>" > $i
			cat $i.bak >> $i
			rm $i.bak
		fi
	fi
done

