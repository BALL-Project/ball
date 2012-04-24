// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBase.C,v 1.2.28.2 2007-03-19 21:43:47 bertsch Exp $
//

#include <BALL/QSAR/simpleBase.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/smartsMatcher.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/path.h>

using namespace std;

#define BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE "/atomic_polarizabilities.data"

namespace BALL
{
	SimpleBase::SimpleBase()
		:	Descriptor()
	{
		data_folder_ = "QSAR/";
	}

	SimpleBase::SimpleBase(const SimpleBase& sb)
		:	Descriptor(sb)
	{
		data_folder_ = "QSAR/";
	}

	SimpleBase::SimpleBase(const String& name)
		:	Descriptor(name)
	{
		data_folder_ = "QSAR/";
	}

	SimpleBase::SimpleBase(const String& name, const String& unit)
		:	Descriptor(name, unit)
	{
		data_folder_ = "QSAR/";
	}

	SimpleBase::~SimpleBase()
	{
	}
	
	void SimpleBase::setDataFolder(const char* folder)
	{
		data_folder_ = folder;
	}	

	SimpleBase& SimpleBase::operator = (const SimpleBase& sb)
	{
		setUnit(sb.getUnit());
		setName(sb.getName());
		data_folder_ = sb.data_folder_;
		return *this;
	}

	double SimpleBase::calcPrincipalMomentOfInertia_(double& pmi_x, double& pmi_y, double& pmi_z, AtomContainer& ac)
	{
		Vector3 sum(0.0, 0.0, 0.0);
		double sum_weight = 0;
		for (AtomConstIterator i=ac.beginAtom();i!=ac.endAtom();++i)
		{
			double weight = i->getElement().getAtomicWeight();
			Vector3 pos = i->getPosition();
			Vector3 tmp(pos[0]*weight, pos[1]*weight, pos[2]*weight);
			sum += tmp;
			sum_weight += weight;
		}
		
		Vector3 mid(sum[0] / sum_weight, sum[1] / sum_weight, sum[2] / sum_weight);
		double pmi = 0;
		
		for (AtomConstIterator i = ac.beginAtom(); i != ac.endAtom(); ++i)
		{
			double weight = i->getElement().getAtomicWeight();
			Vector3 atom = i->getPosition();
			pmi_x += pow(atom[0] - mid[0], 2) * weight;
			pmi_y += pow(atom[1] - mid[1], 2) * weight;
			pmi_z += pow(atom[2] - mid[2], 2) * weight;
			pmi += atom.getSquareDistance(mid) * weight;
		}
		return pmi;
	}


	bool SimpleBase::isValid_(AtomContainer& ac)
	{
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = ac.getModificationTime();
		Handle mol_handle = ac.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				return false;
			}
		}
		else
		{
			mod_times.insert(std::make_pair(mol_handle, last_mod));
			return false;
		}
	}

	void SimpleBase::computeAllDescriptors(AtomContainer& ac)
	{	
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
	}
	
	void SimpleBase::calculate_(AtomContainer& ac)
	{
		// ring processing
		vector<vector<Atom*> > sssr;
		RingPerceptionProcessor rpp;
		Size size_of_sssr = rpp.calculateSSSR(sssr, ac);

		// the size of the SSSR
		ac.setProperty("SizeOfSSSR", (double)size_of_sssr);
			
		// aromatic processing
		AromaticityProcessor aro;
		aro.aromatize(sssr, ac);
	
		// pmi calculator
		double pmi(0), pmi_x(0), pmi_y(0), pmi_z(0);
		pmi = calcPrincipalMomentOfInertia_(pmi_x, pmi_y, pmi_z, ac);
		ac.setProperty("PrincipalMomentOfInertia", pmi);
		ac.setProperty("PrincipalMomentOfInertiaX", pmi_x);
		ac.setProperty("PrincipalMomentOfInertiaY", pmi_y);
		ac.setProperty("PrincipalMomentOfInertiaZ", pmi_z);

		// 1. part counts atoms (not counting aromatic atoms(?))

		// simple array to store the numbers of atoms (53 because iodine has highest number)
		// [0] holds the number of atoms
		Size num_elem[Element::NUMBER_OF_ELEMENTS];
		Size num_heavy_atoms(0), num_h_acc_atoms(0), num_h_don_atoms(0), num_aromatic_atoms(0), num_hydrophobic_atoms(0);
		double formal_charges(0), mol_weight(0), atom_polarizabilities(0);		
			
		for (Size i = 0; i < Element::NUMBER_OF_ELEMENTS; i++)
		{
			num_elem[i] = 0;
		}

		// donors are hydrogen of amino or hydrogen of hydroxyl
		//static Expression ex_don("element(O) AND connectedTo((C)) AND connectedTo((H)) OR 
		//													element(N) AND connectedTo((H))");
		// acceptors are carbonyl oxygens, hydroxyl oxygen, amino nitrogen and flourine
		//static Expression ex_acc("element(O) AND doubleBonds((C)) OR
		//													element(O) AND connectedTo((C)(H)) OR
		//													element(F)");
		
		SmartsMatcher smarts_matcher;

		String smarts_h_don("[$(O(#1)C),$(N(#1))]");
		String smarts_h_acc("[$(O=C),$(O#1),F]");
	
		HashSet<const Atom*> h_bond_donors, h_bond_acceptors;
		SmartsMatcher::Match matchings;
		Molecule* mol = static_cast<Molecule*>(&ac);
		smarts_matcher.match(matchings, *mol, smarts_h_don);
		for (SmartsMatcher::Match::const_iterator it1 = matchings.begin(); it1 != matchings.end(); ++it1)
		{
			for (set<const Atom*>::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
			{
				h_bond_donors.insert(*it2);
			}
		}

		matchings.clear();
		smarts_matcher.match(matchings, *mol, smarts_h_acc);
		for (SmartsMatcher::Match::const_iterator it1 = matchings.begin(); it1 != matchings.end(); ++it1)
    {
      for (set<const Atom*>::const_iterator it2 = it1->begin(); it2 != it1->end(); ++it2)
      {
        h_bond_acceptors.insert(*it2);
      }
    }


		for (AtomConstIterator a_it = ac.beginAtom(); a_it != ac.endAtom(); a_it++)
		{
			// elements
			num_elem[a_it->getElement().getAtomicNumber()]++;
			num_elem[0]++;
			mol_weight += a_it->getElement().getAtomicWeight();
			// heavy atoms
			if (a_it->getElement().getAtomicNumber() > 1) 
			{ 
				num_heavy_atoms++;	
			}
			
			if (h_bond_donors.has(&*a_it))
			{	
				num_h_don_atoms++; 
			}

			if (h_bond_acceptors.has(&*a_it)) 
			{ 
				num_h_acc_atoms++; 
			}
			if (isHydrophobic_(&(*a_it)))	
			{	
				num_hydrophobic_atoms++; 
			}
			formal_charges += a_it->getCharge();
			atom_polarizabilities += getAtomicPolarizability_(a_it->getElement().getAtomicNumber());
			
			if (a_it->getProperty("IsAromatic").getBool())
			{
				++num_aromatic_atoms;
			}
		}

		// calc info content Descriptor (mean)
		double atom_info_content(0);
		for (int i=1;i<Element::NUMBER_OF_ELEMENTS;i++)
		{
			if (num_elem[i] != 0)
			{
				double p_i = ((double)num_elem[i])/((double)num_elem[0]);
				atom_info_content += -p_i * 3.3219280948873626 * log10(p_i);
			}
		}
		
		// write the data in the ac properties
		ac.setProperty("NumberOfAtoms", (double)num_elem[0]);
		ac.setProperty("NumberOfHydrogen", (double)num_elem[1]);
		ac.setProperty("NumberOfBoron", (double)num_elem[5]);
		ac.setProperty("NumberOfCarbon", (double)num_elem[6]);
		ac.setProperty("NumberOfNitrogen", (double)num_elem[7]);
		ac.setProperty("NumberOfOxygen", (double)num_elem[8]);
		ac.setProperty("NumberOfFlourine", (double)num_elem[9]);
		ac.setProperty("NumberOfPhosphorus", (double)num_elem[15]);
		ac.setProperty("NumberOfSulfur", (double)num_elem[16]);
		ac.setProperty("NumberOfChlorine", (double)num_elem[17]);
		ac.setProperty("NumberOfBromine", (double)num_elem[35]);
		ac.setProperty("NumberOfIodine", (double)num_elem[53]);
		ac.setProperty("FormalCharge", formal_charges);
		ac.setProperty("NumberOfHydrogenBondAcceptors", (double)num_h_acc_atoms);
		ac.setProperty("NumberOfHydrogenBondDonors", (double)num_h_don_atoms);
		ac.setProperty("NumberOfHydrophobicAtoms", (double)num_hydrophobic_atoms);
		ac.setProperty("NumberOfHeavyAtoms", (double)num_heavy_atoms);
		ac.setProperty("MeanAtomInformationContent", atom_info_content);
		ac.setProperty("MolecularWeight", mol_weight);
		ac.setProperty("NumberOfAromaticAtoms", (double)num_aromatic_atoms);
		
		// 2. part count bonds
		Size single_bonds(0), double_bonds(0), triple_bonds(0), aromatic_bonds(0),
				num_of_bonds(0), num_heavy_bonds(0), num_rot_single_bonds(0), num_rot_bonds(0);
		double bond_polarizabilities(0);
	
		AtomConstIterator a_it = ac.beginAtom();
		Atom::BondConstIterator b_it = a_it->beginBond();
		BALL_FOREACH_BOND (ac, a_it, b_it)
		{
			switch (b_it->getOrder())
			{
				case Bond::ORDER__SINGLE : single_bonds++; break;
				case Bond::ORDER__DOUBLE : double_bonds++; break;
				case Bond::ORDER__TRIPLE : triple_bonds++; break;
				case Bond::ORDER__AROMATIC : aromatic_bonds++; break;
				case Bond::ORDER__UNKNOWN : break; 
			}
			num_of_bonds++;

			// rotatable bonds
			const Atom* a1 = b_it->getFirstAtom();
			const Atom* a2 = b_it->getSecondAtom();
			short an1 = a1->getElement().getAtomicNumber();
			short an2 = a2->getElement().getAtomicNumber();

			// testing atoms of ring memberships, bc SSSR marks not all bonds as ring bonds!
			if (a1->countBonds() > 1 && a2->countBonds() > 1  &&
					!(a1->getProperty("InRing").getBool() && a2->getProperty("InRing").getBool()))
			{
				if (b_it->getOrder() == Bond::ORDER__SINGLE)
				{
					num_rot_single_bonds++;
				}
				num_rot_bonds++;				
			}

			// heavy bonds
			if (an1 > 1 && an2 > 1)
			{
				num_heavy_bonds++;
			}

			// atomic polarizabilities
			bond_polarizabilities += fabs(getAtomicPolarizability_(b_it->getFirstAtom()->getElement().getAtomicNumber()) - 
																		getAtomicPolarizability_(b_it->getSecondAtom()->getElement().getAtomicNumber()));
		}
		
		//write the bond data in the ac properties
		ac.setProperty("NumberOfSingleBonds", (double)single_bonds);
		ac.setProperty("NumberOfDoubleBonds", (double)double_bonds);
		ac.setProperty("NumberOfTripleBonds", (double)triple_bonds);
		ac.setProperty("NumberOfAromaticBonds", (double)aromatic_bonds);
		ac.setProperty("NumberOfBonds", (double)num_of_bonds);
		ac.setProperty("NumberOfHeavyBonds", (double)num_heavy_bonds);
		ac.setProperty("NumberOfRotatableBonds", (double)num_rot_bonds);
		ac.setProperty("NumberOfRotatableSingleBonds", (double)num_rot_single_bonds);
		ac.setProperty("AtomicPolarizabilities", atom_polarizabilities);
		ac.setProperty("BondPolarizabilities", bond_polarizabilities);
	}

	bool SimpleBase::isHydrophobic_(const Atom* atom) const
	{
		if (atom->getElement() == PTE[Element::C])
		{
			Atom::BondConstIterator b_it(atom->beginBond());
			for (; b_it != atom->endBond(); ++b_it)
			{
				if (b_it->getBoundAtom(*atom)->getElement() != PTE[Element::C] &&
						b_it->getBoundAtom(*atom)->getElement() != PTE[Element::H])
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}
		return true;
	}

	void SimpleBase::readAtomicPolarizabilities_(vector<float>& atomic_polarizabilties)
	{
		Path path;
    String filename = path.find(data_folder_+BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE);
		// if filename is empty the file could not be found
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, data_folder_+BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE);
		}
		File pol_file(filename);
		// skip comment line 
		pol_file.ignore(1000, '\n');	
		// read the polarizability values
		float apol(0);
		while (pol_file.good())
		{
			pol_file >> apol;
			atomic_polarizabilties.push_back(apol);
		}
	}

	float SimpleBase::getAtomicPolarizability_(int atomic_number)
	{
		static vector<float> a_pols;
		if (a_pols.empty())
		{
			readAtomicPolarizabilities_(a_pols);
		}
		
		if (atomic_number > 0 && atomic_number < (int)a_pols.size())
		{
			return a_pols[atomic_number - 1];
		}
		else
		{
			Log.error() << "SimpleBase::getAtomicPolarizability: cannot find polarizability for Element " << 
				atomic_number  << " check '" << BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE << "'!" << endl;
		}

		// here we return 0, because unknown atom types are considered not to contribute
		// to the polarizability, mostly this occurs when the atom type is set to 0 from the file parser
		return 0;
	}
} // namespace BALL

