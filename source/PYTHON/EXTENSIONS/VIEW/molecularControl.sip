class MolecularControl
	: DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/molecularControl.h>
%End
	public:

	static Size countInstances();
	static MolecularControl* getInstance(Position);

	MolecularControl() throw();
	~MolecularControl() throw();

	Size removeComposite(Composite&) throw();
	PyCompositeList getSelection() throw();
	virtual void updateContextMenu(Composite&) throw();
	virtual void initializeWidget(MainControl&) throw();
	Size applySelector(const String&) throw();
	Size applySelector();
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
	void clearSelector();
	void switchShowSecondaryStructure();
};
