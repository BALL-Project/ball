// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.h,v 1.10 2004/09/01 14:14:11 amoll Exp $
//

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

#ifndef BALL_DATATYPE_REGULARDATA1D_H
# include <BALL/DATATYPE/regularData1D.h>
#endif 

#ifndef BALL_DATATYPE_REGULARDATA2D_H
# include <BALL/DATATYPE/regularData2D.h>
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

		/**	DatasetControl is a widget to manipulate Trajectories and RegularData instances.
		 		It is derived from GenricControl.
				There are two columns. The <b>Name</b> column and the
				<b>Type</b> column. In the Name column the item tree will be shown and in 
				the Type column the type of each item.
				The method buildContextMenu() can create a customizable context menu.
				\ingroup ViewWidgets
		*/
		class BALL_EXPORT DatasetControl
			: public GenericControl
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(DatasetControl,GenericControl)

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
			void add1DGrid() throw();


			///
			void add2DGrid() throw();


			///
			void add3DGrid() throw();

			///
			void updateSelection()
				throw();

			///
			List<std::pair<RegularData3D*, String> > get3DGrids()
				throw();

			///
			Size count3DGrids() const
				throw() { return item_to_grid3_.size();}

			/// Overloaded from GenericControl, calls cut
			virtual void deleteCurrentItems()
				throw() {deleteItems_();}

			//@} 
			/** @name Protected members 
			*/ 
			//@{
		  protected slots:

			void deleteItem_(QListViewItem& item);
			void deleteItems_();
			void visualiseTrajectory_();
			void bufferTrajectory_();
			void saveTrajectory_();
			void visualiseGrid_();
			void save1DGrid_() throw();
			void save2DGrid_() throw();
			void save3DGrid_() throw();
			String chooseGridFileForSave_() throw();
			String chooseGridFileForOpen_() throw();
	
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

			//@}

		  protected:

			void insertTrajectory_(TrajectoryFile* file, System& system)
				throw();

			void insertGrid_(RegularData1D* file, System* system, const String& name)
				throw();

			void insertGrid_(RegularData2D* file, System* system, const String& name)
				throw();
		
			void insertGrid_(RegularData3D* file, System* system, const String& name)
				throw();
			
			void insertComposite_(Composite* composite, QListViewItem* item)
				throw();

			SnapShotManager* 							context_trajectory_;
			QListViewItem* 								context_item_;

			SnapshotVisualisationDialog* 	dialog_;
			Index 												open_trajectory_id_;

			HashMap<QListViewItem*	, SnapShotManager*> 					item_to_trajectory_;
			HashMap<QListViewItem*	, RegularData1D*>   					item_to_grid1_;
			HashMap<QListViewItem*	, RegularData2D*>   					item_to_grid2_;
			HashMap<QListViewItem*	, RegularData3D*>   					item_to_grid3_;
			HashMap<Composite*      , HashSet<QListViewItem*> > 	composite_to_items_;
			HashMap<QListViewItem*  , Composite*>  								item_to_composite_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_DATASETCONTROL_H
