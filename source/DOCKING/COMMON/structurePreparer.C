// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/DOCKING/COMMON/structurePreparer.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/QSAR/partialChargeProcessor.h>
#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/MOLMEC/PARAMETER/templates.h>
#include <BALL/MOLMEC/PARAMETER/lennardJones.h>
#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <BALL/STRUCTURE/buildBondsProcessor.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Parameters.h>
#include <BALL/DATATYPE/options.h>
#include <BALL/SCORING/COMPONENTS/PLP.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

namespace BALL
{
	StructurePreparer::StructurePreparer(String type, bool ignore)
	{
		use_mmff94_ = 0;
		use_PLP_ = 0;
		if (type == "MMFF94") use_mmff94_ = 1;
		else if (type == "PLP") use_PLP_ = 1;
		ignore_unassigned_ = ignore;
		simple_atom_types_.insert("C");
		simple_atom_types_.insert("H");
		simple_atom_types_.insert("N");
		simple_atom_types_.insert("O");
		simple_atom_types_.insert("P");
		simple_atom_types_.insert("S");
	}

	void StructurePreparer::setScoringType(String type)
	{
		if (type == "MMFF94") use_mmff94_ = 1;
		else if (type == "PLP") use_PLP_ = 1;
	}


	void StructurePreparer::setSimpleAtomTypes(set<String>& new_types)
	{
		simple_atom_types_ = new_types;
	}

	const set<String>* StructurePreparer::getSimpleAtomTypes()
	{
		return &simple_atom_types_;
	}


	void StructurePreparer::assignPLPAtomTypes(AtomContainer* at)
	{
		for (AtomIterator atom_it = at->beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			int type = PLP::getAtomType(&*atom_it);
			atom_it->setType(type);
			atom_it->setTypeName(String(type));
		}
	}


	void StructurePreparer::assignMMFF94AtomTypes(AtomContainer* at)
	{
		Options o;
		o.setDefaultBool("overwrite_charges", 0);
		o.setDefaultBool("overwrite_non-zero_charges", 0);
		MMFF94 m(*(System*)at, o);

		for (AtomIterator atom_it = at->beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			cout<<atom_it->getFullName()<<"  "<<atom_it->getType()<<"  "<<atom_it->getCharge()<<endl;
		}
	}


	void StructurePreparer::assignSimpleAtomTypes(AtomContainer* at, String iniFile)
	{
		//String folder(path.find("MMFF94"));
		//String prefix = folder + FileSystem::PATH_SEPARATOR;
		//MMFF94AtomTypes mmff_types;
		//mmff_types.readParameters(prefix + "MMFFPROP.PAR");

		if (use_mmff94_)
		{
			cout<<"sorry, this function does not (yet) work for MMFF94!!"<<endl;
		}


		Path path;
		String fullpath = path.find(iniFile);
		if (fullpath == "")
		{
			throw BALL::Exception::FileNotFound(__FILE__, __LINE__, iniFile);
		}
		ForceFieldParameters par;
		par.setFilename(fullpath); par.init();
		AtomTypes ff_types = par.getAtomTypes();

		for (AtomIterator atom_it = at->beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			String element = atom_it->getElement().getSymbol();
			if (element == "" || element == "?")
			{
				String mess = "Atom without an element found!";
				String n="StructurePreparer::assignSimpleAtomTypes() error";
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, n, mess);
			}

			if (simple_atom_types_.find(element) == simple_atom_types_.end())
			{
				simple_atom_types_.insert(element);
				//cout<<"added \'"<<element<<"\' to the simple atom types"<<endl;
			}
			atom_it->setTypeName(element);
			int type = ff_types.getType(element);
			if (type != -1)
			{
				atom_it->setTypeName(element);
				atom_it->setType(type);
			}
			else
			{
				String mess = "No AtomType for \'"+element+"\' defined in the given parameter file!!";
				String n="StructurePreparer::assignSimpleAtomTypes() : AtomType assignment error while reading file ";
				n = n+iniFile;
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, n, mess);
			}
		}
	}


	void StructurePreparer::assignAtomTypes(AtomContainer* at, String iniFile, String AtomType_file)
	{
		int unassigned_types = 0;
		int atoms = 0;

		Path path;
		String fullpath = path.find(iniFile);
		if (fullpath == "")
		{
			throw BALL::Exception::FileNotFound(__FILE__, __LINE__, iniFile);
		}
		ForceFieldParameters par;
		par.setFilename(fullpath);
		AtomTypes atom_types;
		Templates templ;
		if (!use_mmff94_)
		{
			par.init();
			atom_types = par.getAtomTypes();
			templ.extractSection(par, "ChargesAndTypeNames");
		}

		/** use MMFF94-method to try to assign types */
	// 	else
	// 	{
	// 		MMFF94 m;
	// 		AtomContainer at_copy = *at;
	// 		Molecule mol; mol.insert(at_copy); // removes atoms from at_copy (so do not insert 'at' instead!)
	// 		System s; s.insert(mol);
	// 		Options o;
	// 		o.setDefaultBool("overwrite_charges",0);
	// 		o.setDefaultBool("overwrite_non-zero_charges",0);
	// 		m.assignAtomTypes(&s);
	//
	// 		*at = *s.getMolecule(0);
	// 	}


		///  read AtomType-file
		ifstream atomType_input(AtomType_file.c_str());
		bool atomType_input_exists = 0;
		if (AtomType_file != "")
		{
			if (atomType_input)
			{
				atomType_input_exists = 1;
			}
			else
			{
				Log.warn()<<"StructurePreparer: given atom-type file does not exist!"<<endl;
			}
		}
		bool found_comp = 0;
		String tmp;
		String sb="[";
		if (atomType_input_exists)
		{
			while (!found_comp && atomType_input)
			{
				atomType_input >> tmp;
				tmp = tmp.trim();
				if (tmp[0] == sb && tmp.substr(1, tmp.size()-2) == at->getName())
				{
					found_comp = 1;
				}
			}
			if (!found_comp)
			{
				Log.warn()<<"No section for compound "<<at->getName()<<" found in atom-type file \'"<<AtomType_file<<"\' , trying to use element symbol instead.."<<endl;
			}
		}
		if (atomType_input_exists && !found_comp)
		{
			Log.error()<<"Found no section for molecule \'"<<at->getName()<<"\' in atom-type file \'"<<AtomType_file<<"\' !!"<<endl;
		}

		cout<<"#atoms= "<<at->countAtoms()<<endl;
		for (AtomIterator atom_it = at->beginAtom(); !atom_it.isEnd(); ++atom_it)
		{
			atoms++;

			// if type has already been set by MMFF94
			if (atom_it->getType() != -1 && atom_it->getType() != 0)
			{
				continue;
			}


			String name;
			if (found_comp) // read typenames from tab seperated test file
			{
				atomType_input >> name;
			}
			else
			{
				name = atom_it->getTypeName();
			}
			/// use typename (for cases in which typename is set but not type-id; e.g. after using antechamber to write structure files containing amber atom type names)
			if (name != "" && name != "?" && name != "DU")
			{
				Atom::Type type = atom_types.getType(name);
				if (type != Atom::UNKNOWN_TYPE)
				{
					atom_it->setType(type);
					atom_it->setTypeName(name);
					continue;
				}

				// fallback ; uses similar atomType-names instead
				bool found = 0;
				String name0 = name;
				while (name.size() > 1)
				{
					name = name.getSubstring(0, name.size()-1);
					type = atom_types.getType(name);
					if (type != Atom::UNKNOWN_TYPE)
					{
						Log.warn()<<"AtomType for "<<atom_it->getFullName()<<" ("<<name0<<") not found in \'"<<iniFile<<"\', using "<<name<<" instead"<<endl;
						atom_it->setType(type);
						atom_it->setTypeName(name);
						found = 1;
						break;
					}
				}
				if (found)
				{
					continue;
				}
			}

			/// use atom name
			Atom::Type type = atom_types.getType(atom_it->getName());
			if (type != Atom::UNKNOWN_TYPE)
			{
				atom_it->setType(type);
				atom_it->setTypeName(atom_it->getName());
				continue;
			}

			/// use templates for pdb amino acid atom types
			if (!use_mmff94_)
			{
				name = templ.getTypeName(atom_it->getFullName());
				if (name != "" &&  (type = atom_types.getType(name)) != Atom::UNKNOWN_TYPE)
				{
					atom_it->setType(type);
					atom_it->setTypeName(name);
					continue;
				}
			}

			/// use substring of atom name as fallback if full name does not exist in parameter file; e.g. "CE" instead of "CE1"
			name = atom_it->getName();
			String name0 = name;
			while (name.size() > 1)
			{
				name = name.getSubstring(0, name.size()-1);
				type = atom_types.getType(name);
				if (type != Atom::UNKNOWN_TYPE)
				{
					Log.warn()<<"AtomType for "<<atom_it->getFullName()<<" ("<<atom_it->getTypeName()<<") not found in \'"<<iniFile<<"\', using "<<name<<" instead"<<endl;
					atom_it->setType(type);
					atom_it->setTypeName(name);
					break;
				}
			}

			if (!ignore_unassigned_ && type == Atom::UNKNOWN_TYPE)
			{
				String ex="No AtomType for "+atom_it->getFullName()+" ("+name0+") found!!";
				String n="StructurePreparer::assignAtomTypes() : AtomType assignment error while reading file ";
				n = n+iniFile;
				throw BALL::Exception::GeneralException(__FILE__, __LINE__, n, ex);
			}
			else if (ignore_unassigned_)
			{
				Log.error()<<"No AtomType for "+atom_it->getFullName()+" ("+name0+") found!!"<<endl;
				unassigned_types++;
			}
		}

		cout<<unassigned_types<<" out of "<<atoms<<" AtomTypes could not be assigned!!"<<endl;
	}


	// sets the charge of an unbonded metal atom according to its most frequent oxidation state
	// return true if a charge was assigned; else false.
	bool StructurePreparer::setIonStandardCharge(Atom* atom)
	{
		if (atom->countBonds() > 0)
		{
			return false;
		}
		String element = atom->getElement().getSymbol();
		if (element == "Na" || element == "K")
		{
			atom->setCharge(1);
			return true;
		}
		if (element == "Mg" || element == "Ca" || element == "Mn" || element == "Zn")
		{
			atom->setCharge(2);
			return true;
		}
		if (element == "Fe")
		{
			atom->setCharge(3);
			return true;
		}
		if (element == "Cl" || element == "I" || element == "Br" || element == "F")
		{
			atom->setCharge(-1);
			return true;
		}

		return false;
	}


	// overwrite_mode: 0=no overwriting of charges, 1=overwrite all charges,
	//  2=automatic (overwrite all charges only if most atoms do not have a charge yet)
	void StructurePreparer::assignCharges(AtomContainer* at, int overwrite_mode)
	{
		if (overwrite_mode != 1)
		{
			int existing_charges = 0;
			int no_atoms = 0;
			for (AtomIterator atom_it = at->beginAtom(); atom_it != at->endAtom(); ++atom_it)
			{
				if (fabs(atom_it->getCharge()) > 1e-10) existing_charges++;
				no_atoms++;
			}
			if (overwrite_mode == 0 && existing_charges > 0)	return; // there are charges already, so nothing should be done
			if (((double)existing_charges)/no_atoms > 0.5) return; // most atoms do already have partial charges, so nothing should be done
		}

		PartialChargeProcessor pcp;
		pcp.calculatePEOE(*at);

		int ions = 0;

		for (AtomIterator atom_it = at->beginAtom(); atom_it != at->endAtom(); ++atom_it)
		{
			if (!setIonStandardCharge(&*atom_it))
			{
				double charge = atom_it->getProperty("PEOEPartialCharge").getDouble();
				atom_it->setCharge(charge);
			}
			else ions++;
		}
		if (ions > 0) Log.level(10)<<"Set "<<ions<<" ion charges."<<endl;
	}


	// overwrite_mode: 0=no overwriting of charges, 1=overwrite all charges,
	//  2=automatic (overwrite all charges only if most atoms do not have a charge yet)
	bool StructurePreparer::assignProteinCharges(AtomContainer* at, String parameterFile, int overwrite_mode)
	{
		if (overwrite_mode != 1)
		{
			int existing_charges = 0;
			int no_atoms = 0;
			for (AtomIterator atom_it = at->beginAtom(); atom_it != at->endAtom(); ++atom_it)
			{
				if (fabs(atom_it->getCharge()) > 1e-10) existing_charges++;
				no_atoms++;
			}
			if (overwrite_mode == 0 && existing_charges > 0)	return 1; // there are charges already, so nothing should be done
			if (((double)existing_charges)/no_atoms > 0.5) return 1; // most atoms do already have partial charges, so nothing should be done
		}

		System* sys = dynamic_cast<System*>(at);
		Protein* protein = dynamic_cast<Protein*>(at);

		if (!protein && sys) protein = sys->getProtein(0);
		if (protein)
		{
			Log.setMaxLevel(cout, 100000);
			Log.remove(cerr); // do not send any messages to error-stream during use of FragmentDB
			FragmentDB fdb("fragments/Fragments.db");
			protein->apply(fdb.normalize_names);
			Log.setMaxLevel(cout, 1999);
			Log.insert(cerr, 2000);
		}
		else return false;

		Path path;
		String fullpath = path.find(parameterFile);
		if (fullpath == "")
		{
			throw BALL::Exception::FileNotFound(__FILE__, __LINE__, parameterFile);
		}
		ForceFieldParameters par;
		par.setFilename(fullpath);
		par.init();
		Templates templ;
		bool ok = templ.extractSection(par, "ChargesAndTypeNames");
		if (!ok) return false;
		bool overwrite_charges = true;
		System* system = dynamic_cast<System*>(at);
		Size max_unassigned_atoms = system->countAtoms()/10;

		if (system)
		{
			Log.warn().disableOutput();
			templ.assignCharges(*system, overwrite_charges);
			Log.warn().enableOutput();


			// If charges for too many atoms could not be assigned, return false.
			// In that case, PEOE will be used for *all* atoms of the entire protein.
			if (templ.getNumberOfUnassignedAtoms() > max_unassigned_atoms) return 0;

			// In case of a small number of atoms ( < 10%) for which no partial charges could be extracted from the template file, use PEOE only for those residues that contain such atoms.
			HashSet<const Atom*>& unassigned_atoms = templ.getUnassignedAtoms();
			if (unassigned_atoms.size() > 0)
			{
				String peoe_residues="";
				for (ResidueIterator rit = protein->beginResidue(); +rit; rit++)
				{
					bool residue_has_unassigned_atoms = 0;
					for (AtomIterator ait = rit->beginAtom(); +ait; ait++)
					{
						if (unassigned_atoms.find(&*ait) != unassigned_atoms.end())
						{
							residue_has_unassigned_atoms = true;
							break;
						}
					}
					if (residue_has_unassigned_atoms)
					{
						peoe_residues += rit->getName()+rit->getID()+" ";
						assignCharges(&*rit, 1); // overwrite useless template charges here ..
					}
				}

				if (peoe_residues != "")
				{
					Log.level(10)<<"Using PEOE for the following residues because template-based assignment could not set partial charges for some of its atoms: "<<peoe_residues<<endl<<endl;
				}
			}
		}
		else return false;

		// make sure to assign charges to metal-ions included in the pdb-structure
		Size ions = 0;
		for (AtomIterator atom_it = at->beginAtom(); atom_it != at->endAtom(); ++atom_it)
		{
			if (overwrite_mode == 1 || fabs(atom_it->getCharge()) < 1e-10)
			{
				if (setIonStandardCharge(&*atom_it)) ions++;
			}
		}
		if (ions > 0) Log.level(10)<<"Set "<<ions<<" ion charges."<<endl;

		return 1;
	}


	void StructurePreparer::buildBonds(AtomContainer* at)
	{
		BuildBondsProcessor bbp;
		bbp(*at);
	}


	void StructurePreparer::saturate(AtomContainer* at)
	{
		RingPerceptionProcessor rpp;
		vector<vector<Atom*> > rings;
		rpp.calculateSSSR(rings, *at);
		rings = rpp.getAllSmallRings();

		AddHydrogenProcessor ahp;
		ahp.setRings(rings);
		at->apply(ahp);
	}


	void StructurePreparer::selectHeavyAtoms(AtomContainer* at)
	{
		for (AtomIterator atom_it = at->beginAtom(); +atom_it; ++atom_it)
		{
			if (atom_it->getElement().getName() != "Hydrogen")
			{
				atom_it->select();
			}
			else
			{
				atom_it->deselect();
			}
		}
	}


	void StructurePreparer::prepare(AtomContainer* at, String parameterFile, String /*AtomType_file*/)
	{
		//saturate(at); // < -- using BALL-methods to add Hydrogens unfortunately never works correctly, so better do not use it (use Corina resp. Reduce instead!)

		buildBonds(at);

		bool temp_charges_assigned = 0;
		System* sys = dynamic_cast<System*>(at);
		if ((sys && sys->countProteins() > 0) || dynamic_cast < Protein* > (at))
		{
			temp_charges_assigned = assignProteinCharges(at, parameterFile, 2);
			if (!temp_charges_assigned)
			{
				Log.level(15)<<"Assigning template based protein charges failed, will use PEOE instead."<<endl;
				assignCharges(at, 1);
			}
		}
		else
		{
			assignCharges(at, 2);
		}

		//assignAtomTypes(at, parameterFile, AtomType_file);
		if (!use_PLP_ && !use_mmff94_)
		{
			assignSimpleAtomTypes(at, parameterFile);
		}
		else if (use_PLP_)
		{
			assignPLPAtomTypes(at);
		}
		else if (use_mmff94_)
		{
			assignMMFF94AtomTypes(at);
		}
	}
}
