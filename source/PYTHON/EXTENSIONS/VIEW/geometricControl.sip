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
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static GeometricControl* getInstance(Position /index/);

	GeometricControl() throw();
  ~GeometricControl() throw();

	RepresentationList getHighlightedRepresentations() const throw();
			
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
