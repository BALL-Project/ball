// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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
  Selector(const Selector&);
  Selector(const String&);
  ~Selector();
  Processor::Result operator () (Composite&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

  virtual bool start();
  Size getNumberOfSelectedAtoms() const;
	list<Atom*> getSelectedAtoms();
};
