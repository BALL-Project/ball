// $Id: rtti.C,v 1.7 2001/06/28 14:13:34 oliver Exp $

#include <BALL/COMMON/rtti.h>
#include <typeinfo>
#include <ctype.h>

#ifdef __GNUC__
	extern "C" char* cplus_demangle(const char* s, int options); 
#endif

namespace BALL 
{

	string streamClassName(const std::type_info& t)
	{
#ifdef __GNUC__
    #if (__GNUC__ < 3)
			string s(t.name());
      s = GNUDemangling::demangle(s);
    #else
      string s("_Z");
      s += t.name();
      char* name = cplus_demangle(s.c_str(), 1 << 8);
      if (name != 0)
      {
        s = name;
			}
    #endif                                                                                                                                   
#else
		string s(t.name());
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
 
#ifdef __GNUC__

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

#endif // __GNUC__

} // namespace BALL

