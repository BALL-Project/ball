// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class GeometricControl
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/geometricControl.h>
	#include <BALL/VIEW/DIALOGS/modifyRepresentationDialog.h>
	#include <list>
	using namespace BALL;
	using namespace BALL::VIEW;
	using namespace std;
%End
  public:

	static Size countInstances();
	static GeometricControl* getInstance(Position);

	GeometricControl() throw();
  ~GeometricControl() throw();

	list<Representation*> getHighlightedRepresentations() const throw();
			
	virtual void selectAtoms();
//	virtual void selectedRepresentation(Representation& representation, bool state);
	virtual void updateSelection();
	virtual void focus();
	virtual void enterMoveMode();
	virtual void flipClippingPlane();
	virtual void setClippingPlaneX();
	virtual void setClippingPlaneY();
	virtual void setClippingPlaneZ();
	void hideShowClippingPlane();
	virtual void selectClipRepresentations();
	void createNewClippingPlane();
	void duplicate();
	void renameRepresentation();
	ModifyRepresentationDialog* getModifySurfaceDialog();
};
