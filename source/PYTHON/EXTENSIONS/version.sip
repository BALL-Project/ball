// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: version.sip,v 1.5 2003/10/29 22:08:40 oliver Exp $
//

class VersionInfo
{
%TypeHeaderCode
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
	SIP_PYOBJECT __str__();
%MethodCode
  return PyString_FromString(VersionInfo::getVersion());  	
%End 
};
