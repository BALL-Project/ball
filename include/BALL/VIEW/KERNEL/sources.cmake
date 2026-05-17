### list all filenames of the directory here ###
SET(GROUP VIEW/KERNEL)

FILE(GLOB HEADERS_LIST "include/BALL/${GROUP}/*.h" "include/BALL/${GROUP}/*.iC")

ADD_VIEW_HEADERS("${GROUP}" "${HEADERS_LIST}")

include(${CMAKE_SOURCE_DIR}/include/BALL/VIEW/KERNEL/MODES/sources.cmake)

# Phase 999.40 — BALLView Refresh: pick up tokens.h + themeManager.h.
SET(GROUP_THEME VIEW/KERNEL/theme)
FILE(GLOB THEME_HEADERS_LIST "include/BALL/${GROUP_THEME}/*.h")
ADD_VIEW_HEADERS("${GROUP_THEME}" "${THEME_HEADERS_LIST}")
