#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/forEach.h>
#include <map>
#include <vector>

namespace BALL
{
	ConnectedComponentsProcessor::ConnectedComponentsProcessor()
	{
	}

	ConnectedComponentsProcessor::~ConnectedComponentsProcessor()
	{
	}

	bool ConnectedComponentsProcessor::start()
	{
		clear();
		return true;
	}

	
	/// Computes processor intern "components_", which contains the molecules
	/// separate molecules
	Processor::Result ConnectedComponentsProcessor::operator () (AtomContainer& ac)
	{
		// if possible one should try to refactor this, so that we can use the
		// disjoint set without having to rely on the mapping. The mappings take
		// about 30-50% of the whole computation time in this method!!!
		const int num_atoms = ac.countAtoms();
		// mapping: atom to int, and int to atom:
		std::vector <Atom*> i_to_atm(num_atoms, NULL);
		std::map< Atom*, int> atm_to_i; // std::map was most efficient, compared with hashmaps
		
		// create empty disjoint set:
		std::vector <int> atm_rank(num_atoms);
		std::vector <int> atm_parent(num_atoms);
		
		DisjointSet dset(&atm_rank[0], &atm_parent[0]);

		//------------------------------------------------
		// Populate the dset with elements and
		// create a mapping from atom pointer to vertex (for adding edges later)
		// and from int to atom-pointer
		AtomIterator atom_it;
		int i1 = 0;
		BALL_FOREACH_ATOM(ac, atom_it)
		{
			dset.make_set(i1); // enter singleton sets for every atom

			// create the necessary mappings:
			atm_to_i[&*atom_it] = i1;
			i_to_atm[i1] = (&*atom_it);
			
			i1++;
		}

		//------------------------------------------------
		// Add edges from the atom container (ac) to the disjoint set
		// (iterate over all unique bonds)
		int v1, v2;
		Atom::BondIterator bond_iter;
		BALL_FOREACH_INTRABOND(ac, atom_it, bond_iter)
		{
			v1 = atm_to_i[bond_iter->getFirstAtom()];
			v2 = atm_to_i[bond_iter->getSecondAtom()];

			dset.union_set(v1, v2); // union the elements that are connected
		}

		//------------------------------------------------
		// Iterate through the component indices
		// split according to the ds-information:
		std::vector <int> pnames (num_atoms, -1); // mapps representative parents
																							// to component numbers
		int num = 0;
		int par = -1;
		int idx = -1;
		for (int i = 0; i < num_atoms; i++)
		{
			par = dset.find_set(i); // get parent of element 'i'
			idx = pnames[par];
			if (idx != -1)
			{ // add element to existing component
				components_[idx].push_back(i_to_atm[i]);
			}
			else
			{ // create new component for current element:
				Component tmp;
				tmp.push_back(i_to_atm[i]);
				components_.push_back(tmp);
				pnames[par] = num;
				num++;
			}
		}
		
		return Processor::BREAK;
	}// END OF PROCESSOR


	bool ConnectedComponentsProcessor::finish()
	{
		return true;
	}

	void ConnectedComponentsProcessor::clear()
	{
		components_.clear();
	}
	
	Size ConnectedComponentsProcessor::getNumberOfConnectedComponents()
	{
		return components_.size();
	}
	
	void ConnectedComponentsProcessor::getComponents(ComponentVector &comp)
	{
		comp.assign(components_.begin(), components_.end());
	}
	

	/// Return the component with most atoms
	void ConnectedComponentsProcessor::getLargestComponent(Molecule& result)
	{
		size_t pos = 0, max = 0;
		for(size_t i = 0; i < components_.size(); i++)
		{
			size_t tmp = components_[i].size() ;
			if (tmp > max)
			{
				max = tmp;
				pos = i;
			}
		}

		for (size_t i = 0; i < components_[pos].size(); i++)
		{
			result.insert( *(components_[pos][i]) );
		}
	}
	

	void ConnectedComponentsProcessor::getMinAtomsComponents(MolVec& result, size_t min)
	{
		for(size_t i = 0; i < components_.size(); i++)
		{
			size_t siz = components_[i].size();
			if (siz >= min)
			{
				Molecule tmp = Molecule();
				
				for(size_t k = 0; k < siz; k++)
				{
					tmp.insert( *(components_[i][k]) );
				}
				result.push_back(tmp);
			}
		}
	}
	
	void ConnectedComponentsProcessor::getAllComponents(MolVec& results)
	{
		for(size_t i = 0; i < components_.size(); i++)
		{
			Molecule tmp = Molecule();
			
			for(size_t k = 0; k < components_[i].size(); k++)
			{
				tmp.insert( *(components_[i][k]) );
			}
			results.push_back(tmp);
		}
	}

}
