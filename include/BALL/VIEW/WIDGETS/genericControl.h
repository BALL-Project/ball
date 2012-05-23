// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: genericControl.h,v 1.14.16.1 2007/03/25 21:26:20 oliver Exp $

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
#define BALL_VIEW_WIDGETS_GENERICCONTROL_H

#ifndef BALL_VIEW_WIDGETS_DOCKWIDGET_H
# include <BALL/VIEW/WIDGETS/dockWidget.h>
#endif

#include <QtGui/QKeyEvent>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QTreeWidgetItem>
#include <QtCore/QList>

namespace BALL
{
	namespace VIEW
	{
		/** Base class for all control widgets.
				\ingroup ViewWidgets
		*/
		class TreeWidget
			: public QTreeWidget
		{
			public:

			///
			TreeWidget(QWidget* parent = 0);

			///
			void selectItems(const list<QTreeWidgetItem*>& items);
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
				To use this widget in the application just create it with MainGenericControl as
				parent.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT GenericControl
			: public DockWidget
		{
			Q_OBJECT

			public:

			/// typedef
 			typedef QList<QTreeWidgetItem*> ItemList;

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
					(See documentation of QT-library for information concerning widgets and 
					signal/slot mechanism.) \par
					Calls registerWidget().
					\param      parent the parent widget of the GenericControl 
					\param      name the name of the GenericControl 
					\see        ModularWidget
			*/
			GenericControl(QWidget* parent = 0, const char* name = 0);
			
			/** Destructor.
			*/
			virtual ~GenericControl();

 			ItemList getSelectedItems();

			///
			QTreeWidgetItem* addRow(const QStringList& entries);

			/** React to a DeselectControlsMessage.
			 		If such a message is send from other GenericControls, this GenericControl 
					deselects all its items, so that only one GenericControl has a Selection 
					at any time.
			 		Call this Method in the derived Classes in their onNotify().
			*/
 			virtual void onNotify(Message *message);

			/**	Initialize the menu entries:
					  - delete
					\par
					This method is called automatically	immediately before the main application 
					is started by MainControl::show.
					\param main_control the MainControl object to be initialized with this ModularWidget
			*/
 			virtual void initializeWidget(MainControl& main_control);

			//@}

			public slots:
			
			/// Called by if del is pressed
 			virtual void deleteCurrentItems(){};

		  protected slots:

 			virtual void deselectOtherControls_();

			/*_ Call deselectOtherControls_ if a selection exists.
			 		Call this Method in the derived Classes in their updateSelection()
			*/
 			virtual void updateSelection();

			virtual void onItemClicked(QTreeWidgetItem*, int) {};
			
		  protected:

 			virtual void removeItem_(QTreeWidgetItem* item);

 			QTreeWidgetItem* 			context_item_;
			TreeWidget* 					listview;
			bool 									checkable_;
		};
		
} } // namespaces
#endif // BALL_VIEW_WIDGETS_GENERICCONTROL_H
