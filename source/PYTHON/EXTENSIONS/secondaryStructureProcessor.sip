
class SecondaryStructureProcessor
	:	CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
	using namespace BALL;
%End
	public:
	SecondaryStructureProcessor();
	Processor::Result operator() (Composite& /composite/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
