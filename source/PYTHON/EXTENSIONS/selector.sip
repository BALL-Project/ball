// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selector.sip,v 1.4 2003/10/29 22:54:46 oliver Exp $
//
class Selector
//  : UnaryProcessor<Composite>
	: CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/KERNEL/selector.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
  public:
  // BALL_CREATE_NODEEP(Selector)
  Selector();
  Selector(const Selector& /selector/);
  Selector(const String& /expression_string/);
  ~Selector() throw();
  Processor::Result operator () (Composite& /composite/) throw();
  virtual bool start() throw();
  Size getNumberOfSelectedAtoms() const;
};
