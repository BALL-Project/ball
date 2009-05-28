
class SecondaryStructureProcessor
	:	CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
	typedef UnaryProcessor<Composite> CompositeProcessor;
	using namespace BALL;
%End
	public:
	SecondaryStructureProcessor();
	Processor::Result operator() (Composite& /composite/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
