// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: version.sip,v 1.4 2003/08/26 09:18:25 oliver Exp $
//

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
