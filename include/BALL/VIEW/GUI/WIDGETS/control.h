// $Id: control.h,v 1.4 2000/10/22 15:26:18 hekl Exp $

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

using namespace BALL;
using namespace BALL::VIEW;

namespace BALL
{

	namespace VIEW
	{

		/**
		 */
		class Control
			: public QListView,
			public ModularWidget
		{
			
			Q_OBJECT
				
			public:
			
			BALL_EMBEDDABLE(Control)
				
			/**	@name	Enums
			 */
			//@{
			enum ColumnID
			{
				COLUMN_ID__NAME      = 0,
				COLUMN_ID__TYPE      = 1,
				COLUMN_ID__ADDRESS   = 6
			};
			//@}
			
			/**	@name	Type Definitions
			 */
			
			/**	@name	Constructors and Destructors
			 */
			//@{
			
			// constructs a widget 
			Control(QWidget* parent = 0, const char* name = 0);
			
			// virtual destructor
			virtual ~Control();
			//@}
			
			// --- ACCESSORS: INSPECTORS and MUTATORS
			
			// adds a composite to the control, using the given name as identifier
			// returns true, if tree changed		
			bool addComposite(Composite* composite, QString* name = 0);
			
			// removes the composite from the tree
			// returns true, if tree changed		
			bool removeComposite(Composite* composite);
			
			// updates the tree structure belonging to the composite
			// returns true, if tree changed		
			bool updateComposite(Composite* composite);
			
			// get selected composites from the tree
			List<Composite*>& getSelection();
			
			// get selected composites from the tree
			const List<Composite*>& getSelection() const;
			
			// processes messages
			virtual void onNotify(Message *message);
			
			// ModularWidget stuff
			virtual void initializeWidget(MainControl& main_control);
			virtual void finalizeWidget(MainControl& main_control);
			virtual void checkMenu(MainControl& main_control);
			
			public slots:
				
			// clear all selected listviewitems in the tree
			void invalidateSelection();
			
			// calculates a new selection and calls 'sentSelection'
			void updateSelection();
			
			// override for distinguishing selected composites
			virtual void sentSelection();
			
			void cut();
			void copy();
			void paste();
			void clearClipboard();

			
		  signals:
			
			
		  protected:
			
			// returns the actual Information visitor
			virtual Information& getInformationVisitor_();
			
			// recursive iteration methode for "generateListViewItem_"
			// item is the item belonging to the composite parameter
			// all subtrees of composite will be inserted to item
			virtual void recurseGeneration_(QListViewItem* item, Composite* composite);
			
			// recursive iteration methode for "updateListViewItem_"
			// item is the item belonging to the composite parameter
			// all subtrees of composite will be updated below the item
			// return true, if subtree is changed
			virtual bool recurseUpdate_(QListViewItem* item, Composite* composite);
			
			// override this function to catch different messages and 
			// act accordingly 
			// return true, if update of control necessay
			virtual bool reactToMessages_(Message* message);
			
			
			// filters the given selection list according to the filter object
			// changes the contents of the selection list
			void filterSelection_(Filter& filter, bool refill_list = false);

			// get copied composites
			List<Composite*>& getCopyList_();
			
			// get copied composites
			const List<Composite*>& getCopyList_() const;

			
			// function to generate the tree recursivly
			void generateListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0);
			
			// function to update the tree recursivly
			// return true, if tree is changed
			bool updateListViewItem_
				(QListViewItem* item, Composite* composite, QString* default_name = 0);
			
			// finds the corresponding listviewitem to the given composite
			QListViewItem* findListViewItem_(Composite* composite);
			
			// insert all selected composites into the selection list
			void fillSelectionList_();

			// MenuId's
			int cut_id_;
			int copy_id_;
			int paste_id_;
			int clipboard_id_;
			

		  private:
			
			QListViewItem*	getRoot_(QListViewItem* item);
			Composite*			getCompositeAddress_(QListViewItem* item);
			QString					getRootName_(QListViewItem* item);
			QString					getName_(QListViewItem* item);
			QString					getTypeName_(QListViewItem* item);
			QString					getRootTypeName_(QListViewItem* item);
			
			
			// ATTRIBUTES
			List<Composite*> selected_;
			List<Composite*> copy_list_;
			
			bool selection_changed_;
			
			// Information visitor
			Information information_;
			
			// GeometricObject Filter
			Filter geometric_filter_;
		};
		
		
#		ifndef BALL_NO_INLINE_FUNCTIONS
#			include <BALL/VIEW/GUI/WIDGETS/control.iC>
#		endif

	} // namespace VIEW

} // namespace BALL

#endif // BALL_VIEW_GUI_WIDGETS_CONTROL_H_
