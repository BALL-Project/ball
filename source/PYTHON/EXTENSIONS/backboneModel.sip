// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: backboneModel.sip,v 1.1 2003/12/11 16:05:39 amoll Exp $
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
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  AddBackboneModel() throw();
  AddBackboneModel(const AddBackboneModel&) throw();
//  Processor::Result operator() (Composite&);
//  SplinePoint();
//  SplinePoint(const Vector3&, const Atom*);
  bool finish();
  bool start();
  const Atom* getAtom() const;
  const Vector3& getTangentialVector() const;
  const Vector3& getVector() const;
  float getTubeRadius() const throw();
  void clear() throw();
  void setAtom(const Atom*);
  void setTangentialVector(const Vector3&);
  void setTubeRadius(float) throw();
  void setVector(const Vector3&);
  ~AddBackboneModel() throw();
  //~SplinePoint();
};
