#include <BALL/VIEW/KERNEL/common.h>

namespace BALL
{
	namespace VIEW
	{

String getModelName(ModelTypes type) 
	throw()
{
	switch (type)
	{
		case MODEL_LINES:
			return "Line";
		case MODEL_STICK:
			return "Stick";
		case MODEL_BALL_AND_STICK:
			return "BallandStick";
		case MODEL_VDW:
			return "VDW";
		case MODEL_SE_SURFACE:
			return "SES";
		case MODEL_SA_SURFACE:
			return "SAS";
		case MODEL_BACKBONE:
			return "Backbone";
		case MODEL_CARTOON:
			return "Cartoon";
		case MODEL_HBONDS:
			return "H-Bonds";
		case MODEL_LABEL:
			return "Label";

		case MODEL_UNKNOWN:
			return "Unknown";
	}
	return "Unknown";
}


} } //namespaces
