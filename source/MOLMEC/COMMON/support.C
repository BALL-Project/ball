// $Id: support.C,v 1.22 2001/02/23 10:17:34 anker Exp $

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/COMMON/limits.h>

using namespace std;

namespace BALL 
{

	namespace MolmecSupport 
	{

		Size calculateNonBondedAtomPairs
			(vector< pair <Atom*, Atom*> >& pair_vector, 
			 const AtomVector& atom_vector,
			 const Box3& box, double distance,
			 bool periodic_boundary_enabled, 
			 PairListAlgorithmType type)
		{

			// determine lower and upper corner of the hash grid that contains
			// the box plus "distance" many units on each side
			Vector3 lower(Limits<float>::max());
			Vector3 upper(Limits<float>::min());

			// Iterators for the atom vector
			AtomVector::ConstIterator atom_it;
			AtomVector::ConstIterator atom_it2;

			// Position vectors
			Vector3 position;
			Vector3 new_position; 
			Vector3 difference;
			
			// the box width / length / depth
			double period_x;
			double period_y;
			double period_z;
			Vector3 period;

			// Are there atoms stored in atom_vector at all?
			if (atom_vector.size() == 0)
			{
				Log.warn() << "calculateNonBondedAtomPairs: atom_vector is empty " 
					<< endl;
				return 0;
			}

			// Test whether the periodic boundary is enabled or not and calculate
			// the box size accordingly
			if (periodic_boundary_enabled) 
			{
				// Just take the box that was given as argument...
				period_x = box.getWidth();
				period_y = box.getHeight();
				period_z = box.getDepth(); 
				period = Vector3(period_x, period_y, period_z);
			
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

			// Counter for the number of neighbored atom pairs.
			Size	counter = 0;

			// Squared distance
			double  squared_distance = distance * distance;

			// initialize the hash grid
			HashGrid3<Atom*> grid(lower - Vector3(0.1),
					upper - lower + Vector3(0.2), distance);

			// Iterators and hash box pointer for the grid search
			HashGridBox3<Atom*>* hbox;
			HashGridBox3<Atom*>::BoxIterator box_it;
			HashGridBox3<Atom*>::DataIterator data_it;
	
			if (periodic_boundary_enabled) 
			{
				// Calculate the half periods for determining the minimal image
				double half_period_x = period_x * 0.5;
				double half_period_y = period_y * 0.5;
				double half_period_z = period_z * 0.5;

				// Check what kind of algorithm should be used for determining the
				// neighbours

				if (type == BRUTE_FORCE) 
				{
					// Brute force algorithm:

					for (atom_it = atom_vector.begin(); atom_it != atom_vector.end();
							++atom_it) 
					{
						position = (*atom_it)->getPosition();
						atom_it2 = atom_it; ++atom_it2;
						for (atom_it2 = atom_it, atom_it2++; 
								atom_it2 != atom_vector.end(); ++atom_it2) 
						{
							new_position = position;
							difference = position - (*atom_it2)->getPosition();
							new_position = calculateMinimumImage(difference, period);

							/*
							if (difference.x < -half_period_x) 
							{
								new_position.x += period_x;
							}
							else 
							{
								if (difference.x > half_period_x)
								{
									new_position.x -= period_x;
								}
							}

							if (difference.y < -half_period_y) 
							{
								new_position.y += period_y;
							}
							else 
							{
								if (difference.y > half_period_y)
								{
									new_position.y -= period_y;
								}
							}

							if (difference.z < -half_period_z) 
							{
								new_position.z += period_z;
							}
							else 
							{
								if (difference.z > half_period_z)
								{
									new_position.z -= period_z;
								}
							}
							*/

							if ((new_position.getSquareDistance((*atom_it2)->getPosition())) 
									<= squared_distance) 
							{
								pair_vector.push_back(pair<Atom*, Atom*>(*atom_it, *atom_it2));
								counter++;
							}
						}
					}	
				} 
				else 
				{ 
					// Use a hash grid with box length "distance" to determine all
					// neigboured atom pairs

					for (atom_it = atom_vector.begin(); atom_it != atom_vector.end();
							++atom_it) 
					{
						position = (*atom_it)->getPosition();

						// Search all neighbour atoms of "atom_it" that are stored in
						// the hash grid

						// Calculate the 27 images of the atom and determine their
						// neighbours

						for (short x = -1; x < 2 ; x++) 
						{
							new_position.x = position.x + x * period_x;
							for (short y = -1; y < 2 ; y++) 
							{
								new_position.y = position.y + y * period_y;
								for (short z = -1; z < 2 ; z++) 
								{
									new_position.z = position.z + z * period_z;
									hbox = grid.getBox(new_position);

									if (hbox != 0)
									{
										for (box_it = hbox->beginBox(); +box_it; ++box_it) 
										{
											for (data_it = box_it->beginData(); +data_it; ++data_it) 
											{
												difference = new_position - (*data_it)->getPosition();
												if ((difference.x > -half_period_x) 
													&& (difference.x < half_period_x) 
													&& (difference.y > -half_period_y) 
													&& (difference.y < half_period_y)
													&& (difference.z > -half_period_z) 
													&& (difference.z < half_period_z) 
													&& (difference.getSquareLength() <= squared_distance))
												{
													pair_vector.push_back(pair<Atom*, Atom*>(*atom_it, *data_it));
													counter++;
												}
											}
										}
									}
								}
							}
						}
						// insert the new atom into the hash grid
						grid.insert(position, (*atom_it));
					}
				}
			} 
			// periodic boundary not enabled
			else 
			{
				// Check what kind of algorithm should be used for calculating the
				// neighbours
				if (type == BRUTE_FORCE) 
				{
					// Brute force algorithm
					for (Size atom_index_a = 0; atom_index_a < (atom_vector.size() - 1); 
							++atom_index_a) 
					{
						position = atom_vector[atom_index_a]->getPosition();
						for (Size atom_index_b = atom_index_a + 1; 
								atom_index_b < atom_vector.size(); ++atom_index_b) 
						{
							if (((position.getSquareDistance(atom_vector[atom_index_b]->getPosition())) <= squared_distance) 
									&& !atom_vector[atom_index_a]->isBoundTo(*atom_vector[atom_index_b])
									&& !atom_vector[atom_index_a]->isGeminal(*atom_vector[atom_index_b]))
							{
								pair_vector.push_back(pair<Atom*,Atom*>(atom_vector[atom_index_a], atom_vector[atom_index_b]));
								counter++;
							}
						}
					}	

				} 
				else 
				{  	
					// Use a hash grid with box length "distance" to determine all
					// neigboured atom pairs

					for (atom_it = atom_vector.begin(); atom_it != atom_vector.end();
							++atom_it) 
					{
						position = (*atom_it)->getPosition();

						// Search all neighbor atoms of "atom_it" that are stored in
						// the hash grid

						hbox = grid.getBox(position);
						if (hbox != 0)
						{
							for (box_it = hbox->beginBox(); +box_it; ++box_it) 
							{
								for (data_it = (*box_it).beginData(); +data_it; ++data_it) 
								{
									if (((position.getSquareDistance((*data_it)->getPosition())) <= squared_distance) 
											&& !(*data_it)->isBoundTo(**atom_it)
											&& !(*data_it)->isGeminal(**atom_it))
									{
										pair_vector.push_back(pair<Atom*,Atom*>((*atom_it),(*data_it)));
										counter++;
									}
								}
							}
						} 
						else 
						{
							Log.warn() << "calculateNonBondedAtomPairs: hbox = 0 for position " << position
													<< " (grid dimensions: " << grid.getOrigin() << ")" << endl;
						}

						// Insert the new atom into the hash grid
						grid.insert(position, (*atom_it));
					}
				}
			}

			// DEBUG
			Log.info() << "calculateNonBondedAtomPairs: " << counter << endl;

			return counter;
		}


		// Add solvent molecules from "solvent" to "system" if they lie 
		// in the box "box" and if they do not overlap with molecules in 
		// "solute_grid"
		Size addNonOverlappingMolecules
			(System& system, const HashGrid3<Atom*>& solute_grid, 
			 const System& solvent, const Box3& box, double distance)
		{
			Molecule* old_molecule = 0;
			Molecule* new_molecule = 0;
			bool add = true;
			Size atom_counter = 0;
			Size mol_counter = 0;
			double square_distance = distance * distance;
			double mass = 0;
			Vector3	center_of_gravity(0.0);

			// Iterate over all atoms in solvent and test the different molecules
			// as follows: 
			// Calculate the number of atoms of the molecule and its mass. If the
			// number of molecules is larger 0 and the mass is larger 0, then a
			// another test has to be carried out whether the center of gravity
			// is in the box. If so, the molecule will be inserted in solute:

			// This is not the most obviuos way to iterate over different
			// molecules. We iterate over all solvent _atoms_ and distinguish
			// _molecules_ by their dedicated molecule (by examining getMolecule())
	 
			// DEBUG
			Size reject_counter = 0;

			AtomIterator atom_it = solvent.beginAtom();
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
						if (add == true) 
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
								// DEBUG
								else
								{
									reject_counter++;
								}
							}
						}
						// DEBUG
						else
						{
							reject_counter++;
						}

						old_molecule = new_molecule;
						add = true;
						center_of_gravity.x = 0;
						center_of_gravity.y = 0;
						center_of_gravity.z = 0;
						atom_counter = 0;
						mass = 0;
					}


					Vector3 position = atom_it->getPosition();

					float atomic_mass = atom_it->getElement().getAtomicWeight();
					center_of_gravity += atomic_mass * position;
					mass += atomic_mass; 
					atom_counter++;
					
					// check for all collisions with any of the solute's atoms
					const HashGridBox3<Atom*>* hbox = solute_grid.getBox(position);
					if ((hbox != 0) && add)
					{	
						HashGridBox3<Atom*>::ConstDataIterator data_it;
						HashGridBox3<Atom*>::ConstBoxIterator box_it;
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
				if (add == true) 
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
						// DEBUG
						else
						{
							reject_counter++;
						}
					}
				}
				// DEBUG
				else
				{
					reject_counter++;
				}
			}

			// DEBUG
			/*
			Log.info() << "Number of rejected molecules = " << reject_counter 
				<< endl;
			Log.info() << "Number of inserted molecules = " << mol_counter 
				<< endl;
			*/
			// return the number of processed solvent molecules
			return mol_counter;
		}


		// This little function adapts a waterbox from external programs that
		// use a different definition of the periodic box.
		void adaptWaterBox(System& system, const Box3& box)
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
			Log.info() << "processed " << mol_count << " solvent molecules" << endl;
		}


		Vector3& calculateMinimumImage(const Vector3& distance,
				const Vector3& period)
		{
			Vector3 half_period(period * 0.5);
			Vector3 result;

			if (distance.x <= -half_period.x)
			{
				result.x += period.x;
			}
			else 
			{
				if (distance.x > half_period.x)
				{
					result.x -= period.x;
				}
			}

			if (distance.y <= -half_period.y)
			{
				result.y += period.y;
			}
			else 
			{
				if (distance.y > half_period.y)
				{
					result.y -= period.y;
				}
			}

			if (distance.z <= -half_period.z)
			{
				result.z += period.z;
			}
			else 
			{
				if (distance.z > half_period.z)
				{
					result.z -= period.z;
				}
			}
			return result;
		}

	}	// namespace MolmecSupport

} // namespace BALL
