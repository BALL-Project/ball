// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include "rmsdBinner.h"
#include "base.h"

using namespace BALL;
using namespace std;

RMSDBinner::RMSDBinner(bool star_align, float threshold, int limit):
	_threshold(threshold),
	_variant_limit(limit),
	_use_star_align(star_align)
{}

RMSDBinner::~RMSDBinner()
{}


void RMSDBinner::addMolecule(const String &key, AtomContainer &molecule)
{
	all_sorted = false;
	
	insertMoleculeToLib( key, molecule);
}


map <String, vector< pair<AtomContainer*, int> > >::iterator RMSDBinner::begin()
{
	if( !all_sorted )
	{
		sortAll();
		all_sorted = true;
	}
	return _map_of_bins.begin();
}


map <String, vector< pair<AtomContainer*, int> > >::iterator RMSDBinner::end()
{
	if( !all_sorted )
	{
		sortAll();
		all_sorted = true;
	}
	return _map_of_bins.end();
}

int RMSDBinner::size()
{
	return _map_of_bins.size();
}


void RMSDBinner::insertMoleculeToLib(const String &key, AtomContainer &molecule)
{
	// simple case: a new entry:
	if( _map_of_bins.find(key) == _map_of_bins.end() )
	{
		_map_of_bins[key].push_back( make_pair(&molecule, 1) );
		return;
	}
	// entry exists:
	else
	{
		insertMoleculeIntoBins(_map_of_bins[key], molecule);
	}
}


void RMSDBinner::insertMoleculeIntoBins(vector<pair<AtomContainer *, int> > &bins, AtomContainer &molecule)
{
	if(_variant_limit > 0)
	{
		// cut down if at least 100 entries exist. When you do, then filter with the actual limit
		int limit = ( (_variant_limit*10) > 100) ? (_variant_limit*10) : 100;
		
		if ( bins.size() > limit )
		{ 
			filter2TopX( bins, _variant_limit);
		}
	}
	
	vector<pair<AtomContainer *, int> >::iterator bit;
	
	for( bit = bins.begin(); bit != bins.end(); ++bit)
	{
		// not different enough: delete this instance
		if( _getRMSD(molecule, *bit->first) <= _threshold )
		{
			delete &molecule;
			
			bit->second += 1;
			return;
		}
	}
	// must be a new shape: add to the list
		bins.push_back( make_pair( &molecule, 1) );
}


float RMSDBinner::_getRMSD(AtomContainer &mol1, AtomContainer &mol2 )
{
	if( mol1.countAtoms()!= mol2.countAtoms())
	{
		cout <<"ERROR: in RMSDBinner two molecules had different size: "<<endl;
		cout<< "Mol1: "<<LigBase::printInlineMol(&mol1)<<endl;
		cout<< "Mol2: "<<LigBase::printInlineMol(&mol2)<<endl;
	}
	if( _use_star_align )
	{
		_star_aligner.setMolecules( mol1, mol2);
	
		return _star_aligner.bestRMSD();
	}
	else
	{
		if(mol1.countAtoms() < 3)
		{
			_star_aligner.setMolecules(mol1, mol2);
			
			return _star_aligner.bestRMSD();
		}
		else
		{
			AtomBijection bj;

			bj.assignTrivial(mol1, mol2);
			if(bj.size() < 3)
			{
				cout <<"ERROR: in RMSDBinner bijection was smaller 3: "<<endl;
				cout<< "Mol1: "<<LigBase::printInlineMol(&mol1)<<endl;
				cout<< "Mol2: "<<LigBase::printInlineMol(&mol2)<<endl;
			}
			
			pair<Matrix4x4, double> rmsd = RMSDMinimizer::computeTransformation(bj);
		
			return (float)rmsd.second;
		}
	}
}


void RMSDBinner::filter2TopX(vector<pair<AtomContainer *, int> > &mol_list, const int& num_top)
{
	if( !all_sorted )
	{
		sort(mol_list.begin(), mol_list.end(), comparator);
	}
	
	vector<pair<AtomContainer *, int> > new_list;
	
	for(int i = 0; i < num_top; i++)
	{
		new_list.push_back( mol_list[i] );
	}
	
	// delete remaining items:
	for(int i = num_top; i < mol_list.size(); i++)
	{
		delete mol_list[i].first;
	}
	
	mol_list.swap( new_list );
}


void RMSDBinner::sortAll()
{
	if( !all_sorted )
	{
		map <String, vector< pair<AtomContainer*, int> > >::iterator mit;
		
		for(mit = _map_of_bins.begin(); mit != _map_of_bins.end(); ++mit)
		{
			vector< pair<AtomContainer*, int> >* bin_list = & mit->second;
			
			sort(bin_list->begin(), bin_list->end(), comparator);
		}
		
		all_sorted = true;
	}
}
