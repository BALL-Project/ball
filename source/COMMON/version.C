// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: version.C,v 1.14 2004/02/19 12:24:33 oliver Exp $
//

#include <BALL/COMMON/version.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/DATATYPE/stringHashMap.h>

namespace BALL
{
	const char* VersionInfo::getVersion() 
	{
		return BALL_RELEASE_STRING " (" __DATE__", " __TIME__ ")";
	}

	int VersionInfo::getMinorRevision()
	{
		static String release(BALL_RELEASE_STRING);
		String minor = release.getField(1, ".");
		String tmp = minor;
		tmp.trimRight(String::CHARACTER_CLASS__ASCII_NUMERIC);
		if (tmp.size() > 0)
		{
			minor.trimRight(String::CHARACTER_CLASS__ASCII_NUMERIC);
		}
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
		StringHashMap<VersionInfo::Type> type_mapper;
		type_mapper.insert(std::pair<String, VersionInfo::Type>("alpha", ALPHA));
		type_mapper.insert(std::pair<String, VersionInfo::Type>("beta", BETA));
		type_mapper.insert(std::pair<String, VersionInfo::Type>("nonpublic", NONPUBLIC));
		type_mapper.insert(std::pair<String, VersionInfo::Type>("pre", PRERELEASE));
		type_mapper.insert(std::pair<String, VersionInfo::Type>("patch", PATCHLVL));

		static String release(BALL_RELEASE_STRING);
		
		// check for the suffix of the release string
		// a = alpha, b = beta, n = nonpublic, no letter = stable
		if (release.size() == 0)
		{
			return UNKNOWN;
		}
		
		String minor = release.getField(1, ".");
		String tmp = minor;
		tmp.trimLeft(String::CHARACTER_CLASS__ASCII_NUMERIC);
		if (tmp.size() == 0)
		{
			return STABLE;
		}

		String type = tmp.trimRight("0123456789.");
		if (type_mapper.has(type))
		{
			return(type_mapper[type]);
		}
		else
		{
			return(UNKNOWN);
		}
	}

	const char* VersionInfo::RELEASE_DATE_AND_TIME = "@(#)BALL kernel compiled on: " __DATE__ ", " __TIME__;
	const char* VersionInfo::RELEASE_WHAT_STRING = "@(#)BALL kernel version: V." BALL_RELEASE_STRING;
					  
}
