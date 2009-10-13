### list all filenames of the directory here ###
SET(SOURCES_LIST "")

### the list of all files requiring a moc run ###
SET(MOC_SOURCES_LIST "")

ADD_VIEW_SOURCES("VIEW/RENDERING/RENDERERS" "${SOURCES_LIST}")

ADD_BALL_MOCFILES("VIEW/RENDERING/RENDERERS" "include/BALL/VIEW/RENDERING/RENDERERS" "${MOC_SOURCES_LIST}")


