// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: selector.sip,v 1.2 2003/08/26 09:18:23 oliver Exp $
//
class Selector
//  : UnaryProcessor<Composite>
	: CompositeProcessor
{
%HeaderCode
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
  virtual Processor::Result operator () (Composite& /composite/) throw();
  virtual bool start() throw();
  Size getNumberOfSelectedAtoms() const;
};
