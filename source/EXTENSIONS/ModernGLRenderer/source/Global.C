#include <Global.h>


using namespace std;

namespace Global
{
	namespace
	{
		float scale_factor_ = 0.1;

		bool transparency_support_ = false;
	}

	float getScaleFactor()
	{
		return scale_factor_;
	}

	void setScaleFactor ( float scale_factor )
	{
		scale_factor_ = scale_factor;
	}

	bool getTransparencySupport()
	{
		return transparency_support_;
	}

	void setTransparencySupport(bool supported)
	{
		transparency_support_ = supported;
	}
}
