// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef RMSDBINNER_H
#define RMSDBINNER_H

#include <BALL/STRUCTURE/LIGAND3DGEN/starAligner.h>

#include <BALL/STRUCTURE/RMSDMinimizer.h>

namespace BALL 
{
/**
	ATTENTION: if using this with molecule Fragments (not starLike Molecules and
	thus constructing not with a StarAligner) means that all molecules will be
	transformed for rmsd calculation. Currently this behaviour is non problematic
	and probably saves coding+computation time.
	
	PRECONDITION: for molecule fragments, we assume an already canonicalised list
	of atoms for each molecule!!!
	
 * @brief The RMSDBinner class clusters eiter molecules (thus also fragments)
 * or star like molecules according to their RMSD. If the RMSD between two
 * input molecules is below 'threshold' they are part of the same bin and thus
 * only the representative of that bin is kept. If a molecule is new or the RMSD
 * is larger than 'threshol' a new bin will be created.
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
	void addMolecule(const BALL::String& key, BALL::AtomContainer& molecule);
	
	/**
	 * Get iterators to the complete library
	 */
	std::map <BALL::String, std::vector< std::pair<BALL::AtomContainer*, int> > >::iterator begin();
	std::map <BALL::String, std::vector< std::pair<BALL::AtomContainer*, int> > >::iterator end();
	
	int size();
	
private:
	float compareDistances( BALL::AtomContainer& mol1, BALL::AtomContainer& mol2);

	float _getRMSD(BALL::AtomContainer &mol1, BALL::AtomContainer &mol2 );
	
	void insertMoleculeIntoBins(std::vector< std::pair<BALL::AtomContainer*, int> >& bins, BALL::AtomContainer& molecule);
	
	void filter2TopX(std::vector< std::pair<BALL::AtomContainer*, int> >& mol_list, const int &num_top=1);
	
	void insertMoleculeToLib(const BALL::String& key, BALL::AtomContainer& molecule);
	
	void sortAll();
	
	static bool comparator(std::pair<BALL::AtomContainer*, int>& a, std::pair<BALL::AtomContainer*, int> & b)
	{return b.second < a.second;}
	
	
	/**
	 *
	 *  P R I V A T E    F I E L D S
	 * 
	 */
	StarAligner _star_aligner; 
	
	float _threshold; // starting RMSD for different bins
	std::map <BALL::String, std::vector< std::pair<BALL::AtomContainer*, int> > > _map_of_bins;
	
	int _variant_limit;
	bool all_sorted;
	bool _use_star_align;
};

} // End Namespace BALL
#endif // RMSDBINNER_H
