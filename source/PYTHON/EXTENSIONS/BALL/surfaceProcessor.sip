class SurfaceProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/surfaceProcessor.h>
%End
	public:

	enum SurfaceType
	{
		SOLVENT_EXCLUDED_SURFACE,
		SOLVENT_ACCESSIBLE_SURFACE
	};

	SurfaceProcessor();
	bool start();
	bool finish();
	Surface& getSurface();

	void setProbeRadius(double);
	double getProbeRadius() const;
	void setDensity(double);
	double getDensity() const;
	void setType(SurfaceType);
	SurfaceType getType() const;
};
