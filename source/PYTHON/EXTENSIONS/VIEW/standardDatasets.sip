class RegularData3DController
	: DatasetController
{
%TypeHeaderCode
	#include <BALL/VIEW/DATATYPE/standardDatasets.h>
%End
	public:
	static Size countInstances();
	static RegularData3DController* getInstance(Position);

	RegularData3DController();
	RegularData3DController(RegularData3DController& );
	virtual ~RegularData3DController() throw();
	virtual bool write(Dataset*, String /*filetype*/, String /*filename*/);
	virtual Dataset* open(String /*filetype*/, String /*filename*/);
	bool computeIsoContourSurface(Dataset&, const ColorRGBA&, float);
	bool isGridSizePowerOfTwo(const RegularData3D&) const;
	RegularData3D* resizeGrid(const RegularData3D&);
	RegularData3D* createHistogramGrid(const RegularData3D&);
	RegularData3D* getData(Dataset*);

	public Q_SLOTS:
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
%End
	public:
	static Size countInstances();
	static TrajectoryController* getInstance(Position);
	TrajectoryController();
	TrajectoryController(TrajectoryController& );
	virtual ~TrajectoryController() throw();
	virtual bool write(Dataset*, String /*filetype*/, String /*filename*/);
	virtual Dataset* open(String /*filetype*/, String /*filename*/);
	SnapShotManager* getData(Dataset*);

	public Q_SLOTS:
	bool visualizeTrajectory();
	bool bufferTrajectory();
};
