

#include <BALL/KERNEL/forEach.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/bond.h>
//#include <BALL/COMMON/hash.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>
//#include <BALL/MATHS/angle.h>
//#include <BALL/MATHS/vector3.h>
//#include <BALL/MATHS/matrix44.h>


#include <vector>

//#include <BALL/STRUCTURE/geometricProperties.h>
//#include <BALL/MATHS/quaternion.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>


//#include <BALL/STRUCTURE/geometricTransformations.h>

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/FORMAT/lineBasedFile.h>

#include <boost/unordered/unordered_set.hpp>
#include <list>

using namespace BALL;
using namespace std;

static unsigned int num_covalent_fault = 0;
static unsigned int num_vdw_fault      = 0;

// covalent bond is valid if it does not *derive (+/-)* more than 30 % from its ideal
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

bool isValid(AtomContainer& mol)
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

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Remove bad Molecules", " filter badly clashing molecules out", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "general sdfile", INFILE, true);
	parpars.registerParameter("o", "single output SDF", OUTFILE, true);
	parpars.registerParameter("ugly", "structures you do not like", OUTFILE, true);
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("ugly","sdf");
	parpars.setOutputFormatSource("o","i");

	String manual = "does stuff";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
/// 

	Log << "Reading molecules..."<<endl;
	
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile out_file(parpars.get("o"), ios::out);
	SDFile uglies(parpars.get("ugly"), ios::out);

	AtomContainer* tmp_mol = 0;
	unsigned int num_failed_reads = 0;
	unsigned int num_sucess_reads = 0;
	unsigned int num_invalid      = 0;
	unsigned int num_valid        = 0;
	while(true)
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
		
		if( tmp_mol == 0 )
			break;
		
		// check the molecule
		if( isValid(*tmp_mol) )
		{
			num_valid++;
			out_file << *tmp_mol;
		}
		else
		{
			num_invalid++;
			uglies << *tmp_mol;
		}
			
		delete tmp_mol;
		
	}

	Log<< " read "<< num_failed_reads+num_sucess_reads<< " from which "<<num_failed_reads<<" could not be parsed"<<endl;
	Log<< " from total read input molecules ("<< num_sucess_reads<< ") "<<num_valid <<" were valid and "<<num_invalid<<" were invalid"<<endl;
	Log<<" Total vdw-faults: "<<num_vdw_fault<<" | Total covalent-faults: "<<num_covalent_fault<<endl;
	Log << "......done!"<<endl;
}


