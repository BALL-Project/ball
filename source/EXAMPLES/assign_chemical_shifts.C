// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// A small program for assigning chemical shifts to a molecule 
// given an NMRStar file. 

#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/NMR/shiftModule.h>

#include <BALL/SYSTEM/path.h>

using namespace std;
using namespace BALL;

int main(int argc, char** argv)
{

	Log << "Loading pdb file ...";
	PDBFile pdb_file("../../TEST/data/NMRStarFile_test_1z0r.pdb");
	if (!pdb_file.good())
	{
		// if file does not exist: complain and abort
		Log.error() << "error opening  pdb file" << endl;
		return 2;
	}

	System system;
	pdb_file >> system;
	Log << " done." << endl;

	Log << "Initializing FragmentDB...";
	FragmentDB db("");
	Log << " done." << endl;

	Log << "Normalizing names...";
	system.apply(db.normalize_names);
	Log << " done." << endl;

	Chain& chain = *(system.beginChain());

	Log << "Loading NMRStar file...";
	NMRStarFile nmr_file;

	try{
		nmr_file.open("../../TEST/data/NMRStarFile_test_1z0r.bmr", std::ios::in);
		nmr_file.read();
	}
	catch(...){
		Log.error() << "error opening the NMRStar file" << endl;
		return 2;
	}
	Log << " done." << endl;
	Log << "File contains "      << nmr_file.getNumberOfAtoms() << " chemical shifts" << endl;
	Log << "Number of samples: " << nmr_file.getNumberOfSamples() << endl;

	NMRStarFile::MolecularSystem nmr_mol_sys = nmr_file.getMolecularInformation();
	Log << "Molecular systems name: " <<  nmr_mol_sys.system_name << endl;
	Log << "Number of possible chemical units: " <<  nmr_mol_sys.getNumberOfChemicalUnits() << endl;

	// the chemical units of this file
	for (Size k=0; k <  nmr_mol_sys.getNumberOfChemicalUnits(); k++)
	{
		NMRStarFile::MolecularSystem::ChemicalUnit cu = nmr_mol_sys.getChemicalUnit(k);
		Log << " " << k << " : " <<  cu.component_name
			  << " " <<  cu.label << endl;
	}

	// get the shift data
	std::vector<NMRStarFile::NMRAtomDataSet> const& nmr_data = nmr_file.getNMRData();
	Log << "Number of shift sets: " <<  nmr_data.size() << endl;

	for (Size k=0; k < nmr_data.size(); k++)
	{
		Log << " " << k << " shift set name: " << nmr_data[k].name << "   -- condition name: " << nmr_data[k].condition << endl;
	}

	// decide for a chemical unit	
	NMRStarFile::MolecularSystem::ChemicalUnit cu = nmr_mol_sys.getChemicalUnit(0);
	String chemical_unit_label = cu.label;
	Log << "Consider chemical unit named " <<  cu.component_name << endl;

	// get the ChemicalUnit by label
	NMRStarFile::MolecularSystem::ChemicalUnit chemical_unit = nmr_file.getChemicalUnitByLabel(chemical_unit_label);

	// find the corresponding SampleCondition
	String pH, temperature, pressure;
	if (chemical_unit.shifts && nmr_file.hasSampleCondition(chemical_unit.shifts->condition))
	{
		NMRStarFile::SampleCondition& condition = nmr_file.getSampleConditionByName(chemical_unit.shifts->condition);
		if (condition.hasType("pH"))
			pH = condition.values["pH"];
		if (condition.hasType("pH*"))
			pH = condition.values["pH*"];
		if (condition.hasType("temperature"))
			temperature  = condition.values["temperature"];
		if (condition.hasType("pressure"))
			pressure  = condition.values["pressure"];

		Log << "NMR conditions: pH=" << pH << ", temp=" << temperature << ", press=" << pressure << endl;
	}

	// get the sequences to compute an alignment
	Log << "Sequences to align: " << endl;
	Log <<  "   " << Peptides::GetSequence(chain) << endl;
	Log <<  "   " << nmr_file.getResidueSequence(0) << endl;

	// decide for a chemical unit
	Log << "Label of selected chemical unit: " << chemical_unit_label << endl;

	// create a mapping based on an alignment
	Log << "Create a mapping... " ;
	NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file, chemical_unit_label);
	mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT");
	Log << " done." << endl;

	Log << "Number of mismatches: " << mapper.getNumberOfMismatches() << endl;
	Log << "Number of gaps: "       << mapper.getNumberOfGaps() << endl;

	// assign the shifts
	Log << "Assign the chemical shifts..." ;
	nmr_file.assignShifts(mapper);
	Log << "done." << endl;
	Log << "Assigned "      << nmr_file.getNumberOfShiftsAssigned()
		  << " shifts given " << nmr_file.getNumberOfAtoms() << " chemical shifts." << endl << endl;

	// print
	for (AtomIterator at_it = chain.beginAtom(); at_it != chain.endAtom(); at_it++)
	{
		if (at_it->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT))
		{
			cout << " " << at_it->getFullName() << " has shift "
				   << at_it->getProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT).getFloat() << endl;
		}
	}

	return 0;
}


