// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ModifyRepresentationDialog
  : ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	// methods implemented by the BALL_EMBEDDED macro
	static Size countInstances();
	static ModifyRepresentationDialog* getInstance(Position);

  ~ModifyRepresentationDialog() throw();

	void setRepresentation(Representation*) throw();
	void setGrid(RegularData3D*) throw();
	void setMinValue(float);
	void setMaxValue(float);
	void setMidValue(float);
	void setSplitRadius(float);

	void accept();
	void reject();
	void tabChanged();		
	void autoScale();
	void choosePressed();
	void gridSelected();
	void show();
	void applySplit();

	private:
		ModifyRepresentationDialog(const ModifyRepresentationDialog&);
};
