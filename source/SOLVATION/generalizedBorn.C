#include <BALL/SOLVATION/generalizedBorn.h>
#include <BALL/KERNEL/forEach.h>

#include <cmath>
#include <vector>

#include <BALL/STRUCTURE/geometricProperties.h>

#define DEBUG 1
// This flag deactivates the (at the moment probably incorrect)
// sophisticated iteration over near boxes.
#define BRUTE_FORCE 1

#ifdef DEBUG
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/FORMAT/HINFile.h>
#endif

using namespace std;

namespace BALL
{

	GeneralizedBorn::GeneralizedBorn()
		: k_(0.0f),
			k_prime_(0.0f),
			eps_solvent_(0.0f),
			eps_solute_(0.0f),
			atom_container_(0),
			lookup_grid_(),
			shell_radii_(),
			integration_points_(),
			integration_weights_CFA_(),
			integration_weights_CT_(),
			nearest_radius_rec_(),
			beta_(0.0f),
			lambda_(0.0f),
			gamma_0_(0.0f),
			P_(0.0f),
			alpha_0_(0.0f),
			verbosity_(100)
	{
		// These values should come form a parameter file
		eps_solute_ = 2.0f;
		eps_solvent_ = 78.0f;
	}

	GeneralizedBorn::GeneralizedBorn(const AtomContainer& atom_container)
		: k_(0.0f),
			k_prime_(0.0f),
			eps_solvent_(0.0f),
			eps_solute_(0.0f),
			atom_container_(&atom_container),
			lookup_grid_(),
			shell_radii_(),
			integration_points_(),
			integration_weights_CFA_(),
			integration_weights_CT_(),
			nearest_radius_rec_(),
			beta_(0.0f),
			lambda_(0.0f),
			gamma_0_(0.0f),
			P_(0.0f),
			alpha_0_(0.0f),
			verbosity_(100)
	{
		// These values should come form a parameter file
		eps_solute_ = 2.0f;
		eps_solvent_ = 78.0f;
	}

	bool GeneralizedBorn::setup(const AtomContainer& atom_container)
	{
		atom_container_ = &atom_container;
		return(setup());
	}

	bool GeneralizedBorn::setup()
	{
		// k is in units of A * kcal / (mol * (atomic charge)^2)
		k_ = 166.0f;

		beta_ = -100.0f;
		lambda_ = 0.1f;
		gamma_0_ = 0.44f;
		P_ = 2.92f;
		alpha_0_ = -0.5f;

		k_prime_ = k_ * (1.0f/eps_solute_ - 1.0f/eps_solvent_);

		// This should be done through a paramter file
		shell_radii_.clear();
		for (float f = 0.5; f < 4.0; f += 0.5)
		{
			shell_radii_.push_back(f);
		}
		for (float f = 4.0; f <= 20.0; f += 1.0)
		{
			shell_radii_.push_back(f);
		}

		nearest_radius_rec_.clear();
		Size max_shell_index;

		for (Size i = 0; i < shell_radii_.size(); ++i)
		{
			// It makes no sense to allow radii greater 4.0 Angstroms
			max_shell_index = i;
			if (shell_radii_[i] > 4.0)
			{
				break;
			}
		}

		AtomConstIterator it = atom_container_->beginAtom();
		for (; +it; ++it)
		{
			float radius = it->getRadius();
			float diff;
			float min_diff = 100.0f;
			float current_radius;
			for (Size i = 0; i < max_shell_index; ++i)
			{
				diff = fabs(shell_radii_[i] - radius);
				if (diff < min_diff) 
				{
					min_diff = diff;
					current_radius = shell_radii_[i];
					nearest_radius_rec_[&*it] = 1.0f/current_radius;
				}
			}
			if (verbosity_ > 99)
			{
				Log.info() << "Mapping actual radius " << radius << " of "
					<< it->getFullName() << " on shell radius " << current_radius 
					<< endl;
			}
		}

		bool retval = setupLookupGrid_();
		retval = retval && setupIntegrationPoints_();
		retval = retval && setupIntegrationWeights_();

		dump();

		return(retval);
	}

	void GeneralizedBorn::update()
	{
		updateLookupGrid_();
	}

	void GeneralizedBorn::setSoluteDC(float solute_dc)
	{
		eps_solute_ = solute_dc;
	}

	void GeneralizedBorn::setSolventDC(float solvent_dc)
	{
		eps_solvent_ = solvent_dc;
	}

	void GeneralizedBorn::calculateBornRadii_()
	{
		born_radii_.clear();

		float integration_value;
		AtomConstIterator it = atom_container_->beginAtom();

		for (; +it; ++it)
		{
			integration_value = integrate_(&*it);
			if (integration_value == 0.0f)
			{
				// ????? Make this an exception later
				Log.error() << "Division by zero! Integration yielded 0.0" << endl;
			}
			else
			{
				born_radii_[&*it] = 1.0f / integration_value + alpha_0_;
#ifdef DEBUG
				cout << "alpha = " << born_radii_[&*it] << endl;
				born_radii_[&*it] *= 3.70f;
				cout << "modified alpha = " << born_radii_[&*it] << endl;
#endif
			}
		}
	}

	float GeneralizedBorn::calculateEnergy()
	{

		// ????? should this method be run here or somewhere else?
		calculateBornRadii_();

		float e = 0.0f;
		float E = 0.0f;

		AtomConstIterator it1;
		AtomConstIterator it2;

		BALL_FOREACH_ATOM_PAIR(*atom_container_, it1, it2)
		{
			float q_i = it1->getCharge();
			float q_j = it2->getCharge();
			float square_distance 
				= (it1->getPosition() - it2->getPosition()).getSquareLength();
			float alpha_i_alpha_j = born_radii_[&*it1] * born_radii_[&*it2];

			e = q_i * q_j / sqrt(square_distance + alpha_i_alpha_j 
					* exp(-(square_distance/(4.0f * alpha_i_alpha_j))));

#ifdef DEBUG
			cout << "e = " << e << endl;
#endif

			E += e;
			
		}
#ifdef DEBUG
		cout << "E = " << E << endl;
#endif

		E = - k_prime_ * E * 4.184f;

#ifdef DEBUG
		cout << "E' = " << E << endl;
#endif

		return(E);
	}

	float GeneralizedBorn::calculateAtomicVolumeContribution_(const Vector3& r)
	{
		float s = 0.0f;
		float gamma_0_ln_lambda = gamma_0_ * log(lambda_);
#ifdef DEBUG
		cout << "gamma_0 * log(lambda) = " << gamma_0_ln_lambda << endl;
		cout << "r = " << r << endl;
#endif
		float distance;

#ifdef BRUTE_FORCE
		AtomConstIterator it = atom_container_->beginAtom();
#else
		// ????
		// This is not correct. Should we iterate over boxes?
		HashGridBox3<const Atom*>* box;
		box = lookup_grid_.getBox(it->getPosition());
		HashGridBox3<const Atom*>::ConstDataIterator it = box->beginData();
#endif
		for (; +it; ++it)
		{
			distance = (r - it->getPosition()).getLength();
			float s_j = exp(gamma_0_ln_lambda / (pow(it->getRadius(), 4.0f))
					* pow(distance, 4.0f));
			s += s_j;
#ifdef DEBUG
			cout << "x_j = " << it->getPosition() << endl;
			cout << "s_j = " << s_j << " @ " << distance << " A" << endl;
#endif
		}
#ifdef DEBUG
		cout << "s = " << s << endl;
#endif
		float H = 1.0f/(1.0f + exp(beta_ * (s - lambda_)));

		return(H);
	}

	bool GeneralizedBorn::setupLookupGrid_()
	{
		Log.info() << "Setting up lookup grid. This will take a while." 
			<< std::endl;
		// This might move to the paramters.
		epsilon_ = 1e-8f;
		// grid_spacing_ = 0.2f;
		grid_spacing_ = 3.0f;
		r_buffer_ = 0.3;
		// Two constants used in the calculation of r_iw
		C_ = pow(log(epsilon_) / log(lambda_ * gamma_0_), 0.25f);
		D_ = 0.5f * sqrt(3.0f) * grid_spacing_ + r_buffer_;

		// set boundary to something connected with the integration shells
		float boundary = shell_radii_[shell_radii_.size() - 1] + 1.0f;
		if (verbosity_ > 99)
		{
			Log.info() << "Setting boundary to " << boundary << " A" << endl;
		}

		BoundingBoxProcessor bb_proc;
		const_cast<AtomContainer*>(atom_container_)->apply(bb_proc);
		lookup_grid_.clear();
		Vector3 lower = bb_proc.getLower() - boundary * Vector3(1.0);
		if (verbosity_ > 99)
		{
			Log.info() << "Bounding box: " << bb_proc.getLower() << " " 
				<< bb_proc.getUpper() << endl;
		}
		Vector3 grid_size = bb_proc.getUpper() - lower + boundary * Vector3(1.0);
		lookup_grid_ = HashGrid3<const Atom*>(lower, grid_size, grid_spacing_);

		// The 3.0f factor is a wild guess. It should be the greatest radius of
		// an atom. We take the double of that for finding atoms in order to be
		// sure not to miss an atom
		helper_grid_ = HashGrid3<const Atom*>(lower, grid_size, 
				2.0f * 3.0f * C_ + D_);
		// Fill the helper grid with all atoms for fast update of the lookup
		// grid.
		Log.info() << "Setting up helper grid...";
		AtomConstIterator it = atom_container_->beginAtom();
		for (; +it; ++it) helper_grid_.insert(it->getPosition(), &*it);
		Log.info() << "done." << endl;

		return(updateLookupGrid_());
	}

	bool GeneralizedBorn::updateLookupGrid_()
	{
		// Fill all boxes with the neighbouring atoms according to the distance
		// criterion described in the paper
		Position x = 0;
		Position y = 0;
		Position z = 0;

		Vector3 origin = lookup_grid_.getOrigin();
		Vector3 half_cell(0.5 * grid_spacing_);
		Vector3 box_centre;
		float r_iw;

		for (x = 0; x < lookup_grid_.getSizeX(); ++x)
		{
			for (y = 0; y < lookup_grid_.getSizeY(); ++y)
			{
				for (z = 0; z < lookup_grid_.getSizeZ(); ++z)
				{
					// clear the boxes.
					HashGridBox3<const Atom*>* lookup_box = lookup_grid_.getBox(x, y, z);
					box_centre = origin + grid_spacing_ * Vector3(x, y, z) + half_cell;
					lookup_box->clear();

					HashGridBox3<const Atom*>::ConstBoxIterator helper_box_it;
					const HashGridBox3<const Atom*>* helper_box 
						= helper_grid_.getBox(box_centre);
					// If there is no atom in the area of that box, there will be no
					// box.
					if (helper_box != 0)
					{
						helper_box_it = helper_box->beginBox();
						for (; +helper_box_it; ++helper_box_it)
						{
							HashGridBox3<const Atom*>::ConstDataIterator helper_data_it;
							helper_data_it = helper_box_it->beginData();
							for (; +helper_data_it; ++helper_data_it)
							{
								// For readability reasons
								const Atom* current_atom = *helper_data_it;
								r_iw = current_atom->getRadius() * C_ + D_;
								if ((box_centre - current_atom->getPosition()).getLength() 
										< r_iw)
								{
									if (verbosity_ > 199)
									{
										Log.info() << "Inserting " << current_atom->getFullName()
											<< "@" << current_atom->getPosition() 
											<< " into (" << x << ", " << y << ", " << z 
											<< "), distance is " 
											<< (box_centre - current_atom->getPosition()).getLength()
											<< " < " << r_iw << endl;
									}
									lookup_grid_.insert(x, y, z, current_atom);
								}
							}
						}
					}
				}
			}
		}
		return(true);
	}

	bool GeneralizedBorn::setupIntegrationPoints_()
	{
		Log.info() << "Setting up integration points...";
		// Integration points are stored as normalized vectors from the atom
		// contre to a node of a regular dodecahedron. This means that we only
		// have to calculate a dodecahedron once with the smallest shell
		// radius. The other shells can be calculated then by multiplying the
		// radius.
		integration_points_.clear();
		integration_points_.push_back(Vector3( 0.0f,       0.0f,       1.0f));
		integration_points_.push_back(Vector3( 0.666667f,  0.0f,       0.745356f));
		integration_points_.push_back(Vector3(-0.333333f,  0.57735f,   0.745356f));
		integration_points_.push_back(Vector3(-0.333333f, -0.57735f,   0.745356f));
		integration_points_.push_back(Vector3( 0.745356f,  0.57735f,   0.333333f));
		integration_points_.push_back(Vector3( 0.745356f, -0.57735f,   0.333333f));
		integration_points_.push_back(Vector3(-0.872678f,  0.356822f,  0.333333f));
		integration_points_.push_back(Vector3( 0.127322f,  0.934172f,  0.333333f));
		integration_points_.push_back(Vector3( 0.127322f, -0.934172f,  0.333333f));
		integration_points_.push_back(Vector3(-0.872678f, -0.356822f,  0.333333f));
		integration_points_.push_back(Vector3( 0.872678f,  0.356822f, -0.333333f));
		integration_points_.push_back(Vector3( 0.872678f, -0.356822f, -0.333333f));
		integration_points_.push_back(Vector3(-0.745356f,  0.57735f,  -0.333333f));
		integration_points_.push_back(Vector3(-0.127322f,  0.934172f, -0.333333f));
		integration_points_.push_back(Vector3(-0.127322f, -0.934172f, -0.333333f));
		integration_points_.push_back(Vector3(-0.745356f, -0.57735f,  -0.333333f));
		integration_points_.push_back(Vector3( 0.333333f,  0.57735f,  -0.745356f));
		integration_points_.push_back(Vector3( 0.333333f, -0.57735f,  -0.745356f));
		integration_points_.push_back(Vector3(-0.666667f,  0.0f,      -0.745356f));
		integration_points_.push_back(Vector3( 0.0f,       0.0f,      -1.0f));
		Log.info() << "done." << std::endl;
		return(true);
	}

	bool GeneralizedBorn::setupIntegrationWeights_()
	{
		Log.info() << "Setting up integration weights...";
		float factor = - 1.0f / (80.0f * Constants::PI);
		float rk_rec;
		float rk1_rec;
		float weight_CFA;
		float weight_CT;

		// Resize the vectors needed for storing the integration weights
		integration_weights_CFA_.clear();
		integration_weights_CT_.clear();
		integration_weights_CFA_.resize(shell_radii_.size() - 1);
		integration_weights_CT_.resize(shell_radii_.size() - 1);

		// calculate the integration weights for all shells
		for (Size shell_index = 0; shell_index < shell_radii_.size() - 1;
				++shell_index)
		{
			rk_rec = 1.0f/shell_radii_[shell_index];
			rk1_rec = 1.0f/shell_radii_[shell_index + 1];
			weight_CFA = factor * (rk_rec - rk1_rec);
			weight_CT = 0.5f * factor 
				* (rk_rec * rk_rec - rk1_rec * rk1_rec);
			integration_weights_CFA_[shell_index] = weight_CFA;
			integration_weights_CT_[shell_index] = weight_CT;
		}
		Log.info() << "done." << std::endl;
		return(true);
	}


	float GeneralizedBorn::integrate_(const Atom* atom)
	{
		// The two contributions, CFA and non-CFA correction term
		float G0_i = 0.0f;
		float G1_i = 0.0f;

#ifdef DEBUG
		Molecule ip;
#endif

		// Only count atoms that have non-vanishing radius. Be sure to condense
		// charged hydrogens with zero radius (like those of the PARSE
		// parameter set) onto the heavy atoms they are connected to.
		if (atom->getRadius() > 0.0)
		{
			float sum_CFA = 0.0f;
			float sum_CT = 0.0f;

			for (Size shell_index = 0; shell_index < shell_radii_.size() - 1;
					++shell_index)
			{
				Vector3 r;
				double V;
				for (Size point_index = 0; point_index < integration_points_.size();
						++point_index)
				{
					r = atom->getPosition() 
						+ shell_radii_[shell_index] * integration_points_[point_index];
					V = calculateAtomicVolumeContribution_(r);
					sum_CFA += integration_weights_CFA_[shell_index] * V;
					sum_CT += integration_weights_CT_[shell_index] * V;
#ifdef DEBUG
					cout << "V" << r << " [" << shell_index << "/" << point_index 
						<< "]: " << V << endl;

					Atom* point = new Atom;
					point->setPosition(r);
					point->setElement(PTE[Element::Fe]);
					point->setCharge(V);
					point->setName("IP" + String(shell_index) + "/" 
							+ String(point_index));
					ip.insert(*point);
#endif
				}
			}
			// Splitting up G into several variables is for debugging purposes.
			// Should be changed as soon as the code functions.
			G0_i += nearest_radius_rec_[&*atom] - sum_CFA;
			G1_i += sqrt(0.5f * nearest_radius_rec_[&*atom] 
					* nearest_radius_rec_[&*atom] - sum_CT);
		}

#ifdef DEBUG
		HINFile ip_file("integration_points.hin", std::ios::out);
		ip_file << ip;
		ip_file.close();
#endif

		return(- G0_i + P_ * G1_i);

	}


	void GeneralizedBorn::dump(ostream& os)
	{
		os << "k_: " << k_ << endl;
		os << "k_prime_: " << k_prime_ << endl;
		os << "eps_solute_: " << eps_solute_ << endl;
		os << "eps_solvent_: " << eps_solvent_ << endl;
		os << endl;
		os << "beta_: " << beta_ << endl;
		os << "lambda_: " << lambda_ << endl;
		os << "gamma_0_: " << gamma_0_ << endl;
		os << "P_: " << P_ << endl;
		os << "alpha_0_: " << alpha_0_ << endl;
		os << endl;
		os << "epsilon_: " << epsilon_ << endl;
		os << "grid_spacing_: " << grid_spacing_ << endl;
		os << "r_buffer_: " << r_buffer_ << endl;
		os << "C_: " << C_ << endl;
		os << "D_: " << D_ << endl;
		os << endl;
		if (atom_container_ != 0)
		{
			os << "Atoms: " << atom_container_->countAtoms() << endl;
		}
		else
		{
			os << "No atom container." << endl;
		}
		os << "Lookup grid: " << lookup_grid_.getSizeX() << " x " 
			<< lookup_grid_.getSizeY() << " x " << lookup_grid_.getSizeZ() << " @ "
			<< lookup_grid_.getOrigin() << endl;
		os << "Helper grid: " << helper_grid_.getSizeX() << " x " 
			<< helper_grid_.getSizeY() << " x " << helper_grid_.getSizeZ() << " @ "
			<< helper_grid_.getOrigin() << endl;
		os << "Shell radii: ";
		for (Size i = 0; i < shell_radii_.size(); ++i)
		{
			os << shell_radii_[i] << " ";
		}
		os << endl;
		os << "Integration points: " << integration_points_.size() << endl;
		os << "Integration weights CFA: ";
		for (Size i = 0; i < integration_weights_CFA_.size(); ++i)
		{
			os << integration_weights_CFA_[i] << ", ";
		}
		os << endl;
		os << "Integration weights CT: ";
		for (Size i = 0; i < integration_weights_CT_.size(); ++i)
		{
			os << integration_weights_CT_[i] << ", ";
		}
		os << endl;
		os << "Nearest radius map: " << nearest_radius_rec_.size() << endl;
	}

}
