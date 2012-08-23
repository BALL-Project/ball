// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class MolecularControl
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/molecularControl.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static MolecularControl* getInstance(Position);

	MolecularControl() throw();
  ~MolecularControl() throw();
  //void addComposite(Composite&, String) throw();
  Size removeComposite(Composite&) throw();
	
  //constList<Composite*> & getSelection() const throw();
  PyCompositeList getSelection() throw();
  
  //void onNotify(Message *message) throw();

	virtual void updateContextMenu(Composite&) throw();

	virtual void initializeWidget(MainControl&) throw();

  Size applySelector(const String&) throw();
  Size applySelector();

	// void writePreferences(INIFile& inifile) throw();
	// void fetchPreferences(INIFile& inifile) throw();

	void showDistance();
  void createRepresentation();
  void compositeProperties();
  void bondProperties();
  void checkResidue();
  void buildBonds();
  void centerCamera();
  void countItems();
  void showFilename();
  void updateSelection();
	void highlightSelection() throw();
	void highlight(PyCompositeList) throw();
  void cut();
  void copy();
  void paste();
  void clearClipboard();
  void select();
  void deselect();
  void moveItems();
  void collapseAll();
  void expandAll();
	// see above: applySelector()
  void showSelectorHelp();
	// virtual void deleteCurrentItems() throw();
  void clearSelector();
	void switchShowSecondaryStructure();
};
