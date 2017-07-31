class AddSurfaceModel
	: ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/surfaceModel.h>
%End
	public:
	AddSurfaceModel() throw();
	AddSurfaceModel(const AddSurfaceModel&) throw();
	~AddSurfaceModel() throw();
	virtual void clear() throw();
	virtual void clearComposites() throw();
	SurfaceProcessor::SurfaceType getType() const;
	void setType(SurfaceProcessor::SurfaceType);
	void setProbeRadius(float) throw();
	float getProbeRadius() const throw();
	virtual bool createGeometricObjects() throw();
};
