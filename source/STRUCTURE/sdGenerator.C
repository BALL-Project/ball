// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringAnalyser.h>

#include <BALL/MATHS/matrix44.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/selector.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>

#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/PTE.h>

#include <algorithm>

//#define BALL_DEBUG_SDGENERATOR

#ifdef BALL_DEBUG_SDGENERATOR
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{
	const char* SDGenerator::Option::SHOW_HYDROGENS       = "sd_generator_show_hydrogens";
	const char* SDGenerator::Option::STANDARD_BOND_LENGTH = "sd_generator_standard_bond_length";

	const bool  SDGenerator::Default::SHOW_HYDROGENS       = true;
	const float SDGenerator::Default::STANDARD_BOND_LENGTH = 2.0f;
 
	SDGenerator::SDGenerator(bool show_hydrogens)
	{
		setDefaultOptions();
		options[SDGenerator::Option::SHOW_HYDROGENS] = show_hydrogens;
	}

	SDGenerator::~SDGenerator()
	{
	}

	void SDGenerator::prepare_()
	{
		// check if all hydrogen atoms should be deleted from the System
		bool show_H = options.getBool(Option::SHOW_HYDROGENS);

		if (!show_H)
		{
			std::list<Atom*> to_delete;

			AtomIterator at_it;
			BALL_FOREACH_ATOM(*system_, at_it)
			{
				if (at_it->getElement().getSymbol() == "H")
				{
					to_delete.push_back(&*at_it);
					Atom* partner = at_it->getBond(0)->getPartner(*at_it);

					if (partner->hasProperty("SDGenerator::NUM_REMOVED_HYDROGENS"))
					{
						int num_removed = partner->getProperty("SDGenerator::NUM_REMOVED_HYDROGENS").getInt();
						partner->setProperty("SDGenerator::NUM_REMOVED_HYDROGENS", ++num_removed);
					}
					else
						partner->setProperty("SDGenerator::NUM_REMOVED_HYDROGENS", 1);
				}
			}

			for (std::list<Atom*>::iterator d_it = to_delete.begin(); d_it != to_delete.end(); ++d_it)
				delete(*d_it);

			DEBUG("all H's removed")
		}

		// compute the smallest set of smallest rings and analyze them
		// NOTE: the RingAnalyser sets the "InRing" property for us
		system_->apply(ring_analyser_);

		// compute the Shelley priorities for each atom
		computeShelleyPriorities_();

		AtomIterator at_it;

		// distinguish between ring-atoms and core-chain-atoms
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			//  declare atoms as core-chain if they fulfil the following conditions:
			//   - acylic
			//   - have at least two neighbours, at least one of which is acyclic
			//   - have at least one acyclic beta atom on the other side of an acyclic neighbour (??? TODO!)
			//   - do not have an adjacent triple bond, or two adjacent double-bonds
			//
			// also, switch to equal angle distribution, if
			//   - all substituents are heteroatoms
			//   - three or more substituents are primary heteroatoms
			//   - atom has four or more substituents, and at least one double bond
			//   - two or more substituents have more than 6 beta atoms
			if (!at_it->getProperty("InRing").getBool())
			{
				Size acyclic_neighbours = 0;
				Size num_triple_bonds = 0;
				Size num_double_bonds = 0;
				bool has_acyclic_beta = false;

				bool only_hetero_substituents = true;
				Size num_primary_hetero_substituents = 0;
				Size num_congested_substituents = 0;

				// - have at least two neighbours
				if (at_it->countBonds() > 1)
				{
					Atom::BondIterator bond_it;
					BALL_FOREACH_ATOM_BOND(*at_it, bond_it)
					{
						if (bond_it->getOrder() == Bond::ORDER__DOUBLE)
							num_double_bonds++;

						else if (bond_it->getOrder() == Bond::ORDER__TRIPLE)
							num_triple_bonds++;

						// - at least one of the neighbours must be acyclic
						Atom* partner = bond_it->getPartner(*at_it);
						if (!(partner->getProperty("InRing").getBool()))
						{
							++acyclic_neighbours;
						}
						
						const String& partner_element = partner->getElement().getSymbol();
						if (partner_element == "H" || partner_element == "C")
							only_hetero_substituents = false;
						else if (partner->countBonds() == 1)
							++num_primary_hetero_substituents;

						if (partner->countBonds() >= 6)
							++num_congested_substituents;

						// - and have another acyclic neighbour
						Atom::BondIterator next_bond_it;
						BALL_FOREACH_ATOM_BOND(*partner, next_bond_it)
						{
							Atom* beta = next_bond_it->getPartner(*partner);

							if (beta == &*at_it)
								continue;

							if (!beta->getProperty("InRing").getBool())
							{
								has_acyclic_beta = true;
								break;
							}
						}

					}

					// - check for all of the conditions and decide whether the atom might be core-chain or not
					if ((acyclic_neighbours > 0) && (num_double_bonds < 2) && (num_triple_bonds == 0))// && has_acyclic_beta)
					{
						// later used in the chain analysis
						if (    only_hetero_substituents 
							  || (num_primary_hetero_substituents >= 3)
								|| ((at_it->countBonds() >= 4) && (num_double_bonds >= 1))
								|| (num_congested_substituents >= 2))
						{
							at_it->setProperty(SDGenerator::EQAS);
						}
						else
						{
							at_it->setProperty(SDGenerator::CORE_CHAIN);
							at_it->setProperty(SDGenerator::FXAS);
						}
					}
				}
			}
		}

		DEBUG("\t-*-[checkAtoms]:\tRing-atoms and core-chain-atoms found.")
	}

	void SDGenerator::clear()
	{
		ring_analyser_.clear();
		chains_.clear();

		// priority_queue has no clear() function...
		if (!redraw_queue_.empty())
		{
			Size to_pop = redraw_queue_.size();
			for (Position i=0; i<to_pop; ++i)
				redraw_queue_.pop();
		}

		// now remove all properties we put in (apart from InRing - this one which 
		// might come in handy later...)
		AtomIterator at_it;
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			for (Position i=FIRST_SDGENERATOR_PROPERTY; i<=LAST_SDGENERATOR_PROPERTY; ++i)
			{
				if (at_it->hasProperty(i))
					at_it->clearProperty(i);
			}

			// also, we had some named properties we need to erase
			String properties[9] = {"SDGenerator::NUM_REMOVED_HYDROGENS",
			                        "SDGenerator::PRIORITY",
			                        "SDGenerator::CFS_high",
			                        "SDGenerator::CFS_low",
			                        "SDGenerator::CFS_old_high",
			                        "SDGenerator::CFS_old_low",
			                        "SDGenerator::AngularDemand",
			                        "SDGenerator::PFU_x_pos",
			                        "SDGenerator::PFU_y_pos"};

			for (Position i=0; i<9; ++i)
			{
				if (at_it->hasProperty(properties[i]))
					at_it->clearProperty(properties[i]);
			}
		}
	}

	void SDGenerator::generateSD(System& molecule_sys)
	{
		// store the system
		system_ = &molecule_sys;

		// make sure that there is no old crap lying around
		clear();

		// distinguish between core-chain-atoms, ring-atoms, and others
		prepare_();

		for (Size i = 0; i < ring_analyser_.getNumberOfRingSystems(); i++)
		{
			// construct each ringsystem in the suitable way
			constructRingSystem_(i);
		}

		treatChains_();

		// assemble the Structure Diagram from the prepared Fragments
		assembleSD_();

		// put the hydrogens back
		std::list<Atom*> to_add;

		AtomIterator at_it;
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			if (at_it->hasProperty("SDGenerator::NUM_REMOVED_HYDROGENS"))
			{
				to_add.push_back(&*at_it);
			}
		}

		for (std::list<Atom*>::iterator at_it = to_add.begin(); at_it != to_add.end(); ++at_it)
		{
			// add them back
			int num_hydrogens = (*at_it)->getProperty("SDGenerator::NUM_REMOVED_HYDROGENS").getInt();

			Angle cfs_low  = getCFS_(*at_it, false); 
			Angle cfs_high = getCFS_(*at_it, true); 

			Angle demand = cfs_high - cfs_low;
			demand.normalize(Angle::RANGE__UNSIGNED);
			if (demand.toRadian() == 0)
				demand.set(2.*M_PI);

			float new_beta = demand.toRadian() / (num_hydrogens+1);

			for (int i=0; i<num_hydrogens; ++i)
			{
				Atom *new_hydrogen   = new Atom(PTE["H"], "H");
				Vector3 new_position = (*at_it)->getPosition();

				new_position.x += cos(cfs_low.toRadian() + (i+1)*new_beta);
				new_position.y += sin(cfs_low.toRadian() + (i+1)*new_beta);

				new_hydrogen->setPosition(new_position);

				(*at_it)->getMolecule()->insert(*new_hydrogen);
				new_hydrogen->createBond(**at_it)->setOrder(Bond::ORDER__SINGLE);
			}
		}

		// be nice and clean up old stuff
		clear();

		DEBUG("Structure Diagram has been generated.")
	}

	void SDGenerator::computeShelleyPriorities_()
	{
		// compute the Shelley score for each atom, which is defined as follows:
		//
		// S_a,0 = 1 + 2 \delta (\delta = 1 if a cyclic; 0 otherwise)
		// S_a,n = 3S_a,n-1 + \sum_j S_j,n-1
		//
		// In the original formulation, the algorithm iterates until no new score
		// classes are created. Here, we just use the heuristics given by Helson to
		// iterate 5 times in practice.
		//

		std::map<Atom*, int> last_scores, new_scores;
		
		AtomIterator at_it;
		BALL_FOREACH_ATOM(*system_, at_it) 
		{
			last_scores[&*at_it] = (at_it->getProperty("InRing").getBool()) ? 2 : 0;
		}

		for (Position i=0; i<5; ++i)
		{
			BALL_FOREACH_ATOM(*system_, at_it)
			{
				int& current_score = new_scores[&*at_it];
				current_score = 3*last_scores[&*at_it];

				Atom::BondIterator b_it;
				BALL_FOREACH_ATOM_BOND(*at_it, b_it)
				{
					current_score += last_scores[b_it->getPartner(*at_it)];	
				}
			}

			std::swap(last_scores, new_scores);
		}

		// save the values as properties in the atoms
		// NOTE: we always swap new_scores and last_scores in the loop,
		//       so after the algorithm terminates, last_scores contains
		//       the final values... alright, alright, don't shout....
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			at_it->setProperty("SDGenerator::PRIORITY", last_scores[&*at_it]);
		}
	}

	void SDGenerator::buildRegularPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, bool clockwise)
	{
		Position second_anchor_index = (first_anchor_index + 1) % ring.atoms.size();

		Vector3& anchor_start = ring.atoms[first_anchor_index]->getPosition();
		Vector3& anchor_end   = ring.atoms[second_anchor_index]->getPosition();

		Vector3 anchor = anchor_end - anchor_start;

		float phi = 2.*M_PI / ring.atoms.size();

		float l = anchor.getLength();
		float r = l/(2.*sin(phi/2.));
		float h = l/(2.*tan(phi/2.));

		Vector3 orth_anchor(anchor.y, -anchor.x, 0.);
		orth_anchor.normalize();

		if (!clockwise)
		{
			orth_anchor *= -1.f;
			phi *= -1.;
		}

		Vector3 center          = anchor_start + anchor*0.5 + orth_anchor*h;
		Vector3 center_to_start = anchor_start - center;

		float phi_0 = ((center_to_start.x > 0) ? +1. : -1.) * acos(std::max(std::min(center_to_start.y/r, 1.f), -1.f));

		Position current_atom_index = (second_anchor_index + 1) % ring.atoms.size();

		for (Position i=2; i<ring.atoms.size(); ++i)
		{
			Vector3 new_position = center + Vector3(r*sin(phi_0 + i*phi), r*cos(phi_0 + i*phi), 0);
			ring.atoms[current_atom_index]->setPosition(new_position);
			ring.atoms[current_atom_index]->setProperty(SDGenerator::DEPOSITED);
			current_atom_index = (current_atom_index + 1) % ring.atoms.size();
		}

	}

	void SDGenerator::buildOpenPolygon_(RingAnalyser::Ring& ring, Position first_anchor_index, Position second_anchor_index)
	{
		const size_t N = ring.atoms.size();
		double alpha = 2 * M_PI / N;

		double bond_length = options.getReal(Option::STANDARD_BOND_LENGTH);

		double l = (bond_length / 2) / cos(alpha / 2.);
		Vector3 d_vect = ring.atoms[second_anchor_index]->getPosition()
		               - ring.atoms[first_anchor_index]->getPosition();

		const double D = d_vect.getLength();

		double hh = l*l - D*D/4.;

		while (hh < 0)
		{
			bond_length *= 1.2;
		  l = (bond_length / 2) / cos((alpha) / 2.);
		  hh = l*l - D*D/4.;
		}
		const double h = sqrt(hh);

		Vector3 d_ortho(d_vect.y, -d_vect.x, 0);

		// find out in which direction we should attach the new ring
		// (it should be the direction pointing away from the center of
		// gravity of all previously deposited atoms)

		int pos = 1;
		Vector3 cog_deposited;
		for (Position i = first_anchor_index  % N; i != (second_anchor_index + 1) % N; i = (i + 1) % N, ++pos)
		{
			cog_deposited += ring.atoms[i]->getPosition();	
		}
		cog_deposited /= pos;

		Vector3 mid_anchor = ring.atoms[first_anchor_index]->getPosition() + d_vect/2;

		if ((cog_deposited - mid_anchor) * d_ortho > 0)
		{
			d_ortho *= -1.0;
			alpha *= -1.0;
		}

		Vector3 center = ring.atoms[first_anchor_index]->getPosition() + d_vect/2 + d_ortho*(h/D);
		Vector3 center_to_start = ring.atoms[second_anchor_index]->getPosition() - center;

		float phi_0 = ((center_to_start.x > 0) ? +1. : -1.) * acos(std::min(center_to_start.y/l, 1.));

		pos = 1;
		for (int i = (second_anchor_index + 1) % N; i != (int)first_anchor_index; i = (i + 1) % N, ++pos)
		{
			Vector3 new_position(center.x + l*sin(phi_0 + pos*alpha), center.y + l*cos(phi_0 +pos*alpha), 0);
			ring.atoms[i]->setPosition(new_position);
			ring.atoms[i]->setProperty(SDGenerator::DEPOSITED);
		}

	}

	void SDGenerator::constructRingSystem_(Position current_ring_system_index)
	{
		std::vector<RingAnalyser::Ring> current_system = ring_analyser_.getRingSystem(current_ring_system_index);
		std::vector<Position> peeling_order = ring_analyser_.getPeelingOrder(current_ring_system_index);

		// undo the peeling backwards
		for (std::vector<Position>::reverse_iterator ring_it = peeling_order.rbegin(); ring_it != peeling_order.rend(); ++ring_it)
		{
			RingAnalyser::Ring& current_ring = current_system[*ring_it];

			switch (current_ring.type)
			{
				case RingAnalyser::CORE:	
					DEBUG("Attaching Core");
					attachCore_(*ring_it, current_system, (float)current_ring_system_index);
					break;
				case RingAnalyser::TEMPLATE:
					DEBUG("Attaching Template");
					attachTemplate_(*ring_it, current_system);
					break;
				case RingAnalyser::SPIRO:
					DEBUG("Attaching Spiro");
					attachSpiro_(*ring_it, current_system);
					break;
				case RingAnalyser::FUSED:
					DEBUG("Attaching Fused");
					attachFused_(*ring_it, current_system);
					break;
				case RingAnalyser::BRIDGED:
					DEBUG("Attaching Bridged");
					attachBridged_(*ring_it, current_system);
					break;
				default:
					Log.error() << "SDGenerator::constructRingSystem_(): Cannot attach unknown ring type!" << std::endl;
					break;
			}
		}

		// finally, compute the angular demands of each atom
		for (Position i=0; i<current_system.size(); ++i)
		{
			RingAnalyser::Ring& current_ring = current_system[i];

			for (Position j=0; j<current_ring.atoms.size(); ++j)
			{
				computeAngularDemand_(current_ring.atoms[j]);
			}
		}
	}

	Angle SDGenerator::getCFS_(Atom const* atom, bool high)
	{
		Angle result(atom->getProperty(high ? "SDGenerator::CFS_high" : "SDGenerator::CFS_low").getFloat(), true);

		return result;
	}

	Angle SDGenerator::computeCFS_(Vector3 const& input)
	{
		float alpha = atan2(input.y, input.x);

		if (alpha < 0.)
		{
			alpha += 2.*M_PI;
		}

		return Angle(alpha, true);
	}

	void SDGenerator::setCFS_(Atom* atom, Angle cfs, bool high)
	{
		cfs.normalize(Angle::RANGE__UNSIGNED);
		atom->setProperty(high ? "SDGenerator::CFS_high" : "SDGenerator::CFS_low", cfs.toRadian());
	}

	void SDGenerator::pushCFS_(Atom* atom)
	{
		if (atom->hasProperty("SDGenerator::CFS_high") && atom->hasProperty("SDGenerator::CFS_low"))
		{
			atom->setProperty("SDGenerator::CFS_old_high", getCFS_(atom, true).toRadian());
			atom->setProperty("SDGenerator::CFS_old_low", getCFS_(atom, false).toRadian());
		}
	}

	Angle SDGenerator::getBackupCFS_(Atom const* atom, bool high)
	{
		Angle result(atom->getProperty(high ? "SDGenerator::CFS_old_high" : "SDGenerator::CFS_old_low").getFloat(), true);

		return result;
	}


	Angle SDGenerator::computeAngularDemand_(Atom* seed)
	{
		// do we already know the angular demand?
		if (seed->hasProperty("SDGenerator::AngularDemand"))
		{
			return (Angle(seed->getProperty("SDGenerator::AngularDemand").getFloat(), true));
		}

		// the angular demand is the angle covered by the PFU, so the opposite angle of the CFS
		Angle CFS_hi = getCFS_(seed, true);
		Angle CFS_lo = getCFS_(seed, false);

		Angle demand = CFS_lo - CFS_hi;
		demand.normalize(Angle::RANGE__UNSIGNED);

		seed->setProperty("SDGenerator::AngularDemand", demand.toRadian());

		return demand;
	}

	void SDGenerator::computeCoreCFS_(RingAnalyser::Ring& ring, bool clockwise)
	{
		// store the initial CFS values
		for (Position i=0; i<ring.atoms.size(); ++i)
		{
			Atom* center_atom = ring.atoms[i];

			Vector3& center = center_atom->getPosition();

			Vector3 low  = ring.atoms[ring.successor(i)  ]->getPosition() - center;
			Vector3 high = ring.atoms[ring.predecessor(i)]->getPosition() - center;

			Angle cfs_low  = computeCFS_(low);
			Angle cfs_high = computeCFS_(high);

			if (!clockwise)
				std::swap(cfs_low, cfs_high);

			setCFS_(center_atom, cfs_high, true);
			setCFS_(center_atom, cfs_low,  false);
		}
	}

	void SDGenerator::attachCore_(Size core_index, std::vector<RingAnalyser::Ring>& current_system, float x_start)
	{
		std::vector<Atom*>& ring = current_system[core_index].atoms;

		// set the standard bond-length
		double bond_length = options.getReal(Option::STANDARD_BOND_LENGTH);

		// prepare the first two atom positions
		if (ring.size() % 2)
		{ 
			// odd numbered ring
			ring[0]->setPosition(Vector3(0.f, x_start, 0.f));
			ring[1]->setPosition(Vector3(bond_length, x_start, 0.f));
		}
		else
		{
			// even numbered ring
			ring[0]->setPosition(Vector3(x_start, 0.f, 0.f));
			ring[1]->setPosition(Vector3(x_start, bond_length, 0.f));
		}

		ring[0]->setProperty(SDGenerator::DEPOSITED);
		ring[1]->setProperty(SDGenerator::DEPOSITED);

		buildRegularPolygon_(current_system[core_index], 0, true);
		current_system[core_index].setProperty(SDGenerator::DEPOSITED);

		computeCoreCFS_(current_system[core_index], true);

		DEBUG("\t-*-[RSConstructor]:\t (attachCore_):\t done.")
	}

	// interface for a Ring Template Database 
	// if a RTD will be installed, this interface can be used to get information from it
	void SDGenerator::attachTemplate_(Position /* current_ring */, std::vector<RingAnalyser::Ring>& /*current_system*/)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachTemplate):\t started.")
	}

	// attach a fused-ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachFused_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachFused_):\t\t started.")

		// the fused ring that is to be attached
		std::vector<Atom*>& ring = current_system[current_ring_index].atoms;

		// the bonds this ring shares with other rings
		std::set<Bond const*>& shared_bonds = current_system[current_ring_index].shared_bonds;

		// find the two atoms that are shared by the ring that is to be 
		// constructed with another ring of the ring system that has already
		// been deposited

		// compute all ring memberships
		HashMap<Atom const*, std::set<Position> > rings_per_atom;

		for (Position i=0; i<current_system.size(); ++i)
		{
			for (Position j=0; j<current_system[i].atoms.size(); ++j)
			{
				rings_per_atom[current_system[i].atoms[j]].insert(i);
			}
		}

		Bond const* shared_bond = NULL;
		Atom const* first_atom  = NULL;
		Atom const* second_atom = NULL;

		Position neighbouring_ring;

		for (std::set<Bond const*>::iterator bond_it  = shared_bonds.begin(); 
		                                     bond_it != shared_bonds.end(); ++bond_it)
		{
			first_atom  = (*bond_it)->getFirstAtom();
			second_atom	= (*bond_it)->getSecondAtom();

			std::set<Position>&  first_rings = rings_per_atom[first_atom];
			std::set<Position>& second_rings = rings_per_atom[second_atom];

			std::set<Position> intersection;

			std::set_intersection(first_rings.begin(),  first_rings.end(),
			                      second_rings.begin(), second_rings.end(),
														std::inserter(intersection, intersection.begin()));

			for (std::set<Position>::iterator int_it = intersection.begin(); int_it != intersection.end(); ++int_it)
			{
				if (current_system[*int_it].hasProperty(SDGenerator::DEPOSITED))
				{
					shared_bond = *bond_it;
					neighbouring_ring = *int_it;
					break;
				}
			}

			if (shared_bond)
				break;
		}

		// if we have found a suitable bond...
		if (shared_bond)
		{
			// now we need to know if both rings are sequenced such that they see the bond in the same
			// direction, i.e. that the first atom of the bond preceeds the second one in both rings, or
			// vice versa

			// find the bond in the old ring that will be taken for the correct direction of the bonds 
			// in the new ring, i.e. the bond between the second shared atom and the next atom in the old ring
			Position first_atom_index=0, second_atom_index=0;
			for (Position i=0; i<ring.size(); ++i)
			{
				if (ring[i] == first_atom)
					first_atom_index = i;
				if (ring[i] == second_atom)
					second_atom_index = i;
			}

			Index index_diff = second_atom_index - first_atom_index;

			bool forward;

			if (abs(index_diff) == 1)
			{
				forward = (index_diff > 0);
			}
			else
			{
				forward = (second_atom_index == 0);
			}

			if (!forward)
			{
				std::swap(first_atom_index, second_atom_index);
			}

			std::vector<Atom*>& ref_ring = current_system[neighbouring_ring].atoms;

			// compute the center of the deposited neighbor
			Position num_depos_atoms = 1;
			Vector3 deposited_cog;
			for (Position i=0; i<ref_ring.size(); ++i, ++num_depos_atoms)
			{
				if (ref_ring[i]->hasProperty(SDGenerator::DEPOSITED))
					deposited_cog += ref_ring[i]->getPosition();
			}
			deposited_cog /= num_depos_atoms;

			Vector3 anchor = ring[second_atom_index]->getPosition() - ring[first_atom_index ]->getPosition();

			Vector3 orth_anchor(anchor.y, -anchor.x, 0.);
			Vector3 center = ring[first_atom_index]->getPosition() + anchor*0.5;
			
			bool clockwise = true;
			if ((deposited_cog - center) * orth_anchor > 0)
				clockwise = false;

			buildRegularPolygon_(current_system[current_ring_index], first_atom_index, clockwise);

			// store the old CFS of the shared atoms
			Angle old_CFS_lo_first = getCFS_(ring[first_atom_index], false);
			Angle old_CFS_hi_first = getCFS_(ring[first_atom_index], true);

			Angle old_CFS_lo_second = getCFS_(ring[second_atom_index], false);
			Angle old_CFS_hi_second = getCFS_(ring[second_atom_index], true);

			computeCoreCFS_(current_system[current_ring_index], clockwise);

			// and retrieve the new CFS values
			Angle new_CFS_lo_first = getCFS_(ring[first_atom_index], false);
			Angle new_CFS_hi_first = getCFS_(ring[first_atom_index], true);

			Angle new_CFS_lo_second = getCFS_(ring[second_atom_index], false);
			Angle new_CFS_hi_second = getCFS_(ring[second_atom_index], true);

			Angle anchor_cfs = computeCFS_(anchor);

			// test which of the vectors are collinear with the bond we attach to
			if (fabs((old_CFS_hi_first - anchor_cfs).toRadian()) < 1e-3)
			{
				// in this case, we take the low vector from the first and the high from the second ring
				// for the first atom, and for the second atom vice versa
				setCFS_(ring[first_atom_index], old_CFS_lo_first, false);
				setCFS_(ring[first_atom_index], new_CFS_hi_first, true);

				setCFS_(ring[second_atom_index], new_CFS_lo_first, false);
				setCFS_(ring[second_atom_index], old_CFS_hi_first, true);
			}
			else
			{
				// in this case, we take the high vector from the first and the low from the second ring
				// for the first atom, and for the second atom vice versa
				setCFS_(ring[first_atom_index], new_CFS_lo_first, false);
				setCFS_(ring[first_atom_index], old_CFS_hi_first, true);

				setCFS_(ring[second_atom_index], old_CFS_lo_first, false);
				setCFS_(ring[second_atom_index], new_CFS_hi_first, true);
			}
			
			// and determine, which of these is the new high and which the new low

			current_system[current_ring_index].setProperty(SDGenerator::DEPOSITED);

			DEBUG("\t-*-[SDGenerator]:\t (attachFused):\t done.")
		}
		//      if not both of the shared atoms have been found, append the cuurent ring at the end of the ringsystem and construct it later
		else
		{
// TODO: do i have to append it again?
			DEBUG("\t-*-[SDGenerator]:\t (attachFused):\t aborted. Ring appended.")
		}
	}

	// attach a bridged ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachSpiro_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		// for now, we do something extremely simple: construct a core-ring and translate it
		// to the correct position.
		// TODO: take care of the overlaps this procedure *will* produce!
		RingAnalyser::Ring& current_ring = current_system[current_ring_index];

		Position i;
		for (i=0; i<current_ring.atoms.size(); ++i)
		{
			if (current_ring.atoms[i]->hasProperty(SDGenerator::DEPOSITED))
				break;
		}

		if (i == current_ring.atoms.size())
		{
			Log.error() << "SDGenerator::attachSpiro_(): Can only attach rings with one deposited atom!" << std::endl;
			return;
		}

		Vector3 correct_position = current_ring.atoms[i]->getPosition();

		attachCore_(current_ring_index, current_system, 0);

		Vector3 translation = correct_position - current_ring.atoms[i]->getPosition();

		for (Position i=0; i<current_ring.atoms.size(); ++i)
		{
			current_ring.atoms[i]->setPosition(current_ring.atoms[i]->getPosition() + translation);
		}

		// TODO: update the CFS correctly!
		current_ring.setProperty(SDGenerator::DEPOSITED);
	}

	// attach a bridged ring to a (partially) prefabricated ringsystem
	void SDGenerator::attachBridged_(Position current_ring_index, std::vector<RingAnalyser::Ring>& current_system)
	{
		// construct a BRIDGED ring by the open polygon - method
		DEBUG("\t-*-[SDGenerator]:\t (attachBridged):\t started.")

		if(current_system[current_ring_index].hasProperty(SDGenerator::DEPOSITED))
		{
			return;
		}

		RingAnalyser::Ring& ring  = current_system[current_ring_index];
		std::vector<Atom*>& atoms = ring.atoms;

		//Make sure the ring contains at least two deposited atoms
		Position counter = 0;
		for(Position j = 0; j < atoms.size(); ++j)
		{
			if(atoms[j]->hasProperty(SDGenerator::DEPOSITED))
			{
				++counter;
			}
		}

		if(counter < 2)
		{
			// This is clearly not a bridged ring or the peeling went wrong
			Log.error() << "SDGenerator::attachBridged_(): Can't attach ring with only one placed atom!" << std::endl;

			return;
		}
		else if (counter == atoms.size())
		{
			ring.setProperty(SDGenerator::DEPOSITED);
			//Ring is already deposited and we should bail out
			return;
		}

		// ok, let's find the first bridge head
		Position first_anchor_point = 0; 
		Position second_anchor_point = 0;

		for (Position i=0; i<atoms.size(); ++i)
		{
			bool last_deposited = atoms[ring.predecessor(i)]->hasProperty(SDGenerator::DEPOSITED);
			bool   is_deposited = atoms[i]->hasProperty(SDGenerator::DEPOSITED);
			bool next_deposited = atoms[ring.successor(i)]->hasProperty(SDGenerator::DEPOSITED);

			if (is_deposited && !last_deposited)
			{
				first_anchor_point = i;
			}

			if (is_deposited && !next_deposited)
			{
				second_anchor_point = i;
			}
		}

		// if both bridge heads are next to each other, we can treat this like a fused ring
		if (counter == 2)
		{
			attachFused_(current_ring_index, current_system);
			return;
		}
		else
		{
			buildOpenPolygon_(ring, first_anchor_point, second_anchor_point);
		}

		// store the old CFS values at the anchor
		Angle first_CFS_old_low  = getCFS_(atoms[first_anchor_point], false);
		Angle first_CFS_old_high = getCFS_(atoms[first_anchor_point], true);

		Angle second_CFS_old_low  = getCFS_(atoms[second_anchor_point], false);
		Angle second_CFS_old_high = getCFS_(atoms[second_anchor_point], true);

		// now, update the circular free sweep
		computeCoreCFS_(ring, ringIsClockwise_(ring, second_anchor_point));

		// and get the new CFS values at the anchor
		Angle first_CFS_new_low  = getCFS_(atoms[first_anchor_point], false);
		Angle first_CFS_new_high = getCFS_(atoms[first_anchor_point], true);

		Angle second_CFS_new_low  = getCFS_(atoms[second_anchor_point], false);
		Angle second_CFS_new_high = getCFS_(atoms[second_anchor_point], true);

		// the correct CFS values are the intersection of the old ones
		// TODO: this is most probably incorrect...

		setCFS_(atoms[first_anchor_point], std::min(first_CFS_old_low,  first_CFS_new_low),  false);
		setCFS_(atoms[first_anchor_point], std::max(first_CFS_old_high, first_CFS_new_high), true);

		setCFS_(atoms[second_anchor_point], std::min(second_CFS_old_low,  second_CFS_new_low),  false);
		setCFS_(atoms[second_anchor_point], std::max(second_CFS_old_high, second_CFS_new_high), true);

		ring.setProperty(SDGenerator::DEPOSITED);
	}

	void SDGenerator::computeAdjacencyMatrix_(std::vector<Atom*>& chain, std::vector<bool>& result)
	{
		Size nodes = chain.size();
		
		// yes, this matrix should be symmetric, really, or even sparse. but even for large
		// chains, this does not really matter
		result.resize(nodes*nodes, false);

		for (Position i=0; i<nodes; ++i)
		{
			for (Position j=0; j<nodes; ++j)
			{
				// the diagonal should be set to true also (for the later stages)
				if ((i == j) || (chain[i]->isBoundTo(*(chain[j]))))
					result[i+j*nodes] = true;
			}
		}
	}

	bool SDGenerator::compareChains_(const vector<Atom*>& x, const vector<Atom*>& y)
	{
		return x.size() > y.size();
	}

	void SDGenerator::findFloydWarshallPath_(std::vector<int>& path, std::vector<Index>& next, Size remaining_atoms, 
	                                         Position i, Position j, std::list<Index>& output)
	{
		if (path[i+j*remaining_atoms] == std::numeric_limits<int>::max())
		{
			return;
		}

		Index intermediate = next[i+j*remaining_atoms];

		if (intermediate == -1)
		{
			return;
		}
		else
		{
			findFloydWarshallPath_(path, next, remaining_atoms, i, intermediate, output);
			output.push_back(intermediate);
			findFloydWarshallPath_(path, next, remaining_atoms, intermediate, j, output);
		}
	}

	void SDGenerator::treatChains_()
	{
		// A chain is defined as the longest continuous path between core chain atoms,
		// plus two capping (arbitrarily chosen) substituents not on the path.
		//
		// To determine the paths, we use a Floyd-Warshall algorithm with unit positive
		// edge weights. Since all core chain atoms are by definition acyclic, the graph
		// connecting them will be acyclic also, so we can use the longest shortest path
		// between arbitrary nodes as the longest path in the graph.
		//
		// We can have multiple chains in our molecule, possibly connected, so after
		// each run of the algorithm we delete all atoms on the current path from the input
		// and iterate.
		//

		std::list<Atom*> core_chain_atoms;

		// initialize the core chain atoms with all candidates
		AtomIterator at_it;
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			if (at_it->hasProperty(SDGenerator::CORE_CHAIN))
			{
				core_chain_atoms.push_back(&*at_it);
			}
		}

		// and iterate the Floyd-Warshall algorithm
		Size remaining_atoms = core_chain_atoms.size();
		while (remaining_atoms > 0)
		{
			// working with indices in the backtracking is more convenient
			std::vector<std::list<Atom*>::iterator> index_to_atom_list(remaining_atoms);

			std::list<Atom*>::iterator l_it=core_chain_atoms.begin();
			for (Position i=0; i<remaining_atoms; ++i, ++l_it)
			{
				index_to_atom_list[i] = l_it;
			}

			// initialize the path matrix
			std::vector<int> path(remaining_atoms*remaining_atoms, std::numeric_limits<int>::max());
			// and the backtracking matrix
			std::vector<Index> next(remaining_atoms*remaining_atoms, -1);

			// initialize the dp matrix
			for (Position i=0; i<remaining_atoms; ++i)
			{
				Atom* first_atom = *index_to_atom_list[i];
				path[i+i*remaining_atoms] = 0;

				for (Position j=i+1; j<remaining_atoms; ++j)
				{
					Atom* second_atom = *index_to_atom_list[j];

					if (first_atom->isBoundTo(*second_atom))
					{
						path[i+j*remaining_atoms] = 1;
						path[j+i*remaining_atoms] = 1;
					}
				}
			}

			// fill the dp matrix
			for (Position k=0; k<remaining_atoms; ++k)
			{
				for (Position i=0; i<remaining_atoms; ++i)
				{
					int& p_ik = path[i+k*remaining_atoms];

					if (p_ik == std::numeric_limits<int>::max())
						continue;

					for (Position j=0; j<remaining_atoms; ++j)
					{
						int& p_kj = path[k+j*remaining_atoms];
						int& p_ij = path[i+j*remaining_atoms];

						if (p_kj == std::numeric_limits<int>::max())
							continue;

						if (p_ik + p_kj < p_ij)
						{
							p_ij = p_ik + p_kj;
							next[i+j*remaining_atoms] = k;
						}
					}
				}
			}

			// find the path of maximal minimum length
			int max_value = std::numeric_limits<int>::min();
			Position max_i = 0, max_j = 0;

			for (Position i=0; i<remaining_atoms; ++i)
			{
				for (Position j=0; j<remaining_atoms; ++j)
				{
						int current_value = path[i+j*remaining_atoms];
						
						if ((current_value != std::numeric_limits<int>::max()) && (current_value > max_value ))
						{
							max_value = path[i+j*remaining_atoms];
							max_i = i;
							max_j = j;
						}
				}
			}

			// and do the backtracking for the path from max_i to max_j.
			chains_.push_back(std::list<Atom*>());

			std::list<Index> longest_path;
			longest_path.push_back(max_i);
			findFloydWarshallPath_(path, next, remaining_atoms, max_i, max_j, longest_path);
			if (max_i != max_j)
				longest_path.push_back(max_j);

			// finally, remove all found atoms from the input
			for (std::list<Index>::iterator l_it = longest_path.begin(); l_it != longest_path.end(); ++l_it)
			{
				chains_.back().push_back(*(index_to_atom_list[*l_it]));
				core_chain_atoms.erase(index_to_atom_list[*l_it]);
			}

			// and add one capping substituent to the beggining of the chain
			Atom::BondIterator b_it;
			Atom* first_atom  = *(chains_.back().begin());
			Atom* second_atom = (chains_.back().size() > 1) ? *(++chains_.back().begin()) : first_atom;

			BALL_FOREACH_ATOM_BOND(*first_atom, b_it)
			{
				if (b_it->getPartner(*first_atom) != second_atom)
				{
					chains_.back().push_front(b_it->getPartner(*first_atom));
					break;
				}
			}

			// and to the end
			first_atom  = chains_.back().back();
			second_atom = *(--(--chains_.back().end()));

			BALL_FOREACH_ATOM_BOND(*first_atom, b_it)
			{
				if (b_it->getPartner(*first_atom) != second_atom)
				{
					chains_.back().push_back(b_it->getPartner(*first_atom));
					break;
				}
			}

			remaining_atoms = core_chain_atoms.size();
		}
	}

	Angle SDGenerator::computeAngularSeparation_(Atom* seed)
	{
		Angle demand = computeAngularDemand_(seed);
		Size num_sub = 0;

		HashSet<Bond*> unplaced_PFU_bonds;

		bool seed_in_ring = seed->getProperty("InRing").getBool();

		Atom::BondIterator b_it;
		BALL_FOREACH_ATOM_BOND(*seed, b_it)
		{
			Atom* partner = b_it->getPartner(*seed);
			if (!partner->hasProperty(SDGenerator::ASSEMBLED))
			{
				Bond* b = &*b_it;

				// does this bond belong to a previously placed PFU?
				if (unplaced_PFU_bonds.find(b) != unplaced_PFU_bonds.end())
					continue;

				// is this bond part of a PFU at all?
				if (seed_in_ring && partner->getProperty("InRing").getBool())
				{
					// find the ring system this bond belongs to
					Position ring_system_index;
					for (ring_system_index=0; ring_system_index<ring_analyser_.getNumberOfRingSystems(); ++ring_system_index)
					{
						if (   ring_analyser_.isInRingSystem(seed,    ring_system_index) 
							  && ring_analyser_.isInRingSystem(partner, ring_system_index))
						{
							break;
						}
					}

					// place all its bonds into the list
					std::vector<RingAnalyser::Ring> ring_system = ring_analyser_.getRingSystem(ring_system_index);

					for (Position current_ring_index = 0; current_ring_index < ring_system.size(); ++current_ring_index)
					{
						RingAnalyser::Ring const& current_ring = ring_system[current_ring_index];

						for (Position atom=0; atom < current_ring.atoms.size(); ++atom)
						{
							Bond* b = current_ring.atoms[atom]->getBond(*(current_ring.atoms[current_ring.successor(atom)]));

							unplaced_PFU_bonds.insert(b);
						}
					}
				} 
				else 
				{
					++num_sub;
				}
			}
		}

		if (seed->hasProperty(SDGenerator::HEAD))
			--num_sub;
		
		if (seed->hasProperty(SDGenerator::FXAS))
		{
			--num_sub;
			demand += Angle(2./3.*M_PI);
		}

		Angle free_CFS(2.*M_PI - demand.toRadian(), true);
		free_CFS.normalize(Angle::RANGE__UNSIGNED);

		if (fabs(free_CFS.toRadian()) < 1e-4)
			free_CFS.set(2.*M_PI, true);

		return Angle((free_CFS.toRadian())/(num_sub + 1));
	}

	std::vector<Atom*> SDGenerator::sequenceSubstituents_(Atom* seed)
	{
		std::vector<Atom*> result;

		// if this atom is a chain atom, find all its chains
		bool in_chain = seed->hasProperty(SDGenerator::CORE_CHAIN);

		std::list<Atom*> seed_chain;

		bool found = false;
		if (in_chain)
		{
			for (std::list<std::list<Atom*> >::iterator chains_it = chains_.begin(); 
			     chains_it != chains_.end() && !found; ++chains_it)
			{
				for (std::list<Atom*>::iterator chain_it = chains_it->begin(); chain_it != chains_it->end(); ++chain_it)
				{
					if (*chain_it == seed)
					{
						seed_chain = *chains_it;
						found = true;
						break;
					}
				}
			}
		}

		// we need the set of substituents to treat, and already treated ones
		std::list<Atom*> left, done;
		
		Atom::BondIterator b_it;
		BALL_FOREACH_ATOM_BOND(*seed, b_it)
		{
			Atom* partner = b_it->getPartner(*seed);
			if (partner->hasProperty(SDGenerator::ASSEMBLED))
				done.push_back(partner);
			else
				left.push_back(partner);
		}

		Atom* last_substituent = NULL;
		while (!left.empty())
		{
			Atom* next_substituent = NULL;

			// if the seed atom is in a chain...
			if (in_chain)
			{
				// if the last atom was the CW chain neighbour and the CCW belongs to left...
				if (   last_substituent 
					  && last_substituent->hasProperty(SDGenerator::CORE_CHAIN))
				{
					bool found_cw = false;

					for (std::list<Atom*>::iterator at_it = seed_chain.begin(); at_it != seed_chain.end(); ++at_it)
					{
						if (*at_it == last_substituent)
						{
							found_cw = true;
							break;
						}
					}

					if (found_cw)
					{
						// ok. now try to find the CCW neighbour in left
						bool found_ccw = false;
						std::list<Atom*>::iterator n_it;
						for (n_it = left.begin(); n_it != left.end() && !found_ccw; ++n_it)
						{
							if ((*n_it)->hasProperty(SDGenerator::CORE_CHAIN))
							{
								for (std::list<Atom*>::iterator c_it = seed_chain.begin(); c_it != seed_chain.end(); ++c_it)
								{
									if (*c_it == *n_it)
									{
										found_ccw = true;
										next_substituent = *n_it;
										break;
									}
								}
							}
						}
					}
				}

				// otherwise, if done is empty, use the CW chain neigbour.
				if (!next_substituent && done.empty())
				{
					// use one of the neighbours in one of the chains
					for (std::list<Atom*>::iterator n_it = seed_chain.begin(); n_it != seed_chain.end(); ++n_it)
					{
						if (*n_it == seed)
						{
							if (n_it == seed_chain.begin())
							{
								next_substituent = seed_chain.back();
							}
							else
							{
								next_substituent = *(--n_it);
							}
							break;
						}
					}
				}
			}

			if (!next_substituent)
			{
				// otherwise, set s to the highest priority atom of left
				int max_value = std::numeric_limits<int>::min();
				for (std::list<Atom*>::iterator a_it = left.begin(); a_it != left.end(); ++a_it)
				{
					if ((*a_it)->getProperty("SDGenerator::PRIORITY").getInt() > max_value)
					{
						next_substituent = *a_it;
						max_value = (*a_it)->getProperty("SDGenerator::PRIORITY").getInt();
					}
				}
			}

			// ensure that s is not from the wrong side of a PFU
			if (seed->getProperty("InRing").getBool() && next_substituent->getProperty("InRing").getBool())
			{
				// get the atom's current CFS_hi
				Angle seed_CFS_hi = getCFS_(seed, true);

				Position i;
				// find the PFU (the ring system) containing the bond
				for (i=0; i<ring_analyser_.getNumberOfRingSystems(); ++i)
				{
					if (ring_analyser_.isInRingSystem(seed, i))
					{
						// every atom can only be a member of one ring system
						break;
					}
				}
				
				// this should not happen if noone screwed up the properties from the outside
				if (i >= ring_analyser_.getNumberOfRingSystems())
				{
					Log.error() << "SDGenerator::sequenceSubstituents_(): invalid ring system found!" << std::endl;
				}
				else
				{
					// find the bond in the PFU adjacent to seed_atom that equals seed_atom's CFS_hi 
					// (this has to exist)
					Atom::BondIterator b_it;
					BALL_FOREACH_ATOM_BOND(*seed, b_it)
					{
						Atom* partner = b_it->getPartner(*seed);

						if (ring_analyser_.isInRingSystem(partner, i))
						{
							Vector3 bond = partner->getPosition() - seed->getPosition();

							Angle bond_CFS = computeCFS_(bond);
							
							if (fabs(bond_CFS - seed_CFS_hi) < 1e-3)
							{
								next_substituent = partner;
								break;
							}
						}
					}

					// remove all pfu atoms from left and put them into done
					std::vector<RingAnalyser::Ring> pfu = ring_analyser_.getRingSystem(i);

					for (Position j=0; j<pfu.size(); ++j)
					{
						std::vector<Atom*> ring = pfu[j].atoms;

						for (Position k=0; k<ring.size(); ++k)
						{
							std::list<Atom*>::iterator to_delete = std::find(left.begin(), left.end(), ring[k]);

							if (to_delete != left.end())
							{
								done.push_back(*to_delete);
								left.erase(to_delete);
							}
						}
					}
				}
			}

			result.push_back(next_substituent);

			done.push_back(next_substituent);
			left.remove(next_substituent);

			last_substituent = next_substituent;
		}

		return result;
	}

	void SDGenerator::smoothCFSAngle_(Atom* /*seed*/)
	{
		// TODO: implement the individual bond alignment!!!
	}

	void SDGenerator::placeSubstituent_(Atom* seed, Atom* head, Atom* next)
	{
		bool is_in_pfu = seed->getProperty("InRing").getBool() && next->getProperty("InRing").getBool();

		// if the seed atom equals the head atom initialize its CFS values if necessary
		if (seed == head && !(seed->hasProperty(SDGenerator::INITIALIZED_HEAD_CFS)))
		{
			pushCFS_(seed);

			// if the head atom is part of a PFU...
			if (is_in_pfu)
			{
				// use the precomputed bond vector as the new CFS high and low
				Vector3 old_head_pos;
				old_head_pos.x = seed->getProperty("SDGenerator::PFU_x_pos").getFloat();
				old_head_pos.y = seed->getProperty("SDGenerator::PFU_y_pos").getFloat();

				Vector3 bond = next->getPosition() - old_head_pos;

				setCFS_(seed, computeCFS_(bond), false);
				setCFS_(seed, computeCFS_(bond), true);
			}
			else
			{
				if (next->hasProperty(SDGenerator::CORE_CHAIN))
				{
					setCFS_(seed, Angle(1./3.*M_PI, true), false);
					setCFS_(seed, Angle(1./3.*M_PI, true), true);
				}
				else
				{
					// use an angle of zero => proceed in x direction
					setCFS_(seed, Angle(0.f, true), false);
					setCFS_(seed, Angle(0.f, true), true);
				}
			}

			seed->setProperty(SDGenerator::INITIALIZED_HEAD_CFS);
		}

		// for the bond length, use the standard value ...
		double bond_length = options.getReal(Option::STANDARD_BOND_LENGTH);

		// ... or, if the bond is in a PFU, use the precomputed length
		if (is_in_pfu)
		{
			// this is a little tricky, since the seed has already been moved
			// => use the stored seed position instead
			Vector3 old_seed_pos(0.f);
			old_seed_pos.x = seed->getProperty("SDGenerator::PFU_x_pos").getFloat();
			old_seed_pos.y = seed->getProperty("SDGenerator::PFU_y_pos").getFloat();

			bond_length = next->getPosition().getDistance(old_seed_pos);
		}

		// perform individual bond alignment
		smoothCFSAngle_(seed);

		// and place the next atom the the correct distance and angle
		Angle seed_CFS_lo = getCFS_(seed, false);

		if (next->getProperty("InRing").getBool())
		{
			pushCFS_(next);

			next->setProperty("SDGenerator::PFU_x_pos", next->getPosition().x);
			next->setProperty("SDGenerator::PFU_y_pos", next->getPosition().y);
		}
		Vector3 new_position;

		new_position.x = cos(seed_CFS_lo.toRadian())*bond_length;
		new_position.y = sin(seed_CFS_lo.toRadian())*bond_length;

		next->setPosition(seed->getPosition() + new_position);
		next->setProperty(SDGenerator::ASSEMBLED);

		// set up the next atom's CFS as pointing back towards the seed
		setCFS_(next, Angle(getCFS_(seed, false).toRadian() - M_PI), false);
		setCFS_(next, Angle(getCFS_(seed, false).toRadian() - M_PI), true);

		// and take care of the zig-zagging
		if (next->hasProperty(CORE_CHAIN))
		{
			if (!seed->hasProperty(CORE_CHAIN) || seed->hasProperty(ZIG))
			{
				next->setProperty(ZAG);
			}
			else
			{
				next->setProperty(ZIG);
			}
		}
	}

	void SDGenerator::depositPFU_(Atom* seed_atom, Atom* next_neighbour)
	{
		// we have encountered the first bond in a PFU (in our case: a ring system).
		// the seed atom and the next neighbour have already been placed, so we
		// will need their old and new coordinates
		Vector3 seed_pos_new = seed_atom->getPosition();
		Vector3 next_pos_new = next_neighbour->getPosition();

		Vector3 seed_pos_old(0.f);
		seed_pos_old.x = seed_atom->getProperty("SDGenerator::PFU_x_pos").getFloat();
		seed_pos_old.y = seed_atom->getProperty("SDGenerator::PFU_y_pos").getFloat();

		Vector3 next_pos_old(0.f);
		next_pos_old.x = next_neighbour->getProperty("SDGenerator::PFU_x_pos").getFloat();
		next_pos_old.y = next_neighbour->getProperty("SDGenerator::PFU_y_pos").getFloat();

		//and the old and new bond vectors
		Vector3 bond_new = next_pos_new - seed_pos_new;
		Vector3 bond_old = next_pos_old - seed_pos_old;

		// and the transformation matrix
		Angle alpha = bond_old.getAngle(bond_new);

		if ((bond_old % bond_new).z < 0)
			alpha *= -1.f;

		// the transform works as follows:
		//
		//   -- move the anchor of the old bond into the origin
		//   -- rotate the old bond onto the direction of the new bond
		//   -- translate the anchor of the rotated old bond to the new anchor position
		//
		Matrix4x4 transform, to_origin, rotate, translate;
		to_origin.setTranslation(-seed_pos_old);
		rotate.setRotationZ(alpha);
		translate.setTranslation(seed_pos_new);

		transform = translate * rotate * to_origin;

		// collect all other PFU atoms (excluding seed and next) and ensure we only
		// transform each of them once
		std::set<Atom*> to_transform;

		Position pfu_index;
		for (pfu_index = 0; pfu_index < ring_analyser_.getNumberOfRingSystems(); ++pfu_index)
		{
			if (ring_analyser_.isInRingSystem(seed_atom, pfu_index))
				break;
		}

		std::vector<RingAnalyser::Ring> ringsystem = ring_analyser_.getRingSystem(pfu_index);

		for (Position ring_index = 0; ring_index < ringsystem.size(); ++ring_index)
		{
			std::vector<Atom*>& ring_atoms = ringsystem[ring_index].atoms;

			for (Position atom_index = 0; atom_index < ring_atoms.size(); ++atom_index)
			{
				Atom* atom = ring_atoms[atom_index];

				if ((atom != seed_atom) && (atom != next_neighbour))
				{
					to_transform.insert(atom);
				}
			}
		}

		// and transform merrily away
		for (std::set<Atom*>::iterator atom_it = to_transform.begin(); atom_it != to_transform.end(); ++atom_it)
		{
			(*atom_it)->setPosition(transform * (*atom_it)->getPosition());
			(*atom_it)->setProperty(SDGenerator::ASSEMBLED);
			setCFS_(*atom_it, getCFS_(*atom_it, false) + alpha, false);
			setCFS_(*atom_it, getCFS_(*atom_it, true)  + alpha, true);
			redraw_queue_.push(*atom_it);
		}

		// now, we just need to put the correct CFS values into seed and next
		setCFS_(seed_atom, getCFS_(seed_atom, false) + alpha, false);
		setCFS_(seed_atom, getCFS_(seed_atom, true) + alpha, true);
		setCFS_(next_neighbour, getCFS_(next_neighbour, false) + alpha, false);
		setCFS_(next_neighbour, getCFS_(next_neighbour, true) + alpha, true);
	} 

	void SDGenerator::checkOverlap_(Atom* /*next_neighbour*/)
	{
		// TODO: implement!!!
	}

	bool SDGenerator::ringIsClockwise_(const RingAnalyser::Ring& ring, Index start_index) const
	{
		const Atom* a = ring.atoms[(start_index + 0) % ring.atoms.size()];
		const Atom* b = ring.atoms[(start_index + 1) % ring.atoms.size()];
		const Atom* c = ring.atoms[(start_index + 2) % ring.atoms.size()];

		return ((b->getPosition() - a->getPosition()) % (c->getPosition() - b->getPosition())).z < 0;
	}

	void SDGenerator::assembleSD_()
	{

		// find the atom with maximum priority
		int max_value = std::numeric_limits<int>::min();
		Atom *head_atom;

		AtomIterator at_it;
		BALL_FOREACH_ATOM(*system_, at_it)
		{
			int value = at_it->getProperty("SDGenerator::PRIORITY").getInt();		

			if (value > max_value)
			{
				max_value = value;
				head_atom = &*at_it;
			}
		}

		head_atom->setProperty(SDGenerator::HEAD);

		// position the head atom (yay!)
		if (head_atom->getProperty("InRing").getBool())
		{
			// if the head is part of a PFU, we need it's original position
			// later to translate and rotate the ring accordingly
			head_atom->setProperty("SDGenerator::PFU_x_pos", head_atom->getPosition().x);
			head_atom->setProperty("SDGenerator::PFU_y_pos", head_atom->getPosition().y);
		}
		head_atom->setPosition(Vector3(0.f));

		// and put it into the queue
		redraw_queue_.push(head_atom);

		head_atom->setProperty(SDGenerator::ASSEMBLED);

		// now place all the atoms
		while (!redraw_queue_.empty())
		{
			Atom* seed_atom = redraw_queue_.top();
			redraw_queue_.pop();

			Angle beta = computeAngularSeparation_(seed_atom);
			std::vector<Atom*> sequence = sequenceSubstituents_(seed_atom);

			// select the placement algorithm
			if (seed_atom->getProperty("InRing").getBool())
			{
				// iterate over the unplaced substituents in the correct order
				for (Position next_neighbour_index = 0; next_neighbour_index < sequence.size(); ++next_neighbour_index)
				{
					Atom* next_neighbour = sequence[next_neighbour_index];

					if (!(next_neighbour->hasProperty(SDGenerator::ASSEMBLED)))
					{
						// rotate the CFS_lo of the seed by beta
						if (seed_atom != head_atom || (seed_atom->hasProperty(SDGenerator::INITIALIZED_HEAD_CFS)))
							setCFS_(seed_atom, getCFS_(seed_atom, false) + beta, false);

						placeSubstituent_(seed_atom, head_atom, next_neighbour);

						redraw_queue_.push(next_neighbour);

						// if the bond was part of a PFU, deposit it straight away
						if (next_neighbour->getProperty("InRing").getBool())
							depositPFU_(seed_atom, next_neighbour);

						// finally, check for overlap (but don't fix it yet)
						checkOverlap_(next_neighbour);
					}
				}
			}
			else if (seed_atom->hasProperty(SDGenerator::CORE_CHAIN))
			{
				// find the correct chain
				std::list<Atom*> chain;

				for (std::list<std::list<Atom*> >::iterator l_it = chains_.begin(); l_it != chains_.end(); ++l_it)
				{
					if (std::find(l_it->begin(), l_it->end(), seed_atom) != l_it->end())
					{
						chain = *l_it;
						break;
					}
				}

				// iterate over the unplaced substituents in the correct order
				for (Position next_neighbour_index = 0; next_neighbour_index < sequence.size(); ++next_neighbour_index)
				{
					Atom* next_neighbour = sequence[next_neighbour_index];

					if (!(next_neighbour->hasProperty(SDGenerator::ASSEMBLED)))
					{
						// is the next atom part of the same chain?
						std::list<Atom*>::iterator atom_in_chain = std::find(chain.begin(), chain.end(), next_neighbour);
						if (atom_in_chain != chain.end())
						{
							if (seed_atom->hasProperty(ZIG))
							{
								setCFS_(seed_atom, Angle(getCFS_(seed_atom, false).toRadian() + 4./3.*M_PI, true), false);
							}
							else
							{
								setCFS_(seed_atom, Angle(getCFS_(seed_atom, false).toRadian() + 2./3.*M_PI, true), false);
							}
						}
						else
						{
							if (seed_atom->hasProperty(ZIG))
							{
								setCFS_(seed_atom, Angle(getCFS_(seed_atom, false).toRadian() + 2 * beta, true), false);
							} else {
								setCFS_(seed_atom, Angle(getCFS_(seed_atom, false) + beta, true), false);
							}
						}

						placeSubstituent_(seed_atom, head_atom, next_neighbour);

						redraw_queue_.push(next_neighbour);

						// TODO: if the seed atom is in a completed double bond, ensure stereochemistry
					}
				}
			}
			else // it is neither in a ring nor in a core chain
			{
				Size num_double_bonds = 0;
				Size num_triple_bonds = 0;

				Atom::BondIterator b_it;
				BALL_FOREACH_ATOM_BOND(*seed_atom, b_it)
				{
					if (b_it->getOrder() == Bond::ORDER__DOUBLE)
						++num_double_bonds;
					else if (b_it->getOrder() == Bond::ORDER__TRIPLE)
						++num_triple_bonds;
				}

				if (    (seed_atom->countBonds() == 2)
				     && (num_double_bonds <= 2)
						 && (num_triple_bonds == 0) )
				{
					// the atom is pseudotrigonal
					beta = Angle(2./3.*M_PI, true);
				}

				// iterate over the unplaced substituents in the correct order
				for (Position next_neighbour_index = 0; next_neighbour_index < sequence.size(); ++next_neighbour_index)
				{
					Atom* next_neighbour = sequence[next_neighbour_index];

					if (!(next_neighbour->hasProperty(SDGenerator::ASSEMBLED)))
					{
						if ((seed_atom != head_atom) || seed_atom->hasProperty(SDGenerator::INITIALIZED_HEAD_CFS))
						{
							// TODO: step (a), step (b)
							setCFS_(seed_atom, getCFS_(seed_atom, false) + beta, false);
						}

						placeSubstituent_(seed_atom, head_atom, next_neighbour);

						redraw_queue_.push(next_neighbour);

						// TODO: ensure stereo chemistry
					}
				}
			}
		}
	}

	void SDGenerator::setDefaultOptions()
	{		
	 	options.setDefaultBool(Option::SHOW_HYDROGENS, 
		                       Default::SHOW_HYDROGENS);

	 	options.setDefaultReal(Option::STANDARD_BOND_LENGTH, 
		                        Default::STANDARD_BOND_LENGTH);
	}
} // namespace BALL
