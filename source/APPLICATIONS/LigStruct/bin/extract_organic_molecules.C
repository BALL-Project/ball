
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include <boost/unordered/unordered_set.hpp>

using namespace BALL;
using namespace std;

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
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Organic Filter", " keep only organic molecules", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "sdfile to be filtered", INFILE, true);
	parpars.registerParameter("o", "organic molecules outputSDF", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "does stuff";
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
	organic_elements.insert( pte[Element::SILICON ].getAtomicNumber());
	organic_elements.insert( pte[Element::SULFUR  ].getAtomicNumber());
	organic_elements.insert( pte[Element::SELENIUM].getAtomicNumber());
	
	
	Log << "Reading molecules..."<<endl;
	
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile out_file(parpars.get("o"), ios::out);

	AtomContainer* tmp_mol = 0;
	unsigned int num_failed_reads = 0;
	unsigned int num_sucess_reads = 0;
	unsigned int num_organic = 0;
	unsigned int num_too_small = 0;
	unsigned int num_valid = 0;
	do
	{
		//try to load a molecule
		while(true) // try reading untill you do not get any exception
		{
			try
			{
				tmp_mol = in_file.read();
				num_sucess_reads++;
				break;
			}
			catch (...)
			{
				num_failed_reads++;
			}
		}
		
		// check the molecule
		if( isOfElementClass(*tmp_mol, organic_elements) )
		{
			num_organic++;
			
			if( tmp_mol->countAtoms() > 1)
			{
				out_file << *tmp_mol;
				num_valid++;
			}
			else
				num_too_small++;
		}
			
		delete tmp_mol;
		
	} while( tmp_mol );

	Log<< " read "<< num_failed_reads+num_sucess_reads<< " from which "<<num_failed_reads<<" could not be parsed"<<endl;
	Log<< " from total "<< num_sucess_reads<< " molecules "<<num_organic <<" were organic and "<<num_sucess_reads-num_organic<<" organo-metal"<<endl;
	Log<< " from the "<<num_organic<<" organic molecules "<< num_valid<< " were larger than 1 atom, these were written to the out-file"<<endl;
	Log << "......done!"<<endl;
}

