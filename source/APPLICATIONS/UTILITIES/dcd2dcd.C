// $Id: dcd2dcd.C,v 1.2 2003/01/29 10:21:47 anker Exp $
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
		Log.error() << "Need two filenames, aborting!" << endl;
		return(1);
	}

	SnapShot snapshot;
	DCDFile in_file(argv[1], ios::in | ios::binary);
	DCDFile out_file(argv[2], ios::out | ios::binary);

	for (Size i = 0; i < in_file.getNumberOfSnapShots(); ++i)
	{
		in_file.read(snapshot);
		out_file.append(snapshot);
	}

	in_file.close();
	out_file.close();

}
