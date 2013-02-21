// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
#define BALL_VIEW_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
# include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#ifndef BALL_CONCEPT_MOLECULARINFORMATION_H
# include <BALL/CONCEPT/molecularInformation.h>
#endif

#ifndef BALL_SYSTEM_MUTEX_H
# include <BALL/SYSTEM/mutex.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#include <QtGui/QMenu>
#include <QtGui/QTreeView>

class QComboBox;
class QPoint;
class QSignalMapper;
class QThread;

namespace BALL
{
	class Residue;
	class RotamerLibrary;

	namespace VIEW
	{
		class BondProperties;

/**	MolecularControl is a widget to display the molecular structure of Composite objects. 
		This class is derived from the class GenericControl and extends it for showing and modifiying
		molecular structures. The methods checkMenu() and buildContextMenu() are overridden 
		for performing special molecular tasks.
		MolecularControl has also a QLineEdit to select Composites by BALL expressions.
		For further informations on this topic have a look at the class Selector.
		\ingroup ViewWidgets
*/
class BALL_VIEW_EXPORT MolecularControl
	: public GenericControl
{
	// for internal usage only:
	class MyTreeWidgetItem
		: public QTreeWidgetItem
	{
		public:

			MyTreeWidgetItem(QTreeWidget* parent, QStringList& sl, Composite* composite);

			MyTreeWidgetItem(QTreeWidgetItem* parent, QStringList& sl, Composite* composite);

			void init_();

			Composite* composite;
	};

	///
	enum MolecularMenuEntries
	{
		/// create a new Representation
		CREATE_REPRESENTATION,
		OBJECT__MOVE,
		SELECT,
		DESELECT,
		EDIT_MENU,
		CUT,
		COPY,
		DELETE_ENTRY,
		PASTE,


		/// Center camera on one Composite (done in MolecularProperites)
		CAMERA__CENTER,
		/// Show a dialog with the atom properties
		COMPOSITE__PROPERTIES,
		///
		COUNT__ITEMS,
		/// Show a dialog with the bond properties
		BOND__PROPERTIES,
		/// Collapse all QListViewItem
		COLLAPSE_ALL,
		/// Expand all QListViewItem
		EXPAND_ALL
	};

	Q_OBJECT

	public:

	BALL_EMBEDDABLE(MolecularControl,GenericControl)

	friend class BondProperties;

	/**	@name	Constructors and Destructor
	*/	
	//@{

	/** Default Constructor.
			\param      parent the parent widget 
			\param      name the name of this widget
	*/
	MolecularControl(QWidget* parent = 0, const char* name = 0);
	
	/** Destructor.
	*/
	virtual ~MolecularControl();
	
	//@} 
	/**	@name	Accessors: inspectors and mutators 
	*/ 
	//@{
	
	/**	Menu checking method.
			This method is called by MainControl::checkMenus before a popup menu is shown.
			The copy menu entry is only enabled for selected System objects.
			\param main_control the MainControl object whose menus should be checked
			\see   Control::checkMenu
	*/
	virtual void checkMenu(VIEW::MainControl& main_control);

	/** Insert a Composite object into this Control.
			If <tt>name == ""</tt> than the Information visitor is used for determining
			a name for the Composite.\par
			Calls generateListViewItem_() for the Composite.
			\param   composite a pointer to the Composite to be inserted into the Control
	*/
	void addComposite(Composite& composite, String given_name = "");

	/** Recursive removal of a Composite from the Control.
	*/
	Size removeComposite(Composite& composite);

	/** Non-mutable inspection of the selection.
	*/
	const std::list<Composite*>& getSelection() const;

	/** Inspection of the entry for which a context menu was requested.
	 */
	Composite* getContextComposite();
	
	/** Message handling.
			Overridden method from ConnectionObject to handle incoming messages.\par
			Calls reactToMessages_().\par
			\param  message a pointer to a Message object
	*/
	virtual void onNotify(Message *message);

	/** Build a context menu for a Composite.
			If the Composite has certain properties a context menu is created.
			This method only creates the necessary interface for derived classes.
			Override this method for creating other context menu entries, but call 
			Control::buildContextMenu at the end of your method to create the standard entries.
			Calls insertContextMenuEntry() for each context menu entry that will be created.
			\param   composite the Composite object for that a context menu should be created
			\see     onContextMenu
	*/
	virtual void updateContextMenu(Composite& composite);

	/**	Initialize the menu entries:
				- cut 
				- copy 
				- paste 
				- delete
				- clear clipboard
			\par
			This method is called automatically	immediately before the main application 
			is started by MainControl::show.
			\param main_control the MainControl object to be initialized with this ModularWidget
	*/
	virtual void initializeWidget(MainControl& main_control);

	/** Apply a given BALL expression
			@return Size number of selected atoms
	*/
	Size applySelector(const String& expression);

	///
	void writePreferences(INIFile& inifile);

	///
	void fetchPreferences(INIFile& inifile);

	///
	void showDistance(Atom* a1, Atom* a2);
	
	///
	void showAngle(Atom* a1, Atom* a2, Atom* a3, Atom* a4 = 0);

	/** Allow access to the context menu
	 */
	QMenu& getContextMenu();

	public slots:
		
	//@}
	/** @name Public slots
	*/
	//@{

	/// Create a Representation with the DisplayProperties Dialog
	void createRepresentation();

	/// Composite properties dialog requested
	void compositeProperties();

	/// Create a disulfid bond
	void toggleDisulfidBond();

	/// Bond properties dialog requested
	void bondProperties();

	/** Check residues
	 */
	void checkResidue();

	/** Build bonds
	 */
	void buildBonds();
	
	/** Center the camera on a composite.
	 */
	void centerCamera();

	/// Count the molecular items and print the info
	void countItems();

	/// Show the filename from which the composite was loaded.
	void showFilename();

	/** Prints the filename from which a Systems was loaded, after its selection.\par
			Calls showFilename()
	*/
	virtual void updateSelection();

	///
	void highlightSelection();

	///
	void highlight(const std::list<Composite*>& composite);

	/** Cut the selected Composite objects and copy them into an internal
			copy list. If there are already Composite objects in the 
			copy list they will be deleted. 
			All Composite objects that are cutted are removed from the Control.
			Cuted systems are moved to the copy list.
			A RemovedCompositeMessage will be sent for each cutted object.
			After all selected objects are removed a SceneMessage will be sent to
			update the Scene. 
	*/
	void cut();

	/** Copy the selected Composite objects into an internal
			copy list. If there are already Composite objects in the 
			copy list they will be deleted. 
	*/
	void copy();

	/** Paste the copied Composite objects from the internal
			copy list into the Control. 
			For each Composite in the copy list a NewCompositeMessage will be sent.
	*/
	void paste();

	/** Clear the copied Composite objects from the internal copy list.
	*/
	void clearClipboard();

	/** Select 
	 */
	void select();
	
	/** Deselect
	 */
	void deselect();

	/// Move a composite
	void moveItems();
	
	/// Collapse all QListViewItem
	void collapseAll();

	/// Expand all QListViewItem
	void expandAll();

	/** Apply the BALL expression from the QLineEdit
			@return Size number of selected atoms
	*/
	Size applySelector();

	/// Show a help dialog for the Selector 
	void showSelectorHelp();

	/// Overloaded from GenericControl, calls cut
	virtual void deleteCurrentItems();

	/// Connected to the clear selection button
	virtual void clearSelector();

	void switchShowSecondaryStructure();

	///
	void showAtomOverview();

	///
	void showAtomOverviewForSelection();

	///
	void showDistance();
	
	///
	void showAngle();
	
	//@} 
	/** @name Protected members 
	*/ 
	//@{
	protected slots:

	/*_ Controlling method for context menus.
			Clear the previously created context menu.
			Calls buildContextMenu for the Composite object belonging
			to the <tt>item</tt> and executes the context menu if menu entries are available.
			\param  point the position to which the context menu should be drawn
			\param  column not used at the moment
			\see    buildContextMenu
	*/
	void showGuestContextMenu(const QPoint& pos);

	//_ called when a model is selected in the context menu
	void activatedItem_(QAction* action);

	//_
	void createRepresentation_();

	//
	void onItemClicked(QTreeWidgetItem* item, int);

	void changeRotamer_(int i);

	protected:

	///
	void buildContextMenu_();

	///
	void buildRotamerMenu_();

	/** Set the selection of the checkboxes and the opening of the tree 
			according to the selection in the MainControl.
			\param open true means, that the item tree is opend and closed according to the changes
			\param force true means, that the item tree is opend and closed
						 also if more than 50 items are selected.
	*/
	void setSelection_(bool open, bool force = false);

	/** Access the MolecularInformation visitor.
			With the MolecularInformation, the names and type entries for the Items
			are generated.
			Override this method if another information visitor is needed.
			This method is used in the method generateListViewItem_() to
			retrieve certain information of the given Composite.
	*/
	virtual MolecularInformation& getInformationVisitor_();
	
	/** Iterate over the children of the Composite and
			call for each the method generateListViewItem_().
			\param   item a pointer to a QTreeWidgetItem to which all children of the Composite 
							 will be inserted
			\param   composite whose children will be inserted into <tt>item</tt>
	*/
	virtual void recurseGeneration_(QTreeWidgetItem* item, Composite& composite);

	/** Message handling.
			Catch the Message objects and react accordingly to the different messages.
			Override this method if new messages should be catched.\par
			<b>Note:</b> If this method is overriden, call this method at the end of the
			new implementation to make sure the old messages are still catched properly.\par
			Messages to be catched and the resulting reaction:
				-  NewCompositeMessage - update always necessary
				-  RemovedCompositeMessage - update necessary if already inserted
				-  ChangedCompositeMessage - update necessary if Composite object has changed
			\par
			\param   message a pointer to the Message object
			\return  bool <tt>true</tt> if an update of the Control is necessary, <tt>false</tt> otherwise
			\see     onNotify
	*/
	virtual bool reactToMessages_(Message* message);

	/** Generate a new QTreeWidgetItem for the given Composite.
			If <tt> default_name == 0</tt> than a new name is created using the 
			Information visitor accessed with the method getInformationVisitor_().
			If <tt>parent == 0</tt> than the new QTreeWidgetItem is a root item.
			Otherwise a new QTreeWidgetItem for is created that will
			be inserted into given <b>parent</b>.
			All children of <b> composite</b> will be inserted recursivly into the newly
			created item by the method recurseGeneration_().
			\param  parent a QTreeWidgetItem into which a subtree of items will be inserted, 
									 or <tt> 0</tt> if a new root item should be created
			\param  composite the Composite object whose subtree will be inserted into <b>parent</b>
			\param  default_name the name of the <b>item</b>
	*/
	QTreeWidgetItem* generateListViewItem_(QTreeWidgetItem* parent,
																					Composite& composite, QString* default_name = 0);

	//_ Test, if its allowed to paste the copy liste into the current selected context item.
	bool pasteAllowedFor_(Composite& composite);

	//
	inline void removeRecursive_(QTreeWidgetItem* item);

	// only for Python Interface
	MolecularControl(const MolecularControl& mc);

	//
	std::list<QTreeWidgetItem*> getAllItems_();

	void enableUpdates_(bool state);
	void newSelection_(std::list<Composite*>& sel, bool selected);

	bool allowPaste_();

	//@} 
	/** @name Menu entries ids
	*/
	//@{

	//_
	QAction* cut_id_, *copy_id_, *paste_id_, *delete_id_, *clipboard_id_, *select_id_, *deselect_id_,
			*show_ss_id_;

	//@}

	std::list<Composite*>   selected_;
	std::list<Composite*>   copy_list_;

	MolecularInformation    information_;

	QComboBox*              selector_edit_;
	QComboBox*              smarts_edit_;

	// the context menus
	QMenu            context_menu_,
									 model_menu_,
									 edit_menu_,
									 color_menu_[MODEL_LABEL - MODEL_LINES];

	QMenu* rotamer_menu_;

	Composite*       context_composite_;

	QTreeWidgetItem* context_item_;

	ModelType        selected_model_;
	ColoringMethod   selected_coloring_method_;
	std::map<Composite*, MyTreeWidgetItem*> composite_to_item_;

	// let cut know to delete the entries, set by deleteCurrentItems()
	bool            was_delete_;

	Size            nr_items_removed_;

	bool            show_ss_;
	QAction* center_camera_action_, *composite_properties_action_, *bond_propertes_action_, *disulfidbond_action_,
					 *select_action_, *deselect_action_, *count_items_action_, *atom_overview_, *atom_overview_selection_,
					 *angle_action_, *distance_action_, *paste_action_;

	bool ignore_messages_;

	QSignalMapper* rotamer_mapper_;
	Residue* current_residue_;
	RotamerLibrary* rotamer_library_;
	ReadWriteLock rotamer_library_mutex_;
	QThread* rl_thread_;
};

}} // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
