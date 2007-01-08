// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleBase.C,v 1.2 2004/09/02 13:12:51 amoll Exp $
//

#include <BALL/QSAR/simpleBase.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/string.h>

#include <iostream>
#include <vector>
#include <string>

using namespace std;

#define BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE "QSAR/atomic_polarizabilities.data"
#define BALL_QSAR_SIMPLEBASE_DEBUG
#undef  BALL_QSAR_SIMPLEBASE_DEBUG

namespace BALL
{
	SimpleBase::SimpleBase()
		:	Descriptor()
	{
	}

	SimpleBase::SimpleBase(const SimpleBase& sb)
		:	Descriptor(sb)
	{
	}

	SimpleBase::SimpleBase(const String& name)
		:	Descriptor(name)
	{
	}

	SimpleBase::SimpleBase(const String& name, const String& unit)
		:	Descriptor(name, unit)
	{
	}

	SimpleBase::~SimpleBase()
	{
	}

	SimpleBase& SimpleBase::operator = (const SimpleBase& sb)
	{
		this->setUnit(sb.getUnit());
		this->setName(sb.getName());
		return *this;
	}

	double SimpleBase::calcPrincipalMomentOfInertia_(double& pmi_x, double& pmi_y, double& pmi_z, Molecule& molecule)
	{
		Vector3 sum(0.0, 0.0, 0.0);
		double sum_weight = 0;
		for (AtomConstIterator i=molecule.beginAtom();i!=molecule.endAtom();++i)
		{
			double weight = i->getElement().getAtomicWeight();
			Vector3 pos = i->getPosition();
			Vector3 tmp(pos[0]*weight, pos[1]*weight, pos[2]*weight);
			sum += tmp;
			sum_weight += weight;
		}
		
		Vector3 mid(sum[0]/sum_weight, sum[1]/sum_weight, sum[2]/sum_weight);
		double pmi = 0;
		
		for (AtomConstIterator i=molecule.beginAtom();i!=molecule.endAtom();++i)
		{
			double weight = i->getElement().getAtomicWeight();
			Vector3 atom = i->getPosition();
			pmi_x += pow(atom[0]-mid[0],2) * weight;
			pmi_y += pow(atom[1]-mid[1],2) * weight;
			pmi_z += pow(atom[2]-mid[2],2) * weight;
			pmi += atom.getSquareDistance(mid) * weight;
		}
		return pmi;
	}


	bool SimpleBase::isValid(Molecule& molecule)
	{
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = molecule.getModificationTime();
		Handle mol_handle = molecule.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				#ifdef BALL_QSAR_SIMPLEBASE_DEBUG
				cerr << ">> SimpleBase::isValid: molcule valid!" << endl;
				#endif
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				#ifdef BALL_QSAR_SIMPLEBASE_DEBUG
				cerr << ">> SimpleBase::isValid: molecule not valid, modified!" << endl;
				#endif
				return false;
			}
		}
		else
		{
			mod_times.insert(std::make_pair(mol_handle, last_mod));
			#ifdef BALL_QSAR_SIMPLEBASE_DEBUG
			cerr << ">> SimpleBase::isValid: molecule not valid, first call!" << endl;
			#endif
			return false;
		}
	}


	void SimpleBase::calculate(Molecule& molecule)
	{
		// ring processing
		vector<vector<Atom*> > sssr;
		RingPerceptionProcessor rpp;
		Size size_of_sssr = rpp.calculateSSSR(sssr, molecule);

		//cerr << "moep";

		// the size of the SSSR
		molecule.setProperty("SizeOfSSSR", (double)size_of_sssr);
			
		// aromatic processing
		AromaticityProcessor aro;
		aro.aromatize(sssr, molecule);
	
		// pmi calculator
		double pmi(0), pmi_x(0), pmi_y(0), pmi_z(0);
		pmi = calcPrincipalMomentOfInertia_(pmi_x, pmi_y, pmi_z, molecule);
		molecule.setProperty("PrincipalMomentOfInertia",pmi);
		molecule.setProperty("PrincipalMomentOfInertiaX",pmi_x);
		molecule.setProperty("PrincipalMomentOfInertiaY",pmi_y);
		molecule.setProperty("PrincipalMomentOfInertiaZ",pmi_z);

		// 1. part counts atoms (not counting aromatic atoms(?))

		// simple array to store the numbers of atoms (53 because iodine has highest number)
		// [0] holds the number of atoms
		Size num_elem[Element::NUMBER_OF_ELEMENTS];
		Size num_heavy_atoms(0), num_h_acc_atoms(0), num_h_don_atoms(0), num_aromatic_atoms(0), num_hydrophobic_atoms(0);
		double formal_charges(0), mol_weight(0), atom_polarizabilities(0);		
			
		for (unsigned int i=0;i<Element::NUMBER_OF_ELEMENTS;i++)
		{
			num_elem[i] = 0;
		}

		// donors are hydrogen of amino or hydrogen of hydroxyl
		static Expression ex_don("element(O) AND connectedTo((C)) AND connectedTo((H)) OR \
															element(N) AND connectedTo((H))");
		// acceptors are carbonyl oxygens, hydroxyl oxygen, amino nitrogen and flourine
		static Expression ex_acc("element(O) AND doubleBonds((C)) OR\
															element(O) AND connectedTo((C)(H)) OR\
															element(F)");

		AtomConstIterator a_it = molecule.beginAtom();
		for (;a_it!=molecule.endAtom();a_it++)
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
			if (ex_don(*a_it))	
			{	
				num_h_don_atoms++; 
			}
			if (ex_acc(*a_it)) 
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
		
		// write the data in the molecule properties
		molecule.setProperty("NumberOfAtoms", (double)num_elem[0]);
		molecule.setProperty("NumberOfHydrogen", (double)num_elem[1]);
		molecule.setProperty("NumberOfBoron", (double)num_elem[5]);
		molecule.setProperty("NumberOfCarbon", (double)num_elem[6]);
		molecule.setProperty("NumberOfNitrogen", (double)num_elem[7]);
		molecule.setProperty("NumberOfOxygen", (double)num_elem[8]);
		molecule.setProperty("NumberOfFlourine", (double)num_elem[9]);
		molecule.setProperty("NumberOfPhosphorus", (double)num_elem[15]);
		molecule.setProperty("NumberOfSulfur", (double)num_elem[16]);
		molecule.setProperty("NumberOfChlorine", (double)num_elem[17]);
		molecule.setProperty("NumberOfBromine", (double)num_elem[35]);
		molecule.setProperty("NumberOfIodine", (double)num_elem[53]);
		molecule.setProperty("FormalCharge", formal_charges);
		molecule.setProperty("NumberOfHydrogenBondAcceptors", (double)num_h_acc_atoms);
		molecule.setProperty("NumberOfHydrogenBondDonors", (double)num_h_don_atoms);
		molecule.setProperty("NumberOfHydrophobicAtoms", (double)num_hydrophobic_atoms);
		molecule.setProperty("NumberOfHeavyAtoms", (double)num_heavy_atoms);
		molecule.setProperty("MeanAtomInformationContent", atom_info_content);
		molecule.setProperty("MolecularWeight", mol_weight);
		molecule.setProperty("NumberOfAromaticAtoms", (double)num_aromatic_atoms);
		
		// 2. part count bonds
		Size single_bonds(0), double_bonds(0), triple_bonds(0), aromatic_bonds(0),
				num_of_bonds(0), num_heavy_bonds(0), num_rot_single_bonds(0), num_rot_bonds(0);
		double bond_polarizabilities(0);
	
		a_it = molecule.beginAtom();
		Atom::BondConstIterator b_it = a_it->beginBond();

		BALL_FOREACH_BOND (molecule, a_it, b_it)
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
			const Atom * a1 = b_it->getFirstAtom();
			const Atom * a2 = b_it->getSecondAtom();
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
		
		//write the bond data in the molecule properties
		molecule.setProperty("NumberOfSingleBonds", (double)single_bonds);
		molecule.setProperty("NumberOfDoubleBonds", (double)double_bonds);
		molecule.setProperty("NumberOfTripleBonds", (double)triple_bonds);
		molecule.setProperty("NumberOfAromaticBonds", (double)aromatic_bonds);
		molecule.setProperty("NumberOfBonds", (double)num_of_bonds);
		molecule.setProperty("NumberOfHeavyBonds", (double)num_heavy_bonds);
		molecule.setProperty("NumberOfRotatableBonds", (double)num_rot_bonds);
		molecule.setProperty("NumberOfRotatableSingleBonds", (double)num_rot_single_bonds);
		molecule.setProperty("AtomicPolarizabilities", atom_polarizabilities);
		molecule.setProperty("BondPolarizabilities", bond_polarizabilities);
	}

	bool SimpleBase::isHydrophobic_(const Atom* atom) const
	{
		if (atom->getElement() == PTE[Element::C])
		{
			Atom::BondConstIterator b_it(atom->beginBond());
			for (;b_it!=atom->endBond();++b_it)
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

	vector<float> SimpleBase::readAtomicPolarizabilities_()
	{
		vector<float> pols;
		Path path;
    String filename = path.find(BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE);
		// if filename is empty the file could not be found
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE);
		}
		File pol_file(filename);
		// skip comment line 
		pol_file.ignore(1000,'\n');	
		// read the polarizability values
		float apol(0);
		while (pol_file.good())
		{
			pol_file >> apol;
			pols.push_back(apol);
		}
		return pols;
	}

	float SimpleBase::getAtomicPolarizability_(int atomic_number)
	{
		static const vector<float> a_pols = readAtomicPolarizabilities_();
		
		if (atomic_number > 0 && atomic_number < (int)a_pols.size())
		{
			return a_pols[atomic_number-1];
		}
		else
		{
			Log.error() << "SimpleBase::getAtomicPolarizability: cannot find polarizability for Element " << 
				atomic_number  << " check '" << BALL_QSAR_ATOMIC_POLARIZABILITIES_FILE << "'!" << endl;
			// here we return 0, because unknown atom types are considered not to contribute
			// to the polarizability, mostly this occurs when the atom type is set to 0 from the file parser
			return 0;
		}
	}
} // namespace BALL
