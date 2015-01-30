// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/commandlineParser.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/STRUCTURE/structureMapper.h>
#include <BALL/STRUCTURE/geometricTransformations.h>

#include <BALL/MATHS/angle.h>
#include <BALL/MATHS/vector3.h>
#include <BALL/MATHS/matrix44.h>

#include <vector>
#include <util.h>
#include <algorithm>
#include <limits>

#include <boost/unordered_map.hpp>
#include <boost/unordered_set.hpp>

#include "../sources/starAligner.h"
#include "../sources/rmsdBinner.h"

using namespace BALL;
using namespace std;


/// Compare a pair of (int , molecule*) according to the int field
/// ----------------------------------------------------------------------------
bool comparator(pair<int, Molecule*>& a, pair<int, Molecule*>& b)
{
	return b.first < a.first;
}

/// Find Bins for a class of connection keys depending on an RMSD threshold
/// ----------------------------------------------------------------------------
void getBins(vector< pair<int, Molecule*> >& bins, vector<Molecule*>& mols)
{
	StarAligner aligner;
	const float THRESHOLD = 0.2;
	/// align each molecule with each other, if they are identical insert to map:
//	boost::unordered_map< Molecule*, int > local_bins;
	map< Molecule*, int > local_bins;
	
	// check if we have more than one template available
	if(mols.size() <2){
		if(mols.size() == 1)
		{
			bins.push_back( make_pair(1, mols[0]) );
		}
		else{
			Log<<"ERROR: got a key without any molecule!"<<endl;
			exit(EXIT_FAILURE);
		}
		return;
	}
	
	vector<Molecule*>::iterator it1, it2;
	for(it1 = mols.begin(); it1 != mols.end(); it1++)
	{
		// do not test molecules AS FISRT molecule that were already replaced
		if( local_bins.find(*it1) != local_bins.end() )
			continue;
		
		// create a 'bin' for this molecule
		local_bins[*it1] += 1;
		
		for(it2 = it1 + 1; it2 != mols.end(); it2++)
		{
			// do not test molecules AS SECOND molecule that were already replaced
			if( local_bins.find(*it2) != local_bins.end() )
				continue;
			
			// align mol2 to match mol1 (special alignment for star-molecules) and get RMSD
			aligner.setMolecules(**it2, **it1);
			aligner.align();
			float rmsd = aligner.getMinRMSD(*it2, *it1);
			
			if ( rmsd < THRESHOLD)
			{
				delete *it2;
				*it2 = *it1; // replace pointer by the group representative
				
				local_bins[*it1] += 1;
			}
		}
	}

	// transform local bins to result 'bins':
//	boost::unordered_map< Molecule*, int >::iterator it = local_bins.begin();
	map< Molecule*, int >::iterator it = local_bins.begin();
	for(; it != local_bins.end(); it++)
	{
		bins.push_back( make_pair((*it).second, (*it).first) );
	}
}
/// ################# M A I N #################
int main(int argc, char* argv[])
{
	CommandlineParser parpars("connectionLibFilter", "select best templates", 0.1, String(__DATE__), "Preparation");
	parpars.registerParameter("i", "input SDF", INFILE, true);
	parpars.registerParameter("o", "output SDF", OUTFILE, true);
	
	parpars.registerFlag("unique", "only output one fragment for each topology");

	parpars.setSupportedFormats("i","sdf");
	parpars.setSupportedFormats("o","sdf");

	String manual = "For creating a connection library.";
	parpars.setToolManual(manual);

	parpars.parse(argc, argv);
	
/// START of CODE#############################################################

///1.) Read all molecules and sort to their respective connection-class:
	
	map <String, vector<Molecule*> > connection_classes;
	
	Log << "Reading input connections from:"<<endl;
	Log << String(parpars.get("i"))<<endl;
	SDFile infile(parpars.get("i"), ios::in); // OPEN IN-FILE
	Molecule* work_frag =	infile.read();
	while ( work_frag )
	{
		String key = work_frag->getProperty("key").getString();
		connection_classes[key].push_back(work_frag);

		work_frag = infile.read(); // read next
	}
	Log << "Read "<< connection_classes.size()<< " connection classes" << endl;
	
	infile.close();
	Log << "done reading."<<endl;
	
	
///2.) Loop over all classes to find the 'best representative':
///
	SDFile outfile(String(parpars.get("o")), ios::out);// OPEN OUT-FILE
//	boost::unordered_map <String, vector<Molecule*> >::iterator cla_it = connection_classes.begin();
	map <String, vector<Molecule*> >::iterator cla_it = connection_classes.begin();
	for(; cla_it != connection_classes.end(); cla_it++)
	{
		///2.1) Get sets of identical molecules
		vector< pair<int, Molecule*> > bins;
		getBins(bins, (*cla_it).second );
		
		///2.2) Select bin with most frequent molecule
		sort(bins.begin(), bins.end(), comparator);
		
		///2.3) write most frequent molecule to outfile
		Log<<" bin "<<(bins[0].second)->getProperty("key").getString()<<" contains: "<<bins[0].first<<endl;
		outfile<< *(bins[0].second);
		
		///2.4) free memory:
		//connection_classes.erase(cla_it);
		for (int i = 0; i < bins.size(); i++){
			delete bins[i].second;
		}
	}
	Log <<endl<<"wrote " << connection_classes.size()<<" structures"<<endl;
	outfile.close();
}
