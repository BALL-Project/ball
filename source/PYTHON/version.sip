// $Id: version.sip,v 1.2 2000/06/30 05:59:50 oliver Exp $

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
};
