// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/genericMolFile.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/UCK.h>
#include <set>
#include <map>
#include "version.h"

using namespace BALL;
using namespace std;

bool isUnique(const Molecule* mol, map<String, pair<int, String> >& mol_hashs, bool unique_topologies)
{
	String hash;
	if (!unique_topologies)
	{
		Conformation::generateHash(mol, hash);
	}
	else
	{
		UCK uck(*mol, false);
		hash = uck.getUCK();
	}
	map<String, pair<int, String> >::iterator it = mol_hashs.find(hash);
	if (it == mol_hashs.end())
	{
		mol_hashs.insert(make_pair(hash, make_pair(1, mol->getName())));
		return true;
	}
	else
	{
		it->second.first++;
		return false;
	}
}


// returns the number of non-unique conformations
int printNonUniqueConformations(map < String, pair < int, String > > & mol_hashs)
{
	int no = 0;
	for (map < String, pair < int, String > > ::iterator it = mol_hashs.begin(); it != mol_hashs.end(); it++)
	{
		if (it->second.first > 1)
		{
			no++;
			Log.level(10) <<" Conformation "<<it->first(0, 10);
			if (it->second.second != "")
			{
				Log.level(10)<<", "<<it->second.second;
			}
			Log.level(10)<<" occured "<<it->second.first<<" times."<<endl;
		}
	}
	return no;
}


/* check whether the given Molecule really contains only one molecule */
void checkBonds(const Atom* atom, set<const Atom*>& visited_atoms)
{
	if (visited_atoms.find(atom) != visited_atoms.end())
	{
		return;
	}

	visited_atoms.insert(atom);
	for (Atom::BondConstIterator b_it = atom->beginBond(); +b_it; b_it++)
	{
		checkBonds(b_it->getPartner(*atom), visited_atoms);
	}
}

// Return true if molecule passed all tests, else return false.
bool checkMolecule(Molecule* mol, int molecule_no, map<String, pair<int, String> >& mol_hashs, bool unique_topologies, bool no_unique_conf_check)
{
	bool bond_length_ok = 1;
	bool elements_ok = 1;
	bool one_molecule = 1;
	bool has_atoms = 1;
	bool has_3D_coordinates = 1;
	bool has_hydrogens = 0;
	bool partial_charges_ok = 1;

	Size no_atoms = 0;
	bool all_x_zero = 1; // are all x-coordinates in the file set to zero?
	bool all_y_zero = 1;
	bool all_z_zero = 1;

	if (!no_unique_conf_check || unique_topologies) // check uniqueness only if desired
	{
		if (!isUnique(mol, mol_hashs, unique_topologies)) return false;
	}

	for (AtomConstIterator it = mol->beginAtom(); +it; it++, no_atoms++)
	{
		if (bond_length_ok) // skip if such errors have already been detected for this molecule
		{
			for (Atom::BondConstIterator b_it = it->beginBond(); +b_it; b_it++)
			{
				double length = b_it->getLength();
				if (length < 0.7 || length > 2.5)
				{
					bond_length_ok = false;
				}
			}
		}

		if (elements_ok)
		{
			if (it->getElement().getName() == BALL_ELEMENT_NAME_DEFAULT)
			{
				elements_ok = false;
			}
		}
		if (!has_hydrogens)
		{
			if (it->getElement().getSymbol() == "H")
			{
				has_hydrogens = 1;
			}
		}

		const TVector3<float>& pos = it->getPosition();
		if (all_x_zero)
		{
			if (fabs(pos[0]) > 0.001) all_x_zero = 0;
		}
		if (all_y_zero)
		{
			if (fabs(pos[1]) > 0.001) all_y_zero = 0;
		}
		if (all_z_zero)
		{
			if (fabs(pos[2]) > 0.001) all_z_zero = 0;
		}
		if (fabs(it->getCharge()) > 5)
		{
			partial_charges_ok = 0;
		}
	}

	if (no_atoms == 0)
	{
		has_atoms = false;
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" molecule has no atoms!"<<endl;
	}
	else
	{
		set<const Atom*> visited_atoms;
		checkBonds(&*mol->beginAtom(), visited_atoms);
		if (visited_atoms.size() != no_atoms)
		{
			one_molecule = false;
			Log.level(10)<<"  "<<"Error for molecule "<<molecule_no<<":"<<" more than one molecule or bonds are missing !"<<endl;
		}
	}
	if (all_x_zero || all_y_zero || all_z_zero)
	{
		has_3D_coordinates = false;
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" no 3D coordinates!"<<endl;
	}
	if (!elements_ok)
	{
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" atoms without element detected!"<<endl;
	}
	if (!bond_length_ok)
	{
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" senseless bond-length detected!"<<endl;
	}
	if (!has_hydrogens)
	{
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" no hydrogens in molecule! We need protonated molecules as input for docking!"<<endl;
	}
	if (!partial_charges_ok)
	{
		Log.level(10)<<"   "<<"Error for molecule "<<molecule_no<<":"<<" atoms with nonsense partial charges detected!"<<endl;
	}

	return has_atoms && has_hydrogens && has_3D_coordinates && one_molecule && bond_length_ok && elements_ok && partial_charges_ok;
}


int main(int argc, char* argv[])
{
	CommandlineParser par("LigCheck", "check molecules for errors", VERSION, String(__DATE__), "Checks and evaluations");
	par.registerMandatoryInputFile("i", "input molecule file");
	par.registerMandatoryOutputFile("o", "output file");
	par.registerOptionalDoubleParameter("ef", "error fraction; print error if fraction of invalid mols is larger", 0.5);
	par.registerFlag("ri", "remove invalid molecules.", true);
	par.registerFlag("ut", "check for unique topologies");
	par.registerFlag("nc", "no not check for unique conformations");
	par.registerFlag("rm", "remove input file when finished");
	String man = "This tool checks all molecules of the given input file for errors. Supported formats are mol2, sdf or drf (DockResultFile, xml-based).\n\nThe following checks are done for each molecule:\n\n\
    * bond-lengths may not be completely senseless (i.e. <0.7 or >2.5 Angstroem)\n\
    * each 'molecule' in the input file may only contain one actual molecule, i.e. there may be no unconnected atoms or fragments.\n\
    * each atom must have a valid assigned element\n\
    * the molecule must be protonated (since this is necessary for docking/(re-)scoring).\n\
    * 3D coordinates must be present (instead of 2D coordinates; also necessary for docking/(re-)scoring)\n\
    * partial charges may not contain completely senseless values (>5 or <-5).\n\
    * each conformation should appear only once within the given file, otherwise it is rejected and not written to the output file. However, if option '-ut' is used, molecules will instead be checked for unique topologies.\n\nIf option '-ri' is used, only those molecules that pass all those tests are written to the output file. If this option is not used, all molecules are written to output containing a property 'score_ligcheck' with a value of 1 if the molecule passed all tests or with a value of 0 if it did not pass them.";
	par.setToolManual(man);
	par.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	par.setSupportedFormats("o",MolFileFactory::getSupportedFormats());
	par.parse(argc, argv);

	GenericMolFile* input = MolFileFactory::open(par.get("i"));
	GenericMolFile* output = 0;
	if (argc > 2) output = MolFileFactory::open(par.get("o"), ios::out, input);
	int no_errors = 0;

	bool remove_invalid = 0;
	bool unique_topologies = 0;
	bool no_unique_conf_check = 0;
	if (par.has("ri")) remove_invalid = 1;
	if (par.has("ut")) unique_topologies = 1;
	if (par.has("nc")) no_unique_conf_check = 1;

	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		String dummy = "0";
		Result::Method method = Result::getMethod(6);
		drf_output->setOutputParameters(method, "score_ligcheck", dummy);
	}

	// map the hash-string of each molecule that has been read already to the number of its occurences and its name
	map<String, pair<int, String> > mol_hashs;

	int molecule_no = 1;
	for (Molecule* mol = 0; molecule_no == 1 || mol; molecule_no++)
	{
		try
		{
			mol = input->read();
		}
		catch(BALL::Exception::GeneralException e)
		{
			no_errors++;
		}
		if (!mol) break;

		bool ok = checkMolecule(mol, molecule_no, mol_hashs, unique_topologies, no_unique_conf_check);
		if (ok)
		{
			if (output)
			{
				if (!remove_invalid) mol->setProperty("score_ligcheck", 1); // for Result-section entry
				*output << *mol;
			}
		}
		else if (output && !remove_invalid)
		{
			mol->setProperty("score_ligcheck", 0); // for Result-section entry
			*output << *mol;
			no_errors++;
		}
		else no_errors++;
		delete mol;
	}
	molecule_no--;

	int no_non_unique = printNonUniqueConformations(mol_hashs);

	int status = 0;
	no_errors -= no_non_unique;

	String ef = par.get("ef");
	double error_fraction = 1;
	if (ef != CommandlineParser::NOT_FOUND)
	{
		error_fraction = ef.toDouble();
	}

	if (no_non_unique > 0)
	{
		status = 1;
		Log.level(20)<<endl<<no_non_unique<<" molecules occured multiple times within the input file.";
		if (output && remove_invalid)
		{
			Log.level(20)<<" Each of those molecules was only written once to the output file !";
		}
		Log.level(20)<<endl;
	}
	if (no_errors > 0)
	{
		if (no_errors == molecule_no)
		{
			status = 1;
			Log.error()<<endl<<"All molecules are invalid!!";
			if (output && remove_invalid)
			{
				Log.error()<<" Thus no molecules have been written to output!";
			}
			Log.error()<<endl;
		}
		else if (no_errors > molecule_no*error_fraction)
		{
			status = 1;
			Log.error()<<endl<<no_errors<<" molecules contained errors";
			if (output && remove_invalid)
			{
				Log.error()<<" and were not written to output file!";
			}
			Log.error()<<endl;
		}
		else
		{
			status = 0;
			Log.level(20)<<endl<<no_errors<<" molecules contained errors";
			if (output && remove_invalid)
			{
				Log.level(20)<<" and were not written to output file!";
			}
			Log.level(20)<<endl;
		}
	}
	if (no_non_unique == 0 && no_errors == 0)
	{
		Log.level(20)<<endl<<"No errors detected.";
		if (output && !drf_output) Log.level(20)<<"All molecules have been written to output file";
		Log.level(20)<<endl;
	}

	delete input;
	delete output;

	if (par.has("rm"))
	{
		File::remove(par.get("i"));
	}

	return status;
}
