# The name of the project to build
PROJECT(CWL)

# Check OS and set appropriate Miniconda setup file URL
IF(WIN32)
	SET(MINICONDA_INSTALLER "Miniconda2-latest-Windows-x86_64.exe")
	# what now? SET(LD_LIBRARY_PATH_VAR "DYLD_LIBRARY_PATH")
ELSEIF(APPLE)
	SET(MINICONDA_INSTALLER "Miniconda2-latest-MacOSX-x86_64.sh")
	SET(LD_LIBRARY_PATH_VAR "DYLD_LIBRARY_PATH")
ELSE()
	SET(MINICONDA_INSTALLER "Miniconda2-latest-Linux-x86_64.sh")
	SET(LD_LIBRARY_PATH_VAR "LD_LIBRARY_PATH")
ENDIF()


# Set required output directories
SET(MINICONDA_INSTALL_DIR "${PROJECT_BINARY_DIR}/miniconda")
SET(CTD_OUTFILE_DIR "${PROJECT_BINARY_DIR}/ctd_files")
SET(CWL_STUBS_DIR "${PROJECT_BINARY_DIR}/cwl_stubs")
SET(LOGFILE_DIR "${PROJECT_BINARY_DIR}/log")

# Configure file that will install CTDConverter and its dependencies
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/workflow_support/common/install_ctdconverter.cmake.in" 
               "${PROJECT_BINARY_DIR}/install_ctdconverter.cmake" @ONLY)

# Configure file that will generate all CTDs
CONFIGURE_FILE("${CMAKE_SOURCE_DIR}/workflow_support/common/generate_ctds.cmake.in" 
               "${PROJECT_BINARY_DIR}/generate_ctds.cmake" @ONLY)

# Configure CWL build file
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/cwl_generator.cmake.in" 
               "${PROJECT_BINARY_DIR}/cwl_generator.cmake" @ONLY)

# Add cwl target
ADD_CUSTOM_TARGET(cwl
	COMMAND ${CMAKE_COMMAND} -P "${PROJECT_BINARY_DIR}/cwl_generator.cmake"
	DEPENDS BALLTOOLS
	COMMENT "Generating CWL stubs."
)
