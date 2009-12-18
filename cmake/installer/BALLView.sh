#!/bin/sh
#

BALLVIEW_BUNDLE="`echo "$0" | sed -e 's/\/Contents\/MacOS\/BALLView//'`"
BALLVIEW_RESOURCES="$BALLVIEW_BUNDLE/Contents/Resources"

export "DYLD_LIBRARY_PATH=$BALLVIEW_RESOURCES/lib"

exec "$BALLVIEW_RESOURCES/bin/BALLView" $@
