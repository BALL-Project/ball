set(CPACK_GENERATOR "DragNDrop")

SET(CMAKE_PROJECT_NAME QuEasy)
SET(CPACK_BUNDLE_NAME QuEasy)


INCLUDE(CPack)

# first, fix dynamic linking ###
add_custom_command(TARGET QuEasyViz POST_BUILD COMMAND cmake/MacOSX/prepare-app.sh)


# add executables  ###
INSTALL(TARGETS QuEasyRun DESTINATION .)
INSTALL(TARGETS QuEasyViz DESTINATION .)

# add documentation, images and data  ###
INSTALL(DIRECTORY ../doc/QuEasyViz/ DESTINATION share/QuEasy/doc/QuEasyViz/)
INSTALL(DIRECTORY ../data/QSAR/ DESTINATION share/QuEasy/data/)


# copy dynamic libraries  ###
#install(TARGETS QuEasyViz LIBRARY DESTINATION lib/ COMPONENT library)
install(DIRECTORY /Users/admin/Documents/BALL-1.3-beta1/contrib/lib/QtGui.framework/ DESTINATION lib/QtGui.framework/ COMPONENT library)
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/qwt/lib/libqwt.5.dylib DESTINATION lib/)
install(FILES /Users/admin/Documents/BALL-1.3-beta1/contrib/qwt/lib/libqwt.5.0.2.dylib DESTINATION lib/)



