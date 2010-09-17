CHECK_CXX_SOURCE_COMPILES(
	"int main(int, char**)
	{
		int &&a = 5;
	}" BALL_HAS_RVALUE_REFERENCES
)

CHECK_CXX_SOURCE_COMPILES(
	"#include <string>

	class Bla : std::string
	{
		public:
			Bla(const char* str)
				: std::string(str)
			{
			}
	};

	Bla operator+(std::string&& a, const Bla& blubb)
	{
		return Bla(\"blubb\");
	}

	int main()
	{
		std::string(\"Hiho\") + \"sdfsdf\";
		return 0;
	}" BALL_STD_STRING_HAS_RVALUE_REFERENCES
)
