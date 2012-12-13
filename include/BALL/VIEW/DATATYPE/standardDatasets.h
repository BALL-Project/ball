// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_VIEW_DATATYPE_STANDARDDATASETS_H
#define BALL_VIEW_DATATYPE_STANDARDDATASETS_H

#ifndef BALL_VIEW_DATATYPE_DATASET_H
# include <BALL/VIEW/DATATYPE/dataset.h>
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

#ifndef BALL_VIEW_DATATYPE_RAYTRACEABLEGRID_H
# include <BALL/VIEW/DATATYPE/raytraceableGrid.h>
#endif

#ifndef BALL_VIEW_DATATYPE_RAYTRACEABLECONTOURSURFACEDIALOG_H
# include <BALL/VIEW/DIALOGS/raytraceableContourSurfaceDialog.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <boost/shared_ptr.hpp>

namespace BALL
{
	class TrajectoryFile;
	class SnapShotManager;
	class DockResult;

	namespace VIEW
	{
		class ContourSurfaceDialog;
		class SnapshotVisualisationDialog;
		class FieldLinesDialog;
		class GridVisualizationDialog;
		class ColorRGBA;


BALL_CREATE_DATASET(RegularData3D)

/** Controller base class for one type of Dataset
*/
class BALL_VIEW_EXPORT RegularData3DController
	: public DatasetController
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(RegularData3DController,DatasetController)

	///
	RegularData3DController();

	///
	RegularData3DController(RegularData3DController& dc);

	///
	virtual ~RegularData3DController();

	///
	virtual bool write();

	///
	virtual bool write(Dataset* set, String filetype, String filename);

	///
	virtual Dataset* open(String filetype, String filename);

	///
	virtual bool createMenuEntries();

	///
	virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

	///
	bool computeIsoContourSurface(Dataset& data, const ColorRGBA& color, float value);

	///
	Size getNextPowerOfTwo_(Size in) const;
	
	///
	bool isGridSizePowerOfTwo(const RegularData3D& grid) const;
	
	///
	RegularData3D* resizeGrid(const RegularData3D& grid);

	///
	RegularData3D* createHistogramGrid(const RegularData3D& grid);

	///
	RegularData3D* getData(Dataset* set);

	static String type;

	public slots:

	///
	bool createVectorGrid();

	///
	bool createRaytraceableGrid();

	///
	void computeIsoContourSurface();
	
	///
	void resizeGrid();

	///
	void createHistogramGrid();

	///
	void visualizeGrid();

	///
	void createSphere();

	protected:

	void deleteDataset_(Dataset* set);

	ContourSurfaceDialog* 		dialog_;
	GridVisualizationDialog*  grid_dialog_;
};


BALL_CREATE_DATASET(SnapShotManager)

/** Controller base class for one type of Dataset
*/
class BALL_VIEW_EXPORT TrajectoryController
	: public DatasetController
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(TrajectoryController,DatasetController)

	///
	TrajectoryController();

	///
	TrajectoryController(TrajectoryController& dc);

	///
	virtual ~TrajectoryController();

	///
	virtual bool write(Dataset* set, String filetype, String filename);

	///
	virtual Dataset* open(String filetype, String filename);

	///
	virtual bool createMenuEntries();

	///
	virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

	///
	SnapShotManager* getData(Dataset* set);

	static String type;

	public slots:

	///
	bool visualizeTrajectory();

	///
	bool bufferTrajectory();

	protected:
	void deleteDataset_(Dataset* set);

	HashMap<SnapShotManager*, boost::shared_ptr<SnapshotVisualisationDialog> > dialogs_per_ssm_;
};

/////////////////////////////////////////////////////////////////////////////

BALL_CREATE_DATASET(DockResult)

/** Controller base class for one type of Dataset
*/
class BALL_VIEW_EXPORT DockResultController
	: public DatasetController
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(DockResultController,DatasetController)

	///
	DockResultController();

	///
	DockResultController(DockResultController& dc);

	///
	virtual ~DockResultController();

	///
	virtual bool write(Dataset* set, String filetype, String filename);

	///
	virtual Dataset* open(String filetype, String filename);

	///
	virtual bool createMenuEntries();

	///
	virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

	///
	DockResult* getData(Dataset* set);

	static String type;

	public slots:

	///
	void showDockResult();

	///
	bool saveDockTrajectory();

	protected:

	void deleteDataset_(Dataset* set);
};

/////////////////////////////////////////////////////////////////////////////

typedef TRegularData3D<Vector3> VectorGrid;

BALL_CREATE_DATASET(VectorGrid)

/** Controller class for VectorGrid
*/
class BALL_VIEW_EXPORT VectorGridController
	: public DatasetController
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(VectorGridController,DatasetController)

	///
	VectorGridController();

	///
	VectorGridController(VectorGridController& dc);

	///
	virtual ~VectorGridController();

	///
	virtual bool write(Dataset* set, String filetype, String filename);

	///
	virtual Dataset* open(String filetype, String filename);

	///
	virtual bool createMenuEntries();

	///
	virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

	///
	VectorGrid* getData(Dataset* set);

	static String type;

	public slots:

	///
	bool visualizeFieldLines();
	
	protected:

	void deleteDataset_(Dataset* set);

	FieldLinesDialog* 	dialog_;
};

/////////////////////////////////////////////////////////////////////////////

BALL_CREATE_DATASET(RaytraceableGrid)

/** Controller class for RaytraceableGrid
 */
class BALL_VIEW_EXPORT RaytraceableGridController
	: public DatasetController
{
	Q_OBJECT

	public:

	BALL_EMBEDDABLE(RaytraceableGridController, DatasetController)

	///
	RaytraceableGridController();

	///
	RaytraceableGridController(RaytraceableGridController& rc);

	///
	virtual ~RaytraceableGridController();

	///
	virtual bool createMenuEntries();

	///
	virtual bool write(Dataset* set, String filetype, String filename);

	///
	virtual QMenu* buildContextMenu(QTreeWidgetItem* item);

	///
	RaytraceableGrid* getData(Dataset* set);

	static String type;

	public slots:
	
	///
	void  visualizeRaytraceableContourSurface();


	protected:

	void deleteDataset_(Dataset* set);
	RaytraceableContourSurfaceDialog* rt_contour_surface_dialog_;
};

	} // namespace VIEW
} // namespace BALL

#endif // BALL_VIEW_DATATYPE_STANDARDDATASETS_H
