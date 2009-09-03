set(CPACK_GENERATOR "DragNDrop")

SET(CMAKE_PROJECT_NAME QuEasy)
SET(CPACK_BUNDLE_NAME QuEasy)
SET(CPACK_RESOURCE_FILE_LICENSE "cmake/QuEasy-license.txt")
set(CPACK_PACKAGE_VERSION ${QUEASY_VERSION})

INCLUDE(CPack)

# first, fix dynamic linking ##
add_custom_command(TARGET QuEasyViz POST_BUILD COMMAND cmake/MacOSX/prepare-app.sh)


# add executables  ##
INSTALL(TARGETS QuEasyRun DESTINATION QuEasy/)
INSTALL(TARGETS QuEasyViz DESTINATION QuEasy/)
INSTALL(FILES cmake/MacOSX/QuEasy-shell DESTINATION QuEasy/ PERMISSIONS OWNER_EXECUTE OWNER_READ OWNER_WRITE)
INSTALL(FILES cmake/MacOSX/QuEasy-shell.rc DESTINATION QuEasy/ RENAME .QuEasy-shell.rc)

# add documentation, images and data  ##
INSTALL(DIRECTORY ../doc/QuEasyViz/ DESTINATION QuEasy/share/doc/QuEasyViz/)
INSTALL(DIRECTORY ../data/QSAR/ DESTINATION QuEasy/share/data/QSAR/)

INSTALL(FILES cmake/QuEasy-license.txt DESTINATION QuEasy/ RENAME License.txt)

# copy dynamic libraries  ##
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/QtGui.framework/Versions/4/QtGui DESTINATION QuEasy/lib/QtGui.framework/Versions/4/QtGui COMPONENT library)
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/QtCore.framework/Versions/4/QtCore DESTINATION QuEasy/lib/QtCore.framework/Versions/4/QtCore COMPONENT library)




