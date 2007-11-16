#!/bin/bash

export CC=cl.exe
export LD=link.exe
export CFLAGS="/IC:/BALL/Windows/Contrib/gsl-1.9/ /Disfinite=_finite /nologo /D__STDC__ /Ox"
export CPP="cl -E"

cd gsl-1.9
./configure

#mv /bin/link.exe /bin/link_cygwin.exe

for i in $(find -name Makefile); do sed -i "s/LN_S = ln -s/LN_S = cp/g" $i; done
for i in $(find -type d); do (cd $i && make); done
for i in $(find -name Makefile); do awk 'BEGIN{inobjects=0; string=""} /am_lib.*la_OBJECTS =/ {inobjects=1} /\\$/ {if (inobjects==1) {st=st $0}} /^[ \t]*$/ {inobjects=0} /[^\\]$/ {if (inobjects==1) {inobjects==0; st=st $0}} END {print st}' $i | sed "s/\\\//g" | sed "s/am_lib.*OBJECTS =/libobjects =/" | cat >> $i; done
#for i in $(find -name Makefile); do echo "include /cygdrive/c/BALL/Windows/Contrib/libgslobj.mak" | cat >> $i; done
for i in $(find -name Makefile); do (cd `dirname $i` && make borg)| sed "s/\([[:alnum:]]*\)\.lo/\1.obj/g"; done |  xargs link.exe msvcrt.lib \/DLL \/OUT:gsl.dll
for i in $(find -name Makefile); do (cd `dirname $i` && make borg)| sed "s/\([[:alnum:]]*\)\.lo/\1.obj/g"; done |  xargs lib.exe msvcrt.lib \/OUT:gsl.lib


#mv link_cygwin.exe /bin/link.exe
