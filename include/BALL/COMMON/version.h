// $Id: version.h,v 1.4 2000/06/27 23:11:18 oliver Exp $

#ifndef BALL_COMMON_VERSION_H
#define BALL_COMMON_VERSION_H

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
		/**
		*/
		enum Type
		{
			STABLE,
			ALPHA,
			BETA,
			NONPUBLIC,
			UNKNOWN
		};

		/**
		*/
		static const char* getVersion();

		/**
		*/
		static int getMajorRevision();

		/**
		*/
		static int getMinorRevision();

		/**
		*/
		static Type getType();
		
		protected:
		static const char* VersionInfo::RELEASE_DATE_AND_TIME;
	  static const char* VersionInfo::RELEASE_WHAT_STRING;
	};
}

#endif // BALL_COMMON_VERSION_H
