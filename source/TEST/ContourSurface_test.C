// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ContourSurface_test.C,v 1.1 2003/05/02 07:44:14 oliver Exp $

#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/DATATYPE/contourSurface.h>

///////////////////////////

START_TEST(ContourSurface, "$Id: ContourSurface_test.C,v 1.1 2003/05/02 07:44:14 oliver Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

ContourSurface* cs_ptr = 0;
CHECK(ContourSurface::ContourSurface())
	cs_ptr = new ContourSurface;
	TEST_NOT_EQUAL(cs_ptr, 0)
RESULT

CHECK(ContourSurface::~ContourSurface())
	delete cs_ptr;
RESULT

CHECK(TContourSurface<T>::operator << (TRegularData3D<T>& data))
	// Create a grid with just the middle point at 1.0 and the
	// remainder at -1.0.
	RegularData3D data(-2.0, -2.0, -2.0, 2.0, 2.0, 2.0, 5, 5, 5);
	STATUS("grid sie: " << data.getSize())
	for (Position i = 0; i < data.size(); i++)
	{
		data[i] = -1.0;
	}
	data[2 + 2 * 5 + 2 * 25] = 1.0;
	STATUS("Coordinate: " << data.getGridCoordinates(62))
	STATUS("min: " << data.getOrigin())
	STATUS("max: " << data.getOrigin() + data.getDimension())

	STATUS("grid constructed")

	ContourSurface cs;
	STATUS("ContourSurface constructed.")
	
	cs << data;

	TEST_EQUAL(cs.vertex.size(), 6)
	TEST_EQUAL(cs.normal.size(), 6)
	TEST_EQUAL(cs.triangle.size(), 8)
	for (Position i = 0; i < cs.vertex.size(); i++)
	{
		STATUS(i << " " << cs.vertex[i] << " " << cs.normal[i])
	}
	for (Position i = 0; i < cs.triangle.size(); i++)
	{
		STATUS(i << " " << cs.triangle[i].v1 << " " << cs.triangle[i].v2 << " " << cs.triangle[i].v3)
	}
	
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
