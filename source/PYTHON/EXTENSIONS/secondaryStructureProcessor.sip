
class SecondaryStructureProcessor
	:	CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/secondaryStructureProcessor.h>
	using namespace BALL;
%End
	public:
	SecondaryStructureProcessor();
	void compute();
	// Processor::Result operator() (Composite& /composite/);
};
