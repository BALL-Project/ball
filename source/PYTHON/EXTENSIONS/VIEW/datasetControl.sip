// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class DatasetControl
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/datasetControl.h>
	#include <BALL/PYTHON/pyDatasetVector.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static DatasetControl* getInstance(Position);

	DatasetControl() throw();
  ~DatasetControl() throw();

//		List<std::pair<RegularData3D*, String> > get3DGrids() throw();
//	PyRegularData3DList get3DGrids() throw();

//	virtual void deleteCurrentItems() throw();
	Size getSelectionSize();
	PyDatasetVector getDatasets(const String& type);
	DatasetController* getController(const String&);
	private:
		DatasetControl(const DatasetControl&);
};
