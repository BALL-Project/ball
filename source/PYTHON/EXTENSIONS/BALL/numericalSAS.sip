// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class NumericalSAS
{
%TypeHeaderCode
  #include <BALL/STRUCTURE/numericalSAS.h>
  
  #include <vector>

  using namespace BALL;
  using namespace std;
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

//    BALL_CREATE(NumericalSAS)

    NumericalSAS();
    NumericalSAS(const Options& /In/);
    ~NumericalSAS();
    
    void operator() (const AtomContainer& /In/);

    float getTotalArea() const;
//    HashMap<const Atom*, float>& getAtomAreas();
    PyAtomDict& getAtomAreas();
//    const HashMap<const Atom*, float>& getAtomAreas() const;

    float getTotalVolume() const;
    //HashMap<const Atom*, float>& getAtomVolumes();
    PyAtomDict& getAtomVolumes();
 //   const HashMap<const Atom*, float>& getAtomVolumes() const;

    Surface& getSurface();
    //const Surface& getSurface() const;
//    HashMap<const Atom*, Surface>& getSurfacePerAtom();
//   const HashMap<const Atom*, Surface>& getSurfacePerAtom() const;

 //   vector<pair<Vector3, Surface> >& getSurfaceMap();
//    const vector<pair<Vector3, Surface> >& getSurfaceMap() const;
    
    void setOptions(Options& /In/);
%MethodCode
  sipCpp->options = (*(Options*)(a0));
%End

    Options options;
};

%ModuleCode
	#include <BALL/STRUCTURE/numericalSAS.h>
	using namespace BALL;
	#if SIP_VERSION >= 0x040a00
		#define PARSE_ARG sipParseErr
	#else
		#define PARSE_ARG sipArgsParsed
	#endif
%End

float calculateSASArea
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
float calculateSASVolume
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400); 
PyAtomDict& calculateSASAtomAreas
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MethodCode
	const AtomContainer *a0;
	float a1 = 1.5;
	Size a2 = 400;

	if (sipParseArgs(&PARSE_ARG,sipArgs,"J1|fi",BALL_SIP_TYPE(AtomContainer),&a0,&a1,&a2))
	{
		PyAtomDict *res;
  printf("%d, %f\n", a2, a1);
 
		res = new PyAtomDict;
		calculateSASAtomAreas(*a0, *res, a1, a2);
		PyObject *resobj = BALL_CONVERT_FROM_MAPPED_TYPE(res,PyAtomDict,NULL);

		delete res;

		return resobj;
	}
%End

Surface& calculateSASPoints
     (const AtomContainer& /fragment/, float /probe_radius/ = 1.5, Size /number_of_dots/ = 400);
%MethodCode
	const AtomContainer *a0;
	float a1 = 1.5;
	Size a2 = 400;

	if (sipParseArgs(&PARSE_ARG,sipArgs,"J1|fi",BALL_SIP_TYPE(AtomContainer), &a0, &a1, &a2))
	{
		Surface* res = new Surface;
		calculateSASPoints(*a0, *res, a1, a2);

		return BALL_CONVERT_FROM_INSTANCE(res,Surface, 0);
	}
%End
