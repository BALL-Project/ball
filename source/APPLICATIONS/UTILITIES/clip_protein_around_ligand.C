// $Id: clip_protein_around_ligand.C,v 1.2 2003/06/02 17:24:23 anker Exp $
//
// A program for extracting a parts of a protein around a ligand.
// The output are XYZFiles because we use this program for creating AMSOL
// input files.
//
// The code is still quite chaotic, though. FIXME.

#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/XYZFile.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/STRUCTURE/residueChecker.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#include <BALL/SYSTEM/path.h>

using namespace BALL;
using namespace std;

void usage(const String& name) 
{
	Log.error() << name << endl << endl
		<< "This program is designed for clipping the protein structures around" <<endl
		<< "a ligand. The artificial chains that are created during that process" <<endl
		<< "are finalised by adding NME and ACE and caps and reconstructed parts" <<endl
		<< "of the protein will be structurally optimised by employing an AMBER" << endl << endl
		<< "forcefield." << endl
		<< "  -p <FILE>    use FILE as receptor (PDB format)" << endl
		<< "  -l <FILE>    use FILE as ligand (PDB format)" << endl
		<< "  -L <FILE>    use FILE as ligand (HIN format)" << endl
		<< "  -c <CUTOFF>  use CUTOFF (default: 8 A)" << endl;
		// << "  -o {yes,no}  optimize reconstructed atoms (default: yes)" << endl;          
}


// *Appends* sulphur bridges to the list
Size find_sulphur_bridges(const System& system,
		list< pair<const Residue*, const Residue*> >& sulphur_bridges)
{
	AtomConstIterator atom_it;
	AtomBondConstIterator bond_it;

	const Residue* residue1 = 0;
	const Residue* residue2 = 0;

	Size found = 0;

	BALL_FOREACH_BOND(system, atom_it, bond_it)
	{
		residue1 = bond_it->getFirstAtom()->getResidue();
		residue2 = bond_it->getSecondAtom()->getResidue();

		if ((bond_it->getFirstAtom()->getElement() == PTE[Element::S])
				&& (bond_it->getSecondAtom()->getElement() == PTE[Element::S])
				&& (residue1->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
				&& (residue2->hasProperty(Residue::PROPERTY__HAS_SSBOND) == true)
				&& (residue1 != residue2)
				&& (residue1 != 0)
				&& (residue2 != 0))
		{
			// cout << "Found sulphur bridge " << residue1->getID() << "---" 
			//	<< residue2->getID() << endl;
			sulphur_bridges.push_back(pair<const Residue*, const Residue*>(residue1, residue2));
			found++;
		}
	}

	return(found);

}


Residue* transform_residue_to_cap(PDBAtom& atom, FragmentDB& fragment_db)
{

	// define a proper name for reconstruction of the end cap
	Residue* new_residue = new Residue;
	new_residue->setProperty(Residue::PROPERTY__AMINO_ACID);
	if (new_residue == 0) return(0);

	if (atom.getName() == "N") 
	{
		new_residue->setName("NME");
	}
	else
	{
		if (atom.getName() == "C") 
		{
			new_residue->setName("ACE");
		}
		else
		{
			return(0);
		}
	}

	// copy anything that is connected to atom within this residue to a new
	// one
	PDBAtom* new_atom = new PDBAtom(atom);
	if (new_atom == 0) return(0);

	// Copy the anchor atom
	new_residue->insert(*new_atom);
	
	AtomBondIterator bond_it = atom.beginBond();
	for (; +bond_it; ++ bond_it)
	{
		PDBAtom* partner = (PDBAtom*) bond_it->getPartner(atom);
		if (partner->getResidue() == atom.getResidue())
		{
			// Copy the connected atoms
			PDBAtom* new_partner = new PDBAtom(*partner);
			if (new_partner == 0) return (0);
			if (new_partner->getName() == "CA")
			{
				new_partner->setName("CH3");
			}
			new_residue->insert(*new_partner);
		}
	}

	// apply a ReconstructFragment
	ReconstructFragmentProcessor reconstruct(fragment_db);
	new_residue->apply(reconstruct);
	new_residue->apply(fragment_db.build_bonds);
	new_residue->apply(fragment_db.normalize_names);

	// Select all reconstructed atoms for subsequent optimisation
	list<Atom*> reconstructed_atoms = reconstruct.getInsertedAtoms();
	list<Atom*>::iterator it = reconstructed_atoms.begin();
	for (; it != reconstructed_atoms.end(); ++it)
	{
		(*it)->select();
	}

	return(new_residue);
}


float get_residue_charge(const Residue& residue)
{
	float charge = 0.0f;
	AtomConstIterator atom_it = residue.beginAtom();
	for(; +atom_it; ++atom_it)
	{
		charge += atom_it->getCharge();
	}
	return(charge);
}


int main(int argc, char** argv)
{

	if (argc == 1)
	{
		usage(argv[0]);
		return 1;
	}

	// define all the variables for the UI

	String protein_file_name;
	String ligand_file_name;

	float cutoff = 8.0;

	bool use_hin_ligand = false;

	for (int i = 1; i < argc; i++)
	{
		// retrieve the current option
		String option(argv[i]);

		// check for leading "-" and length of argument
		if ((option[0] != '-') || (option.size() != 2))
		{
			// print usage hints and an error message, exit
			usage(argv[0]);
			Log.error() << "Illegal option: " << option << endl;
			return 2;
		}

		// check for another argument for those 
		// options requiring a filename (-h)
		if (String("cplL").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage(argv[0]);
			Log.error() << "Option " << option << " requires an additional argument." 
				<< endl;
			return 3;
		}		

		// interpret all command line options
		switch (option[1])
		{
			case 'c':
				cutoff = atoi(argv[++i]);
				break;

			case 'p':
				protein_file_name = argv[++i];
				break;

			case 'l':
				use_hin_ligand = false;
				ligand_file_name = argv[++i];
				break;

			case 'L':
				use_hin_ligand = true;
				ligand_file_name = argv[++i];
				break;

			default:		// unknown option
				// print usage hints and an error message, exit
				usage(argv[0]);
				Log.error() << "Illegal option: " << option << endl;
				return 2;
		}
	}

	// if we don't have a receptor and/or a ligand, we can't compute something
	if (protein_file_name == "")
	{
		Log.error() << "Missing protein file name." << endl;
		return 1;
	}
	if (ligand_file_name == "")
	{
		Log.error() << "Missing ligand file name." << endl;
		return 1;
	}

	// now load the molecules 
	System protein;
	PDBFile protein_file(protein_file_name);
	protein_file >> protein;
	protein_file.close();

	Log.info() << "Initializing fragment database." << endl;
	FragmentDB db;
	Log.info() << "Building bonds (protein)." << endl;
	protein.apply(db.build_bonds);
	Log.info() << "Normalizing names (protein)." << endl;
	protein.apply(db.normalize_names);

	System ligand;

	if (use_hin_ligand == true)
	{
		HINFile ligand_hin_file;
		ligand_hin_file.open(ligand_file_name);
		ligand_hin_file >> ligand;
		ligand_hin_file.close();

		// assign typenames
		// String typename_rule_filename = path.find("Amber/amber_types.rul");
		// if (typename_rule_filename == "")
		// {
		// 	cerr << "Could not find rule file " << std::endl;
		// }
		// INIFile typename_rules(typename_rule_filename);
		// typename_rules.read();
		// TypenameRuleProcessor proc(typename_rules, "TypenameRules");
		// ligand.apply(proc);
	}
	else
	{
		PDBFile ligand_pdb_file;
		ligand_pdb_file.open(ligand_file_name);
		ligand_pdb_file >> ligand;
		ligand_pdb_file.close();
		Log.info() << "Normalizing names (ligand)." << endl;
		ligand.apply(db.normalize_names);
		Log.info() << "Building bonds (ligand)." << endl;
		ligand.apply(db.build_bonds);
	}

	// Build a list of sulphur bridges

	list< pair<const Residue*, const Residue*> > sulphur_bridges;
	find_sulphur_bridges(protein, sulphur_bridges);

	// Find residues that have atoms which are less than cutoff \AA away from
	// any atom of the ligand

	ResidueIterator res_it = protein.beginResidue();
	PDBAtomIterator atom_it;
	AtomConstIterator lig_it;
	Vector3 position;
	float dist;
	HashSet<const Residue*> cut;
	ResidueIterator tmp_res_it;
	Index last_inserted_ID;

	float charge = 0.0f;

	for (; +res_it; ++res_it)
	{

		for (atom_it = res_it->beginPDBAtom(); +atom_it; ++atom_it)
		{
			position = atom_it->getPosition();
			for (lig_it = ligand.beginAtom(); +lig_it; ++lig_it)
			{
				dist = (position - lig_it->getPosition()).getLength();
				if (dist < cutoff)
				{
					if (!cut.has(&*res_it))
					{
						// distance is less than the cutoff, co add this residue to the
						// cut
						cut.insert(&*res_it);

						// if there is too short a gap between chain snippets, add
						// residue that lies inbetween to the cut.
						if ((res_it->getID().toInt() - 2) == last_inserted_ID)
						{
							tmp_res_it = res_it;
							tmp_res_it--;
							cut.insert(&*tmp_res_it);
						}
						last_inserted_ID = res_it->getID().toInt();
						break;
					}
				}
			}
			if (cut.has(&*res_it)) break;
		}
	}

	// Build a hash set containing all residues that still have sulphur
	// bridge partners after the cut
	HashSet<const Residue*> residues_with_sulphur_bridges;
	list< pair<const Residue*, const Residue*> >::const_iterator
		sulphur_bridges_iterator = sulphur_bridges.begin();
	for (; sulphur_bridges_iterator != sulphur_bridges.end();
			++sulphur_bridges_iterator)
	{
		const pair<const Residue*, const Residue*>& bridge = *sulphur_bridges_iterator;
		if (cut.has(bridge.first) && cut.has(bridge.second))
		{
			residues_with_sulphur_bridges.insert(bridge.first);
			residues_with_sulphur_bridges.insert(bridge.second);
		}
	}
	

	Protein* cut_protein = new Protein;
	Chain* chain;
	Residue* residue;
	Residue* tmp_res;
	Residue* last_residue = 0;

	// Iterate over all chains of the protein.
	// NB: This code asssumes that chains contain the proteins in correct
	// consecutive order!
	ChainIterator chain_it = protein.beginChain();
	ChainIterator tmp_chain_it;
	Index current_ID;

	ReconstructFragmentProcessor reconstruct(db);

	// iterate over all chains
	for (; +chain_it; ++chain_it)
	{
		chain = 0;
		for (res_it = chain_it->beginResidue(); +res_it; ++res_it)
		{
			// If the hash contains this residue, create a new chain and place a
			// copy of the residue in it.
			if (cut.has(&*res_it))
			{

				residue = new Residue(*res_it);

				// If a residue loses its sulphur bridge, add the missing H 
				if (residue->hasProperty(Residue::PROPERTY__HAS_SSBOND))
				{

					if (!residues_with_sulphur_bridges.has(&*res_it))
					{
						// Reconstruct  residues that lost their sulphur bridges
						residue->clearProperty(Residue::PROPERTY__HAS_SSBOND);
						residue->apply(reconstruct);
						residue->apply(db.build_bonds);
					}

					// Select all reconstructed atoms for subsequent optimisation
					list<Atom*> reconstructed_atoms = reconstruct.getInsertedAtoms();
					list<Atom*>::iterator it = reconstructed_atoms.begin();
					for (; it != reconstructed_atoms.end(); ++it)
					{
						(*it)->select();
					}

				}
				current_ID = res_it->getID().toInt();

				// if there was no new chain, create a new one with a methyl cap
				if (chain == 0)
				{
					// create a new chain for this fragment of the protein
					chain = new Chain;

					// if we are at the first residue we don't need any caps.
					if ((!res_it->isNTerminal())
							&& (res_it->hasProperty(Residue::PROPERTY__AMINO_ACID)))
					{

						if (res_it == chain_it->beginResidue())
						{
							cout << "Error. The first residue ("
								<< res_it->getFullName() << ") is not N-terminal." << endl;
							res_it->dump();
							exit(1);
						}

						// get a reference to the previous residue...
						tmp_res_it = res_it;
						tmp_res_it--;

						PDBAtomIterator atom_it = tmp_res_it->beginPDBAtom();
						for (; +atom_it; ++atom_it)
						{
							if (atom_it->getName() == "C") break;
						}

						if (atom_it == tmp_res_it->endPDBAtom())
						{
							Log.warn() << "Warning. Did not find C in peptide bond." << endl;
						}
						else
						{
							// ...and transform it into a cap.
							cout << "Transforming " << tmp_res_it->getFullName() << ":"
								<< tmp_res_it->getID() << " into ACE-N" << endl;
							tmp_res = transform_residue_to_cap(*atom_it, db);
							tmp_res->setID(String(current_ID - 1));
							chain->insert(*tmp_res);
						}
					}
				}

				chain->insert(*residue);
				last_residue = &*res_it;
				cout << "Inserting " << res_it->getFullName() << "-"
					<< res_it->getID() << endl;
			}
			else
			{
				// If we created a chain and the current residue is *not* in the
				// cut we have to finalise this chain.
				if (chain != 0)
				{
					// Only transform if the last residue was not terminal.
					if ((!last_residue->isCTerminal())
							&& (last_residue->hasProperty(Residue::PROPERTY__AMINO_ACID)))
					{
						// transform this residue to an end cap
						PDBAtomIterator atom_it = res_it->beginPDBAtom();
						for (; +atom_it; ++atom_it)
						{
							if (atom_it->getName() == "N") break;
						}

						if (atom_it == res_it->endPDBAtom())
						{
							Log.warn() << "Warning. Did not find N in peptide bond." << endl;
						}
						else
						{
							cout << "Transforming " << res_it->getFullName() << ":"
								<< res_it->getID() << " into NME-C" << endl;
							tmp_res = transform_residue_to_cap(*atom_it, db);
							tmp_res->setID(String(current_ID + 1));
							chain->insert(*tmp_res);
						}
					}

					// insert the finalised chain into the output protein.
					cut_protein->insert(*chain);
					chain = 0;
				}
			}
		}
	}

	ResidueChecker check(db);
	cut_protein->apply(check);

	System system;
	system.insert(*cut_protein);
	system.apply(db.build_bonds);

	Path path;

	cout << endl;
	cout << "Assigning charges (protein)." << endl;
	String charge_rule_file_name("/home/staff/anker/Interlec20/fresno/PARSE.rul.modified");
	String tmp = path.find(charge_rule_file_name);
	if (tmp != "") charge_rule_file_name = tmp;

	INIFile charge_ini(charge_rule_file_name);
	charge_ini.read();
	ChargeRuleProcessor charge_rules(charge_ini);
	cut_protein->apply(charge_rules);

	for (res_it = cut_protein->beginResidue(); +res_it; ++res_it)
	{
		float c = get_residue_charge(*res_it);
		cout << "Charge of " << res_it->getFullName() << ":" 
			<< res_it->getID() << " is " << c << endl;
		charge += c;
	}

	cout << "CHARGE=" << charge << endl;

	String parameter_file_name("Amber/amber94gly.ini");
	tmp = path.find(parameter_file_name);
	if (tmp == "") tmp = parameter_file_name;

	Log.info() << "Initializing force field" << endl;
	AmberFF amber_ff;
	amber_ff.options.set(AmberFF::Option::FILENAME, tmp);
	amber_ff.setup(system);

	PDBFile before_minimization("before_minimization.pdb", File::OUT);
	before_minimization << system;

	Log.info() << "Starting minimizer: " << endl << endl;
	ConjugateGradientMinimizer cgm(amber_ff);
	cgm.minimize(1000);

	PDBFile cut_protein_file("cut_protein_file.pdb", File::OUT);
	cut_protein_file << system;

	tmp_res = new Residue;
	tmp_res->setName("LIG");
	AtomIterator tmp_it = ligand.beginAtom();
	for (; +tmp_it; ++tmp_it)
	{
		PDBAtom* atom = new PDBAtom;
		atom->Atom::operator = (*tmp_it);
		tmp_res->insert(*atom);
	}

	Chain* tmp_chain = new Chain;
	tmp_chain->insert(*tmp_res);

	XYZFile cut_protein_file_xyz("cut_protein_file.xyz", File::OUT);
	cut_protein_file_xyz << system;

	cut_protein->insert(*tmp_chain);
	XYZFile cut_system_file_xyz("cut_system_file.xyz", File::OUT);
	cut_system_file_xyz << system;

	XYZFile ligand_file_xyz("ligand_file.xyz", File::OUT);
	ligand_file_xyz << ligand;

}

