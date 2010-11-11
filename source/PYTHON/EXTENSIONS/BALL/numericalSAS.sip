// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
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
