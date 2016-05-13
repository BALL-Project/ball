// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
#include <BALL/STRUCTURE/disulfidBondProcessor.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/MOLMEC/AMBER/amber.h>
#include <BALL/MOLMEC/MINIMIZATION/steepestDescent.h>
#include <BALL/MOLMEC/MINIMIZATION/conjugateGradient.h>
#include <BALL/MOLMEC/COMMON/typenameRuleProcessor.h>
#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MATHS/common.h>

using namespace BALL;
using namespace std;

void usage(const String& name)
{
	Log.error() << name << endl << endl
		<< "This program is designed for clipping the protein structures around" <<endl
		<< "a ligand. The artificial chains that are created during that process" <<endl
		<< "are finalised by adding NME and ACE end caps and reconstructed parts" <<endl
		<< "of the protein will be structurally optimised by employing an AMBER" << endl << endl
		<< "forcefield." << endl
		<< "  -p <FILE>    use FILE as receptor (PDB format)" << endl
		<< "  -P <FILE>    use FILE as receptor (HIN format)" << endl
		<< "  -l <FILE>    use FILE as ligand (PDB format)" << endl
		<< "  -L <FILE>    use FILE as ligand (HIN format)" << endl
		<< "  -c <CUTOFF>  use CUTOFF (default: 8 A)" << endl
		<< "  -n <ATOMS>   the maximum number of atoms the cut system should have"
// 		<< "  -h <ATOMS>   the maximum number of heavy atoms the cut system should have"
//		<< "  -o <FORAMT>  Output format. Choose between pdb and xyz (default: xyz)"
		<< endl;
}


Size count_heavy_atoms(const AtomContainer& container)
{
	AtomConstIterator it(container.beginAtom());
	Size count = 0;
	for (; +it; ++it)
	{
		if (it->getElement() != PTE[Element::H]) count++;
	}
	return count;
}

Residue* transform_residue_to_cap(PDBAtom& atom, FragmentDB& fragment_db,
		list<Atom*>& reconstructed_atoms)
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
			delete new_residue;
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
			// Special case: proline. Don't copy CD.
			if (partner->getName() != "CD")
			{
				PDBAtom* new_partner = new PDBAtom(*partner);
				if (new_partner == 0) return (0);
				if (new_partner->getName() == "CA")
				{
					new_partner->setName("CH3");
				}
				// special case: proline
				if (new_partner->getName() == "CD")
				{
					new_partner->setName("CH3");
				}
				new_residue->insert(*new_partner);
			}
		}
	}

	// apply a ReconstructFragment
	ReconstructFragmentProcessor reconstruct(fragment_db);
	new_residue->apply(reconstruct);
	new_residue->apply(fragment_db.build_bonds);
	new_residue->apply(fragment_db.normalize_names);

	// save all reconstructed atoms for subsequent optimisation
	reconstructed_atoms = reconstruct.getInsertedAtoms();

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
	String output_format = "pdb";

	float cutoff = 8.0;
	Size max_atoms = 0;
	Size max_heavy_atoms = 0;

	bool use_hin_protein = false;
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
		if (String("cplLn").has(option[1]) && (i == (argc - 1)))
		{
			// pring usage hints, an error message, exit
			usage(argv[0]);
			Log.error() << "Option " << option << " requires an additional argument."	<< endl;
			return 3;
		}

		// interpret all command line options
		switch (option[1])
		{

			case 'c':
				cutoff = atof(argv[++i]);
				break;

			case 'n':
				max_atoms = atoi(argv[++i]);
				break;

			case 'h':
				max_heavy_atoms = atoi(argv[++i]);
				break;

			case 'p':
				protein_file_name = argv[++i];
				break;

			case 'P':
				use_hin_protein = true;
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

			case 'o':
				output_format = argv[++i];
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

	Log.info() << "Initializing fragment database." << endl;
	FragmentDB db("");

	// now load the molecules 
	System protein;

	if (use_hin_protein)
	{
		HINFile protein_file(protein_file_name);
		protein_file >> protein;
		protein_file.close();
	}
	else
	{
		PDBFile protein_file(protein_file_name);
		protein_file >> protein;
		protein_file.close();

		Log.info() << "Building bonds (protein)." << endl;
		protein.apply(db.build_bonds);
	}

	Log.info() << "Normalizing names (protein)." << endl;
	protein.apply(db.normalize_names);

	System ligand;

	if (use_hin_ligand)
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

	// initialize atom boundaries
	if (max_atoms == 0) max_atoms = protein.countAtoms() + ligand.countAtoms();
	if (max_heavy_atoms == 0) max_heavy_atoms = max_atoms;

	// Build a list of sulphur bridges	
	DisulfidBondProcessor dbp;
	protein.apply(dbp);
	DisulfidBondProcessor::DisulfidBonds sulphur_bridges = dbp.getDisulfidBonds();

	// Find residues that have atoms which are less than cutoff \AA away from
	// any atom of the ligand

	ResidueIterator res_it;
	PDBAtomIterator atom_it;
	AtomConstIterator lig_it;
	Vector3 position;
	float dist;
	HashSet<const Residue*> cut;
	ResidueIterator tmp_res_it;
	Index last_inserted_ID;

	float charge = 0.0f;
	Size atoms = 0;
	Size heavy_atoms = 0;

	// For calculating the number of atoms our cut system will contain, we
	// need to know:
	// ACE-N residues contain 6 atoms (4 light, 2 heavy)
	// NME-C residues contain 6 atoms (4 light, 2 heavy)

	do {
		cut.clear();
		last_inserted_ID = 65000;
		// Initialize the atom count with the number of atoms of the ligand and
		// an additional 12 atoms for the end-caps of the last chain (which
		// will not be added by the for-loop)
		atoms = ligand.countAtoms() + 12;
		heavy_atoms = count_heavy_atoms(ligand) + 4;
		// DEBUG
		//Log.info() << "init: a: " << atoms << ", h: " << heavy_atoms << endl;
		// /DEBUG

		res_it = protein.beginResidue();
		for (; +res_it && (atoms <= max_atoms) && (heavy_atoms <= max_heavy_atoms);
				++res_it)
		{
			atom_it = res_it->beginPDBAtom();
			for (; +atom_it && (atoms <= max_atoms) && (heavy_atoms <= max_heavy_atoms);
						++atom_it)
			{
				position = atom_it->getPosition();
				lig_it = ligand.beginAtom();
				for (; +lig_it && (atoms <= max_atoms) && (heavy_atoms <= max_heavy_atoms); ++lig_it)
				{
					dist = (position - lig_it->getPosition()).getLength();
					if (dist < cutoff)
					{

						if (!cut.has(&*res_it))
						{
							// Add 12 atoms for two end-caps if the last inserted is
							// further away tha two residues.
							if ((res_it->getID().toInt() - last_inserted_ID) > 2)
							{
								atoms += 12;
								heavy_atoms += 4;
								// DEBUG
								//Log.info() << "caps: a: " << atoms << ", h: " << heavy_atoms << endl;
								// /DEBUG
							}

							// If the residue is terminal, subtract 6 atoms from those 12
							// because the ACE-N or NME-C residue will not be added.
							if ((res_it->isNTerminal() || res_it->isCTerminal())
									&& (res_it->hasProperty(Residue::PROPERTY__AMINO_ACID)))
							{
								atoms -= 6;
								heavy_atoms -= 2;
								// DEBUG
								//Log.info() << "term: a: " << atoms << ", h: " << heavy_atoms << endl;
								// /DEBUG
							}

							// distance is less than the cutoff, co add this residue to the
							// cut
							atoms += res_it->countAtoms();
							heavy_atoms += count_heavy_atoms(*res_it);
							// DEBUG
							//Log.info() << "res:  a: " << atoms << ", h: " << heavy_atoms 
							//	<< " (" << res_it->getFullName() << ":"
//   								<< res_it->getID() << ")" << endl;
							if (atoms > max_atoms || heavy_atoms > max_heavy_atoms)
							{
								cutoff -= 0.25;
								Log.info() << "adding " << res_it->getFullName() << ":"
									<< res_it->getID() << " breaks the bounds: "
									<< atoms << "/" << max_atoms << ", "
									<< heavy_atoms << "/" << max_heavy_atoms
									<< ", setting cutoff to "
									<< cutoff << endl;
							}
							cut.insert(&*res_it);

							// if there is too short a gap between chain snippets, add
							// residue that lies inbetween to the cut.
							if ((res_it->getID().toInt() - 2) == last_inserted_ID)
							{
								tmp_res_it = res_it;
								tmp_res_it--;
								atoms += tmp_res_it->countAtoms();
								heavy_atoms += count_heavy_atoms(*tmp_res_it);
								// DEBUG
								//Log.info() << "gap:  a: " << atoms << ", h: " << heavy_atoms 
								//	<< " (" << tmp_res_it->getFullName() << ":"
								//	<< tmp_res_it->getID() << ")" << endl;
								// /DEBUG
								if (atoms > max_atoms || heavy_atoms > max_heavy_atoms)
								{
									cutoff -= 0.25;
									Log.info() << "adding " << tmp_res_it->getFullName() << ":"
										<< tmp_res_it->getID() << " breaks the bounds: "
										<< atoms << "/" << max_atoms << ", "
										<< heavy_atoms << "/" << max_heavy_atoms
										<< ", setting cutoff to "
										<< cutoff << endl;
									break;
								}
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
	} while ((max_atoms != 0)
			&& (atoms > max_atoms || heavy_atoms > max_heavy_atoms));

	if (cut.size() == 0)
	{
	}

	// Build a hash set containing all residues that still have sulphur
	// bridge partners after the cut
	HashSet<const Residue*> residues_with_sulphur_bridges;
	DisulfidBondProcessor::DisulfidBonds::iterator bond_it = sulphur_bridges.begin();
	for ( ; bond_it != sulphur_bridges.end();  ++bond_it)
	{
		if (cut.has(bond_it->first) && cut.has(bond_it->second))
		{
			residues_with_sulphur_bridges.insert(bond_it->first);
			residues_with_sulphur_bridges.insert(bond_it->second);
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
	Index current_ID = 0;

	ReconstructFragmentProcessor reconstruct(db);
	list<Atom*> reconstructed_atoms;

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
						// Reconstruct residues that lost their sulphur bridges
						residue->clearProperty(Residue::PROPERTY__HAS_SSBOND);
						residue->apply(reconstruct);
						residue->apply(db.build_bonds);
					}

					// Select all reconstructed atoms for subsequent optimisation
					list<Atom*> tmp_list = reconstruct.getInsertedAtoms();
					list<Atom*>::iterator it = tmp_list.begin();
					for (; it != tmp_list.end(); ++it)
					{
						reconstructed_atoms.push_back(*it);
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
							list<Atom*> tmp_list;
							tmp_res = transform_residue_to_cap(*atom_it, db, tmp_list);
							list<Atom*>::iterator it = tmp_list.begin();
							for (; it != tmp_list.end(); ++it)
							{
								reconstructed_atoms.push_back(*it);
							}
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
							list<Atom*> tmp_list;
							tmp_res = transform_residue_to_cap(*atom_it, db, tmp_list);
							list<Atom*>::iterator it = tmp_list.begin();
							for (; it != tmp_list.end(); ++it)
							{
								reconstructed_atoms.push_back(*it);
							}
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

	System system;
	system.insert(*cut_protein);
	system.apply(db.build_bonds);

	PDBFile intermediate("intermediate.pdb", std::ios::out);
	intermediate << system;
	intermediate.close();

	File dumpfile("intermediate.dump", std::ios::out);
	system.dump(dumpfile);
	dumpfile.close();

	Path path;

	cout << endl;

	String parameter_file_name("Amber/amber94gly.ini");
	// String parameter_file_name("Amber/amber96.ini");
	String tmp = path.find(parameter_file_name);
	if (tmp == "") tmp = parameter_file_name;

	Log.info() << "Initializing force field" << endl;
	AmberFF amber_ff;
	amber_ff.options.set(AmberFF::Option::FILENAME, tmp);
	amber_ff.options.setBool(AmberFF::Option::OVERWRITE_TYPENAMES, true);
	amber_ff.options.setBool(AmberFF::Option::OVERWRITE_CHARGES, true);
	amber_ff.setup(system);

	// Print out charges
	for (res_it = cut_protein->beginResidue(); +res_it; ++res_it)
	{
		float c = get_residue_charge(*res_it);
		cout << "Charge of " << res_it->getFullName() << ":" 
			<< res_it->getID() << " is " << c << endl;
		charge += c;
	}

	cout << "CHARGE=" << Maths::round(charge) << endl;

	ResidueChecker check(db);
	cut_protein->apply(check);

	// Now select all those atoms that have been inserted into the system in
	// order to keep the rest of the protein rigid.
	list<Atom*>::iterator list_it = reconstructed_atoms.begin();
	for (; list_it != reconstructed_atoms.end(); ++list_it)
	{
		(*list_it)->select();
	}

	dumpfile.open("before_minimization.dump", std::ios::out);
	system.dump(dumpfile);
	dumpfile.close();

	PDBFile before_minimization("before_minimization.pdb", std::ios::out);
	before_minimization << system;

	Log.info() << "Starting minimizer: " << endl << endl;
	SteepestDescentMinimizer sdm(amber_ff);
	sdm.minimize(100);
	ConjugateGradientMinimizer cgm(amber_ff);
	cgm.minimize(1000);

	PDBFile cut_protein_file("cut_protein_file.pdb", std::ios::out);
	cut_protein_file << system;

	dumpfile.open("after_minimization.dump", std::ios::out);
	system.dump(dumpfile);
	dumpfile.close();

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

	XYZFile cut_protein_file_xyz("cut_protein_file.xyz", std::ios::out);
	cut_protein_file_xyz << system;

	cut_protein->insert(*tmp_chain);
	XYZFile cut_system_file_xyz("cut_system_file.xyz", std::ios::out);
	cut_system_file_xyz << system;
	Log.info() << "# atoms in cut system: " << system.countAtoms() << endl;
	Log.info() << "# heavy atoms in cut system: " << count_heavy_atoms(system) << endl;
	// DEBUG
	// Log.info() << "computed # atoms in cut system: " << atoms << endl;
	// Log.info() << "computed # heavy atoms in cut system: " << heavy_atoms << endl;
	// /DEBUG

	XYZFile ligand_file_xyz("ligand_file.xyz", std::ios::out);
	ligand_file_xyz << ligand;

}

