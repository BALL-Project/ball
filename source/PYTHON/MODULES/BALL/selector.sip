// $Id: selector.sip,v 1.1 2000/07/18 17:02:55 oliver Exp $

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
  ~Selector();
  virtual Processor::Result operator () (Composite& /composite/);
  virtual bool start();
  Size getNumberOfSelectedAtoms() const;
};
