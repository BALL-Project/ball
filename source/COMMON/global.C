// $Id: global.C,v 1.1 1999/08/26 08:02:33 oliver Exp $

#include <BALL/COMMON/global.h>

namespace BALL {

	const char* Version::RELEASE                   = BALL_RELEASE_STRING;
	const short Version::MAJOR_RELEASE             = BALL_MAJOR_RELEASE;
	const short Version::MINOR_RELEASE             = BALL_MINOR_RELEASE;

	const char* Version::RELEASE_DATE_AND_TIME = "@(#)BALL kernel compiled on: " __DATE__ ", " __TIME__;
	const char* Version::release_what_string_ = "@(#)BALL kernel version: V." BALL_RELEASE_STRING;

} // namespace BALL
