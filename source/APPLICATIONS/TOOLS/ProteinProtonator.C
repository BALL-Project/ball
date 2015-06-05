// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <openbabel/mol.h>
#include <openbabel/obconversion.h>
#include "version.h"

using namespace BALL;
using namespace std;

void copyHydrogens(OpenBabel::OBMol* obmol, Protein* protein);

int main(int argc, char* argv[])
{
	CommandlineParser parpars("ProteinProtonator", "protonate protein structures", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryOutputFile("o", "output file");
	parpars.registerOptionalDoubleParameter("ph", "pH-value for pH-dep. protonation", 7.0);
	String man = "";
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i","pdb");
	parpars.setSupportedFormats("o","pdb,drf");
	String manual = "ProteinProtonator allows you add hydrogens to a protein structure.\n\nNote that all hydrogen atoms already present in the input file will be ignored. If desired, you can specify a specific pH value, for which protonation is to be done; otherwise a neutral pH will be assumed.\n\nOutput of this tool is one pdb-file containing the input protein structure with added hydrogens atoms.";
	parpars.setToolManual(manual);
	parpars.parse(argc, argv);
	
	
	double pH = 7.4;
	if (parpars.get("ph") != CommandlineParser::NOT_FOUND)
	{
		pH = parpars.get("ph").toDouble();
	}
	
	//PDBFile pdb_input(parpars.get("i"));
	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	PDBFile* pdb_input = dynamic_cast<PDBFile*>(input); 
	if (!pdb_input)
	{
		Log.error() << "Cannot open PDB input file " << parpars.get("i") << endl;
		return 2;
	}
	
	Protein protein;
	pdb_input->read(protein);

/*	
 	//  !!!! this functionality in now covered by the tool AddMissingAtoms!
  
	FragmentDB fragdb;
	fragdb.setFilename("fragments/Fragments.db");
	fragdb.init();
	
	Log.disableOutput();
	protein.apply(fragdb.normalize_names);
	protein.apply(fragdb.build_bonds);
	Log.enableOutput();
	
	fragdb.destroy();
*/

	// If the input protein already has hydrogens, then delete them first.
	for (AtomIterator it=protein.beginAtom(); it!=protein.endAtom(); it++)
	{
		if (it->getElement().getSymbol() == "H")
		{
			it->select();
		}
		else 
		{
			it->deselect();
		}
	}
	protein.removeSelected();
	
	
	OpenBabel::OBMol* obmol = MolecularSimilarity::createOBMol(protein,0,1);
	obmol->DeleteHydrogens();
	obmol->SetDimension(3);
	obmol->AddConformer(obmol->GetCoordinates());
	
	// Add hydrogens for the specified ph-value
	obmol->AddHydrogens(false, true, pH);
	
	// Fetch final 3D coordinates
	copyHydrogens(obmol, &protein);
	
	// Generate output file and write protonated protein structure
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, pdb_input);
	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		drf_output->setToolInfo(parpars.getStartCommand(), parpars.getStartTime());
	}
	
	bool write_success = output->write(protein);
	
	output->close();
	input->close();
	
	delete obmol;
	delete input;
	delete output;
	
	if (write_success)
	{
		Log.level(20)<<"wrote protonated protein."<<endl;
	}
	else
	{
		Log.level(20)<<"protein protonation failed."<<endl;
	}
}


void copyHydrogens(OpenBabel::OBMol* obmol, Protein* protein)
{
	// We know that heavy-atoms in 'obmol' have exactly the same order as the ones in 'protein', since MolecularSimilarity::createOBMol() creates the former this way.
	HashMap<OpenBabel::OBAtom*, Atom*> atom_map;
	HashMap<Atom*, Size> h_count; // counts the hydrogens that each heavy atom has
	HashMap<Atom*, Size> added_h; // counts the hydrogens were already assigned to each heavy atom
	OpenBabel::OBAtomIterator ob_it=obmol->BeginAtoms();
	for (AtomIterator a_it = protein->beginAtom(); +a_it; a_it++, ob_it++)
	{
		atom_map.insert(make_pair(*ob_it, &*a_it));

		if ((*ob_it)->GetAtomicNum()==1) continue;

		Size num_h = 0;
		for (OpenBabel::OBBondIterator b_it = (*ob_it)->BeginBonds();
			b_it !=  (*ob_it)->EndBonds(); b_it++)
		{
			OpenBabel::OBBond* obbond = *b_it;
			OpenBabel::OBAtom* obatom = obbond->GetBeginAtom();
			if (obatom==*ob_it) obatom = obbond->GetEndAtom();
			if (obatom->GetAtomicNum()==1) num_h++;
		}
		h_count.insert(make_pair(&*a_it, num_h));
		added_h.insert(make_pair(&*a_it, 0));
	}

	for (OpenBabel::OBAtomIterator a_it=obmol->BeginAtoms(); a_it!=obmol->EndAtoms(); a_it++)
	{
		OpenBabel::OBAtom* obatom = *a_it;
		if (obatom->GetAtomicNum()!=1) continue;

		OpenBabel::OBBond* obbond = *obatom->BeginBonds();
		OpenBabel::OBAtom* ob_heavy_atom = obbond->GetBeginAtom();
		if (ob_heavy_atom == obatom)
		{
			ob_heavy_atom = obbond->GetEndAtom();
		}

		Atom* heavy_atom = atom_map.find(ob_heavy_atom)->second;
		PDBAtom* new_hydrogen = new PDBAtom;
		new_hydrogen->setElement(PTE_::getElement("H"));

		String name("H");
		String heavy_atom_name = heavy_atom->getName();
		if (heavy_atom_name.size() > 1)
		{
			if (String(heavy_atom_name[heavy_atom_name.size()-1]).isDigit())
			{
				heavy_atom_name = heavy_atom_name.substr(heavy_atom_name.size()-2);
			}
			else
			{
				heavy_atom_name = heavy_atom_name.substr(heavy_atom_name.size()-1);
			}
		}
		name += heavy_atom_name;

		// if heavy atom has more than one hydrogen, add digit
		HashMap<Atom*, Size>::iterator search_it = h_count.find(heavy_atom);
		if (search_it->second > 1)
		{
			search_it = added_h.find(heavy_atom);
			search_it->second++;
			name += String(search_it->second);
		}

		new_hydrogen->setName(name);
		Vector3 pos(obatom->x(), obatom->y(), obatom->z());
		new_hydrogen->setPosition(pos);
		Bond* bond = heavy_atom->createBond(*new_hydrogen);
		bond->setType(Bond::TYPE__COVALENT);
		heavy_atom->getResidue()->insert(*new_hydrogen);
	}
}
