// $Id: ruleEvaluator.C,v 1.1 2000/05/24 12:16:08 oliver Exp $

#include <BALL/MOLMEC/COMMON/rules.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL 
{

	RuleEvaluator::RuleEvaluator()
		:	prefix_(),
			rule_map_(),
			valid_(false)
	{
	}

	RuleEvaluator::RuleEvaluator(INIFile& file, const String& prefix)
		:	prefix_(),
			rule_map_(),
			valid_(false)
	{
		valid_ = initialize(file, prefix);
	}

	RuleEvaluator::RuleEvaluator(const RuleEvaluator& evaluator)
		:	prefix_(evaluator.prefix_),
			rule_map_(evaluator.rule_map_),
			valid_(evaluator.valid_)
	{
	}


	RuleEvaluator::~RuleEvaluator()
	{
		destroy();
	}
	
	void RuleEvaluator::destroy()
	{
		clear();
	}
	
	void RuleEvaluator::clear()
	{
		valid_ = false;
		prefix_ = "";
		rule_map_.clear();
	}
	

	const String& RuleEvaluator::getPrefix() const
	{
		return prefix_;
	}

	bool RuleEvaluator::initialize(INIFile& file, const String& prefix)
	{
		// destroy the old rules
		rule_map_.clear();
		valid_ = false;

		// store the new prefix
		prefix_ = prefix;

		// check whether the INI file is valid
		if (!file.isValid())
		{
			// we didn't get a valid prefix file: abort
			return false;
		}

		// check for the sections and create the corresponding
		// Expressions
		for (Position i = 0; i < Element::NUMBER_OF_ELEMENTS; i++)
		{
			extractSection_(file, PTE[i].getSymbol());
		}

		// the last rule is a general rule
		extractSection_(file, "*");
	
		if (rule_map_.size() == 0)
		{
			Log.error() << "RuleEvaluator::initialize: no matching sections found for prefix " << prefix_ << endl;
		}
		
		// we create a map - done.
		valid_ = true;
		return true;
	}

	void RuleEvaluator::extractSection_(INIFile& file, const String& symbol)
	{
		// assemble the section name
		String section_name(prefix_ + ":" + symbol);

		// abort if the INI file does not contain the requested section
		if (!file.hasSection(section_name))
		{
			return;
		}

		// we have to create an entry in the hash map for the first
		// entry
		bool has_rules = false;

		// iterate over all lines of the respective section
		Position i = file.getSectionFirstLine(section_name);
		for (; i < file.getSectionLastLine(section_name); i++)
		{
			String line(*file.getLine(i));
			if (line.has('='))
			{
				String value = line.before("=");	
				String expression_string = line.after("=");
				expression_string.trim();
				if (expression_string != "")
				{
					// if this is the first expression for
					// this symbol, we create a new list in the hash map
					if (!has_rules)
					{
						// create a new entry for symbol
						rule_map_.insert(symbol, list<pair<Expression, String> >());
						// do not create the entry twice
						has_rules = true;
					}
					
					// push the expression into the list
					rule_map_[symbol].push_back(pair<Expression, String>(Expression(expression_string), value));
				}
			}
		}
	}

	String RuleEvaluator::operator () (const Atom& atom) const
	{
		// check whether we got a rule for this element
		String symbol = atom.getElement().getSymbol();
		RuleList::const_iterator it;

		// the return value
		String result = "";
		if (rule_map_.has(symbol))
		{
			// iterate over all rules in the list until the first rule
			// matches
			for (it = rule_map_[symbol].begin(); it != rule_map_[symbol].end(); ++it)
			{
				// check whether the expression matches for this atom
				if (it->first(atom))
				{
					// retrieve the return value
					result = it->second;

					// done with the iteration
					it = rule_map_[symbol].end();
				}
			}
		}
		
		// if no rule was applicable, check the default rule "*"
		if ((result == "") && (rule_map_.has("*")))
		{
			// iterate over all rules in the list until the first rule
			// matches
			for (it = rule_map_["*"].begin(); it != rule_map_["*"].end(); ++it)
			{
				// check whether the expression matches for this atom
				if (it->first(atom))
				{
					// retrieve the return value
					result = it->second;

					// done with the iteration
					it = rule_map_["*"].end();
				}
			}
		}
				
		// return the value
		return result;
	}

	const RuleEvaluator& RuleEvaluator::operator = (const RuleEvaluator& evaluator)
	{
		set(evaluator);
		return *this;
	}

	void RuleEvaluator::set(const RuleEvaluator& evaluator)
	{
		valid_ = evaluator.valid_;
		prefix_ = evaluator.prefix_;
		rule_map_ = evaluator.rule_map_;
	}

	bool RuleEvaluator::isValid() const
	{
		return valid_;
	}

	void RuleEvaluator::dump(std::ostream& s, Size indent_depth) const
	{
		// BAUSTELLE
	}
}
