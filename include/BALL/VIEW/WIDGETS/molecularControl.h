// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: molecularControl.h,v 1.7 2003/11/05 23:27:26 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
#define BALL_VIEW_WIDGETS_MOLECULARCONTROL_H

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
#	include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#ifndef BALL_VIEW_KERNEL_MOLECULARINFORMATION_H
#	include <BALL/VIEW/KERNEL/molecularInformation.h>
#endif

#include <qlistview.h>
#include <qpoint.h>
#include <qpopupmenu.h>


namespace BALL
{
	namespace VIEW
	{
		class TransformationDialog;

		/**	MolecularControl is a widget to display the molecular structure of Composite objects. 
				This class is derived from the class Control and extends it for showing and modifiying
				molecular structures. The methods checkMenu() and buildContextMenu() are overridden 
				for performing special molecular tasks.
				\ingroup ViewWidgets
		*/
		class MolecularControl
			: public GenericControl
		{			
			enum MolecularMenuEntries
			{
				CREATE_REPRESENTATION = 1,
				OBJECT__REMOVE      	= 10,
				OBJECT__CUT         	= 11,
				OBJECT__COPY        	= 12,
				OBJECT__PASTE       	= 13,
				OBJECT__MOVE        	= 14,
				SELECT              	= 26,
				DESELECT            	= 27,

				CAMERA__CENTER      	= 110,
				BONDS__BUILD        	= 120,
				BONDS__REMOVE       	= 121,
				RESIDUE__CHECK      	= 140,
				DISPLAY__CHANGE     	= 150,
				ATOM__PROPERTIES			= 160,
				SHOW__FILENAME 				= 170,
				COLLAPSE_ALL 					= 180
			};

			Q_OBJECT

			public:
			
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

				protected:

				// overriden function, used to message to Control
				virtual void stateChange(bool)
					throw();

				Composite* composite_;

				MolecularControl& control_reference_;

				private: 
				
				// prevent use of default cstr
				SelectableListViewItem();
			};

			friend class SelectableListViewItem;

			BALL_EMBEDDABLE(MolecularControl)
			
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

			/** Remove a Composite from this Control.\par
					Calls findListViewItem_() for composite.\par
					Calls updateContents.
					\param   composite a pointer to the Composite to be removed from the Control
			*/
			void removeComposite(Composite& composite)
				throw();
	
			/** Recursive removal of a Composite from the Control.
			 		\see removeComposite
			*/
			Size removeRecursiveComposite(Composite& composite)
				throw();

			/** Access the mutable reference to the selection list of this control.
					\return   List<Composite*>& a mutable reference of the selection list 
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
					Calls sendSelection()
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

			/** Create a new context menu entry with the given parameters.
					See documentation of the QT-library concerning context menu entries and the
					signal/slot mechanism.
					\param name the name of the new menu entry
					\param receiver the object to which the menu action will be connected
					\param slot the function that will be called by activation of the menu entry
					\param accel the acceleration key
					\param entry_ID the id for the new menu entry (default: -1, will create a new one)
					\see   buildContextMenu
			*/
			void insertContextMenuEntry(const String& name, const QObject* receiver = 0, 
																  const char* slot = 0, int entry_ID = -1, int accel = 0)
				throw();

			/**	Initialize the widget.
					Initialize the menus of this control:
					  - the cut menu
					  - the copy menu
					  - the paste menu
					  - the clipboard menu
					\par
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by MainControl::show.
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
			

		public slots:
				
			//@}
			/** @name Public slots
			*/
			//@{
			
			/// Create a Representation with the DisplayProperties Dialog
			void createRepresentation();

			/// Atom properties dialog requested
			void atomProperties();

			/** Check residues
			 */
			void checkResidue();

			/** Build bonds
			 */
			void buildBonds();
			
			/** Center the camera on a composite.
			 */
			void centerCamera();

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
			void move();
			
			/// Collapse all entries in the control
			void collapseAll();

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
			*/
			void setSelection_(bool open)
				throw();

			/** Access the MolecularInformation visitor of this Control.
			 		With the MolecularInformation, the names and type entries for the SelectableListViewItem 
					are generated.
					Override this method if another information visitor is needed.
					This method is used in the method generateListViewItem_() to
					retrieve certain information of the given Composite.
					\return  MolecularInformation a reference to a information visitor.
			*/
			virtual MolecularInformation& getInformationVisitor_()
				throw();
			
			/** Iterate over the children of the Composite and
					call for each the method generateListViewItem_().
					\param   item a pointer to a SelectableListViewItem to which all children of the Composite 
									 will be inserted
					\param   composite a pointer to a Composite whose children will be inserted into <tt>item</tt>
			*/
			virtual void recurseGeneration_(SelectableListViewItem* item, Composite& composite)
				throw();
			
			/** Iterate over the children of the Composite and
					call for each the method updateListViewItem_().
					\param   item a pointer to a SelectableListViewItem containing the subtree structure 
					\param   composite a pointer to a Composite object containing the (possibly) new substructure
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
					Access the mutable reference of the copy list of the Control.
					In this list all copied or cutted Composite objects are stored until
					the method clearClipboard() is called.
					\return  List<Composite*>& a mutable reference of the list of the copied Composite objects
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
					Check if the tree structure of <b> item</b> matches the tree structure of
					<b> composite</b>.
					If an item does not exist a subtree for <b> composite</b>, it will be created with the
					method generateListViewItem_() and inserted into <b>item</b>.
					Otherwise the method recurseUpdate_() will iterate over the childrens	of <b> composite</b>.			
					\param  item a SelectableListViewItem whose subtree of items will be checked against <b>composite
					\param  composite the Composite object whose subtree will be checked against <b>item
					\param  default_name the name of the <b>item</b>
			*/
			void updateListViewItem_(SelectableListViewItem* item, Composite& composite)
				throw();
			
			//@} 
			/** @name Protected member variables 
			*/ 
			//@{

			/** Cut id.
					In this variable the menu id for the cut menu is stored.
					This variable is provided for access to the cut menu. With the help of this
					variable the cut menu can be enabled or disabled in the checkMenu().
			*/
			int cut_id_;

			/** Copy id.
					In this variable the menu id for the copy menu is stored.
					This variable is provided for access to the copy menu. With the help of this
					variable the copy menu can be enabled or disabled in checkMenu().
			*/
			int copy_id_;

			/** Paste id.
					In this variable the menu id for the paste menu is stored.
					This variable is provided for access to the paste menu. With the help of this
					variable the paste menu can be enabled or disabled in checkMenu().
			*/
			int paste_id_;

			/** Clipboard id.
					This variable is provided for access to the clipboard menu. With the help of
					this variable the clipboard menu can be enabled or disabled in the 
					checkMenu	method.
					In this variable the menu id for the clipboard menu is stored.
			*/
			int clipboard_id_;

			//@}

		  protected:
			
			// ATTRIBUTES
			List<Composite*> 				selected_;
			List<Composite*> 				copy_list_;
			
			MolecularInformation 		information_;
			
			// the context menu
			QPopupMenu 							context_menu_;

			Composite* 							context_composite_;
			SelectableListViewItem* context_item_;
			TransformationDialog* 	transformation_dialog_;

			HashMap<Composite*, SelectableListViewItem*> composite_to_item_;

			//@}
		};

}} // namespaces

#endif // BALL_VIEW_WIDGETS_MOLECULARCONTROL_H
