CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		std::string::const_iterator c_it = s.cbegin();
		std::string::iterator result = s.insert(c_it, 1, 'a');

		return 0;
	}" BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		std::string::const_iterator c_it = s.cbegin();
		std::string::iterator result = s.insert(c_it, {'a', 'b'});

		return 0;
	}" BALL_HAS_STD_STRING_CONST_ITERATOR_INITLIST_INSERT
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s(\"a\");
		s.pop_back();

		return 0;
	}" BALL_HAS_STD_STRING_POP_BACK
)
