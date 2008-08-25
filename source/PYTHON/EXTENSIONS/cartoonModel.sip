// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
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

	AddCartoonModel() throw();
  AddCartoonModel(const AddCartoonModel&) throw();
  ~AddCartoonModel() throw();
  //Processor::Result operator() (Composite&);

	void setHelixRadius(float) throw();
	float getHelixRadius() const throw();
	void setArrowWidth(float) throw();
	float getArrowWidth() const throw();
	void setStrandHeight(float) throw();
	float getStrandHeight() const throw();
	void setStrandWidth(float) throw();
	float getStrandWidth() const throw();
	void setDNABaseRadius(float) throw();
	float getDNABaseRadius() const throw();
	void setDNAHelixRadius(float) throw();
	float getDNAHelixRadius() const throw();
	void setDNALadderRadius(float) throw();
	float getDNALadderRadius() const throw();
	void setDrawDNAAsLadderModel(bool) throw();
	bool drawDNAAsLadderModel() throw();
	void enableRibbons(bool) throw();
	bool ribbonsEnabled() const throw();
};
