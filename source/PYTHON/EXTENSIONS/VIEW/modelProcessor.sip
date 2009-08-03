// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/modelProcessor.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

 	ModelProcessor() throw();
  ModelProcessor(const ModelProcessor& /model_processor/) throw();
  ~ModelProcessor() throw();
  virtual void clear() throw();
  virtual bool isValid() const throw();
	virtual void clearComposites() throw();
	//virtual void dump(std::ostream& /*s = std::cout*/, Size /*depth = 0*/) const throw();

	GeometricObjectList& getGeometricObjects() throw();
	// const GeometricObjectList& getGeometricObjects() const throw();

	void setDrawingPrecision(Index) throw();
	Index getDrawingPrecision() const throw();
	void setSurfaceDrawingPrecision(float) throw();
	float getSurfaceDrawingPrecision() const throw();
	virtual bool createGeometricObjects() throw();
				
  Processor::Result operator() (Composite& /composite/);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};
