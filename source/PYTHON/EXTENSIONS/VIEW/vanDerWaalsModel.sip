class AddVanDerWaalsModel
	: AtomBondModelBaseProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/vanDerWaalsModel.h>
%End
	public:
	AddVanDerWaalsModel() throw();
	AddVanDerWaalsModel(const AddVanDerWaalsModel&) throw();
	~AddVanDerWaalsModel() throw();
	void setVDWRadiusFactor(float) throw();
	float getVDWRadiusFactor() const throw();
};
