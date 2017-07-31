class HBondModelProcessor
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/HBondModel.h>
%End
	public:

	HBondModelProcessor() throw();
	HBondModelProcessor(const HBondModelProcessor&) throw();
	~HBondModelProcessor() throw();
	virtual void clear() throw();
	void set(const HBondModelProcessor&) throw();
	void setRadius(float) throw();
	float getRadius() const throw();
};
