// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: partialChargeProcessor.C,v 1.2.28.1 2007/03/16 00:06:47 bertsch Exp $
//

#include <BALL/QSAR/partialChargeProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/CONCEPT/timeStamp.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/SYSTEM/path.h>

#include <utility>

using namespace std;

#define BALL_QSAR_ATOMIC_IONIZATION_ENERGIES_FILE "/atomic_ionization_potentials.data"
#define BALL_QSAR_ATOMIC_ELECTRON_AFFINITIES_FILE "/atomic_electron_affinities.data"

namespace BALL
{
	PartialChargeProcessor::PartialChargeProcessor()
		: UnaryProcessor<AtomContainer>(),
		warned_elements_()
	{
		data_folder_ = "QSAR/";
	}

	PartialChargeProcessor::PartialChargeProcessor(const PartialChargeProcessor& pc)
    : UnaryProcessor<AtomContainer>(pc),
		warned_elements_()
	{
		data_folder_ = "QSAR/";
	}
	
	PartialChargeProcessor::~PartialChargeProcessor()
	{
	}

	PartialChargeProcessor& PartialChargeProcessor::operator = (const PartialChargeProcessor& /* pc */)
	{
		return *this;
	}

	Processor::Result PartialChargeProcessor::operator () (AtomContainer& ac)
	{
		calculatePEOE(ac);
		return Processor::CONTINUE;
	}
	
	void PartialChargeProcessor::setDataFolder(const char* folder)
	{
		data_folder_ = folder;
	}

	void PartialChargeProcessor::calculatePEOE(AtomContainer& ac)
	{
	
		// the PEOE calculation does not touch and does not use
		// the charge stored with the atoms! 

		HashMap<Atom*, double> charges;
		HashMap<Atom*, double> former_charges;
		AtomIterator atom_it = ac.beginAtom();
		// through all atoms in the hashmaps
		for (;atom_it!=ac.endAtom();atom_it++)
		{
			charges.insert(std::make_pair(&(*atom_it), 0.0));
			former_charges.insert(std::make_pair(&(*atom_it), 0.0));
		}
	
		// quick hack to avoid this damn repeats of warnings from logstream!
		warned_elements_.clear();
	
		// main loop to do the PEOE work
		// maybe this should be a while loop which runs until the shifted charge 
		// runs below a certain threshold
		for (int i=1;i!=7;i++)
		{
			atom_it = ac.beginAtom();
			Atom::BondIterator bond_it = atom_it->beginBond();
			
			BALL_FOREACH_BOND (ac, atom_it, bond_it)
			{
				Atom * atom1 = bond_it->getPartner(*(bond_it->getSecondAtom()));
				Atom * atom2 = bond_it->getPartner(*(bond_it->getFirstAtom()));
				
				float a1(0), b1(0), c1(0), a2(0), b2(0), c2(0);
				getabcFactors_(atom1, a1, b1, c1);
				getabcFactors_(atom2, a2, b2, c2);

				// calc the electronegativities
				double chi1 = a1 + b1 * former_charges[atom1] + c1 * former_charges[atom1] * former_charges[atom1];
				double chi2 = a2 + b2 * former_charges[atom2] + c2 * former_charges[atom2] * former_charges[atom2];

				if (chi1 > chi2) // first is more electronegative
				{
					double chi_plus(0);
					if (atom2->getElement() == PTE[Element::H])
					{
						chi_plus = 20.02;
					}
					else
					{
						chi_plus = a2 + b2 + c2;
					}
					charges[atom1] += (chi2 - chi1)/chi_plus/pow((double)2,(double)i);
					charges[atom2] += (chi1 - chi2)/chi_plus/pow((double)2,(double)i);
				} 
				else  // second is more electronegative 
				{
					double chi_plus(0);
					if (atom1->getElement() == PTE[Element::H])
					{
						chi_plus = 20.02;
					}
					else
					{
						chi_plus = a1 + b1 + c1;
					}
					charges[atom1] += (chi2 - chi1)/chi_plus/pow((double)2,(double)i);
					charges[atom2] += (chi1 - chi2)/chi_plus/pow((double)2,(double)i);
				}
			}
			former_charges = charges;
		}
		
		HashMap<Atom*, double>::Iterator it = charges.begin();	
		// assign the calculated values to the atoms
		for (it=charges.begin();it!=charges.end();++it)
		{
			it->first->setProperty("PEOEPartialCharge", it->second);
		}
	}


	void PartialChargeProcessor::getabcFactors_(Atom * atom, float& a, float& b, float& c)
	{
		Size num_bonds = atom->countBonds();
		switch(atom->getElement().getAtomicNumber())
		{ 
			case 1: //H
				a =  7.17;	b =  6.24;	c = -0.56;	break;
			case 6: //C
				if (num_bonds == 4)	{	a =  7.98;	b =  9.18;	c =  1.88;  break; }
				if (num_bonds == 3)	{	a =  8.79;	b =  9.32;	c =  1.51;  break; }
				if (num_bonds == 2)	{	a = 10.39;	b =  9.45;	c =  0.73;  break; }
				//Log.error() << "PartialCharge::calculate_(): cannot determine state of element C, missing H?" << endl;
				// we must set default values here to proceed 
				a = 7.98;
				b = 9.18;
				c = 1.88;
				break;				
			case 7: //N
				if (num_bonds == 3)	{	a = 11.54;	b = 10.82;	c =  1.36;	break; }
				if (num_bonds == 2) {	a = 12.87;	b = 11.15;	c =  0.85;	break; }
				if (num_bonds == 1)	{	a = 15.68;	b = 11.7;		c = -0.27;	break; }
				//Log.error() << "PartialCharge::calculate_(): cannot determine state of element N (" << 
				//	num_bonds << "), missing H?" << endl;
				// we must set some default values to proceed with PEOE calculation
				a = 11.54;
				b = 10.82;
				c =  1.36;
				break;
			case 8: //O
				if (num_bonds == 2)	{ a = 14.18;	b = 12.92;	c =  1.39;	break; }
				if (num_bonds == 1)	{	a = 17.07;	b = 13.79;	c =  0.47;	break; }
				//Log.error() << "PartialCharge::calculate_(): cannot determine state of element O, missing H?" << endl;
				// we must set here some default values to proceed
				a = 14.18;
				b = 12.92;
				c =  1.39;
				break;
			case 9: //F
				a = 14.66;	b = 13.85;	c =  2.31;	break;
			case 16: //S
				a = 10.14;	b =  9.13;	c =  1.38;	break;
			case 17: // Cl
				a = 11.00;	b =  9.69;	c =  1.36;	break;
			case 35: //Br
				a = 10.08;	b =  8.47;	c =  1.16;	break;
			case 53: //I
				a =  9.90;	b =  7.96;	c =  0.96;	break;
			default:
				Element::AtomicNumber atom_num = atom->getElement().getAtomicNumber();
				a = (getIonizationEnergy_(atom_num, 0) + getElectronAffinity_(atom_num, 0))/2;
				b = (getIonizationEnergy_(atom_num, 1) + getElectronAffinity_(atom_num, 1) - 
							getElectronAffinity_(atom_num, 0))/4;
				c = (getIonizationEnergy_(atom_num, 1) - getIonizationEnergy_(atom_num, 0) +
						 getElectronAffinity_(atom_num, 1) - getElectronAffinity_(atom_num, 0))/4;
				
				if (!warned_elements_.has(atom_num))
				{
					warned_elements_.insert(atom_num);
					//Log.warn() << "PartialChargeProcessor: No PEOE values for element " <<
					//	atom_num << " available, calculation might lead to unprecise results!" << endl;
				}
				break;
		}
	}


	std::pair<std::vector<float>, std::vector<float> > PartialChargeProcessor::readIonizationEnergies_()
	{
		vector<float> first_ies, second_ies;
		Path path;
		String filename = path.find(data_folder_+BALL_QSAR_ATOMIC_IONIZATION_ENERGIES_FILE);
		// empty filename means no file could be found
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, data_folder_+BALL_QSAR_ATOMIC_IONIZATION_ENERGIES_FILE);
		}
		File ie_file(filename);
		// skip comment line
		ie_file.ignore(1000, '\n');
		// read the first and second ionization potentials
		float ie_first(0), ie_second(0);
		while (ie_file.good())
		{
			ie_file >> ie_first;
			first_ies.push_back(ie_first);
			ie_file >> ie_second;
			second_ies.push_back(ie_second);			
		}
		return std::make_pair(first_ies, second_ies);
	}

	float PartialChargeProcessor::getIonizationEnergy_(Element::AtomicNumber atomic_number, Size charge)
	{
		static const pair<vector<float>, vector<float> > ies = readIonizationEnergies_();
		if (charge == 0)
		{
			if (atomic_number > 0 && atomic_number < (int)ies.first.size())
			{
				return ies.first[atomic_number-1];
			}
		}
		else
		{
			if (atomic_number > 0 && atomic_number < (int)ies.second.size())
			{
				return ies.second[atomic_number-1];
			}
		}
		
		Log.error()<< "PartialChargeProcessor::getIonizationEnergy_(): element " 
			<< atomic_number << " has no IE, check '" << BALL_QSAR_ATOMIC_IONIZATION_ENERGIES_FILE << "'!" << endl;
		// returning 0 is not right but is not harmful
		return 0.0;
	}


	void PartialChargeProcessor::readElectronAffinities_(vector<float>& electron_affinities)
	{
		Path path;
		String filename = path.find(data_folder_+BALL_QSAR_ATOMIC_ELECTRON_AFFINITIES_FILE);
		// if filename is empty the file is missing
		if (filename == "")
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, data_folder_+BALL_QSAR_ATOMIC_ELECTRON_AFFINITIES_FILE);
		}
		File ea_file(filename);
		// skip comment line
		ea_file.ignore(1000, '\n');
		// read the electron affinity values
		float ea(0);
		while (ea_file.good())
		{
			ea_file >> ea;
			electron_affinities.push_back(ea);
		}
	}

	float PartialChargeProcessor::getElectronAffinity_(Element::AtomicNumber atomic_number, Size charge)
	{
		static vector<float> eas;
		if (eas.empty())
		{
			readElectronAffinities_(eas);
		}
		if (charge == 0)
		{
			if (atomic_number > 0 && atomic_number < (int)eas.size())
			{
				return eas[atomic_number];
			}
		}
		else
		{
			return getIonizationEnergy_(atomic_number, 0);
		}
		Log.error() << "PartialChargeProcessor::getElectronAffinity_(): element " 
			<< atomic_number << " has no EA, check '" << BALL_QSAR_ATOMIC_ELECTRON_AFFINITIES_FILE << "'!" << endl;
		// returning 0 is the most harmless thing to return 
		return 0.0;
	}

} // namespace BALL
