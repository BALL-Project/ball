// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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

  AddBackboneModel() throw();
  AddBackboneModel(const AddBackboneModel&) throw();
  ~AddBackboneModel() throw();
  virtual void clear() throw();
//  Processor::Result operator() (Composite&);
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void setTubeRadius(float) throw();
  float getTubeRadius() const throw();
	virtual bool createGeometricObjects() throw();
};
