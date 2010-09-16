MESSAGE(STATUS "Checking for gnu slist")

CHECK_CXX_SOURCE_COMPILES(
	"#include <ext/slist>
	 int main(int, char**)
	 {
	   __gnu_cxx::slist<int> l;
	   return 0;
	 }" BALL_HAS_GNU_SLIST
)

IF(BALL_HAS_GNU_SLIST)
	MESSAGE(STATUS "Checking for gnu slist - Found")
ELSE()
	MESSAGE(STATUS "Checking for gnu slist - Not found")
ENDIF()
