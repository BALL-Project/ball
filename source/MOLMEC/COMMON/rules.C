// $Id: rules.C,v 1.1 2000/05/19 11:12:12 oliver Exp $

using namespace std;

namespace BALL 
{

	RuleEvaluator::RuleEvaluator()
		:	prefix_(),
			rule_map_(),
			valid(false)
	{
	}

	RuleEvaluator::RuleEvaluator(INIFile& file, const String& prefix)
		:	prefix_(),
			rule_map_(),
			valid(false)
	{
		valid_ = initialize(file, prefix);
	}

	RuleEvaluator::RuleEvaluator(const RuleEvaluator& evaluator)
		:	prefix_(evaluator.prefix_),
			rule_map_(evaluator.rule_map_),
			valid(evaluator.valid_)
	{
	}


	RuleEvalutor::~RuleEvaluator()
	{
		destroy();
	}
	
	RuleEvaluator::destroy()
	{
		clear();
	}
	
	RuleEvaluator::clear()
	{
		valid_ = false;
		rule_map_.clear();
	}
	
	
}
