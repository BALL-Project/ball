// $Id: version.sip,v 1.1 2001/09/03 16:53:18 oliver Exp $

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
	PyMethods:
	__str__
%MemberCode
  return PyString_FromString(VersionInfo::getVersion());  	
%End
};
