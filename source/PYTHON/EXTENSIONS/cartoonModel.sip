// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: cartoonModel.sip,v 1.1 2003/12/11 16:05:39 amoll Exp $
//

class AddCartoonModel 
  : AddBackboneModel
{
%TypeHeaderCode
	#include <BALL/VIEW/MODELS/cartoonModel.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:
//  void dump(std::ostream& s = std::cout, Size depth = 0) const throw();
  AddCartoonModel() throw();
  AddCartoonModel(const AddCartoonModel&) throw();
  //Processor::Result operator() (Composite&);
  bool finish();
  bool start();
  float getArrowHeight() const throw();
  float getArrowWidth() const throw();
  float getHelixRadius() const throw();
  void clear() throw();
  void setArrowHeight(float) throw();
  void setArrowWidth(float) throw();
  void setHelixRadius(float) throw();
  ~AddCartoonModel() throw();
};
