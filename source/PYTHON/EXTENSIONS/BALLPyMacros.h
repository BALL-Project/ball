#ifndef BALL_PYTHON_EXTENSIONS_BALLPYMACROS_H
#define BALL_PYTHON_EXTENSIONS_BALLPYMACROS_H

// some macros to help us with using sip
#define BALL_SIP_TYPE(type)\
	sipType_##type

#define BALL_CONVERT_FROM_INSTANCE(object, type, transferObject)\
	sipConvertFromType(object, sipType_##type, transferObject)

#define BALL_CONVERT_FROM_MAPPED_TYPE(object, type, transferObject)\
	sipConvertFromType(object, sipType_##type, transferObject)

#define BALL_CONVERT_TO_CPP(type)\
	(type *)sipConvertToType(sipPy, sipType_##type, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr)

#define BALL_CONVERT_TO_INSTANCE(object, type, state)\
	reinterpret_cast<type*>(sipConvertToType(object, sipType_##type, 0, SIP_NOT_NONE, &state, sipIsErr))

#define BALL_IS_SUBCLASS_INSTANCE(object, type)\
	sipCanConvertToType(object, sipType_##type, SIP_NOT_NONE | SIP_NO_CONVERTORS)

#define BALL_CAN_CONVERT_TO_INSTANCE(object, type)\
	sipCanConvertToType(object, sipType_##type, SIP_NOT_NONE)

#define BALL_FORCE_CONVERT_TO_TYPE(key, type)\
	sipForceConvertToType(key, sipType_##type, NULL, SIP_NO_CONVERTORS, NULL, sipIsErr)

#define BALL_RELEASE_INSTANCE(object, type, state)\
	sipReleaseType(object, sipType_##type, state)

#define BALL_TO_SIP_MAP(type)\
	if (RTTI::isKindOf<type>(sipCpp))\
	{\
		sipType = sipType_##type;\
	}\
	else\

#endif // BALL_PYTHON_EXTENSIONS_BALLPYMACROS_H
