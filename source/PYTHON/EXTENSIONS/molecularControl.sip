// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.sip,v 1.1 2004/07/26 21:01:07 amoll Exp $
//

class MolecularControl
  : ModularWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/molecularControl.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static MolecularControl* getInstance(Position /index/);
	
  //List<Composite*> & getSelection() throw();
  PyCompositeList getSelection() throw();
  Size removeRecursiveComposite(Composite&) throw();
  //constList<Composite*> & getSelection() const throw();
  //void onContextMenu_(QListViewItem*, const QPoint&, int);
  void createRepresentation();
  //void addComposite(Composite&, QString*);
  void applySelector();
  void bondProperties();
  void buildBonds();
  void buildContextMenu(Composite&) throw();
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
  //void finalizeWidget(MainControl&) throw();
  //void initializeWidget(MainControl&) throw();
  void invalidateSelection();
  void move();
  //void onNotify(Message *message) throw();
  void paste();
  void removeComposite(Composite&) throw();
  void select();
  void showFilename();
  void showSelectorHelp();
  void updateSelection();
  ~MolecularControl() throw();
};
