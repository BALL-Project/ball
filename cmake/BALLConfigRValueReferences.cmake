MESSAGE(STATUS "Checking for rvalue reference support")

CHECK_CXX_SOURCE_COMPILES(
	"int main(int, char**)
	{
		int &&a = 5;
	}" BALL_HAS_RVALUE_REFERENCES
)

IF (BALL_HAS_RVALUE_REFERENCES)
	MESSAGE(STATUS "Checking for rvalue reference support - Found")
ELSE()
	MESSAGE(STATUS "Checking for rvalue reference support - Not found")
ENDIF()
