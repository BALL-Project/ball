// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.h,v 1.2 2003/10/05 21:08:58 amoll Exp $

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
#define BALL_VIEW_WIDGETS_DATASETCONTROL_H

#ifndef BALL_VIEW_KERNEL_COMMON_H
# include <BALL/VIEW/KERNEL/common.h>
#endif

#ifndef BALL_MOLMEC_COMMON_SNAPSHOTMANAGER_H
# include <BALL/MOLMEC/COMMON/snapShotManager.h>
#endif

#ifndef BALL_VIEW_WIDGETS_GENERICCONTROL_H
# include <BALL/VIEW/WIDGETS/genericControl.h>
#endif

#ifndef BALL_DATATYPE_REGULARDATA3D_H
# include <BALL/DATATYPE/regularData3D.h>
#endif 

#ifndef BALL_DATATYPE_LIST_H
# include <BALL/DATATYPE/list.h>
#endif 

namespace BALL
{
	class TrajectoryFile;
	class Composite;

	namespace VIEW
	{
		class SnapshotVisualisationDialog;

		/**	DatasetControl is a widget to manipulate Trajectories and RegularData3D instances.
		 		It is derived from GenricControl.
				There are two columns. The <b>Name</b> column and the
				<b>Type</b> column. In the Name column the item tree will be shown and in 
				the Type column the type of each item.
				The method buildContextMenu() can create a customizable context menu.
				\ingroup ViewWidgets
		*/
		class DatasetControl
			: public GenericControl
		{
			Q_OBJECT

			public:

			/** @name Macros.
		  */
			//@{

			/** Embeddable Macro.
			*/
			BALL_EMBEDDABLE(DatasetControl)

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
					\param      parent the parent widget of the DatasetControl 
					\param      name the name of the DatasetControl 
					\see        ModularWidget
			*/
			DatasetControl(QWidget* parent = 0, const char* name = 0)
				throw();
			
			/** Destructor.
			*/
			virtual ~DatasetControl()
				throw();

			//@} 
			/**	@name	Accessors: inspectors and mutators 
			*/ 
			//@{

			virtual void onNotify(Message *message)
				throw();

			/**	Initialize the widget.
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			/**	Remove the widget.
					Reverse all actions performed in initializeWidget
					(remove menu entries of this DatasetControl).
					This method will be called by MainControl::aboutToExit.
			*/
			virtual void finalizeWidget(MainControl& main_control)
				throw();

			///
			virtual void checkMenu(MainControl& main_control)
				throw();
			
			//@}
			/** @name Public slots 
			*/ 
			//@{
			public slots:

			///
			void addTrajectory()
				throw();

			///
			void add3DGrid()
				throw();

			///
			void updateSelection()
				throw();

			List<std::pair<RegularData3D*, String> > getGrids()
				throw();

			//@} 
			/** @name Protected members 
			*/ 
			//@{
		  protected slots:

			void deleteTrajectory_();
			void deleteGrid_();
			void visualiseTrajectory_();
			void saveTrajectory_();
			void save3DGrid_()
				throw();
	
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

			//@}

		  protected:

			void insertTrajectory_(TrajectoryFile* file, System& system)
				throw();
			
			void insertGrid_(RegularData3D* file, System& system, const String& name)
				throw();
			
			void insertComposite_(Composite* composite, QListViewItem* item)
				throw();

			void deleteItem_(QListViewItem* item)
				throw();

			SnapShotManager* 							context_trajectory_;
			QListViewItem* 								context_item_;

			SnapshotVisualisationDialog* 	dialog_;
			Index 												open_trajectory_id_;
			Index 												open_grid_id_;

			HashMap<QListViewItem*	, SnapShotManager*> 					item_to_trajectory_;
			HashMap<QListViewItem*	, RegularData3D*>   					item_to_grid_;
			HashMap<Composite*      , HashSet<QListViewItem*> > 	composite_to_items_;
			HashMap<QListViewItem*  , Composite*>  								item_to_composite_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_DATASETCONTROL_H
