// $Id: global.C,v 1.2 2000/01/08 12:22:58 oliver Exp $

#include <BALL/COMMON/global.h>

namespace BALL 
{

	const char* Version::RELEASE                   = BALL_RELEASE_STRING;
	const short Version::MAJOR_RELEASE             = BALL_MAJOR_RELEASE;
	const short Version::MINOR_RELEASE             = BALL_MINOR_RELEASE;

	const char* Version::RELEASE_DATE_AND_TIME = "@(#)BALL kernel compiled on: " __DATE__ ", " __TIME__;
	const char* Version::release_what_string_ = "@(#)BALL kernel version: V." BALL_RELEASE_STRING;

} // namespace BALL
