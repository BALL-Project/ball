// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: lineModel.sip,v 1.1 2003/12/11 16:05:40 amoll Exp $
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
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  AddLineModel() throw();
  AddLineModel(const AddLineModel&) throw();
 // Processor::Result operator() (Composite&);
  bool finish();
  bool start();
  ~AddLineModel() throw();
};
