// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: readMMFF94TestFile.C,v 1.1.2.44 2006/02/22 14:08:48 amoll Exp $
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
#include <BALL/MOLMEC/MMFF94/MMFF94Torsion.h>
#include <BALL/MOLMEC/MMFF94/MMFF94OutOfPlaneBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94NonBonded.h>

#include <math.h>

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
	vector<double> charges, fcharges;
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
		ait->setCharge(fcharges[pos]);
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
		results.push_back(fields[0].toFloat());
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

	return diff < diff_max || diff < 0.001;
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

	enableOneComponent("MMFF94 Stretch", mmff);
	mmff.updateEnergy();
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

	if (!isOk(mmff.getEnergy(), results[1]))
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
		energy.push_back(fields[4].toFloat());
		f_ij.push_back(fields[5].toFloat());
	}

	// calulatate stretches and bends
	mmff.getComponent("MMFF94 Stretch")->setEnabled(true);
	mmff.getComponent("MMFF94 Bend")->setEnabled(true);
	mmff.updateEnergy();

	enableOneComponent("MMFF94 StretchBend", mmff);
	mmff.updateEnergy();
	MMFF94StretchBend* comp= (MMFF94StretchBend*) mmff.getComponent("MMFF94 StretchBend");

	for (Position poss = 0; poss < comp->getStretchBends().size(); poss++)
	{
		const MMFF94StretchBend::StretchBend& s = comp->getStretchBends()[poss];

		vector<double> constants, constants_ours;
		constants_ours.push_back(s.kba_kji);
		constants_ours.push_back(s.kba_ijk);

		Index sbtijk = -1;
		double energy1 = 0.0;
		Position found = 0;
		for (Position poss2 = 0; poss2 < atoms1.size(); poss2++)
		{
			if (atoms2[poss2] != s.atom2->ptr->getName()) continue;

			if ((atoms1[poss2] == s.atom1->ptr->getName() &&
					 atoms3[poss2] == s.atom3->ptr->getName()) 
					||
			   (atoms3[poss2] == s.atom1->ptr->getName() &&
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
			
			//s.sbtijk != sbtijk || // may differ!!!

			bool ok1 = BALL_REAL_EQUAL(constants[0], constants_ours[0], 0.0001) &&
								 BALL_REAL_EQUAL(constants[1], constants_ours[1], 0.0001); 

			bool ok2 = BALL_REAL_EQUAL(constants[1], constants_ours[0], 0.0001) &&
								 BALL_REAL_EQUAL(constants[0], constants_ours[1], 0.0001); 

			if ((!ok1 && !ok2) || !isOk(s.energy, energy1, 50))
			{
				Log.error() << std::endl
										<< "Problem StretchBend:   " << filename << "   " 
										<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() << " " << s.atom3->ptr->getName()  << std::endl
										<< "got " << s.sbtijk << "  " << constants_ours[0] << " " << constants_ours[1] << "   " << s.energy << std::endl
										<< "was " << sbtijk << "  " << constants[0] << " " << constants[1]  << "   " << energy1 << std::endl;
			}

			break;
		}
		
		if (found < 2 && s.energy != 0.0)  
		{
 			Log.error() << "Could not find atoms [sb] " << s.atom1->ptr->getName() << " "	
																							    << s.atom2->ptr->getName() << " "
																									<< s.atom3->ptr->getName() << " " << found << std::endl;
		}
	}

	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double s_plus_b = results[4];

	if (!isOk(mmff.getEnergy(), s_plus_b))
	{
		Log.error() << filename << "   " << s_plus_b << "  " << mmff.getEnergy() << std::endl;
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
		energy.push_back(fields[6].toFloat());
		ka.push_back(fields[7].toFloat());
	}

	enableOneComponent("MMFF94 Bend", mmff);
	mmff.updateEnergy();
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

			found = true;
			if (BALL_REAL_EQUAL(s.theta0, theta0[poss2], 0.001) &&
					BALL_REAL_EQUAL(s.ka, ka[poss2], 0.001) &&
					isOk(s.energy, energy[poss2]))
			{
				break;
			}

			if (s.emperical &&
					isOk(s.theta0, theta0[poss2]) &&
					isOk(s.ka, ka[poss2]) &&
					isOk(s.energy, energy[poss2]))
			{
				break;
			}

			Log.error() << std::endl
									<< "Problem Bend:   " << filename << "    emperical? "  << s.emperical << "   "
									<< s.atom1->ptr->getName() << " " << s.atom2->ptr->getName() 
									<< " " << s.atom3->ptr->getName()  << std::endl
									<< "got delta " << s.delta_theta << "  theta " << s.theta0 << " ka " << s.ka << "  ATIJK " << s.ATIJK << "  ENERGY " << s.energy << std::endl
									<< "was delta " << delta[poss2] << " theta " <<  theta0[poss2] << " ka " << ka[poss2] << "  ATIJK " << type[poss2] << "  ENERGY " << energy[poss2]
										<< std::endl;
			ok = false;

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

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	if (!isOk(mmff.getEnergy(), results[2]))
	{
		Log.error() << filename << "   " << results[2] << "  " << mmff.getEnergy() << std::endl;
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
		energy.push_back(fields[6].toFloat());
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

		String n1 = t.atom1->ptr->getName();
		String n2 = t.atom2->ptr->getName();
		String n3 = t.atom3->ptr->getName();
		String n4 = t.atom4->ptr->getName();

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

		ok &= isOk(t.energy, energy[found]);

		if (ok) continue;

		Log.error() << std::endl
								<< "Problem Torsion :   " << filename << " "
								<< t.atom1->ptr->getName() << " " << t.atom2->ptr->getName() << " "
								<< t.atom3->ptr->getName() << " " << t.atom4->ptr->getName() << "   "
								<< t.atom1->type << " " << t.atom2->type << " " << t.atom3->type << " " << t.atom4->type
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
		energy.push_back(fields[5].toFloat());
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

		String n1 = t.i->ptr->getName();
		String n2 = t.j->ptr->getName();
		String n3 = t.k->ptr->getName();
		String n4 = t.l->ptr->getName();

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
								<< t.i->type << " " << t.j->type << " " << t.k->type << " " << t.l->type << std::endl
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
		e_vdw.push_back(fields[3].toFloat());
		e_rep.push_back(fields[4].toFloat());
		e_attr.push_back(fields[5].toFloat());
		e_q.push_back(fields[6].toFloat());
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
					 atoms1[as] == pv[poss].second->getName() &&
					 atoms2[as] == pv[poss].first->getName())
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
				!isOk(data.VDW_energy, e_vdw[as]))
		{
			Log.error() << "Problem NB:   " << filename << " "
									<< atoms1[as] << " " << atoms2[as] << std::endl
									<< "got e " << data.eij << " r " << data.rij << "   " << data.VDW_energy << std::endl
									<< "was e " << eps[as] << " r " << rij[as] << "   " << e_vdw[as] << std::endl;
		}
	}

	if (!compare) return true;

	vector<double> results = getResults(dir +FileSystem::PATH_SEPARATOR + filename);

	double e = results[10];

	if (!isOk(comp->getVDWEnergy(), e))
	{
		Log.error() << filename << "   " << comp->getVDWEnergy() << "   " << e << std::endl;
		return false;
	}

	return true;
}

int runtests(const vector<String>& filenames)
{
	MMFF94 mmff;

	vector<String> not_ok;
	vector<String> rings_wrong;
	vector<String> aromatic_rings_wrong;
	Size ok = 0;
	long wrong_torsion_types = 0;
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

		for (Position p = 0; p < mmff.countComponents(); p++)
		{
			mmff.getComponent(p)->setEnabled(true);
		}

		if (!mmff.setup(*system))
		{
			Log.error() << "Setup failed for " << full_file_name << std::endl;
			return -1;
		}

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

/*
		Log.info() << "Got rings: " << mmff.getRings().size()  << " "  << __FILE__ << "  " << __LINE__<< std::endl;
		for (Position p = 0; p < mmff.getRings().size(); p++)
		{
			HashSet<Atom*>::ConstIterator it = mmff.getRings()[p].begin();
			for (; +it; ++it)
			{
				Log.info() << (**it).getName() << " ";
			}
			Log.info() << std::endl;
		}
*/

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

		bool result = mmff.getUnassignedAtoms().size() == 0;
		if (!result)
		{
			Log.info() << "We have unassigned atoms: " << mmff.getUnassignedAtoms().size() << std::endl;
		}

//    		result &= testStretch(mmff, filenames[pos], true);
//       result &= testBend(mmff, filenames[pos], true);
//    		result &= testStretchBend(mmff, filenames[pos], true);
//    		result &= testTorsions(mmff, filenames[pos], true, wrong_torsion_types);
//    		result &= testPlanes(mmff, filenames[pos], true);
 		result &= testNonBonded(mmff, filenames[pos], true);

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

	Log.info() << "Wrong torsion types: " << wrong_torsion_types << std::endl;
		
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
