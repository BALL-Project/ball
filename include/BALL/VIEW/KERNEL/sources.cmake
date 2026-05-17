### list all filenames of the directory here ###
SET(GROUP VIEW/KERNEL)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")

include(${CMAKE_SOURCE_DIR}/include/BALL/VIEW/KERNEL/MODES/sources.cmake)

# Phase 999.40 — BALLView Refresh: pick up tokens.h + themeManager.h.
SET(GROUP_THEME VIEW/KERNEL/theme)
FILE(GLOB THEME_HEADERS_LIST "include/BALL/${GROUP_THEME}/*.h")
ADD_VIEW_HEADERS("${GROUP_THEME}" "${THEME_HEADERS_LIST}")

# Phase 999.44 — BALLView Refresh: *Controller extraction subtree.
# Headers live under include/BALL/VIEW/KERNEL/controllers/*.h; append
# them to VIEW_headers directly (the ADD_VIEW_HEADERS macro flattens
# basenames and can't carry the subdirectory path).
FILE(GLOB CONTROLLERS_HEADERS_LIST "include/BALL/VIEW/KERNEL/controllers/*.h")
LIST(APPEND VIEW_headers ${CONTROLLERS_HEADERS_LIST})
SOURCE_GROUP("Header Files\\VIEW\\KERNEL\\controllers" FILES ${CONTROLLERS_HEADERS_LIST})
