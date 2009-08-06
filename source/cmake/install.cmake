SET(CMAKE_PROJECT_NAME QuEasy)
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "someone")
INCLUDE(CPack)


INSTALL(DIRECTORY APPLICATIONS/QuEasyViz/images/ DESTINATION share/QuEasy/images/)
INSTALL(PROGRAMS APPLICATIONS/QuEasyViz/QuEasyViz DESTINATION share/QuEasy/)
INSTALL(PROGRAMS APPLICATIONS/QuEasyRun/QuEasyRun DESTINATION share/QuEasy/)

INSTALL(DIRECTORY ../doc/QuEasyViz/ DESTINATION share/QuEasy/doc/QuEasyViz/)
INSTALL(DIRECTORY ../data/QSAR/ DESTINATION share/QuEasy/data/)

# add start script
file(WRITE cmake/QuEasyViz "/usr/share/QuEasy/QuEasyViz")
file(WRITE cmake/QuEasyRun "/usr/share/QuEasy/QuEasyRun")
INSTALL(PROGRAMS cmake/QuEasyViz DESTINATION bin/)
INSTALL(PROGRAMS cmake/QuEasyRun DESTINATION bin/)


