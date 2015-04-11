
#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/bondIterator.h>
#include <BALL/KERNEL/PTE.h>

#include <cmath>
#include "../sources/base.h"
#include "../sources/clashBase.h"

using namespace BALL;
using namespace std;


class CSSAnalyzer
{
public:
	CSSAnalyzer(list< pair<AtomContainer*, float> >& collect_lst, int n = 0):
		total_clash_count(0),
		number_to_collect(n),
		uglies_list(collect_lst),
		total_average(0),
		worst_CSS(0)
	{
	}
	
	bool addStructure( AtomContainer& mol )
	{
		bool inserted = false;
		
		// 1.) get CSS for the structure
		float clash_severity = getStructureCSS( mol );
		
		// 2.) update 'average CSS', 'clash count' and 'worst CSS' if it contained a clash
		if ( clash_severity > 0)
		{
			total_clash_molecules++;
			
			// incremental average update formula:
			// avg(i) = avg(i-1) + ( ( val(i) -avg(i-1) ) / i)
			total_average = total_average + ((clash_severity - total_average) / total_clash_molecules);
			
			if (clash_severity > worst_CSS)
				worst_CSS = clash_severity;
			
			// 3.) update the worst output structures
			if( uglies_list.empty() )
			{
				// add new element
				uglies_list.push_front( make_pair(&mol, clash_severity) );
				inserted = true;
			}
			else if( clash_severity > uglies_list.back().second )
			{
				//		cout<<endl<<endl<<"inserting: "<<max_factor<<endl;
				//		for(list<pair<AtomContainer*, float>>::iterator it = ten_worst.begin();
				//				it != ten_worst.end(); ++it)
				//		{
				//			cout<<"score: "<<it->second<<endl;
				//		}
				// insert sorted:
				for(list<pair<AtomContainer*, float>>::iterator it = uglies_list.begin();
						it != uglies_list.end(); ++it)
				{
					if( it->second < clash_severity)
					{
						uglies_list.insert(it, make_pair(&mol , clash_severity) );
						break;
					}
				}
				inserted = true;
				
				// remove last element if container max is reached:
				if(uglies_list.size() > number_to_collect)
				{
					delete uglies_list.back().first;
					uglies_list.pop_back();
				}
			}
		}
		return inserted;
	}
	
	float getWorstCSS()
	{
		return worst_CSS;
	}

	float getAverageCSS()
	{
		return total_average;
	}
	
	unsigned int getNumberClashes()
	{
		return total_clash_count;
	}
	
	unsigned int getNumberClashingStructures()
	{
		return total_clash_molecules;
	}
	
private:

	int  number_to_collect;
	list< pair<AtomContainer*, float> >& uglies_list;
	
	unsigned int total_clash_count;
	unsigned int total_clash_molecules;
	
	float total_average;
	float worst_CSS;
	
	
	/*
	 * Search for the maximal (most severe clash) and return its inverted
	 * overlap factor.
	 * 
	 * If no clash existed return 0
	 */
	float getStructureCSS(AtomContainer& mol)
	{
		float max_severity = 0.0;
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
					tmp = singleCSS(*ati, *bti);
					
					if( tmp > 0)
						total_clash_count++;
								
					if(tmp > max_severity)
						max_severity = tmp;
				}
			}
		}
		return max_severity;
	}
	
	/*
	 * Get Clash Severity Score for a single Atom-Pair
	 */
	float singleCSS(Atom& atm, Atom& btm)
	{
		float ideal_dist = atm.getElement().getVanDerWaalsRadius() + btm.getElement().getVanDerWaalsRadius();
		
		//CSSvdw = min(10, max(0, distideal vdw/distreal − 1)
		float result =  min(10.f, max(0.f, (ideal_dist / atm.getDistance( btm ) - 1) ));

		return result;
	}
};/// END OF CLASS

///################################# M A I N ###################################
///#############################################################################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("Quality Checker", "Report quality of structures", 0.1, String(__DATE__), "Evaluation");
	parpars.registerParameter("i", "input SDF to be assessed ", STRING, true);
	parpars.registerParameter("o", "output file for the worst sturcures if '-n' was set", STRING, false);
	parpars.registerParameter("n", "number of worst structures to collect", STRING, false);

	parpars.setToolManual("Checks how well the found covalent bond lengths and "
												"vdw-distances agree with known standard parameters");

	parpars.parse(argc, argv);
	
	// -n and -o always need to be specified together:
	if( parpars.has("n") || parpars.has("o") )
		if(  !parpars.has("n") || !parpars.has("o") )
		{
			Log << "If the flag '-n' you also have to specify an outputfile via '-o' "<<endl;
			Log << "or if a output file was specified you also need to give the number of"<<endl;
			Log<<" worst structures to be printed with '-n'."<<endl;
			exit(0);
		}
	
	
	/// C O D E ##################################
	/// 0.) setup the analyzer:
	/// -----------------------------------------------------------------
	list< pair<AtomContainer*, float> > x_worst_structures;
	
	int collect = 0;
	if ( parpars.has("n") )
		collect = parpars.get("n").toInt();
	
	CSSAnalyzer analyzer( x_worst_structures, collect);
	
	
	/// 1.) read and analyze every structure:
	/// -----------------------------------------------------------------
	SDFile in_file(parpars.get("i"), ios::in);
	Log<<"Reading and evaluating input file: "<<endl<<in_file.getName()<<endl;

	int total_molecules = 0;
	AtomContainer* tmp = in_file.read();
	
	while(tmp)
	{
		if ( !analyzer.addStructure( *tmp ) )
			delete tmp; // structure was not added to the x_worst...
		
		total_molecules++;
		
		tmp = in_file.read(); // next loop iteration
	}
	in_file.close();
	
	/// 2.) report output:
	/// -----------------------------------------------------------------
	Log<<"...done analysing"<<endl<<endl<<endl;

	Log << "Reporting faults ( avg severity / worst severity / total occurences / total molecules):" << endl;
	Log << "Total molecules: " << total_molecules << endl << endl;
	Log<< "clashes: " << analyzer.getAverageCSS() << " / " << analyzer.getWorstCSS() << " / "
		 << analyzer.getNumberClashes() << " / "<< analyzer.getNumberClashingStructures() << endl;
	
	/// 3.) write the ugliest structures out
	/// -----------------------------------------------------------------
	if(parpars.has("o"))
	{
		cout<<"writing "<< x_worst_structures.size()<<" worst structures"<<endl;
		SDFile outfile(parpars.get("o"), ios::out);
		
		for(list< pair<AtomContainer*,float> >::iterator it = x_worst_structures.begin(); 
				it != x_worst_structures.end(); ++it )
		{
			it->first->setProperty("clash_score", it->second);
			outfile << * it->first;
			delete it->first;
		}
	}
}
