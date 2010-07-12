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
				                       const Vector3& position, float radius, const ColorRGBA& color)
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
						Handle atom_identifier = atom->getHandle();
						atom_index_ = next_atom_index_;

						// Keep track of atom indices
						index_to_atom_[atom_index_] = atom_identifier;
						atom_to_index_[atom_identifier]  = atom_index_;
						handle_to_atom_[atom_identifier] =  new Atom(*atom,true);
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

		/*
		* Adds a bond between two existing atoms into an existing ballview molecular structure(system)which is an atom container
		* Required parameters are atom indices and bond information
		*/
		Index MolecularStructureContainer::addBondByAtomIndex(/*Index bond_index,*/ Index atom_index_one, Index atom_index_two, Size order)
		{
			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if ((!index_to_atom_.has(atom_index_one)) && (!index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot build a bond between non-existing  atoms!" << std::endl;
				return -1;
			}

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			/*if (!index_to_bond_.has(bond_index))
			{
				Log.error() << "MolecularStructureContainer : bond already existing between specified atoms!" << std::endl;
				return;
			}*/

			AtomContainer* ai = getAtomContainer();

			if (!ai) 
				return -1;

			Atom* atom_one_  = NULL;
			Atom* atom_two_  = NULL;
			Index bond_index_ =  -1;

			if (ai)
			{
				for (AtomIterator at_it = ai->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;

					if (atom)
					{
						Handle atom_identifier = atom->getHandle();
						Index atom_index_  = -1;

						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}

						if (atom_index_ != -1)
						{
							if (atom_index_ == atom_index_one)
							{
								atom_one_ = &*at_it;
							}
							else if (atom_index_ == atom_index_two )
							{
								atom_two_ = &*at_it;
							}
						}
					}
				}

				if (atom_one_ && atom_two_)
				{
					// ToDo - validate if they are null
					Bond* new_bond_ = atom_one_->createBond(*atom_two_);

					if (new_bond_)
					{
						// set the order for the added bond
						new_bond_->setOrder(order);	
						CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
						qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));

						// Do I need it again? yes -> again check after inserted into BALLView
						// How to check if bond inserted?
						if (new_bond_)
						{
							readWriteLock_.lockForWrite();
							Handle bond_identifier = new_bond_->getHandle();
							bond_index_ = next_bond_index_;

							// Keep track of added bond indices after bond addition
							index_to_bond_[bond_index_] = bond_identifier;
							bond_to_index_[bond_identifier]  = bond_index_;
							handle_to_bond_[bond_identifier] =  new Bond(*new_bond_,true);

							next_bond_index_++;
							while (index_to_bond_.has(next_bond_index_))
							{
								++next_bond_index_;
							}

							readWriteLock_.unlock();
						}
					}
				}
			}
			return bond_index_;
		}

		
		/*
		* Removes an existing atom from an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index
		*/
		void MolecularStructureContainer::removeAtomByIndex(Index atom_toberemoved_index)
		{
			bool atomRemoved = false;

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if (!index_to_atom_.has(atom_toberemoved_index))
			{
				Log.error() << "MolecularStructureContainer: cannot remove a non-existing atom!" << std::endl;
				return;
			}

			if (ai)
			{
				for (AtomIterator at_it = ai->beginAtom(); +at_it; ++at_it)
				{
					//Note TODO
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					if (atom)
					
					{
						Handle atom_identifier = atom->getHandle();
						Index atom_index_ = -1;

						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}

						if (atom_index_ != -1)
						{
							if (atom_index_ == atom_toberemoved_index)
							{
								getMainControl()->remove(*atom);
								
								CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
								qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
								
								readWriteLock_.lockForWrite();
								
								//Take care keep tracking of atom removal

								//atom_to_index_.erase(atom_to_index_.find(atom_identifier));
								atom_to_index_[atom_identifier] = -1;

								handle_to_atom_[atom_identifier] =  NULL;

								// Note
								// Not sure if this works
								//index_to_atom_.erase(index_to_atom_.find(atom_index));
								//index_to_atom_[atom_index] = NULL ; // is this needed ?

								readWriteLock_.unlock();
								atomRemoved = true;
								break;
								
							}
						}
					}
				}
			}
			if (!atomRemoved)
			{
				Log.error() << "MolecularStructureContainer: cannot remove a non-existing atom!" << std::endl;
				return;
			}
		}
	

		/*
		* Removes an existing bond between existing atoms from an existing ballview molecular structure(system)which is an atom container
		* Required parameter is bond index, indices of atoms
		*/
		void MolecularStructureContainer::removeBondByAtomIndex(Index bond_index, Index atom_index_one, Index atom_index_two)
		{

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if ((!index_to_atom_.has(atom_index_one)) && (!index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot remove the bond between non-existing  atoms!" << std::endl;
				return;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			Atom* atom_one_  = NULL;
			Atom* atom_two_ = NULL;

			if (ai)
			{
				for (AtomIterator at_it = ai->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					Index atom_index_ = -1;

					if (atom)
					{
						Handle atom_identifer = atom->getHandle();

						if (atom_to_index_.has(atom_identifer))
						{
							atom_index_ = atom_to_index_[atom_identifer];
						}
					}
					 
					if (atom_index_ != -1)
					{
						if (atom_index_ == atom_index_one)
						{
							atom_one_ = &*at_it;
						}
						else if (atom_index_ == atom_index_two)
						{
							atom_two_ = &*at_it;
						}
					}

				}

				if (atom_one_ && atom_two_)
				{
					// Take care keep tracking of bond removal
					// should be done before the bond is actually removed
					// Note : when removing bonds how to handle the atoms ?

					Bond * bond = atom_one_->getBond(*atom_two_);

					if (bond)
					{
						Handle bond_identifier = -1;
						if (index_to_bond_.has(bond_index))
						{
							bond_identifier = index_to_bond_[bond_index];
						}

						Index bondIndex = -1;

						if (bond_to_index_.has(bond_identifier) )  
						{
							bondIndex = bond_to_index_[bond_identifier];
						}

						if (bondIndex!= -1)
						{
							// ToDo - validate if they are null
							atom_one_->destroyBond(*atom_two_);

							CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
							qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
							readWriteLock_.lockForWrite();

							//bond_to_index_.erase(bond_to_index_.find(bond));
							
							bond_to_index_[bond_identifier] = -1;
							handle_to_bond_[bond_identifier] =  NULL;

							//index_to_bond_.erase(index_to_bond_.find(bond_index));

							readWriteLock_.unlock();
						}
					}
				}
			}
		}

		
		/*
		* Updates an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index and necessary information of atoms to be updated.
		*/
		void MolecularStructureContainer::updateAtomByIndex(Index atom_tobeupdate_index, const String& element,
					const Vector3& position /* ,float radius, float charge,  const Vector3& velocity,const Vector3& force*/)

		{

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if (!index_to_atom_.has(atom_tobeupdate_index))
			{
				Log.error() << "MolecularStructureContainer: cannot update a non-existing atom!" << std::endl;
				return;
			}

			/*if(radius < 0)
			{
				Log.error() << " MolecularStructureContainer: Please provide radius greter than zero !" << std::endl;
				return ;
			}*/

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			if (ai)
			{
				for (AtomIterator at_it = ai->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;

					if (atom)
					{
						Handle atom_identifier = atom->getHandle();
						Index atom_index_ = -1;

						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}

						if ((atom_index_ == atom_tobeupdate_index) && (atom_index_ != -1))
						{
							//Atom* atom = index_to_atom_[atom_index];	

							//ToDo
							//setType 
							//setTypeName
							
							atom->setPosition(position);
							
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

							break;
						}
					}
				}
			}
		}


		
		
		/*
		* Updates a bond  between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices and necessary information of bond to be updated.
		*/
		void MolecularStructureContainer::updateBondByAtomIndex(Index atom_index_one, Index atom_index_two,Size order)
		{

			// Note 
			// Not sure if this works
			if ((!index_to_atom_.has(atom_index_one)) && (!index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot update the bond between non-existing  atoms!" << std::endl;
				return;
			}

			AtomContainer* ai = getAtomContainer();

			if (!ai) return;

			Atom* atom_one_  = NULL;
			Atom* atom_two_ = NULL;

			if (ai)
			{
				for (AtomIterator at_it = ai->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					if (atom)
					{
						Index  atom_index_ = -1;
						Handle atom_identifier = atom->getHandle();
						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}

						if (atom_index_ != -1)
						{
							if (atom_index_ == atom_index_one)
							{
								atom_one_ = &*at_it;
							}
							else if (atom_index_ == atom_index_two)
							{
								atom_two_ = &*at_it;
							}
						}
					}
				}

				if (atom_one_ && atom_two_)
				{
					// ToDo - validate if they are null
					Bond* bond = atom_one_->getBond(*atom_two_);

					if (bond)
					{
						// ToDo
						//bond->setType(Type bond_type);
						
						bond->setOrder(order);
						CompositeMessage *cm = new CompositeMessage(*ai, CompositeMessage::CHANGED_COMPOSITE_HIERARCHY, true);
						qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
					}
				}
			}
		}

		

		/*
		* Selects an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::selectAtom(Index atom_tobeselected_index)
		{

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if (!index_to_atom_.has(atom_tobeselected_index))
			{
				Log.error() << "MolecularStructureContainer: cannot select a non-existing atom!" << std::endl;
				return;
			}

			bool atomSelected = false;

			AtomContainer* container = getAtomContainer();

			if (!container) return;
		
			if (container)
			{
				for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					Index atom_index_ =  -1;

					if (atom)
					{
						Handle atom_identifer = atom->getHandle();
						if (atom_to_index_.has(atom_identifer))
						{
							atom_index_ = atom_to_index_[atom_identifer];
						}
					}

					if ((atom_index_ != -1) && atom)
					{
						if (atom_index_ == atom_tobeselected_index )
						{
							getMainControl()->selectCompositeRecursive((Composite *)atom);
							CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::SELECTED_COMPOSITE, true);
							qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
							atomSelected = true;
							break;
						}
					}
					//ToDo
					//Take care keep tracking of atom selection
					//Indroduce another hashmap for keeping track of selected atoms
				}
			}
			
			if (!atomSelected)
			{
				Log.error() << "MolecularStructureContainer: cannot select a non-existing atom!" << std::endl;
				return;
			}
		}

		
		/*
		* Deselects an existing atom in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom index 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::deselectAtom(Index atom_tobedeselect_index)
		{

			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if (!index_to_atom_.has(atom_tobedeselect_index))
			{
				Log.error() << "MolecularStructureContainer: cannot deselect a non-existing atom!" << std::endl;
				return;
			}

			bool atomDeselected = false;
			AtomContainer* container = getAtomContainer();
			
			if (!container) 
				return;
			else
			{
				for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					Index atom_index_ = -1;

					if (atom)
					{
						Handle atom_identifier = atom->getHandle();

						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}
					}
					
					if ((atom_index_ != -1) && atom)
					{
						if (atom_index_ == atom_tobedeselect_index )
						{
							getMainControl()->deselectCompositeRecursive((Composite *)atom);
							CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::DESELECTED_COMPOSITE, true);
							qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
							atomDeselected = true;
						}
					}
					//ToDo
					//Take care keep tracking of atom deselection
					//Indroduce another hashmap for keeping track of deselected atoms
				}
			}
		
			if (!atomDeselected)
			{
				Log.error() << "MolecularStructureContainer: cannot deselect a non-existing atom!" << std::endl;
				return;
			}
		}


		
		/*
		* Selects a bond between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::selectBond(Index atom_index_one,Index atom_index_two)
		{
			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if ((!index_to_atom_.has(atom_index_one)) && (!index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot select the bond between non-existing  atoms!" << std::endl;
				return;
			}

			bool bondSelected = false;
			AtomContainer* container = getAtomContainer();
			
			if (!container) 
				return;
			else
			{
				Atom* atom_one;
				Atom* atom_two;

				for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					Index atom_index_ = -1;

					if (atom)
					{
						Handle atom_identifer = atom->getHandle();

						if (atom_to_index_.has(atom_identifer))
						{
							atom_index_ = atom_to_index_[atom_identifer];
						}
					}

					if (atom_index_ != -1)
					{
						if (atom_index_ == atom_index_one)
						{
							atom_one = &*at_it;
						}
						else if(atom_index_ == atom_index_two)
						{
							atom_two = &*at_it;
						}
					}
				}

				if (atom_one && atom_two)
				{
					//Need to check this :
					Bond* bond_selected = atom_one->getBond(*atom_two);

					if (bond_selected)
					{
						getMainControl()->selectCompositeRecursive((Composite *)bond_selected);
						CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::SELECTED_COMPOSITE, true);
						qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
					}
				}

				//bondSelected = true;
				
				//ToDo
				//Take care keep tracking of bond selection
				//Indroduce another hashmap

				//for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				//{
					//Note
					// This does not work as I wanted
				//	Index atom_index_one_ = at_it->getIndex();

				//	if (atom_index_one_ == atom_index_one)
				//	{
				//		// is the correct to have at_it->beginBond()
				//		for (Atom::BondIterator b_it = at_it->beginBond(); +b_it; ++b_it)
				//		{
				//			Bond* bond = &*b_it;
				//			if(bond_selected->getFirstAtom()->getIndex()== bond->getFirstAtom()->getIndex()
				//				&& bond_selected->getSecondAtom()->getIndex() == bond->getSecondAtom()->getIndex())
				//			{
				//				getMainControl()->selectCompositeRecursive((Composite *)bond);
				//				CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::SELECTED_COMPOSITE, true);
				//				qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
				//				bondSelected = true;
				//				break;
				//			}
				//		}
				//	}
				//}
				//	
				//if (!bondSelected)
				//{
				//	Log.error() << "MolecularStructureContainer: cannot select a non-existing bond!" << std::endl;
				//	return;
				//}
			}

		}

		
		/*
		* Deselects a bond between existing atoms in an existing ballview molecular structure(system)which is an atom container
		* Required parameter is atom indices 
		* Note : functioanlity not tested 
		*/
		void MolecularStructureContainer::deselectBond(Index atom_index_one,Index atom_index_two)
		{
			// Note
			// Not sure if this works
			// If  no datastructure is used, then these condition need be removed
			if ((!index_to_atom_.has(atom_index_one)) && (!index_to_atom_.has(atom_index_two)))
			{
				Log.error() << "MolecularStructureContainer : Cannot deselect the bond between non-existing  atoms!" << std::endl;
				return;
			}

			bool bondDeselected = false;
			AtomContainer* container = getAtomContainer();
			
			if (!container) 
				return;
			else
			{
				Atom* atom_one;
				Atom* atom_two;
				for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				{
					//Note
					// This does not work as I wanted
					//Index atom_index_ = at_it->getIndex();

					Atom* atom = &*at_it;
					Index atom_index_ = -1;

					if (atom)
					{
						Handle atom_identifier = atom->getHandle();
						if (atom_to_index_.has(atom_identifier))
						{
							atom_index_ = atom_to_index_[atom_identifier];
						}
					}

					if (atom_index_ == atom_index_one)
					{
						atom_one = &*at_it;
					}
					else if(atom_index_ == atom_index_two)
					{
						atom_two = &*at_it;
					}
				}

				if (atom_one && atom_two)
				{
					//Need to check this :
					Bond* bond_deselected = atom_one->getBond(*atom_two);
					if (bond_deselected)
					{
						//validate if  bond exist
						getMainControl()->deselectCompositeRecursive((Composite *)bond_deselected);
						CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::DESELECTED_COMPOSITE, true);
						qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
					}
				}

				//bondDeselected = true;

				//ToDo
				//Take care keep tracking of bond deselection
				//Indroduce another hashmap

				//I dont need to do this < i think > if i know exactly which bond is selected
				//by knowing the atoms  that are connected by this bond
				//thne I don't need lo search for bond as followed
				//for (AtomIterator at_it = container->beginAtom(); +at_it; ++at_it)
				//{
					//Note
					// This does not work as I wanted
				//	Index atom_index_one_ = at_it->getIndex();
				//	if (atom_index_one_ == atom_index_one)
				//	{
				//		// is the correct to have at_it->beginBond()
				//		for (Atom::BondIterator b_it = at_it->beginBond(); +b_it; ++b_it)
				//		{
				//			Bond* bond = &*b_it;
				//			if(bond_deselected->getFirstAtom()->getIndex()== bond->getFirstAtom()->getIndex()
				//				&& bond_deselected->getSecondAtom()->getIndex() == bond->getSecondAtom()->getIndex())
				//			{
				//				
				//				getMainControl()->deselectCompositeRecursive((Composite *)bond);
				//				CompositeMessage *cm = new CompositeMessage(*container, CompositeMessage::DESELECTED_COMPOSITE, true);
				//				
				//				qApp->postEvent(MainControl::getInstance(0), new MessageEvent(cm));
				//				bondDeselected = true;
				//				break;
				//			}
				//		}
				//	}
				//	
				//}
				/*if (!bondDeselected)
				{
					Log.error() << "MolecularStructureContainer: cannot select a non-existing bond!" << std::endl;
					return;
				}*/
			}
		}
	}
}
