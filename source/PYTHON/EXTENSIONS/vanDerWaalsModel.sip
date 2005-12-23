// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: vanDerWaalsModel.sip,v 1.3 2005/12/23 17:03:00 amoll Exp $
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
  
	AddVanDerWaalsModel() throw();
  AddVanDerWaalsModel(const AddVanDerWaalsModel&) throw();
  ~AddVanDerWaalsModel() throw();
  //Processor::Result operator() (Composite&);
	//void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void setVDWRadiusFactor(float) throw();
  float getVDWRadiusFactor() const throw();
};
