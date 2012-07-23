MESSAGE(STATUS "Checking for C++11 initializer lists")

CHECK_CXX_SOURCE_COMPILES(
	"#include <vector>
	 int main(int, char**)
	 {
	   std::vector<int> tmp{1, 2, 3};
	   return 0;
	 }" BALL_HAS_INITIALIZER_LISTS
)

IF(BALL_HAS_INITIALIZER_LISTS)
	MESSAGE(STATUS "Checking for C++11 initializer lists - Found")
ELSE()
	MESSAGE(STATUS "Checking for C++11 initializer lists - Not found")
ENDIF()
