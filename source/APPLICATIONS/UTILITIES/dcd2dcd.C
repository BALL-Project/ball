#include <BALL/FORMAT/DCDFile.h>

using namespace BALL;
using namespace std;

int main(int argc, char** argv)
{

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
