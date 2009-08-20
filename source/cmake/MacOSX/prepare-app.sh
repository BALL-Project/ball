#!/bin/bash

QT_PATH=/Users/admin/Documents/BALL-1.3-beta1/contrib/lib
EXECUTABLES="QuEasyViz.app/Contents/MacOS/QuEasyViz QuEasyRun.app/Contents/MacOS/QuEasyRun"

echo

for exe in `echo $EXECUTABLES`; do
	for i in QtCore QtGui; do
		echo "install_name_tool -change         ${QT_PATH}/$i.framework/Versions/4/$i @executable_path/../../../lib/$i.framework/Versions/4/$i $exe.exe"
		install_name_tool -change         ${QT_PATH}/$i.framework/Versions/4/$i @executable_path/../../../lib/$i.framework/Versions/4/$i $exe
	done

	echo "install_name_tool -change /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/libgsl.0.dylib @executable_path/../../../lib/libgsl.dylib $exe.exe"
	install_name_tool -change /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/libgsl.0.dylib @executable_path/../../../lib/libgsl.dylib $exe
	echo "install_name_tool -change /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/libgslcblas.0.dylib @executable_path/../../../lib/libgslcblas.dylib $exe.exe"
	install_name_tool -change /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/libgslcblas.0.dylib @executable_path/../../../lib/libgslcblas.dylib $exe
done

echo

mv  QuEasyViz.app/Contents/MacOS/QuEasyViz  QuEasyViz.app/Contents/MacOS/QuEasyViz.exe
cp cmake/MacOSX/QuEasyViz QuEasyViz.app/Contents/MacOS/
cp -r APPLICATIONS/QuEasyViz/images QuEasyViz.app/Contents/MacOS/

mv  QuEasyRun.app/Contents/MacOS/QuEasyRun  QuEasyRun.app/Contents/MacOS/QuEasyRun.exe
cp cmake/MacOSX/QuEasyRun QuEasyRun.app/Contents/MacOS/