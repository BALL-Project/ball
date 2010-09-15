  
	class HBondProcessor 
		:	CompositeProcessor
  {
%TypeHeaderCode
	#include <BALL/STRUCTURE/HBondProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End
		public:
    HBondProcessor();
    Processor::Result operator() (Composite& /composite/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End

	Options options;
	private:
		const HBondProcessor& operator=(const HBondProcessor&);
};
