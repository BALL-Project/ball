// $Id: version.sip,v 1.3 2003/03/25 17:09:02 anhi Exp $

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
	// __str__ method: just call getVersion
	char *__str__() /NonLazy/;
%MemberCode
  return PyString_FromString(VersionInfo::getVersion());  	
%End 
};
