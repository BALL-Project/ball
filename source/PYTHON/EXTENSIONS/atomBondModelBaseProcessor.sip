// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: atomBondModelBaseProcessor.sip,v 1.1 2003/12/11 16:05:39 amoll Exp $
//

class AtomBondModelBaseProcessor 
  : ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
//  const AtomBondModelBaseProcessor& operator = (const AtomBondModelBaseProcessor& processor) throw();
  AtomBondModelBaseProcessor() throw();
  AtomBondModelBaseProcessor(const AtomBondModelBaseProcessor&) throw();
  //Processor::Result operator() (Composite&);
  bool finish();
  bool isValid() const throw();
  bool start();
  void clear() throw();
  void set(const AtomBondModelBaseProcessor&) throw();
  void swap(AtomBondModelBaseProcessor&) throw();
  ~AtomBondModelBaseProcessor() throw();
};
