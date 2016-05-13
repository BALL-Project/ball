// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/MATHS/common.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/SYSTEM/sysinfo.h>

#include <cmath>
#include <limits>

//   #define BALL_BENCHMARK

#ifdef BALL_BENCHMARK
 #include <BALL/SYSTEM/timer.h>
#endif 


using namespace std;

namespace BALL 
{
	namespace MolmecSupport 
	{

		// Calculate a vector of non-bonded atom pairs whose distance is
		// smaller than the value of the distance variable
		Size calculateNonBondedAtomPairs
			(vector< pair <Atom*, Atom*> >& pair_vector, 
			 const AtomVector& atom_vector,
			 const SimpleBox3& box, 
			 double distance,
			 bool periodic_boundary_enabled, 
			 PairListAlgorithmType type)
			throw(Exception::OutOfMemory)
		{
#ifdef BALL_BENCHMARK
	Timer t;
	t.start();
#endif

			// determine lower and upper corner of the hash grid that contains
			// the box plus "distance" many units on each side
			Vector3 lower(std::numeric_limits<float>::max());
			Vector3 upper(std::numeric_limits<float>::min());

			// Iterators for the atom vector
			AtomVector::ConstIterator atom_it;

			// Position vectors
			Vector3 position;
			Vector3 new_position; 
			Vector3 difference;

			// Are there atoms stored in atom_vector at all?
			if (atom_vector.size() == 0)
			{
				Log.warn() << "calculateNonBondedAtomPairs: atom_vector is empty " << endl;
				return 0;
			}

			// the box length along each axis
			double period_x = 0.0;
			double period_y = 0.0;
			double period_z = 0.0;
			Vector3 period;

			// Test whether the periodic boundary is enabled or not and calculate
			// the box size accordingly
			if (periodic_boundary_enabled) 
			{
				// the box width / length / depth
				period = Vector3(box.getWidth(), box.getHeight(), box.getDepth());
				period_x = period.x;
				period_y = period.y;
				period_z = period.z;

				// ... and add at least distance to each coordinate to gain a box
				// that contains enough neighbouring boxes.
				if (distance < period_x) 
				{
					lower.x = box.a.x - distance;
					upper.x = box.b.x + distance;
				} 
				else 
				{
					lower.x = box.a.x - period_x;
					upper.x = box.b.x + period_x;
				}	

				if (distance < period_y) 
				{
					lower.y = box.a.y - distance;
					upper.y = box.b.y + distance;
				} 
				else 
				{
					lower.y = box.a.y - period_y;
					upper.y = box.b.y + period_y;
				}	

				if (distance < period_z) 
				{
					lower.z = box.a.z - distance;
					upper.z = box.b.z + distance;
				} 
				else 
				{
					lower.z = box.a.z - period_z;
					upper.z = box.b.z + period_z;
				}	
			} 
			else  
			{
				// we know nothing about the box, so we have to calculate it on our
				// own by looking at the position of every atom in atom_vector
				for (atom_it = atom_vector.begin(); atom_it != atom_vector.end();
						++atom_it)	
				{
					position = (*atom_it)->getPosition();
					if (position.x < lower.x) lower.x = position.x;
					if (position.y < lower.y) lower.y = position.y;
					if (position.z < lower.z) lower.z = position.z;
					if (position.x > upper.x) upper.x = position.x;
					if (position.y > upper.y) upper.y = position.y;
					if (position.z > upper.z) upper.z = position.z;
				}

				lower.x -= distance;
				lower.y -= distance;
				lower.z -= distance;
				upper.x += distance;
				upper.y += distance;
				upper.z += distance;
			}

			// now we have the box, let's look which pairs we shall create

			// Remember the initial number of atom pairs in the pair vector.
			Size number_of_pairs = pair_vector.size();

			// Squared distance
			double squared_distance = distance * distance;

			if (periodic_boundary_enabled) 
			{
				// We always use the brute-force algorithm if PBC are enabled.
				// Brute force algorithm: for every atom, calculate the 
				// image of every other atom and check whether this atom
				// is within the cutoff radius.
				Vector3 position_i;
				double inverse_period_x = 1.0 / period_x;
				double inverse_period_y = 1.0 / period_y;
				double inverse_period_z = 1.0 / period_z;
				for (Position i = 0; i < atom_vector.size() - 1; i++)
				{
					position_i = atom_vector[i]->getPosition();
					for (Position j = i + 1; j < atom_vector.size(); j++) 
					{
						difference = position_i - atom_vector[j]->getPosition();
						difference.x = difference.x - period_x * Maths::rint(difference.x * inverse_period_x);
						difference.y = difference.y - period_y * Maths::rint(difference.y * inverse_period_y);
						difference.z = difference.z - period_z * Maths::rint(difference.z * inverse_period_z);

						// Remove 1-2 and 1-3 pairs!
						if ((difference.getSquareLength() < squared_distance) 
								&& !atom_vector[i]->isBoundTo(*atom_vector[j])
								&& !atom_vector[i]->isGeminal(*atom_vector[j]))
						{
							pair_vector.push_back(pair<Atom*, Atom*>(atom_vector[i], atom_vector[j]));
						}
					}
				}	
			} 
			// periodic boundary not enabled
			else 
			{
				// Check what kind of algorithm should be used for calculating the
				// neighbours
				if (type != BRUTE_FORCE)
				{
					float memory = SysInfo::getAvailableMemory();
					if (memory != -1)
					{
						memory *= 0.7;
						float min_spacing = HashGrid3<const Atom*>::calculateMinSpacing((LongSize)memory, upper - lower + Vector3(0.2F));
						if (min_spacing > distance) type = BRUTE_FORCE;
					}
				}

       	if (type == BRUTE_FORCE)
				{
					// Brute force algorithm
					for (Position i = 0; i < (atom_vector.size() - 1); ++i) 
					{
						position = atom_vector[i]->getPosition();
						for (Position j = i + 1; j < atom_vector.size(); j++) 
						{
							// Remove 1-2 and 1-3 pairs!
							if (((position.getSquareDistance(atom_vector[j]->getPosition())) < squared_distance) 
									&& !atom_vector[i]->isBoundTo(*atom_vector[j])
									&& !atom_vector[i]->isGeminal(*atom_vector[j]))
							{
								pair_vector.push_back(pair<Atom*,Atom*>(atom_vector[i], atom_vector[j]));
							}
						}
					}	
				} 
				else 
				{  	
					// Algorithm using a 3D hash grid
					//
					// Use a hash grid with box length "distance" to determine all
					// neighboring atom pairs
					
					// we enlarge the box by some constant to be sure not to run into
					// numerical problems
 					HashGrid3<Atom*> grid(lower - Vector3(0.1F), upper - lower + Vector3(0.2F), 
																distance + 0.1F);

					for (atom_it = atom_vector.begin(); atom_it != atom_vector.end(); ++atom_it) 
					{
						grid.insert((*atom_it)->getPosition(), *atom_it); // insert atom into grid
					}

					// iterate over all boxes
					HashGrid3<Atom*>::BoxIterator box_it = grid.beginBox();
					for (; box_it != grid.endBox(); ++box_it) 
					{
						Position x, y, z;
						grid.getIndices(*box_it, x, y, z);

						// look in the same box for near atoms
						HashGridBox3<Atom*>::ConstDataIterator bit = box_it->beginData(); 
						for (; +bit; bit++)
						{
							const Vector3& bit_pos = (*bit)->getPosition();
							// iterate over all atoms after bit
 							HashGridBox3<Atom*>::ConstDataIterator tit = bit; 
 							tit++;
							for (; +tit; tit++)
							{
								// Remove 1-2 and 1-3 pairs!
								if (bit_pos.getSquareDistance((*tit)->getPosition()) < squared_distance 
										&& !(*tit)->isBoundTo(**bit)
										&& !(*tit)->isGeminal(**bit))
								{
									pair_vector.push_back(pair<Atom*,Atom*>(*bit, *tit));
								}
							}
						}

						// iterator over neighbour boxes
						for (Index xi = -1; xi <= 1; xi++)
						{
							const Position nx = x + xi;
							for (Index yi = -1; yi <= 1; yi++)
							{
								const Position ny = y + yi;
								for (Index zi = -1; zi <= 1; zi++)
								{
 									if ((xi == 0) && (yi == 0) && (zi == 0)) continue;

									HashGridBox3<Atom*>* bbox = grid.getBox(nx, ny, z+zi);
									// smaller operator also checks for 0 !
									if (bbox < &*box_it || bbox->isEmpty()) 
									{
										continue;
									}

									// iterate over all atoms of current box
									HashGridBox3<Atom*>::ConstDataIterator tit = box_it->beginData();
									for (;+tit; tit++)
									{
										const Vector3& atom_pos = (*tit)->getPosition();
										// iterate over all atoms of neighbour boxes
										HashGridBox3<Atom*>::ConstDataIterator bit = bbox->beginData(); 
										for (; +bit; bit++)
										{
											// Remove 1-2 and 1-3 pairs!
											if (((*bit)->getPosition().getSquareDistance(atom_pos) < squared_distance) 
													&& !(*tit)->isBoundTo(**bit)
													&& !(*tit)->isGeminal(**bit))
											{
												pair_vector.push_back(pair<Atom*,Atom*>(*bit, *tit));
											}
										}
									}
									
								} // zi
							} // yi
						}  // xi
					} // for all boxes
				}
			}
#ifdef BALL_BENCHMARK
t.stop();
Log.error() << "calculateNonBondedAtomPairs time: " << String(t.getClockTime()) << std::endl;
#endif
			// Return the number of pairs *added* to the vector.
			return (pair_vector.size() - number_of_pairs);
		}


		Size sortNonBondedAtomPairsAfterSelection
			(vector< pair <Atom*, Atom*> >& pair_vector)
		{
			Size number_of_pairs_with_selection = 0;

			if (!pair_vector.empty())
			{
				// sort the list such that those pairs where one of the atoms
				// is selected appear at the beginning of the list and the 
				// remaining atoms at the end
				ForceField::PairVector::iterator current_it = pair_vector.begin();
				ForceField::PairVector::iterator last_it = pair_vector.end();
				last_it--;
				while (!(current_it == last_it))
				{
					if (current_it->first->isSelected() || current_it->second->isSelected())
					{
						// increment the current_it iterator and the pair counter
						current_it++;
						number_of_pairs_with_selection++;
					}
					else
					{
						// this one was not selected, put it at the end of the list,
						// increment the last_it iterator
						swap(*current_it, *last_it);
						last_it--;
					}
				}
				if (last_it->first->isSelected() || last_it->second->isSelected())
				{
					number_of_pairs_with_selection++;
				}
			}

			// return the number of pairs where any atom is selected
			// these pairs appear at the beginning of the list

			return number_of_pairs_with_selection;
		}


		// Add solvent molecules from "solvent" to "system" if they lie 
		// in the box "box" and if they do not overlap with molecules in 
		// "solute_grid"
		Size addNonOverlappingMolecules
			(System& system, const HashGrid3<const Atom*>& solute_grid, 
			 const System& solvent, const SimpleBox3& box, double distance)
		{
			const Molecule* old_molecule = 0;
			const Molecule* new_molecule = 0;
			bool add = true;
			bool keep = true;
			Size atom_counter = 0;
			Size mol_counter = 0;
			double square_distance = distance * distance;
			double mass = 0;
			Vector3	center_of_gravity(0.0);
			Vector3 period(box.getWidth(), box.getHeight(), box.getDepth());

			// Iterate over all atoms in solvent and test the different molecules
			// as follows: 
			// Calculate the number of atoms of the molecule and its mass. If the
			// number of molecules is larger 0 and the mass is larger 0, then a
			// another test has to be carried out whether the center of gravity
			// is in the box. If so, the molecule will be inserted in solute:

			// This is not the most obviuos way to iterate over different
			// molecules. We iterate over all solvent _atoms_ and distinguish
			// _molecules_ by their dedicated molecule (by examining getMolecule())

			// variables needed within the loop
			Vector3 position;
			Vector3 new_position;
			float atomic_mass;
			HashGridBox3<const Atom*>::ConstDataIterator data_it;
			HashGridBox3<const Atom*>::ConstBoxIterator box_it;
			Vector3	additional_space(distance * 1.02);
			HashGrid3<const Atom*> solvent_grid(box.a - additional_space,
					box.b - box.a + additional_space + additional_space, distance);

			// determine the maximum indices of gridboxes
			Position max_x, max_y, max_z;
			max_x = solute_grid.getSizeX();
			max_y = solute_grid.getSizeY();
			max_z = solute_grid.getSizeZ();
			Position index_x, index_y, index_z;

			AtomConstIterator atom_it = solvent.beginAtom();
			if (+atom_it)
			{
				old_molecule = atom_it->getMolecule();
				for (; +atom_it; ++atom_it) 
				{
					// Test if a new molecule is reached and if the old can be inserted
					// into the solute system
					new_molecule = atom_it->getMolecule();
					if (new_molecule != old_molecule) 
					{
						if (add)
						{
							if ((atom_counter > 0) && (mass != 0)) 
							{
								center_of_gravity /= mass;
								if ((center_of_gravity.x >= box.a.x) 
										&& (center_of_gravity.x <= box.b.x)
										&& (center_of_gravity.y >= box.a.y) 
										&& (center_of_gravity.y <= box.b.y) 
										&& (center_of_gravity.z >= box.a.z) 
										&& (center_of_gravity.z <= box.b.z)) 
								{
									// copy the solvent molecule and insert it into
									// the solute system 
									Molecule* solvent_molecule = new Molecule(*old_molecule);
									system.insert(*solvent_molecule);
									solvent_grid.insert(atom_it->getPosition(), &*atom_it);
									mol_counter++;
								}
							}
						}

						old_molecule = new_molecule;
						add = true;
						center_of_gravity.x = 0;
						center_of_gravity.y = 0;
						center_of_gravity.z = 0;
						atom_counter = 0;
						mass = 0;
					}


					position = atom_it->getPosition();

					atomic_mass = atom_it->getElement().getAtomicWeight();
					center_of_gravity += atomic_mass * position;
					mass += atomic_mass; 
					atom_counter++;

					// check for all collisions with any of the solute's atoms
					const HashGridBox3<const Atom*>* hbox = solute_grid.getBox(position);
					if ((hbox != 0) && add)
					{	
						for (box_it = hbox->beginBox(); +box_it && add; ++box_it)
						{
							for (data_it = box_it->beginData(); +data_it && add; ++data_it)
							{
								if ((position.getSquareDistance((*data_it)->getPosition())) 
										< square_distance)
								{
									add = false;
								}
							}
						}

					}
				}	

				if (add)
				{
					if ((atom_counter > 0) && (mass != 0)) 
					{
						center_of_gravity /= mass;
						if ((center_of_gravity.x >= box.a.x) 
								&& (center_of_gravity.x <= box.b.x)
								&& (center_of_gravity.y >= box.a.y) 
								&& (center_of_gravity.y <= box.b.y) 
								&& (center_of_gravity.z >= box.a.z) 
								&& (center_of_gravity.z <= box.b.z)) 
						{
							// copy the solvent molecule and insert it into
							// the solute system 
							Molecule* solvent_molecule = new Molecule(*old_molecule);
							system.insert(*solvent_molecule);
							mol_counter++;
						}
					}
				}
			}

			// now we have to check for collisions at the periodic boundary

#ifndef CHECK
			MoleculeIterator mol_it = system.beginMolecule();
			HashSet<Molecule*> delete_set;

			for (; +mol_it; ++mol_it)
			{
				keep = true;
				for (atom_it = mol_it->beginAtom(); +atom_it; ++atom_it)
				{
					// we have to check for boundary conditions so test whether we
					// are at a boundary

					position = atom_it->getPosition();
					const HashGridBox3<const Atom*>* hbox = solvent_grid.getBox(position);
					solvent_grid.getIndices(*hbox, index_x, index_y, index_z);

					// indices below 1 and above max - 1 are at the border, because
					// the grid has an additonal layer of boxes wrapped around for
					// making the algorithm more robust in numerical terms

					// if we are at the border, translate the atom virtually by
					// period (or -period, respectively) and then calculate the
					// distance to every atom in all neighbouring boxes.

					new_position = position;
					if (index_x <= 1)
					{
						new_position.x += period.x;
					}
					if (index_x >= max_x - 1)
					{
						new_position.x -= period.x;
					}

					if (new_position != position)
					{
						hbox = solvent_grid.getBox(new_position);
						if ((hbox != 0))
						{	
							for (box_it = hbox->beginBox(); +box_it && keep; ++box_it)
							{
								for (data_it = box_it->beginData(); +data_it && keep; ++data_it)
								{
									if ((new_position.getSquareDistance((*data_it)->getPosition())) 
											< square_distance)
									{
										keep = false;
									}
								}
							}
						}
					}

					new_position = position;
					if (index_y <= 1)
					{
						new_position.y += period.y;
					}
					if (index_y >= max_y - 1)
					{
						new_position.y -= period.y;
					}

					if (new_position != position)
					{
						hbox = solvent_grid.getBox(new_position);
						if ((hbox != 0))
						{	
							for (box_it = hbox->beginBox(); +box_it && keep; ++box_it)
							{
								for (data_it = box_it->beginData(); +data_it && keep; ++data_it)
								{
									if ((new_position.getSquareDistance((*data_it)->getPosition())) 
											< square_distance)
									{
										keep = false;
									}
								}
							}
						}
					}

					new_position = position;
					if (index_z <= 1)
					{
						new_position.z += period.z;
					}
					if (index_z >= max_z - 1)
					{
						new_position.z -= period.z;
					}

					if (new_position != position)
					{
						hbox = solvent_grid.getBox(new_position);
						if ((hbox != 0))
						{	
							for (box_it = hbox->beginBox(); +box_it && keep; ++box_it)
							{
								for (data_it = box_it->beginData(); +data_it && keep; ++data_it)
								{
									if ((new_position.getSquareDistance((*data_it)->getPosition())) 
											< square_distance)
									{
										keep = false;
									}
								}
							}
						}
					}

				}
				if (!keep)
				{
					delete_set.insert(&*mol_it);
				}
			}

			// update the number of atoms we return to tha caller
			mol_counter -= delete_set.size();

			// delete all molecules we gathered
			HashSet<Molecule*>::Iterator del_it = delete_set.begin();
			for(; del_it.operator + (); ++del_it)
			{
				delete *del_it;
			}
#endif

			// return the number of processed solvent molecules
			return mol_counter;
		}


		// This little function adapts a waterbox from external programs that
		// use a different definition of the periodic box.
		void adaptWaterBox(System& system, const SimpleBox3& box)
		{

			float width = box.getWidth();
			float height = box.getHeight();
			float depth = box.getDepth();

			Vector3	center_of_gravity(0.0);
			float atomic_mass;
			Size atom_counter;
			float total_mass;
			Vector3 translation;

			// iterate over every molecule of the system and translate every
			// molecule with center of gravity outside to the opposite wall of
			// the box.
			Size mol_count = 0;
			MoleculeIterator mol_it = system.beginMolecule();
			AtomIterator atom_it;
			for (; +mol_it; ++mol_it)
			{
				atom_counter = 0;
				center_of_gravity.clear();
				total_mass = 0.0;

				// iterate over all atoms of this molecule to calculate the center
				// of gravity of this molecule
				atom_it = mol_it->beginAtom();
				for (; +atom_it; ++atom_it)
				{
					atom_counter++;
					atomic_mass = atom_it->getElement().getAtomicWeight();
					center_of_gravity += atomic_mass * atom_it->getPosition();
					total_mass += atomic_mass;
				}

				center_of_gravity /= total_mass;

				bool translate = false;

				if (center_of_gravity.x < box.a.x) 
				{
					translation.x += width;
					translate = true;
				} 
				else if (center_of_gravity.x > box.b.x) 
				{
					translation.x -= width;
					translate = true;
				}

				if (center_of_gravity.y < box.a.y) 
				{
					translation.y += height;
					translate = true;
				} 
				else if (center_of_gravity.y > box.b.y) 
				{
					translation.y -= height;
					translate = true;
				}

				if (center_of_gravity.z < box.a.z) 
				{
					translation.z += depth;
					translate = true;
				} 
				else if (center_of_gravity.z > box.b.z) 
				{
					translation.z -= depth;
					translate = true;
				}

				// Translate the atoms of the molecule if it has to be translated
				if (translate) 
				{
					for (atom_it = mol_it->beginAtom(); +atom_it; ++atom_it)
					{
						atom_it->setPosition(atom_it->getPosition() + translation);
					}
				}
				mol_count++;
			} // iteration over molecules
		}


		void calculateMinimumImage
			(Vector3& distance,	const Vector3& period)
		{
			distance.x = distance.x - period.x * Maths::rint(distance.x / period.x);
			distance.y = distance.y - period.y * Maths::rint(distance.y / period.y);
			distance.z = distance.z - period.z * Maths::rint(distance.z / period.z);
		}

	}	// namespace MolmecSupport

} // namespace BALL
