// $Id: version.C,v 1.1 2000/06/27 23:09:52 oliver Exp $

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
	const char* VersionInfo::getVersion()
	{
		return BALL_RELEASE_STRING " ("__DATE__", " __TIME__ ")";
	}

	int VersionInfo::getMajorRevision()
	{
		static String release(BALL_RELEASE_STRING);
		return release.getField(0, ".").toInt();
	}
	
	int VersionInfo::getMinorRevision()
	{
		static String release(BALL_RELEASE_STRING);
		return release.getField(1, ".").toInt();
	}
	
	VersionInfo::Type VersionInfo::getType()
	{
		static String release(BALL_RELEASE_STRING);
		
		// check for the suffix of the release string
		// a = alpha, b = beta, n = nonpublic, no letter = stable
		if (release.size() == 0)
		{
			return UNKNOWN;
		}
		switch (release[-1])
		{
			case 'a': 
				return ALPHA; break;

			case 'b': 
				return BETA; break;

			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				return STABLE; break;

			default:
				return UNKNOWN;
		}
	}

	const char* VersionInfo::RELEASE_DATE_AND_TIME = "@(#)BALL kernel compiled on: " __DATE__ ", " __TIME__;
	const char* VersionInfo::RELEASE_WHAT_STRING = "@(#)BALL kernel version: V." BALL_RELEASE_STRING;
					  
}
