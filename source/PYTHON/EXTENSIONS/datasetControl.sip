// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.sip,v 1.3.18.1 2007/03/25 21:37:07 oliver Exp $
//

class DatasetControl
  : DockWidget
{
%TypeHeaderCode
	#include <BALL/VIEW/WIDGETS/datasetControl.h>
	using namespace BALL;
	using namespace BALL::VIEW;
%End
  public:

	static Size countInstances();
	static DatasetControl* getInstance(Position /index/);

	DatasetControl() throw();
  ~DatasetControl() throw();

//		List<std::pair<RegularData3D*, String> > get3DGrids() throw();
//	PyRegularData3DList get3DGrids() throw();

//	virtual void deleteCurrentItems() throw();
};
