// $Id: support.C,v 1.3 1999/09/08 12:08:07 len Exp $

#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/DATATYPE/hashGrid.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/COMMON/limits.h>

namespace BALL {

	namespace MolmecSupport {

		Size calculateNonBondedAtomPairs
			(vector< pair <Atom*, Atom*> >& pair_vector, 
			 const vector<Atom*>& atom_vector,
			 const Box3& box, float distance,
			 bool periodic_boundary_enabled, int type)
		{

			// determine lower and upper corner of the hash grid that contains the box 
			// plus "distance" many units on each side
			Vector3 lower(Limits<float>::max(), Limits<float>::max(), Limits<float>::max());
			Vector3 upper(Limits<float>::min(), Limits<float>::min(), Limits<float>::min());

			// Iterators for the STL atom vector
			vector<Atom*>::const_iterator atom_it;
			vector<Atom*>::const_iterator atom_it2;

			// Position vectors
			Vector3 position;
			Vector3 new_position; 
			Vector3 difference;
			
			// the box width / length / depth
			float period_x;
			float period_y;
			float period_z;

			// Are atoms stored in atom_?

			if (atom_vector.size() == 0)
			{
				Log.warn() << " calculateNonBondedAtomPairs: atom_vector is empty " << endl;
				return 0;
			}

			// Test if periodic boundary is enabled or not

			if (periodic_boundary_enabled) 
			{
				// calculate the size of the box
				period_x = box.getWidth();
				period_y = box.getHeight();
				period_z = box.getDepth(); 
			
				if (distance < period_x) 
				{
					lower.x = box.a.x - distance;
					upper.x = box.b.x + distance;
				} else {
					lower.x = box.a.x - period_x;
					upper.x = box.b.x + period_x;
				}	

				if (distance < period_y) 
				{
					lower.y = box.a.y - distance;
					upper.y = box.b.y + distance;
				} else {
					lower.y = box.a.y - period_y;
					upper.y = box.b.y + period_y;
				}	

				if (distance < period_z) 
				{
					lower.z = box.a.z - distance;
					upper.z = box.b.z + distance;
				} else {
					lower.z = box.a.z - period_z;
					upper.z = box.b.z + period_z;
				}	
			} 
			else  
			{
				for (atom_it = atom_vector.begin(); atom_it != atom_vector.end(); ++atom_it)	
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

			// Counter for the number of neighbored atom pairs.
			Size	counter = 0;

			// Squared distance
			float  squared_distance = distance * distance;

			// initialize the hash grid
			HashGrid3<Atom*>	grid(lower, upper - lower, distance);
			HashGridBox3<Atom*>* hbox;

			// Iterators and hash box pointer for the grid search
			HashGridBox3<Atom*>::BoxIterator box_it;
			HashGridBox3<Atom*>::DataIterator data_it;
	
			// Test if periodic boundary is enabled or not

			if (periodic_boundary_enabled) 
			{
				// Calculate the half periods for determining the minimal image
				float half_period_x = period_x * 0.5;
				float half_period_y = period_y * 0.5;
				float half_period_z = period_z * 0.5;

				// Check what kind of algorithm should be used for determining the neighbours

				if (type == 0) 
				{
					// Brute force algorithm

					for ( atom_it = atom_vector.begin(); atom_it != atom_vector.end() ; ++atom_it) 
					{
						position = (*atom_it)->getPosition();
						for (atom_it2 = atom_it, atom_it2++; atom_it2 != atom_vector.end(); ++atom_it2) 
						{
							new_position = position;
							difference = position - (*atom_it2)->getPosition();

							if (difference.x < -half_period_x) 
								new_position.x += period_x;
							else if (difference.x > half_period_x)
								new_position.x -= period_x;

							if (difference.y < -half_period_y) 
								new_position.y += period_y;
							else if (difference.y > half_period_y)
								new_position.y -= period_y;

							if (difference.z < -half_period_z) 
								new_position.z += period_z;
							else if (difference.z > half_period_z)
								new_position.z -= period_z;

							if ((new_position.getSquareDistance((*atom_it2)->getPosition())) <= squared_distance) 
							{
								pair_vector.push_back(pair<Atom*, Atom*>(*atom_it, *atom_it2));
								counter++;
							}
						}
					}	
				} else { 
					// Use a hash grid with box length "distance" to determine all neigboured atom pairs

					for (atom_it = atom_vector.begin() ; atom_it != atom_vector.end() ; ++atom_it) 
					{
						position = (*atom_it)->getPosition();

						// Search all neighbor atoms of "atom_it" that are stored in the hash grid

						// Calculate the 27 images of the atom and determine their neighbours

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

									for (box_it = hbox->beginBox(); +box_it; ++box_it) 
									{
										for (data_it = (*box_it).beginData(); +data_it; ++data_it) 
										{

											difference = new_position - (*data_it)->getPosition();
											if (difference.x > -half_period_x && difference.x <= half_period_x &&
													difference.y > -half_period_y && difference.y <= half_period_y &&
													difference.z > -half_period_z && difference.z <= half_period_z &&
													difference.getSquareLength() <= squared_distance) 
											{
												pair_vector.push_back(pair<Atom*, Atom*>(*atom_it, *data_it));
												counter++;
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
			} else {

				// Check what kind of algorithm should be used for calculating the neighbours
				if (type == 0) 
				{
					// Brute force algorithm
					for (Size atom_index_a = 0 ; atom_index_a < (atom_vector.size() - 1) ; ++atom_index_a) 
					{
						position = atom_vector[atom_index_a]->getPosition();
						for (Size atom_index_b = atom_index_a + 1; atom_index_b < atom_vector.size(); ++atom_index_b) 
						{
							if (((position.getSquareDistance(atom_vector[atom_index_b]->getPosition())) <= squared_distance) 
									&& !atom_vector[atom_index_a]->isBondedTo(*atom_vector[atom_index_b])
									&& !atom_vector[atom_index_a]->isGeminal(*atom_vector[atom_index_b]))
							{
								pair_vector.push_back(pair<Atom*,Atom*>(atom_vector[atom_index_a], atom_vector[atom_index_b]));
								counter++;
							}
						}
					}	

				} else {  
					
					// Use a hash grid with box length "distance" to determine all neigboured atom pairs

					for (atom_it = atom_vector.begin(); atom_it != atom_vector.end() ; ++atom_it) 
					{
						position = (*atom_it)->getPosition();

						// Search all neighbor atoms of "atom_it" that are stored in the hash grid
						hbox = grid.getBox(position);
						if (hbox != 0)
						{
							for (box_it = hbox->beginBox(); +box_it; ++box_it) 
							{
								for (data_it = (*box_it).beginData(); +data_it; ++data_it) 
								{
									if (((position.getSquareDistance((*data_it)->getPosition())) <= squared_distance) 
											&& !(*data_it)->isBondedTo(**atom_it)
											&& !(*data_it)->isGeminal(**atom_it))
									{
										pair_vector.push_back(pair<Atom*,Atom*>((*atom_it),(*data_it)));
										counter++;
									}
								}
							}
						} else {
							Log.warn() << "calculateNonBondedAtomPairs: hbox = 0 for position " << position
													<< " (grid dimensions: " << grid.getOrigin() << ")" << endl;
						}

						// Insert the new atom into the hash grid
						grid.insert(position, (*atom_it));
					}
				}
			}

			return counter;
		}

		// Add solvent molecules to systemA if they lie in the box "box" and if
		// they do not overlap with molecules in systemA. The solvent molecules
		// are added to systemA.

		Size addNonOverlappingMolecules
			(System&  system_A, const System& system_B, 
			 const Box3& box, float distance)
		{
			Vector3	vector(distance);

			// Initialize HashGrid for storing atoms of system_A
			HashGrid3<Atom*> grid(box.a - vector, box.b + vector, distance);

			::BALL::AtomIterator atom_it;

			// Insert the atoms into the hash grid
			for (atom_it = system_A.beginAtom(); +atom_it; ++atom_it) 
			{
				grid.insert((*atom_it).getPosition(),&(*atom_it));
			}

			Molecule* old_molecule = 0;
			Molecule* new_molecule = 0;
			bool	add = false;
			Size atom_counter = 0;
			Size mol_counter = 0;
			float square_distance = distance * distance;
			float mass = 0;
			Vector3	center_of_gravity(0.0);

			// Iterate over all atoms in system_B and test the different molecules as follows:
			// Calculate the number of atoms of the molecule and its mass. If the number of molecules
			// is larger 0 and the mass is larger 0, then a another test has to be carried out whether
			// the center of gravity is in the box. If so, the molecule will be inserted in system_A:
	 
			for (atom_it = system_B.beginAtom(); +atom_it; ++atom_it) 
			{
				// Test if a new molecule is reached and if the old can be inserted into sytemA

				new_molecule = (*atom_it).getMolecule();
				if (new_molecule != old_molecule) {
					if (add == true) {
						if (atom_counter > 0 && mass != 0) {
							center_of_gravity /= mass;
							if (center_of_gravity.x >= box.a.x && center_of_gravity.x <= box.b.x &&
									center_of_gravity.y >= box.a.y && center_of_gravity.y <= box.b.y &&
									center_of_gravity.z >= box.a.z && center_of_gravity.z <= box.b.z ) {
										system_A.insert(*new Molecule(*old_molecule));
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


				HashGridBox3<Atom*>* hbox;
				HashGridBox3<Atom*>::BoxIterator box_it;
				HashGridBox3<Atom*>::DataIterator data_it;
				Vector3 position = (*atom_it).getPosition();

				float atomic_mass = (*atom_it).getElement().getAtomicWeight();
				center_of_gravity += (atomic_mass * position);
				mass += atomic_mass; 
				atom_counter++;
				
				hbox = grid.getBox(position);

				for (box_it = hbox->beginBox(); +box_it && add; ++box_it)
				{
					for (data_it = (*box_it).beginData(); +data_it && add ; ++data_it)
					{
						if ((position.getSquareDistance((*data_it)->getPosition())) < square_distance)
						{
							add = false;
						}
					}
				}
			}
			return(mol_counter);
		}

	}	// namespace MolmecSupport

} // namespace BALL
