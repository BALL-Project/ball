// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/connectivityBase.h>
#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/bond.h>

#include <queue>
#include <numeric>

using namespace std;

#define BALL_QSAR_CONNECTIVITYBASE_DEBUG
#undef  BALL_QSAR_CONNECTIVITYBASE_DEBUG

namespace BALL
{
	ConnectivityBase::ConnectivityBase()
		:	Descriptor()
	{
	}

	ConnectivityBase::ConnectivityBase(const ConnectivityBase& cb)
		:	Descriptor(cb)
	{
	}

	ConnectivityBase::ConnectivityBase(const String& name)
		:	Descriptor(name)
	{
	}

	ConnectivityBase::ConnectivityBase(const String& name, const String& unit)
		:	Descriptor(name, unit)
	{
	}

	ConnectivityBase& ConnectivityBase::operator = (const ConnectivityBase& cb)
	{
		this->setName(cb.getName());
		this->setUnit(cb.getUnit());
		return *this;
	}

	ConnectivityBase::~ConnectivityBase()
	{
	}
	
	bool ConnectivityBase::isValid_(AtomContainer& ac)
	{
		static HashMap<Handle, PreciseTime> mod_times;
		PreciseTime last_mod = ac.getModificationTime(); 
		Handle mol_handle = ac.getHandle();
		if (mod_times.has(mol_handle))
		{
			if (mod_times[mol_handle] == last_mod)
			{
				#ifdef BALL_QSAR_CONNECTIVITYBASE_DEBUG
				cerr << ">> ConnectivityBase::isValid: molcule valid!" << endl;
				#endif
				return true;
			}
			else
			{
				mod_times[mol_handle] = last_mod;
				#ifdef BALL_QSAR_CONNECTIVITYBASE_DEBUG
				cerr << ">> ConnectivityBase::isValid: atom container not valid, modified!" << endl; 
				#endif
				return false;
			}
		}
		else
		{
			mod_times.insert(std::make_pair(mol_handle, last_mod));
			#ifdef BALL_QSAR_CONNECTIVITYBASE_DEBUG
			cerr << ">> ConnectivityBase::isValid: atom container not valid, first call!" << endl;
			#endif
			return false;
		}
	}
	
	void ConnectivityBase::computeAllDescriptors(AtomContainer& ac)
	{	
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
	}

	void ConnectivityBase::calculate_(AtomContainer& ac)
	{
		// we need aromaitcity detected and the number of heavy bonds
		NumberOfHeavyBonds nhb;
		ac.apply(nhb);

		//cerr << "ConnectivityBase::calculate_(AtomContainer& ac)" << endl;
		// iterators needed in this function
		AtomConstIterator atom_it;
		Atom::BondConstIterator bond_it;

		// hashmap to map the atoms to a size for direct matrix access
		// indices from [0-num_heavy_atoms]  are heavy atoms rest hydrogen
		HashMap<const Atom*, Size> index_map;
		Size num_heavy_atoms = 0;
		for (atom_it = ac.beginAtom(); atom_it != ac.endAtom(); ++atom_it)
		{
			if (atom_it->getElement() != PTE[Element::H])
			{
				index_map.insert(std::make_pair(&(*atom_it), num_heavy_atoms++));
			}
		}

		//cerr << "Calc dist matrix";
		vector<double> row_sums(num_heavy_atoms, 0.0);
		// do for every atom the min dists to all other atoms 
		// (single source shortest path for every single atom)
		for (atom_it = ac.beginAtom(); atom_it != ac.endAtom(); ++atom_it)
		{
			if (atom_it->getElement() != PTE[Element::H])
			{
				vector<double> dists(num_heavy_atoms, std::numeric_limits<double>::max());
				recursion_(dists, &(*atom_it), index_map);		
				// calc ro sum
				double sum(0);
				sum = accumulate(dists.begin(), dists.end(), sum);
				row_sums[index_map[&(*atom_it)]] = sum;
			}
		}	
		
		// compute Balanbans J indices
		atom_it = ac.beginAtom();
		bond_it = atom_it->beginBond();
		double value(0);
		BALL_FOREACH_BOND (ac, atom_it, bond_it)
		{
			const Atom * a1 = bond_it->getFirstAtom();
			const Atom * a2 = bond_it->getSecondAtom();
			if (a1->getElement() != PTE[Element::H] && a2->getElement() != PTE[Element::H])
			{
				value += pow(row_sums[index_map[a1]] * row_sums[index_map[a2]], -0.5);
			}
		}
		
		double balaban_j_idx(0);

		// here is not the NumberOfBonds descriptor used to keep this descriptor independend
		double num_heavy_bonds = ac.getProperty("NumberOfHeavyBonds").getDouble();
		
		double q = num_heavy_bonds;
		double mu = num_heavy_bonds-num_heavy_atoms+1;
		balaban_j_idx = q/(mu+1) * value;
		ac.setProperty("BalabanIndexJ", balaban_j_idx);
	}



	void ConnectivityBase::recursion_(vector<double>& dists, const Atom* source, HashMap<const Atom*, Size>& index_map)
	{
		//cerr << "ConnectivityBase::recursion_(...)" << endl;
		priority_queue<pair<double, const Atom*> > todo_pq;
		
		// the distances the bond order are associated with
		const double bond_value[] = { 0.0, 1.0, 0.5, 1.0/3.0, 0.0, 2.0/3.0 };

		// set the distance to source to itself to zero
		Size source_idx = index_map[source];
		dists[source_idx] = 0.0;
		
		// set the distances from the atoms next to the source atom
		for (Atom::BondConstIterator it = source->beginBond(); it != source->endBond(); it++)
		{
			if (it->getBoundAtom(*source)->getElement() != PTE[Element::H])
			{
				Size bound_idx = index_map[it->getBoundAtom(*source)];
				dists[bound_idx] = bond_value[it->getOrder()];
				todo_pq.push(std::make_pair(dists[bound_idx], it->getBoundAtom(*source)));
			}
		}
	
		// reach all nodes
		while (!todo_pq.empty())
		{
			// extract-min O(1)
			const Atom * min_atom;
			min_atom = todo_pq.top().second;
			Size min_idx = index_map[min_atom];
			todo_pq.pop();
			
			// relax O(1)
			Atom::BondConstIterator bond_it;
			for (bond_it = min_atom->beginBond(); bond_it != min_atom->endBond(); ++bond_it)
			{
				// if the bound atom is a hydrogen we don't process it
				if (bond_it->getBoundAtom(*min_atom)->getElement() != PTE[Element::H])
				{
					Size bound_idx = index_map[(bond_it->getBoundAtom(*min_atom))];
					// if distnace smaller set distance to new value, and add the atoms to the pq
					if (dists[bound_idx] > dists[min_idx] + bond_value[bond_it->getOrder()])
					{
						dists[bound_idx] = dists[min_idx] + bond_value[bond_it->getOrder()];
						todo_pq.push(std::make_pair(dists[bound_idx], bond_it->getBoundAtom(*min_atom)));
					}
				}
			}
		}
	}

} // namespace BALL
