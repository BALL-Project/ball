// $Id: geometricProperties.sip,v 1.1 2001/09/03 16:53:08 oliver Exp $

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
  virtual bool start() throw();
  virtual bool finish() throw();
  virtual Processor::Result operator()(Atom& /atom/) throw();
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
  virtual bool start() throw();
  virtual bool finish() throw();
  virtual Processor::Result operator()(Atom& /atom/) throw();
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
	~FragmentDistanceCollector() throw();
  virtual bool start() throw();
  virtual bool finish() throw();
  virtual Processor::Result operator()(Composite& /composite/) throw();
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
