// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: trajectoryControl.h,v 1.1 2003/09/01 22:29:07 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_TrajectoryControl_H
#define BALL_VIEW_WIDGETS_TrajectoryControl_H

#ifndef BALL_VIEW_KERNEL_MODULARWIDGET_H
#	include <BALL/VIEW/KERNEL/modularWidget.h>
#endif

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H
# include <BALL/MOLMEC/COMMON/snapShotManager.h>
#endif

#include <qlistview.h>
#include <qmessagebox.h>
#include <qpoint.h>
#include <qpopupmenu.h>

namespace BALL
{
	class Composite;

	namespace VIEW
	{
		class SnapshotVisualisationDialog;

		/**	TrajectoryControl is a widget to display the structure of Composite objects. 
		 		It uses the datastructure QListView from the QT-libary.
				There are two columns. The <b>Name</b> column and the
				<b>Type</b> column. In the Name column the item tree will be shown and in 
				the Type column the type of each item.
		    There are methods available to change the structure as well as
				copy or paste objects into the TrajectoryControl.
				Various virtual methods can be overridden to customize the behavior of these
				structure changing methods. 
				The method buildContextMenu() can create a customizable context menu.
				To use this widget in the application just create it with MainTrajectoryControl as
				parent and all inserted Composite objects sent with NewCompositeMessage
				will be shown in the structure view of the TrajectoryControl.
		*/
		class TrajectoryControl
			: public QListView, 
				public ModularWidget
		{
			Q_OBJECT

			public:

			/** @name Macros.
		  */
			//@{

			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(TrajectoryControl)

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
					\param      parent the parent widget of the TrajectoryControl 
					\param      name the name of the TrajectoryControl 
					\see        ModularWidget
			*/
			TrajectoryControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~TrajectoryControl()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			virtual void onNotify(Message *message)
				throw();

			/** Create a new context menu entry with the given parameters.
			*/
			void insertContextMenuEntry(const String& name, const QObject* receiver = 0, 
																  const char* slot = 0, int entry_ID = -1, int accel = 0)
				throw();

			/**	Initialize the widget.
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					Reverse all actions performed in initializeWidget
					(remove menu entries of this TrajectoryControl).
					This method will be called by MainTrajectoryControl::aboutToExit.
					\param main_TrajectoryControl the MainTrajectoryControl object to be finalized with this TrajectoryControl
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();

			virtual void checkMenu(MainControl& main_control)
				throw();
			
			///
			virtual void fetchPreferences(INIFile& inifile)
				throw();
			
			///
			virtual void writePreferences(INIFile& inifile)
				throw();

			//@}
			/** @name Public slots 
			*/ 
			//@{
			public slots:

			/** Invalidate the selection.
					All selected items in the tree will be deselected. \par
					Calls updateSelection().
			*/
			void invalidateSelection();

			/** Update the selection.
			 		Stores the selected items from the QListView to a list and sends the selection.
					\see   sendSelection
			*/
//			virtual void updateSelection();

			void addTrajectory()
				throw();

			/// Show or hide widget (Called by menu entry in "WINDOWS")
			void switchShowWidget()
				throw();

			//@} 
			/** @name Protected members 
			*/ 
			//@{
		  protected slots:

			void deleteTrajectory_();
			void visualiseTrajectory_();
			void saveTrajectory_();
	
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

		  protected:

			//@}

		  protected:
			
			// the context menu
			QPopupMenu 										context_menu_;
			SnapShotManager* 							context_trajectory_;
			QListViewItem* 								context_item_;

			SnapshotVisualisationDialog* 	dialog_;
			Index 												visualise_id_;

			HashMap<QListViewItem*	, SnapShotManager*> item_to_trajectory_;
		};
		
	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_WIDGETS_TRAJECTORYCONTROL_H
