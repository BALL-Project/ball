CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		std::string::const_iterator c_it = s.cbegin();

		return 0;
	}" BALL_HAS_STD_STRING_CONST_ITERATORS
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		std::string::const_iterator c_it = s.cbegin();
		s.insert(c_it, 1, 'a');

		return 0;
	}" BALL_HAS_STD_STRING_CONST_ITERATOR_FUNCTIONS
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		s.shrink_to_fit();

		return 0;
	}" BALL_HAS_STD_STRING_SHRINK_TO_FIT
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>
	int main(int, char**)
	{
		std::string s;
		s.front();
		s.back();

		return 0;
	}" BALL_HAS_STD_STRING_FRONT_BACK
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
