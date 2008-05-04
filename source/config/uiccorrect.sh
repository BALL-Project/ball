#!/bin/bash
find ./ -type f -name \*.[h,iC,C] | xargs grep -l "#include <BALL/VIEW/UIC" 2>/dev/null > uifiles.dat

for file in `cat uifiles.dat`; do
sed -i s"+#include <BALL/VIEW/UIC/\(.*\)Data.h>+#ifdef BALL_COMPILER_MSVC\n# include <ui_\1.h>\n#else\n# include <BALL/VIEW/UIC/\1Data.h>\n#endif\n+g" $file;
done
