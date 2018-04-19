#include <BALL/COMMON/global.h>
#include <BALL/COMMON/rtti.h>

#include <typeinfo>
#include <ctype.h>


// Nasty hacks to demangle the stupid name mangling schemes 
// of diverse compilers.

// GNU g++:
// Starting V3.0 we use __cxa_demangle to demangle the names,
// which is declared in <cxxabi.h>.
#if defined(BALL_COMPILER_GXX) || defined(BALL_COMPILER_LLVM)
	#include <cxxabi.h>
#elif defined(BALL_COMPILER_INTEL)
	// Declare the __cxa_demangle method for Intel's C++ compiler.
	// Intel does not provide the cxxabi.h header G++ provides, so 
	// this hack is somewhat rough.
	namespace abi
	{
		extern "C" char* __cxa_demangle(const char*, char*, size_t*, int*);
	}
#endif

namespace BALL 
{

	string streamClassName(const std::type_info& t)
	{
#if (defined(BALL_COMPILER_GXX) || defined(BALL_COMPILER_INTEL)|| defined(BALL_COMPILER_LLVM))
		char buf[BALL_MAX_LINE_LENGTH];
		size_t length = BALL_MAX_LINE_LENGTH - 1;
		int status = 0;
		string s("_Z");
		s += t.name();
		char* name = abi::__cxa_demangle(s.c_str(), buf, &length, &status);
		if (name)
		{
			s = name;
		}
#else
		string s(t.name());
		#ifdef BALL_COMPILER_MSVC
			// MSVC prefixes all class names with "class " -- delete it!
			while (s.find("class ") != string::npos) 
				s.erase(s.find("class "), 6);
		#endif
#endif

		for (unsigned int i = 0; i < s.size(); i++)
		{
			if (s[i] == ' ')
			{
				s[i] = '_';
			}
		}

		if (string(s, 0, 6) == "const_")
		{
			s.erase(0, 6);
		}

		return s;
	}

} // namespace BALL

