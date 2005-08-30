// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: datasetControl.sip,v 1.1.2.1 2005/06/20 10:23:08 amoll Exp $
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

	Size count3DGrids() const throw();

	virtual void deleteCurrentItems() throw();

	void computeIsoContourSurface();

};
