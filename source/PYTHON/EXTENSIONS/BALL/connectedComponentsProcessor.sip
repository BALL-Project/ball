class ConnectedComponentsProcessor
	: AtomContainerProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/EXTENSIONS/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
	using std::vector;
%End

	public:
	ConnectedComponentsProcessor();
	virtual ~ConnectedComponentsProcessor();

	void clear();
	virtual Processor::Result operator () (AtomContainer&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End

	Size getNumberOfConnectedComponents();
	void getAllComponents(vector<Molecule>& results);
	void getMinAtomsComponents(vector<Molecule>& result, const int &min);
	void getLargestComponent(Molecule& result);
};
