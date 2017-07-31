class ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/modelProcessor.h>
%End
	public:
	ModelProcessor() throw();
	ModelProcessor(const ModelProcessor&) throw();
	~ModelProcessor() throw();
	virtual void clear() throw();
	virtual bool isValid() const throw();
	virtual void clearComposites() throw();
	list<GeometricObject*>& getGeometricObjects() throw();
	void setDrawingPrecision(Index) throw();
	Index getDrawingPrecision() const throw();
	void setSurfaceDrawingPrecision(float) throw();
	float getSurfaceDrawingPrecision() const throw();
	virtual bool createGeometricObjects() throw();

	Processor::Result operator() (Composite&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
