// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.h,v 1.40 2004/11/23 17:27:10 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
#define BALL_VIEW_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
#	include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_MOLECULARINFORMATION_H
#	include <BALL/VIEW/KERNEL/molecularInformation.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_MATHS_MATRIX44_H
 #include <BALL/MATHS/matrix44.h>
#endif

#include <qlistview.h>
#include <qpoint.h>
#include <qpopupmenu.h>


namespace BALL
{
	namespace VIEW
	{
		class BondProperties;

		/**	MolecularControl is a widget to display the molecular structure of Composite objects. 
				This class is derived from the class GenericControl and extends it for showing and modifiying
				molecular structures. The methods checkMenu() and buildContextMenu() are overridden 
				for performing special molecular tasks.
				MolecularControl has also a QLineEdit to select Composites by regular expressions.
				For further informations on this topic have a look at the class Selector.
				\ingroup ViewWidgets
		*/
		class BALL_EXPORT MolecularControl
			: public GenericControl
		{			
			///
			enum MolecularMenuEntries
			{
				/// create a new Representation
				CREATE_REPRESENTATION,
				OBJECT__MOVE,
				SELECT,
				DESELECT,

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
			
			/// A selectable list view item with a pointer to a Composite
			class SelectableListViewItem
				: public QCheckListItem
			{
				public:

				/// Detailed Constructor with an other SelectableListViewItem as parent
				SelectableListViewItem(QListViewItem* parent, const QString& text, const QString& type, 
															 Composite* composite, VIEW::MolecularControl& control)
					throw();

				/// Detailed Constructor with a list view as parent
				SelectableListViewItem(QListView* parent, const QString& text, const QString& type, 
															 Composite* composite, VIEW::MolecularControl& control)
					throw();

				/// Get the associated Composite pointer
				Composite* getComposite() { return composite_;};

 				/** Overriden function, to deselect child items, if this item is deselected.
				 		This is necessary because, when using the SHIFT-modifier in selection,
						also the collapsed child items get selected.
				*/
				virtual void setSelected (bool state);

				protected:

				// overriden function, used to message to Control
				virtual void stateChange(bool state)
					throw();

				Composite* 					composite_;
				MolecularControl& 	control_reference_;
				bool 								ignore_change_;

				private: 
				
				// prevent use of default cstr
				SelectableListViewItem();
			};

			friend class SelectableListViewItem;
			friend class BondProperties;

			/**	@name	Constructors and Destructor
			*/	
			//@{

			/** Default Constructor.
					\param      parent the parent widget 
					\param      name the name of this widget
			*/
			MolecularControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~MolecularControl()
				throw();
			
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
			virtual void checkMenu(VIEW::MainControl& main_control)
				throw();

			/** Insert a Composite object into this Control.
					If <tt>name == 0</tt> than the Information visitor is used for determining
					a name for the Composite.\par
					Calls generateListViewItem_() for the Composite.
					\param   composite a pointer to the Composite to be inserted into the Control
					\param   name a name, the SelectableListViewItem for the Composite should receive
			*/
			void addComposite(Composite& composite, QString* name = 0)
				throw();

			/** Recursive removal of a Composite from the Control.
			 		\see removeComposite
			*/
			Size removeRecursiveComposite(Composite& composite)
				throw();

			/** Access the mutable reference to the selection list of this control.
			*/
			List<Composite*>& getSelection()
				throw();
			
			/** Non-mutable inspection of the selection.
			*/
			const List<Composite*>& getSelection() const
				throw();
			
			/** Message handling.
					Overridden method from ConnectionObject to handle incoming messages.\par
					Calls reactToMessages_().\par
					\param  message a pointer to a Message object
			*/
			virtual void onNotify(Message *message)
				throw();

			/** Build a context menu for a Composite and an SelectableListViewItem object.
					If the Composite has certain properties a context menu is created.
					This method only creates the necessary interface for derived classes.
					Override this method for creating other context menu entries, but call 
					Control::buildContextMenu at the end of your method to create the standard entries.
					Calls insertContextMenuEntry() for each context menu entry that will be created.
					\param   composite the Composite object for that a context menu should be created
					\see     onContextMenu
			*/
			virtual void buildContextMenu(Composite& composite)
				throw();

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
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					Reverse all actions performed in initializeWidget
					(remove menu entries of this Control).
					This method will be called by MainControl::aboutToExit.
					\param main_control the MainControl object to be finalized with this Control
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();
			
			/** Apply a given regular expression
			 		@return Size number of selected atoms
			*/
			Size applySelector(const String& expression)
				throw();

		public slots:
				
			//@}
			/** @name Public slots
			*/
			//@{
			
			/// Create a Representation with the DisplayProperties Dialog
			void createRepresentation();

			/// Composite properties dialog requested
			void compositeProperties();
		
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

			/** Invalidate the selection.
					All selected items in the tree will be deselected. \par
					Calls updateSelection().
			*/
			void invalidateSelection();

			///
			void highlightSelection()
				throw();

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

			/** Apply the regular expression from the QLineEdit
			 		@return Size number of selected atoms
			*/
			Size applySelector();

			/// Show a help dialog for the Selector 
			void showSelectorHelp();

			/// Overloaded from GenericControl, calls cut
			virtual void deleteCurrentItems()
				throw();

			/// Connected to the clear selection button
			virtual void clearSelector();
			
			//@} 
			/** @name Protected members 
			*/ 
			//@{
		  protected slots:

			/*_ Controlling method for context menus.
					Clear the previously created context menu.
					Calls buildContextMenu for the Composite object belonging
					to the <tt>item</tt> and executes the context menu if menu entries are available.
					\param  item the SelectableListViewItem for which a context menu should be created
					\param  point the position to which the context menu should be drawn
					\param  column not used at the moment
					\see    buildContextMenu
			*/
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

		  protected:

			/*_ Method is called if checkbox of an item is clicked.\par
			 		Called by SelectableListViewItem::stateChange
			*/
			void selectedComposite_(Composite* composite, bool state);

			/** Set the selection of the checkboxes and the opening of the tree 
			 		according to the selection in the MainControl.
					\param open true means, that the item tree is opend and closed according to the changes
					\param force true means, that the item tree is opend and closed according to the changes,
																	 also if more than 50 items are selected.
			*/
			void setSelection_(bool open, bool force = false)
				throw();

			/** Access the MolecularInformation visitor.
			 		With the MolecularInformation, the names and type entries for the SelectableListViewItem 
					are generated.
					Override this method if another information visitor is needed.
					This method is used in the method generateListViewItem_() to
					retrieve certain information of the given Composite.
			*/
			virtual MolecularInformation& getInformationVisitor_()
				throw();
			
			/** Iterate over the children of the Composite and
					call for each the method generateListViewItem_().
					\param   item a pointer to a SelectableListViewItem to which all children of the Composite 
									 will be inserted
					\param   composite whose children will be inserted into <tt>item</tt>
			*/
			virtual void recurseGeneration_(SelectableListViewItem* item, Composite& composite)
				throw();
			
			/** Iterate over the children of the Composite and
					call for each the method updateListViewItem_().
					\param   item a pointer to a SelectableListViewItem containing the subtree structure 
					\param   composite object containing the (possibly) new substructure
					\see     updateListViewItem_
			*/
			virtual void recurseUpdate_(SelectableListViewItem* item, Composite& composite)
				throw();

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
			virtual bool reactToMessages_(Message* message)
				throw();
			
			/** Mutable inspection of the copy list.
					In this list all copied or cutted Composite objects are stored until
					the method clearClipboard() is called.
			*/
			List<Composite*>& getCopyList_()
				throw();
			
			/** Non-mutable inspection of the copy list.
					For further information see getCopyList_().
			*/
			const List<Composite*>& getCopyList_() const
				throw();

			/** Generate a new SelectableListViewItem for the given Composite.
					If <tt> default_name == 0</tt> than a new name is created using the 
					Information visitor accessed with the method getInformationVisitor_().
					If <tt>parent == 0</tt> than the new SelectableListViewItem is a root item.
					Otherwise a new SelectableListViewItem for is created that will
					be inserted into given <b>parent</b>.
					All children of <b> composite</b> will be inserted recursivly into the newly
					created item by the method recurseGeneration_().
					\param  parent a SelectableListViewItem into which a subtree of items will be inserted, 
											 or <tt> 0</tt> if a new root item should be created
					\param  composite the Composite object whose subtree will be inserted into <b>parent</b>
					\param  default_name the name of the <b> item</b>
			*/
			SelectableListViewItem* generateListViewItem_(SelectableListViewItem* parent, 
																										Composite& composite, QString* default_name = 0)
				throw();
			
			/** Update the item tree recursivly.
					Check if the tree structure of <b>item</b> matches the tree structure of
					<b>composite</b>.
					If an item does not exist a subtree for <b>composite</b>, it will be created with the
					method generateListViewItem_() and inserted into <b>item</b>.
					Otherwise the method recurseUpdate_() will iterate over the childrens	of <b> composite</b>.			
					\param  item a SelectableListViewItem whose subtree of items will be checked against <b>composite</b>
					\param  composite the Composite object whose subtree will be checked against <b>item</b>
			*/
			void updateListViewItem_(SelectableListViewItem* item, Composite& composite)
				throw();

			//_ Test, if its allowed to paste the copy liste into the current selected context item.
			bool pasteAllowedFor_(Composite& composite)
				throw();

			//@} 
			/** @name Menu entries ids
			*/ 
			//@{

			//_
			int cut_id_;
			//_
			int copy_id_;
			//_
			int paste_id_;
			//_
			int delete_id_;
			//_
			int clipboard_id_;
			//_
			int select_id_;
			//_
			int deselect_id_;

			//@}
			
			protected slots:

			//_ called when a model is selected in the context menu
			void activatedItem_(int pos);

			//_
			void createRepresentation_();

		  protected:

			// only for Python Interface
			MolecularControl(const MolecularControl& mc)
				throw();
			
			// ATTRIBUTES
			List<Composite*> 				selected_;
			List<Composite*> 				copy_list_;
			
			MolecularInformation 		information_;
			
			QLineEdit* 							selector_edit_;
			// the context menus
			QPopupMenu 							context_menu_, 
															model_menu_, 
															color_menu_[MODEL_LABEL - MODEL_LINES];

			Composite* 							context_composite_;

			// the dialogs
			SelectableListViewItem* context_item_;

			HashMap<Composite*, SelectableListViewItem*> composite_to_item_;

			ModelType 			selected_model_;
			ColoringMethod  selected_coloring_method_;
			// let cut know to delete the entries, set by deleteCurrentItems()
			bool 						was_delete_;
		};

}} // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
