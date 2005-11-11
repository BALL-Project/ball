// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.sip,v 1.2.6.1 2005/11/11 17:21:26 leonhardt Exp $
//

class AddSurfaceModel 
  : ModelProcessor
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/surfaceModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
  
	AddSurfaceModel() throw();
  AddSurfaceModel(const AddSurfaceModel&) throw();
  ~AddSurfaceModel() throw();
  virtual void clear() throw();
	virtual void clearComposites() throw();
  SurfaceProcessor::SurfaceType getType() const;
  void setType(SurfaceProcessor::SurfaceType);
  //Processor::Result operator() (Composite&);
  //void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  void setProbeRadius(float) throw();
  float getProbeRadius() const throw();
	virtual bool createGeometricObjects() throw();
};
