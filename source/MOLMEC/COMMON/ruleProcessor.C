// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: ruleProcessor.C,v 1.8 2002/12/16 12:23:00 sturm Exp $

#include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#include <BALL/FORMAT/INIFile.h>

using namespace std;

namespace BALL 
{

	RuleProcessor::RuleProcessor()
		:	valid_(false)
	{
	}

	RuleProcessor::RuleProcessor(INIFile& file, const String& prefix)
		:	valid_(false)
	{
		valid_ = initialize(file, prefix);
	}

	RuleProcessor::RuleProcessor(const RuleProcessor& rule_processor)
		:	UnaryProcessor<Atom>(),
			valid_(rule_processor.valid_)
	{
	}

	RuleProcessor::~RuleProcessor()
	{
		destroy();
	}
	
	void RuleProcessor::destroy()
	{
		clear();
	}
	
	void RuleProcessor::clear()
	{
		valid_ = false;
	}
	
	bool RuleProcessor::initialize(INIFile& file, const String& prefix)
	{
		return evaluator_.initialize(file, prefix);
	}
	
	const RuleProcessor& RuleProcessor::operator = (const RuleProcessor& rule_processor)
	{
		set(rule_processor);
		return *this;
	}

  void RuleProcessor::set(const RuleProcessor& /* rule_processor */)
	{
		// ?????
	}

	bool RuleProcessor::start()
	{
		return true;
	}

	bool RuleProcessor::finish()
	{
		return true;
	}

	String RuleProcessor::evaluate(const Atom& atom)
	{
		return evaluator_.operator()(atom);
	}

	bool RuleProcessor::isValid() const
	{
		return valid_;
	}

	void RuleProcessor::dump(std::ostream& /* s */) const
	{
		//?????
	}
}
