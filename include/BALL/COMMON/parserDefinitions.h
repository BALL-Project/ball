#ifndef BALL_COMMON_PARSERDEFINITIONS_H
#define BALL_COMMON_PARSERDEFINITIONS_H

static void ball_fatal_error(yyconst char* msg)
{
	throw BALL::Exception::ParseError(__FILE__, __LINE__, "", msg);
}

#undef YY_FATAL_ERROR
#define YY_FATAL_ERROR(msg) ball_fatal_error( msg )

#endif //BALL_COMMON_PARSERDEFINITIONS_H
