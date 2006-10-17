// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: compareForces.C,v 1.1.2.1 2006/10/17 23:07:03 amoll Exp $
//
// test program for the MMFF94 implementation

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/forEach.h>

#include <math.h>

using namespace std;
using namespace BALL;

String dir;

Size wrong_types = 0;
File* out = 0;

float diff(double original, double our)
{
	double x = original - our;
	x = fabs(x);
	return x / fabs(original);
}

int main(int argc, char** argv)
{
	vector<Vector3> charmm_forces, ball_forces;

	vector<String> names;

	LineBasedFile clf("charmm.out");
	while(clf.readLine())
	{
		vector<String> fields;
		if (clf.getLine().split(fields) != 10)
		{
			Log.error() << "Not enough fields!" << std::endl;
			return 0;
		}

		Vector3 v(fields[4].toFloat(),
							fields[5].toFloat(),
							fields[6].toFloat());

		charmm_forces.push_back(-v);
	}

	LineBasedFile blf("BALL.out");
	while(blf.readLine())
	{
		vector<String> fields;
		if (blf.getLine().split(fields) != 4)
		{
			Log.error() << "Not enough fields!" << std::endl;
			return 0;
		}

		Vector3 v(fields[1].toFloat(),
							fields[2].toFloat(),
							fields[3].toFloat());

		names.push_back(fields[0]);

		ball_forces.push_back(v);
	}

	if (charmm_forces.size() != ball_forces.size())
	{
		Log.error() << "Different number of atoms!" << std::endl;
		return 0;
	}

	float max_diff = 0.01;
	for (Position p = 0; p < charmm_forces.size(); p++)
	{
		if (diff(charmm_forces[p].x, ball_forces[p].x) > max_diff ||
		    diff(charmm_forces[p].y, ball_forces[p].y) > max_diff ||
		    diff(charmm_forces[p].z, ball_forces[p].z) > max_diff)
		{
			Log.error() << names[p] << ": " << std::endl 
				          << charmm_forces[p] << std::endl
				          << ball_forces[p] << std::endl;
		}
	}

	return 0;
}
