// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: control.h,v 1.14 2003/03/03 14:18:30 anhi Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_CONTROL_H
#define BALL_VIEW_GUI_WIDGETS_CONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_CONCEPT_COMPOSITE_H
#	include <BALL/CONCEPT/composite.h>
#endif

#ifndef BALL_VIEW_GUI_WIDGETS_MODULARWIDGET_H
#	include <BALL/VIEW/GUI/WIDGETS/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_GEOMETRICOBJECT_H
#	include <BALL/VIEW/KERNEL/geometricObject.h>
#endif

#ifndef BALL_VIEW_KERNEL_MESSAGE_H
#	include <BALL/VIEW/KERNEL/message.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_INFORMATION_H
#	include <BALL/VIEW/FUNCTOR/information.h>
#endif

#ifndef BALL_VIEW_FUNCTOR_FILTER_H
#	include <BALL/VIEW/FUNCTOR/filter.h>
#endif

#ifndef BALL_VIEW_PRIMITIV_MESH_H
# include <BALL/VIEW/PRIMITIV/mesh.h>
#endif

#ifndef BALL_VIEW_GUI_DIALOGS_COLORMESHDIALOG_H
# include <BALL/VIEW/GUI/DIALOGS/colorMeshDialog.h>
#endif

#ifndef BALL_VIEW_COMMON_GLOBAL_H
# include <BALL/VIEW/COMMON/global.h>
#endif 

#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpopupmenu.h>

namespace BALL
{
	namespace VIEW
	{

		/**	The Control class.
				The Control class is a widget to display the structure of  \link Composite Composite \endlink 
				objects. There are methods available to change the structure as well as
				copy or paste objects into {\em *this} control.
				Various virtual methods can be overridden to customize the behavior of these
				structure changing methods. The method  \link buildContextMenu buildContextMenu \endlink  is a means to 
				create a customizable context menu entries.
				To use this widget in the application just create it with  \link MainControl MainControl \endlink  as
				parent and all inserted  \link Composite Composite \endlink  objects sent with  \link NewCompositeMessage NewCompositeMessage \endlink 
				will be shown in the structure view of {\em *this} control.
				<b>Definition:</b> BALL/VIEW/GUI/WIDGETS/control.h
		*/
		class Control
			: public QListView, 
				public ModularWidget
		{
			Q_OBJECT

			class SelectableListViewItem
				: public QCheckListItem
			{
				public:

				SelectableListViewItem(QListViewItem* parent, const QString& text, const QString& type, Composite* composite, 
															 VIEW::Control& control)
					throw();

				SelectableListViewItem(QListView* parent, const QString& text, const QString& type, Composite* composite, 
															 VIEW::Control& control)
					throw();

				Composite* getComposite() { return composite_;};

				protected:

				// overriden function, used to message to Control
				virtual void stateChange(bool)
					throw();

				Composite* composite_;

				VIEW::Control& control_reference_;

				private: 
				
				// prevent use of default cstr
				SelectableListViewItem();
			};

				
			public:
			
			/** @name Macros.
		  */
			//@{

			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(Control)

			//@}
			/**	@name	Constructors
			*/	
			//@{

			/** Default Constructor.
					Construct new control.
					There are two columns for {\em *this} control. The {\em Name} column and the
					{\em Type} column. In the Name column the item tree will be shown and in 
					the Type column the type of each item will be shown.
					There is a connection from the signal  \link rightButtonClicked rightButtonClicked \endlink  from the
					QT-library to the method  \link onContextMenu onContextMenu \endlink . See documentation of QT-library 
					for information concerning signal/slot mechanism.
					Calls  \link registerWidget registerWidget \endlink .
					@param      parent the parent widget of {\em *this} control (See documentation of QT-library for information concerning widgets)
					@param      name the name of {\em *this} control (See documentation of QT-library for information concerning widgets)
					@return     Control new constructed control
					@see        QListView
					@see        ModularWidget
					@see        onContextMenu
			*/
			Control(QWidget* parent = 0, const char* name = 0)
				throw();
			
			//@}
			/** @name Destructors 
			*/
			//@{

			/** Destructor.
					Default destruction of {\em *this} control.
					Calls  \link destroy destroy \endlink .
					@see         destroy
			*/
			virtual ~Control()
				throw();

			/** Explicit default initialization.
					Empty for further purpose.
			*/
			virtual void clear()
				throw();

			/** Explicit destructor.
					Empty for further purpose.
			*/
			virtual void destroy()
				throw();

			//@}
			/**	@name	Accessors: inspectors and mutators 
			*/
			//@{

			/** Insert a composite.
					Insert a  \link Composite Composite \endlink  object {\em composite} into {\em *this} control.
					If <tt>name == 0</tt> than the  \link Information Information \endlink  visitor is used for determining
					a name for {\em composite}.
					Calls  \link generateListViewItem_ generateListViewItem_ \endlink  for {\em composite}.
					@param   composite a pointer to the  \link Composite Composite \endlink  object that will be inserted into {\em *this} control
					@param   name a name {\em composite} should receive
					@return  bool <tt>true</tt> if <tt>composite != 0</tt>, <tt>false</tt> otherwise
					@see     getInformationVisitor_
					@see     generateListViewItem_
					@see     updateContents
			*/
			bool addComposite(Composite* composite, QString* name = 0)
				throw();

			/** Remove a composite.
					Remove a  \link Composite Composite \endlink  object {\em composite} from {\em *this} control.
					Calls  \link findListViewItem_ findListViewItem_ \endlink  for {\em composite}. If an item exists than
					the item will be removed.
					Calls  \link updateContents updateContents \endlink .
					@param   composite a pointer to the  \link Composite Composite \endlink  object that will be removed from {\em *this} control
					@return  bool <tt>true</tt> if {\em composite} could be removed, <tt>false</tt> otherwise
					@see     findListViewItem_
					@see     updateContents
			*/
			bool removeComposite(Composite* composite)
				throw();
	
			/** Recursive removal of composite from control.
			 		@see removeComposite
			*/
			Size removeRecursiveComposite(Composite* composite)
				throw();

			/** Update a composite.
					Update a  \link Composite Composite \endlink  object {\em composite} to {\em *this} control.
					Calls  \link updateListViewItem_ updateListViewItem_ \endlink  for {\em composite}.
					@param   composite a pointer to the  \link Composite Composite \endlink  object that will be updated
					@return  bool <tt>true</tt> if {\em *this} control must be updated, <tt>false</tt> otherwise
					@see     updateListViewItem_
					@see     updateContents
			*/
			bool updateComposite(Composite* composite)
				throw();
			
			/** Mutable inspection of the selection.
					Access the mutable reference to the selection list of {\em *this} control.
					@return   List<Composite*>& a mutable reference of the selection list of {\em *this} control
					@see      filterSelection_
			*/
			List<Composite*>& getSelection()
				throw();
			
			/** Non-mutable inspection of the selection.
					For further information see  \link getSelection getSelection \endlink .
			*/
			const List<Composite*>& getSelection() const
				throw();
			
			/** Message handling.
					Overridden method from  \link ConnectionObject ConnectionObject \endlink  to handle incoming messages.
					Calls  \link reactToMessages_ reactToMessages_ \endlink .
					Calls  \link sentSelection sentSelection \endlink 
					@param  message a pointer to a  \link Message Message \endlink  object
					@see    sentSelection
					@see    reactToMessages_
					@see    ConnectionObject
					@see    Message
			*/
			virtual void onNotify(Message *message)
				throw();

			/** Build a context menu.
					Build a context menu for the  \link Composite Composite \endlink  object {\em composite} and
					the  \link QListViewItem QListViewItem \endlink  object {\em item}.
					If {\em composite} have certain properties (in this case {\em composite} is
					a  \link GeometricObject GeometricObject \endlink ) a context menu is created.
					Override this method for creating other context menu entries.
					Calls  \link insertContextMenuEntry insertContextMenuEntry \endlink  for each context menu entry that will be
					created.
					@param   composite the  \link Composite Composite \endlink  object for that a context menu should be created
					@param   item the  \link QListViewItem QListViewItem \endlink  object for that a context menu should be created
					@see     insertContextMenuEntry
					@see     onContextMenu
			*/
			virtual void buildContextMenu(Composite* composite, QListViewItem* item)
				throw();

			/** Insert a new context menu entry.
					Create a new context menu entry for {\em *this} control with the given
					parameters.
					See documentation of the QT-library concerning context menu entries and the
					signal/slot mechanism.
					@param name the name of the new menu entry
					@param receiver the object to which the menu action will be connected
					@param slot the function that will be called by activation of the menu entry
					@param accel the acceleration key
					@param entry_ID the id for the new menu entry (default: -1, will create a new one)
					@see   buildContextMenu
			*/
			void insertContextMenuEntry(const String& name, const QObject* receiver = 0, 
																  const char* slot = 0, int entry_ID = -1, int accel = 0)
				throw();

			/**	Initialize the widget.
					Initialize the menus of {\em *this} control:
					\begin{itemize}
					  \item the cut menu
					  \item the copy menu
					  \item the paste menu
					  \item the clipboard menu
					\end{itemize}
					This method is called automatically	immediately before the main application 
					is started. 
					This method will be called by  \link show show \endlink  from the  \link MainControl MainControl \endlink  object.
					@param main_control the  \link MainControl MainControl \endlink  object to be initialized with {\em *this} modularWidget
					@see   cut_id_
					@see   copy_id_
					@see   paste_id_
					@see   clipboard_id_
					@see   finalizeWidget
					@see   checkMenu
					@see   insertMenuEntry
					@see   show
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					Reverse all actions performed in  \link initializeWidget initializeWidget \endlink 
					(remove menu entries of {\em *this} control).
					This method will be called by  \link aboutToExit aboutToExit \endlink  from the  \link MainControl MainControl \endlink 
					object.
					@param main_control the  \link MainControl MainControl \endlink  object to be finalized with {\em *this} control
					@see   cut_id_
					@see   copy_id_
					@see   paste_id_
					@see   clipboard_id_
					@see   initializeWidget
					@see   checkMenu
					@see   removeMenuEntry
					@see   aboutToExit
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();

			/**	Menu checking method.
					This method is called by the method  \link checkMenus checkMenus \endlink  from the
					 \link MainControl MainControl \endlink  object before a popup menu is shown.
					Empty for further purpose.
					@param main_control the  \link MainControl MainControl \endlink  object whose menus should be checked
					@see   cut_id_
					@see   copy_id_
					@see   paste_id_
					@see   clipboard_id_
					@see   initializeWidget
					@see   finalizeWidget
					@see   show
					@see   checkMenus
			*/
			virtual void checkMenu(MainControl& main_control)
				throw();
			

			public slots:
				
			//@}			
			/** @name Public slots
			*/
			//@{

			/** Invalidate the selection.
					Invalidate the selection of {\em *this} control.
					All selected items in the tree will be deselected.
					Calls  \link updateSelection updateSelection \endlink .
					@see  updateSelection
			*/
			void invalidateSelection();

			/** Update the selection.
					Calls  \link filterSelection_ filterSelection_ \endlink  with the  \link Filter Filter \endlink  object and calls
					 \link sentSelection sentSelection \endlink  after the selection is filtered.
					@see   filterSelection_
					@see   sentSelection
			*/
			void updateSelection();
			
			/** Sent the selection.
					Sent the selection obtained with the method  \link getSelection getSelection \endlink  with the
					message  \link GeometricObjectSelectionMessage GeometricObjectSelectionMessage \endlink .
					Override this method if other messages should be sent.
					@see  getSelection
					@see  GeometricObjectSelectionMessage
					@see  notify_
			*/
			virtual void sentSelection();

			/** Cut the selected items.
					Cut the selected items { \link Composite Composite \endlink  objects} and copy them into an internal
					copy list. If there are already  \link Composite Composite \endlink  objects in the internal
					copy list they will be deleted. All  \link Composite Composite \endlink  objects that are cutted
					are removed from {\em *this} control.
					A  \link RemovedCompositeMessage RemovedCompositeMessage \endlink  will be sent for each cutted object.
					If all selected objects are removed a  \link SceneMessage SceneMessage \endlink  will be sent to
					update the  \link Scene Scene \endlink . A  \link WindowMessage WindowMessage \endlink  will be sent to the main
					application for status messages.
					@see    copy
					@see    paste
					@see    clearClipboard
					@see    getSelection
					@see    updateSelection
					@see    WindowMessage
					@see    RemovedCompositeMessage
					@see    SceneMessage
			*/
			void cut();

			/** Copy the selected items.
					Copy the selected items { \link Composite Composite \endlink  objects} into an internal
					copy list. If there are already  \link Composite Composite \endlink  objects in the internal
					copy list they will be deleted. The selected object will not be deleted
					from {\em *this} control.
					A  \link WindowMessage WindowMessage \endlink  will be sent to the main application for status
					messages.
					@see    cut
					@see    paste
					@see    clearClipboard
					@see    getSelection
					@see    WindowMessage
			*/
			void copy();

			/** Paste the copied items.
					Paste the copied items { \link Composite Composite \endlink  objects} from the internal
					copy list into {\em *this} control. For each  \link Compsoite Compsoite \endlink  object in the
					copy list a  \link NewCompositeMessage NewCompositeMessage \endlink  will be sent.
					A  \link WindowMessage WindowMessage \endlink  will be sent to the main application for status
					messages.
					@see    cut
					@see    copy
					@see    clearClipboard
					@see    NewCompositeMessage
					@see    WindowMessage
			*/
			void paste();

			/** Clear the clipboard.
					Clear the copied items { \link Composite Composite \endlink  objects} from the internal
					copy list.
					A  \link WindowMessage WindowMessage \endlink  will be sent to the main application for status
					messages.
					@see    cut
					@see    copy
					@see    paste
					@see    WindowMessage
			*/
			void clearClipboard();

			/** Controlling method for context menus.
					Clear the previously created context menu.
					Calls  \link buildContextMenu buildContextMenu \endlink  for the  \link Composite Composite \endlink  object belonging
					to the {\em item} and executes the context menu if menu entries are available.
					@param  item the  \link QListViewItem QListViewItem \endlink  for which a context menu should be created
					@param  point the position to which the context menu should be drawn
					@param  column not used at the moment
					@see    buildContextMenu
			*/
			void onContextMenu(QListViewItem* item, const QPoint& point, int column);

			/** Erase a geometricObject.
					Erase a  \link GeometricObject GeometricObject \endlink  object previously selected with the context menu.
					If the  \link GeometricObject GeometricObject \endlink  object has a parent it will be removed from it
					and the message  \link ChangedCompositeMessage ChangedCompositeMessage \endlink  will be sent. If the 
					 \link GeometricObject GeometricObject \endlink  object has no parent it will be removed entirely from
					{\em *this} control and the message  \link RemovedCompositeMessage RemovedCompositeMessage \endlink  will be sent.
					The message  \link SceneMessage SceneMessage \endlink  will be sent after the former process to 
					update the  \link Scene Scene \endlink .
					Calls  \link updateContents updateContents \endlink  to update {\em *this} control.
					@see    buildContextMenu
					@see    updateContents
					@see    ChangedCompositeMessage
					@see    RemovedCompositeMessage
					@see    SceneMessage
					@see    GeometricObject
			*/
			void eraseGeometricObject();

			/** Method is called if checkbox of an item is clicked.
			*/
			void selectedComposite(Composite* composite, bool state);

		  signals:
			
		  protected:
			
			//@}
			/** @name Protected members
			*/
			//@{

			/** Set the selection of the checkboxes and the opening of the tree according to the selection
			 		in the MainControl.
					@param open true means, that the item tree is opend and closed according to the changes
			*/
			void setSelection_(bool open)
				throw(MainControlMissing);

			/** Access the information visitor.
					Access the  \link Information Information \endlink  visitor of {\em *this} control.
					Override this method if another information visitor is needed.
					This method is used in the method  \link generateListViewItem_ generateListViewItem_ \endlink  to
					retrieve certain information of the given  \link Composite Composite \endlink  object.
					@return  Information a reference to a  \link Information Information \endlink  visitor.
					@see     Information
					@see     generateListViewItem_
			*/
			virtual Information& getInformationVisitor_()
				throw();
			
			/** Recursive iteration method.
					Iterate over the children of the  \link Composite Composite \endlink  {\em composite} and
					call for each the method  \link generateListViewItem_ generateListViewItem_ \endlink .
					@param   item a pointer to a  \link QListViewItem QListViewItem \endlink  to which all children of {\em composite} will be inserted
					@param   composite a pointer to a  \link Composite Composite \endlink  object whose children will be inserted into {\em item}
					@see     generateListViewItem_
			*/
			virtual void recurseGeneration_(QListViewItem* item, Composite* composite)
				throw();
			
			/** Recursive update method.
					Iterate over the children of the  \link Composite Composite \endlink  {\em composite} and
					call for each the method  \link updateListViewItem_ updateListViewItem_ \endlink .
					@param   item a pointer to a  \link QListViewItem QListViewItem \endlink  containing the subtree structure 
					@param   composite a pointer to a  \link Composite Composite \endlink  object containing the (possibly) new substructure
					@return  bool <tt>true</tt> if the subtree structure of {\em composite} and the subtree 
									 structure of {\em item} are unequal, <tt>false</tt> otherwise 
					@see     updateListViewItem_
			*/
			virtual bool recurseUpdate_(QListViewItem* item, Composite* composite)
				throw();
			
			/** Message handling.
					Catch the  \link Message Message \endlink  objects and react accordingly to the different messages.
					Override this method if new messages should be catched. \par
					<b>Note:</b> If this method is overriden, call this method at the end of the
					new implementation to make sure the old messages are still catched properly. \par
					Messages to be catched and the resulting reaction:
					\begin{itemize}
					  \item   \link NewCompositeMessage NewCompositeMessage \endlink  - update always necessary
					  \item   \link RemovedCompositeMessage RemovedCompositeMessage \endlink  - update necessary if already inserted
					  \item   \link ChangedCompositeMessage ChangedCompositeMessage \endlink  - update necessary if  \link Composite Composite \endlink  object has changed
					\end{itemize}
					@param   message a pointer to the  \link Message Message \endlink  object
					@return  bool <tt>true</tt> if an update of {\em *this} control is necessary, <tt>false</tt> otherwise
					@see     onNotify
			*/
			virtual bool reactToMessages_(Message* message)
				throw();
			
			/** Filter the selected tree items.
					Filter the selected tree items of {\em *this} control and collect the
					 \link Composite Composite \endlink  objects that are both selected in the  \link QListViewItem QListViewItem \endlink  tree
					and pass through the  \link Filter Filter \endlink  object {\em filter}.
					All collected  \link Composite Composite \endlink  object are collected in the selection list.
					@param  filter the  \link Filter Filter \endlink  object to be used for filtering the  \link Composite Composite \endlink  objects
					@see    getSelection
					@see    updateSelection
			*/
			void filterSelection_(Filter& filter)
				throw();

			/** Mutable inspection of the copy list.
					Access the mutable reference of the copy list of {\em *this} control.
					In this list all copied or cutted  \link Composite Composite \endlink  objects are stored until
					the method  \link clearClipboard clearClipboard \endlink  is called.
					@return  List<Composite*>& a mutable reference of the list of the copied  \link Composite Composite \endlink  objects
					@see     cut
					@see     copy
					@see     paste
					@see     clearClipboard
			*/
			List<Composite*>& getCopyList_()
				throw();
			
			/** Non-mutable inspection of the copy list.
					For further information see  \link getCopyList_ getCopyList_ \endlink .
			*/
			const List<Composite*>& getCopyList_() const
				throw();

			/** Generate the item tree recursivly.
					Generate a new  \link QListViewItem QListViewItem \endlink  for the given  \link Composite Composite \endlink  object
					{\em composite} with name {\em default_name} if <tt>default_name != 0</tt>.
					If <tt>default_name == 0</tt> than a new name is created using the 
					 \link Information Information \endlink  visitor accessed with the method 
					 \link getInformationVisitor_ getInformationVisitor_ \endlink .
					If <tt>item == 0</tt> than a new  \link QListViewItem QListViewItem \endlink  for {\em composite} is 
					created that becomes a root item in {\em *this} control.
					Otherwise a new  \link QListViewItem QListViewItem \endlink  for {\em composite} is created that will
					be inserted into given {\em item}.
					All children of {\em composite} will be inserted recursivly into the newly
					created item by the method  \link recurseGeneration_ recurseGeneration_ \endlink .
					@param  item a  \link QListViewItem QListViewItem \endlink  into which a subtree of items will be inserted, 
											 or <tt>0</tt> if a new root item should be created
					@param  composite the  \link Composite Composite \endlink  object whose subtree will be inserted into {\em item}
					@param  default_name the name of the {\em item}
					@see    Information
					@see    getInformationVisitor_
					@see    recurseGeneration_
			*/
			void generateListViewItem_(QListViewItem* item, Composite* composite, QString* default_name = 0)
				throw();
			
			/** Update the item tree recursivly.
					Check if the tree structure of {\em item} matches the tree structure of
					{\em composite}.
					Calls the method  \link findListViewItem_ findListViewItem_ \endlink : If an item
					does not exist a subtree for {\em composite} will be created with the
					method  \link generateListViewItem_ generateListViewItem_ \endlink  and inserted into {\em item}.
					Otherwise the method  \link recurseUpdate_ recurseUpdate_ \endlink  will iterate over the childrens
					of {\em composite}.			
					@param  item a  \link QListViewItem QListViewItem \endlink  whose subtree of items will be checked against {\em composite}
					@param  composite the  \link Composite Composite \endlink  object whose subtree will be checked against {\em item}
					@param  default_name the name of the {\em item}
					@return bool <tt>true</tt> if an update of {\em *this} control is necessary, <tt>false</tt> otherwise
					@see    findListViewItem_
					@see    generateListViewItem_
					@see    recurseUpdate_
			*/
			bool updateListViewItem_(QListViewItem* item, Composite* composite, QString* default_name = 0)
				throw();
			
			/** Search the item tree for a composite.
					Search the  \link QListViewItem QListViewItem \endlink  tree of {\em *this} control for the given
					 \link Composite Composite \endlink  object {\em composite}.
					@param  composite a pointer to the  \link Composite Composite \endlink  object to be search for
					@return QListViewItem* a pointer to the  \link QListViewItem QListViewItem \endlink  containing the  \link Composite Composite \endlink  
									object {\em composite}, <tt>0</tt> if no such  \link Composite Composite \endlink  object exists
					@see    updateListViewItem_
			*/
			QListViewItem* findListViewItem_(Composite* composite)
				throw();

			//@}
			/** @name Protected member variables
			*/
			//@{

			/** Cut id.
					In this variable the menu id for the cut menu is stored.
					This variable is provided for access to the cut menu. With the help of this
					variable the cut menu can be enabled or disabled in the  \link checkMenu checkMenu \endlink  method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int cut_id_;

			/** Copy id.
					In this variable the menu id for the copy menu is stored.
					This variable is provided for access to the copy menu. With the help of this
					variable the copy menu can be enabled or disabled in the  \link checkMenu checkMenu \endlink 
					method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int copy_id_;

			/** Paste id.
					In this variable the menu id for the paste menu is stored.
					This variable is provided for access to the paste menu. With the help of this
					variable the paste menu can be enabled or disabled in the  \link checkMenu checkMenu \endlink 
					method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int paste_id_;

			/** Clipboard id.
					This variable is provided for access to the clipboard menu. With the help of
					this variable the clipboard menu can be enabled or disabled in the 
					 \link checkMenu checkMenu \endlink 	method.
					In this variable the menu id for the clipboard menu is stored.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int clipboard_id_;

			//@}

		  protected:
			
			enum ColumnID
			{
				COLUMN_ID__NAME      = 0,
				COLUMN_ID__TYPE      = 1
			};


			QListViewItem*	getRoot_(QListViewItem* item)
				throw();

			Composite*			getCompositeAddress_(QListViewItem* item)
				throw();

			QString					getRootName_(QListViewItem* item)
				throw();

			QString					getName_(QListViewItem* item)
				throw();

			QString					getTypeName_(QListViewItem* item)
				throw();

			QString					getRootTypeName_(QListViewItem* item)
				throw();

			void selectRecursive_(Composite* composite)
				throw();			
			
			void deselectRecursive_(Composite* composite)
				throw();			
			
			// ATTRIBUTES
			List<Composite*> selected_;
			List<Composite*> copy_list_;
			
			// Information visitor
			Information information_;
			
			// GeometricObject Filter
			Filter geometric_filter_;

			// the context menu
			QPopupMenu context_menu_;

			Composite* context_composite_;
			QListViewItem *context_item_;

			ColorMeshDialog* colorMeshDlg_;

			HashMap<Composite*, QListViewItem*> composite_to_item_;
						
		};
		
		
#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/control.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_CONTROL_H_
