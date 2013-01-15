// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/DOCKING/COMMON/sideChainOptimizer.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/molecularInteractions.h>
#include <BALL/QSAR/ringPerceptionProcessor.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMMON/linearBaseFunction.h>
#include <BALL/SCORING/COMMON/fermiBaseFunction.h>
#include <BALL/SYSTEM/path.h>


using namespace std;
using namespace BALL;


const char* ScoringFunction::Option::VERBOSITY = "verbosity";
const char* ScoringFunction::Option::BASE_FUNCTION_TYPE= "base_function_type";
const char* ScoringFunction::Option::HASHGRID_RESOLUTION = "hashgrid_resolution";
const char* ScoringFunction::Option::HASHGRID_SIZE = "hashgrid_size";
const char* ScoringFunction::Option::NONBONDED_CUTOFF = "nonbonded_cutoff";
const char* ScoringFunction::Option::CONFORMATION_SCALE = "conformation_scale";
const char* ScoringFunction::Option::ALL_LIG_NONB_PAIRS = "all_lig_nonb_pairs";
const char* ScoringFunction::Option::USE_STATIC_LIG_FRAGMENTS="use_static_lig_fragments";
const char* ScoringFunction::Option::IGNORE_H_CLASHES="ignore_H_clashes";
const char* ScoringFunction::Option::ALLOWED_INTERMOL_OVERLAP="allowed_intermolecular_overlap";
const char* ScoringFunction::Option::ALLOWED_INTRAMOL_OVERLAP="allowed_intramolecular_overlap";
const char* ScoringFunction::Option::BURIAL_DEPTH_SCALE="burial_depth_scale";

const Size ScoringFunction::Default::VERBOSITY = 0;
const Size ScoringFunction::Default::BASE_FUNCTION_TYPE = ScoringBaseFunction::BASE_FUNCTION_TYPE__FERMI;
int ScoringFunction::Default::HASHGRID_SIZE = 10;
int ScoringFunction::Default::BURIAL_DEPTH_SCALE = 1;
double ScoringFunction::Default::HASHGRID_RESOLUTION = 3;
double ScoringFunction::Default::NONBONDED_CUTOFF = 20;
double ScoringFunction::Default::CONFORMATION_SCALE = 0.25;
double ScoringFunction::Default::ALLOWED_INTERMOL_OVERLAP = 1.0;
double ScoringFunction::Default::ALLOWED_INTRAMOL_OVERLAP = 1.0;
bool ScoringFunction::Default::ALL_LIG_NONB_PAIRS = true;
bool ScoringFunction::Default::USE_STATIC_LIG_FRAGMENTS = true;
bool ScoringFunction::Default::IGNORE_H_CLASHES = true;


ScoringFunction::ScoringFunction()
	:	name_(""),
		options_(),
		receptor_(0),
		ligand_(0),
		score_(0.0),
		intercept_(0.0),
		base_function_(0),
		scoring_components_(),
		unassigned_atoms_(),
		max_number_of_errors_(0),
		number_of_errors_(0)
{
	//setDefaults();
	setup();
}


ScoringFunction::ScoringFunction(const ScoringFunction& sf)
	:	name_(sf.name_),
		options_(sf.options_),
		receptor_(sf.receptor_),
		ligand_(sf.ligand_),
		score_(sf.score_),
		intercept_(sf.intercept_),
		base_function_(sf.base_function_),
		scoring_components_(sf.scoring_components_),
		unassigned_atoms_(),
		max_number_of_errors_(sf.max_number_of_errors_),
		number_of_errors_(sf.number_of_errors_)
{
	// An instance of ScoringFunction created with this copy constructor
	// will use the same (!) components as the original one. Only the
	// pointers will be copied from the original instance.
	setup();
}


ScoringFunction::ScoringFunction(AtomContainer& receptor, AtomContainer& ligand,
				 const Options& options)
	:	name_(""),
		options_(options),
		receptor_(&receptor),
		ligand_(&ligand),
		score_(0.0),
		intercept_(0.0),
		base_function_(0),
		scoring_components_(),
		unassigned_atoms_(),
		max_number_of_errors_(0),
		number_of_errors_(0)
{
	setup();
}


ScoringFunction::ScoringFunction(AtomContainer& receptor, AtomContainer& ligand, Options& options)
{
	receptor_ = &receptor;
	ligand_ = &ligand;

	Options* option_category = options.getSubcategory("Scoring Function");
	if (!option_category) option_category = &options;
	options_ = *option_category;

	//--- to be done for each new ligand candidate ------
	ligand_center_ = calculateGeometricalCenter(ligand_, &ligand_atoms_);
	ligand_radius_ = calculateMoleculeRadius(ligand_, ligand_center_);
	// ------------------------

	ScoringFunction::setup(); // set default values

	int hashgrid_size = options_.setDefaultInteger(Option::HASHGRID_SIZE, Default::HASHGRID_SIZE);
	hashgrid_ = initializeHashGrid(receptor_, ligand_center_, resolution_, hashgrid_size);
	setupReferenceLigand();
	setupFlexibleResidues_();
}


ScoringFunction::ScoringFunction(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
{
	receptor_ = &receptor;
	ligand_ = NULL;

	Options* option_category = options.getSubcategory("Scoring Function");
	if (!option_category) option_category = &options;
	options_ = *option_category;

	ScoringFunction::setup(); // set default values

	int hashgrid_size = options_.setDefaultInteger(Option::HASHGRID_SIZE, Default::HASHGRID_SIZE);
	hashgrid_ = initializeHashGrid(receptor_, hashgrid_origin, resolution_, hashgrid_size);
	setupFlexibleResidues_();
}


ScoringFunction::~ScoringFunction()
{
	clear();
}


void ScoringFunction::clear()
{
	options_.clear();

	// Don't delete the system or the molecules, that's not the job of a
	// scoring function
	receptor_ = 0;
	ligand_ = 0;

	name_ = String("");
	score_ = 0.0;
	intercept_ = 0.0;

	unassigned_atoms_.clear();
	max_number_of_errors_ = 0;
	number_of_errors_ = 0;

	// The base function was (hopefully) assigned by the setup() method, do
	// we have to delete it here.
	if (base_function_ != 0)
	{
		delete base_function_;
		base_function_ = 0;
	}

	if (hashgrid_ != 0)
	{
		delete hashgrid_;
		hashgrid_ = 0;
	}

	if (flexible_residues_hashgrid_ != 0)
	{
		delete flexible_residues_hashgrid_;
		flexible_residues_hashgrid_ = 0;
	}

	if (all_residues_hashgrid_ != 0)
	{
		delete all_residues_hashgrid_;
		all_residues_hashgrid_ = 0;
	}

	for (Size i = 0; i < static_ligand_fragments_.size(); i++)
	{
		delete static_ligand_fragments_[i];
	}
	static_ligand_fragments_.clear();

	for (Size i=0; i < scoring_components_.size(); i++)
	{
		delete scoring_components_[i];
	}
	scoring_components_.clear();
}


ScoringFunction& ScoringFunction::operator = (const ScoringFunction& sf)
{
	options_ = sf.options_;
	name_ = sf.name_;
	score_ = sf.score_;
	intercept_ = sf.intercept_;
	scoring_components_ = sf.scoring_components_;
	unassigned_atoms_ = sf.unassigned_atoms_;
	max_number_of_errors_ = sf. max_number_of_errors_;
	number_of_errors_ = sf.number_of_errors_;

	return(*this);
}


bool ScoringFunction::setup()
{
	// Get defaults from the options
	Size base_function_type
		= options_.setDefaultInteger(ScoringFunction::Option::BASE_FUNCTION_TYPE,
					    ScoringFunction::Default::BASE_FUNCTION_TYPE);

	// Check the environment
	if (receptor_ == 0 || ligand_ == 0)
	{
		Log.error() << "ScoringFunction::setup(): No system or molecules defined"
			<< std::endl;
		return(false);
	}

	// A base function will be assigned in any case.
	if (base_function_type == ScoringBaseFunction::BASE_FUNCTION_TYPE__LINEAR)
	{
		base_function_ = new LinearBaseFunction;
	}
	else
	{
		if (base_function_type == ScoringBaseFunction::BASE_FUNCTION_TYPE__FERMI)
		{
			base_function_ = new FermiBaseFunction;
		}
	}

	store_interactions_ = false;
	store_interactions_phC_only_ = false;
	all_ligand_nonbonded_ = NULL;
	resolution_ = options_.setDefaultReal(Option::HASHGRID_RESOLUTION, Default::HASHGRID_RESOLUTION);
	nonbonded_cutoff_ = options_.setDefaultReal(Option::NONBONDED_CUTOFF, Default::NONBONDED_CUTOFF);
	nonbonded_cutoff_2_ = nonbonded_cutoff_*nonbonded_cutoff_;
	conformation_scale_ = options_.setDefaultReal(Option::CONFORMATION_SCALE, Default::CONFORMATION_SCALE);
	reference_neighbors_ = 0;
	hashgrid_search_radius_ = (int) ceil(nonbonded_cutoff_/resolution_);
	use_all_lig_nonb_ = options_.setDefaultBool(Option::ALL_LIG_NONB_PAIRS, Default::ALL_LIG_NONB_PAIRS);
	use_static_lig_fragments_ = options_.setDefaultBool(Option::USE_STATIC_LIG_FRAGMENTS, Default::USE_STATIC_LIG_FRAGMENTS);
	neighbor_cutoff_2_ = 16;
	burial_depth_scale_ = options_.setDefaultInteger(Option::BURIAL_DEPTH_SCALE, Default::BURIAL_DEPTH_SCALE);

	// If nonbonded-parameters are not set, set them to senseful default values
	options_.setDefaultReal("electrostatic_cutoff", 20.0);
	options_.setDefaultReal("electrostatic_cuton", 17.000000);
	options_.setDefaultReal("vdw_cutoff", 20.000000);
	options_.setDefaultReal("vdw_cuton", 17.000000);

	String iniFile;
	if (options_.get("filename") != "")
	{
		iniFile = options_.get("filename");
	}
	else
	{
		iniFile = "Amber/amber96-docking.ini";
	}
	Path path;
	String fullpath = path.find(iniFile);
	if (fullpath == "")
	{
		throw BALL::Exception::FileNotFound(__FILE__, __LINE__, iniFile);
	}
	options_.setDefault("filename", fullpath);

	//--- determine what to consider as sterical clashes ----
	ignore_h_clashes_ = options_.setDefaultBool(Option::IGNORE_H_CLASHES, Default::IGNORE_H_CLASHES);
	allowed_intermolecular_overlap_ = options_.setDefaultReal(Option::ALLOWED_INTERMOL_OVERLAP, Default::ALLOWED_INTERMOL_OVERLAP);
	allowed_intramolecular_overlap_ = options_.setDefaultReal(Option::ALLOWED_INTRAMOL_OVERLAP, Default::ALLOWED_INTRAMOL_OVERLAP);
	// ------------------------

	exp_energy_stddev_ = 0;
	exp_energy_mean_ = 0;
	flexible_residues_hashgrid_ = NULL;
	all_residues_hashgrid_ = NULL;

	return true;
}

bool ScoringFunction::setup(AtomContainer& receptor, AtomContainer& ligand)
{
	setReceptor(receptor);
	setLigand(ligand);

	return(setup());
}


bool ScoringFunction::setup(AtomContainer& receptor, AtomContainer& ligand,
			    const Options& options)
{
	setReceptor(receptor);
	setLigand(ligand);
	options_ = options;

	return setup();
}


void ScoringFunction::setMaximumNumberOfErrors(Size nr)
{
	max_number_of_errors_ = nr;
}


Size ScoringFunction::getMaximumNumberOfErrors() const
{
	return(max_number_of_errors_);
}


void ScoringFunction::setFirstMolecule(AtomContainer& molecule)
{
	receptor_ = &molecule;
}


AtomContainer* ScoringFunction::getFirstMolecule() const
{
	return(receptor_);
}


void ScoringFunction::setSecondMolecule(AtomContainer& molecule)
{
	ligand_ = &molecule;
}


AtomContainer* ScoringFunction::getSecondMolecule() const
{
	return(ligand_);
}


void ScoringFunction::setReceptor(AtomContainer& molecule)

{
	setFirstMolecule(molecule);
}


AtomContainer* ScoringFunction::getReceptor() const

{
	return(getFirstMolecule());
}


void ScoringFunction::setLigand(AtomContainer& molecule)
{
	setSecondMolecule(molecule);
	atoms_to_fragments_.clear();

	ligand_ = &molecule;
	if (store_interactions_)
	{
		clearStoredInteractions_();
		if (store_interactions_phC_only_)
		{
			for (AtomIterator a_it = ligand_->beginAtom(); +a_it; a_it++)
			{
				a_it->disableStoreInteractions();
			}
		}
	}

	ligand_center_ = calculateGeometricalCenter(ligand_, &ligand_atoms_);
	ligand_radius_ = calculateMoleculeRadius(ligand_, ligand_center_);

	if (use_static_lig_fragments_)
	{
		createStaticLigandFragments();
	}

	if (use_all_lig_nonb_)
	{
		createAllLigandNonBondedPairs();
	}

	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		scoring_components_[i]->setupLigand();
	}

	resetFlexibleResidues();
}


AtomContainer* ScoringFunction::getLigand() const

{
	return getSecondMolecule();
}


void ScoringFunction::setIntercept(double intercept)

{
	intercept_ = intercept;
}


double ScoringFunction::getIntercept() const

{
	return intercept_;
}


ScoringBaseFunction* ScoringFunction::getBaseFunction() const
{
	return base_function_;
}


void ScoringFunction::insertComponent(ScoringComponent* component)
{
	scoring_components_.push_back(component);
}


void ScoringFunction::insertComponent(ScoringComponent* component,
				      float coefficient)
{
	component->setCoefficient(coefficient);
	scoring_components_.push_back(component);
}


void ScoringFunction::removeComponent(const ScoringComponent* component)
{
	std::vector<ScoringComponent*>::iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if (*it == component)
		{
			delete (*it);
			scoring_components_.erase(it);

			break;
		}
	}
}


void ScoringFunction::removeComponent(const String& name)
{
	std::vector<ScoringComponent*>::iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			delete (*it);
			scoring_components_.erase(it);

			break;
		}
	}
}


void ScoringFunction::setCoefficient(const ScoringComponent* component,
				     float coefficient)

{
	std::vector<ScoringComponent*>::iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it) == component)
		{
			(*it)->setCoefficient(coefficient);

			break;
		}
	}
}


void ScoringFunction::setCoefficient(const String& name,
				     float coefficient)
{
	std::vector<ScoringComponent*>::iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			(*it)->setCoefficient(coefficient);

			break;
		}
	}
}


bool ScoringFunction::getCoefficient(const ScoringComponent* component,
				     float& coefficient) const
{
	std::vector<ScoringComponent*>::const_iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it) == component)
		{
			coefficient = (*it)->getCoefficient();;

			return true;
		}
	}

	return false;
}


bool ScoringFunction::getCoefficient(const String& name,
				     float& coefficient) const
{
	std::vector<ScoringComponent*>::const_iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			coefficient = (*it)->getCoefficient();

			return true;
		}
	}

	return false;
}


ScoringComponent* ScoringFunction::getComponent(const String& name) const
{
	std::vector<ScoringComponent*>::const_iterator it;
	for (it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it)->getName() == name)
		{
			return (*it);
		}
	}

	return 0;
}


ScoringComponent* ScoringFunction::getComponent(const Size index) const
{
	if (index < scoring_components_.size())
	{
		return scoring_components_[index];
	}
	else
	{
		return 0;
	}
}


double ScoringFunction::calculateScore()
{
	double score_ = 0.0;

	for (vector<ScoringComponent*>::iterator it = scoring_components_.begin(); it!=scoring_components_.end(); it++)
	{
		(*it)->updateScore();
		score_ += (*it)->getScaledScore();

		//score_ += (*it)->updateScore();
	}

	return (intercept_ + score_);
}


const HashSet<const Atom*>& ScoringFunction::getUnassignedAtoms() const
{
	return unassigned_atoms_;
}


HashSet<const Atom*>& ScoringFunction::getUnassignedAtoms()
{
	return unassigned_atoms_;
}


Vector3 ScoringFunction::calculateGeometricalCenter(AtomContainer* s, int* no_ligand_atoms)
{
	Vector3 ligand_center(0, 0, 0);
	int no = 0;
	for (AtomIterator atom_it = s->beginAtom(); !atom_it.isEnd(); ++atom_it)
	{
		ligand_center += atom_it->getPosition();
		no++;
	}
	if (no == 0)
	{
		throw Exception::GeneralException(__FILE__, __LINE__, "ScoringFunction::calculateGeometricalCenter(ligand_) Error", "There are no atoms within the given AtomContainer!");
	}
	if (no_ligand_atoms != NULL)
	{
		*no_ligand_atoms = no;
	}

	return ligand_center/no;
}


double ScoringFunction::calculateMoleculeRadius(AtomContainer* sys, Vector3& center)
{
	double max_dist_2 = -1e10;
	for (AtomIterator atom_it = sys->beginAtom(); !atom_it.isEnd(); ++atom_it)
	{
		Vector3 v = center-atom_it->getPosition();
		double dist_2 = v[0]*v[0]+v[1]*v[1]+v[2]*v[2];
		if (dist_2 > max_dist_2)
		{
			max_dist_2 = dist_2;
		}
	}

	return sqrt(max_dist_2);
}


HashGrid3<Atom*>* ScoringFunction::initializeHashGrid(AtomContainer* sys, Vector3& center, double& resolution, int hashgrid_size)
{
	double radius_in_angstroem = ((double)hashgrid_size)/2*resolution;
	Vector3 v(radius_in_angstroem, radius_in_angstroem, radius_in_angstroem);
	Vector3 origin = center-v;
	HashGrid3<Atom*>* hashgrid = new HashGrid3<Atom*>(origin, hashgrid_size, hashgrid_size, hashgrid_size, resolution);

	Size size = 0; Size inserted = 0;
	bool use_selection = sys->containsSelection();

	for (AtomIterator atom_it = sys->beginAtom(); !atom_it.isEnd(); ++atom_it)
	{
		// if receptor contains selections, do not use unselected atoms for calculation of receptor-ligand interactions!
		// --> Caution: Also no check for clashes with those unselected atoms!!
		if (use_selection && !atom_it->isSelected())
		{
			continue;
		}

		++size;

		// position of the current atoms within the HashGrid
		Vector3 atom_pos = (atom_it->getPosition() - origin);

		atom_pos[0] /= hashgrid->getUnit()[0];
		atom_pos[1] /= hashgrid->getUnit()[1];
		atom_pos[2] /= hashgrid->getUnit()[2];

		// insert all target atoms that are located within the grid boundaries
		if (atom_pos[0] >= 0 && atom_pos[0] < hashgrid_size && atom_pos[1] >= 0 && atom_pos[1] < hashgrid_size && atom_pos[2] >= 0 && atom_pos[2] < hashgrid_size )
		{
			hashgrid->insert(atom_it->getPosition(), &*atom_it);
			inserted++;
		}
	}

	//Log<<"inserted "<<inserted<<" atoms into "<<hashgrid->countNonEmptyBoxes()<<" hashgrid boxes"<<endl;

	return hashgrid;
}


void ScoringFunction::setupReferenceLigand()
{
	atoms_to_fragments_.clear();
	// estimate depth of burial of the reference ligand
	overlaps_ = 0;
	AtomPairVector* tmp = createNonbondedPairVector(hashgrid_, overlaps_, 1);
	delete tmp; // This vector is not used anywhere, delete it!

	reference_neighbors_ = neighboring_target_atoms_;

	//	if(use_static_lig_fragments_) createStaticLigandFragments();
	//	if(use_all_lig_nonb_) createAllLigandNonBondedPairs();
}


void ScoringFunction::createAllLigandNonBondedPairs()
{
	delete all_ligand_nonbonded_;
	all_ligand_nonbonded_ = NULL;

	//if (ligand_atoms_ < 200)
	{
		double nonbonded_cutoff_2_backup = nonbonded_cutoff_2_;
		nonbonded_cutoff_2_ = 999999; // calculate all nonbonded pairs of the current ligand (no cutoff)
		int overlaps = 0;
		all_ligand_nonbonded_ = createLigandNonbondedPairVector(0, overlaps);
		nonbonded_cutoff_2_ = nonbonded_cutoff_2_backup;
	}

	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if ((*it)->isLigandIntraMolecular())
		{
			(*it)->update(*all_ligand_nonbonded_);
		}
	}
}


AtomPairVector* ScoringFunction::createNonbondedPairVector(HashGrid3<Atom*>* hashgrid, int& overlaps, bool rec_lig, bool check_fragment, bool intra_fragment)
{
	misplaced_ligand_atoms_ = 0;
	neighboring_target_atoms_ = 0;

	AtomPairVector* nonbonded_pairs = new AtomPairVector;
	nonbonded_pairs->reserve(ligand_atoms_*100);
	int ligand_atoms = 0;

	int x_size = (int)hashgrid->getSizeX();
	int y_size = (int)hashgrid->getSizeY();
	int z_size = (int)hashgrid->getSizeZ();
	bool use_selection = ligand_->containsSelection();

	/// find receptor atoms that could interact with the current ligand atom
	for (AtomIterator atom_it = ligand_->beginAtom(); !atom_it.isEnd(); ++atom_it)
	{
		// if ligand contains selections, do not use unselected atoms for calculation of receptor-ligand interactions!
		// --> Caution: Also no check for clashes with those unselected atoms!!
		if (use_selection && !atom_it->isSelected())
		{
			continue;
		}

		ligand_atoms++;

		// calculate position of current ligand atom in hashgrid
		Vector3 lig_atom_pos = (atom_it->getPosition() - hashgrid->getOrigin());
		lig_atom_pos.x /= hashgrid->getUnit().x;
		lig_atom_pos.y /= hashgrid->getUnit().y;
		lig_atom_pos.z /= hashgrid->getUnit().z;

		// treat atoms outside of the interaction grid as putative sterical clashes
		if (lig_atom_pos.x < 0 || lig_atom_pos.x > x_size || lig_atom_pos.y < 0 || lig_atom_pos.y > y_size || lig_atom_pos.z < 0 || lig_atom_pos.z > z_size )
		{
			misplaced_ligand_atoms_++;
			continue;
		}

		// indices in HashGrid, where the search for interacting target atoms should begin ( != position of ligand atom)
		int i = static_cast < int > (lig_atom_pos.x-hashgrid_search_radius_); if (i < 0){i = 0; }
		int j0 = static_cast < int > (lig_atom_pos.y-hashgrid_search_radius_); if (j0 < 0){j0 = 0; }
		int k0 = static_cast < int > (lig_atom_pos.z-hashgrid_search_radius_); if (k0 < 0){k0 = 0; }

		/** find all atoms in neighboring boxes (depth = hashgridsearch_radius_) around the current ligand atom  */
		for (; i <= lig_atom_pos.x+hashgrid_search_radius_ && i < x_size; i++)
		{
			for (int j = j0; j <= lig_atom_pos.y+hashgrid_search_radius_ && j < y_size; j++)
			{
				for (int k = k0; k <= lig_atom_pos.z+hashgrid_search_radius_ && k < z_size; k++)
				{
					HashGridBox3<Atom*>* box = hashgrid->getBox(i, j, k);

					// ... for all atoms in that box
					for (HashGridBox3 < Atom* > ::DataIterator di = box->beginData(); di != box->endData(); di++)
					{
						Vector3 d = atom_it->getPosition() - (*di)->getPosition();
						double distance_2 = d.getSquareLength();

						if (distance_2 < nonbonded_cutoff_2_)
						{
							if (rec_lig)
							{
								// explicit check for sterical clash
								if (!ignore_h_clashes_ || (atom_it->getElement().getAtomicNumber() != 1 && (*di)->getElement().getAtomicNumber() != 1))
								{
									double radii = atom_it->getElement().getVanDerWaalsRadius() + (*di)->getElement().getVanDerWaalsRadius();
									double dist = sqrt(distance_2);

									if (radii >= dist+allowed_intermolecular_overlap_)
									{
										overlaps++;
									}
								}

								nonbonded_pairs->push_back(make_pair(&*atom_it, *di));
							}

							else
							{
								bool b = 0;
								if (!check_fragment) b = 1;
								else
								{
									map<Atom*, int>::iterator map_it1 = atoms_to_fragments_.find(&*atom_it);
									map<Atom*, int>::iterator map_it2 = atoms_to_fragments_.find(*di);
									if (map_it1 != atoms_to_fragments_.end() && map_it2 != atoms_to_fragments_.end())
									{
										if (!intra_fragment)
										{
											if (map_it1->second != map_it2->second)
											{
												b = 1;
											}
										}
										else
										{
											if (map_it1->second == map_it2->second)
											{
												b = 1;
											}
										}
									}
								}
								if (b)
								{
									if (!atom_it->isBoundTo(**di) && !atom_it->isGeminal(**di) &&  &*atom_it != &**di)
									{

										if (!ignore_h_clashes_ || (atom_it->getElement().getAtomicNumber() != 1 && (*di)->getElement().getAtomicNumber() != 1))
										{
											double radii = atom_it->getElement().getVanDerWaalsRadius() + (*di)->getElement().getVanDerWaalsRadius();
											double dist = sqrt(distance_2);
											if (radii >= dist+allowed_intermolecular_overlap_)
											{
												overlaps++;
											}
										}

										nonbonded_pairs->push_back(make_pair(&*atom_it, *di));
									}
								}
							}
						}
						if (distance_2 < neighbor_cutoff_2_)
						{
							neighboring_target_atoms_++;
						}
					}
				}
			}
		}
	}

	//Log<<"found "<<misplaced_ligand_atoms_<<" atoms outside of hashgrid !!"<<endl;
	if (rec_lig)
	{
		neighboring_target_atoms_ /= ligand_atoms;
	}

	return nonbonded_pairs;
}


double ScoringFunction::getExpEnergyStddev()
{
	return exp_energy_stddev_;
}


int ScoringFunction::checkForAtomOverlaps(const AtomPairVector* pair_vector)
{
	Size overlaps = 0;
	for (AtomPairVector::const_iterator it = pair_vector->begin(); it != pair_vector->end(); it++)
	{
		double allowed_overlap = allowed_intermolecular_overlap_;
		bool intramol = (it->first->getMolecule() == it->second->getMolecule());
		if (intramol)
		{
			allowed_overlap = allowed_intramolecular_overlap_;
		}

		if (!ignore_h_clashes_ || (it->first->getElement().getSymbol() != "H" && it->second->getElement().getSymbol() != "H"))
		{
			double dist = it->first->getPosition().getDistance(it->second->getPosition());
			double radii = it->first->getElement().getVanDerWaalsRadius() + it->second->getElement().getVanDerWaalsRadius();
			if (radii >= dist+allowed_overlap)
			{
				overlaps++;
				//Log<<it->first->getFullName()<<", "<<it->second->getFullName()<<" : "<<dist<<endl;
				//Log<<"vdw-radii: "<<it->first->getElement().getVanDerWaalsRadius()<<", "<<it->second->getElement().getVanDerWaalsRadius()<<endl;
			}
		}
	}

	return overlaps;
}


AtomPairVector* ScoringFunction::createLigandNonbondedPairVector(bool intra_fragment, int& overlaps)
{
	AtomPairVector* nonbonded_pairs = NULL;
	bool check_fragment = 0;
	bool use_selection = ligand_->containsSelection();
	if (static_ligand_fragments_.size() != 0)
	{
		check_fragment = 1;
	}

// 	if(!intra_fragment && ligand_atoms_>300)
// 	{
// 		// set size large enough to include all ligand atoms interactions...
// 		int size = (int)(ceil(ligand_radius_*2/resolution_)+1);
// 		ligand_center_ = calculateGeometricalCenter(ligand_);
// 		ligand_radius_ = calculateMoleculeRadius(ligand_,ligand_center_);
//
// 		// create new temporary HashGrid for the current ligand pose only!
// 		// Since the ligand will change its conformation during docking
// 		// we cannot keep one static HashGrid (as it is possible to do for the receptor)
// 		HashGrid3<Atom*>* temp_grid = initializeHashGrid(ligand_,ligand_center_,resolution_,size);
//
// 		int overlaps=0;
// 		nonbonded_pairs = createNonbondedPairVector(temp_grid,overlaps,0,check_fragment,intra_fragment);
// 		delete temp_grid;
// 	}
	if (!check_fragment)
	{
		nonbonded_pairs = new AtomPairVector;
		nonbonded_pairs->reserve((ligand_atoms_*ligand_atoms_)/2);
		neighboring_target_atoms_ = 0;

		for (AtomIterator atom_it1 = ligand_->beginAtom(); +atom_it1; ++atom_it1)
		{
			if (use_selection && !atom_it1->isSelected()) continue;

			AtomIterator atom_it2 = atom_it1;
			atom_it2++;			// check each pair only once!
			for (; +atom_it2; ++atom_it2)
			{
				if (use_selection && !atom_it2->isSelected()) continue;

				Vector3 d = atom_it1->getPosition() - atom_it2->getPosition();
				double distance_2 = pow(d[0], 2)+pow(d[1], 2)+pow(d[2], 2);

				if (distance_2 < nonbonded_cutoff_2_)
				{
					if (!atom_it1->isBoundTo(*atom_it2) && !atom_it1->isGeminal(*atom_it2) &&  *atom_it1 != *atom_it2)
					{
						if (!ignore_h_clashes_ || (atom_it1->getElement().getSymbol() != "H" && atom_it2->getElement().getSymbol() != "H"))
						{
							double radii = atom_it1->getElement().getVanDerWaalsRadius() + atom_it2->getElement().getVanDerWaalsRadius();
							double dist = sqrt(distance_2);
							if (radii >= dist+allowed_intramolecular_overlap_)
							{
								overlaps++;
							}
						}

						nonbonded_pairs->push_back(make_pair(&*atom_it1, &*atom_it2));
					}
				}
			}
		}
	}
	else if (!intra_fragment)
	{
		nonbonded_pairs = new AtomPairVector;
		nonbonded_pairs->reserve((ligand_atoms_*ligand_atoms_)/2);
		neighboring_target_atoms_ = 0;

		for (Size i = 0; i < static_ligand_fragments_.size(); i++)
		{
			for (Size j = i+1; j < static_ligand_fragments_.size(); j++)
			{
				if (i != j) // do not check intra-fragment interactions
				{
					for (list < Atom* > ::iterator it_i = static_ligand_fragments_[i]->atoms.begin();
									       it_i!=static_ligand_fragments_[i]->atoms.end(); ++it_i)
					{
						if (use_selection && !(*it_i)->isSelected()) continue;

						for (list < Atom* > ::iterator it_j = static_ligand_fragments_[j]->atoms.begin();
										it_j!=static_ligand_fragments_[j]->atoms.end(); ++it_j)
						{
							if (use_selection && !(*it_j)->isSelected()) continue;

							Vector3 d = (*it_i)->getPosition() - (*it_j)->getPosition();
							double distance_2 = pow(d[0], 2)+pow(d[1], 2)+pow(d[2], 2);

							if (distance_2 < nonbonded_cutoff_2_)
							{
								if ( !(*it_i)->isBoundTo(**it_j) && !(*it_i)->isGeminal(**it_j) && *it_i != *it_j)
								{
									if (!ignore_h_clashes_ || ((*it_i)->getElement().getSymbol() != "H" && (*it_j)->getElement().getSymbol() != "H"))
									{
										double radii = (*it_i)->getElement().getVanDerWaalsRadius() + (*it_j)->getElement().getVanDerWaalsRadius();
										double dist = sqrt(distance_2);
										if (radii >= dist+allowed_intramolecular_overlap_)
										{
											overlaps++;
										}
									}

									nonbonded_pairs->push_back(make_pair(&**it_i, &**it_j));
								}
							}
						}
					}
				}
			}
		}
	}
	else if (intra_fragment)
	{
		nonbonded_pairs = new AtomPairVector;
		nonbonded_pairs->reserve((ligand_atoms_*ligand_atoms_)/2);
		neighboring_target_atoms_ = 0;

		for (Size i = 0; i < static_ligand_fragments_.size(); i++) // examine all intra-fragment pairs
		{
			for (list < Atom* > ::iterator it_1 = static_ligand_fragments_[i]->atoms.begin();
						 it_1!=static_ligand_fragments_[i]->atoms.end(); ++it_1)
			{
				if (use_selection && !(*it_1)->isSelected()) continue;

				list<Atom*>::iterator it_2 = it_1;
				it_2++;  // check each pair only once!
				for (; it_2 != static_ligand_fragments_[i]->atoms.end(); ++it_2)
				{
					if (use_selection && !(*it_2)->isSelected()) continue;

					Vector3 d = (*it_1)->getPosition() - (*it_2)->getPosition();
					double distance_2 = pow(d[0], 2)+pow(d[1], 2)+pow(d[2], 2);

					if (distance_2 < nonbonded_cutoff_2_)
					{
						if ( !(*it_1)->isBoundTo(**it_2) && !(*it_1)->isGeminal(**it_2) && *it_1 != *it_2)
						{
							// within a StaticLigandFragment atoms may have a small distance to each other (e.g. to due to ring electron orbitals), so do NOT check for overlaps here!
							nonbonded_pairs->push_back(make_pair(&**it_1, &**it_2));
						}
					}
				}
			}
		}
	}

	return nonbonded_pairs;
}


bool ScoringFunction::hasPharmacophoreConstraints_()
{
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		if ((*it)->getType() == "PharmacophoreConstraint")
		{
			return true;
		}
	}

	return false;
}


void ScoringFunction::enableStoreInteractionsOnlyForPhContraints()
{
	store_interactions_ = 1;
	store_interactions_phC_only_ = 1;

	// speed-up: make sure not to save all pairwise interactions but just the sums for each atom and type of interaction
	MolecularInteractions::SAVE_ONLY_SUMS = 1;

	for (AtomIterator it = receptor_->beginAtom(); +it; it++)
	{
		it->disableStoreInteractions();
	}
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
	{
		PharmacophoreConstraint* ph = dynamic_cast<PharmacophoreConstraint*>(*it);
		if (!ph) continue;

		const list<const AtomContainer*
		>* interaction_partners = ph->getInteractionPartners();
		for (list < const AtomContainer* > ::const_iterator c_it = interaction_partners->begin();
		     c_it!=interaction_partners->end(); c_it++)
		     {
			     AtomContainer* ac = const_cast<AtomContainer*>(*c_it);
			     for (AtomIterator a_it = ac->beginAtom(); +a_it; a_it++)
			     {
				     a_it->enableStoreInteractions();
			     }
		     }
	}

	for (AtomIterator a_it = ligand_->beginAtom(); +a_it; a_it++)
	{
		a_it->disableStoreInteractions();
	}
}


void ScoringFunction::clearStoredInteractions_()
{
	if (!store_interactions_phC_only_)
	{
		for (AtomIterator it = ligand_->beginAtom(); +it; it++)
		{
			if (it->interactions) it->interactions->clear();
		}
		for (AtomIterator it = receptor_->beginAtom(); +it; it++)
		{
			if (it->interactions) it->interactions->clear();
		}
	}
	else
	{
		for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++)
		{
			PharmacophoreConstraint* ph = dynamic_cast<PharmacophoreConstraint*>(*it);
			if (!ph) continue;

			const list<const AtomContainer*
			>* interaction_partners = ph->getInteractionPartners();
			for (list < const AtomContainer* > ::const_iterator c_it = interaction_partners->begin(); c_it!=interaction_partners->end(); c_it++)
			{
				AtomContainer* ac = const_cast<AtomContainer*>(*c_it);
				for (AtomIterator a_it = ac->beginAtom(); +a_it; a_it++)
				{
					if (a_it->interactions) a_it->interactions->clear();
				}
			}
		}
	}
}


double ScoringFunction::calculateConstraintsScore()
{
	double total = 0;
	result_.refArea_penalties.resize(constraints.size());

	Size i = 0;
	for (list < Constraint* > ::iterator it = constraints.begin(); it != constraints.end(); it++, i++)
	{
		double score_i = (*it)->calculateScore(ligand_);
		result_.refArea_penalties[i] = score_i;
		total += score_i;
	}
	return total;
}


void ScoringFunction::enableInteractionComponents_(const list<String>& type_names)
{
	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		scoring_components_[i]->disable();
	}

	for (list < String > ::const_iterator it = type_names.begin(); it != type_names.end(); it++)
	{
		bool found_components = 0;
		for (Size i = 0; i < scoring_components_.size(); i++)
		{
			if (scoring_components_[i]->isLigandIntraMolecular()) continue;

			if (scoring_components_[i]->getTypeName() == *it)
			{
				found_components = 1;
				scoring_components_[i]->enable();
			}
		}
		if (!found_components)
		{
			String m = "No interaction ScoringComponent for type '"+*it+"' found!";
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoringFunction::enableComponents() error", m);
		}
	}
}


double ScoringFunction::getScore()
{
	return score_;
}


BALL::String ScoringFunction::convertTime(double t)
{
	String unit = "s";

	if (t >= 60)
	{
		t /= 60;
		unit="minutes";

		if (t >= 60)
		{
			t /= 60;
			unit="hours";

			if (t >= 24)
			{
				t /= 24;
				unit="days";
			}
		}
	}
	else if (t <= 0.01)
	{
		t *= 1000;
		unit = "milliseconds";

		if (t <= 0.01)
		{
			t *= 1000;
			unit = "microseconds";

			if (t <= 0.01)
			{
				t *= 1000;
				unit = "nanoseconds";
			}
		}
	}

	String diff = String(t)+" "+unit;
	return diff;
}

void ScoringFunction::enableAllComponents_()
{
	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		scoring_components_[i]->enable();
	}
}


double ScoringFunction::updateScore()
{
	if (!store_interactions_ && hasPharmacophoreConstraints_())
	{
		Log.level(10)<<"[info:] enabling storing interaction with PharmacophoreConstraint-residues"<<endl;
		enableStoreInteractionsOnlyForPhContraints();
	}

	score_ = 0;
	if (store_interactions_) clearStoredInteractions_();

	double conf_energy = 0;

	int i = 0;
	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); it++)
	{
		if (!(*it)->isEnabled()) continue;

		/// add ligand conformation contribution
		if ((*it)->isLigandIntraMolecular())
		{
			conf_energy += (*it)->updateScore();
		}
		/// add receptor-ligand contribution
		else
		{
			score_ += (*it)->updateScore();
		}
		i++;
	}

	if (static_ligand_fragments_.size() != 0)
	{
		conf_energy += static_ligand_energy_;
	}
	result_.ligand_conformation = conf_energy;

	if (conf_energy < 0) conf_energy *= conformation_scale_;

	/// add penalty for atoms outside of the hashgrid
	score_ += misplaced_ligand_atoms_*1e10;

	result_.interaction_score = score_;

	score_ += conf_energy;

	// backtransform
	score_ += exp_energy_mean_;


	/// If current ligand is buried less deeply then reference ligand, penalize this linearly.
	if (burial_depth_scale_ > 0 && reference_neighbors_ != 0)
	{
		double factor;
		if (neighboring_target_atoms_ != 0)
		{
			// the lower 'factor', the harder the penalization will be
			factor = (((double)neighboring_target_atoms_) /reference_neighbors_);
		}
		else // if there are no neighboring receptor atoms ...
			{
				factor = 0.5/reference_neighbors_;
			}

			if (factor < 1)
			{
				factor /= burial_depth_scale_;

				if (score_ > 0)
				{
					score_ /= factor;
				}
				else if (score_ < 0)
				{
					score_ *= factor;
				}
			}
	}

	/// add penalty if ligand candidate lies outside of some ReferenceAreas (if any have been defined by the user) or if some other Contraints are not fullfilled
	score_ += calculateConstraintsScore();

	/// explicit check for sterical clashes!
	if (overlaps_ > 0)
	{
		score_ += overlaps_*1e10;
		result_.interaction_score += overlaps_*1e10;

	}
	if (ligand_intramol_overlaps_ > 0)
	{
		result_.ligand_conformation += ligand_intramol_overlaps_*1e10;
		score_ += ligand_intramol_overlaps_*1e10;
	}

	return score_;
}


void ScoringFunction::update()
{
	overlaps_ = 0;
	ligand_intramol_overlaps_ = 0;
	AtomPairVector* ligand_nonbonded = NULL;
	bool update_ligand_nonbonded = 0;

	// Check for ligand intramolecular atom clashes
	if (all_ligand_nonbonded_ != NULL)
	{
		// all_ligand_nonbonded_ contains only interfragment pairs
		if (static_ligand_fragments_.size() > 0)
		{
			ligand_intramol_overlaps_ += checkForAtomOverlaps(all_ligand_nonbonded_);
		}
		else
		{
			ligand_nonbonded = createLigandNonbondedPairVector(0, ligand_intramol_overlaps_);
		}
	}
	if (all_ligand_nonbonded_ == NULL) // calculate ligand nonbonded pairs
	{
		ligand_nonbonded = createLigandNonbondedPairVector(0, ligand_intramol_overlaps_);
		update_ligand_nonbonded = 1;
	}

	AtomPairVector* receptor_ligand = createNonbondedPairVector(hashgrid_, overlaps_, 1);
	AtomPairVector empty_vector(0);

	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if (!(*it)->isEnabled()) continue;

		if (!(*it)->isAtomPairwise())
		{
			(*it)->update(empty_vector);
			continue;
		}

		if ((*it)->isLigandIntraMolecular())
		{
			if (update_ligand_nonbonded)
			{
				(*it)->update(*ligand_nonbonded);
			}
		}
		else
		{
			(*it)->update(*receptor_ligand);
		}
	}

	delete ligand_nonbonded;
	delete receptor_ligand;
}


bool ScoringFunction::hasFlexibleResidues()
{
	return !flexible_residues_.empty();
}


void ScoringFunction::resetFlexibleResidues()
{
	list<list<Vector3> >::iterator l_it = flexres_org_positions_.begin();
	for (set<Residue*> ::iterator s_it = flexible_residues_.begin(); s_it != flexible_residues_.end(); s_it++, l_it++)
	{
		list<Vector3>::iterator l2_it = l_it->begin();
		for (AtomIterator atom_it = (*s_it)->beginAtom(); +atom_it; atom_it++, l2_it++)
		{
			flexible_residues_hashgrid_->remove(atom_it->getPosition(), &*atom_it);
			atom_it->setPosition(*l2_it);
			flexible_residues_hashgrid_->insert(atom_it->getPosition(), &*atom_it);
		}
	}
}


void ScoringFunction::printResult(bool detail)
{
	list<Constraint*>::iterator it = constraints.begin();
	for (Size i = 0; i < result_.refArea_penalties.size() && it != constraints.end(); i++, it++)
	{
		double penalty = result_.refArea_penalties[i];
		if (penalty > 0)
		{
			Log.level(20)<<"\tpenalty score for constraint "<<i;
			String name = (*it)->getName();
			if (name != "")
			{
				Log.level(20)<<", \'"<<name<<"\'";
			}
			Log.level(20)<<": "<<penalty<<endl;
		}
	}

	Log.level(20)<<"ligand conformation: "<<result_.ligand_conformation<<endl;
	Log.level(20)<<"ligand-receptor interaction: "<<result_.interaction_score<<endl;
	Log.level(20)<<"av. no. of neighbors: "<<neighboring_target_atoms_<<endl;
	Log.level(20)<<"-----------------------------"<<endl;
	Log.level(20)<<"total score :   "<<score_<<endl;

	if (detail)
	{
		vector<double> contributions;
		vector<String> names;
		getScoreContributions(contributions, names);

		Log.level(20)<<endl<<"Individual contributions to above score:"<<endl;
		for (Size i = 0; i < names.size(); i++)
		{
			Log.level(20)<<"  "<<names[i]<<" : ";
			Log.level(20)<<"  "<<contributions[i]<<endl;
		}
		Log.level(20)<<endl;
	}
}


void ScoringFunction::getScoreContributions(vector<double>& score_contributions, vector<String>& names)
{
	Size size = scoring_components_.size();
	score_contributions.resize(size);
	names.resize(size);

	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		score_contributions[i] = scoring_components_[i]->getScaledScore();
		names[i] = scoring_components_[i]->getName();
	}
}


void ScoringFunction::getDefaultOptions(Options& options)
{
	Options* option_category = options.createSubcategory("Scoring Function");

	// units in Armstrongs
	list<String> allowed_values;
	allowed_values.push_back("1");
	allowed_values.push_back("5");

	list<String> boolean_values;
	boolean_values.push_back("true");
	boolean_values.push_back("false");

	option_category->setDefaultInteger(Option::HASHGRID_RESOLUTION, Default::HASHGRID_RESOLUTION);
	option_category->addParameterDescription(Option::HASHGRID_RESOLUTION, "hashgrid resolution", BALL::INT, &allowed_values);

	option_category->setDefaultInteger(Option::HASHGRID_SIZE, Default::HASHGRID_SIZE);
	option_category->addParameterDescription(Option::HASHGRID_SIZE, "hashgrid size (num of boxes)", BALL::INT);

	option_category->setDefaultReal(Option::NONBONDED_CUTOFF, Default::NONBONDED_CUTOFF);
	option_category->addParameterDescription(Option::NONBONDED_CUTOFF, "nonbonded cutoff", DOUBLE);

	option_category->setDefaultInteger(Option::BURIAL_DEPTH_SCALE, Default::BURIAL_DEPTH_SCALE);
	option_category->addParameterDescription(Option::BURIAL_DEPTH_SCALE, "relative-depth-of-burial scale", BALL::DOUBLE, &allowed_values);

	option_category->setDefaultBool(Option::IGNORE_H_CLASHES, Default::IGNORE_H_CLASHES);
	option_category->addParameterDescription(Option::IGNORE_H_CLASHES, "ignore clashes involving hydrogens", BALL::STRING, &boolean_values);

	option_category->setDefaultReal(Option::ALLOWED_INTERMOL_OVERLAP, Default::ALLOWED_INTERMOL_OVERLAP);
	allowed_values.clear();
	allowed_values.push_back("0");
	allowed_values.push_back("2");
	option_category->addParameterDescription(Option::ALLOWED_INTERMOL_OVERLAP, "allowed intermolecular atom-overlap", DOUBLE, &allowed_values);

	option_category->setDefaultReal(Option::ALLOWED_INTRAMOL_OVERLAP, Default::ALLOWED_INTRAMOL_OVERLAP);
	option_category->addParameterDescription(Option::ALLOWED_INTRAMOL_OVERLAP, "allowed intramolecular atom-overlap", DOUBLE, &allowed_values);

	option_category->setDefaultReal("electrostatic_cutoff", 20.0);
	option_category->addParameterDescription("electrostatic_cutoff", "electrostatic cutoff", BALL::DOUBLE);

	option_category->setDefaultReal("electrostatic_cuton", 17.000000);
	option_category->addParameterDescription("electrostatic_cuton", "electrostatic cuton", BALL::DOUBLE);

	option_category->setDefaultReal("vdw_cutoff", 20.000000);
	option_category->addParameterDescription("vdw_cutoff", "vdw cutoff", BALL::DOUBLE);

	option_category->setDefaultReal("vdw_cuton", 17.000000);
	option_category->addParameterDescription("vdw_cuton", "vdw cuton", BALL::DOUBLE);

	option_category->setDefault("filename", "Amber/amber96-docking.ini");
	option_category->addParameterDescription("filename", "file with electrostatics and vdW parameters", INFILE);
}


String ScoringFunction::getEquation()
{
	String eq="";
	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		String name = scoring_components_[i]->getName();
		double mean; double stddev;
		scoring_components_[i]->getNormalizationParameters(stddev, mean);

		eq += valueToString(scoring_components_[i]->getCoefficient())+"*";
		if (stddev != 0 && stddev != 1) eq += "(";
				    if (mean != 0) eq += "("+name+"-"+valueToString(mean)+")";
				    else eq += name;
				    if (stddev != 0 && stddev != 1) eq += "/"+valueToString(stddev)+")";

				 if (i < scoring_components_.size()-1) eq += " + ";
	}
	return eq;
}


void ScoringFunction::enableStoreInteractions(bool b)
{
	store_interactions_ = b;
}


String ScoringFunction::getName()
{
	return name_;
}


int ScoringFunction::getBurialDepthScale()
{
	return burial_depth_scale_;
}


String ScoringFunction::valueToString(double value)
{
	BALL::String t(value);
	int index = (Size)t.find_last_not_of("0");
	if (index < 0)
	{
		if (index+1 <= (int)t.size() && t[index] != '.') index++;
		    t = t.substr(0, index);
	}
	return t;
}


void ScoringFunction::unsetTrainingParameters()
{
	for (Size i = 0; i < scoring_components_.size(); i++)
	{
		scoring_components_[i]->setCoefficient(1);
		scoring_components_[i]->setNormalizationParameters(0, 0);
	}
	setNormalizationParameters(0, 0);
}


void ScoringFunction::setNormalizationParameters(double stddev, double mean)
{
	exp_energy_stddev_ = stddev;
	exp_energy_mean_ = mean;
}


const HashGrid3<Atom*>* ScoringFunction::getHashGrid()
{
	return hashgrid_;
}


const Vector3& ScoringFunction::getLigandCenter() const
{
	return ligand_center_;
}


double ScoringFunction::getLigandRadius() const
{
	return ligand_radius_;
}


bool ScoringFunction::storeInteractionsEnabled()
{
	return store_interactions_;
}


int ScoringFunction::getNoNeighboringReceptorAtoms()
{
	return neighboring_target_atoms_;
}


int ScoringFunction::getNoLigandAtoms()
{
	return ligand_atoms_;
}


void ScoringFunction::createStaticLigandFragments()
{
	atoms_to_fragments_.clear();
	vector<vector<Atom*> > sssr;
	RingPerceptionProcessor rpp;
	rpp.calculateSSSR(sssr, *ligand_);
	rotatable_ligand_bonds_.clear();

	if (static_ligand_fragments_.size() != 0) // delete fragments of previous ligand
		{
			for (Size i = 0; i < static_ligand_fragments_.size(); i++)
			{
				delete static_ligand_fragments_[i];
			}
			static_ligand_fragments_.resize(0);
		}

		StaticLigandFragment* fragment = new StaticLigandFragment;
	fragment->ID = 0;
	static_ligand_fragments_.push_back(fragment);
	fetchStaticLigandFragment(ligand_->getAtom(0), 0);
	static_ligand_energy_ = calculateStaticLigandFragmentEnergy();

	if (rotatable_ligand_bonds_.size() == 0)
	{
		bool only_one_atom = 1;
		AtomConstIterator it = ligand_->beginAtom();
		if (it != ligand_->endAtom()) it++;
		if (it != ligand_->endAtom()) only_one_atom = false;

		if (!only_one_atom)
		{
			//Log<<"[warning:]"<<" no rotatable bonds found within ligand! Is this really correct?!"<<endl;
		}
	}
}


void ScoringFunction::fetchStaticLigandFragment(Atom* a1, int index)
{
	static_ligand_fragments_[index]->atoms.push_back(a1);
	atoms_to_fragments_.insert(make_pair(a1, index));

	for (Atom::BondIterator bond_it = a1->beginBond(); !bond_it.isEnd(); ++bond_it)
	{
		// make sure to ignore intermolecular bonds !!
		int type = bond_it->getType();
		if (type != Bond::TYPE__COVALENT && type != Bond::TYPE__UNKNOWN)
		{
			continue;
		}

		Atom* a2 = const_cast<Atom*>(bond_it->getPartner(*a1));
		bool a2_checked = (atoms_to_fragments_.find(a2)!=atoms_to_fragments_.end());
		if (a2_checked)
		{
			continue;
		}

		bool rotatable_bond = 0;
		int order = bond_it->getOrder();
		if ( order == 1 && !bond_it->getProperty("InRing").getBool() && countCovalentBonds(a1, 2) > 1 && countCovalentBonds(a2, 2) > 1 && !isPeptideBond(&*bond_it) )
		{
			// found a rotatable bond between a1 and a2
			rotatable_bond = 1;
		}

		if (!rotatable_bond)
		{
			// process next atom of the same ring ...
			fetchStaticLigandFragment(a2, index);
		}
		else   // found a new connection to another fragment
		{
			rotatable_ligand_bonds_.push_back(&*bond_it);

			int new_index = static_ligand_fragments_.size();

			StaticLigandFragment::Connection nc1;
			nc1.atom = a1;
			nc1.neighbor_atom = a2;

			StaticLigandFragment* fragment = new StaticLigandFragment;
			fragment->ID = new_index;
			static_ligand_fragments_.push_back(fragment);
			StaticLigandFragment::Connection nc2;
			nc2.atom = a2;
			nc2.neighbor_atom = a1;

			nc1.fragment = fragment;
			nc2.fragment = static_ligand_fragments_[index];
			static_ligand_fragments_[index]->connections.push_back(nc1);
			static_ligand_fragments_[new_index]->connections.push_back(nc2);

			fetchStaticLigandFragment(a2, new_index);
		}
	}
}


int ScoringFunction::countCovalentBonds(const Atom* atom, int threshold)
{
	int no = 0;
	for (Atom::BondConstIterator bond_it = atom->beginBond(); !bond_it.isEnd(); ++bond_it)
	{
		// Make sure to ignore non-covalent bonds,
		// but allow TYPE_UNKNOWN since BALL stupidly uses this as default for all created bonds.
		int type = bond_it->getType();
		if (type == Bond::TYPE__COVALENT || type == Bond::TYPE__UNKNOWN)
		{
			no++;
		}
		if (threshold > 0 && no >= threshold) break; // for speed-up
	}
	return no;
}


double ScoringFunction::calculateStaticLigandFragmentEnergy()
{
	int overlaps = 0;
	AtomPairVector* staticfragment_pairs = createLigandNonbondedPairVector(1, overlaps); // nonbonded pairs WITHIN StaticLigandFragments
	
	double static_fragment_score = 0;

	if (scoring_components_.size() == 0)
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "calculateStaticLigandFragmentEnergy() error", "This ScoringFunction does not have any ScoringComponents yet!!");
	}
	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		if (!(*it)->isEnabled()) continue;

							// make sure just to use pair-wise components here; else score-contributions would be added here _and_ a second time when calling update() later
		if ((*it)->isLigandIntraMolecular() && (*it)->isAtomPairwise())
		{
			(*it)->update(*staticfragment_pairs);
			static_fragment_score += (*it)->updateScore();
		}
	}

	delete staticfragment_pairs;

	static_fragment_score += overlaps*1e10;

	return static_fragment_score;
}


bool ScoringFunction::isPeptideBond(const Bond* bond) const
{
	const Atom* a1 = bond->getFirstAtom();
	const Atom* a2 = bond->getSecondAtom();
	const Atom* C;
	const Atom* N;
	if ( a1->getElement().getSymbol() == "C" && a2->getElement().getSymbol() == "N" )
	{
		C = a1;
		N = a2;
	}
	else if ( a1->getElement().getSymbol() == "N" && a2->getElement().getSymbol() == "C" )
	{
		C = a2;
		N = a1;
	}
	else return false;

							Size no = 0; bool CO = 0; bool CR = 0;
	for (Atom::BondConstIterator it = C->beginBond(); !it.isEnd(); ++it, no++)
	{
		if (it->getPartner(*C) == N) continue;
							if (it->getOrder() == 2 && it->getPartner(*C)->getElement().getSymbol() == "O") CO = 1;
							else if (it->getPartner(*C)->getElement().getSymbol() == "C") CR = 1;
	}
	if (no != 3 || !CO || !CR) return false;

							no = 0; bool NR = 0; bool NH = 0;
	for (Atom::BondConstIterator it = N->beginBond(); !it.isEnd(); ++it, no++)
	{
		if (it->getPartner(*N) == C) continue;
							if (it->getPartner(*N)->getElement().getSymbol() == "C") NR = 1;
							else if (it->getPartner(*N)->getElement().getSymbol() == "H") NH = 1;
	}
	if ( (no != 3 || !NR || !NH) && (no != 2 || !NR || NH) ) return false;

							Log<<"[info:] found a peptide bond"<<endl;
	return true;
}


Options ScoringFunction::getOptions()
{
	return options_;
}


Options* ScoringFunction::getOptionsToModify()
{
	return &options_;
}


const vector<Bond*>* ScoringFunction::getRotatableLigandBonds() const
{
	return &rotatable_ligand_bonds_;
}


const vector<StaticLigandFragment*>* ScoringFunction::getStaticLigandFragments() const
{
	return &static_ligand_fragments_;
}


void ScoringFunction::setName(String name)
{
	name_ = name;
}


double ScoringFunction::getES()
{
	double es = 0.0;

	return es;
}


void ScoringFunction::setFlexibleResidues(const set<Residue*>& flexible_residues)
{
	delete flexible_residues_hashgrid_;
	delete all_residues_hashgrid_;
	flexible_residues_ = flexible_residues;
	static_residues_hashgrid_ = hashgrid_;
	flexible_residues_hashgrid_ = new HashGrid3<Atom*>(hashgrid_->getOrigin(), hashgrid_->getSizeX(), hashgrid_->getSizeY(), hashgrid_->getSizeZ(), hashgrid_->getUnit()[0]);
	all_residues_hashgrid_ = new HashGrid3<Atom*>(hashgrid_->getOrigin(), hashgrid_->getSizeX(), hashgrid_->getSizeY(), hashgrid_->getSizeZ(), hashgrid_->getUnit()[0]);
	*all_residues_hashgrid_ = *hashgrid_;
	flexres_org_positions_.clear();

	for (set<Residue*> ::iterator it = flexible_residues_.begin(); it != flexible_residues_.end(); it++)
	{
		flexres_org_positions_.resize(flexres_org_positions_.size()+1);
		list<Vector3>& pos_list = flexres_org_positions_.back();
		
		for (AtomIterator atom_it = (*it)->beginAtom(); +atom_it; atom_it++)
		{
			// Ignore backbone atoms.
			// They will remain part of the static grid, since applying a different rotamer does not changes their positions.
			String name = atom_it->getName();
			if (name == "N" || name == "O" || name == "CA" || name == "C"
				||name == "1H"||name == "2H"||name == "3H"||name == "HA"||name == "OXT")
			{
				continue;
			}

			static_residues_hashgrid_->remove(atom_it->getPosition(), &*atom_it);
			flexible_residues_hashgrid_->insert(atom_it->getPosition(), &*atom_it);
			pos_list.push_back(atom_it->getPosition());
		}
	}
}


bool ScoringFunction::assignRotamer(Residue* residue, ResidueRotamerSet* rotamer_set, const Rotamer* rotamer)
{
	// backup atom positions in order to be able to restore them later if the new rotamer overlaps with other protein atoms
	HashGrid3<Atom*>* optimization_grid = hashgrid_;
	if (all_residues_hashgrid_) optimization_grid = all_residues_hashgrid_;
	list<Vector3> old_positions;
	for (AtomIterator it = residue->beginAtom(); +it; it++)
	{
		old_positions.push_back(it->getPosition());
		if (flexible_residues_hashgrid_)
		{
			flexible_residues_hashgrid_->remove(it->getPosition(), &*it);
		}
		optimization_grid->remove(it->getPosition(), &*it);
	}

	rotamer_set->setRotamer(*residue, *rotamer);
	bool overlaps_detected = false;

	// make sure to check for overlaps with all other residues, not just the flexible ones!
	HashGrid3<Atom*>* backup = hashgrid_;
	hashgrid_ = optimization_grid;

	// First, check for overlaps
	for (AtomIterator it = residue->beginAtom(); +it; it++)
	{
		int no_of_overlaps = 0;
		countNeighboringReceptorAtoms(&*it, 4, 0, &no_of_overlaps);
		if (no_of_overlaps > 0)
		{
			//Log.level(5)<<"Overlap for new rotamer found, resetting residue to previously used rotamer!"<<endl;
			overlaps_detected = true;
			break;
		}
	}

	// if overlaps were detected, reset atom positions
	if (overlaps_detected)
	{
		list<Vector3>::iterator l_it = old_positions.begin();
		for (AtomIterator it = residue->beginAtom(); +it; it++, l_it++)
		{
			it->setPosition(*l_it);
		}
	}

	// update hashgrids
	for (AtomIterator it = residue->beginAtom(); +it; it++)
	{
		if (flexible_residues_hashgrid_)
		{
			flexible_residues_hashgrid_->insert(it->getPosition(), &*it);
		}
		optimization_grid->insert(it->getPosition(), &*it);
	}

	hashgrid_ = backup;
	return !overlaps_detected;
}


Size ScoringFunction::countNeighboringReceptorAtoms(const Atom* atom, double distance_threshold, bool onePerCell, int* number_of_overlaps) const
{
	if (number_of_overlaps != NULL) *number_of_overlaps = 0;

	int x_size = (int)hashgrid_->getSizeX();
	int y_size = (int)hashgrid_->getSizeY();
	int z_size = (int)hashgrid_->getSizeZ();

	// calculate position of current ligand atom in hashgrid
	Vector3 lig_atom_pos = (atom->getPosition() - hashgrid_->getOrigin());
	lig_atom_pos[0] /= hashgrid_->getUnit()[0];
	lig_atom_pos[1] /= hashgrid_->getUnit()[1];
	lig_atom_pos[2] /= hashgrid_->getUnit()[2];

	// atoms outside of the interaction grid are defined as having no neighboring receptor atoms
	if (lig_atom_pos[0] < 0 || lig_atom_pos[0] > x_size || lig_atom_pos[1] < 0 || lig_atom_pos[1] > y_size || lig_atom_pos[2] < 0 || lig_atom_pos[2] > z_size )
	{
		return 0;
	}

	double neighbor_cutoff_2 = pow(distance_threshold, 2);
	Size search_radius = (Size)ceil(distance_threshold/hashgrid_->getUnit()[0]);

	// indices in HashGrid, where the search for interacting target atoms should begin ( != position of ligand atom)
	int i = static_cast < int > (lig_atom_pos[0]-search_radius); if (i < 0){i = 0; }
	int j0 = static_cast < int > (lig_atom_pos[1]-search_radius); if (j0 < 0){j0 = 0; }
	int k0 = static_cast < int > (lig_atom_pos[2]-search_radius); if (k0 < 0){k0 = 0; }

	/** find all atoms in neighboring boxes (depth = search_radius) around the current ligand atom  */
	Size neighboring_atoms = 0;
	for (; i <= lig_atom_pos[0]+search_radius && i < x_size; i++)
	{
		for (int j = j0; j <= lig_atom_pos[1]+search_radius && j < y_size; j++)
		{
			for (int k = k0; k <= lig_atom_pos[2]+search_radius && k < z_size; k++)
			{
				HashGridBox3<Atom*>* box = hashgrid_->getBox(i, j, k);

				// ... for all atoms in that box
				for (HashGridBox3 < Atom* > ::DataIterator di = box->beginData(); di != box->endData(); di++)
				{
					Vector3 d = atom->getPosition() - (*di)->getPosition();
					double distance_2 = pow(d[0], 2)+pow(d[1], 2)+pow(d[2], 2);
					if (distance_2 < neighbor_cutoff_2)
					{
						neighboring_atoms++;
						if (onePerCell) break;
					}

					if (number_of_overlaps != NULL)
					{
						double radii = atom->getElement().getVanDerWaalsRadius() + (*di)->getElement().getVanDerWaalsRadius();
						double dist = sqrt(distance_2);

						if (atom != (*di) && radii >= dist+allowed_intramolecular_overlap_ &&
							!atom->isBoundTo(**di) && !atom->isGeminal(**di))
						{
							(*number_of_overlaps)++;
						}
					}
				}
			}
		}
	}

	return neighboring_atoms;
}


void ScoringFunction::resetResiduePositions(Residue* residue, list<Vector3>& old_positions)
{
	list<Vector3>::iterator l_it = old_positions.begin();
	for (AtomIterator it = residue->beginAtom(); +it; it++, l_it++)
	{
		hashgrid_->remove(it->getPosition(), &*it);
		it->setPosition(*l_it);
		hashgrid_->insert(it->getPosition(), &*it);
	}
}


const map<Atom*, int>* ScoringFunction::getAtomFragmentMap()
{
	return &atoms_to_fragments_;
}

void ScoringFunction::setupFlexibleResidues_()
{
	String flex_ids = options_.get("flexible_residues");
	if (flex_ids == "") return;

	    string names="";
	set<Residue*> residue_set;
	if (flex_ids == "automatic")
	{
		SideChainOptimizer sco(this);
		sco.findFlexibleSideChains(residue_set, 5, 50);
		for (set<Residue*> ::iterator it = residue_set.begin(); it != residue_set.end(); it++)
		{
			names += (*it)->getFullName()+(*it)->getID()+" ";
		}
	}
	else
	{
		vector<String> ids;
		flex_ids.split(ids);
		set<String> name_set;
		for (Size i = 0; i < ids.size(); i++)
		{
			name_set.insert(ids[i]);
		}
		
		Protein* protein = dynamic_cast<Protein*>(receptor_);
		System* system;
		if (!protein && (system = dynamic_cast < System* > (receptor_)))
		{
			protein = dynamic_cast<Protein*>(system->getMolecule(0));
		}
		if (!protein)
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "SideChainOptimizer::setFlexibleResidues() error", "Receptor is no Protein!");
		}
		
		for (ResidueIterator it = protein->beginResidue(); +it; it++)
		{
			String id = it->getID();
			if (name_set.find(id) != name_set.end())
			{
				residue_set.insert(&*it);
				names += it->getFullName()+it->getID()+" ";
			}
		}
	}
	if (residue_set.size() > 0)
	{
		setFlexibleResidues(residue_set);
		Log.level(20)<<"Defined as flexible residues: "<<names<<endl;
	}
}
