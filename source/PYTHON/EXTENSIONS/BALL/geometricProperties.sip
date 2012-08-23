// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class BoundingBoxProcessor
//  : public UnaryProcessor<Atom>
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  virtual bool start() throw();
  virtual bool finish() throw();
  Processor::Result operator()(Atom&) throw();
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
	SimpleBox3 getBox() const;
  const Vector3& getLower() const;
  const Vector3& getUpper() const;
};

class GeometricCenterProcessor
//	:	public UnaryProcessor<Atom> 
	: AtomProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Atom> AtomProcessor;
%End
  public:
  virtual bool start() throw();
  virtual bool finish() throw();
  Processor::Result operator()(Atom&) throw();
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
  Vector3& getCenter();
};

class FragmentDistanceCollector
//  : public UnaryProcessor<Composite> 
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
  public:
  FragmentDistanceCollector();
  FragmentDistanceCollector(const Composite&);
  FragmentDistanceCollector(const Composite&, float);
	~FragmentDistanceCollector() throw();
  virtual bool start() throw();
  virtual bool finish() throw();
  Processor::Result operator()(Composite&) throw();
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
  Size getNumberOfFragments();
  void setComposite(const Composite&);
  const Composite* getComposite() const;
  float getDistance() const;
  void setDistance(float);
  // vector<Fragment*> fragments;
};


%ModuleCode
	#include <BALL/STRUCTURE/geometricProperties.h>
	using namespace BALL;
%End
Angle calculateTorsionAngle(const Atom&, const Atom&, const Atom&, const Atom&);
Angle setTorsionAngle(const Atom&, const Atom&, Atom&, const Atom&, Angle a);
Angle calculateBondAngle(const Atom&, const Atom&, const Atom&);
