class AromaticityProcessor
	: AtomContainerProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/QSAR/aromaticityProcessor.h>
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

	AromaticityProcessor();
	virtual ~AromaticityProcessor();

	Options options;
	void setDefaultOptions();
	Processor::Result operator () (AtomContainer& ac);

%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
