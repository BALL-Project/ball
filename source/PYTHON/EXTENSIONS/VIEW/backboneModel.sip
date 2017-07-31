class AddBackboneModel
	: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/backboneModel.h>
%End
	public:

	AddBackboneModel() throw();
	AddBackboneModel(const AddBackboneModel&) throw();
	~AddBackboneModel() throw();
	virtual void clear() throw();
	void setTubeRadius(float) throw();
	float getTubeRadius() const throw();
	virtual bool createGeometricObjects() throw();
};
