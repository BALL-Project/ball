// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

%ModuleCode
	#include <BALL/QSAR/aromaticityProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End

class AromaticityProcessor
	:	CompositeProcessor
{
%TypeHeaderCode
	#include <BALL/QSAR/aromaticityProcessor.h>
	using namespace BALL;
	using namespace BALL::Exception;
	typedef UnaryProcessor<Composite> CompositeProcessor;
%End

	public:

		struct Option
		{	
			static const char* OVERWRITE_BOND_ORDERS;	
		};

		struct Default
		{
			static const bool OVERWRITE_BOND_ORDERS;
		};

		// BALL_CREATE(AromaticityProcessor)

		AromaticityProcessor();
//		AromaticityProcessor(const AromaticityProcessor& aro);

		virtual ~AromaticityProcessor();

//		AromaticityProcessor& operator = (const AromaticityProcessor& aro);

		Options options;
		void setDefaultOptions();

//		void aromatize(const vector<vector<Atom*> >&, AtomContainer&);

//		void aromatizeSimple(vector<vector<Atom*> >&);

		Processor::Result operator () (AtomContainer& ac);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};
