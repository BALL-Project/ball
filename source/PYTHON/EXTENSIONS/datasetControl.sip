// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.sip,v 1.3.6.1 2006/02/14 15:02:37 amoll Exp $
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

	void addTrajectory(const String&);

	void addTrajectory() throw();

	void add1DGrid() throw();

	void add2DGrid() throw();

	void add3DGrid() throw();

	void updateSelection() throw();

//		List<std::pair<RegularData3D*, String> > get3DGrids() throw();

	virtual void deleteCurrentItems() throw();

	void computeIsoContourSurface();

};
