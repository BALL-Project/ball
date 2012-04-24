// $Id: aromaticRingStacking.C,v 1.1 2005/11/21 19:27:09 anker Exp $

// THIS IS PRELIMINARY AND UNTESTED CODE! Use at your own risk.

#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/QSAR/aromaticityProcessor.h>

namespace BALL
{

	const String AromaticRingStacking::Option::VERBOSITY 
		= "verbosity";
	const String AromaticRingStacking::Option::F2F_PLANE_DISTANCE_LOWER
		= "f2f_plane_distance_lower";
	const String AromaticRingStacking::Option::F2F_PLANE_DISTANCE_UPPER 
		= "f2f_plane_distance_upper";
	const String AromaticRingStacking::Option::F2F_LATERAL_DISPLACEMENT_LOWER 
		= "f2f_lateral_displacement_lower";
	const String AromaticRingStacking::Option::F2F_LATERAL_DISPLACEMENT_UPPER 
		= "f2f_lateral_displacement_upper";
	const String AromaticRingStacking::Option::F2E_CENTER_DISTANCE_LOWER 
		= "f2e_center_distance_lower";
	const String AromaticRingStacking::Option::F2E_CENTER_DISTANCE_UPPER 
		= "f2e_center_distance_upper";
	const String AromaticRingStacking::Option::SCORING_TOLERANCE 
		= "scoring_tolerance";

	// Values from Angew. Chem. 115(11):1244-1287, 2003
	const Size AromaticRingStacking::Default::VERBOSITY 
		= 0;
	const float AromaticRingStacking::Default::F2F_PLANE_DISTANCE_LOWER
		= 3.40f;
	const float AromaticRingStacking::Default::F2F_PLANE_DISTANCE_UPPER 
		= 3.60f;
	const float AromaticRingStacking::Default::F2F_LATERAL_DISPLACEMENT_LOWER 
		= 1.60f;
	const float AromaticRingStacking::Default::F2F_LATERAL_DISPLACEMENT_UPPER 
		= 1.80f;
	const float AromaticRingStacking::Default::F2E_CENTER_DISTANCE_LOWER 
		= 4.96f - 0.10f;
	const float AromaticRingStacking::Default::F2E_CENTER_DISTANCE_UPPER 
		= 4.96f + 0.10f;
	const float AromaticRingStacking::Default::SCORING_TOLERANCE 
		= 0.10f;


	AromaticRingStacking::AromaticRingStacking()
		
	{
		// Set the name
		setName("SLICK AromaticRingStacking");
	}

	AromaticRingStacking::AromaticRingStacking(ScoringFunction& sf)
		
		: ScoringComponent(sf)
	{
		// Set the name
		setName("SLICK AromaticRingStacking");
	}

	AromaticRingStacking::~AromaticRingStacking()
		
	{
	}

	bool AromaticRingStacking::setup()
		
	{


		ScoringFunction* scoring_function = getScoringFunction();
		if (scoring_function == 0)
		{
			Log.error() << "CHPI::setup(): "
				<< "component not bound to scoring function." << std::endl;
			return false;
		}

		// Clear all data structures
		clear();

		// Set all paramters
		Options options;
		options.setDefaultInteger(Option::VERBOSITY, Default::VERBOSITY);
		options.setDefaultReal(Option::F2F_PLANE_DISTANCE_LOWER,
				Default::F2F_PLANE_DISTANCE_LOWER);
		options.setDefaultReal(Option::F2F_PLANE_DISTANCE_UPPER,
				Default::F2F_PLANE_DISTANCE_UPPER);
		options.setDefaultReal(Option::F2F_LATERAL_DISPLACEMENT_LOWER,
				Default::F2F_LATERAL_DISPLACEMENT_LOWER);
		options.setDefaultReal(Option::F2F_LATERAL_DISPLACEMENT_UPPER,
				Default::F2F_LATERAL_DISPLACEMENT_UPPER);
		options.setDefaultReal(Option::F2E_CENTER_DISTANCE_LOWER,
				Default::F2E_CENTER_DISTANCE_LOWER);
		options.setDefaultReal(Option::F2E_CENTER_DISTANCE_UPPER,
				Default::F2E_CENTER_DISTANCE_UPPER);
		options.setDefaultReal(Option::SCORING_TOLERANCE,
				Default::SCORING_TOLERANCE);

		f2f_plane_distance_lower_ 
			= options.getReal(Option::F2F_PLANE_DISTANCE_LOWER);
		f2f_plane_distance_upper_ 
			= options.getReal(Option::F2F_PLANE_DISTANCE_UPPER);
		f2f_lateral_displacemant_lower_ 
			= options.getReal(Option::F2F_LATERAL_DISPLACEMENT_LOWER);
		f2f_lateral_displacemant_upper_
			= options.getReal(Option::F2F_LATERAL_DISPLACEMENT_UPPER);
		f2e_center_distance_lower_ 
			= options.getReal(Option::F2E_CENTER_DISTANCE_LOWER);
		f2e_center_distance_upper_
			= options.getReal(Option::F2E_CENTER_DISTANCE_UPPER);
		scoring_tolerance_ = options.getReal(Option::SCORING_TOLERANCE);

		// Find aromatic rings in receptor and ligand

		Molecule& receptor = *(scoring_function->getReceptor());
		
		// First, find the rings of the receptor (via SSSR)
		std::vector< std::vector< Atom*> > SSSR_r;
		RingPerceptionProcessor rpp;
		Size rings_r = rpp.calculateSSSR(SSSR_r, receptor);
		
		// If there are no rings, we cannot compute anything.
		if (rings_r == 0)
		{
			return(true);
		}

		// Now look for aromatic rings
		AromaticityProcessor is_aromatic_r;
		is_aromatic_r.aromatize(SSSR_r, receptor);
		std::vector< std::vector< Atom* > >::iterator SSSR_it;
		for (SSSR_it = SSSR_r.begin(); SSSR_it != SSSR_r.end(); ++SSSR_it)
		{
			std::vector< Atom* >::iterator ring_atom_it = SSSR_it->begin();
			if ((*ring_atom_it)->hasProperty("IsAromatic"))
			{
				CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
				receptor_rings_.push_back(*current_ring);
			}
		}
		
		if (receptor_rings_.empty())
		{
			return(true);
		}

		Molecule& ligand = *(scoring_function->getLigand());

		std::vector< std::vector< Atom*> > SSSR_l;
		Size rings_l = rpp.calculateSSSR(SSSR_l, ligand);
		
		// If there are no rings, we cannot compute anything.
		if (rings_l == 0)
		{
			return(true);
		}

		// Now look for aromatic rings
		AromaticityProcessor is_aromatic_l;
		is_aromatic_l.aromatize(SSSR_l, ligand);
		for (SSSR_it = SSSR_l.begin(); SSSR_it != SSSR_l.end(); ++SSSR_it)
		{
			std::vector< Atom* >::iterator ring_atom_it = SSSR_it->begin();
			if ((*ring_atom_it)->hasProperty("IsAromatic"))
			{
				CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
				ligand_rings_.push_back(*current_ring);
			}
		}

		if (ligand_rings_.empty())
		{
			return(true);
		}
		

		// Build pairs of aromatic rings
		std::vector<CHPI::AromaticRing>::const_iterator r_it;
		std::vector<CHPI::AromaticRing>::const_iterator l_it;
		for (r_it = receptor_rings_.begin(); r_it != receptor_rings_.end(); ++r_it)
		{
			for (l_it = ligand_rings_.begin(); l_it != ligand_rings_.end(); ++l_it)
			{
				// ??? Maybe introduce some cut-off
				possible_interactions_.push_back(std::pair<const CHPI::AromaticRing*, const CHPI::AromaticRing*>(&*r_it, &*l_it));
			}
		}

		return(true);
	}

	double AromaticRingStacking::calculateScore()
		
	{

		if (possible_interactions_.empty())
		{
			return(0.0);
		}

#ifdef DEBUG
		Molecule debug_molecule;
#endif

		// Reset the energy value.
		score_ = 0.0f;

		float f2f_score = 0.0f;
		float f2e_score = 0.0f;

		Vector3 c_r;
		Vector3 c_l;
		Vector3 n_r;
		Vector3 n_l;
		float angle;
		Vector3 distance;

		// iterate over all ring pairs
		std::vector< std::pair<const CHPI::AromaticRing*, const CHPI::AromaticRing*> >::const_iterator it = possible_interactions_.begin();
		for (; it != possible_interactions_.end(); ++it)
		{
			n_r = it->first->getNormalVector();
			n_l = it->second->getNormalVector();
			c_r = it->first->getCentre();
			c_l = it->second->getCentre();

			angle = n_r.getAngle(n_l).toDegree();
			distance = c_l - c_r;

			// face to face
			if ((fabs(angle) < angle_tolerance_) 
					|| (fabs(180.0f - angle) < angle_tolerance_))
			{
				// Calculate the distance of the planes
				Vector3 plane_distance_vec = ((distance * n_r) * n_r);
				float plane_distance = plane_distance_vec.getLength();

				// Calculate projected distance of one ring centre to the other
				// one
				Vector3 lateral_displacement_vec = c_r + plane_distance_vec - c_l;
				float lateral_displacement = lateral_displacement_vec.getLength();

				f2f_score
					+= getScoringFunction()->getBaseFunction()->calculate(plane_distance,
							f2f_plane_distance_lower_ + scoring_tolerance_,
							f2f_plane_distance_lower_ - scoring_tolerance_)
					* getScoringFunction()->getBaseFunction()->calculate(plane_distance,
							f2f_plane_distance_upper_ - scoring_tolerance_,
							f2f_plane_distance_upper_ + scoring_tolerance_)
					* getScoringFunction()->getBaseFunction()->calculate(lateral_displacement,
							f2f_lateral_displacemant_lower_ + scoring_tolerance_,
							f2f_lateral_displacemant_lower_ - scoring_tolerance_)
					* getScoringFunction()->getBaseFunction()->calculate(lateral_displacement,
							f2f_lateral_displacemant_upper_ - scoring_tolerance_,
							f2f_lateral_displacemant_upper_ + scoring_tolerance_);
				
#ifdef DEBUG
#endif

			}

			// face to edge
			if ((fabs(90 - angle) < angle_tolerance_)
					|| (fabs(270 -angle) < angle_tolerance_))
			{
				float d = distance.getLength();
				f2e_score 
					+= getScoringFunction()->getBaseFunction()->calculate(d,
							f2e_center_distance_lower_ + scoring_tolerance_,
							f2e_center_distance_lower_ - scoring_tolerance_)
					* getScoringFunction()->getBaseFunction()->calculate(d,
							f2e_center_distance_upper_ - scoring_tolerance_,
							f2e_center_distance_upper_ + scoring_tolerance_);

#ifdef DEBUG
#endif

			}
		}

		score_ = f2f_score + f2e_score;

		// Iterate over all pairs of aromatic rings
		// Compute score
		// Sum up scores
		return(score_);
	}

}
