// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.h,v 1.10 2004/02/10 15:51:35 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
#define BALL_VIEW_WIDGETS_GENERICCONTROL_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <qlistview.h>

namespace BALL
{
	namespace VIEW
	{
		/** Overloaded QListView, which notifies it parent GenericControl,
		 		that a entry has to be deleted, after "del" was pressed.
		*/
		class MyListView
			: public QListView
		{
			public:

				///
				MyListView(QWidget* parent)
					: QListView(parent) {}

				///
 				void keyPressEvent(QKeyEvent * e);
		};
		
		
		/**	GenericControl is a widget to display the structure of Composite objects. 
		 		It uses the datastructure QListView from the QT-libary.
				There are two columns. The <b>Name</b> column and the
				<b>Type</b> column. In the Name column the item tree will be shown and in 
				the Type column the type of each item.
		    There are methods available to change the structure as well as
				copy or paste objects into the GenericControl.
				Various virtual methods can be overridden to customize the behavior of these
				structure changing methods. 
				The method buildContextMenu() can create a customizable context menu.
				To use this widget in the application just create it with MainGenericControl as
				parent.
				\ingroup ViewWidgets
		*/
		class GenericControl
			: public DockWidget
		{
			Q_OBJECT

			public:

			/// typedef
			typedef List<QListViewItem*> ItemList;

			/** @name Macros.
		  */
			//@{

			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(GenericControl,DockWidget)

			//@}
			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
					There is a connection from the signal rightButtonClicked from the
					QT-library to the method onContextMenu(). 
					(See documentation of QT-library for information concerning widgets and 
					signal/slot mechanism.) \par
					Calls registerWidget().
					\param      parent the parent widget of the GenericControl 
					\param      name the name of the GenericControl 
					\see        ModularWidget
			*/
			GenericControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~GenericControl()
				throw();

			///
			ItemList getSelectedItems()
				throw();

			/** React to a DeselectControlsMessage.
			 		If such a message is send from other GenericControls, this GenericControl 
					deselects all its items, so that only one GenericControl has a Selection 
					at any time.
			 		Call this Method in the derived Classes in their onNotify().
			*/
			virtual void onNotify(Message *message)
				throw();

			//@}

			public slots:
			
			/// Called by MyListView, if del is pressed
			virtual void deleteCurrentItems()
				throw() {};
			
		  protected slots:
			
			virtual void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

			virtual void deselectOtherControls_();

			/*_ Call deselectOtherControls_ if a selection exists.
			 		Call this Method in the derived Classes in their updateSelection()
			*/
			virtual void updateSelection();
			
		  protected:

			QListViewItem* 								context_item_;
			MyListView* 										listview;
		};
		
} } // namespaces
#endif // BALL_VIEW_WIDGETS_GENERICCONTROL_H
