// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_COMMON_VERSION_H
#define BALL_COMMON_VERSION_H

#ifndef BALL_CONFIG_CONFIG_H
# include <BALL/CONFIG/config.h>
#endif

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif


namespace BALL
{

	/**	Version information class.
			The BALL release, release data and related version can be retrieved by these methods. \par
			Stable releases are designated with numbers only. \par
			Alpha, beta, nonpublic, or prereleases are suffixed as "alpha", "beta", "nonpublic", and "pre", respectively. \par
			The VersionInfo class contains only static methods that can be used to retrieve the desired information. \par
			The release string (BALL_RELEASE_STRING) is defined in include/BALL/CONFIG/config.h \par
	\ingroup Common
	*/
	class BALL_EXPORT VersionInfo
	{
		public:

			/** Enumeration of all possible release types.
			*/
			enum Type
			{
				STABLE,
				ALPHA,
				BETA,
				NONPUBLIC,
				PRERELEASE,
				PATCHLVL, // this can't be PATCHLEVEL due to a collision with historic Python versions (< 2.2).
				UNKNOWN
			};

			/** Return the version number, which is defined in BALL_RELEASE_STRING
			*/
			static const char* getVersion();

			/** Return the major revision number.
					(The part of the release number before the point)
					\throws Exception::InvalidFormat
			*/
			static int getMajorRevision();

			/** Return the minor revision number.
					(The part of the release number after the point)
					\throws Exception::InvalidFormat
			*/
			static int getMinorRevision();

			/** Return the type of release:
					ALPHA, BETA, NONPUPLIC, STABLE, PRERELEASE, PATCHLVL, UNKNOWN.
			*/
			static Type getType() ;
			

		protected:
			static const char* RELEASE_DATE_AND_TIME;
			static const char* RELEASE_WHAT_STRING;
	};
	
}

#endif // BALL_COMMON_VERSION_H
