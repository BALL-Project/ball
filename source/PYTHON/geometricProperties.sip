// $Id: geometricProperties.sip,v 1.1 2000/07/06 14:08:47 oliver Exp $

class BoundingBoxProcessor
//  : public UnaryProcessor<Atom>
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  virtual bool start();
  virtual bool finish();
  virtual Processor::Result operator()(Atom& /atom/);
	Box3 getBox() const;
  const Vector3& getLower() const;
  const Vector3& getUpper() const;
};

class GeometricCenterProcessor
//	:	public UnaryProcessor<Atom> 
	: AtomProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  virtual bool start();
  virtual bool finish();
  virtual Processor::Result operator()(Atom& /atom/);
  Vector3& getCenter();
};

class FragmentDistanceCollector
//  : public UnaryProcessor<Composite> 
	: CompositeProcessor
{
%HeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
  public:
  FragmentDistanceCollector();
  FragmentDistanceCollector(const Composite& /composite/);
  FragmentDistanceCollector(const Composite& /composite/, float /distance/);
  virtual bool start();
  virtual bool finish();
  virtual Processor::Result operator()(Composite& /composite/);
  Size getNumberOfFragments();
  void setComposite(const Composite& /composite/);
  const Composite* getComposite() const;
  float getDistance() const;
  void setDistance(float /distance/);
  // vector<Fragment*> fragments;
};


%HeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
%End
Angle calculateTorsionAngle(const Atom& /a1/, const Atom& /a2/, const Atom& /a3/, const Atom& /a4/);
Angle calculateBondAngle(const Atom& /a1/, const Atom& /a2/, const Atom& /a3/); 
