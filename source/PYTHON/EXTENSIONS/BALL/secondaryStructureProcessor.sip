
class SecondaryStructureProcessor
	:	CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
	public:
	SecondaryStructureProcessor();
	Processor::Result operator() (Composite&);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
