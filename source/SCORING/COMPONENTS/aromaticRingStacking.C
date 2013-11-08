// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Slick-development Team, Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMPONENTS/aromaticRingStacking.h>

using namespace std;

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
		setName("AromaticRingStacking");
		valid_ = 0;
		gridable_ = 0;
		angle_tolerance_ = 5;
		atom_pairwise_ = 0;
	}

	AromaticRingStacking::AromaticRingStacking(ScoringFunction& sf)
		: ScoringComponent(sf)
	{
		// Set the name
		setName("SLICK AromaticRingStacking");
		valid_ = 0;
		gridable_ = 0;
		angle_tolerance_ = 5;
		atom_pairwise_ = 0;
	}

	AromaticRingStacking::~AromaticRingStacking()
	{
		clear();
	}


	void AromaticRingStacking::clear()
	{
		for (Size i = 0; i < receptor_rings_.size(); i++)
		{
			delete receptor_rings_[i];
		}
		for (Size i = 0; i < ligand_rings_.size(); i++)
		{
			delete ligand_rings_[i];
		}
		ligand_rings_.clear();
		receptor_rings_.clear();
		possible_interactions_.clear();
	}


	bool AromaticRingStacking::setup(Options& options)
	{
		if (scoring_function_ == 0)
		{
			Log.error() << "AromaticRingStacking::setup(): "
				<< "component not bound to scoring function." << std::endl;
			return false;
		}

		clear();

		// Set all paramters
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

		// All ring-pairs with a distance larger than this value would result in a score of zero, so simple ignore those pairs!
		distance_cutoff_ = max(max(f2f_plane_distance_upper_, f2f_lateral_displacemant_upper_), f2e_center_distance_upper_)+scoring_tolerance_;


		/// Find aromatic rings in receptor
		AtomContainer* receptor = scoring_function_->getReceptor();

		// First, find the rings of the receptor (via SSSR)
		std::vector< std::vector< Atom*> > SSSR_r;
		Size rings_r = rp_.calculateSSSR(SSSR_r, *receptor);

		// If there are no rings, we cannot compute anything.
		if (rings_r == 0)
		{
			return true;
		}

		// Now look for aromatic rings
		ap_.aromatize(SSSR_r, *receptor);
		std::vector< std::vector< Atom* > >::iterator SSSR_it;
		for (SSSR_it = SSSR_r.begin(); SSSR_it != SSSR_r.end(); ++SSSR_it)
		{
			std::vector< Atom* >::iterator ring_atom_it = SSSR_it->begin();
			if ((*ring_atom_it)->hasProperty("IsAromatic"))
			{
				CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
				receptor_rings_.push_back(current_ring);
				//current_ring->dump();
			}
		}
		cout<<"AromaticRingStacking::setup() found "<<receptor_rings_.size()<<" aromatic rings within the receptor"<<endl;

		valid_ = 1;

		setupLigand();

		// remember the setup time in order to known later whether the receptor-structure was modified (translation of entire system or rotation of side-chains)
		update_time_stamp_.stamp();

		return true;
	}



	// This function needs to be called once for every new ligand
	void AromaticRingStacking::setupLigand()
	{
		if (valid_ == 0)
		{
			Log.error() << "AromaticRingStacking::setupLigand() error: Component has not been set up properly!" << endl;
			return;
		}

		if (receptor_rings_.size() == 0)
		{
			return;
		}

		AtomContainer* ligand = scoring_function_->getLigand();

		vector< std::vector< Atom*> > SSSR_l;
		Size rings_l = rp_.calculateSSSR(SSSR_l, *ligand);

		// If there are no rings, we cannot compute anything.
		if (rings_l == 0)
		{
			cout << "AromaticRingStacking::setupLigand() found no aromatic rings within the ligand" << endl;
			return;
		}

		/// Find aromatic rings in ligand
		for (Size i = 0; i < ligand_rings_.size(); i++)
		{
			delete ligand_rings_[i];
		}
		ligand_rings_.clear();
		ap_.aromatize(SSSR_l, *ligand);
		vector<vector<Atom*> >::iterator SSSR_it;
		for (SSSR_it = SSSR_l.begin(); SSSR_it != SSSR_l.end(); ++SSSR_it)
		{
			std::vector< Atom* >::iterator ring_atom_it = SSSR_it->begin();
			if ((*ring_atom_it)->hasProperty("IsAromatic"))
			{
				CHPI::AromaticRing* current_ring = new CHPI::AromaticRing(*SSSR_it);
				ligand_rings_.push_back(current_ring);
			}
		}

		if (ligand_rings_.size() == 0)
		{
			cout << "AromaticRingStacking::setupLigand() found no aromatic rings within the ligand" << endl;
			return;
		}

		cout << "AromaticRingStacking::setupLigand() found " << ligand_rings_.size()<< " aromatic rings within the ligand" << endl;

		AtomPairVector empty(0);
		update(empty);
	}


	void AromaticRingStacking::update(const vector<std::pair<Atom*, Atom*> >& /*atom_pairs*/)
	{
		// ignore 'atom_pairs', since this component does not calculate a pair-wise score

		 /// Recalculate ring-centers and normal-vectors
		if (update_time_stamp_.isOlderThan(scoring_function_->getReceptor()->getModificationTime()))
		{
			cout<<"Receptor has been translated or rotated; updating the aromatic ring centers and normal-vectors..."<<endl;
			for (vector < CHPI::AromaticRing* > ::const_iterator r_it = receptor_rings_.begin(); r_it != receptor_rings_.end(); ++r_it)
			{
				(*r_it)->update();
			}
			update_time_stamp_.stamp();
		}

		for (vector < CHPI::AromaticRing* > ::const_iterator l_it = ligand_rings_.begin(); l_it != ligand_rings_.end(); ++l_it)
		{
			(*l_it)->update();
		}


		/// Build pairs of aromatic rings
		possible_interactions_.clear();
		vector<CHPI::AromaticRing*>::const_iterator r_it;
		vector<CHPI::AromaticRing*>::const_iterator l_it;

		for (r_it = receptor_rings_.begin(); r_it != receptor_rings_.end(); ++r_it)
		{
			for (l_it = ligand_rings_.begin(); l_it != ligand_rings_.end(); ++l_it)
			{
				if ((*r_it)->getCentre().getDistance((*l_it)->getCentre()) < distance_cutoff_)
				{
					possible_interactions_.push_back(std::pair<const CHPI::AromaticRing*, const CHPI::AromaticRing*>(*r_it, *l_it));
				}
			}
		}
		//if (possible_interactions_.size() != 0) cout<<"AromaticRingStacking::update() found "<<possible_interactions_.size()<<" possible ring interactions"<<endl;
	}




	double AromaticRingStacking::updateScore()
	{
		if (possible_interactions_.size() == 0)
		{
			return 0.0;
		}

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
					 += base_function_->calculate(plane_distance,
							f2f_plane_distance_lower_ + scoring_tolerance_,
							f2f_plane_distance_lower_ - scoring_tolerance_)
					* base_function_->calculate(plane_distance,
							f2f_plane_distance_upper_ - scoring_tolerance_,
							f2f_plane_distance_upper_ + scoring_tolerance_)
					* base_function_->calculate(lateral_displacement,
							f2f_lateral_displacemant_lower_ + scoring_tolerance_,
							f2f_lateral_displacemant_lower_ - scoring_tolerance_)
					* base_function_->calculate(lateral_displacement,
							f2f_lateral_displacemant_upper_ - scoring_tolerance_,
							f2f_lateral_displacemant_upper_ + scoring_tolerance_);
			}

			// face to edge
			if ((fabs(90 - angle) < angle_tolerance_)
					|| (fabs(270 -angle) < angle_tolerance_))
			{
				float d = distance.getLength();
				f2e_score += base_function_->calculate(d,
							f2e_center_distance_lower_ + scoring_tolerance_,
							f2e_center_distance_lower_ - scoring_tolerance_)
					* base_function_->calculate(d,
							f2e_center_distance_upper_ - scoring_tolerance_,
							f2e_center_distance_upper_ + scoring_tolerance_);
			}
		}

		// we want a negative score for a good pose, thus we will use the negative of the value computed above
		score_ = -(f2f_score + f2e_score);

		/*
		scaleScore();
		return score_;
		*/

		return getScaledScore();
	}
}
