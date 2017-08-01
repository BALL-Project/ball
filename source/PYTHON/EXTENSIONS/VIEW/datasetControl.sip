class DatasetControl
	: DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/datasetControl.h>
	#include <BALL/PYTHON/EXTENSIONS/pyDatasetVector.h>
%End
	public:

	static Size countInstances();
	static DatasetControl* getInstance(Position);

	DatasetControl() throw();
	~DatasetControl() throw();

	Size getSelectionSize();
	PyDatasetVector getDatasets(const String& type);
	DatasetController* getController(const String&);

	private:
	DatasetControl(const DatasetControl&);
};
