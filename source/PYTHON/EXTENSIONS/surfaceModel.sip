// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: surfaceModel.sip,v 1.1 2003/12/11 16:05:40 amoll Exp $
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
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  AddSurfaceModel() throw();
  AddSurfaceModel(const AddSurfaceModel&) throw();
  //Processor::Result operator() (Composite&);
  //SurfaceProcessor::SurfaceType getType() const;
  short getType() const;
  bool finish();
  bool start();
  float getProbeRadius() const throw();
  void clear() throw();
  void setProbeRadius(float) throw();
  //void setType(SurfaceProcessor::SurfaceType);
  void setType(short);
  ~AddSurfaceModel() throw();
};
