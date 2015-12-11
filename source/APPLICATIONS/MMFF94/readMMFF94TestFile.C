// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: readMMFF94TestFile.C,v 1.1.8.4 2007/08/09 17:33:33 amoll Exp $
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
#include <BALL/KERNEL/PTE.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Processors.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <cmath>

using namespace std;
using namespace BALL;

String dir;

void readCharges(vector<String>& filenames)
{
	HashMap<String, float> charge_map;

	for (Position p = 0; p < filenames.size(); p++)
	{
		vector<String> atoms, names, symbols, fields;
		vector<double> charges, fcharges;
		vector<short> types;

		LineBasedFile infile(dir + "/" + filenames[p]+".atoms");
		while (infile.readLine())
		{
			if (infile.getLine().split(fields) != 6)
			{
				Log.error() << "Error in " << filenames[p] << " Not 6 fields in one line " << infile.getLine() << std::endl;
			}

			String symbol = fields[3];
			float charge = fields[5].toFloat();

			if (charge_map.has(fields[3]))
			{
				if (charge_map[fields[3]] != charge)
				{
					Log.error() << "# " <<   symbol    << "   " << charge_map[fields[3]] << "   " << charge << std::endl;
				}
			}
			else
			{
				charge_map[symbol] = charge;
			}
		}
	}

	HashMap<String, float>::Iterator it = charge_map.begin();
	for (; it != charge_map.end(); ++it)
	{
		Log.info() << it->first << "   " << it->second << std::endl;
	}
}


System* readTestFile(String filename)
{
	MOL2File mol2_file(filename);
	System* system = new System;
	mol2_file >> *system;

	filename = filename.getSubstring(0, filename.size() - 4);
	filename += "atoms";

	vector<String> atoms, names, symbols, fields;
	vector<double> charges, fcharges;
	vector<short> types;

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
		symbols.push_back(fields[3]);
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
//   ait->setType(types[pos]); // <---------------------------------
		ait->setProperty("Type", types[pos]);
		ait->setProperty("TypeName", symbols[pos]);
		ait->setProperty("OriginalInitialCharge", fcharges[pos]);
 		ait->setFormalCharge((Index)fcharges[pos]);
		ait->setRadius(charges[pos]);
	}

	return system;
}


vector<double> getResults(String filename)
{
	filename += ".results";

	vector<double> results;
	LineBasedFile infile(filename);
	while (infile.readLine())
	{
		vector<String> fields;
		infile.getLine().split(fields);
		results.push_back(fields[0].toFloat() * Constants::JOULE_PER_CAL);
	}

	return results;
}


void enableOneComponent(const String& comp, MMFF94& mmff)
{
	for (Position p = 0; p < mmff.countComponents(); p++)
	{
		ForceFieldComponent& cp = *mmff.getComponent(p);
		cp.setEnabled(cp.getName() == comp);
	}
}


bool isOk(double value, double reference, double max_difference = 100)// max 1 percent difference
{
	double diff = fabs(value - reference);
	double diff_max = fabs(reference / max_difference); 
	
	if (max_difference == 10) return diff < diff_max || diff < 0.005;

	return diff < diff_max || diff < 0.01;
}
	

bool testStretch(MMFF94& mmff, const String& filename, bool compare)
{
	bool ok = true;

	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".stretch");
	LineBasedFile infile(full_file_name);
	vector<String> atoms1, atoms2;
	vector<bool>   is_sbmb;
	vector<double>  r0s, kbs;
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

	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	MMFF94StretchBend* stretch = (MMFF94StretchBend*) mmff.getComponent("MMFF94 StretchBend");
	for (Position poss = 0; poss < stretch->getStretches().size(); poss++)
	{
		const MMFF94StretchBend::Stretch& s = stretch->getStretches()[poss];
		bool found = false;

		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms1[poss2] != s.atom1->getName() ||
					atoms2[poss2] != s.atom2->getName())
			{
				continue;
			}

			found = true;

			bool sbmb_ok = is_sbmb[poss2] == s.sbmb;

			// equal values for parameters?
			if (sbmb_ok &&
					BALL_REAL_EQUAL(s.r0, r0s[poss2], 0.0001) && 
					BALL_REAL_EQUAL(s.kb, kbs[poss2], 0.0001))
			{
				break;
			}

			// almost equal for emeperical values ?
			if (sbmb_ok && s.emperical)
			{
				if (isOk(s.r0, r0s[poss2]) &&
						isOk(s.kb, kbs[poss2]))
				{
					break;
				}
			}

			Log.error() << std::endl
									<< "Problem Stretch:   " << filename << "     emperical? " << s.emperical  << "   "
									<< s.atom1->getName() << " " << s.atom2->getName() << std::endl
									<< "got r " << s.r0 << "   kb " << s.kb << "   " << s.sbmb << "    " << std::endl
									<< "was r " << r0s[poss2] << "   kb " << kbs[poss2] << "   " << is_sbmb[poss2]
									<< std::endl;

			ok = false;

			break;
		}
		
		if (!found) 
		{
 			Log.error() << "Could not find atoms [s] " << s.atom1->getName() << " "	
																							    << s.atom2->getName() << " " << std::endl;
		}
	}

	if (!compare) return ok;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float e = mmff.getStretchEnergy();
	if (!isOk(e, results[1]))
	{
		Log.error() << filename << "   " << results[1] << "  " << e << std::endl;
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
	vector<double>  energy, f_ij;
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
		energy.push_back(fields[4].toFloat() * Constants::JOULE_PER_CAL);
		f_ij.push_back(fields[5].toFloat());
	}

	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	MMFF94StretchBend* comp= (MMFF94StretchBend*) mmff.getComponent("MMFF94 StretchBend");

	for (Position poss = 0; poss < comp->getStretchBends().size(); poss++)
	{
		const MMFF94StretchBend::StretchBend& s = comp->getStretchBends()[poss];
		const MMFF94StretchBend::Bend& b = comp->getBends()[s.bend_index];

		vector<double> constants, constants_ours;
		constants_ours.push_back(s.kba_kji);
		constants_ours.push_back(s.kba_ijk);

		Index sbtijk = -1;
		double energy1 = 0.0;
		Position found = 0;
		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms2[poss2] != b.atom2->getName()) continue;

			if ((atoms1[poss2] == b.atom1->getName() &&
					 atoms3[poss2] == b.atom3->getName()) 
					||
			   (atoms3[poss2] == b.atom1->getName() &&
					atoms1[poss2] == b.atom3->getName()))
			{
				energy1 += energy[poss2];
				constants.push_back(f_ij[poss2]);
				sbtijk = type[poss2];
				found ++;
			}


			if (found != 2) continue;

			sort(constants.begin(), constants.end());
			sort(constants_ours.begin(), constants_ours.end());
			
			//s.sbtijk != sbtijk || // may differ!!!

			bool ok1 = BALL_REAL_EQUAL(constants[0], constants_ours[0], 0.0001) &&
								 BALL_REAL_EQUAL(constants[1], constants_ours[1], 0.0001); 

			bool ok2 = BALL_REAL_EQUAL(constants[1], constants_ours[0], 0.0001) &&
								 BALL_REAL_EQUAL(constants[0], constants_ours[1], 0.0001); 

			if ((!ok1 && !ok2) || !isOk(s.energy, energy1, 10))
			{
				Log.error() << std::endl
										<< "Problem StretchBend:   " << filename << "   " 
										<< b.atom1->getName() << " " << b.atom2->getName() << " " << b.atom3->getName()  << std::endl
										<< "got " << s.sbtijk << "  " << constants_ours[0] << " " << constants_ours[1] << "   " << s.energy << std::endl
										<< "was " << sbtijk << "  " << constants[0] << " " << constants[1]  << "   " << energy1 << std::endl;
			}

			break;
		}
		
		if (found < 1 && s.energy != 0.0)  
		{
 			Log.error() << "Could not find atoms [sb] " << filename << " " 
									<< b.atom1->getName() << " "	<< b.atom2->getName() << " " << b.atom3->getName() << " " << found << std::endl;
		}
	}

	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double s_plus_b = results[4];

	float e = mmff.getStretchBendEnergy();
	if (!isOk(e, s_plus_b, 40))
	{
		Log.error() << filename << "   " << s_plus_b << "  " << e << std::endl;
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
	vector<double>  theta0, delta, energy, ka;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 8)
		{
			Log.error() << "Problem: " << __FILE__ << " " << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		atoms3.push_back(fields[2]);
		type.push_back(fields[3].toUnsignedInt());
		theta0.push_back(fields[4].toFloat());
		delta.push_back(fields[5].toFloat());
		energy.push_back(fields[6].toFloat() * Constants::JOULE_PER_CAL);
		ka.push_back(fields[7].toFloat());
	}

	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	MMFF94StretchBend* bend = (MMFF94StretchBend*) mmff.getComponent("MMFF94 StretchBend");
	for (Position poss = 0; poss < bend->getBends().size(); poss++)
	{
		const MMFF94StretchBend::Bend& s = bend->getBends()[poss];
		bool found = false;

		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if ((atoms1[poss2] != s.atom1->getName() ||
					atoms2[poss2] != s.atom2->getName() ||
					atoms3[poss2] != s.atom3->getName()) 
					&&
			    (atoms3[poss2] != s.atom1->getName() ||
					atoms2[poss2] != s.atom2->getName() ||
					atoms1[poss2] != s.atom3->getName()))

			{
				continue;
			}

			found = true;
			if (BALL_REAL_EQUAL(s.theta0, theta0[poss2], 0.001) &&
					BALL_REAL_EQUAL(s.ka, ka[poss2], 0.001) &&
					isOk(s.energy, energy[poss2], 10))
			{
				break;
			}

			if (s.emperical &&
					isOk(s.theta0, theta0[poss2]) &&
					isOk(s.ka, ka[poss2]) &&
					isOk(s.energy, energy[poss2], 10))
			{
				break;
			}

			Log.error() << std::endl
									<< "Problem Bend:   " << filename << "    emperical? "  << s.emperical << "   "
									<< s.atom1->getName() << " " << s.atom2->getName() 
									<< " " << s.atom3->getName()  << std::endl
									<< "got delta " << s.delta_theta << "  theta " << s.theta0 << " ka " << s.ka << "  ATIJK " << s.ATIJK << "  ENERGY " << s.energy << std::endl
									<< "was delta " << delta[poss2] << " theta " <<  theta0[poss2] << " ka " << ka[poss2] << "  ATIJK " << type[poss2] << "  ENERGY " << energy[poss2]
										<< std::endl;
			ok = false;

			break;
		}
		
		if (!found) 
		{
 			Log.error() << "Could not find atoms [b] "
									<< s.atom1->getName() << " "	<< s.atom2->getName() << " " << s.atom3->getName() << " " << std::endl;
		}
	}

	if (!compare) return ok;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	float e = mmff.getBendEnergy();
	if (!isOk(e, results[2]))
	{
		Log.error() << filename << "   " << results[2] << "  " << e << std::endl;
		return false;
	}

	return ok;
}

///////////////////////////////////////////////////////////
bool testTorsions(MMFF94& mmff, const String& filename, bool compare, long& wrong_types)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".torsions");
	LineBasedFile infile;

	// some molecules dont have torsions
	try
	{
		infile.open(full_file_name);
	}
	catch(...)
	{
		return true;
	}

	vector<String> atoms1, atoms2, atoms3, atoms4;
	vector<Position>   type;
	vector<double>  energy, angle, v1, v2, v3;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 9)
		{
			Log.error() << "Problem: " << __FILE__ << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		atoms3.push_back(fields[2]);
		atoms4.push_back(fields[3]);
		type.push_back(fields[4].toUnsignedInt());
		angle.push_back(fields[5].toFloat());
		energy.push_back(fields[6].toFloat() * Constants::JOULE_PER_CAL);
		v1.push_back(fields[7].toFloat());
		v2.push_back(fields[8].toFloat());
		v3.push_back(fields[9].toFloat());
	}

	enableOneComponent("MMFF94 Torsion", mmff);
	mmff.updateEnergy();
	MMFF94Torsion* comp= (MMFF94Torsion*) mmff.getComponent("MMFF94 Torsion");

	Size empty_torsions = 0;
	for (Position p = 0; p < v1.size(); p++)
	{
		if (v1[p] == 0.0 && 
				v2[p] == 0.0 &&
				v3[p] == 0.0)
		{
			empty_torsions ++;
		}
	}

	if (comp->getTorsions().size() != atoms4.size() - empty_torsions)
	{
		Log.error() << "Not all torsions found " << filename << " got "
								<< comp->getTorsions().size() << " was " << atoms4.size() - empty_torsions << std::endl;
	}

	HashSet<Position> found_torsions;

	for (Position poss = 0; poss < comp->getTorsions().size(); poss++)
	{
		const MMFF94Torsion::Torsion& t = comp->getTorsions()[poss];

		vector<double> constants, constants_ours;

		String n1 = t.atom1->getName();
		String n2 = t.atom2->getName();
		String n3 = t.atom3->getName();
		String n4 = t.atom4->getName();

		Index found = -1;
		bool swap = false;

		for (Position as = 0; as < atoms1.size(); as++)
		{
			if (atoms1[as] == n1 && atoms2[as] == n2 && atoms3[as] == n3 && atoms4[as] == n4)
			{
				found = as;
				break;
			}

			if (atoms4[as] == n1 && atoms3[as] == n2 && atoms2[as] == n3 && atoms1[as] == n4)
			{
				found = as;
				swap = true;
				break;
			}
		}

		if (found == -1)
		{
			Log.error() << "Could not find atoms [torsion] " << n1 << " " << n2 << " " << n3 << " " << n4 << std::endl;
			continue;
		}

		found_torsions.insert(found);

		bool ok; 
		
		if (!t.heuristic)
		{
			ok = (BALL_REAL_EQUAL(t.v1 , v1[found], 0.0001) &&
						BALL_REAL_EQUAL(t.v2 , v2[found], 0.0001) &&
						BALL_REAL_EQUAL(t.v3 , v3[found], 0.0001))
					||
					 (BALL_REAL_EQUAL(t.v3 , v1[found], 0.0001) &&
						BALL_REAL_EQUAL(t.v2 , v2[found], 0.0001) &&
						BALL_REAL_EQUAL(t.v1 , v3[found], 0.0001));
		}
		else
		{
			ok = isOk(t.v1, v1[found]) &&
					 isOk(t.v2, v2[found]) &&	
					 isOk(t.v3, v3[found]);
		}

		bool type_ok = (t.type == (Index)type[found]);
		if (!type_ok)
		{
			wrong_types++;
			ok = false;
		}

		ok &= isOk(t.energy, energy[found], 10);

		if (ok) continue;

		Log.error() << std::endl
								<< "Problem Torsion :   " << filename << " "
								<< t.atom1->getName() << " " << t.atom2->getName() << " "
								<< t.atom3->getName() << " " << t.atom4->getName() << "   "
								<< t.atom1->getType() << " " << t.atom2->getType() << " " << t.atom3->getType() << " " << t.atom4->getType()
								<< " got type " << t.type << " was " << type[found] << std::endl
								<< "got " << t.v1 << " " << t.v2 << " " << t.v3 << " angle " << t.angle << "   E: " << t.energy<< std::endl
								<< "was " << v1[found] << " " << v2[found] << " " << v3[found] << " angle " << angle[found] << "   E: " << energy[found] << std::endl;
	}

	for (Position p = 0; p < atoms1.size(); p++)
	{
		if (v1[p] == 0.0 && v2[p] == 0.0 && v3[p] == 0.0) continue;
		if (found_torsions.has(p)) continue;

		Log.error() << "No data for torsion " << type[p] << "  " << atoms1[p] << " " << atoms2[p] << " " << atoms3[p] << " " << atoms4[p] << std::endl;
	}

		
	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double e = results[7];

	if (!isOk(mmff.getEnergy(), e))
	{
		Log.error() << filename << "   " << mmff.getEnergy() << "   " << e << std::endl;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////
bool testPlanes(MMFF94& mmff, const String& filename, bool compare)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".planes");
	LineBasedFile infile;

	// some molecules dont have planes
	try
	{
		infile.open(full_file_name);
	}
	catch(...)
	{
		return true;
	}

	vector<String> atoms1, atoms2, atoms3, atoms4;
	vector<double>  energy, angle, k;
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
		atoms4.push_back(fields[3]);
		angle.push_back(fields[4].toFloat());
		energy.push_back(fields[5].toFloat() * Constants::JOULE_PER_CAL);
		k.push_back(fields[6].toFloat());
	}

	enableOneComponent("MMFF94 OutOfPlaneBend", mmff);
	mmff.updateEnergy();
	MMFF94OutOfPlaneBend* comp= (MMFF94OutOfPlaneBend*) mmff.getComponent("MMFF94 OutOfPlaneBend");

	Size empty = 0;
	for (Position p = 0; p < k.size(); p++)
	{
		if (k[p] == 0.0) empty ++;
	}

	if (comp->getOutOfPlaneBends().size() * 3 != atoms4.size() - empty)
	{
		Log.error() << "Not all planes found " << filename << " got "
								<< comp->getOutOfPlaneBends().size() * 3 << " was " << atoms4.size() - empty << std::endl;
	}

	HashSet<Position> found_planes;

	for (Position poss = 0; poss < comp->getOutOfPlaneBends().size(); poss++)
	{
		const MMFF94OutOfPlaneBend::OutOfPlaneBend& t = comp->getOutOfPlaneBends()[poss];

		String n1 = t.i->getName();
		String n2 = t.j->getName();
		String n3 = t.k->getName();
		String n4 = t.l->getName();

		Index found = -1;

		for (Position as = 0; as < atoms1.size(); as++)
		{
			if (atoms2[as] != n2) continue;

			HashSet<String> set;
			set.insert(atoms1[as]);
			set.insert(atoms2[as]);
			set.insert(atoms3[as]);

			if (!set.has(n1) || !set.has(n2) || !set.has(n3)) continue;

			found = as;
			break;
		}

		if (found == -1)
		{
			Log.error() << "Could not find [plane] " << n1 << " " << n2 << " " << n3 << " " << n4 << std::endl;
			continue;
		}


		if (BALL_REAL_EQUAL(t.k_oop, k[found], 0.0001)) continue;

		Log.error() << std::endl
								<< "Problem Plane:   " << filename << " "
								<< n1 << " " << n2 << " " << n3 << " " << n4 << "   "
								<< t.i->getType() << " " << t.j->getType() << " " << t.k->getType() << " " << t.l->getType() << std::endl
								<< "got " << t.k_oop << t.energy<< std::endl
								<< "was " << k[found] << "   E: " << energy[found] << std::endl;
	}

	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double e = results[3];

	if (!isOk(mmff.getEnergy(), e))
	{
		Log.error() << filename << "   " << mmff.getEnergy() << "   " << e << std::endl;
		return false;
	}

	return true;
}

///////////////////////////////////////////////////////////
bool testNonBonded(MMFF94& mmff, const String& filename, bool compare)
{
	String full_file_name = (dir +FileSystem::PATH_SEPARATOR + filename + ".nonbonded");
	LineBasedFile infile;

	// some molecules dont have planes
	try
	{
		infile.open(full_file_name);
	}
	catch(...)
	{
		return true;
	}

  // ATOMPAIR           R       VDW      EREP    EATTR      EQ     R*    EPS
	vector<String> atoms1, atoms2;
	vector<double>  d, e_vdw, e_rep, e_attr, e_q, rij, eps;
	while (infile.readLine())
	{
		vector<String> fields;
		if (infile.getLine().split(fields) < 9)
		{
			Log.error() << "Problem: " << __FILE__ << __LINE__ << std::endl;
			continue;
		}

		atoms1.push_back(fields[0]);
		atoms2.push_back(fields[1]);
		d.push_back(fields[2].toFloat());
		e_vdw.push_back(fields[3].toFloat() * Constants::JOULE_PER_CAL);
		e_rep.push_back(fields[4].toFloat() * Constants::JOULE_PER_CAL);
		e_attr.push_back(fields[5].toFloat() * Constants::JOULE_PER_CAL);
		e_q.push_back(fields[6].toFloat() * Constants::JOULE_PER_CAL);
		rij.push_back(fields[7].toFloat());
		eps.push_back(fields[8].toFloat());
	}

	enableOneComponent("MMFF94 NonBonded", mmff);
	mmff.updateEnergy();
	MMFF94NonBonded* comp= (MMFF94NonBonded*) mmff.getComponent("MMFF94 NonBonded");

 	const ForceField::PairVector& pv = comp->getAtomPairs();

	for (Position as = 0; as < atoms1.size(); as++)
	{
		bool found = false;
		Position poss = 0;
		for (; poss < pv.size(); poss++)
		{
			if ((atoms2[as] == pv[poss].second->getName() &&
					 atoms1[as] == pv[poss].first->getName()) 
					||
					(atoms1[as] == pv[poss].second->getName() &&
					 atoms2[as] == pv[poss].first->getName()))
			{
				found = true;
				break;
			}
		}

		if (!found)
		{
			Log.error() << "Could not find VDW pair " << atoms1[as] << " " << atoms2[as] << std::endl;
			continue;
		}

 		const MMFF94NonBonded::NonBondedPairData& data = comp->getNonBondedData()[poss];

		if (!isOk(data.eij, eps[as]) ||
				!isOk(data.rij, rij[as]) ||
				!isOk(data.vdw_energy, e_vdw[as], 10))
		{
			Log.error() << "Problem NB VDW:   " << filename << " "
									<< atoms1[as] << " " << atoms2[as] << std::endl
									<< "got e " << data.eij << " r " << data.rij << "   " << data.vdw_energy << std::endl
									<< "was e " << eps[as] << " r " << rij[as] << "   " << e_vdw[as] << std::endl;
		}

		if (!isOk(data.es_energy, e_q[as], 10))
		{
			Log.error() << "Problem NB ES:   " << filename << " "
									<< atoms1[as] << " " << atoms2[as] << std::endl
									<< "got E " << data.es_energy << std::endl
									<< "was E " << e_q[as] << std::endl;
		}
	}

	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double e = results[10];

	if (!isOk(comp->getVDWEnergy(), e))
	{
		Log.error() << filename << " VDW  " << comp->getVDWEnergy() << "   " << e << std::endl;
		return false;
	}

	e = results[11];

	if (!isOk(comp->getESEnergy(), e))
	{
		Log.error() << filename << " ES  " << comp->getESEnergy() << "   " << e << std::endl;
		return false;
	}


	return true;
}

bool testCharge(System& system, String filename)
{
	bool ok = true;
	AtomIterator ait = system.beginAtom();
	for (; +ait; ++ait)
	{
		// we only compare correctly assigned atom types
		if (ait->getType() != ait->getProperty("Type").getInt())
		{
			continue;
		}

		float icharge = ait->getProperty("InitialCharge").getFloat();
		float oicharge = ait->getProperty("OriginalInitialCharge").getFloat();
		if (!isOk(icharge, oicharge))
		{
			ok = false;
			Log.error() << "IC! " << filename << " " << ait->getName() << " " << ait->getTypeName() << " " 
									<< icharge << " " << oicharge << std::endl;
			continue;
		}
		float f = ait->getCharge();
		if (!isOk(f,ait->getRadius()))
		{
			ok = false;
			Log.error() << "C!  " << filename << " " << ait->getName() << " " << ait->getTypeName() << " " 
									<< f << " " << ait->getRadius()<< std::endl;
		}
	}

	return ok;
}


Size all_atoms;

HashSet<String> type_errors_bigger;
HashSet<String> type_errors_lower;

Size wrong_types, type_errors;

bool testType(System& system, String filename, AtomTyper& /* typer */)
{
//    	typer.assignTo(system);

	bool ok = true;
	AtomIterator ait = system.beginAtom();
	for (; +ait; ++ait)
	{
		Atom& a = *ait;
		String org_symbol = a.getProperty("TypeName").getString();
		String our_symbol = a.getTypeName();
		all_atoms++;

		Index org_type = a.getProperty("Type").getInt();
//   		if (org_type == a.getType()) continue;
		if (org_symbol == our_symbol) continue;

		if (a.getElement().getSymbol() == "H")
		{
			Bond& bond = *a.getBond(0);
			Atom& a2 = *bond.getPartner(a);
			if (a2.getType() != a2.getProperty("Type").getInt()) continue;
		}

		type_errors++;

		if (org_type < a.getType()) 
		{
//   			if (type_errors_bigger.has(a.getType())) continue;
			type_errors_bigger.insert(a.getType());
		}
		else
		{
//   			if (type_errors_lower.has(org_type)) continue;
			type_errors_lower.insert(org_type);
		}
	

		String out(org_symbol + " <-> " + our_symbol);

		wrong_types++;

		ok = false;
		Log.error() << "Type! " << filename << " " << a.getName() << "  was " << String(a.getProperty("Type").getInt()) 
								<< " " << out << " " << String(a.getType()) << " "  << std::endl;
	}

	return ok;
}



int runtests(const vector<String>& filenames)
{
	MMFF94AtomTyper typer;
	typer.setup(Path().find("MMFF94/TYPES.PAR"));
	Parameters p(Path().find("MMFF94/mmff94.ini"));
	typer.setupHydrogenTypes(p, "HydrogenTypes");
	typer.setupSymbolsToTypes(p, "Symbols");
	typer.setupAromaticTypes(p, "Aromatic");
	MMFF94AtomTypes types;
	types.readParameters(p, "AtomTypeProperties");
	typer.collectHeteroAtomTypes(types);

	MMFF94 mmff;
	mmff.options[MMFF94::Option::VDW_CUTOFF] = 0;
	mmff.options[MMFF94::Option::VDW_CUTON] = 0;
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTOFF] = 0;
	mmff.options[MMFF94::Option::ELECTROSTATIC_CUTON] = 0;
	mmff.options[MMFF94::Option::NONBONDED_CUTOFF] = 99999;
	mmff.options[MMFF94::Option::DISTANCE_DEPENDENT_DIELECTRIC] = false;


	vector<String> not_ok;
	vector<String> rings_wrong;
	vector<String> aromatic_rings_wrong;
	Size ok = 0;
	long wrong_torsion_types = 0;
	for (Position pos = 0; pos < filenames.size(); pos++)
	{
//   		Log.info() << "> " << filenames[pos] << std::endl;
		bool result = true;
		String full_file_name(dir +FileSystem::PATH_SEPARATOR + filenames[pos] + ".mol2");
		System* system = readTestFile(full_file_name);
		if (system == 0)
		{
			Log.error() << "Could not read mol2 file " << full_file_name << std::endl;
			return -1;
		}

		for (Position p = 0; p < mmff.countComponents(); p++)
		{
			mmff.getComponent(p)->setEnabled(true);
		}

		
		if (!mmff.setup(*system))
		{
			Log.error() << "Setup failed for " << full_file_name << std::endl;
			return -1;
		}

		/*
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
		if (mmff.getRings().size() < nr_rings)
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

		result = mmff.getUnassignedAtoms().size() == 0;
		if (!result)
		{
			Log.info() << "We have unassigned atoms: " << mmff.getUnassignedAtoms().size() << std::endl;
		}
*/
    result &=testType(*system, filenames[pos], typer);
				
		result &= testStretch(mmff, filenames[pos], true);
		result &= testBend(mmff, filenames[pos], true);
		result &= testStretchBend(mmff, filenames[pos], true);
		result &= testTorsions(mmff, filenames[pos], true, wrong_torsion_types);
		result &= testPlanes(mmff, filenames[pos], true);
		result &= testNonBonded(mmff, filenames[pos], true);
		result &= testCharge(*system, filenames[pos]);


	

 		if (result) ok++;
//    		else if (!wrong_rings) not_ok.push_back(filenames[pos]);
		delete system;
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

	Log.info() << "Wrong torsion types: " << wrong_torsion_types << std::endl;

	Log.info() << "Types: Ok " << all_atoms - type_errors << " False: " << type_errors << " cat: " 
							<< type_errors_lower.size() + type_errors_bigger.size() << std::endl;

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

int expressionTest(vector<String>& filenames, String expr, Index type, String type_name)
{
	HashSet<String> wrong_files;
	HashSet<String> correct;

	Size errors = 0;
	SmartsMatcher sm;
	for (Position pos = 0; pos < filenames.size(); pos++)
	{
		String filename(filenames[pos]);
		String full_file_name(dir +FileSystem::PATH_SEPARATOR + filenames[pos] + ".mol2");
		System* system = readTestFile(full_file_name);
		if (system == 0)
		{
			Log.error() << "Could not read mol2 file " << full_file_name << std::endl;
			return -1;
		}
		
		vector<vector<Atom*> > rings;
		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(rings, *system);
		AromaticityProcessor ap;
		ap.aromatizeSimple(rings);
		
		for (Position r = 0; r < rings.size(); r++)
		{
			const vector<Atom*>& ring = rings[r];

			for (Position a = 0; a < ring.size(); a++)
			{
				Atom& atom = *ring[a];
				atom.setProperty("IsAromatic", true); 
			}
		}

		HashSet<String> names_set;
		AtomIterator ait = system->beginAtom();
		for (; +ait; ++ait)
		{
			if (ait->getProperty("Type").getInt() == type)
			{
				if (type_name != (String)ait->getProperty("TypeName").getString()) continue;
				names_set.insert(ait->getName());
			}
		}

		vector<set<const Atom*> > result;
		sm.match(result, *system->getMolecule(0), expr);

		for (Position pos = 0; pos < result.size(); pos++)
		{
			set<const Atom*>& set = result[pos];
			if (set.size() != 1) 
			{
				Log.error() << "Problem with smarts expr " << expr  << " in " << __FILE__ << " " << __LINE__ << std::endl;
				continue;
			}

			Atom& atom = *(Atom*)*set.begin();

			Index org_type = atom.getProperty("Type").getInt();
			names_set.erase(atom.getName());

			if (org_type > type ||
			    atom.getProperty("TypeName").getString() == type_name)
			{
				continue;
			}

			errors ++;
			Log.error() << filename << "  " << atom.getName() << "  " << org_type << " " << atom.getProperty("TypeName").getString()<< std::endl;
			wrong_files.insert(filename);
		}

		HashSet<String>::Iterator it = names_set.begin();
		for (; +it; ++it)
		{
			errors ++;
			Log.error() << filename << "  " << *it << "  ###### " << std::endl;
			wrong_files.insert(filename);
		}
	}


	Log.error() << "Errors:  " << errors << std::endl;

	Log.info () << std::endl;
	HashSet<String>::Iterator sit = wrong_files.begin();
	for (; +sit; ++sit)
	{
		Log.info() << "test/" << *sit << ".mol2  ";
	}

	Log.info() << std::endl;

	return 0;
}

int main(int argc, char** argv)
{
	if (argc < 3)
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
	else if (String(argv[2]) == "bpti")
	{
		MMFF94 mmff;

		HINFile pdb("../BALLVIEW/bpti.hin");
		System system;
		pdb >> system;
 		mmff.setup(system);

	SmartsMatcher sm;
	vector<set<const Atom*> > result;
	sm.match(result, *system.getMolecule(0), "[O;$(O=#6)]");
		/*
		///
		vector<vector<Atom*> > rings;
		RingPerceptionProcessor rpp;
		rpp.calculateSSSR(rings, system);


		vector<HashSet<Atom*> > 		rings_;
		vector<HashSet<Atom*> > 		aromatic_rings_;
		for (Position i = 0; i < rings.size(); i++)
		{
			// all 3er and 4er rings are copied below
 			if (rings[i].size() == 3 || rings[i].size() == 4) continue;

			HashSet<Atom*> set;
			for (Position j = 0; j < rings[i].size(); j++)
			{
				set.insert(rings[i][j]);
			}
			rings_.push_back(set);
		}

		vector<vector<Atom*> > rings2 = rpp.getAllSmallRings();

		// copy 3er and 4er rings
		for (Position i = 0; i < rings2.size(); i++)
		{
			HashSet<Atom*> set;
			for (Position j = 0; j < rings2[i].size(); j++)
			{
				set.insert(rings2[i][j]);
			}
			rings_.push_back(set);
		}


		///////////////////////////////////////
		/// calculate all aromatic rings in the molecule
		aromatic_rings_.clear();

		AromaticityProcessor ap;
		ap.aromatizeSimple(rings);
	
		for (Position i = 0; i < rings.size(); i ++)
		{
			HashSet<Atom*> set;
			for (Position j = 0; j < rings[i].size(); j++)
			{
				set.insert(rings[i][j]);
			}
			aromatic_rings_.push_back(set);
		}

*/
		return true;
	}
	else
	{
   files.push_back(argv[2]);
	}

//   readCharges(files);
//   return 0;

	wrong_types = 0;
	all_atoms = 0;
	type_errors = 0;

	if (argc == 4)
	{
		LineBasedFile lf(Path().find("MMFF94/TYPES.PAR"));
		String type_name(argv[3]);
		String expr;
		String type;
		while(lf.readLine())
		{
			String line = lf.getLine();
			if (line.hasPrefix("*")) continue;

			vector<String> fields;
			line.split(fields, "|");
			fields[1].trim();
			if (fields[1] == type_name)
			{
				fields[2].trim();
				type = fields[2];
				expr = fields[3];
				expr.trim();
				break;
			}
		}

		Log.info() << expr << std::endl;
		return expressionTest(files, expr, type.toInt(), type_name);
	}

	int result = runtests(files);
	StringHashMap<float>::Iterator hit = AtomTyper::rule_times.begin();
	for (; hit != AtomTyper::rule_times.end(); ++hit)
	{
		Log.error() << hit->first << " " << hit->second << std::endl;
	}

	return result;
}
