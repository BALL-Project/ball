// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: readMMFF94TestFile.C,v 1.1.2.12 2005/03/28 12:39:11 amoll Exp $
//
// A small program for adding hydrogens to a PDB file (which usually comes
// without hydrogen information) and minimizing all hydrogens by means of a
// conjugate gradient minimizer.

#include <BALL/common.h>

#include <BALL/KERNEL/system.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/fileSystem.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/FORMAT/MOL2File.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Bend.h>


using namespace std;
using namespace BALL;

String dir;

System* readTestFile(String filename)
{
	MOL2File mol2_file(filename);
	System* system = new System;
	mol2_file >> *system;

	filename = filename.getSubstring(0, filename.size() - 4);
	filename += "atoms";

	vector<String> atoms, names, symbols;
	vector<float> charges, fcharges;
	vector<short> types;
	vector<String> fields;

	HashMap<String, Position> name_to_pos;
	Position pos = 0;
	
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		if (infile.getLine().split(fields) != 6)
		{
			Log.error() << "Error in " << filename << " Not 6 fields in one line " << infile.getLine() << std::endl;
			return 0;
		}

		atoms.push_back(fields[0]);
		types.push_back(fields[2].toUnsignedShort());
		charges.push_back(fields[4].toFloat());
		fcharges.push_back(fields[5].toFloat());

		name_to_pos[fields[0]] = pos;
		pos ++;
	}

	infile.close();

	AtomIterator ait;
	BALL_FOREACH_ATOM(*system, ait)
	{
		if (!name_to_pos.has(ait->getName()))
		{
			Log.error() << "We have no data for the atom " << ait->getName() << " in file " << filename << std::endl;
			continue;
		}

		Position pos = name_to_pos[ait->getName()];
		ait->setType(types[pos]);
		ait->setCharge(charges[pos]);
	}

	return system;
}


vector<float> getResults(String filename)
{
	filename += ".results";

	vector<float> results;
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		vector<String> fields;
		infile.getLine().split(fields);
		results.push_back(fields[0].toFloat());
	}

	return results;
}

bool testStretch(MMFF94& mmff, const String& filename)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".stretch");
	LineBasedFile infile(full_file_name);
	vector<String> atoms1, atoms2;
	vector<bool>   is_sbmb;
	vector<float>  r0s, kbs;
	while (infile.readLine())
	{
		vector<String> fields;
		infile.getLine().split(fields);
		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		is_sbmb.push_back(fields[2].toUnsignedShort());
		r0s.push_back(fields[3].toFloat());
		kbs.push_back(fields[4].toFloat());
	}

	MMFF94Stretch* stretch = (MMFF94Stretch*) mmff.getComponent("MMFF94 Stretch");
	for (Position poss = 0; poss < stretch->getStretches().size(); poss++)
	{
		const MMFF94Stretch::Stretch& s = stretch->getStretches()[poss];
		bool found = false;

		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms1[poss2] != s.atom1->getName() ||
					atoms2[poss2] != s.atom2->getName())
			{
				continue;
			}

			found = true;
			if (s.r0 != r0s[poss2] ||
					s.kb != kbs[poss2])
			{
				Log.error() << std::endl
										<< "Problem Stretch:   " << filename << "   " 
										<< s.atom1->getName() << " " << s.atom2->getName() << std::endl
										<< "got " << s.r0 << "   " << s.kb << "   " << s.sbmb << "    " << std::endl
										<< "was " << r0s[poss2] << "   " << kbs[poss2] << "   " << is_sbmb[poss2]
										<< std::endl;
			}

			break;
		}
		
		if (!found) 
		{
			Log.error() << "Could not find atoms " << atoms1[poss] << " " << atoms2[poss] << std::endl;
		}
	}

	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float stretch_diff = std::fabs(mmff.getEnergy() - results[1]);

	if (std::fabs(stretch_diff / results[1]) > 1.0 / 100.0 && stretch_diff > 0.001)
	{
		Log.error() << filename << "   " << results[1] << "  " << mmff.getEnergy() << std::endl;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////
bool testStretchBend(MMFF94& mmff, const String& filename)
{
	/*
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".bend");
	LineBasedFile infile(full_file_name);
	vector<String> atoms1, atoms2, atoms3;
	vector<Position>   type;
	vector<float>  theta0, delta, energy;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 7)
		{
			Log.error() << "Problem: " << __FILE__ << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		atoms3.push_back(fields[2]);
		type.push_back(fields[3].toUnsignedInt());
		theta0.push_back(fields[4].toFloat());
		delta.push_back(fields[5].toFloat());
		energy.push_back(fields[6].toFloat());
	}

	MMFF94Bend* bend = (MMFF94Bend*) mmff.getComponent("MMFF94 Bend");
	for (Position poss = 0; poss < bend->getBends().size(); poss++)
	{
		const MMFF94Bend::Bend& s = bend->getBends()[poss];
		bool found = false;

		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms1[poss2] != s.atom1->ptr->getName() ||
					atoms2[poss2] != s.atom2->ptr->getName() ||
					atoms3[poss2] != s.atom3->ptr->getName())
			{
				continue;
			}

			float delta = std::fabs(s.energy - energy[poss2]);
			found = true;
			if (s.theta0 != theta0[poss2] ||
					delta > energy[poss2] / 20.0 && delta > 0.001)
			{
				Log.error() << std::endl
										<< "Problem Bend:   " << filename << "   " 
										<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() 
										<< " " << s.atom3->ptr->getName()  << std::endl
										<< "got " << s.theta0 << "   " << s.ATIJK << "   " << s.energy << std::endl
										<< "was " << theta0[poss2] << "   " << type[poss2] << "   " << energy[poss2]
										<< std::endl;
			}

			break;
		}
		
		if (!found) 
		{
			Log.error() << "Could not find atoms " << atoms1[poss] 
				<< " " << atoms2[poss] << " "
				<< " " << atoms3[poss] << std::endl;
		}
	}
*/
	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float s_plus_b = results[2] + results[1];
	float diff = std::fabs(mmff.getEnergy() - results[4] - s_plus_b);

	if (std::fabs(diff / results[1]) > 1.0 / 100.0 && diff > 0.001)
	{
		Log.error() << filename << "   " << results[4] + s_plus_b << "  " 
																		 << mmff.getEnergy() << std::endl;
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////
bool testBend(MMFF94& mmff, const String& filename)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".bend");
	LineBasedFile infile(full_file_name);
	vector<String> atoms1, atoms2, atoms3;
	vector<Position>   type;
	vector<float>  theta0, delta, energy;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 7)
		{
			Log.error() << "Problem: " << __FILE__ << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		atoms3.push_back(fields[2]);
		type.push_back(fields[3].toUnsignedInt());
		theta0.push_back(fields[4].toFloat());
		delta.push_back(fields[5].toFloat());
		energy.push_back(fields[6].toFloat());
	}

	MMFF94Bend* bend = (MMFF94Bend*) mmff.getComponent("MMFF94 Bend");
	for (Position poss = 0; poss < bend->getBends().size(); poss++)
	{
		const MMFF94Bend::Bend& s = bend->getBends()[poss];
		bool found = false;

		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms1[poss2] != s.atom1->ptr->getName() ||
					atoms2[poss2] != s.atom2->ptr->getName() ||
					atoms3[poss2] != s.atom3->ptr->getName())
			{
				continue;
			}

			float delta = std::fabs(s.energy - energy[poss2]);
			found = true;
			if (s.theta0 != theta0[poss2] ||
					delta > energy[poss2] / 20.0 && delta > 0.001)
			{
				Log.error() << std::endl
										<< "Problem Bend:   " << filename << "   " 
										<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() 
										<< " " << s.atom3->ptr->getName()  << std::endl
										<< "got " << s.theta0 << "   " << s.ATIJK << "   " << s.energy << std::endl
										<< "was " << theta0[poss2] << "   " << type[poss2] << "   " << energy[poss2]
										<< std::endl;
			}

			break;
		}
		
		if (!found) 
		{
			Log.error() << "Could not find atoms " << atoms1[poss] 
				<< " " << atoms2[poss] << " "
				<< " " << atoms3[poss] << std::endl;
		}
	}

	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float bend_diff = std::fabs(mmff.getEnergy() - results[2]);

	if (std::fabs(bend_diff / results[1]) > 1.0 / 100.0 && bend_diff > 0.001)
	{
		Log.error() << filename << "   " << results[2] << "  " << mmff.getEnergy() << std::endl;
		return false;
	}

	return true;
}

int runtests(const vector<String>& filenames)
{
	MMFF94 mmff;

	Size ok = 0;
	for (Position pos = 0; pos < filenames.size(); pos++)
	{
		String full_file_name(dir +FileSystem::PATH_SEPARATOR + filenames[pos] + ".mol2");
		System* system = readTestFile(full_file_name);
		if (system == 0)
		{
			Log.error() << "Could not read mol2 file " << full_file_name << std::endl;
			return -1;
		}

		if (!mmff.setup(*system))
		{
			Log.error() << "Setup failed for " << full_file_name << std::endl;
			return -1;
		}

		mmff.updateEnergy();

//    		if (testStretch(mmff, filenames[pos])) ok++;
//   		if (testBend(mmff, filenames[pos])) ok++;
		if (testStretchBend(mmff, filenames[pos])) ok++;
	}

	Log.info() << "Tested " << filenames.size() << " files, " << ok << " files ok" << std::endl;
		
	return 0;
}

vector<String> getTestFiles()
{
	vector<String> results;
	LineBasedFile infile(dir + FileSystem::PATH_SEPARATOR + "filenames.txt");
	while (infile.readLine())
	{
		results.push_back(infile.getLine());
	}

	results.pop_back();

	return results;
}


int main(int argc, char** argv)
{
	if (argc != 3)
	{
		Log.error() << "Usage: readMMFF94TestFile <dir with extracted test files> <all|system name>" << std::endl;
		return 1;
	}

	dir = argv[1];

	vector<String> files;
	if (String(argv[2]) == "all")
	{
   files = getTestFiles();
	}
	else
	{
   files.push_back(argv[2]);
	}

	return runtests(files);
}
