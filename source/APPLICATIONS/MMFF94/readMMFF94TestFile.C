// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: readMMFF94TestFile.C,v 1.1.2.19 2005/04/10 23:09:04 amoll Exp $
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
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
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

bool testStretch(MMFF94& mmff, const String& filename, bool compare)
{
	bool ok = true;

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

				ok = false;
			}

			break;
		}
		
		if (!found) 
		{
 			Log.error() << "Could not find atoms [s] " << s.atom1->getName() << " "	
																							    << s.atom2->getName() << " " << std::endl;
		}
	}

	if (!compare) return ok;

	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float stretch_diff = std::fabs(mmff.getEnergy() - results[1]);

	if (std::fabs(stretch_diff / results[1]) > 1.0 / 100.0 && stretch_diff > 0.001)
	{
		Log.error() << filename << "   " << results[1] << "  " << mmff.getEnergy() << std::endl;
		return false;
	}

	return ok;
}

///////////////////////////////////////////////////////////
bool testStretchBend(MMFF94& mmff, const String& filename, bool compare)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".stretchbend");
	LineBasedFile infile(full_file_name);
	vector<String> atoms1, atoms2, atoms3;
	vector<Position>   type;
	vector<float>  energy, f_ij;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 6)
		{
			Log.error() << "Problem: " << __FILE__ << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		atoms3.push_back(fields[2]);
		type.push_back(fields[3].toUnsignedInt());
		energy.push_back(fields[4].toFloat());
		f_ij.push_back(fields[5].toFloat());
	}

	MMFF94StretchBend* comp= (MMFF94StretchBend*) mmff.getComponent("MMFF94 StretchBend");
	for (Position poss = 0; poss < comp->getStretchBends().size(); poss++)
	{
		const MMFF94StretchBend::StretchBend& s = comp->getStretchBends()[poss];
		Position found = 0;
		vector<float> constants;
		vector<float> constants_ours;
		constants_ours.push_back(s.kba_kji);
		constants_ours.push_back(s.kba_ijk);

		Position sbtijk = 0;

		float energy1 = 0.0;
		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if ((atoms1[poss2] == s.atom1->ptr->getName() &&
					atoms2[poss2] == s.atom2->ptr->getName() &&
					atoms3[poss2] == s.atom3->ptr->getName()) 
					||
			   (atoms3[poss2] == s.atom1->ptr->getName() &&
					atoms2[poss2] == s.atom2->ptr->getName() &&
					atoms1[poss2] == s.atom3->ptr->getName()))
			{
				energy1 += energy[poss2];
				constants.push_back(f_ij[poss2]);
				sbtijk = type[poss2];
				found ++;
			}


			if (found != 2) continue;

			sort(constants.begin(), constants.end());
			sort(constants_ours.begin(), constants_ours.end());

			if (//s.sbtijk != sbtijk ||
					constants[0] != constants_ours[0] ||
					constants[1] != constants_ours[1] )
			{
				Log.error() << std::endl
										<< "Problem StretchBend:   " << filename << "   " 
										<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() << " " << s.atom3->ptr->getName()  << std::endl
										<< "got " << s.sbtijk << "  " << constants_ours[0] << " " << constants_ours[1] << "   " << s.energy << std::endl
										<< "was " << sbtijk << "  " << constants[0] << " " << constants[1]  << "   " << energy1 << std::endl;
			}

			break;
		}
		
		if (found != 2)  
		{
 			Log.error() << "Could not find atoms [sb] " << s.atom1->ptr->getName() << " "	
																							    << s.atom2->ptr->getName() << " "
																									<< s.atom3->ptr->getName() << " " << std::endl;
		}
	}

	if (!compare) return true;

	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float s_plus_b = results[2] + results[1] + results[4];
	float diff = std::fabs(mmff.getEnergy() - s_plus_b);

	if (std::fabs(diff / s_plus_b) > 5.0 / 100.0 && diff > 0.001)
	{
		Log.error() << filename << "   " << s_plus_b << "  " 
																		 << mmff.getEnergy() << std::endl;
		return false;
	}

	return true;
}


///////////////////////////////////////////////////////////
bool testBend(MMFF94& mmff, const String& filename, bool compare)
{
	bool ok = true;
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
			if ((atoms1[poss2] != s.atom1->ptr->getName() ||
					atoms2[poss2] != s.atom2->ptr->getName() ||
					atoms3[poss2] != s.atom3->ptr->getName()) 
					&&
			    (atoms3[poss2] != s.atom1->ptr->getName() ||
					atoms2[poss2] != s.atom2->ptr->getName() ||
					atoms1[poss2] != s.atom3->ptr->getName()))

			{
				continue;
			}

			float deltae = std::fabs(s.energy - energy[poss2]);
			found = true;
			if (s.theta0 != theta0[poss2] ||
					deltae > energy[poss2] / 20.0 && deltae > 0.001)
			{
				Log.error() << std::endl
										<< "Problem Bend:   " << filename << "   " 
										<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() 
										<< " " << s.atom3->ptr->getName()  << std::endl
										<< "got " << s.delta_theta << "  " << s.theta0 << "   " << s.ATIJK << "   " << s.energy << std::endl
										<< "was " << delta[poss2] << " " <<  theta0[poss2] << "   " << type[poss2] << "   " << energy[poss2]
										<< std::endl;
				ok = false;
			}

			break;
		}
		
		if (!found) 
		{
 			Log.error() << "Could not find atoms [b] " << s.atom1->ptr->getName() << " "	
																							    << s.atom2->ptr->getName() << " "
																									<< s.atom3->ptr->getName() << " " << std::endl;
		}
	}

	if (!compare) return ok;

	vector<float> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float bend_diff = std::fabs(mmff.getEnergy() - results[2]);

	if (std::fabs(bend_diff / results[1]) > 5.0 / 100.0 && bend_diff > 0.001)
	{
		Log.error() << filename << "   " << results[2] << "  " << mmff.getEnergy() << std::endl;
		return false;
	}

	return ok;
}

int runtests(const vector<String>& filenames)
{
	MMFF94 mmff;

//   	mmff.removeComponent("MMFF94 Stretch");
	mmff.removeComponent("MMFF94 Bend");
	mmff.removeComponent("MMFF94 StretchBend");

	vector<String> not_ok;
	vector<String> rings_wrong;
	vector<String> aromatic_rings_wrong;
	Size ok = 0;
	for (Position pos = 0; pos < filenames.size(); pos++)
	{
		Log.info() << "> " << filenames[pos] << std::endl;
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

		Size nr_rings = 0;
		Size nr_aromatic_rings = 0;
		
		try
		{
			String rings_file_name(dir +FileSystem::PATH_SEPARATOR + filenames[pos] + ".rings");
			LineBasedFile ring_file(rings_file_name);
			ring_file.readLine();
			nr_rings = ring_file.getLine().toUnsignedInt();
			ring_file.readLine();
			nr_aromatic_rings = ring_file.getLine().toUnsignedInt();
		}
		catch(...)
		{
		}

		bool wrong_rings = false;
		if (mmff.getRings().size() != nr_rings)
		{
			Log.error() << "Got " << mmff.getRings().size() << "          rings, was " << nr_rings << std::endl;
			rings_wrong.push_back(filenames[pos]);
			wrong_rings = true;
		}

		if (mmff.getAromaticRings().size() != nr_aromatic_rings)
		{
			Log.error() << "Got " << mmff.getAromaticRings().size() << " aromatic rings, was " << nr_aromatic_rings << std::endl;
			aromatic_rings_wrong.push_back(filenames[pos]);
			wrong_rings = true;
		}

		bool result = mmff.getUnassignedAtoms().size() == 0;
		if (!result)
		{
			Log.info() << "We have unassigned atoms: " << mmff.getUnassignedAtoms().size() << std::endl;
		}

 		result &= testStretch(mmff, filenames[pos], true);
//          result &= testBend(mmff, filenames[pos], true);
//     		result &= testStretchBend(mmff, filenames[pos], true);


		if (result) ok++;
		else if (!wrong_rings) not_ok.push_back(filenames[pos]);
	}

	Log.info() << "Tested " << filenames.size() << " files, " << ok << " files ok" << std::endl;

	Log.info() << "Tests failed (and rings were correct) for: " << std::endl;;
	for (Position pos = 0; pos < not_ok.size(); pos++)
	{
		Log.info() << not_ok[pos] << " ";
	}
	Log.info() << std::endl;

	Log.info() << "Wrong rings for: " << std::endl;;
	for (Position pos = 0; pos < rings_wrong.size(); pos++)
	{
		Log.info() << rings_wrong[pos] << " ";
	}
	Log.info() << std::endl;

	Log.info() << "Wrong aromatic rings for: " << std::endl;;
	for (Position pos = 0; pos < aromatic_rings_wrong.size(); pos++)
	{
		Log.info() << aromatic_rings_wrong[pos] << " ";
	}
	Log.info() << std::endl;
		
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

int bla()
{
	MMFF94StretchBendParameters sb_param;
	Path    path;
	String  filename1(path.find("MMFF94/MMFFSTBN.PAR"));
	String  filename2(path.find("MMFF94/MMFFDFSB.PAR"));
	sb_param.readParameters(filename1, filename2);

	Atom a1, a2, a3;
	a1.setType(7);
	a2.setType(3);
	a3.setType(37);
	float k1, k2;
	Log.info() << sb_param.getParameters(2, a1, a2, a3, k1, k2) << " " << k1 << " " << k2 << std::endl;
	return 1;
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
