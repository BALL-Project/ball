// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.sip,v 1.2 2003/12/11 21:08:48 amoll Exp $
//

class AddBackboneModel  
 : ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/backboneModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//SplinePoint();
//SplinePoint(const Vector3&, const Atom*);
//const Atom* getAtom() const;
//const Vector3& getTangentialVector() const;
//const Vector3& getVector() const;
//void setVector(const Vector3&);
//~SplinePoint();
//void setAtom(const Atom*);
//void setTangentialVector(const Vector3&);

  AddBackboneModel() throw();
  AddBackboneModel(const AddBackboneModel&) throw();
  ~AddBackboneModel() throw();
  void clear() throw();
  bool start();
  bool finish();
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  Processor::Result operator() (Composite&);
  void setTubeRadius(float) throw();
  float getTubeRadius() const throw();
};
