// $Id: version.h,v 1.6 2000/11/13 01:49:31 amoll Exp $

#ifndef BALL_COMMON_VERSION_H
#define BALL_COMMON_VERSION_H

#ifndef BALL_COMMON_EXCEPTION_H
#	include <BALL/COMMON/exception.h>
#endif

//
//  master data for all revisions is stored in this define!
//
#define BALL_RELEASE_STRING             "0.96a"

namespace BALL
{

	/**	@name Version information class.
			The BALL release, release data and related version
			can be retrieved by these methods.\\
			BALL releases ending on "a" are alpha releases and
			release strings ending on "b" are beta releases.
			Stable releases are not suffixed by a letter.
			The VersionInfo class contains only static methods
			that can be used to retrieve the desired information.
			\\
			{\bf Definition:}\URL{BALL/COMMON/version.h}
			\\
	*/
	class VersionInfo
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
			UNKNOWN
		};

		/** Return the version number, which is defined in
				BALL_RELEASE_STRING in \URL{BALL/COMMON/version.h}
		*/
		static const char* getVersion() throw();

		/** Return the major revision number.
				(The part of the release number before the point)
		*/
		static int getMajorRevision() throw(Exception::InvalidFormat);

		/** Return the minor revision number.
				(The part of the release number after the point)
		*/
		static int getMinorRevision() throw(Exception::InvalidFormat);

		/** Return the type of release:
				ALPHA, BETA, NONPUPLIC, STABLE, UNKNOWN.
		*/
		static Type getType() throw();
		

		protected:

		static const char* RELEASE_DATE_AND_TIME;
	  static const char* RELEASE_WHAT_STRING;
	};
}

#endif // BALL_COMMON_VERSION_H
