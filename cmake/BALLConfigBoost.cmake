# Find and configure Boost library
SET(Boost_NO_BOOST_CMAKE ON)

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
# CMake 3.1, which is minimum for configuring BALL, already knowns versions 1.33 up to 1.56
SET(Boost_ADDITIONAL_VERSIONS "1.65.0" "1.65" "1.64.0" "1.64" "1.63.0" "1.63" "1.62.0" "1.62" "1.61.0" "1.61"
                              "1.60.0" "1.60" "1.59.0" "1.59" "1.58.0" "1.58" "1.57.0" "1.57")

# Detailed messaging in case of failures
SET(Boost_DETAILED_FAILURE_MSG ON)

# Invoke CMake FindBoost module
FIND_PACKAGE(Boost 1.55 REQUIRED COMPONENTS ${BALL_BOOST_COMPONENTS})
