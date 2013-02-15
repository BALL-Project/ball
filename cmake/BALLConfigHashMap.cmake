INCLUDE(CheckIncludeFileCXX)

SET(CMAKE_REQUIRED_INCLUDES ${BOOST_INCLUDE_DIRS})
CHECK_CXX_SOURCE_COMPILES(
"#include <boost/unordered_map.hpp>
int main(int, char**)
{
	boost::unordered_map<int, int> a;

	return 0;
}"
BALL_HAS_BOOST_UNORDERED_MAP)

IF (NOT BALL_HAS_BOOST_UNORDERED_MAP)
	MESSAGE(FATAL_ERROR "boost::unordered_map is a required dependency!")
ENDIF()