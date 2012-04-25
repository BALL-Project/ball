#include <BALL/STRUCTURE/connectedComponentsProcessor.h>
#include <BALL/KERNEL/bond.h>

#include <vector>
#include <stack>
#include <set>

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

	Processor::Result ConnectedComponentsProcessor::operator () (AtomContainer& ac)
	{
		// for faster access, store all pointers to atoms
		std::vector<Atom*> atom_vector(ac.countAtoms());
		Size current_index = 0;

		for (AtomIterator at_it = ac.beginAtom(); +at_it; ++at_it, ++current_index)
		{
			at_it->setProperty("__CC_CONSIDER_ATOM", true);
			at_it->clearProperty("BALL_CC_INDEX");
			atom_vector[current_index] = &(*at_it);
		}

		// for each atom, the component_index contains the index of its 
		// connected component
		vector<Index> component_index(atom_vector.size(), -1);

		// now calculate all connected components in the graph
		// formed by atoms and bonds of the system

		// index of the current connected component
		current_index = 0;

		AtomIterator start_atom = ac.beginAtom();
		// search components until all atoms have been considered
		while (+start_atom)
		{
			// jump over all atoms that are already assigned a component
			while (+start_atom && start_atom->hasProperty("BALL_CC_INDEX"))
				++start_atom;

			if (+start_atom)
			{
				// create a new component
				components_.push_back(Component());

				// create a stack containing all atoms to be examined for this component
				std::stack<Atom*> atom_stack;

				// our start atom is the first to be considered and is marked, too
				atom_stack.push(&(*start_atom));
				start_atom->setProperty("BALL_CC_INDEX", current_index);
				components_[current_index].push_back(&(*start_atom));

				// never examine this atom again as start atom
				++start_atom;

				// calculate the connected component
				while (!atom_stack.empty())
				{
					// check all bonds of this atom and remove it from the stack
					Atom* current_atom = atom_stack.top();
					atom_stack.pop();

					Atom::BondIterator bond_it = current_atom->beginBond();
					for (; +bond_it; ++bond_it)
					{
						// add the atom if it is not marked yet
						// NOTE: if the bond points outside the current atom container,
						//       e.g., if we are applied to a Residue and have a bond to
						//       the next one, we ignore it here, so that we do not step
						//			 out of bounds of ac
						Atom* partner = bond_it->getPartner(*current_atom);
						if (partner->hasProperty("__CC_CONSIDER_ATOM")) // otherwise, it points outside ac
						{
							if (!partner->hasProperty("BALL_CC_INDEX"))
							{
								// remember this atom in the stack and store its component index
								atom_stack.push(partner);
								partner->setProperty("BALL_CC_INDEX", current_index);
								components_[current_index].push_back(partner);
							}
						}
					}
				}

				// done with this component, increase the component counter
				++current_index;
			}
		}

		// clear our helper property
		for (AtomIterator at_it = ac.beginAtom(); +at_it; ++at_it)
			at_it->clearProperty("__CC_CONSIDER_ATOM");

		// We always search for connected components in the uppermost
		// AtomContainer we are given. So we are done now, since the
		// connected components of all children have already been
		// computed.
		return Processor::BREAK;
	}

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

  ConnectedComponentsProcessor::ComponentVector& ConnectedComponentsProcessor::getComponents()
	{
		return components_;
	}

	const ConnectedComponentsProcessor::ComponentVector& ConnectedComponentsProcessor::getComponents() const
	{
		return components_;
	}

	Size ConnectedComponentsProcessor::splitIntoMolecules(System& to_split)
	{
		// first, compute the connected components
		to_split.apply(*this);

		// these will be removed later...
		std::set<Molecule*> to_delete;

		// for each connected component...
		for (Size current_component_index = 0; 
						  current_component_index < components_.size(); 
							++current_component_index)
		{
			Component& current_component = components_[current_component_index];

			// insert a new molecule into the system
			Molecule *current_mol = new Molecule;
			to_split.insert(*current_mol);

			current_mol->setName(to_split.getName()+"_comp_"+String(current_component_index));

			for (Size current_atom_index = 0;
								current_atom_index < current_component.size();
								++current_atom_index)
			{
				to_delete.insert(current_component[current_atom_index]->getMolecule());
				current_mol->insert(*(current_component[current_atom_index]));
			}
		}

		// now clean up a little...
		std::set<Molecule*>::iterator del_it = to_delete.begin();
		for (; del_it != to_delete.end(); ++del_it)
		{
			delete *del_it;
		}

		return getNumberOfConnectedComponents();
	}
}
