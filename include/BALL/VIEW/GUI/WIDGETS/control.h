// $Id: control.h,v 1.8 2001/07/09 19:13:08 sturm Exp $

#ifndef BALL_VIEW_GUI_WIDGETS_CONTROL_H
#define BALL_VIEW_GUI_WIDGETS_CONTROL_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <iostream>

#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpopupmenu.h>


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

namespace BALL
{

	namespace VIEW
	{

		/**	The Control class.
				{\bf Framework:} BALL/VIEW/GUI/WIDGETS\\
				{\bf Definition:} \URL{BALL/VIEW/GUI/WIDGETS/control.h}\\ \\
				The Control class is a widget to display the structure of \Ref{Composite}
				objects. There are methods available to change the structure as well as
				copy or paste objects into {\em *this} control.
				Various virtual methods can be overridden to customize the behavior of these
				structure changing methods. The method \Ref{buildContextMenu} is a means to 
				create a customizable context menu entries.
				To use this widget in the application just create it with \Ref{MainControl} as
				parent and all inserted \Ref{Composite} objects sent with \Ref{NewCompositeMessage}
				will be shown in the structure view of {\em *this} control.
				@memo    Control class (BALL VIEW gui widgets framework)
				@author  $Author: sturm $
				@version $Revision: 1.8 $
				@date    $Date: 2001/07/09 19:13:08 $
		*/
		class Control: public QListView, public ModularWidget
		{
			
			Q_OBJECT
				
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
					There is a connection from the signal \Ref{rightButtonClicked} from the
					QT-library to the method \Ref{onContextMenu}. See documentation of QT-library 
					for information concerning signal/slot mechanism.
					Calls \Ref{registerWidget}.
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
					Calls \Ref{destroy}.
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
					Insert a \Ref{Composite} object {\em composite} into {\em *this} control.
					If {\tt name == 0} than the \Ref{Information} visitor is used for determining
					a name for {\em composite}.
					Calls \Ref{generateListViewItem_} for {\em composite}.
					@param   composite a pointer to the \Ref{Composite} object that will be inserted into {\em *this} control
					@param   name a name {\em composite} should receive
					@return  bool {\tt true} if {\tt composite != 0}, {\tt false} otherwise
					@see     getInformationVisitor_
					@see     generateListViewItem_
					@see     updateContents
			*/
			bool addComposite(Composite* composite, QString* name = 0)
				throw();

			/** Remove a composite.
					Remove a \Ref{Composite} object {\em composite} from {\em *this} control.
					Calls \Ref{findListViewItem_} for {\em composite}. If an item exists than
					the item will be removed.
					Calls \Ref{updateContents}.
					@param   composite a pointer to the \Ref{Composite} object that will be removed from {\em *this} control
					@return  bool {\tt true} if {\em composite} could be removed, {\tt false} otherwise
					@see     findListViewItem_
					@see     updateContents
			*/
			bool removeComposite(Composite* composite)
				throw();

			/** Update a composite.
					Update a \Ref{Composite} object {\em composite} to {\em *this} control.
					Calls \Ref{updateListViewItem_} for {\em composite}.
					@param   composite a pointer to the \Ref{Composite} object that will be updated
					@return  bool {\tt true} if {\em *this} control must be updated, {\tt false} otherwise
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
					For further information see \Ref{getSelection}.
			*/
			const List<Composite*>& getSelection() const
				throw();
			
			/** Message handling.
					Overridden method from \Ref{ConnectionObject} to handle incoming messages.
					Calls \Ref{reactToMessages_}.
					Calls \Ref{sentSelection}
					@param  message a pointer to a \Ref{Message} object
					@see    sentSelection
					@see    reactToMessages_
					@see    ConnectionObject
					@see    Message
			*/
			virtual void onNotify(Message *message)
				throw();

			/** Build a context menu.
					Build a context menu for the \Ref{Composite} object {\em composite} and
					the \Ref{QListViewItem} object {\em item}.
					If {\em composite} have certain properties (in this case {\em composite} is
					a \Ref{GeometricObject}) a context menu is created.
					Override this method for creating other context menu entries.
					Calls \Ref{insertContextMenuEntry} for each context menu entry that will be
					created.
					@param   composite the \Ref{Composite} object for that a context menu should be created
					@param   item the \Ref{QListViewItem} object for that a context menu should be created
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
			void insertContextMenuEntry
				(const String& name, const QObject* receiver = 0, 
				 const char* slot = 0, int accel = 0, int entry_ID = -1)
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
					This method will be called by \Ref{show} from the \Ref{MainControl} object.
					@param main_control the \Ref{MainControl} object to be initialized with {\em *this} modularWidget
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
					Reverse all actions performed in \Ref{initializeWidget}
					(remove menu entries of {\em *this} control).
					This method will be called by \Ref{aboutToExit} from the \Ref{MainControl}
					object.
					@param main_control the \Ref{MainControl} object to be finalized with {\em *this} control
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
					This method is called by the method \Ref{checkMenus} from the
					\Ref{MainControl} object before a popup menu is shown.
					Empty for further purpose.
					@param main_control the \Ref{MainControl} object whose menus should be checked
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
			//@}			
			

			public slots:
				
			/** @name Public slots
			*/
			//@{

			/** Invalidate the selection.
					Invalidate the selection of {\em *this} control.
					Alle selected items in the tree will be deselected.
					Calls \Ref{updateSelection}.
					@see  updateSelection
			*/
			void invalidateSelection();

			/** Update the selection.
					Calls \Ref{filterSelection_} with the \Ref{Filter} object and calls
					\Ref{sentSelection} after the selection is filtered.
					@see   filterSelection_
					@see   sentSelection
			*/
			void updateSelection();
			
			/** Sent the selection.
					Sent the selection obtained with the method \Ref{getSelection} with the
					message \Ref{GeometricObjectSelectionMessage}.
					Override this method if other messages should be sent.
					@see  getSelection
					@see  GeometricObjectSelectionMessage
					@see  notify_
			*/
			virtual void sentSelection();

			/** Cut the selected items.
					Cut the selected items {\Ref{Composite} objects} and copy them into an internal
					copy list. If there are already \Ref{Composite} objects in the internal
					copy list they will be deleted. All \Ref{Composite} objects that are cutted
					are removed from {\em *this} control.
					A \Ref{RemovedCompositeMessage} will be sent for each cutted object.
					If all selected objects are removed a \Ref{SceneMessage} will be sent to
					update the \Ref{Scene}. A \Ref{WindowMessage} will be sent to the main
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
					Copy the selected items {\Ref{Composite} objects} into an internal
					copy list. If there are already \Ref{Composite} objects in the internal
					copy list they will be deleted. The selected object will not be deleted
					from {\em *this} control.
					A \Ref{WindowMessage} will be sent to the main application for status
					messages.
					@see    cut
					@see    paste
					@see    clearClipboard
					@see    getSelection
					@see    WindowMessage
			*/
			void copy();

			/** Paste the copied items.
					Paste the copied items {\Ref{Composite} objects} from the internal
					copy list into {\em *this} control. For each \Ref{Compsoite} object in the
					copy list a \Ref{NewCompositeMessage} will be sent.
					A \Ref{WindowMessage} will be sent to the main application for status
					messages.
					@see    cut
					@see    copy
					@see    clearClipboard
					@see    NewCompositeMessage
					@see    WindowMessage
			*/
			void paste();

			/** Clear the clipboard.
					Clear the copied items {\Ref{Composite} objects} from the internal
					copy list.
					A \Ref{WindowMessage} will be sent to the main application for status
					messages.
					@see    cut
					@see    copy
					@see    paste
					@see    WindowMessage
			*/
			void clearClipboard();

			/** Controlling method for context menus.
					Clear the previously created context menu.
					Calls \Ref{buildContextMenu} for the \Ref{Composite} object belonging
					to the {\em item} and executes the context menu if menu entries are
					available.
					@param  item the \Ref{QListViewItem} for which a context menu should be created
					@param  point the position to which the context menu should be drawn
					@param  column not used at the moment
					@see    buildContextMenu
			*/
			void onContextMenu(QListViewItem* item,  const QPoint& point, int column);

			/** Erase a geometricObject.
					Erase a \Ref{GeometricObject} object previously selected with the context
					menu.
					If the \Ref{GeometricObject} object has a parent it will be removed from it
					and the message \Ref{ChangedCompositeMessage} will be sent. If the 
					\Ref{GeometricObject} object has no parent it will be removed entirely from
					{\em *this} control and the message \Ref{RemovedCompositeMessage} will be
					sent.
					The message \Ref{SceneMessage} will be sent after the former process to 
					update the \Ref{Scene}.
					Calls \Ref{updateContents} to update {\em *this} control.
					@see    buildContextMenu
					@see    updateContents
					@see    ChangedCompositeMessage
					@see    RemovedCompositeMessage
					@see    SceneMessage
					@see    GeometricObject
			*/
			void eraseGeometricObject();
			//@}
		

		  signals:
			
			
		  protected:
			
			/** @name Protected members
			*/
			//@{
			/** Access the information visitor.
					Access the \Ref{Information} visitor of {\em *this} control.
					Override this method if another information visitor is needed.
					This method is used in the method \Ref{generateListViewItem_} to
					retrieve certain information of the given \Ref{Composite} object.
					@return  Information a reference to a \Ref{Information} visitor.
					@see     Information
					@see     generateListViewItem_
			*/
			virtual Information& getInformationVisitor_()
				throw();
			
			/** Recursive iteration method.
					Iterate over the children of the \Ref{Composite} {\em composite} and
					call for each the method \Ref{generateListViewItem_}.
					@param   item a pointer to a \Ref{QListViewItem} to which all children of {\em composite} will be inserted
					@param   composite a pointer to a \Ref{Composite} object whose children will be inserted into {\em item}
					@see     generateListViewItem_
			*/
			virtual void recurseGeneration_(QListViewItem* item, Composite* composite)
				throw();
			
			/** Recursive update method.
					Iterate over the children of the \Ref{Composite} {\em composite} and
					call for each the method \Ref{updateListViewItem_}.
					@param   item a pointer to a \Ref{QListViewItem} containing the subtree structure 
					@param   composite a pointer to a \Ref{Composite} object containing the (possibly) new substructure
					@return  bool {\tt true} if the subtree structure of {\em composite} and the subtree structure of {\em item} are unequal, {\tt false} otherwise 
					@see     updateListViewItem_
			*/
			virtual bool recurseUpdate_(QListViewItem* item, Composite* composite)
				throw();
			
			/** Message handling.
					Catch the \Ref{Message} objects and react accordingly to the different
					messages.
					Override this method if new messages should be catched.\\
					{\bf Note:} If this method is overriden, call this method at the end of the
					new implementation to make sure the old messages are still catched properly.\\
					Messages to be catched and the resulting reaction:
					\begin{itemize}
					  \item  \Ref{NewCompositeMessage} - update always necessary
					  \item  \Ref{RemovedCompositeMessage} - update necessary if already inserted
					  \item  \Ref{ChangedCompositeMessage} - update necessary if \Ref{Composite} object has changed
					\end{itemize}
					@param   message a pointer to the \Ref{Message} object
					@return  bool {\tt true} if an update of {\em *this} control is necessary, {\tt false} otherwise
					@see     onNotify
			*/
			virtual bool reactToMessages_(Message* message)
				throw();
			
			/** Filter the selected tree items.
					Filter the selected tree items of {\em *this} control and collect the
					\Ref{Composite} objects that are both selected in the \Ref{QListViewItem} tree
					and pass through the \Ref{Filter} object {\em filter}.
					All collected \Ref{Composite} object are collected in the selection list.
					@param  filter the \Ref{Filter} object to be used for filtering the \Ref{Composite} objects
					@see    getSelection
					@see    updateSelection
			*/
			void filterSelection_(Filter& filter)
				throw();

			/** Mutable inspection of the copy list.
					Access the mutable reference of the copy list of {\em *this} control.
					In this list all copied or cutted \Ref{Composite} objects are stored until
					the method \Ref{clearClipboard} is called.
					@return  List<Composite*>& a mutable reference of the list of the copied \Ref{Composite} objects
					@see     cut
					@see     copy
					@see     paste
					@see     clearClipboard
			*/
			List<Composite*>& getCopyList_()
				throw();
			
			/** Non-mutable inspection of the copy list.
					Access the constant reference of the copy list of {\em *this} control.
					In this list all copied or cutted \Ref{Composite} objects are stored until
					the method \Ref{clearClipboard} is called.
					@return  List<Composite*>& a constant reference of the list of the copied \Ref{Composite} objects
					@see     cut
					@see     copy
					@see     paste
					@see     clearClipboard
			*/
			const List<Composite*>& getCopyList_() const
				throw();

			/** Generate the item tree recursivly.
					Generate a new \Ref{QListViewItem} for the given \Ref{Composite} object
					{\em composite} with name {\em default_name} if {\tt default_name != 0}.
					If {\tt default_name == 0} than a new name is created using the 
					\Ref{Information} visitor accessed with the method 
					\Ref{getInformationVisitor_}.
					If {\tt item == 0} than a new \Ref{QListViewItem} for {\em composite} is 
					created that becomes a root item in {\em *this} control.
					Otherwise a new \Ref{QListViewItem} for {\em composite} is created that will
					be inserted into given {\em item}.
					All children of {\em composite} will be inserted recursivly into the newly
					created item by the method \Ref{recurseGeneration_}.
					@param  item a \Ref{QListViewItem} into which a subtree of items will be inserted, or {\tt 0} if a new root item should be created
					@param  composite the \Ref{Composite} object whose subtree will be inserted into {\em item}
					@param  default_name the name of the {\em item}
					@see    Information
					@see    getInformationVisitor_
					@see    recurseGeneration_
			*/
			void generateListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0)
				throw();
			
			/** Update the item tree recursivly.
					Check if the tree structure of {\em item} matches the tree structure of
					{\em composite}.
					Calls the method \Ref{findListViewItem_}: If an item
					does not exist a subtree for {\em composite} will be created with the
					method \Ref{generateListViewItem_} and inserted into {\em item}.
					Otherwise the method \Ref{recurseUpdate_} will iterate over the childrens
					of {\em composite}.			
					@param  item a \Ref{QListViewItem} whose subtree of items will be checked against {\em composite}
					@param  composite the \Ref{Composite} object whose subtree will be checked against {\em item}
					@param  default_name the name of the {\em item}
					@return bool {\tt true} if an update of {\em *this} control is necessary, {\tt false} otherwise
					@see    findListViewItem_
					@see    generateListViewItem_
					@see    recurseUpdate_
			*/
			bool updateListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0)
				throw();
			
			/** Search the item tree for a composite.
					Search the \Ref{QListViewItem} tree of {\em *this} control for the given
					\Ref{Composite} object {\em composite}.
					@param  composite a pointer to the \Ref{Composite} object to be search for
					@return QListViewItem* a pointer to the \Ref{QListViewItem} containing the \Ref{Composite} object {\em composite}, {\tt 0} if no such \Ref{Composite} object exists
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
					variable the cut menu can be enabled or disabled in the \Ref{checkMenu} method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int cut_id_;

			/** Copy id.
					In this variable the menu id for the copy menu is stored.
					This variable is provided for access to the copy menu. With the help of this
					variable the copy menu can be enabled or disabled in the \Ref{checkMenu}
					method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int copy_id_;

			/** Paste id.
					In this variable the menu id for the paste menu is stored.
					This variable is provided for access to the paste menu. With the help of this
					variable the paste menu can be enabled or disabled in the \Ref{checkMenu}
					method.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int paste_id_;

			/** Clipboard id.
					This variable is provided for access to the clipboard menu. With the help of
					this variable the clipboard menu can be enabled or disabled in the 
					\Ref{checkMenu}	method.
					In this variable the menu id for the clipboard menu is stored.
					@see   initializeWidget
					@see   finalizeWidget
					@see   checkMenu
			*/
			int clipboard_id_;
			//@}
			

		  private:
			
			enum ColumnID
			{
				COLUMN_ID__NAME      = 0,
				COLUMN_ID__TYPE      = 1,
				COLUMN_ID__ADDRESS   = 6
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
		};
		
		
#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/control.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_CONTROL_H_
