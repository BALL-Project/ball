set(CPACK_GENERATOR "DragNDrop")

SET(CMAKE_PROJECT_NAME QuEasy)
SET(CPACK_BUNDLE_NAME QuEasy)


INCLUDE(CPack)

# first, fix dynamic linking ##
add_custom_command(TARGET QuEasyViz POST_BUILD COMMAND cmake/MacOSX/prepare-app.sh)


# add executables  ##
INSTALL(TARGETS QuEasyRun DESTINATION .)
INSTALL(TARGETS QuEasyViz DESTINATION .)

# add documentation, images and data  ##
INSTALL(DIRECTORY ../doc/QuEasyViz/ DESTINATION share/QuEasy/doc/QuEasyViz/)
INSTALL(DIRECTORY ../data/QSAR/ DESTINATION share/QuEasy/data/)


# copy dynamic libraries  ##
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/QtGui.framework/Versions/4/QtGui DESTINATION lib/QtGui.framework/Versions/4/QtGui COMPONENT library)
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/QtCore.framework/Versions/4/QtCore DESTINATION lib/QtCore.framework/Versions/4/QtCore COMPONENT library)




