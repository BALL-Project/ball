
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/STRUCTURE/LIGAND3DGEN/ligand3Dbase.h>

#include <boost/unordered/unordered_set.hpp>

using namespace BALL;
using namespace std;


/// ################# Q u a l i t y F i l t e r #################
static unsigned int num_covalent_fault = 0;
static unsigned int num_vdw_fault      = 0;
static unsigned int num_elem_fault     = 0;

// covalent bond is valid if it does not derive more than 30 % from its ideal length
bool validCovalentBond(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getCovalentRadius() + btm.getElement().getCovalentRadius();
	
	float actual_dist = atm.getDistance( btm );
	float diff = std::abs(ideal_dist - actual_dist);
	
	if ( diff > (ideal_dist * 0.3) )
	{
		num_covalent_fault++;
		return false;
	}
	
	return true;
}

// vdw dist is okay, if dist is not *smaller* than 40 % of ideal
bool validVdwDist(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getVanDerWaalsRadius() + btm.getElement().getCovalentRadius();
	ideal_dist = ideal_dist - ideal_dist * 0.4;
	
	float actual_dist = atm.getDistance( btm );
	
	if ( actual_dist < ideal_dist)
	{
		num_vdw_fault++;
		return false;
	}
	
	return true;
}

bool hasValidGeometry(AtomContainer& mol)
{
	for (AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		Atom& atm = *ati;
		AtomIterator bti = ati;
		for (++bti; +bti; ++bti)
		{
			Atom& btm = *bti;
			
			// atm & btm are covalently bond:
			if ( atm.getBond( btm ) != 0)
			{
				if ( !validCovalentBond(atm, btm) )
					return false;
			}
			else
			{
				if (!validVdwDist(atm, btm) )
					return false;
			}
		}
	}
	return true;
}

bool containsUnknownElement(AtomContainer &ac)
{
	for( AtomIterator it = ac.beginAtom(); +it; ++it)
	{
		if( it->getElement().getSymbol() == "?")
			return true;
	}
	
	return false;
}

bool isValid(AtomContainer& mol)
{
	if( containsUnknownElement( mol ) )
	{
		num_elem_fault++;
		return false;
	}
	else
	{
		return hasValidGeometry( mol );
	}
}


/// ################# E l e m e n t F i l t e r #################
bool isOfElementClass(AtomContainer& mol, boost::unordered_set< Element::AtomicNumber >& elems)
{
	for (AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		Atom& atm = *ati;
		if ( elems.find(atm.getElement().getAtomicNumber()) == elems.end() )
			return false;
	}
	return true;
}

/// ################# M A I N #################
/// File names: 
/// - rejected_metal.sdf : contains all metal or metal-organic molecules
/// - rejected_invalid-organic.sdf : contains all organic molecules which have
///                                  an invalid structure (severe clashes)
/// - filtered_organic.sdf : contains all valid organic molecule structures
/// 
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Filter Structures", " keep only valid organic molecules", 
														0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "sdfile to be filtered");
	parpars.setSupportedFormats("i","sdf");

	String manual = "Selects only molecules conaining atoms of the organic set. "
									"In this case that is: "
									"[As, B, Br, C, Cl, F, H, I, N, O, P, S, Se, Si] all other "
									"(metal and metal-organic) molecules are removed."
									"Following files will be created:";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
/// 
	// 1.) define the set of elements we want to keep if a molecule
	//     consists of only these:
	
	boost::unordered_set< Element::AtomicNumber > organic_elements;
	PTE_ pte;
	
	organic_elements.insert( pte[Element::ARSENIC].getAtomicNumber());
	organic_elements.insert( pte[Element::BORON].getAtomicNumber()   );
	organic_elements.insert( pte[Element::BROMINE].getAtomicNumber() );
	organic_elements.insert( pte[Element::CARBON ].getAtomicNumber());
	organic_elements.insert( pte[Element::CHLORINE].getAtomicNumber());
	organic_elements.insert( pte[Element::FLUORINE].getAtomicNumber());
	organic_elements.insert( pte[Element::HYDROGEN].getAtomicNumber());
	organic_elements.insert( pte[Element::IODINE  ].getAtomicNumber());
	organic_elements.insert( pte[Element::NITROGEN].getAtomicNumber());
	organic_elements.insert( pte[Element::OXYGEN  ].getAtomicNumber());
	organic_elements.insert( pte[Element::PHOSPHORUS].getAtomicNumber());
	organic_elements.insert( pte[Element::SULFUR  ].getAtomicNumber());
	organic_elements.insert( pte[Element::SELENIUM].getAtomicNumber());
	organic_elements.insert( pte[Element::SILICON ].getAtomicNumber());
	
	Log << "Reading molecules..."<<endl;
	
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile results_file("filtered_organic.sdf", ios::out);
	SDFile invalids_file("rejected_invalid-organic.sdf", ios::out);
	SDFile metals_file("rejected_metal.sdf", ios::out);

	AtomContainer* tmp_mol = 0;
	unsigned int num_failed_reads = 0;
	unsigned int num_sucess_reads = 0;
	unsigned int num_organic = 0;
	unsigned int num_metal = 0;
	unsigned int num_too_small = 0;
	unsigned int num_valid = 0;
	unsigned int num_invalid = 0;
	
	unsigned int cnt = 0;
	do
	{
		//try 'try_limit' times to load a molecule
		const int try_limit = 10000;
		int i = 0;
		for(; i < try_limit; ++i) // try reading untill you do not get any exception
		{
			// some user info every 1000 molecules:
			if( cnt % 1000 == 0)
			{
				cout << "\r" << flush;
				cout << "     Filtered: "<< cnt<<" structures to "<<num_valid<<" valid organic molecules"<<endl;
			}
			cnt++;
			
			try
			{
				tmp_mol = in_file.read(); // EOF gives a successful read, thus also escaping the loop (sets tmp_mol to NULL)
				num_sucess_reads++;
				break;
			}
			catch (...)
			{
				num_failed_reads++;
			}
		}
		if (i >= try_limit)
		{
			cout<<"ERROR: Tried "<<try_limit<<" times to read a molecule from the input file"
					<< "but failed every time"<<endl;
			exit(EXIT_FAILURE);
		}
		
		LigBase::removeHydrogens( *tmp_mol );
		
		if(tmp_mol->countAtoms() == 1)
		{
			num_too_small++;
			num_invalid++;
			continue;
		}
		
		// check the molecule
		if( isOfElementClass(*tmp_mol, organic_elements) ) // is organic molecule
		{
			num_organic++;
			
			if( isValid(*tmp_mol)) // is valid-organic
			{
				results_file << *tmp_mol;
				num_valid++;
			}
			else // is invalid-organic
			{
				num_invalid++;
				invalids_file << *tmp_mol;
			}
		}
		else // is metal-molecule:
		{
			num_metal++;
			metals_file << *tmp_mol;
		}
			
		delete tmp_mol;
		
	} while( tmp_mol );
	cout << "\r" << flush;
	cout << "                                                                              " << endl;

	Log<< " Successfully read structures: "<< num_sucess_reads << endl;
	Log<< " Failed read attempts:         "<< num_failed_reads << endl;
	Log<< endl;
	Log<< " after excluding single atom structures: " << num_sucess_reads - num_too_small<<endl; 
	Log<< " after excluding non-organic structures: " << num_organic << endl;
	Log<< " after excluding invalid structures:     " << num_valid   << endl;
	Log<< endl;
	Log<< " Number of invalid Structures:" << num_invalid <<endl;
	Log<< " Found invalid types:"<<endl;
	Log<< "       Contained Unknown Element: " << num_elem_fault <<endl;
	Log<< "   Contained Covalent Bond Fault: " << num_covalent_fault <<endl;
	Log<< "          Contained Severe Clash: " << num_vdw_fault <<endl;
	Log<< "......done!" << endl;
}

