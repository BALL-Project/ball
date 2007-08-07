// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: dcd2dcd.C,v 1.3.28.1 2007/08/07 18:26:18 oliver Exp $
//
// a little helper program converting unreadable CHARMm DCD files to
// simpler but usable X-PLOR DCD files.

#include <BALL/common.h>
#include <BALL/FORMAT/DCDFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if (argc != 3)
	{
		Log.info() << "Usage: " << argv[0] << " <infile.dcd> <outfile.dcd>." << endl;
		return(1);
	}

	SnapShot snapshot;
	DCDFile in_file(argv[1], ios::in | ios::binary);
	DCDFile out_file(argv[2], ios::out | ios::binary);

	Log.info() << "Converting the snapshots...." << endl;

	for (Size i = 0; i < in_file.getNumberOfSnapShots(); ++i)
	{
		in_file.read(snapshot);
		out_file.append(snapshot);
	}

	in_file.close();
	out_file.close();
	Log.info() << "  done." << endl; 
}
