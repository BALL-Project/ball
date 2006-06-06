#include <BALL/KERNEL/system.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/FORMAT/MOLFile.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/atomContainer.h>
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/SYSTEM/file.h>
#include <BALL/MOLMEC/COMMON/assignTypes.h>
#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#include <BALL/FORMAT/parameters.h>
#include <BALL/CONCEPT/property.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/MATHS/matrix44.h>

#include <string>
#include <vector>
#include <cmath>
#include <algorithm>

#include <BALL/STRUCTURE/sdGenerator.h>
#include <BALL/STRUCTURE/ringClusterer.h>
#include <BALL/STRUCTURE/ringAnalyser.h>
#include <BALL/STRUCTURE/rsConstructor.h>
#include <BALL/STRUCTURE/chainBuilder.h>
#include <BALL/STRUCTURE/moleculeAssembler.h>

namespace BALL
{


	MoleculeAssembler::MoleculeAssembler()
			:aq_()
	{}


	MoleculeAssembler::~MoleculeAssembler()
	{}

	//	compare-function, used to sort the ringsystems
	bool comparison(const vector<vector<Atom*> >& x, const vector<vector<Atom*> >& y)
	{
		return x.size() > y.size();
	}

	//	determine the free space around an atom, remaining to attach further substituents
	void MoleculeAssembler::updateCFS(Atom*& atom)
	{
		//	get the atoms's coordinates
		TVector3<float> atom_position = atom -> getPosition();

		//	create vectors that will border the free space and the angle between them
		TVector3<float> hi_vec;
		TVector3<float> lo_vec;
		TAngle<float> free_angle;

		//	variable that will temporarily hold the free angle
		float temp;

		//	flag, indicating, whether the first of the bonds bordering the free angle has been found
		bool first_found = false;

		for (Atom::BondIterator bond_it = atom -> beginBond(); bond_it != atom -> endBond(); bond_it++)
		{
			//	only check bonds to atoms, that already have been positioned
			Atom*	partner = bond_it -> getPartner(*atom);
			if (partner -> hasProperty(pre_assembled) )
			{
				//	find the first bond
				if ( !(first_found) )
				{
					//	get vector, representing the bond
					TVector3<float> partner_position = partner -> getPosition();
					hi_vec = (partner_position - atom_position);

					//	as long as the second bond is not found, lo_vec and hi_vec are equal
					lo_vec = hi_vec;

					//	in this case the free angle is 360° (the whole area around the atom is free)
					temp = 360.0;
					free_angle.set(temp, true);

					first_found = true;
				}
				//	when the first bond has already been found
				else
				{
					//	get vector, representing another bond
					Atom* partner = bond_it -> getPartner(*atom);
					TVector3<float> partner_position = partner -> getPosition();
					TVector3<float> curr_vec = (partner_position - atom_position);

					//	get the opposite angle of that included by the two bonds
					float temp2 = (free_angle - (hi_vec.getAngle(curr_vec)/(2 * Constants::PI)*359.9) );
					TAngle<float> new_angle;
					new_angle.set(temp2, true);

					//	if the free angle has decreased, accept the latest found bond
					if (new_angle < free_angle)
					{
						temp = temp2;
						free_angle = new_angle;
						lo_vec = curr_vec;
					}
				}
			}
		}

		//	set the atom's Properties according to the results
		atom -> setProperty("CFS_lo_x", lo_vec[0]);
		atom -> setProperty("CFS_lo_y", lo_vec[1]);

		atom -> setProperty("CFS_hi_x", hi_vec[0]);
		atom -> setProperty("CFS_hi_y", hi_vec[1]);

		atom -> setProperty("CFS_free", temp);

		NamedProperty CFS_lo_x = atom -> getProperty("CFS_lo_x");
		NamedProperty CFS_lo_y = atom -> getProperty("CFS_lo_y");
		NamedProperty CFS_hi_x = atom -> getProperty("CFS_hi_x");
		NamedProperty CFS_hi_y = atom -> getProperty("CFS_hi_y");


	}

	//	attach a ringsystem to a partially constructed structure diagram
	void MoleculeAssembler::assembleRS(Atom*& start_atom, Atom*& assemble_atom, vector<vector<vector<Atom*> > >& ringsystems)
	{
		cout << "\nMoleculeAssembler : assembleRS called." << endl;


		Size keep_i;
		Size keep_j;
		bool breaker = 0;

		//	find the ring system, that is to be positioned
		for (Size i = 0; i != ringsystems.size(); i++)
		{
			for (Size j = 0; j != ringsystems[i].size(); j++)
			{
				for (Size k = 0; k != ringsystems[i][j].size(); k++)
				{
					if (ringsystems[i][j][k] == assemble_atom)	//	if the assemble-atom is part of the ringsystem, the correct one is found
					{
						keep_i = i;
						keep_j = j;
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

		//	calculate the direction of an imaginary straight bond from the assembly-atom
		TVector3<float> ref_vec(0.0, 1.0, 0.0);

		//	get Circular Free Sweep - Data of the assemble-atom
		TVector3<float> lo_vec;
		TVector3<float> hi_vec;

		NamedProperty ass_CFS_lo_x = assemble_atom -> getProperty("CFS_lo_x");
		NamedProperty ass_CFS_lo_y = assemble_atom -> getProperty("CFS_lo_y");


		lo_vec[0] = ass_CFS_lo_x.getFloat();
		lo_vec[1] = ass_CFS_lo_y.getFloat();
		lo_vec[2] = 0.0;


		NamedProperty ass_CFS_hi_x = assemble_atom -> getProperty("CFS_hi_x");
		NamedProperty ass_CFS_hi_y = assemble_atom -> getProperty("CFS_hi_y");

		hi_vec[0] = ass_CFS_hi_x.getFloat();
		hi_vec[1] = ass_CFS_hi_y.getFloat();
		hi_vec[2] = 0.0;

		//	angles needed, to determine the imaginary straight bond
		float test_angle_lo = lo_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;
		float test_angle_hi = hi_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;

		//	determine the angle by which either lo_vec or hi_vec must be rotated, to get the imaginary straight bond
		float dir_temp = hi_vec.getAngle(lo_vec) /(2 * Constants::PI)*359.9 ;
		TAngle<float> dir_angle;
		dir_angle.set(180.0 - dir_temp/2, true);

		//	determine, whether lo_vec or hi_vec must be rotated to get the imaginary straight bond
		TVector4<float> ass_straight;
		ass_straight[0]  = hi_vec[0];
		ass_straight[1]  = hi_vec[1];
		ass_straight[2]  = 0.0;
		ass_straight[3]  = 1.0;

		if (lo_vec[0] < 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo > test_angle_hi)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
		}
		else if (lo_vec[0] < 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < 90.0 && test_angle_hi < 90.0)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
			if (test_angle_lo + test_angle_hi < 180.0)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo + test_angle_hi > 180.0)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < test_angle_hi)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
		}

		//	rotate the determined vector by the determined angle
		TMatrix4x4<float> rot_matr;
		rot_matr.setRotationZ(dir_angle);
		ass_straight = rot_matr * ass_straight;

		//	calculate the directon of the bond that connects the ringsystem to the structure diagram

		TVector4<float> assemble_atom_pos_vec;

		//	get Circular Free Sweep - Data of the assemble-atom
		ass_CFS_lo_x = start_atom -> getProperty("CFS_lo_x");
		ass_CFS_lo_y = start_atom -> getProperty("CFS_lo_y");

		ass_CFS_hi_x = start_atom -> getProperty("CFS_hi_x");
		ass_CFS_hi_y = start_atom -> getProperty("CFS_hi_y");


		lo_vec[0] = ass_CFS_lo_x.getFloat();
		lo_vec[1] = ass_CFS_lo_y.getFloat();
		lo_vec[2] = 0.0;

		hi_vec[0] = ass_CFS_hi_x.getFloat();
		hi_vec[1] = ass_CFS_hi_y.getFloat();
		hi_vec[2] = 0.0;

		//	angles, needed to determine the direction of the bond between structure diagram and ringsystem
		test_angle_lo = lo_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;
		test_angle_hi = hi_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;

		//	determine the angle by which either lo_vec or hi_vec must be rotated, to get the bond between structure diagram and ringsystem
		dir_temp = hi_vec.getAngle(lo_vec) /(2 * Constants::PI)*359.9 ;
		dir_angle.set(180.0 - dir_temp/2, true);

		//	determine, whether lo_vec or hi_vec must be rotated to get the bond between structure diagram and ringsystem
		assemble_atom_pos_vec[0]  = hi_vec[0];
		assemble_atom_pos_vec[1]  = hi_vec[1];
		assemble_atom_pos_vec[2]  = 0.0;
		assemble_atom_pos_vec[3]  = 1.0;

		if (lo_vec[0] < 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo > test_angle_hi)
			{
				assemble_atom_pos_vec[0]  = lo_vec[0];
				assemble_atom_pos_vec[1]  = lo_vec[1];
				assemble_atom_pos_vec[2]  = 0.0;
				assemble_atom_pos_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] < 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < 90.0 && test_angle_hi < 90.0)
			{
				assemble_atom_pos_vec[0]  = lo_vec[0];
				assemble_atom_pos_vec[1]  = lo_vec[1];
				assemble_atom_pos_vec[2]  = 0.0;
				assemble_atom_pos_vec[3]  = 1.0;
			}

			if (test_angle_lo + test_angle_hi < 180.0)
			{
				assemble_atom_pos_vec[0]  = lo_vec[0];
				assemble_atom_pos_vec[1]  = lo_vec[1];
				assemble_atom_pos_vec[2]  = 0.0;
				assemble_atom_pos_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo + test_angle_hi > 180.0)
			{
				ass_straight[0]  = lo_vec[0];
				ass_straight[1]  = lo_vec[1];
				ass_straight[2]  = 0.0;
				ass_straight[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < test_angle_hi)
			{
				assemble_atom_pos_vec[0]  = lo_vec[0];
				assemble_atom_pos_vec[1]  = lo_vec[1];
				assemble_atom_pos_vec[2]  = 0.0;
				assemble_atom_pos_vec[3]  = 1.0;
			}
		}

		NamedProperty CFS_free = start_atom -> getProperty("CFS_free");
		dir_temp = (CFS_free.getFloat() / 2);


		//	consider the continuation of the zig-zag-order of an adjacent chain
		if (!(start_atom -> hasProperty(in_ring)))
		{
			if (ringsystems[keep_i].size() < 2)
			{
				//	if the las t bond before the bond between structure diagram and ringsystem was a zag-bond, append a zig-bond
				if (start_atom -> hasProperty(zag))
				{
					dir_angle.set(dir_temp - 56.0, false);
				}
				//	and vice versa
				else
				{
					dir_angle.set(dir_temp + 56.0, false);
				}
			}
			else
			{
				dir_angle.set(dir_temp - 56.0, false);
			}
		}
		//	otherwise attach a straigth bond
		else
		{
			dir_angle.set(dir_temp, false);
		}

		rot_matr.setRotationZ(dir_angle);
		assemble_atom_pos_vec = rot_matr * assemble_atom_pos_vec;

		//	copy of the assemble_atom_pos_vec for later calculation of the rotation angle
		TVector3<float> aapv_temp(assemble_atom_pos_vec[0], assemble_atom_pos_vec[1], 0.0);

		//	finish the creation of the bond between structure diagram and ringsystem
		assemble_atom_pos_vec[0] += start_atom -> getPosition()[0];
		assemble_atom_pos_vec[1] += start_atom -> getPosition()[1];

		// shift the ring system to its correct position
		TVector3<float> shift_vec;

		//	determine the direction and distance, by which the ringsystem must be shifted
		shift_vec[0] = assemble_atom -> getPosition()[0] - assemble_atom_pos_vec[0];
		shift_vec[1] = assemble_atom -> getPosition()[1] - assemble_atom_pos_vec[1];
		shift_vec[2] = 0.0;

		//	shift each atom of the ringsystem by the determined distance an direction
		for (Size i = 0; i != ringsystems[keep_i].size(); i++)
		{
			for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
			{
				if (!(ringsystems[keep_i][i][j] -> hasProperty(shifted)))
				{
					TVector3<float> new_pos = ringsystems[keep_i][i][j] -> getPosition() - shift_vec;
					ringsystems[keep_i][i][j] -> setPosition(new_pos);
					ringsystems[keep_i][i][j] -> setProperty(shifted);
				}
			}
		}

		//	copy of the ass_straight - vector for calculation of the rotation angle
		TVector3<float> astr_temp(ass_straight[0], ass_straight[1], 0.0);

		// calculate the angle to rotate the ring system into the correct orientation
		float rot_angle_fl = aapv_temp.getAngle(astr_temp);
		TAngle<float> rot_angle;
		rot_angle.set(Constants::PI + rot_angle_fl, true);

		//	rotate the ring system into its correct orientation
		rot_matr.setRotationZ(rot_angle);

		for (Size i = 0; i != ringsystems[keep_i].size(); i++)
		{
			for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
			{
				if (!(ringsystems[keep_i][i][j] -> hasProperty(rotated)))
				{
					//	rotate each atom by the determined angle around the assemble-atom
					TVector4<float> rel_pos;
					rel_pos[0] = ringsystems[keep_i][i][j] -> getPosition()[0] - assemble_atom -> getPosition()[0];
					rel_pos[1] = ringsystems[keep_i][i][j] -> getPosition()[1] - assemble_atom -> getPosition()[1];
					rel_pos[2] = 0.0;
					rel_pos[3] = 1.0;

					rel_pos = rot_matr * rel_pos;

					TVector3<float> new_pos(assemble_atom -> getPosition()[0] + rel_pos[0], assemble_atom -> getPosition()[1] + rel_pos[1], 0.0);

					ringsystems[keep_i][i][j] -> setPosition(new_pos);
					ringsystems[keep_i][i][j] -> setProperty(rotated);
				}
			}
		}


		//	if necessary, correct the orientation of the ring system

		SDGenerator sdg;
		pair<Atom*, Atom*> neighbours = sdg.getNeighbours(ringsystems[keep_i][keep_j], assemble_atom);

		Vector3 ass_at_neighb_1;
		Vector3 ass_at_neighb_2;
		Vector3 con_vec;

		ass_at_neighb_1[0] = neighbours.first -> getPosition()[0] - assemble_atom -> getPosition()[0];
		ass_at_neighb_1[1] = neighbours.first -> getPosition()[1] - assemble_atom -> getPosition()[1];
		ass_at_neighb_1[2] = 0.0;

		ass_at_neighb_2[0] = neighbours.second -> getPosition()[0] - assemble_atom -> getPosition()[0];
		ass_at_neighb_2[1] = neighbours.second -> getPosition()[1] - assemble_atom -> getPosition()[1];
		ass_at_neighb_2[2] = 0.0;

		con_vec[0] = start_atom -> getPosition()[0] - assemble_atom -> getPosition()[0];
		con_vec[1] = start_atom -> getPosition()[1] - assemble_atom -> getPosition()[1];
		con_vec[2] = 0.0;

		float neigh_1_angle = con_vec.getAngle(ass_at_neighb_1) / (2 * Constants::PI)*360.0;
		float neigh_2_angle = con_vec.getAngle(ass_at_neighb_2) / (2 * Constants::PI)*360.0;

		//	if the angles between the new bond between ringsystem an structure diagram and the 2 adjacent bonds of the ringsystem are not equal, the ringsystem is rotated step by step until they fit
		float diff = fabs(neigh_1_angle - neigh_2_angle);

		while( diff > 2.1)
		{

			rot_angle.set(2.0, false);

			rot_matr.setRotationZ(rot_angle);

			for (Size i = 0; i != ringsystems[keep_i].size(); i++)
			{
				for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
				{
					ringsystems[keep_i][i][j] -> clearProperty(rotated);
				}
			}

			for (Size i = 0; i != ringsystems[keep_i].size(); i++)
			{
				for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
				{
					if (!(ringsystems[keep_i][i][j] -> hasProperty(rotated)))
					{
						TVector4<float> rel_pos;
						rel_pos[0] = ringsystems[keep_i][i][j] -> getPosition()[0] - assemble_atom -> getPosition()[0];
						rel_pos[1] = ringsystems[keep_i][i][j] -> getPosition()[1] - assemble_atom -> getPosition()[1];
						rel_pos[2] = 0.0;
						rel_pos[3] = 1.0;

						rel_pos = rot_matr * rel_pos;

						TVector3<float> new_pos(assemble_atom -> getPosition()[0] + rel_pos[0], assemble_atom -> getPosition()[1] + rel_pos[1], 0.0);

						ringsystems[keep_i][i][j] -> setPosition(new_pos);
						ringsystems[keep_i][i][j] -> setProperty(rotated);
					}
				}
			}

			ass_at_neighb_1[0] = neighbours.first -> getPosition()[0] - assemble_atom -> getPosition()[0];
			ass_at_neighb_1[1] = neighbours.first -> getPosition()[1] - assemble_atom -> getPosition()[1];
			ass_at_neighb_1[2] = 0.0;

			ass_at_neighb_2[0] = neighbours.second -> getPosition()[0] - assemble_atom -> getPosition()[0];
			ass_at_neighb_2[1] = neighbours.second -> getPosition()[1] - assemble_atom -> getPosition()[1];
			ass_at_neighb_2[2] = 0.0;

			con_vec[0] = start_atom -> getPosition()[0] - assemble_atom -> getPosition()[0];
			con_vec[1] = start_atom -> getPosition()[1] - assemble_atom -> getPosition()[1];
			con_vec[2] = 0.0;

			neigh_1_angle = con_vec.getAngle(ass_at_neighb_1) / (2 * Constants::PI)*360.0;
			neigh_2_angle = con_vec.getAngle(ass_at_neighb_2) / (2 * Constants::PI)*360.0;

			diff =  std::abs(neigh_1_angle - neigh_2_angle);
		}

		//	if the ringsystem has been positioned in the wrong direction, it is rotated by 180°
		if (neigh_1_angle < 90.0)
		{

			rot_angle.set(Constants::PI, true);

			rot_matr.setRotationZ(rot_angle);

			for (Size i = 0; i != ringsystems[keep_i].size(); i++)
			{
				for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
				{
					ringsystems[keep_i][i][j] -> clearProperty(rotated);
				}
			}

			for (Size i = 0; i != ringsystems[keep_i].size(); i++)
			{
				for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
				{
					if (!(ringsystems[keep_i][i][j] -> hasProperty(rotated)))
					{
						TVector4<float> rel_pos;
						rel_pos[0] = ringsystems[keep_i][i][j] -> getPosition()[0] - assemble_atom -> getPosition()[0];
						rel_pos[1] = ringsystems[keep_i][i][j] -> getPosition()[1] - assemble_atom -> getPosition()[1];
						rel_pos[2] = 0.0;
						rel_pos[3] = 1.0;

						rel_pos = rot_matr * rel_pos;

						TVector3<float> new_pos(assemble_atom -> getPosition()[0] + rel_pos[0], assemble_atom -> getPosition()[1] + rel_pos[1], 0.0);

						ringsystems[keep_i][i][j] -> setPosition(new_pos);
						ringsystems[keep_i][i][j] -> setProperty(rotated);
					}
				}
			}
		}

		//	check the newly assembled ring system for atoms that have not yet been asembled
		for (Size i = 0; i != ringsystems[keep_i].size(); i++)
		{
			for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
			{
				for (Atom::BondIterator bond_it = ringsystems[keep_i][i][j] -> beginBond(); bond_it != ringsystems[keep_i][i][j] -> endBond(); bond_it++)
				{
					ringsystems[keep_i][i][j] -> setProperty(deposited);
					ringsystems[keep_i][i][j] -> setProperty(pre_assembled);
					ringsystems[keep_i][i][j] -> setProperty(assembled);

					if ( !((bond_it -> getPartner(*(ringsystems[keep_i][i][j])) -> hasProperty(assembled))))
					{
						aq_.push_back(ringsystems[keep_i][i][j]);
						break;
					}
				}
			}
		}

		//	update the free space around the atoms that have been newly positioned

		for (Size i = 0; i != ringsystems[keep_i].size(); i++)
		{
			for (Size j = 0; j != ringsystems[keep_i][i].size(); j++)
			{
				for (Atom::BondIterator bond_it = ringsystems[keep_i][i][j] -> beginBond(); bond_it != ringsystems[keep_i][i][j] -> endBond(); bond_it++)
				{
					updateCFS(ringsystems[keep_i][i][j]);
				}
			}
		}

	}


	void MoleculeAssembler::assembleChain(Atom*& start_atom, Atom*& assemble_atom, vector<vector<Atom*> >& chains)
	{
		cout << "\n\tMoleculeAssembler : assembleChain called." << endl;

		//	set the chain angle to (180° - 120°)
		float chain_angle = 60.0;

		//	find the correct chain

		bool breaker = 0;
		Size keep_i;
		Size keep_j;
		for (Size i = 0; i != chains.size(); i++)
		{
			for (Size j = 0; j != chains[i].size(); j++)
			{
				if (chains[i][j] == assemble_atom)
				{
					keep_i = i;
					keep_j = j;
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

		Index start = keep_j;



		TVector3<float> ref_vec(0.0, 1.0, 0.0);

		TVector3<float> lo_vec;
		TVector3<float> hi_vec;

		//	calculate the main direction of the chain

		NamedProperty CFS_lo_x = start_atom -> getProperty("CFS_lo_x");
		NamedProperty CFS_lo_y = start_atom -> getProperty("CFS_lo_y");
		NamedProperty CFS_hi_x = start_atom -> getProperty("CFS_hi_x");
		NamedProperty CFS_hi_y = start_atom -> getProperty("CFS_hi_y");

		NamedProperty CFS_free = start_atom -> getProperty("CFS_free");



		lo_vec[0] = CFS_lo_x.getFloat();
		lo_vec[1] = CFS_lo_y.getFloat();
		lo_vec[2] = 0.0;


		hi_vec[0] = CFS_hi_x.getFloat();
		hi_vec[1] = CFS_hi_y.getFloat();
		hi_vec[2] = 0.0;

		//	determine the angle by which either lo_vec or hi_vec must be rotated, to get main direction

		float test_angle_lo = lo_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;

		float test_angle_hi = hi_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;


		float dir_temp = hi_vec.getAngle(lo_vec) /(2 * Constants::PI)*359.9 ;

		TAngle<float> dir_angle;

		dir_angle.set(180.0 - dir_temp/2, false);

		TVector4<float> straight_vec;


		//	determine, whether lo_vec or hi_vec must be rotated to get the main direction
		straight_vec[0]  = hi_vec[0];
		straight_vec[1]  = hi_vec[1];
		straight_vec[2]  = 0.0;
		straight_vec[3]  = 1.0;

		if (lo_vec[0] < 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo > test_angle_hi)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] < 0 && hi_vec[0] > 0)
		{
			if ((test_angle_lo + test_angle_hi) < 180.0 )
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < test_angle_hi)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo + test_angle_hi > 180.0)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}

		//	create two bond-vectors to establish the zig-zag-structure of the chain

		TVector4<float> zig_bond = straight_vec;
		TVector4<float> zag_bond = straight_vec;

		TMatrix4x4<float> rot_matr;

		rot_matr.setRotationZ(dir_angle);

		straight_vec = rot_matr * straight_vec;

		dir_angle.set(3.0, false);

		rot_matr.setRotationZ(dir_angle);

		zig_bond = rot_matr * straight_vec;

		dir_angle.set(chain_angle - 3.0, false);
		rot_matr.setRotationZ(dir_angle);

		zag_bond = rot_matr * straight_vec;

		TVector3<float> pos_vec;
		TVector4<float> bond_vec;

		//	alternately assemble the zig- and the zag-bond to tha last positioned atom

		if (start == 0)
		{
			//	either start at the beginning of the chain and pass forward

			Size cnt = 0;
			for (Index i = start; i != (Index)chains[keep_i].size(); i++)
			{
				cout << chains[keep_i][i] -> getName() << "\t" << flush;

				//	if the new chain connects to another, readily constructed chain, the first bond of the new chain must be must be positioned in the calculated "straight"-direction
				if (start_atom -> hasProperty(core_chain))
				{

					if (i == start)
					{
						bond_vec = straight_vec;
						chains[keep_i][i] -> setProperty(straight);

					}
					else
					{
						if (i % 2 == 0)
						{
							bond_vec = zig_bond;
							chains[keep_i][i] -> setProperty(zig);

						}
						else
						{
							bond_vec = zag_bond;
							chains[keep_i][i] -> setProperty(zag);

						}
					}
				}
				else
				{
					zig_bond = straight_vec;
					dir_angle.set(chain_angle, false);
					rot_matr.setRotationZ(dir_angle);
					zag_bond = rot_matr * straight_vec;

					if (cnt % 2 == 0)
					{
						bond_vec = zig_bond;
						chains[keep_i][i] -> setProperty(zig);
					}
					else
					{
						bond_vec = zag_bond;
						chains[keep_i][i] -> setProperty(zag);
					}
				}


				if (i == start)
				{
					pos_vec[0] = start_atom -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = start_atom -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}
				else
				{
					pos_vec[0] = chains[keep_i][i-1] -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = chains[keep_i][i-1] -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}

				chains[keep_i][i] -> setPosition(pos_vec);
				chains[keep_i][i] -> setProperty(deposited);
				chains[keep_i][i] -> setProperty(pre_assembled);
				chains[keep_i][i] -> setProperty(assembled);

				cnt++;
			}

		}
		else if (start == (Index)(chains[keep_i].size() - 1))
		{
			//	or start at the end of the chain and pass backward through the chain
			Size cnt = 0;
			for (Index i = start; i != - 1; i--)
			{
				cout << chains[keep_i][i] -> getName() << "\t" << flush;


				if (start_atom -> hasProperty(core_chain))
				{
					if (i == start)
					{
						bond_vec = straight_vec;
						chains[keep_i][i] -> setProperty(straight);

					}
					else
					{
						if (i % 2 == 0)
						{
							bond_vec = zig_bond;
							chains[keep_i][i] -> setProperty(zig);

						}
						else
						{
							bond_vec = zag_bond;
							chains[keep_i][i] -> setProperty(zag);

						}
					}
				}
				else
				{
					zig_bond = straight_vec;
					dir_angle.set(chain_angle, false);
					rot_matr.setRotationZ(dir_angle);
					zag_bond = rot_matr * straight_vec;

					if (cnt % 2 == 0)
					{
						bond_vec = zig_bond;
						chains[keep_i][i] -> setProperty(zig);
					}
					else
					{
						bond_vec = zag_bond;
						chains[keep_i][i] -> setProperty(zag);
					}
				}


				if (i == start)
				{
					pos_vec[0] = start_atom -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = start_atom -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}
				else
				{
					pos_vec[0] = chains[keep_i][i+1] -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = chains[keep_i][i+1] -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}

				chains[keep_i][i] -> setPosition(pos_vec);
				chains[keep_i][i] -> setProperty(deposited);
				chains[keep_i][i] -> setProperty(pre_assembled);
				chains[keep_i][i] -> setProperty(assembled);

				cnt++;
			}

		}

		//	check the newly assembled chain for atoms that have not yet been assembled

		for (Size i = 0; i != chains[keep_i].size(); i++)
		{
			updateCFS(chains[keep_i][i]);

			for (Atom::BondIterator bond_it = chains[keep_i][i] -> beginBond(); bond_it != chains[keep_i][i] -> endBond(); bond_it++)
			{
				if ( !((bond_it -> getPartner(*(chains[keep_i][i])) -> hasProperty(assembled))))
				{
					aq_.push_back(chains[keep_i][i]);
					break;
				}
			}
		}


		cout << "chain finished!!" << endl;
	}


	void MoleculeAssembler::assembleSubstituents(Atom*& start_atom, System& molecule_sys)
	{
		cout << "\n\tMoleculeAssembler : assembleSubstituents called." << endl;

		//	get all the not yet assembled substituent of the start-atom

		vector<Atom*> subs;

		for (Atom::BondIterator bond_it = start_atom -> beginBond(); bond_it != start_atom -> endBond(); ++bond_it)
		{
			if (!(bond_it -> getPartner(*start_atom) -> hasProperty(assembled)))
			{
				subs.push_back(bond_it -> getPartner(*start_atom));
			}
		}

		//	calculate the free space that is left around the start-atom


		TVector3<float> ref_vec(0.0, 1.0, 0.0);

		TVector3<float> lo_vec;
		TVector3<float> hi_vec;

		NamedProperty CFS_lo_x = start_atom -> getProperty("CFS_lo_x");
		NamedProperty CFS_lo_y = start_atom -> getProperty("CFS_lo_y");
		NamedProperty CFS_hi_x = start_atom -> getProperty("CFS_hi_x");
		NamedProperty CFS_hi_y = start_atom -> getProperty("CFS_hi_y");

		NamedProperty CFS_free = start_atom -> getProperty("CFS_free");



		lo_vec[0] = CFS_lo_x.getFloat();
		lo_vec[1] = CFS_lo_y.getFloat();
		lo_vec[2] = 0.0;


		hi_vec[0] = CFS_hi_x.getFloat();
		hi_vec[1] = CFS_hi_y.getFloat();
		hi_vec[2] = 0.0;


		float test_angle_lo = lo_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.8;


		float test_angle_hi = hi_vec.getAngle(ref_vec)/(2 * Constants::PI)*359.9;


		float dir_temp = (hi_vec.getAngle(lo_vec) /(2 * Constants::PI)*359.9) ;

		if (dir_temp == 0.0)
		{
			dir_temp = 360.0;
		}


		//	divide the free space by the number of substituents to be assembled

		TAngle<float> dir_angle;

		if (dir_temp < 359.0)
		{
			if (subs.size() == 1)
			{
				dir_angle.set((180.0 - dir_temp/(subs.size() + 1)), false);
			}
			else
			{
				dir_angle.set(((180.0 +dir_temp)/(subs.size()+2)), false);
			}
		}
		else
		{
			if (subs.size() == 1)
			{
				//	consider the possible continuation of a chain's zig-zag-structure
				if (start_atom -> hasProperty(zag))
				{
					dir_angle.set((dir_temp/(subs.size() + 1) - 60.0), false);
				}
				else
				{
					dir_angle.set((dir_temp/(subs.size() + 1) + 60.0), false);
				}
			}
			else
			{

				dir_angle.set((dir_temp/(subs.size() + 1) ) , false);
			}
		}

		TVector4<float> straight_vec;

		//	determine, whether lo_vec or hi_vec must be rotated to get the correct direction for the substituents 

		straight_vec[0]  = hi_vec[0];
		straight_vec[1]  = hi_vec[1];
		straight_vec[2]  = 0.0;
		straight_vec[3]  = 1.0;

		if (lo_vec[0] < 0 && hi_vec[0] < 0)
		{
			if (test_angle_lo > test_angle_hi)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] < 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < 90.0 && test_angle_hi < 90.0)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}

			if (test_angle_lo + test_angle_hi < 180.0)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] > 0)
		{
			if (test_angle_lo < test_angle_hi)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}
		}
		else if (lo_vec[0] > 0 && hi_vec[0] < 0)
		{
			if ((test_angle_lo + test_angle_hi) > 180.0)
			{
				straight_vec[0]  = lo_vec[0];
				straight_vec[1]  = lo_vec[1];
				straight_vec[2]  = 0.0;
				straight_vec[3]  = 1.0;
			}

		}

		//	assemble each substituent in its own sector of the free space

		TMatrix4x4<float> rot_matr;
		TVector3<float> pos_vec;

		for (Size i = 0; i != subs.size(); i++)
		{
			rot_matr.setRotationZ(dir_angle);

			straight_vec = rot_matr * straight_vec;


			pos_vec[0] = start_atom -> getPosition()[0] + straight_vec[0];
			pos_vec[1] = start_atom -> getPosition()[1] + straight_vec[1];


			subs[i] -> setPosition(pos_vec);
			subs[i] -> setProperty(deposited);
			subs[i] -> setProperty(pre_assembled);
			subs[i] -> setProperty(assembled);
		 

			//	check for possible collisions
			cout << "checking for collisions ... " << endl;

			//	scaling variable for the size of the square, that is checked around each newly assembled atom
			float scale = 0.05;

			for (AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); atom_it++)
			{
				if (*atom_it != *subs[i])
				{
					//	if one atom lies inside the checked square of another, a collision is detected
					if ((atom_it -> getPosition()[0] > (subs[i] -> getPosition()[0] - scale)) && (atom_it -> getPosition()[0] < (subs[i] -> getPosition()[0] + scale)))
					{
						if ((atom_it -> getPosition()[1] > (subs[i] -> getPosition()[1] - scale)) && (atom_it -> getPosition()[1] < (subs[i] -> getPosition()[1] + scale)))
						{
							cout << "collision detected!" << endl;

							//	if the colliding substituent is a single ending atom of a chain, it is rotated by the chain-angle into the other direction
							if (subs.size() == 1)
							{
								cout << "dir_angle = " << dir_angle << endl;

								TAngle<float> coll_angle;

								coll_angle.set(-2*dir_angle, true);

								cout << "coll_angle = " << coll_angle << endl;

								rot_matr.setRotationZ(coll_angle);

								TVector4<float> sv_copy = straight_vec;

								TVector4<float> coll_vec = rot_matr * sv_copy;

								coll_vec[0] = 0.85 * coll_vec[0];
								coll_vec[1] = 0.85 * coll_vec[1];

								pos_vec[0] = start_atom -> getPosition()[0] + coll_vec[0];
								pos_vec[1] = start_atom -> getPosition()[1] + coll_vec[1];
							}
							//	otherwise the connecting bond to the colliding atom is shortened to 85% of its original length
							else
							{
								TVector4<float> coll_vec;
								TVector4<float> sv_copy = straight_vec;

								coll_vec[0] = 0.85 * sv_copy[0];
								coll_vec[1] = 0.85 * sv_copy[1];

								pos_vec[0] = start_atom -> getPosition()[0] + coll_vec[0];
								pos_vec[1] = start_atom -> getPosition()[1] + coll_vec[1];
							}
							subs[i] -> setPosition(pos_vec);
						}
					}
				}
			}

		}

		//	check the newly assembled substituents for atoms that have not yet been asembled

		for (Size i = 0; i != subs.size(); i++)
		{

			for (Atom::BondIterator bond_it = subs[i] -> beginBond(); bond_it != subs[i] -> endBond(); ++bond_it)
			{
				if (!(bond_it -> getPartner(*subs[i]) -> hasProperty(assembled)))
				{
					aq_.push_back(subs[i]);
					break;
				}
			}
		}

		cout << "MoleculeAssembler : assembleSubstituents finished.\n" << endl;
	}


	void MoleculeAssembler::assembleMolecule(System& molecule_sys, vector<vector<vector<Atom*> > >& ringsystems, vector<vector<Atom*> >& chains )
	{
		cout << "MoleculeAssembler : assembleMolecule called." << endl;


		// if the molecule contains ringsystem(s) start the assembly with the largest of them

		if (ringsystems.size() != 0)
		{
			std::sort(ringsystems.begin(), ringsystems.end(), comparison);

			Size i, j, k;

			for (i = 0; i != ringsystems.size(); i++)
			{
				for (j = 0; j != ringsystems[i].size(); j++)
				{
					//cout << "i = " << i << endl;
					for (k = 0; k != ringsystems[i][j].size(); k++)
					{
						ringsystems[i][j][k] -> setProperty(pre_assembled);
					}
				}
			}



			for (i = 0; i != ringsystems.size(); i++)
			{
				for (j = 0; j != ringsystems[i].size(); j++)
				{
					for (k = 0; k != ringsystems[i][j].size(); k++)
					{
						updateCFS(ringsystems[i][j][k]);
					}
				}
			}



			for (i = 0; i != ringsystems.size(); i++)
			{
				for (j = 0; j != ringsystems[i].size(); j++)
				{
					for (k = 0; k != ringsystems[i][j].size(); k++)
					{
						ringsystems[i][j][k] -> clearProperty(pre_assembled);
					}
				}
			}



			for (i = 0; i != ringsystems[0].size(); i++)
			{
				for (j = 0; j != ringsystems[0][i].size(); j++)
				{
					ringsystems[0][i][j] -> setProperty(pre_assembled);
				}
			}

			//	check the initially assembled ring system for atoms that have not yet been assembled and put them into the assmenbly queue

			for (i = 0; i != ringsystems[0].size(); i++)
			{
				for (j = 0; j != ringsystems[0][i].size(); j++)
				{
					updateCFS(ringsystems[0][i][j]);

					if (!(ringsystems[0][i][j] -> hasProperty(assembled)))
					{
						ringsystems[0][i][j] -> setProperty(pre_assembled);
						ringsystems[0][i][j] -> setProperty(assembled);

						for (Atom::BondIterator bond_it = ringsystems[0][i][j] -> beginBond(); bond_it != ringsystems[0][i][j] -> endBond(); bond_it++)
						{
							if (bond_it -> getPartner(*(ringsystems[0][i][j])) -> hasProperty(built_in_chain))
							{
								aq_.push_back(ringsystems[0][i][j]);
								break;
							}
						}
					}

				}
			}
		}
		else
		{
			//	otherwise start the assembly by the main chain

			float chain_angle = 60.0;

			TVector4<float> straight_vec(0.0, 2.0, 0.0, 1.0);

			TVector4<float> zig_bond = straight_vec;
			TVector4<float> zag_bond = straight_vec;

			TMatrix4x4<float> rot_matr;

			TAngle<float> dir_angle;
			dir_angle.set(- chain_angle/2, false);

			rot_matr.setRotationZ(dir_angle);

			zig_bond = rot_matr * straight_vec;

			dir_angle.set(chain_angle/2, false);
			rot_matr.setRotationZ(dir_angle);

			zag_bond = rot_matr * straight_vec;

			TVector3<float> pos_vec;
			TVector4<float> bond_vec;

			for (Size i = 0; i != chains[0].size(); i++)
			{
				cout << chains[0][i] -> getName() << "\t" << flush;


				if (i % 2 == 0)
				{
					bond_vec = zig_bond;

				}
				else
				{
					bond_vec = zag_bond;

				}


				if (i == 0)
				{
					pos_vec[0] = 0.0;
					pos_vec[1] = 0.0;
					pos_vec[2] = 0.0;
				}
				else
				{
					pos_vec[0] = chains[0][i-1] -> getPosition()[0] + bond_vec[0];
					pos_vec[1] = chains[0][i-1] -> getPosition()[1] + bond_vec[1];
					pos_vec[2] = 0.0;
				}

				chains[0][i] -> setPosition(pos_vec);
				chains[0][i] -> setProperty(deposited);
				chains[0][i] -> setProperty(pre_assembled);
			}

			//	check the initially assembled chain for atoms that have not yet been assembled and put them into the assmenbly queue

			for (Size i = 0; i != chains[0].size(); i++)
			{
				updateCFS(chains[0][i]);

				if (!(chains[0][i] -> hasProperty(assembled)))
				{
					for (Atom::BondIterator bond_it = chains[0][i] -> beginBond(); bond_it != chains[0][i] -> endBond(); bond_it++)
					{
						if ((bond_it -> getPartner(*(chains[0][i])) -> hasProperty(built_in_chain)) || (bond_it -> getPartner(*(chains[0][i])) -> hasProperty(in_ring)))
						{
							aq_.push_back(chains[0][i]);
							break;
						}
					}
				}
				chains[0][i] -> setProperty(assembled);
			}


		}



		while(!(aq_.empty()))
		{
			//	step by step take one atom from the assembly queue, chaeck whether its either connected to a ring atom or a chain atom and assemble the appropriate fragment

			Atom* start_atom = *aq_.begin();
			aq_.erase(aq_.begin());


			for (Atom::BondIterator bond_it = start_atom -> beginBond(); bond_it != start_atom -> endBond(); bond_it++)
			{
				Atom* assemble_atom = bond_it -> getPartner(*start_atom);

				if (!(assemble_atom -> hasProperty(assembled)))
				{

					if (assemble_atom -> hasProperty(in_ring))
					{

						assembleRS(start_atom, assemble_atom, ringsystems);
					}

					else if (assemble_atom -> hasProperty(built_in_chain))
					{

						assembleChain(start_atom, assemble_atom, chains);
					}
				}
			}
		}

		//	check the newly assembled fragments for atoms that have not yet been assembled and put them into the assmenbly queue

		for (AtomIterator atom_it = molecule_sys.beginAtom(); atom_it != molecule_sys.endAtom(); ++atom_it)
		{
			if (atom_it -> hasProperty(assembled))
			{
				for (Atom::BondIterator bond_it = atom_it -> beginBond(); bond_it != atom_it -> endBond(); bond_it++)
				{
					if (!(bond_it -> getPartner(*atom_it) -> hasProperty(assembled)))
					{
						Atom* a = &*atom_it;
						aq_.push_back(a);
						break;
					}
				}
			}
		}


		while(!(aq_.empty()))
		{
			//	step by step take one atom from the assembly queue, chaeck whether its either connected to a ring atom or a chain atom or neither of them and assemble the appropriate fragment or substituent

			Atom* start_atom = *aq_.begin();
			aq_.erase(aq_.begin());

			for (Atom::BondIterator bond_it = start_atom -> beginBond(); bond_it != start_atom -> endBond(); bond_it++)
			{
				Atom* assemble_atom = bond_it -> getPartner(*start_atom);


				if (!(assemble_atom -> hasProperty(assembled)))
				{
					if (assemble_atom -> hasProperty(in_ring))
					{

						updateCFS(start_atom);

						assembleRS(start_atom, assemble_atom, ringsystems);
					}
					else if (assemble_atom -> hasProperty(built_in_chain))
					{

						updateCFS(start_atom);

						assembleChain(start_atom, assemble_atom, chains);
					}
					else
					{
						updateCFS(start_atom);
						assembleSubstituents(start_atom, molecule_sys);
					}
				}

			}

		}
		cout << "MoleculeAssembler : Molecule assembled." << endl;
	}

} // namespace BALL

