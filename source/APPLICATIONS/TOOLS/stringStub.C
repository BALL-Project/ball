// stringStub.h / stringStub.C are needed for VC2010!
#include <string>
#include <BALL/common.h>


#ifdef BALL_COMPILER_MSVC
template std::string::size_type std::string::npos;
template std::wstring::size_type std::wstring::npos;
#endif
