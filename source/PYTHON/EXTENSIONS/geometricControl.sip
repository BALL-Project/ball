// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: geometricControl.sip,v 1.2.6.1 2006/02/14 15:02:39 amoll Exp $
//

class GeometricControl
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/geometricControl.h>
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
	virtual void focusRepresentation();
	virtual void moveClippingPlane();
	virtual void flipClippingPlane();
	virtual void setClippingPlaneX();
	virtual void setClippingPlaneY();
	virtual void setClippingPlaneZ();
	void hideShowClippingPlane();
	virtual void selectClipRepresentations();
	void createNewClippingPlane();
	void duplicateRepresentation();
};
