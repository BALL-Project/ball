// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class RegularData3DController 
	: DatasetController
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/standardDatasets.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
	public:
	static Size countInstances();
	static RegularData3DController* getInstance(Position);

	RegularData3DController();
	RegularData3DController(RegularData3DController& );
	virtual ~RegularData3DController() throw();
	virtual bool write(Dataset*, String /*filetype*/, String /*filename*/);
	virtual Dataset* open(String /*filetype*/, String /*filename*/);
	//virtual bool createMenuEntries();
	//virtual QMenu* buildContextMenu(QTreeWidgetItem* item);
	bool computeIsoContourSurface(Dataset&, const ColorRGBA&, float);
	//Size getNextPowerOfTwo_(Size) const;
	bool isGridSizePowerOfTwo(const RegularData3D&) const;
	RegularData3D* resizeGrid(const RegularData3D&);
	RegularData3D* createHistogramGrid(const RegularData3D&);
	RegularData3D* getData(Dataset*);

	public slots:
	bool createVectorGrid();
	void computeIsoContourSurface();
	void resizeGrid();
	void createHistogramGrid();
	void visualizeGrid();
	void createSphere();
};


class TrajectoryController
	: DatasetController
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/standardDatasets.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	static Size countInstances();
	static TrajectoryController* getInstance(Position);

	TrajectoryController();
	TrajectoryController(TrajectoryController& );
	virtual ~TrajectoryController() throw();
	virtual bool write(Dataset*, String /*filetype*/, String /*filename*/);
	virtual Dataset* open(String /*filetype*/, String /*filename*/);
	//virtual bool createMenuEntries();
	//virtual QMenu* buildContextMenu(QTreeWidgetItem* item);
	SnapShotManager* getData(Dataset*);

	public slots:
	bool visualizeTrajectory();
	bool bufferTrajectory();
};


/*
class DockResultController
	: DatasetController
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/standardDatasets.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	static Size countInstances();
	static DockResultController* getInstance(Position);

	DockResultController();
	DockResultController(DockResultController& );
	virtual ~DockResultController() throw();
	virtual bool write(Dataset*, String, String);
	virtual Dataset* open(String, String);
	//virtual bool createMenuEntries();
	//virtual QMenu* buildContextMenu(QTreeWidgetItem* item);
	DockResult* getData(Dataset*);

	public slots:
	void showDockResult();
	bool saveDockTrajectory();
};


class VectorGridController 
	: DatasetController
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/standardDatasets.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End

	public:
	static Size countInstances();
	static VectorGridController* getInstance(Position);

	VectorGridController();
	VectorGridController(VectorGridController& );
	virtual ~VectorGridController() throw();
	virtual bool write(Dataset*, String, String);
	virtual Dataset* open(String, String);
	//virtual bool createMenuEntries();
	//virtual QMenu* buildContextMenu(QTreeWidgetItem* item);
	VectorGrid* getData(Dataset*);

	public slots:
	bool visualizeFieldLines();
};
*/
