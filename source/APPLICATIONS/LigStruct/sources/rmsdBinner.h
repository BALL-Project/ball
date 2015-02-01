// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef RMSDBINNER_H
#define RMSDBINNER_H

#include "../sources/starAligner.h"
//#include "../sources/canonicalizer.h" // important for molecules only

#include <BALL/KERNEL/atomContainer.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/STRUCTURE/RMSDMinimizer.h>

#include <util.h>
#include <algorithm>

#include <boost/container/map.hpp>

using namespace BALL;
using namespace std;

/**
	ATTENTION: if using this with molecule Fragments (not starLike Molecules and
	thus constructing not with a StarAligner) means that all molecules will be
	transformed for rmsd calculation. Currently this behaviour is no problem and 
	probably saves coding+computation time.
	
	PRECONDITION: for molecule fragments, we assume a already canonicalised list
	of atoms for each molecule!!!
	
 * @brief The RMSDBinner class
 */
class RMSDBinner
{
public:
	RMSDBinner(bool star_align=true, float threshold = 0.2, int limit=100 );
	
	~RMSDBinner();
	
	/**
	 * @brief addMolecule, add molecules to the internals of the RMSDBinner
	 * and automatically let him handle the binning
	 * @param key
	 * @param molecule
	 */
	void addMolecule(const String& key, AtomContainer& molecule);
	
	/**
	 * Get iterators to the complete library
	 */
	map <String, vector< pair<AtomContainer*, int> > >::iterator begin();
	map <String, vector< pair<AtomContainer*, int> > >::iterator end();
	
	int size();
	
private:
	
	float _getRMSD(AtomContainer &mol1, AtomContainer &mol2 );
	
	void insertMoleculeIntoBins(vector< pair<AtomContainer*, int> >& bins, AtomContainer& molecule);
	
	void filter2TopX(vector< pair<AtomContainer*, int> >& mol_list, const int &num_top=1);
	
	void insertMoleculeToLib(const String& key, AtomContainer& molecule);
	
	void sortAll();
	
	static bool comparator(pair<AtomContainer*, int>& a, pair<AtomContainer*, int> & b)
	{return b.second < a.second;}
	
	
	/**
	 *
	 *  P R I V A T E    F I E L D S
	 * 
	 */
	StarAligner _star_aligner; 
	
	float _threshold; // starting RMSD for different bins
	map <String, vector< pair<AtomContainer*, int> > > _map_of_bins;
	
	int _variant_limit;
	bool all_sorted;
	bool _use_star_align;
};

#endif // RMSDBINNER_H
