// $Id: version.C,v 1.6 2000/07/17 22:38:16 oliver Exp $

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
	const char* VersionInfo::getVersion()
	{
		return BALL_RELEASE_STRING " ("__DATE__", " __TIME__ ")";
	}

	int VersionInfo::getMinorRevision()
	{
		static String release(BALL_RELEASE_STRING);
		String minor = release.getField(1, ".");
		minor.trimRight(String::CHARACTER_CLASS__ASCII_ALPHA);

		return minor.toInt();
	}
	
	int VersionInfo::getMajorRevision()
	{
		static String release(BALL_RELEASE_STRING);
		return release.getField(0, ". ").toInt();
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
		char release_suffix = release[release.size() - 1];
		switch (release_suffix)
		{
			case 'a': 
				return ALPHA;

			case 'b': 
				return BETA;

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
				return STABLE;

			default:
				return UNKNOWN;
		}
	}

	const char* VersionInfo::RELEASE_DATE_AND_TIME = "@(#)BALL kernel compiled on: " __DATE__ ", " __TIME__;
	const char* VersionInfo::RELEASE_WHAT_STRING = "@(#)BALL kernel version: V." BALL_RELEASE_STRING;
					  
}
