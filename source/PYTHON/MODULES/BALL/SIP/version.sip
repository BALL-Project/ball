// $Id: version.sip,v 1.1 2000/07/18 21:28:35 oliver Exp $

class VersionInfo
{
%HeaderCode
	#include <BALL/COMMON/version.h>
	using namespace BALL;
%End
  public:
  enum Type
  {
   STABLE,
   ALPHA,
   BETA,
   NONPUBLIC,
   UNKNOWN
  };
  static const char* getVersion();
  static int getMajorRevision();
  static int getMinorRevision();
  static Type getType();
	// __rep__ method: just call getVersion
	operator const char* ();
%MemberCode
  return PyString_FromString(VersionInfo::getVersion());  	
%End
};
