class AtomBondModelBaseProcessor
	: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/atomBondModelBaseProcessor.h>
%End
	public:

	AtomBondModelBaseProcessor() throw();
	AtomBondModelBaseProcessor(const AtomBondModelBaseProcessor&) throw();
	~AtomBondModelBaseProcessor() throw();
	virtual void clear() throw();
	void set(const AtomBondModelBaseProcessor&) throw();
	virtual void clearComposites() throw();
	virtual bool createGeometricObjects() throw();
};
