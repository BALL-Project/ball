// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scoringFunction.C,v 1.1 2005/11/21 19:27:09 anker Exp $

#include <BALL/SCORING/COMMON/scoringFunction.h>
#include <BALL/SCORING/COMMON/linearBaseFunction.h>
#include <BALL/SCORING/COMMON/fermiBaseFunction.h>

namespace BALL
{

	//
	const char* ScoringFunction::Option::VERBOSITY = "verbosity";
	const char* ScoringFunction::Option::BASE_FUNCTION_TYPE
		= "base_function_type";

	//
	const Size ScoringFunction::Default::VERBOSITY = 0;
	const Size ScoringFunction::Default::BASE_FUNCTION_TYPE
		= ScoringBaseFunction::BASE_FUNCTION_TYPE__FERMI;


	ScoringFunction::ScoringFunction()
		throw()
		:	options(),
			system_(0),
			molecule1_(0),
			molecule2_(0),
			name_(""),
			score_(0.0),
			intercept_(0.0),
			base_function_(0),
			components_(),
			unassigned_atoms_(),
			max_number_of_errors_(0),
			number_of_errors_(0)
	{
		// ????
	}


	ScoringFunction::ScoringFunction(const ScoringFunction& sf)
		throw()
		:	options(),
			system_(sf.system_),
			molecule1_(sf.molecule1_),
			molecule2_(sf.molecule2_),
			name_(sf.name_),
			score_(sf.score_),
			intercept_(sf.intercept_),
			base_function_(sf.base_function_),
			components_(sf.components_),
			unassigned_atoms_(),
			max_number_of_errors_(sf.max_number_of_errors_),
			number_of_errors_(sf.number_of_errors_)
	{
		// An instance of ScoringFunction created with this copy constructor
		// will use the same (!) components as the original one. Only the
		// pointers will be copied from the original instance.
		setup();
	}


	ScoringFunction::ScoringFunction(System& system)
		throw()
		:	options(),
			system_(&system),
			molecule1_(0),
			molecule2_(0),
			name_(""),
			score_(0.0),
			intercept_(0.0),
			base_function_(0),
			components_(),
			unassigned_atoms_(),
			max_number_of_errors_(0),
			number_of_errors_(0)
	{
		setup();
	}


	ScoringFunction::ScoringFunction(System& system, const Options& options)
		throw()
		:	options(options),
			system_(&system),
			molecule1_(0),
			molecule2_(0),
			name_(""),
			score_(0.0),
			intercept_(0.0),
			base_function_(0),
			components_(),
			unassigned_atoms_(),
			max_number_of_errors_(0),
			number_of_errors_(0)
	{
		setup();
	}


	ScoringFunction::ScoringFunction(System& system, Molecule& receptor,
			Molecule& ligand, const Options& options)
		throw()
		:	options(options),
			system_(&system),
			molecule1_(&receptor),
			molecule2_(&ligand),
			name_(""),
			score_(0.0),
			intercept_(0.0),
			base_function_(0),
			components_(),
			unassigned_atoms_(),
			max_number_of_errors_(0),
			number_of_errors_(0)
	{
		setup();
	}


	ScoringFunction::~ScoringFunction()
		throw()
	{
		clear();
	}


	void ScoringFunction::clear()
		throw()
	{
		options.clear();

		// Don't delete the system or the molecules, that's not the job of a
		// scoring function
		system_ = 0;
		molecule1_ = 0;
		molecule2_ = 0;

		name_ = String("");
		score_ = 0.0;
		intercept_ = 0.0;

		// The base function was (hopefully) assigned by the setup() method, do
		// we have to delete it here.
		if (base_function_ != 0)
		{
			delete base_function_;
			base_function_ = 0;
		}

		components_.clear();
		unassigned_atoms_.clear();
		max_number_of_errors_ = 0;
		number_of_errors_ = 0;
	}


	ScoringFunction& ScoringFunction::operator = (const ScoringFunction& sf)
		throw()
	{
		options = sf.options;
		system_ = sf.system_;
		name_ = sf.name_;
		score_ = sf.score_;
		intercept_ = sf.intercept_;
		components_ = sf.components_;
		unassigned_atoms_ = sf.unassigned_atoms_;
		max_number_of_errors_ = sf. max_number_of_errors_;
		number_of_errors_ = sf.number_of_errors_;

		return(*this);
	}


	bool ScoringFunction::setup()
		throw()
	{

		// Get defaults from the options
		Size base_function_type 
			= options.setDefaultInteger(ScoringFunction::Option::BASE_FUNCTION_TYPE,
					ScoringFunction::Default::BASE_FUNCTION_TYPE);


		// Check the environment
		if (system == 0 || (molecule1_ == 0 || molecule2_ == 0))
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


		// parts that are specific to the scoring function
		bool success = false;
		try
		{
			success = specificSetup();
		}
		catch(...)
		{
		}

		if (!success)
		{
			Log.error() << "ScoringFunction::setup(): specificSetup() failed!" 
				<< std::endl;
			return(false);
		}

		// Now setup all the components of this scoring function
		std::vector< std::pair<ScoringComponent*, float> >::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			success = false;
			try
			{
				success = it->first->setup();
			}
			catch(...)
			{
			}

			if (!success)
			{
				Log.error() << "ScoringFunction::setup(): Setup of component "
					<< it->first->getName() << " failed!" << std::endl;
			}

		}
																
		return(success);
	}


	bool ScoringFunction::setup(System& system, Molecule& receptor,
			Molecule& ligand)
		throw()
	{
		setSystem(system);
		setReceptor(receptor);
		setLigand(ligand);
		return(setup());
	}


	bool ScoringFunction::setup(System& system, Molecule& receptor,
			Molecule& ligand, const Options& opt)
		throw()
	{
		setSystem(system);
		setReceptor(receptor);
		setLigand(ligand);
		options = opt;
		return(setup());
	}


	bool ScoringFunction::specificSetup()
		throw(Exception::TooManyErrors)
	{
		// ????
		return(true);
	}


	void ScoringFunction::setMaximumNumberOfErrors(Size nr)
		throw()
	{
		max_number_of_errors_ = nr;
	}


	Size ScoringFunction::getMaximumNumberOfErrors() const
		throw()
	{
		return(max_number_of_errors_);
	}

	
	void ScoringFunction::setSystem(System& system)
		throw()
	{
		// Note: This method does NOT set up the scoring function. In order to
		// have a up-to-date version of the scoring function in memory, you
		// have to call setup() yourself.
		system_ = &system;
	}


	System* ScoringFunction::getSystem() const
		throw()
	{
		return(system_);
	}


	void ScoringFunction::setFirstMolecule(Molecule& molecule)
		throw()
	{
		molecule1_ = &molecule;
	}


	Molecule* ScoringFunction::getFirstMolecule() const
		throw()
	{
		return(molecule1_);
	}

	void ScoringFunction::setSecondMolecule(Molecule& molecule)
		throw()
	{
		molecule2_ = &molecule;
	}


	Molecule* ScoringFunction::getSecondMolecule() const
		throw()
	{
		return(molecule2_);
	}


	void ScoringFunction::setReceptor(Molecule& molecule)
		throw()
	{
		setFirstMolecule(molecule);
	}


	Molecule* ScoringFunction::getReceptor() const
		throw()
	{
		return(getFirstMolecule());
	}


	void ScoringFunction::setLigand(Molecule& molecule)
		throw()
	{
		setSecondMolecule(molecule);
	}


	Molecule* ScoringFunction::getLigand() const
		throw()
	{
		return(getSecondMolecule());
	}


	void ScoringFunction::setIntercept(double intercept)
		throw()
	{
		intercept_ = intercept;
	}


	double ScoringFunction::getIntercept() const
		throw()
	{
		return(intercept_);
	}


	ScoringBaseFunction* ScoringFunction::getBaseFunction() const
		throw()
	{
		return(base_function_);
	}


	void ScoringFunction::insertComponent(ScoringComponent* component,
			float coefficient)
		throw()
	{
		components_.push_back(std::pair<ScoringComponent*, float>(component, coefficient));
	}


	void ScoringFunction::removeComponent(const ScoringComponent* component)
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first == component)
			{
				delete it->first;
				components_.erase(it);
				break;
			}
		}
	}


	void ScoringFunction::removeComponent(const String& name)
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first->getName() == name)
			{
				delete it->first;
				components_.erase(it);
				break;
			}
		}
	}


	void ScoringFunction::setCoefficient(const ScoringComponent* component, 
			float coefficient)
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first == component)
			{
				it->second = coefficient;
				break;
			}
		}
	}


	void ScoringFunction::setCoefficient(const String& name, 
			float coefficient)
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first->getName() == name)
			{
				it->second = coefficient;
				break;
			}
		}
	}


	bool ScoringFunction::getCoefficient(const ScoringComponent* component,
			float& coefficient) const
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::const_iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first == component)
			{
				coefficient = it->second;
				return(true);
			}
		}
		return(false);
	}


	bool ScoringFunction::getCoefficient(const String& name,
			float& coefficient) const
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::const_iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first->getName() == name)
			{
				coefficient = it->second;
				return(true);
			}
		}
		return(false);
	}


	ScoringComponent* ScoringFunction::getComponent(const String& name) const
		throw()
	{
		std::vector< std::pair<ScoringComponent*, float> >::const_iterator it;
		for (it = components_.begin(); it != components_.end(); ++it)
		{
			if (it->first->getName() == name)
			{
				return(it->first);
			}
		}
		return(0);
	}


	ScoringComponent* ScoringFunction::getComponent(const Size index) const
		throw()
	{
		if (index < components_.size()) 
		{
			return(components_[index].first);
		}
		else 
		{
			return(0);
		}
	}


	double ScoringFunction::calculateScore()
		throw()
	{
		score_ = 0.0;
		for (Size i = 0; i < components_.size(); ++i)
		{
			float score = components_[i].first->calculateScore();
			float factor = components_[i].second;
			score_ += factor * score;
		}
		return(intercept_ + score_);
	}


	const HashSet<const Atom*>& ScoringFunction::getUnassignedAtoms() const
		throw()
	{
		return(unassigned_atoms_);
	}


	HashSet<const Atom*>& ScoringFunction::getUnassignedAtoms()
		throw()
	{
		return(unassigned_atoms_);
	}

}

