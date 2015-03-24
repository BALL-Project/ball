
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/STRUCTURE/fragmentDB.h>

#include "../sources/base.h"
#include "../sources/clashBase.h"

using namespace BALL;
using namespace std;

static unsigned int total_molecules = 0;

// number of all faults (possibly more than 1 per molecule)
static unsigned int total_stretch_faults = 0;
static unsigned int total_bend_faults    = 0;
static unsigned int total_clashes        = 0;

// number of the most severe faults per molecule (if any occured in molecule)
static unsigned int num_severe_stretches = 0;
static unsigned int num_severe_bends = 0;
static unsigned int num_severe_clashes = 0;

// averaged severity scores over all severest (per molecule) faults
static double severity_stretches = 0;
static double severity_bends     = 0;
static double severity_clashes   = 0;

// worst found value:
static double worst_stretch = 0;
static double worst_bend    = 0;
static double worst_clash   = 0;


/*
 * 
 */
float singleStretch(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getCovalentRadius() + btm.getElement().getCovalentRadius();
	
	// return absolute difference to ideal
	float diff = std::abs( ideal_dist - atm.getDistance( btm ) );
	float stretch_severity = diff / ideal_dist;
	
	if( stretch_severity > 0.01 )
		++total_stretch_faults;
	
	if( stretch_severity > worst_stretch)
		worst_stretch = stretch_severity;
	
	return stretch_severity;
}

/*
 * 
 */
float getStretchSeverity( AtomContainer& mol)
{
	float max_stretch = 0.0;
	float tmp = 0.0;
	
	// iterate over all non-covalent atom pairs:
	for(AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		AtomIterator bti = ati;
		for (++bti; +bti; ++bti)
		{
			// atm & btm ARE bonded:
			if ( ati->getBond( *bti ) != 0)
			{
				tmp = singleStretch(*ati, *bti);
				
				if(tmp > max_stretch)
					max_stretch = tmp;
			}
		}
	}
	
	return max_stretch;
}

/*
 * Get inverted overlap factor
 */
float singleInvertedOverlapFactor(Atom& atm, Atom& btm)
{
	float ideal_dist = atm.getElement().getVanDerWaalsRadius() + btm.getElement().getCovalentRadius();
	
	float result =  (ideal_dist / atm.getDistance( btm ));
	
	// limit worst score to 10:
	if (result > 10)
		result = 10;
	
	if (result > 1.0)
		++total_clashes;
	
	if (result > worst_clash)
		worst_clash = result;
		
	return result;
}

/*
 * Search for the maximal (most severe clash) and return its inverted
 * overlap factor.
 * 
 * If no clash existed return 0
 */
float getInvertedOverlapFactor(AtomContainer& mol)
{
	float max_factor = 0.0;
	float tmp = 0.0;
	
	// iterate over all non-covalent atom pairs:
	for(AtomIterator ati = mol.beginAtom(); +ati; ++ati)
	{
		AtomIterator bti = ati;
		for (++bti; +bti; ++bti)
		{
			// atm & btm are NOT bonded:
			if ( ati->getBond( *bti ) == 0)
			{
				tmp = singleInvertedOverlapFactor(*ati, *bti);
				
				if(tmp > max_factor)
					max_factor = tmp;
			}
		}
	}
	
	return max_factor;
}


/*
 * 
 */
float singleBendSeverity( MMFF94StretchBend::Bend& bend)
{
	float angle_severity = bend.delta_theta;
	
	if( angle_severity > 1. )
		++total_stretch_faults;
	
	// to radiant:
	angle_severity = angle_severity / 180 * Constants::PI;
	
	if( angle_severity > worst_bend)
		worst_bend = angle_severity;
	
	return angle_severity;
}


/*
 * 
 */
float getBendSeverity( AtomContainer& mol )
{
	float max_factor = 0.0;
	float tmp = 0.0;

	// extract and calculated the bends via MMFF94:
	System tmp_sys;
	tmp_sys.insert( *(Molecule*)&mol );

	FragmentDB db("");
	tmp_sys.apply(db.normalize_names);
	tmp_sys.apply(db.build_bonds);

	MMFF94 mmff( tmp_sys );

	MMFF94StretchBend bend_component( mmff );

	vector<MMFF94StretchBend::Bend> bends = bend_component.getBends();
	
	// iterate over all bends:
	for(vector<MMFF94StretchBend::Bend>::iterator bit; bit != bends.end(); ++bit)
	{
		tmp = singleBendSeverity( *bit );
		
		if(tmp > max_factor)
			max_factor = tmp;
	}
	
	return max_factor;
}


void updateTotalMeasure( AtomContainer& mol)
{
	// Covalent deviation
	float this_stretch_severity = getStretchSeverity( mol );
	
	if ( this_stretch_severity > 0.01)
	{
		severity_stretches += this_stretch_severity;
		++num_severe_stretches;
	}
	
	// Angle deviation
	// does NOT work...
//	float this_bend_severity = getBendSeverity( mol );
//	if( this_bend_severity > (Constants::PI / 180 )) // bigger than 1°
//	{
//		severity_bends += this_bend_severity;
//		++num_severe_bends;
//	}
	
	// Clashes
	float this_clash_severity = getInvertedOverlapFactor( mol );
	
	if ( this_clash_severity > 1)
	{
		severity_clashes += this_clash_severity;
		++num_severe_clashes;
	}
}

void calculateAverages()
{
	// bonds
	if( num_severe_stretches )
	{
		severity_stretches /= num_severe_stretches;
	}
	else
		severity_stretches = 0;
	
	// angles
	if( num_severe_bends )
	{
		severity_bends /= num_severe_bends;
		severity_bends -= 1;
	}
	else
		severity_bends = 0;
	
	// clashes
	if (num_severe_clashes)
	{
		severity_clashes /= num_severe_clashes;
		severity_clashes -= 1;
	}
	else
		severity_clashes = 0;
}

///################################# M A I N ###################################
///#############################################################################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Quality Checker", "Report quality of structures", 0.1, String(__DATE__), "Evaluation");
	parpars.registerParameter("i", "input SDF to be assessed ", STRING, true);
	
	parpars.setSupportedFormats("i","sdf");

	parpars.setToolManual("Checks how well the found covalent bond lengths and "
												"vdw-distances agree with known standard parameters");

	parpars.parse(argc, argv);
	
	
	/// C O D E ##################################
	SDFile in_file(parpars.get("i"), ios::in);
	Log<<"Reading and evaluating input file: "<<endl<<in_file.getName()<<endl;

	AtomContainer* tmp;

	tmp = in_file.read();

	while(tmp)
	{
		total_molecules++;
		
		updateTotalMeasure( *tmp );
		
		tmp = in_file.read(); // next loop iteration
	}
	in_file.close();
	
	// get final averages from total counts:
	calculateAverages();
	
	Log<<"...done analysing"<<endl<<endl<<endl;

	Log << "Reporting faults ( avg severity / worst severity / total occurences / total molecules):" << endl;
	Log << "Total molecules: " << total_molecules << endl << endl;
	Log<< "Stretch: " << severity_stretches << " / "<< worst_stretch << " / " 
		 << total_stretch_faults << " / "<< num_severe_stretches << endl;
	
	Log<< "Bend   : " << severity_bends << " / " << worst_bend 
		 << " / " << total_bend_faults << " / "<< num_severe_bends << endl;
	
	Log<< "clashes: " << severity_clashes << " / " << worst_clash << " / "
		 << total_clashes << " / "<< num_severe_clashes << endl;
}
