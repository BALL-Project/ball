// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A very simple utility for combining several pdb snapshots of a system
// into one dcd file

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/MOLMEC/COMMON/snapShot.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

	if (argc == 1)
	{
		Log.info() << "Usage:" << argv[0] << " <PDB infile>* " << endl;
		Log.info() << "Converts all given PDBFiles into snapshots of a DCDFile." << endl;
		return 1;
	}

	String filename;
	SnapShot snapshot;
	DCDFile dcd_file("out.dcd", ios::out | ios::binary);

	for (int i = 1; i < argc; ++i)
	{
		filename = argv[i];
		Log.info() << "Reading file " << filename;
		PDBFile file(filename);
		if (file.bad())
		{
			Log.error() << "cannot read PDB file " << argv[i] << endl;
			return 2;
		}

		System system;
		file >> system;
		file.close();
		Size res_count = system.countResidues();
		Log.info() << " containing " << res_count << " residues." << endl;

		snapshot.takeSnapShot(system);
		dcd_file.append(snapshot);
	}

	// dcd_file.flushToDisk();
	dcd_file.close();
	Log.info() << "Wrote all snapshots into out.dcd." << endl;
}
