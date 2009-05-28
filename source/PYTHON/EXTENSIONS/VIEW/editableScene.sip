// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class EditableScene 
  : Scene
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/editableScene.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

// methods implemented by the BALL_EMBEDDED macro
static Size countInstances();
static EditableScene* getInstance(Position /index/);

  EditableScene() throw();
  EditableScene(const EditableScene&);
  ~EditableScene() throw();

	void addStructure(String name);
	void setCursor(String c);
	void setElementCursor();

	public slots:

	//virtual void mousePressEvent(QMouseEvent* e);
	//virtual void mouseMoveEvent(QMouseEvent *e);
	//virtual void mouseReleaseEvent(QMouseEvent *e);
	//virtual void wheelEvent(QWheelEvent* qmouse_event);
	void createNewMolecule();
	void saturateWithHydrogens();
	void optimizeStructure();
	void setEditElementType(int element_number);
	int getEditElementType();
	};
