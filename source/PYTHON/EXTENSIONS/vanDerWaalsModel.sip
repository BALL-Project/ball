// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.sip,v 1.2 2003/12/15 12:00:13 amoll Exp $
//

class AddVanDerWaalsModel 
: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  AddVanDerWaalsModel() throw();
  AddVanDerWaalsModel(const AddVanDerWaalsModel&) throw();
  //Processor::Result operator() (Composite&);
  bool finish();
  float getVDWRadiusFactor() const throw();
  void setVDWRadiusFactor(float) throw();
  ~AddVanDerWaalsModel() throw();
};
