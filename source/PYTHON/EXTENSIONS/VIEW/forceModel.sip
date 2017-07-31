class ForceModel
	: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/forceModel.h>
%End
	public:
	ForceModel() throw();
	ForceModel(const ForceModel&) throw();
	~ForceModel() throw();
	void setScaling(float) throw();
	void setMaxLength(float) throw();
	float getScaling() const throw();
	float getMaxLength() const throw();
};
