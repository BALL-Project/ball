// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: rtti.C,v 1.14 2003/08/26 09:17:44 oliver Exp $
//

#include <BALL/COMMON/global.h>
#include <BALL/COMMON/rtti.h>

#include <typeinfo>
#include <ctype.h>


// Nasty hacks to demangle the stupid name mangling schemes 
// of diverse compilers.

// GNU g++:
// Starting V3.0 we use __cxa_demangle to demangle the names,
// which is declared in <cxxabi.h>.
#ifdef BALL_COMPILER_GXX
#	if (BALL_COMPILER_VERSION_MAJOR > 2)
		#include <cxxabi.h>
#	endif
#endif

// LLVM/Clang always offers <cxxabi.h>
#ifdef BALL_COMPILER_LLVM
	#include <cxxabi.h>
#endif

#ifdef BALL_COMPILER_INTEL
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
    #if defined(BALL_COMPILER_GXX) && (BALL_COMPILER_VERSION_MAJOR < 3)
			string s(t.name());
      s = GNUDemangling::demangle(s);
    #else
			char buf[BALL_MAX_LINE_LENGTH];
			size_t length = BALL_MAX_LINE_LENGTH - 1;
			int status = 0;
      string s("_Z");
      s += t.name();
      char* name = abi::__cxa_demangle(s.c_str(), buf, &length, &status);
      if (name != 0)
      {
        s = name;
			}
    #endif                                                                                                                                   
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
 
#ifdef BALL_COMPILER_GXX
#	if (BALL_COMPILER_VERSION_MAJOR < 3)

	namespace GNUDemangling 
	{

		string decode_mangling(string& s)
		{
			string tmp;
			int i,len;

			if (s.size() == 0)
				return "";

			if (!isdigit(s[0]))
			{ // decode GNU shortcuts for built-in types
				char c = s[0];
				s.erase(0, 1);
				switch (c)
				{
					case 'Q': // start of class name
						len = atoi(string(s,1,1).c_str());
						s.erase(0, 1);
						for (i = 0; i < len; i++)
						{
							tmp.append(decode_mangling(s));
							tmp.append("::");
						}
						tmp.erase(tmp.end() - 2, tmp.end());
						break;
					case 'Z': // template parameter
						return decode_mangling(s);
						break;

					case 'i':
						return "int";
						break;

					case 'l':
						return "long";
						break;

					case 's':
						return "short";
						break;

					case 'c':
						return "char";
						break;

					case 'x':
						return "long long";
						break;

					case 'f':
						return "float";
						break;

					case 'd':
						return "double";
						break;

					case 'b':
						return "bool";
						break;

					case 'w':
						return "wchar_t";
						break;

					case 'U': // unsigned variants
						tmp = "unsigned ";
						tmp.append(decode_mangling(s));
						break;

					case 'C': // const
						tmp = "const ";
						tmp.append(decode_mangling(s));
						break;

					case 'P': // pointer
						tmp = decode_mangling(s);
						tmp.append("*");
						break;

					case 'R': // reference
						tmp = decode_mangling(s);
						tmp.append("&");
						break;

					case 't':
						tmp = decode_mangling(s);
						tmp.append("<");
					
						len = atoi(string(1, s[0]).c_str());
						s.erase(0,1);
						for (i = 0; i < len; i++)
						{
							tmp.append(decode_mangling(s));
							tmp.append(",");
						}

						// remove last ','
						tmp.erase(tmp.end() - 1, tmp.end());
						tmp.append(">");
						break;

					default:
						tmp = "?";
				}
				return tmp;
			} 
			else 
			{
				i = s.find_first_not_of("0123456789");
				len = atol(string(s, 0, i).c_str());
				if (len == 0)
				{
					s.erase(0,1);
						
					if (s.size() > 0)
					{
						return decode_mangling(s);
					}
					else 
					{
						return "";
					}
				}
				else 
				{
					string h(s, i, len);
					s.erase(0, i + len);
				
					return h;
				}
			}
		}

		string demangle(string s)
		{
			string tmp = decode_mangling(s);

			while (tmp[tmp.size() - 1] == ':')
			{
				tmp.erase(tmp.end() - 1, tmp.end());
			}

			while (tmp[0] == ':')
			{
				tmp.erase(0, 1);
			}

			return tmp;
		}

	} // namespace GNUDemangling 

#	endif // (BALL_COMPILER_VERSION_MAJOR < 3)
#endif // BALL_COMPILER_GXX

} // namespace BALL

