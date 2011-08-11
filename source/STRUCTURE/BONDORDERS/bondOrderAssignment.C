#include <BALL/STRUCTURE/BONDORDERS/bondOrderAssignment.h>
#include <BALL/STRUCTURE/assignBondOrderProcessor.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/molecule.h>

namespace BALL
{

	BondOrderAssignment::BondOrderAssignment(AssignBondOrderProcessor* abop)
		: PartialBondOrderAssignment(abop),
			valid(false),
			bond_order_map(),
			number_of_virtual_hydrogens(),
			atoms_to_delete(),
			total_charge(0.),
			node_expansions(0),
			queue_size(0),
			ac(NULL)
	{
	}

	BondOrderAssignment::~BondOrderAssignment()
	{
	}

	void BondOrderAssignment::clear()
	{
		// NOTE: do *not* clear the abop!
		valid = false;
		bond_order_map.clear();	
		number_of_virtual_hydrogens.clear();
		atoms_to_delete.clear();
		total_charge = 0.;
		node_expansions = 0;
		queue_size = 0;
		ac = NULL;
	}

	bool BondOrderAssignment::apply()
	{
		if (valid)
		{
			// we assume, that the AtomContainer is valid and the correct one! 

			// delete all former VIRTUAL BONDs
			// did we already apply a solution?
			if (abop->last_applied_solution_>=0)
			{	
				for (Size j=0; j < abop->solutions_[abop->last_applied_solution_].atoms_to_delete.size(); j++)
				{
					//NOTE: all adjacent bonds of these atoms will be deleted automatically
					delete(abop->solutions_[abop->last_applied_solution_].atoms_to_delete[j]); 
				}
			}

			// set all the original bonds to the assigned order
			AtomIterator a_it = ac->beginAtom();
			Atom::BondIterator b_it = a_it->beginBond();

			HashMap<Bond*, int> tmp_bond_orders = bond_order_map;

			BALL_FOREACH_BOND(*ac, a_it, b_it)
			{
				HashMap<Bond*, int>::Iterator it = tmp_bond_orders.find(&*b_it);
				if (it != bond_order_map.end())
				{
					b_it->setOrder(it->second);
				}
			}

			// for each virtual bond add the corresponding number of hydrogens
			// NOTE: the positions of the hydrogens are kind of randomly!
			atoms_to_delete.clear();

			HashMap<Atom*, int> tmp_virtual_hydrogens = number_of_virtual_hydrogens;

			int num_of_atoms = ac->countAtoms()+1;

			vector<Vector3> pos;
			pos.push_back(Vector3(1.,0.,0.));
			pos.push_back(Vector3(0.,1.,0.));
			pos.push_back(Vector3(0.,0.,1.));
			pos.push_back(Vector3(0.7,0.,0.7));

			a_it = ac->beginAtom();
			b_it = a_it->beginBond();
			BALL_FOREACH_ATOM(*ac, a_it)
			{
				HashMap<Atom*, int>::Iterator it = tmp_virtual_hydrogens.find(&*a_it);
				if (it != tmp_virtual_hydrogens.end())
				{
					Size missing_hydrogens = (Size)it->second;
					for (Size m=0; m < missing_hydrogens; m++)
					{
						// add a hydrogen atom  
						Atom* hydrogen = new Atom;
						hydrogen->setElement(PTE[Element::H]);
						hydrogen->setName(PTE[Element::H].getSymbol()+String(num_of_atoms));
						num_of_atoms++;
						hydrogen->setProperty("VIRTUAL__ATOM", true);
						if (a_it->getFragment())
						{
							a_it->getFragment()->insert(*hydrogen);
						}
						else
						{
							a_it->getMolecule()->insert(*hydrogen);
						}
						atoms_to_delete.push_back(hydrogen);

						// TODO: set the hydrogen's positions correctly!
						hydrogen->setPosition(a_it->getPosition() + pos[m] );

						// and a bond
						Bond* new_bond = a_it->createBond(*hydrogen);
						new_bond->setProperty("VIRTUAL__BOND", true);
						new_bond->setOrder(1);
					}
				}
			}
		}

		return valid;
	}


}
