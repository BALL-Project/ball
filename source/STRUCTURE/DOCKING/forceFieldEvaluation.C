#include <BALL/STRUCTURE/DOCKING/forceFieldEvaluation.h>

namespace BALL
{
	ForceFieldEvaluation::ForceFieldEvaluation()
		throw()
	{
		ff_ = new ForceField;
		delete_force_field_ = true;
	}

	ForceFieldEvaluation::~ForceFieldEvaluation()
		throw()
	{
		if (delete_force_field_)
			delete ff_;
	}

	void ForceFieldEvaluation::setForceField(ForceField& ff)
		throw()
	{
		ff_ = &ff;
	}

	void ForceFieldEvaluation::setOptions(const Options& options)
		throw()
	{
		options_ = options;
	}

	ForceField& ForceFieldEvaluation::getForceField()
		throw()
	{
		return *ff_;
	}

	const ForceField& ForceFieldEvaluation::getForceField() const
		throw()
	{
		return *ff_;
	}

	Options& ForceFieldEvaluation::getOptions()
		throw()
	{
		return options_;
	}

	const Options& ForceFieldEvaluation::getOptions() const
		throw()
	{
		return options_;
	}

	std::vector<ConformationSet::Conformation> ForceFieldEvaluation::operator () (ConformationSet& conformations)
		throw()
	{
		std::vector<ConformationSet::Conformation> result;
		Log.info() << "in ForceFieldEvaluation::operator() " << std::endl;
		if (!ff_)
			return result;

		System S = conformations.getSystem();

		ff_->setup(S);

		Log.info() << "in ForceField:: Option of Amber FF:" << std::endl;
		Options::Iterator it = ff_->options.begin();
		for(; +it; ++it)
		{
			Log.info() << it->first << " : " << it->second << std::endl;
		}
		
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
