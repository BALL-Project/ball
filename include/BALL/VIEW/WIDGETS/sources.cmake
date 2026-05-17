### list all filenames of the directory here ###
SET(GROUP VIEW/WIDGETS)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

# Phase 999.44 — Unified Inspector subtree headers live under
# include/BALL/VIEW/WIDGETS/inspector/{,sections/}*.h. The standard
# ADD_VIEW_HEADERS macro flattens basenames so it can't carry
# subdirectory paths; append the full paths to VIEW_headers
# directly so AUTOMOC discovers Q_OBJECT classes in InspectorDock /
# InspectorView / InspectorTabs / InspectorBody / InspectorSection
# headers and the per-tab section subclasses.
FILE(GLOB INSPECTOR_HEADERS_LIST
	"include/BALL/${GROUP}/inspector/*.h"
	"include/BALL/${GROUP}/inspector/sections/*.h")

IF(NOT BALL_HAS_QTWEBENGINE)
	LIST(REMOVE_ITEM HEADERS_LIST
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/HTMLPage.h"
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/HTMLView.h"
	)
ENDIF()

IF(NOT BALL_PYTHON_SUPPORT)
	LIST(REMOVE_ITEM HEADERS_LIST
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/hotkeyTable.h"
		"${CMAKE_SOURCE_DIR}/include/BALL/${GROUP}/pyWidget.h"
	)
ENDIF()

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")

# Append inspector-subtree headers directly (basename-flattening macro
# can't preserve their paths). Inspector/inspector_sections groups are
# kept distinct for IDE source-group readability.
LIST(APPEND VIEW_headers ${INSPECTOR_HEADERS_LIST})
SOURCE_GROUP("Header Files\\VIEW\\WIDGETS\\inspector" FILES ${INSPECTOR_HEADERS_LIST})
