// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: HBondModel.sip,v 1.1 2003/12/11 16:05:39 amoll Exp $
//

class HBondModelProcessor  
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/HBondModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  const HBondModelProcessor& operator = (const HBondModelProcessor& model) throw();
  HBondModelProcessor() throw();
  HBondModelProcessor(const HBondModelProcessor&) throw();
  //Processor::Result operator() (Composite&);
  bool finish();
  float getRadius() const throw();
  void clear() throw();
  void set(const HBondModelProcessor&) throw();
  void setRadius(float) throw();
  void swap(HBondModelProcessor&) throw();
  ~HBondModelProcessor() throw();
};
