// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.h,v 1.11.6.5 2005/09/01 22:18:04 amoll Exp $
//

#ifndef BALL_VIEW_WIDGETS_DATASETCONTROL_H
#define BALL_VIEW_WIDGETS_DATASETCONTROL_H

#ifndef BALL_KERNEL_SYSTEM_H
# include <BALL/KERNEL/system.h>
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

#include <qpopupmenu.h>

namespace BALL
{
	class TrajectoryFile;
	class SnapShotManager;

	namespace VIEW
	{
		class SnapshotVisualisationDialog;
		class ContourSurfaceDialog;

		/**	DatasetControl is a widget to manipulate Trajectories and RegularData instances.
		 		To add further datatypes, derive from this class and add further HashMaps for types
				and overload deleteItem_(), createContextMenu_() and checkMenu().
				The DatasetControl has three columns: Name, from (Composite) and Type.
				\ingroup ViewWidgets
		*/
		class BALL_VIEW_EXPORT DatasetControl
			: public GenericControl
		{
			Q_OBJECT

			public:

			BALL_EMBEDDABLE(DatasetControl,GenericControl)

			/**	@name	Constructors and Destructors
			*/	
			//@{

			/** Default Constructor.
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

			/// Overload this method to react to further messages
			virtual void onNotify(Message *message)
				throw();

			/**	Initialize the widget.
			*/
			virtual void initializeWidget(MainControl& main_control)
				throw();

			///
			virtual void checkMenu(MainControl& main_control)
				throw();

			/// insert a trajectory for the currently selected System
			void addTrajectory(const String& filename);
			
			///
			List<std::pair<RegularData3D*, String> > get3DGrids()
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
			void updateSelection() throw();

			/// Overloaded from GenericControl, calls cut
			virtual void deleteCurrentItems() throw() {deleteItems_();}

			///
			void computeIsoContourSurface();

			//@} 
			/** @name Protected members 
			*/ 
			//@{
		  protected slots:

			// overload this method to add furter data types
			virtual bool deleteItem_(QListViewItem& item);

			void deleteItems_();
			void visualiseTrajectory_();
			void bufferTrajectory_();
			void saveTrajectory_();
			void visualiseGrid_();
			void saveGrid_() throw();
			String chooseGridFileForSave_() throw();
			String chooseGridFileForOpen_() throw();
	
			void onContextMenu_(QListViewItem* item, const QPoint& point, int column);

			//@}

		  protected:
			
			// only for Python Interface
			DatasetControl(const DatasetControl& control) throw();

			// overload this method to add further types of data to the context menu.
			// Use context_item_ and the Hashmaps to differ between the different types.
			virtual void createContextMenu_();

			void insertTrajectory_(TrajectoryFile* file, System& system)
				throw();

			void insertGrid_(RegularData1D* file, System* system, const String& name)
				throw();

			void insertGrid_(RegularData2D* file, System* system, const String& name)
				throw();
		
			void insertGrid_(RegularData3D* file, System* system, const String& name)
				throw();

			QListViewItem* createListViewItem_(System* system, const String& name, const String& type)
				throw();
			
			void insertComposite_(Composite* composite, QListViewItem* item)
				throw();

			void insertContextMenuEntry_(const QString & text, const char* member);

			QPopupMenu 							 			context_menu_;
			QListViewItem* 								context_item_;

			SnapshotVisualisationDialog* 	dialog_;
			ContourSurfaceDialog* 				surface_dialog_;

			HashMap<QListViewItem*	, SnapShotManager*> 					item_to_trajectory_;
			HashMap<QListViewItem*	, RegularData1D*>   					item_to_grid1_;
			HashMap<QListViewItem*	, RegularData2D*>   					item_to_grid2_;
			HashMap<QListViewItem*	, RegularData3D*>   					item_to_grid3_;
			// insert new HashMaps like above for new data type objects.
			
			HashMap<Composite*      , HashSet<QListViewItem*> > 	composite_to_items_;
			HashMap<QListViewItem*  , Composite*>  								item_to_composite_;

			Index menu_cs_, open_trajectory_id_;
		};
		
} } // namespaces

#endif // BALL_VIEW_WIDGETS_DATASETCONTROL_H
