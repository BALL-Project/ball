class NumericalSAS
{
%TypeHeaderCode
	#include <vector>
	#include <BALL/STRUCTURE/numericalSAS.h>
	using std::vector;
%End
	public:

	struct Option
	{
		static const String COMPUTE_AREA;
		static const String COMPUTE_VOLUME;
		static const String COMPUTE_SURFACE;
		static const String COMPUTE_SURFACE_PER_ATOM;
		static const String COMPUTE_SURFACE_MAP;
		static const String NUMBER_OF_POINTS;
		static const String PROBE_RADIUS;
	};

	struct Default
	{
		static const bool COMPUTE_AREA;
		static const bool COMPUTE_VOLUME;
		static const bool COMPUTE_SURFACE;
		static const bool COMPUTE_SURFACE_PER_ATOM;
		static const bool COMPUTE_SURFACE_MAP;
		static const Size NUMBER_OF_POINTS;
		static const float PROBE_RADIUS;
	};

	NumericalSAS();
	NumericalSAS(const Options& /In/);
	~NumericalSAS();

	void operator() (const AtomContainer& /In/);

	float getTotalArea() const;
	PyAtomDict& getAtomAreas();
	float getTotalVolume() const;
	PyAtomDict& getAtomVolumes();
	Surface& getSurface();
	void setOptions(Options& /In/);
%MethodCode
	sipCpp->options = (*(Options*)(a0));
%End

	Options options;
};
