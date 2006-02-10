// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: MMFF94StretchBend.C,v 1.1.2.8 2006/02/10 17:25:20 amoll Exp $
//

#include <BALL/MOLMEC/MMFF94/MMFF94StretchBend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Stretch.h>
#include <BALL/MOLMEC/MMFF94/MMFF94Bend.h>
#include <BALL/MOLMEC/MMFF94/MMFF94.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/SYSTEM/path.h>

//      #define BALL_DEBUG_MMFF

using namespace std;

namespace BALL 
{

	// Constant 
	#define K0 2.51210


	// default constructor
	MMFF94StretchBend::MMFF94StretchBend()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("MMFF94 StretchBend");
	}


	// constructor
	MMFF94StretchBend::MMFF94StretchBend(ForceField& force_field)
		: ForceFieldComponent(force_field)
	{
		// set component name
		setName("MMFF94 StretchBend");
	}


	// copy constructor
	MMFF94StretchBend::MMFF94StretchBend(const MMFF94StretchBend&	component)
		:	ForceFieldComponent(component)
	{
	}

	// destructor
	MMFF94StretchBend::~MMFF94StretchBend()
	{
	}


	// setup the internal datastructures for the component
	bool MMFF94StretchBend::setup()
		throw(Exception::TooManyErrors)
	{
		if (getForceField() == 0) 
		{
			Log.error() << "MMFF94StretchBend::setup(): component not bound to force field" << endl;
			return false;
		}

		if (!parameters_.isInitialized())
		{
			Path    path;
			String  filename1(path.find("MMFF94/MMFFSTBN.PAR"));
			String  filename2(path.find("MMFF94/MMFFDFSB.PAR"));

			if (filename1 == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename1);
			if (filename2 == "") throw Exception::FileNotFound(__FILE__, __LINE__, filename2);

			parameters_.readParameters(filename1, filename2);
		}

		stretch_bends_.clear();
		
		// obtain the Stretch and Bend data from the MMFF94 force field
		MMFF94& mmff = *(MMFF94*)getForceField();
		const MMFF94Stretch& stretch = *(MMFF94Stretch*)mmff.getComponent("MMFF94 Stretch");
		const MMFF94Bend& bend = 			 *(MMFF94Bend*)   mmff.getComponent("MMFF94 Bend");

		const vector<MMFF94Stretch::Stretch>& stretches = stretch.getStretches();
		const vector<MMFF94Bend::Bend>& 			bends 		= bend.getBends();

		// build up a lookup table for the stretch data
		HashMap<long, Position> stretch_map;
		for (Position stretch_pos = 0; stretch_pos < stretches.size(); stretch_pos++)
		{
			long index = ((long) stretches[stretch_pos].atom1) * 
									 ((long) stretches[stretch_pos].atom2);
if (stretch_map.has(index)) Log.error() << "#~~#   5 "             << " "  << __FILE__ << "  " << __LINE__<< std::endl;

			stretch_map[index] = stretch_pos;
		}

		// a working instance to put the current values in and push it back
		StretchBend sb;

		// get all needed data from the stretches and bends
		
		// iterator on the stretch search results
		HashMap<long, Position>::Iterator stretch_it1, stretch_it2;
		
		// iterate over all bends and look for the corresponding bends in the lookup table
		for (Position bend_pos = 0; bend_pos < bends.size(); bend_pos++)
		{
			// "Currently, stretch-bend interactions are omitted when the 
			// i-j-k interaction corresponds to a linear bond angle."
			if (bends[bend_pos].is_linear) continue;

			sb.delta_theta = &bends[bend_pos].delta_theta;
			sb.atom1 = 			  bends[bend_pos].atom1;
			sb.atom2 = 			  bends[bend_pos].atom2;
			sb.atom3 = 			  bends[bend_pos].atom3;

			// make sure calculateSBTIJK gets the sbmb in the right order:
			// pair with the smaller atom type partner first !
			if (sb.atom1->type > sb.atom3->type)
			{
				Atom::StaticAtomAttributes*	temp = sb.atom1;
				sb.atom1 = sb.atom3;
				sb.atom3 = temp;
			}
			
			Atom* a1 = sb.atom1->ptr;
			Atom* a2 = sb.atom2->ptr;
			Atom* a3 = sb.atom3->ptr;

			// find the i->j stretch
			stretch_it1 = stretch_map.find((long) a1 * (long) a2);
			
			// find the j->k stretch
			stretch_it2 = stretch_map.find((long) a2 * (long) a3);

			if (!+stretch_it1 || !+stretch_it2)
			{
				errorOccured_("stretch", *a1, *a2, *a3);
				continue;
			}

			const Position pos1 = stretch_it1->second;
			const Position pos2 = stretch_it2->second;

			// store deltas for the stretches
			sb.delta_r_ij = &stretches[pos1].delta_r;
			sb.delta_r_kj = &stretches[pos2].delta_r;

			sb.sbtijk = calculateSBTIJK(bends[bend_pos].ATIJK, 
																	stretches[pos1].sbmb,
																	stretches[pos2].sbmb);

			// get kba_ijk and kba_kji
			if (sb.sbtijk == -1 ||
			    !parameters_.getParameters(sb.sbtijk, *a1, *a2, *a3, sb.kba_ijk, sb.kba_kji))
			{
				errorOccured_("stretch-bend", *a1, *a2, *a3);
				continue;
			}

/*
Log.info() << sb.atom1->ptr->getName() << " " << sb.atom2->ptr->getName() << " "<< sb.atom3->ptr->getName() << " "  
					 << sb.atom1->type << " " << sb.atom2->type << " " << sb.atom3->type << " :"
					 << sb.kba_ijk << " " << sb.kba_kji << " " << sbtijk << std::endl;
*/
			// we are done for this bend
			stretch_bends_.push_back(sb);
		}

		return true;
	}


	void MMFF94StretchBend::errorOccured_(const String& string, 
																				const Atom& a1, const Atom& a2, const Atom& a3)
	{
		getForceField()->error() << "MMFF94 StretchBend: Could not find " << string << " data! "
														 << a1.getName() << " " << a2.getName() << " " << a3.getName() << "   "
														 << a1.getType() << " " << a2.getType() << " " << a3.getType() 
														 << std::endl;

		getForceField()->getUnassignedAtoms().insert(&a1);
		getForceField()->getUnassignedAtoms().insert(&a2);
		getForceField()->getUnassignedAtoms().insert(&a3);
	}


	// calculates the current energy of this component
	double MMFF94StretchBend::updateEnergy()
	{
		// initial energy is zero
		energy_ = 0;

		for (Size i = 0; i < stretch_bends_.size(); i++)
		{
			StretchBend& sb = stretch_bends_[i];
			double energy = (double)K0 * (sb.kba_ijk * (*sb.delta_r_ij) +
													  				sb.kba_kji * (*sb.delta_r_kj)) 
										 	   					* (*sb.delta_theta);
	
   #ifdef BALL_DEBUG_MMFF
			Log.info() << "MMFF94 SB "  
				<< sb.atom1->ptr->getName() << " "
				<< sb.atom2->ptr->getName() << " "
				<< sb.atom3->ptr->getName() << " "
				<< sb.atom1->type << " "
				<< sb.atom2->type << " "
				<< sb.atom3->type << " :"
				<< sb.kba_ijk << " " 
				<< sb.kba_kji  << " r_ij: " 
				<< *sb.delta_r_ij << " r_ik: " 
				<< *sb.delta_r_kj << " d: " 
				<< *sb.delta_theta<< "      " 
				<< energy << std::endl;
   #endif

			sb.energy = energy;

			energy_ += energy;
		}


		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void MMFF94StretchBend::updateForces()
	{
		if (getForceField() == 0)
		{
			return;
		}
	}

	/* "The stretch-bend types are defined in terms of the constituent bond types BTIJ 
		 and BTJK and the angle type ATIJK as shown below:"

	   Stretch-Bend       Angle           ---- Bond Type ----
					 Type         Type            I-J             J-K
	 -----------------------------------------------------------
						0             0               0               0
						1             1               1               0
						2             1(*)            0               1 		* error in CHARMM docu
						3             2               1               1
						4             4               0               0
						5             3               0               0
						6             5               1               0
						7             5               0               1
						8             6               1               1
						9             7               1               0
					 10             7               0               1
					 11             8 (*)           1               1
	*/
	Index MMFF94StretchBend::calculateSBTIJK(Position angle_type, 
																					 bool bond_type1, bool bond_type2)
	{
		if (angle_type  == 0 || angle_type == 4) return angle_type;

		if (angle_type == 1)
		{
			if (bond_type1) return 1;
			return 2;
		}

		if (angle_type == 2) return 3;
		if (angle_type == 3) return 5;

		if (angle_type == 5)
		{
			if (bond_type1) return 6;
			else 						return 7;
		}

		if (angle_type == 6) return 8;

		if (angle_type == 7)
		{
			if (bond_type1) return 9;
			else 						return 10;
		}

		if (angle_type == 8) return 11;

		Log.error() << "MMFF94 StretchBend: Could not calculate sbtijk " << angle_type << " " 
								<< bond_type1 << " " << bond_type2 << std::endl;

		return -1;
	}

} // namespace BALL
