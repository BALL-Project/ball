// $Id: version.sip,v 1.2 2001/12/22 16:39:16 oliver Exp $

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
	PyMethod __str__
%MemberCode
  return PyString_FromString(VersionInfo::getVersion());  	
%End
};
