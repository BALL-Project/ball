#include <BALL/STRUCTURE/DOCKING/forceFieldEvaluation.h>

namespace BALL
{
	ForceFieldEvaluation::ForceFieldEvaluation()
		
		: EnergeticEvaluation(),
			ff_(new ForceField()),
			delete_force_field_(true)
	{
	}

	ForceFieldEvaluation::ForceFieldEvaluation(ForceField& ff)
		
		: EnergeticEvaluation(),
			ff_(&ff),
			delete_force_field_(false)
	{
	}

	ForceFieldEvaluation::~ForceFieldEvaluation()
		
	{
		if (delete_force_field_) delete ff_;
	}

	void ForceFieldEvaluation::setForceField(ForceField& ff)
		
	{
		ff_ = &ff;
		delete_force_field_ = false;
	}

	void ForceFieldEvaluation::setOptions(const Options& options)
		
	{
		options_ = options;
	}

	ForceField& ForceFieldEvaluation::getForceField()
		
	{
		return *ff_;
	}

	const ForceField& ForceFieldEvaluation::getForceField() const
		
	{
		return *ff_;
	}

	Options& ForceFieldEvaluation::getOptions()
		
	{
		return options_;
	}

	const Options& ForceFieldEvaluation::getOptions() const
		
	{
		return options_;
	}

	std::vector<ConformationSet::Conformation> ForceFieldEvaluation::operator () (ConformationSet& conformations)
		throw(Exception::TooManyErrors)
	{
		std::vector<ConformationSet::Conformation> result;
		if (!ff_) return result;

		System S = conformations.getSystem();

		ff_->setup(S);
		
		for (int i=0; i<(int)conformations.size(); i++)
		{
			conformations[i].applySnapShot(S);
			ff_->updateEnergy();	
			float energy = ff_->getEnergy();
			ConformationSet::Conformation current_conf(i, energy);
			result.push_back(current_conf);
		}

		return result;
	}
	
}
