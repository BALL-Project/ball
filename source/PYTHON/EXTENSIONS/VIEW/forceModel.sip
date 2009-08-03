class ForceModel 
: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/forceModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
			
	ForceModel() throw();
	ForceModel(const ForceModel&) throw();
	~ForceModel() throw();
	void setScaling(float) throw();
	void setMaxLength(float) throw();
	float getScaling() const throw();
	float getMaxLength() const throw();
	//virtual Processor::Result operator() (Composite& composite);
};
