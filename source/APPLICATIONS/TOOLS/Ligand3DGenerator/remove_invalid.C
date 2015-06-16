
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/PTE.h>

#include  <BALL/STRUCTURE/LIGAND3DGEN/base.h>

using namespace BALL;
using namespace std;

static unsigned int num_covalent_fault = 0;
static unsigned int num_vdw_fault      = 0;
static unsigned int num_elem_fault     = 0;

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

bool isValid(AtomContainer& mol)
{
	if( LigBase::containsUnknownElement( mol ) )
	{
		num_elem_fault++;
		return false;
	}
	else
	{
		return hasValidGeometry( mol );
	}
}

/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Remove Invalid", " filter out invalid molecules", 
														0.1, String(__DATE__), "Preparation");
	parpars.registerMandatoryInputFile("i", "general sdfile");
	parpars.registerMandatoryOutputFile("o", "single output SDF");
	parpars.registerOptionalOutputFile("ugly", "structures you do not like");
	
	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");
	parpars.setSupportedFormats("ugly","sdf");

	String manual = "From a given input multi SDFile, all structures that have "
									"severe clashes or containing atoms of unkonwn elements are "
									"removed. Additionally from all valid structures reamaining "
									"explicit hydrogens are removed.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// C O D E ##################################
/// 

	Log << "Reading molecules..."<<endl;
	
	SDFile in_file(parpars.get("i"), ios::in);
	SDFile out_file(parpars.get("o"), ios::out);
	
	SDFile* uglies = 0;
	bool write_ugly = parpars.has("ugly");
	
	if( write_ugly )
		uglies = new SDFile( parpars.get("ugly"), ios::out);

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
		
		LigBase::removeHydrogens( *tmp_mol );
		
		// check the molecule
		if( isValid(*tmp_mol) )
		{
			num_valid++;
			out_file << *tmp_mol;
		}
		else
		{
			num_invalid++;
			
			if( write_ugly )
				*uglies << *tmp_mol;
		}
			
		delete tmp_mol;
		
	}

	Log<< " Read "<< num_failed_reads+num_sucess_reads<< " from which "<<num_failed_reads<<" could not be parsed"<<endl;
	Log<< " from total read input molecules ("<< num_sucess_reads<< ") "<<num_valid <<" were valid and "<<num_invalid<<" were invalid"<<endl;
	Log<<" Total unknown-element-faults: "<<num_elem_fault    <<endl;
	Log<<" Total vdw-faults            : "<<num_vdw_fault     <<endl;
	Log<<" Total covalent-faults       : "<<num_covalent_fault<<endl;
	Log<<endl<<"All "<<num_valid<<" valid molecules were written to: "<<parpars.get("o")<<endl;
	
	if( write_ugly )
		Log<<endl<<"All "<<num_invalid<<" invalid molecules were written to: "<<parpars.get("ugly")<<endl;
	
	Log << "......done!"<<endl;
}


