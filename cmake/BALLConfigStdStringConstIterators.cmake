CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		std::string::const_iterator c_it = s.cbegin();

		return 0;
	}" BALL_HAS_STD_STRING_CONST_ITERATORS
)
