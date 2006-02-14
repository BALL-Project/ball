// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.sip,v 1.3.8.1 2006/02/14 15:02:40 amoll Exp $
//

class AddLineModel 
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/lineModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

  AddLineModel() throw();
  AddLineModel(const AddLineModel&) throw();
  ~AddLineModel() throw();
  //Processor::Result operator() (Composite&);
	//void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
};
