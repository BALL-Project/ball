// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/SYSTEM/timer.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/DCDFile.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/MATHS/matrix44.h>
#include <BALL/DOCKING/COMMON/dockResult.h>

#include <iostream>
#include <map>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{
	// print usage information
	if (argc < 5)
	{
		Log.error() << "Compute the C_alpha-RMSD values after a rigid docking run." << endl;
		Log.error() << "Usage: " << argv[0] << " reference.pdb docked.pdb dockResult.bdr static_chain_name" << endl;

		return 1;
	}

	map<String, Position> type_map;
	type_map["ALA"] = 0;
	type_map["GLY"] = 1;
	type_map["VAL"] = 2;
	type_map["LEU"] = 3;
	type_map["ILE"] = 4;
	type_map["SER"] = 5;
	type_map["CYS"] = 6;
	type_map["THR"] = 7;
	type_map["MET"] = 8;
	type_map["PHE"] = 9;
	type_map["TYR"] = 10;
	type_map["TRP"] = 11;
	type_map["PRO"] = 12;
	type_map["HIS"] = 13;
	type_map["LYS"] = 14;
	type_map["ARG"] = 15;
	type_map["ASP"] = 16;
	type_map["GLU"] = 17;
	type_map["ASN"] = 18;
	type_map["GLN"] = 19;


	// define two systems and two proteins
	System    reference, docked;
	Protein   static_reference, static_docked;

	// we'll need that one later
	System    docked_backup;

	// and a PDB file to read and write the proteins
	PDBFile   pdb_file;

	// read the proteins
	pdb_file.open(argv[1]);
	Log.info() << "Reading reference " << argv[1] << "... " << flush;
	pdb_file >> reference;
	Log.info() << "Read " << reference.countAtoms() << " atoms." << endl;
	pdb_file.close();

	pdb_file.open(argv[2]);
	Log.info() << "Reading docked " << argv[2] << "... " << flush;
	pdb_file >> docked;
	Log.info() << "Read " << docked.countAtoms() << " atoms." << endl;
	pdb_file.close();

	docked_backup = docked;
	// Find the static chain
	// Remark: insert removes the chain in the original system!
	// that's why we need docked_backup 
	ChainIterator pi = reference.beginChain();
	for (; +pi; ++pi)
	{
		if (pi->getName() == argv[4])
			static_reference.insert(*pi);
	}
	for (pi = docked.beginChain(); +pi; ++pi)
	{
		if (pi->getName() == argv[4])
			static_docked.insert(*pi);
	}

	// map the two static chains
	Matrix4x4       T;
	StructureMapper mapper;
	Size            no_ca;
	double					rmsd;

	Log.info() << "Mapping static chain of " << argv[1] << " onto static chain of " << argv[2] << endl;
	Log.info() << " (this may take a while)..." << endl;

	Timer t;
	t.start();

	// default values
	double upper = 8.0;
	double lower = 4.0;
	double tolerance = 0.6;

	T = mapper.mapProteins(static_reference, static_docked, type_map, no_ca, rmsd, upper, lower, tolerance);

	t.stop();

	if (no_ca < 1)
	{
		Log.info() << "Sorry - couldn't map (no CA atoms?)" << endl;
		return 1;
	}

	Log.info() << "Mapped " << no_ca << " CA atoms." << endl;
	Log.info() << "RMSD (CA only): " << rmsd << " A" << endl;
	Log.info() << endl << "Transformation: " << endl;
	Log.info() << T;

	Log.info() << "Time to map the proteins: " << t.getClockTime() << "s" << endl;
	Log.info() << "Transforming " << argv[1] << endl;

	/** The static chain is still stored in static_..., while the remainder is stored
	 *  in docked and reference.
	 */
	mapper.setTransformation(T);
	reference.apply(mapper);
	static_reference.apply(mapper);

	Log.info() << "Reading the docking results..." << endl;
	DockResult dr;
	dr.readDockResult(argv[3]);
	const ConformationSet* conformation_set = dr.getConformationSet();
	vector<ConformationSet::Conformation> conformations = conformation_set->getScoring();
	Log.info() << "Done." << endl;

	Log.info() << "Computing RMSD values..." << endl;
	SnapShot snap;
	ofstream out("docking_rmsds.txt");
	// Calculate RMSD for each snapshot
	for (Size i=0; i<conformations.size(); ++i)
	{
		// get the whole docked system (static AND mobile chain), to apply the snapshot
		docked = docked_backup;
		snap = (*conformation_set)[i];
		snap.applySnapShot(docked);

		// split static chain from docked since we want to calculate the RMSD for static and mobile sperately
		for (pi = docked.beginChain(); +pi; ++pi)
		{
			if (pi->getName() == argv[4])
				static_docked.insert(*pi);
		}

		rmsd = 0.;          // RMSD for whole complex
		float rmsd_1 = 0.;  // RMSD for mobile chains
		float rmsd_2 = 0.;  // RMSD for static chains
		int count = 0;
		int count_1 = 0;
		int count_2 = 0;

		// calculate RMSD of CA atoms for mobile chains
		AtomIterator docked_atom=docked.beginAtom(), reference_atom=reference.beginAtom();
		while (+docked_atom && +reference_atom)
		{
			while (+docked_atom && docked_atom->getName() != "CA") docked_atom++;
			while (+reference_atom && reference_atom->getName() != "CA") reference_atom++;

			if (+docked_atom && +reference_atom)
			{
				rmsd += (docked_atom->getPosition() - reference_atom->getPosition()).getSquareLength();
				count++;

				rmsd_1 += (docked_atom->getPosition() - reference_atom->getPosition()).getSquareLength();
				count_1++;

				docked_atom++;
				reference_atom++;
			}
		}

		// calculate RMSD of CA atoms for static chains
		docked_atom=static_docked.beginAtom(), reference_atom=static_reference.beginAtom();
		while (+docked_atom && +reference_atom)
		{
			while (+docked_atom && docked_atom->getName() != "CA") docked_atom++;
			while (+reference_atom && reference_atom->getName() != "CA") reference_atom++;

			if (+docked_atom && +reference_atom)
			{
				rmsd += (docked_atom->getPosition() - reference_atom->getPosition()).getSquareLength();
				count++;

				rmsd_2 += (docked_atom->getPosition() - reference_atom->getPosition()).getSquareLength();
				count_2++;

				docked_atom++;
				reference_atom++;
			}
		}

		Log.info() << "***** Snaphot " << i << " *****" << endl;
		Log.info() << "RMSD is " << sqrt(rmsd / count) << " for " << count << " atoms." << endl;
		Log.info() << "RMSD of static protein " << sqrt(rmsd_2 / count_2) << " for " << count_2 << " atoms." << endl;
		Log.info() << "RMSD of mobile protein " << sqrt(rmsd_1 / count_1) << " for " << count_1 << " atoms." << endl;
		Log.info() << endl;

		// write out score of ith snapshot and calculated RMSD
		out << conformations[i].second << " " << sqrt(rmsd/count) << endl;
	}
	Log.info() << "Done." << endl;
	out.close();
	return 0;
}
