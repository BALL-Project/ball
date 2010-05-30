// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/rsConstructor.h>

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/MATHS/matrix44.h>

#ifdef BALL_DEBUG_RSCONSTRUCTOR
# define DEBUG(a) Log.info() << a << std::endl;
#else
# define DEBUG(a) 
#endif

namespace BALL
{
	RSConstructor::RSConstructor()
	{}


	RSConstructor::~RSConstructor()
	{}


	void RSConstructor::buildRegularPolygon(vector<Atom*>& ring, Size& z)
	{
		double bl = 2.0;                                                //      setting the standard bond-length

		TAngle<double> phi;

		//      set the angle phi to 360/n
		const double temp = (360.0 / ring.size());
		phi.set(temp, false);

		//      declare rotation matrix
		TMatrix4x4<double> rot_matr;

		//      set bond-vector to bond-length and x-direction
		TVector4<double> bond_vec;
		bond_vec.set(bl, 0.0, 0.0, 1.0);

		//      position the first ring-atom
		Vector3 pos_vec1;

		pos_vec1[0] = (float)z;
		pos_vec1[1] = 0.0;
		pos_vec1[2] = 0.0;

		ring[0] -> setPosition(pos_vec1);
		ring[0] -> setProperty(SDGenerator::DEPOSITED);


		//      position the second ring-atom
		Vector3 pos_vec2;

		pos_vec2[0] = pos_vec1[0] + bond_vec[0];
		pos_vec2[1] = pos_vec1[1] + bond_vec[1];
		pos_vec2[2] = 0.0;

		ring[1] -> setPosition(pos_vec2);
		ring[1] -> setProperty(SDGenerator::DEPOSITED);


		//      position every other atom consecutively
		for (vector<Atom*>::size_type i = 2; i != ring.size(); i++)
		{
			//      rotate the bond-vector by the angle phi
			rot_matr.setRotationZ(phi);

			bond_vec = rot_matr * bond_vec;

			//      position the current atom relative to the previously positioned atom
			Vector3 pos_vec3;

			pos_vec3[0] = ring[i-1] -> getPosition()[0] + bond_vec[0];
			pos_vec3[1] = ring[i-1] -> getPosition()[1] + bond_vec[1];
			pos_vec3[2] = 0.0;

			ring[i] -> setPosition(pos_vec3);
			ring[i] -> setProperty(SDGenerator::DEPOSITED);
		}

		DEBUG("\t-*-[RSConstructor]:\t (buildRegularPolygon):\t done." << endl)
	}


	//      check, whether an atom has been positioned inside the enclosed area of another ring
	bool RSConstructor::inside(Atom*& test_a, vector<Atom*>& ring)
	{

		//      get the coordinates of the vertexes of the tested ring's embracing square
		float x_max = 0.0;
		float x_min = 1000.0;

		float y_max = 0.0;
		float y_min = 1000.0;

		for (Size i = 0; i != ring.size(); i++)
		{
			if (ring[i] -> getPosition()[0] < x_min)
			{
				x_min = ring[i] -> getPosition()[0];
			}

			if (ring[i] -> getPosition()[0] > x_max)
			{
				x_max = ring[i] -> getPosition()[0];
			}

			if (ring[i] -> getPosition()[1] < y_min)
			{
				y_min = ring[i] -> getPosition()[1];
			}

			if (ring[i] -> getPosition()[1] > y_max)
			{
				y_max = ring[i] -> getPosition()[1];
			}
		}

		//      check, whether the atom is positioned inside the embracing square
		float test_a_x = test_a -> getPosition()[0];
		float test_a_y = test_a -> getPosition()[1];

		if (test_a_x > x_min && test_a_x < x_max && test_a_y > y_min && test_a_y < y_max)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}


	//      attach a fused-ring to a (partially) prefabricated ringsystem
	void RSConstructor::attachFused(RingInfo& ring_info, vector<RingInfo>& ringsystem)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachFused):\t\t started.")

		//      the fused ring that is to be attached
		vector<Atom*> ring = ring_info.ring;

		//      flags indicating, whether the shared atoms have been found
		bool first_found = false;
		bool second_found = false;

		//      variables to save the shared atoms' indices
		vector<Atom*>::size_type q_first = 0;
		vector<Atom*>::size_type q_second = 0;

		Size keep_k = 0;

		//      find the two atoms that are shared by the ring, that is to be constructed an another ring of the ring system
		bool breaker = 0;

		for (vector<RingInfo>::size_type k = 0; k != ringsystem.size(); k++)
		{
			//      the other ring must already be constructed (deposited) and may not be the ring that is to be constructed itself
			if ((ringsystem[k].ring != ring) && ringsystem[k].ring[1] -> hasProperty(SDGenerator::DEPOSITED) && ringsystem[k].ring[2] -> hasProperty(SDGenerator::DEPOSITED) && ringsystem[k].ring[3] -> hasProperty(SDGenerator::DEPOSITED))
			{

				for (vector<Atom*>::size_type j = 0; j != ringsystem[k].ring.size(); j++)
				{
					//      find the shared bond first
					for (Atom::BondIterator bond_it2 = ringsystem[k].ring[j] -> beginBond(); bond_it2 != ringsystem[k].ring[j] -> endBond(); bond_it2++)
					{
						//      only check bonds that are part of the ring
						RingAnalyser ra;
						if (ra.isInRing(bond_it2 -> getFirstAtom(), ringsystem[k].ring) && ra.isInRing(bond_it2 -> getSecondAtom(), ringsystem[k].ring))
						{
							for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
							{
								for (Atom::BondIterator bond_it = ring[i] -> beginBond(); bond_it != ring[i] -> endBond(); bond_it++)
								{
									if (ra.isInRing(bond_it -> getFirstAtom(), ring) && ra.isInRing(bond_it -> getSecondAtom(), ring) )
									{
										keep_k = k;
										//      when the shared bond is found
										if (*bond_it == *bond_it2)
										{
											//      get the two atoms at the ends of the bond
											for (vector<Atom*>::size_type q = 0; q != ring.size(); q++)
											{
												if ((*(ring[q]) ==  *(bond_it2 -> getFirstAtom()) || *(ring[q]) ==  *(bond_it2 -> getSecondAtom())) && !(first_found))
												{
													q_first = q;                    //      the first shared atom is found
													first_found = true;
												}
												else if ((*(ring[q]) ==  *(bond_it2 -> getFirstAtom()) && first_found && !(second_found)) || (*(ring[q]) ==  *(bond_it2 -> getSecondAtom()) && first_found && !(second_found)))
												{
													q_second = q;                   //      the second shared atom is found
													second_found = true;

													breaker = 1;
													break;
												}
												if (breaker)
												{
													break;
												}
											}
											if (breaker)
											{
												break;
											}
										}
										if (breaker)
										{
											break;
										}
									}
									if (breaker)
									{
										break;
									}
								}
								if (breaker)
								{
									break;
								}
							}
							if (breaker)
							{
								break;
							}
						}
						if (breaker)
						{
							break;
						}
					}
					if (breaker)
					{
						break;
					}
				}
				if (breaker)
				{
					break;
				}
			}
			if (breaker)
			{
				break;
			}
		}

		//      if both shared atoms have been found
		if (first_found && second_found)
		{
			//      find the bond in the old ring that will be taken the correct direction of the bonds in the new ring, i.e. the bond between the second shared atom and the next atom in the old ring
			//      (a reference-bond)

			//      this will hold the next atom in the old ring
			Atom* ref_atom = new Atom;

			RingAnalyser ra;

			//      check all bonds of second shared atom
			for (Atom ::BondIterator bond_it = ring[q_second] -> beginBond(); bond_it != ring[q_second] -> endBond(); ++bond_it)
			{
				//      choose the one that is part of the ring, but does not connect the first shared atom
				if (bond_it -> getPartner(*ring[q_second]) != ring[q_first] && ra.isInRing(bond_it -> getPartner(*ring[q_second]),ringsystem[keep_k].ring))
				{
					ref_atom = bond_it -> getPartner(*ring[q_second]);
				}
			}

			//      create a vector representing the refernce-bond
			TVector3<double> ref_vec;
			ref_vec[0] = (ref_atom -> getPosition()[0] - ring[q_second] -> getPosition()[0]);
			ref_vec[1] = (ref_atom -> getPosition()[1] - ring[q_second] -> getPosition()[1]);
			ref_vec[2] = (ref_atom -> getPosition()[2] - ring[q_second] -> getPosition()[2]);



			//      set angle phi to 360/n
			TAngle<double> phi;
			double temp = (360.0 / ring.size());
			phi.set(temp, false);

			// declare rotation matrix
			TMatrix4x4<double> rot_matr;

			//      set bond vector to the vector between the shared atoms
			TVector4<double> bond_vec;

			TVector3<double> temp_vec;
			temp_vec[0] = (ring[q_second] -> getPosition()[0] - ring[q_first] -> getPosition()[0]);
			temp_vec[1] = (ring[q_second] -> getPosition()[1] - ring[q_first] -> getPosition()[1]);
			temp_vec[2] = (ring[q_second] -> getPosition()[2] - ring[q_first] -> getPosition()[2]);

			bond_vec.set(temp_vec[0], temp_vec[1], 0.0, 1.0);


			//      check, whether the bonds will be rotated in the correct direction
			//      therefore create a copy of the first bon-vector
			TVector3<double> temp_bond;
			temp_bond.set(bond_vec[0], bond_vec[1], bond_vec[2]);

			//      rotate it in the way the first bond shall be rotated
			rot_matr.setRotationZ(phi);
			temp_bond = rot_matr * temp_bond;

			//      get the angle between the rotated copy of the bond-vector and the reference-bond
			float angle = temp_bond.getAngle(ref_vec)/(2 * Constants::PI)*360.0;

			//      indicate, whether the rotation angle must be inverted
			ring_info.neg_angle = 0;

			//      if the angle between the rotated copy of the bond-vector and the reference-bond is < 90°, the rotation angle must be inverted
			if (angle < 90.0 || ( angle == 0))
			{
				phi.set(-temp, false);
				ring_info.neg_angle = 1;
			}

			//      set the border to finish the construction-loop
			vector<Atom*>::size_type border;

			if (q_second + 1 == ring.size())
			{
				border = 0;
			}
			else
			{
				border = q_second + 1;
			}

			//      if the border - atom has not been positioned before, run the positioning loop forward
			if (!(ring[border] -> hasProperty(SDGenerator::DEPOSITED)))
			{
				for (vector<Atom*>::size_type i = border; i != q_first; i++)
				{
					//      continue with the first atom, after the last one in the ring-vector was reached
					if (i == ring.size())
					{
						i = 0;
					}

					//      stop-condition
					if (i == q_first)
					{
						break;
					}

					//      rotate the bond-vector by the angle phi
					rot_matr.setRotationZ(phi);

					bond_vec = rot_matr * bond_vec;

					//      position the atom relative to the previously positioned atom
					Vector3 pos_vec3;

					if (i == 0)
					{
						pos_vec3[0] = ring[ring.size() - 1] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[ring.size() - 1] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}

					else
					{
						pos_vec3[0] = ring[i - 1] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[i - 1] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}

					//      declare atom as "deposited"
					if (!(ring[i] -> hasProperty(SDGenerator::DEPOSITED)))
					{
						ring[i] -> setPosition(pos_vec3);
					}
					ring[i] -> setProperty(SDGenerator::DEPOSITED);

				}
			}
			//      if the border - atom has been positioned before,  run the positioning loop backward
			else
			{
				//      the bond vector must have the inverse direction
				bond_vec[0] = (ring[q_second] -> getPosition()[0] - ring[q_first] -> getPosition()[0]);
				bond_vec[1] = (ring[q_second] -> getPosition()[1] - ring[q_first] -> getPosition()[1]);
				bond_vec[2] = 0.0;
				bond_vec[3] = 1.0;


				double temp = ((360.0 / ring.size()));
				phi.set(temp, false);
				if (ring_info.neg_angle)
				{
					phi.set(-temp, false);
				}


				for (vector<Atom*>::size_type i = q_second - 1; i != border; i--)
				{
					if (i <= 0)
					{
						i = ring.size()-1;
					}

					//      stop-condition
					if (i == border)
					{
						break;
					}

					//      rotate the bond-vector by the angle phi
					rot_matr.setRotationZ(phi);
					bond_vec = rot_matr * bond_vec;

					//      position the atom relative to the previous positioned atom
					Vector3 pos_vec3;
					if (i == ring.size() - 1 )
					{
						pos_vec3[0] = ring[0] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[0] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}

					else
					{
						pos_vec3[0] = ring[i + 1] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[i + 1] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}

					//      declare atom as "deposited"
					if (!(ring[i] -> hasProperty(SDGenerator::DEPOSITED)))
					{
						ring[i] -> setPosition(pos_vec3);
						//ring[i] -> setProperty(deposited);
					}
					ring[i] -> setProperty(SDGenerator::DEPOSITED);

				}
			}

			DEBUG("\t-*-[RSConstructor]:\t (attachFused):\t done.")
		}
		//      if not both of the shared atoms have been found, append the cuurent ring at the end of the ringsystem and construct it later
		else
		{
			DEBUG("\t-*-[RSConstructor]:\t (attachFused):\t aborted. Ring appended.")
			ringsystem.push_back(ring_info);
		}
	}



	void RSConstructor::attachBridged(vector<Atom*>& ring, vector<RingInfo>& ringsystem)
	{
		//      construct a BRIDGED ring by the "open polygon" - method
		DEBUG("\t-*-[RSConstructor]:\t (attachBridged):\t started.")


		//              finding the brigdhead atoms, that is the atoms, that are part of the ring, that is to be constructed and also part of any other ring in the ringsystem

		//      flag, indicating whether the first bridgehead-atom has been found
		bool first_found = false;
		bool different_rings = false;

		vector<Atom*>::size_type keep_j = 0;

		Size q_first = 0;
		Size q_second = 0;
		vector<Size> q_s;

		double temp_dist = 0.0;

		for (vector<vector<Atom*> >::size_type j = 0; j != ringsystem.size(); j++)
		{
			//      the other ring must already be constructed (deposited) and may not be the ring that is to be constructed itself
			if ((ringsystem[j].ring != ring) && (ringsystem[j].ring[0] -> hasProperty(SDGenerator::DEPOSITED)))
			{
				for (vector<Atom*>::size_type k = 0; k != ring.size(); k++)
				{
					for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
					{
						//      if an atom is part of the ring, that is to be constructed and also part of another ring in the ringsystem and the first bridgehead atom has not been found yet
						if ((ringsystem[j].ring[k] == ring[i])/* && (!(first_found)) */)
						{
							if (!first_found)
							{
								keep_j = j;
								first_found = true;
							}
							else
							{
								//      check, whether two bridgehead-atoms are situated in different rings
								if (j != keep_j)
								{
									different_rings = true;
								}
							}
							//      collect all shared atoms in a vector
							q_s.push_back(i);
						}
					}
				}
			}
		}


		//      search for the pair of shared atoms that have got the most atoms in between
		for (Size i = 0; i != q_s.size(); i++)
		{
			for (Size j = 0; j != q_s.size(); j++)
			{
				Size a = q_s[i] - q_s[j];

				if (sqrt(pow(a, 2.0)) > temp_dist && sqrt(pow(a, 2.0)) < (ring.size() - 1))
				{
					if (ring[q_s[i]+1] -> hasProperty(SDGenerator::DEPOSITED))
					{

						q_first = q_s[j];
						q_second = q_s[i];
					}
					else
					{
						q_first = q_s[i];
						q_second = q_s[j];
					}
					temp_dist = sqrt(pow(a, 2.0));
				}
			}
		}

		TAngle<double> alpha;
		TAngle<double> beta;

		//      set alpha to 360/n
		double temp = (360.0 / ring.size());
		alpha.set(temp, false);

		//      get the vector between the brigde-head-atoms to start the construction
		TVector3<float> temp_vec = (ring[q_first] -> getPosition() - ring[q_second] -> getPosition());
		if (ring[q_first + 1] -> hasProperty(SDGenerator::DEPOSITED))
		{
			TVector3<float> temp_vec = (ring[q_first] -> getPosition() - ring[q_second] -> getPosition());
		}

		//      set the vector's length to 2/3 of the distance between the bridgehead-atoms (not very sophisticated)
		temp_vec *= (0.6);

		//      declare a rotation matrix
		TMatrix4x4<double> rot_matr;

		//      set an initial distance for the binary search to find the best constellation of alpha and beta
		double distance = 100.0;
		double old_distance = 100.0;

		//      faktor to scale down the steps in the binary search
		double bisector = 20.0;

		//      flag, indicating the direction in which the search is runnuing
		bool descending = true;

		//      initial aspired distance between the second bridgehead-atom and the last atom to be positioned (which is positioned only virtually)
		double fin_dist = 0.01;

		double min_dist = 100.0;
		//      loop for the binary search
		while(!(distance < fin_dist))
		{
			//      if the faktor that scales down the search-steps becomes smaller than 0.0001, increase the aspired distance
			if (bisector <= 0.001)
			{
				fin_dist += 0.05;
				bisector = 5.0;
			}

			//      set the bond-vector's x- and y-coordinates to those of the vector between the brigehead-atoms
			TVector4<double> bond_vec;
			bond_vec.set(static_cast<double>(temp_vec[0]), static_cast<double>(temp_vec[1]), 0.0, 1.0);

			//      set alpha
			alpha.set(temp, false);

			//      set beta correspondng to the formula on page 338, [HELSON]

			double temp2 = ((((ring.size() - 2)*(180.0-alpha))/2.0) +180.0);
			if (different_rings)
			{
				temp2 = ((((ring.size() - 2)*(180.0-alpha))/2.0) + 90.0);
			}
			beta.set(temp2, false);

			Vector3 pos_vec;

			//      start the construction from the next atom in the ring after the first bridgehead-atom
			for (vector<Atom*>::size_type i = (q_first + 1); i != q_second + 1 ; i++)
			{

				//      if the last atom is reached, continue with the first one in the ring-vector
				if (i == ring.size())
				{
					i = 0;
				}

				//      set beta as the angle for the bond between bridgehead-atom and and next atom
				if (i == q_first+1)
				{
					rot_matr.setRotationZ(beta);
				}
				//      set alpha as the angle for every other bond
				else
				{
					rot_matr.setRotationZ(alpha);
				}

				bond_vec = rot_matr * bond_vec;

				//      set the coordinates relative to those of the last positioned atom
				if (i != 0)
				{
					pos_vec[0] = ring[i-1] -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = ring[i-1] -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}
				else
				{
					pos_vec[0] = ring[ring.size()-1] -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = ring[ring.size()-1] -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}


				if (i != q_second)
				{
					ring[i] -> setPosition(pos_vec);
					ring[i] -> setProperty(SDGenerator::DEPOSITED);
				}

				//      binary search
				else
				{

					Vector3 dist_vec = (pos_vec - ring[q_second] -> getPosition());
					//      calculate the distance between the (virtual) last atom and the real second bridge-head
					distance = sqrt(pow(dist_vec[0], 2) + pow(dist_vec[1], 2));

					if (distance < min_dist)
					{
						min_dist = distance;
					}
					//      if the distance has become smalller
					if (!(distance > old_distance))
					{
						// update the distance
						old_distance = distance;

						//      keep the direction of search
						if (descending)
						{
							temp -= bisector;
							descending = true;
						}
						else if (!(descending))
						{
							temp += bisector;
							descending = false;
						}
					}
					//      if the distance has not become smaller
					else
					{
						// update the distance
						old_distance = distance;

						//      change the direction of search, bisect the scaling factor and update the angles
						if (descending)
						{
							bisector = bisector*0.5;
							temp += bisector;
							descending = false;
						}
						else if (!(descending))
						{
							bisector = bisector*0.5;
							temp -= bisector;
							descending = true;
						}
					}
				}
			}
		}
		//      if the bridged ring bridges over two different rings, check for possible overlaps
		if (different_rings)
		{
			DEBUG("checking for overlaps...")

			//      flag to indicate an overlap
			bool overlap = false;

			//      limit the checking to the newly positioned atoms
			Size start;
			Size stop;

			if (q_first == ring.size())
			{
				start = 0;
			}
			else
			{
				start = q_first + 1;
			}

			if (q_second == 0)
			{
				stop = ring.size();
			}
			else
			{
				stop = q_second;
			}

			//      for each newly positioned atom, check whether it has been positioned inside another ring of the ringsystem
			RSConstructor rsc;
			for (vector<Atom*>::size_type i = start ; i != stop  ; i++)
			{
				//      if the last atom is reached, continue with the first one in the ring-vector
				if (i == ring.size())
				{
					i = 0;
				}

				//      do not check the bridgehead-atoms
				if (i != q_second && i != q_first)
				{
					for (Size j = 0; j != ringsystem.size(); j++)
					{
						// do not check rings, that have not been positioned yet
						bool undepos_ring = false;
						for (Size k = 0; k != ringsystem[j].ring.size(); k++)
						{
							if (!(ringsystem[j].ring[k] -> hasProperty(SDGenerator::DEPOSITED)))
							{
								undepos_ring = true;
							}
						}
						if ((!undepos_ring) && ringsystem[j].ring != ring)
						{
							//      check for overlap with a readily positiones ring
							if (rsc.inside(ring[i], ringsystem[j].ring))
							{
								overlap = true;
							}
						}
					}

				}

			}

			//      if overlap has occured, re-position the bridged ring on the other side of the bridhehead-atoms
			if (overlap)
			{

				double temp = -(360.0 / ring.size());
				//      set an initial distance for the binary search to find the best constellation of alpha and beta
				double distance = 100.0;
				double old_distance = 100.0;

				//      faktor to scale down the steps in the binary search
				double bisector = 20.0;

				//      flag, indicating the direction in which the search is runnuing
				bool descending = true;

				//      initial aspired distance between the seconnd bridgehead-atom and the last atom to be positioned (which is positiones only virtually)
				double fin_dist = 0.01;

				double min_dist = 100.0;

				//      loop for the binary search
				while(!(distance < fin_dist))
				{
					//      if the faktor that scales down the search-steps becomes smaller than 0.0001, increase the aspired distance
					if (bisector <= 0.001)
					{
						fin_dist += 0.05;
						bisector = 5.0;
					}

					//      set the bond-vector's x- and y-coordinates to those of the vector between the brigehead-atoms
					TVector4<double> bond_vec;
					bond_vec.set(static_cast<double>(temp_vec[0]), static_cast<double>(temp_vec[1]), 0.0, 1.0);

					//      set alpha
					alpha.set(temp, false);

					//      set beta correspondng to the formula on page 338

					double temp2 = ((((ring.size() - 2)*(180.0-alpha))/2.0) + 180.0);
					if (different_rings)
					{
						temp2 = ((((ring.size() - 2)*(180.0-alpha))/2.0) - 90.0);
					}
					beta.set(temp2, false);

					Vector3 pos_vec;


					//      start the construction from the next atom in the ring after the first bridgehead-atom
					for (vector<Atom*>::size_type i = (q_first + 1); i != q_second + 1 ; i++)
					{
						//      if the last atom is reached, continue with the first one in the ring-vector
						if (i == ring.size())
						{

							i = 0;
						}

						//      set beta as the angle for the bond between bridgehead-atom and and next atom
						if (i == q_first+1)
						{
							rot_matr.setRotationZ(beta);
						}
						//      set alpha as the angle for every other bond
						else
						{
							rot_matr.setRotationZ(-alpha);
						}

						bond_vec = rot_matr * bond_vec;

						//      set the coordinates relative to those of the last positioned atom
						if (i != 0)
						{
							pos_vec[0] = ring[i-1] -> getPosition()[0] + bond_vec[0];
							pos_vec[1] = ring[i-1] -> getPosition()[1] + bond_vec[1];
							pos_vec[2] = 0.0;
						}
						else
						{
							pos_vec[0] = ring[ring.size()-1] -> getPosition()[0] + bond_vec[0];
							pos_vec[1] = ring[ring.size()-1] -> getPosition()[1] + bond_vec[1];
							pos_vec[2] = 0.0;
						}

						if (i != q_second)
						{
							ring[i] -> setPosition(pos_vec);

							ring[i] -> setProperty(SDGenerator::DEPOSITED);
						}

						//      binary search
						else
						{

							//      virtual positioning of the last atom (which is the same as the second bridgehead)

							Vector3 dist_vec = (pos_vec - ring[q_second] -> getPosition());
							//      calculate the distance between the (virtual) last atom and the real second bridge-head
							distance = sqrt(pow(dist_vec[0], 2) + pow(dist_vec[1], 2));

							if (distance < min_dist)
							{
								min_dist = distance;
							}
							//      if the distance has become smalller
							if (!(distance > old_distance))
							{
								// update the distance
								old_distance = distance;

								//      keep the direction of search
								if (descending)
								{
									temp -= bisector;
									descending = true;
								}
								else if (!(descending))
								{
									temp += bisector;
									descending = false;
								}
							}

							else
							{
								// update the distance
								old_distance = distance;

								//      change the direction of search, bisect the scaling factor and update the angles
								if (descending)
								{
									bisector = bisector*0.5;
									temp += bisector;
									descending = false;
								}
								else if (!(descending))
								{
									bisector = bisector*0.5;
									temp -= bisector;
									descending = true;
								}
							}
						}
					}
				}
			}
		}

		DEBUG("\t-*-[RSConstructor]:\t (attachBridged):\t done.")
	}



	void RSConstructor::attachSpiro(RingInfo& ring_info, vector<RingInfo>& ringsystem)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachSpiro):\t\t started.")

		vector<Atom*> ring = ring_info.ring;

		// finding the shared atom

		vector<Atom*>::size_type keep_i = 0;
		vector<Atom*>::size_type init_poSize1 = 0;
		vector<Atom*>::size_type init_poSize2 = 0;
		vector<vector<Atom*> >::size_type keep_j = 0;

		// check for the atom that is part of the ring, that is to be constructed and part of another ring

		bool spiro_found = 0;

		for (vector<Atom*>::size_type i = 0; i != ring.size(); i++)
		{
			for (vector<vector<Atom*> >::size_type j = 0; j != ringsystem.size(); j++)
			{
				//      the other ring must already be constructed (deposited) and may not be the ring that is to be constructed itself
				if ((ringsystem[j].ring != ring) && (ringsystem[j].ring[0] -> hasProperty(SDGenerator::DEPOSITED)))
				{
					for (vector<Atom*>::size_type k = 0; k != ring.size(); k++)
					{
						// when the atom is found, save its index and the index of the ring that shares it
						if (ringsystem[j].ring[k] == ring[i])
						{
							spiro_found = 1;
							keep_i = i;             //      index in the ring that is to be constructed
							keep_j = j;             //      index of the other ring in the ringsystem-vector


							if (k != ringsystem[j].ring.size())
							{
								init_poSize2 = k + 1;   //      get the index of the next atom in the constucted ring
							}
							else
							{
								init_poSize2 = 0;                       // set it to 0, if the shared atom is the last one in the vector
							}

							init_poSize1 = k;                               // get the index of the shared-atom in the constructed ring
						}
					}
				}
			}
		}

		if (spiro_found)
		{
			//      calculate the direction and length of the bond between the shared atom and the next atom in the constructed ring (initial vector)
			Vector3 init_vec = (ringsystem[keep_j].ring[init_poSize1] -> getPosition() - ringsystem[keep_j].ring[init_poSize2] -> getPosition());

			//      set the angle of ring-bonds relative to its size
			TAngle<double> phi;
			TAngle<double> phi_first;

			const double temp = (360.0 / ring.size());

			double other_ring_angle = (360.0 / ringsystem[keep_j].ring.size());



			phi.set(temp, false);

			//      if the positioning-angle of the readily positioned ring has been inverted previously, the angle must be inverted now aswell
			if (ringsystem[keep_j].neg_angle)
			{
				phi.set(-temp, false);
			}

			//      set the angle for the first bond to the difference of the positioned ring's ring-angle and the new spiro-ring's ring-angle
			double temp_first = temp - other_ring_angle;
			phi_first.set(temp_first, false);

			//      if the positioning-angle of the readily positioned ring has been inverted previously, the angle must be inverted now aswell
			if (ringsystem[keep_j].neg_angle)
			{
				phi_first.set(-temp_first, false);
			}


			//      declaration of a rotation matrx
			TMatrix4x4<double> rot_matr;

			//      declaration of the bond-vector
			TVector4<double> bond_vec;

			//      set the x- and y-coordinates of the bond-vector the hose of the initial vector
			bond_vec.set(init_vec[0],init_vec[1], 0.0, 1.0);

			TVector4<double> test_vec;
			test_vec[0] = bond_vec[0];
			test_vec[1] = bond_vec[1];
			test_vec[2] = bond_vec[2];
			test_vec[3] = bond_vec[3];

			rot_matr.setRotationZ(phi);
			test_vec = rot_matr * test_vec;

			Vector3 test_vec_3;
			test_vec_3[0] = test_vec[0];
			test_vec_3[1] = test_vec[1];
			test_vec_3[2] = test_vec[2];


			//      start the construction at the next index after that of the shared atom
			vector<Atom*>::size_type i = keep_i;


			vector<Atom*>::size_type bound1;
			vector<Atom*>::size_type bound2;

			//      start at the first atom, if the shared one was the last one in the ring-vector
			if (i == ring.size())
			{
				bound1 = 0;
			}
			else
			{
				bound1 = i+1;
			}

			//      finish the construction at the last index before that of the shared atom

			//      finish at the last atom, if the shared one was the first one in the ring-vector
			if (i == 0)
			{
				bound2 = ring.size();
			}
			else
			{
				bound2 = i;
			}


			for (i = bound1; i != bound2; i++)
			{

				//      continue with the first atom, after the last one in the ring-vector was reached
				if (i == ring.size())
				{
					i = 0;
				}

				//      in each pass, except the first one rotate the bond-vector by the angle phi
				if (i != bound1)
				{
					rot_matr.setRotationZ(phi);
					bond_vec = rot_matr * bond_vec;
				}
				else
				{
					rot_matr.setRotationZ(phi_first);
					bond_vec = rot_matr * bond_vec;
				}


				//      set the coordinates for current atom relative to those of the last positioned atom
				Vector3 pos_vec3;
				if (i > 0)
				{
					pos_vec3[0] = ring[i-1] -> getPosition()[0] + bond_vec[0];
					pos_vec3[1] = ring[i-1] -> getPosition()[1] + bond_vec[1];
					pos_vec3[2] = 0.0;
				}
				else
				{
					pos_vec3[0] = ring[ring.size()-1] -> getPosition()[0] + bond_vec[0];
					pos_vec3[1] = ring[ring.size()-1] -> getPosition()[1] + bond_vec[1];
					pos_vec3[2] = 0.0;
				}


				ring[i] -> setPosition(pos_vec3);

				//      declare the current atom as deposited
				ring[i] -> setProperty(SDGenerator::DEPOSITED);

				if (i == bound2)
				{
					break;
				}
			}

			DEBUG("checking for overlaps...")

			bool overlap = false;

			RSConstructor rsc;
			for (vector<Atom*>::size_type i = (keep_i + 1 ); i != keep_i ; i++)
			{
				//      if the last atom is reached, continue with the first one in the ring-vector
				if (i == ring.size())
				{

					i = 0;
				}

				if (i != keep_i)
				{



					for (Size j = 0; j != ringsystem.size(); j++)
					{
						bool undepos_ring = false;
						for (Size k = 0; k != ringsystem[j].ring.size(); k++)
						{
							if (!(ringsystem[j].ring[k] -> hasProperty(SDGenerator::DEPOSITED)))
							{
								undepos_ring = true;
							}
						}
						if ((!undepos_ring) && ringsystem[j].ring != ring)
						{
							if (rsc.inside(ring[i], ringsystem[j].ring))
							{
								overlap = true;

							}
						}
					}

				}

			}

			if (overlap)
			{


				phi.set(-temp, false);

				if (ringsystem[keep_j].neg_angle)
				{

					phi.set(temp, false);
				}


				//      declaration of a rotation matrx
				TMatrix4x4<double> rot_matr;

				//      declaration of the bond-vector
				TVector4<double> bond_vec;

				//      set the x- and y-coordinates of the bond-vector the hose of the initial vector
				bond_vec.set(init_vec[0],init_vec[1], 0.0, 1.0);


				//      start the construction at the next index after that of the shared atom
				vector<Atom*>::size_type i = keep_i;


				vector<Atom*>::size_type bound1;
				vector<Atom*>::size_type bound2;

				//      start at the first atom, if the shared one was the last one in the ring-vector
				if (i == ring.size())
				{
					bound1 = 0;
				}
				else
				{
					bound1 = i+1;
				}

				//      finish the construction at the last index before that of the shared atom

				//      finish at the last atom, if the shared one was the first one in the ring-vector
				if (i == 0)
				{
					bound2 = ring.size();
				}
				else
				{
					bound2 = i;
				}


				for (i = bound1; i != bound2; i++)
				{

					//      continue with the first atom, after the last one in the ring-vector was reached
					if (i == ring.size())
					{
						i = 0;
					}

					//      in each pass, except the first one rotate the bond-vector by the angle phi
					if (i != bound1)
					{
						rot_matr.setRotationZ(phi);
						bond_vec = rot_matr * bond_vec;
					}
					else
					{
						rot_matr.setRotationZ(-phi_first);
						bond_vec = rot_matr * bond_vec;
					}


					//      set the coordinates for current atom relative to those of the last positioned atom
					Vector3 pos_vec3;
					if (i > 0)
					{
						pos_vec3[0] = ring[i-1] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[i-1] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}
					else
					{
						pos_vec3[0] = ring[ring.size()-1] -> getPosition()[0] + bond_vec[0];
						pos_vec3[1] = ring[ring.size()-1] -> getPosition()[1] + bond_vec[1];
						pos_vec3[2] = 0.0;
					}


					ring[i] -> setPosition(pos_vec3);

					//      declare the current atom as deposited
					ring[i] -> setProperty(SDGenerator::DEPOSITED);

					if (i == bound2)
					{
						break;
					}
				}
			}
		}
		else
		{
			ringsystem.push_back(ring_info);
		}
	}


	//      interface for a Ring Template Database  // TODO: ???? was ist hiermit?
	//			if a RTD will be installed, this interface can be used to get information from it
	void RSConstructor::attachTemplate(vector<Atom*>& /* ring */)
	{
		DEBUG("\t-*-[RSConstructor]:\t (attachTemplate):\t started.")
	}

	void RSConstructor::constructRS(vector<RingInfo>& analysed_rings, Size& z)
	{
		// construct the CORE-Ring first
		for (vector<RingInfo>::size_type i = 0; i != analysed_rings.size(); i++)
		{
			if (analysed_rings[i].assignment == RingAnalyser::CORE)                          //assigment = CORE
			{
				buildRegularPolygon(analysed_rings[i].ring, z);
			}
		}

		// then check for TEMPLATE Rings and construct them if necessary
		for (vector<RingInfo>::size_type i = 0; i != analysed_rings.size(); i++)
		{
			if (analysed_rings[i].assignment == RingAnalyser::TEMPLATE)   //assignment = TEMPLATE
			{
				attachTemplate(analysed_rings[i].ring);
			}
		}

		// then check for SPIRO and FUSED Rings and construct them if necessary
		for (vector<RingInfo>::size_type i = 0; i != analysed_rings.size(); i++)
		{
			if (analysed_rings[i].assignment == RingAnalyser::SPIRO)           //assignment = SPIRO
			{
				attachSpiro(analysed_rings[i], analysed_rings);
			}

			if (analysed_rings[i].assignment == RingAnalyser::FUSED)           //assignment = FUSED
			{
				attachFused(analysed_rings[i], analysed_rings);
			}
		}
		// then check for BRIDGED, SPIRO and FUSED Rings and construct them if necessary
		for (vector<RingInfo>::size_type i = 0; i != analysed_rings.size(); i++)
		{

			if (analysed_rings[i].assignment == RingAnalyser::BRIDGED)           //assignment = BRIDGED
			{
				attachBridged(analysed_rings[i].ring, analysed_rings);
			}

			if (analysed_rings[i].assignment == RingAnalyser::SPIRO)           //assignment = SPIRO
			{
				attachSpiro(analysed_rings[i], analysed_rings);
			}

			if (analysed_rings[i].assignment == RingAnalyser::FUSED)           //assignment = FUSED
			{
				attachFused(analysed_rings[i], analysed_rings);
			}
		}
	}

} // namespace BALL
