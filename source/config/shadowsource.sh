#!/bin/sh



CP=cp
LN=ln
MKDIR=mkdir

if test $# != 3 ; then
	echo "$0 <target> <source> <dirs>"
	exit
fi

DIRS="$3"
SOURCE_PATH="$2"
TARGET_PATH=$1

echo "creating directories..."
${MKDIR} -p ${TARGET_PATH} 
if test ! -d ${TARGET_PATH} ; then
	echo "could not create target directory ${TARGET_PATH}. Aborting." >&2
	exit
fi

for i in ${DIRS} ; do 
	SHADOW_DIRS="${SHADOW_DIRS} $i"\ `cd ${SOURCE_PATH}/$i;find . -type d ! -name RCS ! -name \. | sed "s/^\.\//$i\//g"`
done

for i in ${SHADOW_DIRS} ; do
	${MKDIR} ${TARGET_PATH}/$i 2>/dev/null
	if test ! -d ${TARGET_PATH}/$i ; then
		echo "ERROR: cannot create ${TARGET_PATH}/$i!"
	fi
done

echo "creating links for source files..."
for i in ${DIRS} ; do
	FILES=`cd ${SOURCE_PATH}/$i;find . -name \*.C | sed "s/^\.\///g"`
	FILES="${FILES} "`cd ${SOURCE_PATH}/$i;find . -name Makefile | sed "s/^\.\///g"`
	for j in ${FILES}; do
		${LN} -s ${SOURCE_PATH}/$i/$j ${TARGET_PATH}/$i/$j 2>/dev/null
		if test ! -f ${TARGET_PATH}/$i/$j ; then
			echo "ERROR: cannot create link for ${TARGET_PATH}/$i/$j!"
		fi
	done
done
