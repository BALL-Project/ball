// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: bounding_box.C,v 1.2 2002/02/27 12:25:05 sturm Exp $
// molview tutorial example
// ------------------------
// create a processor that computes the bounding box of a molecular structure

// standard includes
#include <iostream>

// BALL includes
#include <BALL/common.h>
#include <BALL/FORMAT/HINFile.h>

// include our new processor
#include "glBoundingBoxModel.h"

using namespace BALL;
using namespace BALL::MOLVIEW;
using namespace std;

int main()
{
	// read a molecule from a file
	HINFile infile("bounding_box_example.hin");
	System system;
	infile >> system;
	infile.close();

	// apply the bounding box processor
	GLBoundingBoxModel bb_processor;
	system.apply(bb_processor);

	// TODO
	// generate some output
	cout << "Bounding Box: " << endl;
}

