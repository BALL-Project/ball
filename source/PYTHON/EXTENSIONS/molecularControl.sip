// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.sip,v 1.11 2005/07/16 21:00:42 oliver Exp $
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
	static MolecularControl* getInstance(Position /index/);

	MolecularControl() throw();
	
  //List<Composite*> & getSelection() throw();
  PyCompositeList getSelection() throw();
  Size removeComposite(Composite&) throw();
  //constList<Composite*> & getSelection() const throw();
  //void onContextMenu_(QListViewItem*, const QPoint&, int);
  void createRepresentation();
  //void addComposite(Composite&, QString*);
  Size applySelector();
  Size applySelector(const String&) throw();
  void bondProperties();
  void buildBonds();
  void centerCamera();
  //void checkMenu(VIEW::MainControl&) throw();
  void checkResidue();
  void clearClipboard();
  void clearSelector();
  void collapseAll();
  void compositeProperties();
  void copy();
  void countItems();
  void cut();
  void deleteCurrentItems() throw();
  void deselect();
  void expandAll();
  virtual void finalizeWidget(MainControl&) throw();
  virtual void initializeWidget(MainControl&) throw();
  void invalidateSelection();
  void moveItems();
  //void onNotify(Message *message) throw();
  void paste();
  void select();
  void showFilename();
  void showSelectorHelp();
  void updateSelection();
  ~MolecularControl() throw();
};
