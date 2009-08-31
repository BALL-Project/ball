#!/bin/bash

QT_PATH=/Users/admin/Documents/BALL-1.3-beta1/contrib/lib
EXECUTABLES="QuEasyViz.app/Contents/MacOS/QuEasyViz QuEasyRun.app/Contents/MacOS/QuEasyRun"

echo

# fix linking of applications to qt-libs
for exe in `echo $EXECUTABLES`; do
	for i in QtCore QtGui; do
		echo "install_name_tool -change ${QT_PATH}/$i.framework/Versions/4/$i @executable_path/../../../lib/$i.framework/Versions/4/$i $exe.exe"
		install_name_tool -change ${QT_PATH}/$i.framework/Versions/4/$i @executable_path/../../../lib/$i.framework/Versions/4/$i $exe
	done
done

# fix qt-libs
for i in QtCore QtGui; do
		# fix id's
		echo "install_name_tool -id @executable_path/../../../lib/$i.framework/Versions/4/$i ${QT_PATH}/$i.framework/Versions/4/$i"
		install_name_tool -id @executable_path/../../../lib/$i.framework/Versions/4/$i ${QT_PATH}/$i.framework/Versions/4/$i

		# fix linking to QtCore
		echo "install_name_tool -change ${QT_PATH}/QtCore.framework/Versions/4/QtCore @executable_path/../../../lib/QtCore.framework/Versions/4/QtCore ${QT_PATH}/$i.framework/Versions/4/$i"
		install_name_tool -change ${QT_PATH}/QtCore.framework/Versions/4/QtCore @executable_path/../../../lib/QtCore.framework/Versions/4/QtCore ${QT_PATH}/$i.framework/Versions/4/$i
	
done


echo

mv  QuEasyViz.app/Contents/MacOS/QuEasyViz  QuEasyViz.app/Contents/MacOS/QuEasyViz.exe
cp cmake/MacOSX/QuEasyViz QuEasyViz.app/Contents/MacOS/
cp -r APPLICATIONS/QuEasyViz/images QuEasyViz.app/Contents/MacOS/

mv  QuEasyRun.app/Contents/MacOS/QuEasyRun  QuEasyRun.app/Contents/MacOS/QuEasyRun.exe
cp cmake/MacOSX/QuEasyRun QuEasyRun.app/Contents/MacOS/