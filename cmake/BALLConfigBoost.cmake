# Find and configure Boost library

# Mandatory boost components
SET(BALL_BOOST_COMPONENTS
	chrono
	date_time
	iostreams
	regex
	serialization
	system
	thread
)

# Additional Boost versions that should be included by CMake
# CMake 2.8.12, which is minimum for configuring BALL, already knowns versions 1.33 up to 1.56
SET(Boost_ADDITIONAL_VERSIONS "1.60" "1.59" "1.58" "1.57")

# Detailed messaging in case of failures
SET(Boost_DETAILED_FAILURE_MSG ON)

# Invoke CMake FindBoost module
FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ${BALL_BOOST_COMPONENTS})

# Required definitions on Windows machines
IF(WIN32)
	ADD_DEFINITIONS(-DBOOST_ALL_NO_LIB)
	ADD_DEFINITIONS(-DBOOST_ALL_DYN_LINK)
ENDIF()
