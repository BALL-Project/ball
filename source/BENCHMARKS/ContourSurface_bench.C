// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ContourSurface_bench.C,v 1.2 2003/05/06 20:20:20 oliver Exp $
//
#include <BALL/CONCEPT/benchmark.h>

///////////////////////////

#include <BALL/DATATYPE/contourSurface.h>

///////////////////////////

using namespace BALL;

START_BENCHMARK(ContourSurface, 1.0, "$Id: ContourSurface_bench.C,v 1.2 2003/05/06 20:20:20 oliver Exp $")


/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////


START_SECTION(Ocatahedra, 0.2)
	ContourSurface cs;
	RegularData3D grid(RegularData3D::IndexType(5, 5, 5), Vector3(-2.0), Vector3(2.0));
	for (Position i = 0; i < grid.size(); grid[i++] = 1.0);
	grid[2 + 2 * 5 + 2 * 25] = -1.0;
	for (Position i = 0; i < 5000; i++)
	{
		START_TIMER
		cs << grid;
		STOP_TIMER
	}
END_SECTION

START_SECTION(Large Grid, 0.8)
	ContourSurface cs2(5.0);
	RegularData3D grid2(RegularData3D::IndexType(200, 200, 200), Vector3(-10.0), Vector3(10.0));
	for (Position i = 0; i < grid2.size(); i++)
	{
		grid2[i] = grid2.getCoordinates(i).getLength();
	}
	for (Position i = 0; i < 10; i++)
	{
		START_TIMER
		cs2 << grid2;
		STOP_TIMER
	}
END_SECTION

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

END_BENCHMARK
