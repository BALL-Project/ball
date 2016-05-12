// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/FORMAT/molFileFactory.h>
#include <BALL/FORMAT/dockResultFile.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <boost/unordered_map.hpp>

#include <openbabel/mol.h>
#include <openbabel/builder.h>
#include <openbabel/forcefield.h>
#include <openbabel/plugin.h>
#include <openbabel/obconversion.h>
#include "version.h"


using namespace BALL;
using namespace std;


// Check whether the given molecule really contains only one molecule
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


// Check if a molecule has atoms, is single connected component and has elements defined
bool basicMoleculeCheck(Molecule* mol)
{
	// Check if it molecule has no atoms
	if (mol->countAtoms() == 0)
	{
		return false;
	}
	
	// Check if molecule is a contains a single fragment
	set<const Atom*> visited_atoms;
	checkBonds(&*mol->beginAtom(), visited_atoms);
	if (visited_atoms.size() != mol->countAtoms())
	{
		return false;
	}
	
	// Check if there are undefined elements
	for (AtomConstIterator it=mol->beginAtom(); it!=mol->endAtom(); ++it)
	{
		if (it->getElement().getName() == BALL_ELEMENT_NAME_DEFAULT)
		{
			return false;
		}
	}
	
	return true;
}


// Basic check if a ligand contains a hydrogen and thus can be regarded as "protonated"
bool hasHydrogensCheck(Molecule* mol)
{
	for (AtomConstIterator it = mol->beginAtom(); +it; it++)
	{
		if (it->getElement().getSymbol() == "H")
		{
			return true;
		}
	}
	
	return false;
}


// Check if a valid molecule is ready for docking
bool advancedMoleculeCheck(Molecule* mol)
{
	bool all_x_zero = true;
	bool all_y_zero = true;
	bool all_z_zero = true;
	
	for (AtomConstIterator it = mol->beginAtom(); +it; it++)
	{
		if (fabs(it->getCharge()) > 5)
		{
			return false;
		}
		
		for (Atom::BondConstIterator b_it = it->beginBond(); +b_it; b_it++)
		{
			if (b_it->getLength() < 0.7 || b_it->getLength() > 2.5)
			{
				return false;
			}
		}
		
		const TVector3<float>& pos = it->getPosition();
		if (all_x_zero)
		{
			if (fabs(pos[0]) > 0.001)
			{
				all_x_zero = false;
			}
		}
		if (all_y_zero)
		{
			if (fabs(pos[1]) > 0.001)
			{
				all_y_zero = false;
			}
		}
		if (all_z_zero)
		{
			if (fabs(pos[2]) > 0.001)
			{
				all_z_zero = false;
			}
		}
	}
	
	return !(all_x_zero || all_y_zero || all_z_zero);
}


int main(int argc, char* argv[])
{
	CommandlineParser parpars("Ligand3DGenerator", "generate 3D coordinates for small molecules", VERSION, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "input file");
	parpars.registerMandatoryOutputFile("o", "output file");
	parpars.registerOptionalDoubleParameter("ph", "pH-value for pH-dep. protonation", 7.0);
	parpars.registerOptionalStringParameter("ff", "Forcefield to use for optimization (any available OpenBabel plugin)", "MMFF94");
	parpars.registerFlag("rm", "remove input file when finished");
	parpars.registerFlag("k", "keep existing 3D coordinates (flag precedes '-kp')");
	parpars.registerFlag("kp", "keep existing 3D coordinates but re-protonate compound in a pH dependent manner (flag is preceded by '-k')");
	String man = String("This tool takes input molecules and generates a single 3D conformation which is ready for docking. The input has to be a chemical file\n\
containing valid topologies and conistent bond order assignements. 2D coordinates in the input file are overwritten, 3D coordinates can be kept ('-k' or '-kp').\n\n\
  '-k':   3D coordinates are tried to be kept. Molecules with non-zero coordinates in all three dimensions are passed through without modifications. \n\
          However, if a molecule is not ready for docking (i.e. unusual bond leghts, unusual charges), the molecule will be rebuild and new \n\
          3D coordinates are assigned. If only hydrogens are missing, the coordinates of non-hydrogen atoms are kept but the molecule gets newly protonated.\n\
  '-kp':  3D coordinates are tried to be kept as for the '-k' flag but the molecule will be newly protonated.\n\n\
Please note that the main purpose of this tool is to generate valid starting conformations for docking or other optimization procedures.\n\
Therefore, the generated 3D coordinates for each fragment should be all right, but in extreme cases (i.e. very large and/or complex molecules) \n\
different fragments might still overlap with each other.\n\n\
Supported formats are ") + MolFileFactory::getSupportedFormats() + String(".");
	parpars.setToolManual(man);
	parpars.setSupportedFormats("i",MolFileFactory::getSupportedFormats());
	parpars.setSupportedFormats("o",MolFileFactory::getSupportedFormats());
	parpars.parse(argc, argv);
	
	// Create a dummy OBConversion object, that loads the available plugins.
	// If you know another, more elegant way to teach OpenBabel to do so
	// be my guest
	OpenBabel::OBConversion();

	GenericMolFile* input = MolFileFactory::open(parpars.get("i"), ios::in);
	GenericMolFile* output = MolFileFactory::open(parpars.get("o"), ios::out, input);
	DockResultFile* drf_output = dynamic_cast<DockResultFile*>(output);
	if (drf_output)
	{
		drf_output->setToolInfo(parpars.getStartCommand(), parpars.getStartTime());
	}
	
	double pH = 7.4;
	if (parpars.get("ph") != CommandlineParser::NOT_FOUND)
	{
		pH = parpars.get("ph").toDouble();
	}
	
	bool keep3Dcoords = false;
	if (parpars.has("k"))
	{
		keep3Dcoords = true;
	}
	
	bool keep3DcoordsProtonate = false;
	if (parpars.has("kp"))
	{
		keep3DcoordsProtonate = true;
	}
	
	std::string ff = parpars.get("ff");
	OpenBabel::OBPlugin* plugin = OpenBabel::OBPlugin::GetPlugin("forcefields", ff.c_str());

	if(!plugin)
	{
		std::vector<std::string> forcefields;
		OpenBabel::OBPlugin::ListAsVector("forcefields", ff.c_str(), forcefields);

		cerr << "[Error:] No OpenBabel plugin containing the specified force field could be found. "
		        "Available forcefields are:\n";

		for(std::vector<std::string>::iterator it = forcefields.begin(); it != forcefields.end(); ++it)
		{
			std::cerr << "\t" << *it << "\n";
		}

		cerr << "Please make sure that the plugin is installed and BABEL_LIBDIR is set accordingly."
		        "\nYo can verify this by typing \"obabel -L forcefields\"" << endl;

		return 1;
	}

	OpenBabel::OBForceField* pFF = dynamic_cast<OpenBabel::OBForceField*>(plugin);
	if (!pFF)
	{
		cerr << "[Error:] Could not convert the provided plugin to a OBForceField. This is most likely a problem with OpenBabel.\n"
		        "If you can rule this out, please report a bug on the BALL bugtracker." << std::endl;
		return 1;
	}
	
	String mol_name;
	bool setup_ok;
	bool ob_error;
	bool rebuild_mol = false;
	int no_written = 0;
	int no_ignored = 0;
	Molecule *mol, *new_mol;
	OpenBabel::OBMol *obmol;
	boost::unordered_map<unsigned int, OpenBabel::vector3> coord_backup;
	for (Size i=1; (mol = input->read()); i++)
	{
		if (no_written%5 == 0)
		{
			Log.level(20) << "\r" << no_written << " molecules";
			Log.flush();
		}
		
		mol_name = mol->getName();
		if (mol_name=="")
		{
			mol_name = "?";
		}
		
		// Check if molecule fulfills minimal validity criteria
		if (!basicMoleculeCheck(mol))
		{
			no_ignored++;
			
			Log.level(20) << "Error " << mol_name << " is skipped because of either containing no atoms, multiple fragments or undefined elements." << endl;
			
			delete mol;
			
			continue;
		}
		
		
		if (keep3Dcoords || keep3DcoordsProtonate)
		{
			if (advancedMoleculeCheck(mol))
			{
				rebuild_mol = false;
				
				if (keep3Dcoords)
				{
					if (hasHydrogensCheck(mol))
					{
						if (output->write(*mol)) 
						{
							no_written++;
						}
						else 
						{
							no_ignored++;
						}
						
						delete mol;
						
						continue;
					}
					
					
					Log.level(20) << "Molecule <" << mol_name << "> is newly protonated because of missing hydrogens (non-hydrogen atoms keep their 3D coordinates)." << endl;
				}
			}
			else
			{
				rebuild_mol = true;
				
				Log.level(20) << "Molecule <" << mol_name << "> is rebuild because of either missing 3D coordinates, unusual charges or unusual bond lengths." << endl;
			}
		}
		
		
		// Rebuild or re-protonate and minimze molecule using OpenBabel tools
		
		// Note: Openbabel::OpGen3D::Do() is NOT used directly here, because it re-assigns hydrogens for a static ph of 7.4, 
		// i.e. the pH can not be specified. Furthermore, it does also not remove hydrogens before using OBBuilder, resulting in hydrogens sometimes having more than one bond!
		obmol = MolecularSimilarity::createOBMol(*mol);
		
		ob_error = false;
		try
		{
			// Remove all hydrogens before using OBBuilder, otherwise hydrogens end up having more than one bond! (Sigh!)
			obmol->DeleteHydrogens();
			
			// Backup original atom coordinates
			coord_backup.clear();
			if (!rebuild_mol && keep3DcoordsProtonate)
			{
				for(OpenBabel::OBAtomIterator a_it=obmol->BeginAtoms(); a_it!=obmol->EndAtoms(); ++a_it)
				{
					coord_backup.insert(make_pair((*a_it)->GetIdx(), (*a_it)->GetVector()));
				}
			}
			
			
			// Create 3D coordinates for individual fragments using template library
			OpenBabel::OBBuilder builder;
			builder.Build(*obmol);
			obmol->SetDimension(3);
			
			
			// Set up a constraints object for OpenBabel minimization
			// It is used to set positional constraints on non-hydrogen atoms when only hydrogens should be minimized
			OpenBabel::OBFFConstraints constraints;
			
			
			// Reset original coordinates and set positional constraints on non-hydrogen atoms
			if (!rebuild_mol && keep3DcoordsProtonate)
			{
				for(OpenBabel::OBAtomIterator a_it=obmol->BeginAtoms(); a_it!=obmol->EndAtoms(); ++a_it)
				{
					(*a_it)->SetVector(coord_backup[(*a_it)->GetIdx()]);
					
					constraints.AddAtomConstraint((*a_it)->GetIdx());
				}
			}
			
			
			// Now add hydrogens
			obmol->AddHydrogens(false, true, pH);
			
			
			// Setup MMFF94 force-field
			setup_ok = pFF->Setup(*obmol, constraints);
			
			
			if (setup_ok)
			{
				// Optimize bond-lengths and torsions
				pFF->SteepestDescent(70, 1.0e-4); // 250, 1e-04
				
				// Rotate fragments around rotatable bonds
				pFF->WeightedRotorSearch(10, 5); // 200, 25
				
				// Optimize bond-lengths and torsions again
				pFF->ConjugateGradients(70, 1.0e-4); // 250, 1e-06
				pFF->UpdateCoordinates(*obmol);
			}
			else
			{
				Log.level(20) << "[Warning:] Openbabel force-field setup failed for molecule " << i << ". Will only use template-based approach without force-field optimization for this molecule!" << endl;
			}
		}
		catch (...)
		{
			Log.level(20) << "Error while trying to generate 3D coordinates for molecule " << i<< " ";
			if (mol->getName() != "")
			{
				Log.level(20) << "'" << mol->getName() << "'";
			}
			Log.level(20) << ". Will write unmodified input coordinates to output-file." << endl;
			
			delete obmol;
			
			ob_error = true;
			
			if (output->write(*mol))
			{
				no_written++;
			}
			else
			{
				no_ignored++;
			}
		}
		
		if (!ob_error)
		{
			// Fetch final 3D coordinates
			new_mol = MolecularSimilarity::createMolecule(*obmol);
			for (NamedPropertyIterator it=mol->beginNamedProperty(); it!=mol->endNamedProperty(); it++)
			{
				new_mol->setProperty(*it);
			}
			new_mol->setName(mol->getName());
			
			if (output->write(*new_mol)) 
			{
				no_written++;
			}
			else 
			{
				no_ignored++;
			}
			
			delete mol;
			delete obmol;
			delete new_mol;
		}
	}
	
	Log.level(20) << "\r";
	if (no_ignored > 0) 
	{
		Log.level(20) << "ignored " << no_ignored << " identical molecules!" << endl;
	}
	Log.level(20) << "wrote " << no_written << " molecules." << endl;
	
	input->close();
	output->close();
	
	delete input;
	delete output;
	
	if (parpars.has("rm"))
	{
		File::remove(parpars.get("i"));
	}
}

