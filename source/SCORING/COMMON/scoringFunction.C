// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: scoringFunction.C,v 1.2 2006/05/21 18:15:29 anker Exp $

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
		
		:	options(),
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
		
		:	options(),
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


	ScoringFunction::ScoringFunction(Molecule& receptor, Molecule& ligand,
			const Options& options)
		
		:	options(options),
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
		
	{
		clear();
	}


	void ScoringFunction::clear()
		
	{
		options.clear();

		// Don't delete the system or the molecules, that's not the job of a
		// scoring function
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
		
	{
		options = sf.options;
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
		
	{

		// Get defaults from the options
		Size base_function_type 
			= options.setDefaultInteger(ScoringFunction::Option::BASE_FUNCTION_TYPE,
					ScoringFunction::Default::BASE_FUNCTION_TYPE);


		// Check the environment
		if (molecule1_ == 0 || molecule2_ == 0)
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


	bool ScoringFunction::setup(Molecule& receptor, Molecule& ligand)
		
	{
		setReceptor(receptor);
		setLigand(ligand);
		return(setup());
	}


	bool ScoringFunction::setup(Molecule& receptor, Molecule& ligand,
			const Options& opt)
		
	{
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
		
	{
		max_number_of_errors_ = nr;
	}


	Size ScoringFunction::getMaximumNumberOfErrors() const
		
	{
		return(max_number_of_errors_);
	}

	
	void ScoringFunction::setFirstMolecule(Molecule& molecule)
		
	{
		molecule1_ = &molecule;
	}


	Molecule* ScoringFunction::getFirstMolecule() const
		
	{
		return(molecule1_);
	}

	void ScoringFunction::setSecondMolecule(Molecule& molecule)
		
	{
		molecule2_ = &molecule;
	}


	Molecule* ScoringFunction::getSecondMolecule() const
		
	{
		return(molecule2_);
	}


	void ScoringFunction::setReceptor(Molecule& molecule)
		
	{
		setFirstMolecule(molecule);
	}


	Molecule* ScoringFunction::getReceptor() const
		
	{
		return(getFirstMolecule());
	}


	void ScoringFunction::setLigand(Molecule& molecule)
		
	{
		setSecondMolecule(molecule);
	}


	Molecule* ScoringFunction::getLigand() const
		
	{
		return(getSecondMolecule());
	}


	void ScoringFunction::setIntercept(double intercept)
		
	{
		intercept_ = intercept;
	}


	double ScoringFunction::getIntercept() const
		
	{
		return(intercept_);
	}


	ScoringBaseFunction* ScoringFunction::getBaseFunction() const
		
	{
		return(base_function_);
	}


	void ScoringFunction::insertComponent(ScoringComponent* component,
			float coefficient)
		
	{
		components_.push_back(std::pair<ScoringComponent*, float>(component, coefficient));
	}


	void ScoringFunction::removeComponent(const ScoringComponent* component)
		
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
		
	{
		return(unassigned_atoms_);
	}


	HashSet<const Atom*>& ScoringFunction::getUnassignedAtoms()
		
	{
		return(unassigned_atoms_);
	}

}

