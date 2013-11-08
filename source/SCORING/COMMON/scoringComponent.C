// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/scoringComponent.h>
#include <BALL/SCORING/COMMON/fermiBaseFunction.h>
#include <BALL/SCORING/COMMON/linearBaseFunction.h>
#include <BALL/SCORING/COMMON/scoringFunction.h>

using namespace std;
using namespace BALL;

ScoringComponent::ScoringComponent()
	:	scoring_function_(0),
		score_(0.0),
		name_("")
{
	base_function_ = new LinearBaseFunction;
	ligand_intra_molecular_ = 0;
	coefficient_ = 1;
	gridable_ = 1;
	atom_pairwise_ = 1;
	stddev_ = 0;
	mean_ = 0;
	enabled_ = true;
}


ScoringComponent::ScoringComponent(const ScoringComponent& sc)
	:	scoring_function_(sc.scoring_function_),
		score_(sc.score_),
		name_(sc.name_)
{
	base_function_ = new ScoringBaseFunction(*sc.base_function_);
	ligand_intra_molecular_ = 0;
	coefficient_ = 1;
	gridable_ = 1;
	atom_pairwise_ = 1;
	stddev_ = 0;
	mean_ = 0;
	enabled_ = true;
}


ScoringComponent::ScoringComponent(ScoringFunction& sf)
	:	scoring_function_(&sf),
		score_(0.0),
		name_("")
{
	base_function_ = new LinearBaseFunction;
	ligand_intra_molecular_ = 0;
	coefficient_ = 1;
	gridable_ = 1;
	atom_pairwise_ = 1;
	stddev_ = 0;
	mean_ = 0;
	enabled_ = true;
}


ScoringComponent::~ScoringComponent()
{
	clear();
}


void ScoringComponent::selectBaseFunction(String function)
{
	if (function == "fermi")
	{
		delete base_function_;
		base_function_ = new FermiBaseFunction;
	}
	else if (function == "linear")
	{
		delete base_function_;
		base_function_ = new LinearBaseFunction;
	}
	else
	{
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "BaseFunction selection error", "BaseFunction '"+function+"' unknown!");
	}
}

void ScoringComponent::clear()
{
	delete base_function_;
	scoring_function_ = 0;
	score_ = 0.0;
	name_ = "";
}


bool ScoringComponent::setup()
{
	return true;
}


ScoringFunction* ScoringComponent::getScoringFunction() const
{
	return scoring_function_;
}


void ScoringComponent::setScoringFunction(ScoringFunction& sf)
{
	scoring_function_ = &sf;
}


String ScoringComponent::getName() const
{
	return name_;
}


void ScoringComponent::setName(const String& name)
{
	name_ = name;
}


double ScoringComponent::getRawScore() const
{
	return score_;
}

double ScoringComponent::getScaledScore() const
{
	return scaleScore(score_);
}

void ScoringComponent::setLigandIntraMolecular(bool b)
{
	ligand_intra_molecular_ = b;
}

bool ScoringComponent::isLigandIntraMolecular()
{
	return ligand_intra_molecular_;
}

// should be overloaded by all ScoringComponents that are to be used for Scoring
void ScoringComponent::update(const AtomPairVector& /*pair_vector*/)
{
	string mess="update(AtomPairVector* pair_vector) is not implemented for this ScoringComponent!! Thus this ScoringComponent can not (yet) be used!!";
	throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoringComponent error", mess);
}

double ScoringComponent::updateScore()
{
	return 0.0;
}

void ScoringComponent::setCoefficient(const double& coefficient)
{
	coefficient_ = coefficient;
}

const double& ScoringComponent::getCoefficient()
{
	return coefficient_;
}

void ScoringComponent::setupLigand()
{
}

bool ScoringComponent::isGridable()
{
	return gridable_;
}

bool ScoringComponent::isAtomPairwise()
{
	return atom_pairwise_;
}

void ScoringComponent::setNormalizationParameters(double stddev, double mean)
{
	stddev_ = stddev;
	mean_ = mean;
}

void ScoringComponent::getNormalizationParameters(double& stddev, double& mean)
{
	stddev = stddev_;
	mean = mean_;
}

/*
void ScoringComponent::scaleScore()
{
	scaleScore(score_);
}
*/

double ScoringComponent::scaleScore(double score) const
{
	double scaled_score = score;

	// if desired, transform score
	if (stddev_ > 0.01)
	{
		scaled_score -= mean_;
		scaled_score /= stddev_;

		scaled_score *= coefficient_;

		// BACK-transform accoring to normalization of the binding free energies of the traning data set
		if (scoring_function_ != 0)
		{
			double act_stddev = scoring_function_->getExpEnergyStddev();
			if (act_stddev > 0.01)
			{
				scaled_score *= act_stddev;
			}
		}
		else
		{
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "ScoringComponent update error", "This components must be bound to a ScoringFunction before a score can be calculated!");
		}
	}
	else
	{
		scaled_score *= coefficient_;
	}

	return scaled_score;
}


void ScoringComponent::enable()
{
	enabled_ = true;
}

void ScoringComponent::disable()
{
	enabled_ = false;
}

bool ScoringComponent::isEnabled()
{
	return enabled_;
}

const String& ScoringComponent::getTypeName()
{
	return type_name_;
}
