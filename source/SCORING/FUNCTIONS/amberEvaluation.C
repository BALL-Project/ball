#include <BALL/SCORING/FUNCTIONS/amberEvaluation.h>

using namespace BALL;

//namespace BALL
//{
	AmberEvaluation::AmberEvaluation()
		throw()
	{
		ff_ = new AmberFF();
		delete_force_field_ = true;
	}

	AmberEvaluation::AmberEvaluation(AmberFF& ff)
		throw()
	{
		ff_ = &ff;
		delete_force_field_ = false;
	}

	AmberEvaluation::AmberEvaluation(AmberFF& ff, Options& options)
		throw()
	{
		ff_ = &ff;
		options_ = options;
		//ff_->setOptions(options_);
		delete_force_field_ = false;
	}

	AmberEvaluation::~AmberEvaluation()
		throw()
	{
		if (delete_force_field_)
			delete ff_;
	}

//}
