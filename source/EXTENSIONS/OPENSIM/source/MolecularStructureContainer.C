#include "../include/MolecularStructureContainer.h"

namespace BALL
{
	namespace VIEW
	{

		MolecularStructureContainer::MolecularStructureContainer():
				next_atom_index_(0),
				next_bond_index_(0),
				readWriteLock_()
		{
			
		}

		/*
		* Gets a reference to an atom container
		* Util function
		*/
		AtomContainer* MolecularStructureContainer::getAtomContainer()
		{
			std::list<Composite*> composite_list = MainControl::getInstance(0)->getMolecularControlSelection(); 				
			if (composite_list.size() != 1)
			{
				Log.error() << " MolecularStructureContainer: Please highlight only one system!" << std::endl;
				return NULL;
			}
			
			AtomContainer* ai = dynamic_cast<AtomContainer*>(*(composite_list.begin()));

			if (!ai)
			{
				Log.error() << "MolecularStructureContainer: The highlighted object is not an atom container!" << std::endl;
			}
			
			return ai;
		}

		/*
		* Adds atom into an existing ballview molecular structure(system)which is an atom container
		* All parameters are required.
		* Note : Currently , atom indices are kept track in hash map,but
		* there is a problem when removing atom indices from the hashmaps.
		*/
		Index MolecularStructureContainer::addAtom( const String& element,
				                       const Vector3& position, float radius, const ColorRGBA& /*color*/)
		{
			if (radius < 0)
			{
				Log.error() << " MolecularStructureContainer: Please provide radius greater than zero !" << std::endl;
				return -1;
			}

			PDBAtom* atom = new PDBAtom(PTE[element], element);
			Index atom_index_ =  -1;

			if (atom)
			{
				atom->setPosition(position);
				atom->setRadius(radius);

				// ToDo
				// How to do setting color ( can not be done ?)

				AtomContainer* ai = getAtomContainer();

				if (!ai) 
					return -1;
				else
				{
					ai->insert(*atom);

					CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
					
					qApp->postEvent(getMainControl(), new MessageEvent(cm));

					// Do I need it again? yes -> again check after inserted into BALLView
					// How to check if atom inserted?
					if (atom)
					{
						readWriteLock_.lockForWrite();
						atom_index_ = next_atom_index_;

						// Keep track of atom indices
						index_to_atom_[atom_index_] = atom;
						atom_to_index_[atom]        = atom_index_;

						next_atom_index_++;
						
						while (index_to_atom_.has(next_atom_index_))
						{
							++next_atom_index_;
						}

						readWriteLock_.unlock();
					}
				}
			}

			return atom_index_;
		}

		/* Adds a new atom into the hash map and returns its index.
		 */
		Index MolecularStructureContainer::hashAtom(Atom* new_atom)
		{
			readWriteLock_.lockForWrite();

			Index atom_index = next_atom_index_;

			index_to_atom_[atom_index] = new_atom;
			atom_to_index_[new_atom]   = atom_index;

			atom_hash_times_[next_atom_index_] = PreciseTime::now();

			while (index_to_atom_.has(next_atom_index_))
			{
				++next_atom_index_;
			}

			readWriteLock_.unlock();

			return atom_index;
		}

		/* Adds a new bond into the hash map and returns its index.
		 */
		Index MolecularStructureContainer::hashBond(Bond* new_bond)
		{
			readWriteLock_.lockForWrite();

			Index bond_index = next_bond_index_;

			index_to_bond_[bond_index] = new_bond;
			bond_to_index_[new_bond]   = bond_index;

			while (index_to_bond_.has(next_bond_index_))
			{
				++next_bond_index_;
			}

			readWriteLock_.unlock();

			return bond_index;
		}

		Index MolecularStructureContainer::getAtomIndex(Atom const* atom) const
		{
			Index result = -1;

			readWriteLock_.lockForRead();

			// ugly, but necessary
			if (atom_to_index_.has(const_cast<Atom*>(atom)))
				result = atom_to_index_[const_cast<Atom*>(atom)];

			readWriteLock_.unlock();

			return result;
		}

		Index MolecularStructureContainer::getBondIndex(Bond const* bond) const
		{
			Index result = -1;

			readWriteLock_.lockForRead();

			// ugly, but necessary
			if (bond_to_index_.has(const_cast<Bond*>(bond)))
				result = bond_to_index_[const_cast<Bond*>(bond)];

			readWriteLock_.unlock();

			return result;
		}

		bool MolecularStructureContainer::needsUpdate(Index atom_index)
		{
			Index result = false;

			readWriteLock_.lockForWrite();

			if (atom_hash_times_.has(atom_index))
			{
				PreciseTime hash_time = atom_hash_times_[atom_index];

				if (index_to_atom_[atom_index]->getModificationTime() > hash_time)
				{
					result = true;
					// we assume the update will follow right after this call
					atom_hash_times_[atom_index] = PreciseTime::now();
				}
			}
Log.info() << "Needs update? " << result << std::endl;
			readWriteLock_.unlock();

			return result;
		}

		/*
		* Adds a bond between two existing atoms into an existing ballview molecular structure(system)which is an atom container
		* Required parameters are atom indices and bond information
		*/
		Index MolecularStructureContainer::addBondByAtomIndex(/*Index bond_index,*/ Index atom_index_one, Index atom_index_two, Size order)
		{
			if (!(index_to_atom_.has(atom_index_one) && index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot build a bond between non-existing  atoms!" << std::endl;
				return -1;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) 
				return -1;

			Atom* atom_one = index_to_atom_[atom_index_one];
			Atom* atom_two = index_to_atom_[atom_index_two];

			Index bond_index = -1;

			if (!atom_one->isBoundTo(*atom_two))
			{
				Bond* new_bond = atom_one->createBond(*atom_two);

				// set the order for the added bond
				new_bond->setOrder(order);	
				CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
				qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));

				readWriteLock_.lockForWrite();
				bond_index = next_bond_index_;

				// Keep track of added bond indices after bond addition
				index_to_bond_[bond_index] = new_bond;
				bond_to_index_[new_bond]   = bond_index;

				next_bond_index_++;
				while (index_to_bond_.has(next_bond_index_))
				{
					++next_bond_index_;
				}

				readWriteLock_.unlock();
			}

			return bond_index;
		}
		
		void MolecularStructureContainer::unhashAtom(Index atom_index)
		{
			readWriteLock_.lockForWrite();

			if (index_to_atom_.has(atom_index))
			{
				Atom* atom = index_to_atom_[atom_index];

				index_to_atom_.erase(atom_index);
				atom_hash_times_.erase(atom_index);
				atom_to_index_.erase(atom);
			}

			readWriteLock_.unlock();
		}

		void MolecularStructureContainer::unhashBond(Index bond_index)
		{
			readWriteLock_.lockForWrite();

			if (index_to_bond_.has(bond_index))
			{
				Bond* bond = index_to_bond_[bond_index];

				index_to_bond_.erase(bond_index);
				bond_to_index_.erase(bond);
			}

			readWriteLock_.unlock();
		}

		/*
		* Removes an existing atom from an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index
		*/
		void MolecularStructureContainer::removeAtomByIndex(Index atom_index)
		{
			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if (!index_to_atom_.has(atom_index))
			{
				Log.error() << "MolecularStructureContainer: cannot remove a non-existing atom!" << std::endl;
				return;
			}

			Atom* atom = index_to_atom_[atom_index];

			getMainControl()->remove(*atom);

			CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
			qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));

			readWriteLock_.lockForWrite();

			//Take care keep tracking of atom removal

			atom_to_index_.erase(atom);
			index_to_atom_.erase(atom_index);

			readWriteLock_.unlock();
		}
	
		/*
		* Removes an existing bond between existing atoms from an existing ballview molecular structure(system)which is an atom container
		* Required parameter is bond index, indices of atoms
		*/
		void MolecularStructureContainer::removeBondByAtomIndex(Index bond_index, Index atom_index_one, Index atom_index_two)
		{
			if (!(index_to_atom_.has(atom_index_one) && index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot remove the bond between non-existing  atoms!" << std::endl;
				return;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			Atom* atom_one = index_to_atom_[atom_index_one];
			Atom* atom_two = index_to_atom_[atom_index_two];

			Bond* bond = atom_one->getBond(*atom_two);

			Bond* bond_by_index = 0;
			if (index_to_bond_.has(bond_index))
			{
				bond_by_index = index_to_bond_[bond_index];
			}

			if (bond != bond_by_index)
			{
				Log.error() << "MolecularStructureContainer: inconsistency between OpenSim indices and BALLView indices detected!" << std::endl;
			}

			if (bond)
			{
				atom_one->destroyBond(*atom_two);

				CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
				qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
				readWriteLock_.lockForWrite();

				if (bond_by_index)
				{
					bond_to_index_.erase(bond_by_index);
					index_to_bond_.erase(bond_index);
				}
			}

			readWriteLock_.unlock();
		}

		
		/*
		* Updates an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index and necessary information of atoms to be updated.
		*/
		void MolecularStructureContainer::updateAtomByIndex(Index atom_index, const String& /*element*/,
					const Vector3& position /* ,float radius, float charge,  const Vector3& velocity,const Vector3& force*/)
		{
			if (!index_to_atom_.has(atom_index))
			{
				Log.error() << "MolecularStructureContainer: cannot update a non-existing atom!" << std::endl;
				return;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			Atom* atom = index_to_atom_[atom_index];
			atom->setPosition(position);
			
			// TODO: why are these commented out???

			//atom->setRadius(radius);
			//set charge
			//atom->setCharge(charge);
			//set velocity
			//atom->setVelocity(velocity);
			//set force
			//atom->setForce(force);
			//No way to change the color
			//atom->setColor(color);

			CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
			qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
		}

		/*
		* Updates a bond  between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices and necessary information of bond to be updated.
		*/
		void MolecularStructureContainer::updateBondByAtomIndex(Index atom_index_one, Index atom_index_two,Size order)
		{
			if (!(index_to_atom_.has(atom_index_one) && index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot remove the bond between non-existing  atoms!" << std::endl;
				return;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			Atom* atom_one = index_to_atom_[atom_index_one];
			Atom* atom_two = index_to_atom_[atom_index_two];

			Bond* bond = atom_one->getBond(*atom_two);

			if (bond)
			{
				bond->setOrder(order);
				CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
				qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
			}

			readWriteLock_.unlock();
		}

		/*
		* Selects an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::selectAtom(Index atom_index)
		{
			if (!index_to_atom_.has(atom_index))
			{
				Log.error() << "MolecularStructureContainer: cannot select a non-existing atom!" << std::endl;
				return;
			}

			Atom* atom = index_to_atom_[atom_index];

			AtomContainer* container = getAtomContainer();

			getMainControl()->selectCompositeRecursive((Composite *)atom);
			CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::SELECTED_COMPOSITE, true);
			qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
		}

		
		/*
		* Deselects an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::deselectAtom(Index atom_index)
		{
			if (!index_to_atom_.has(atom_index))
			{
				Log.error() << "MolecularStructureContainer: cannot select a non-existing atom!" << std::endl;
				return;
			}

			Atom* atom = index_to_atom_[atom_index];

			AtomContainer* container = getAtomContainer();

			getMainControl()->deselectCompositeRecursive((Composite *)atom);
			CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::SELECTED_COMPOSITE, true);
			qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
		}

		/*
		* Selects a bond between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::selectBond(Index /*atom_index_one*/, Index /*atom_index_two*/)
		{
			// ?? We cannot select bonds in BALLView, can we?
		}

		
		/*
		* Deselects a bond between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::deselectBond(Index /*atom_index_one*/, Index /*atom_index_two*/)
		{
			// ?? We cannot select bonds in BALLView, can we?
		}
	}
}
