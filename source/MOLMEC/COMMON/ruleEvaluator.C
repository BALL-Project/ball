
// $Id: ruleEvaluator.C,v 1.10 2001/07/11 00:12:56 oliver Exp $

#include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/KERNEL/PTE.h>

//#define DEBUG_RULEEVALUATOR

using namespace std;

namespace BALL 
{

	RuleEvaluator::RuleEvaluator() throw()
		:	prefix_(),
			rule_map_(),
			valid_(false)
	{
	}


	RuleEvaluator::RuleEvaluator(INIFile& file, const String& prefix) throw()
		:	prefix_(),
			rule_map_(),
			valid_(false)
	{
		valid_ = initialize(file, prefix);
	}


	RuleEvaluator::RuleEvaluator(const RuleEvaluator& evaluator) throw()
		:	prefix_(evaluator.prefix_),
			rule_map_(evaluator.rule_map_),
			valid_(evaluator.valid_)
	{
	}


	RuleEvaluator::~RuleEvaluator() throw()
	{
		clear();
	}

	
	void RuleEvaluator::clear() 
		throw()
	{
		prefix_ = "";
		rule_map_.clear();

		valid_ = false;
	}
	

	const String& RuleEvaluator::getPrefix() const throw()
	{
		return prefix_;
	}


	void RuleEvaluator::setPrefix(const String& prefix) throw()
	{
		prefix_ = prefix;
	}


	bool RuleEvaluator::initialize
		(INIFile& file, const String& prefix) throw()
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
		throw()
	{
		// assemble the section name
		String section_name(prefix_ + ":" + symbol);

		// abort if the INI file does not contain the requested section
		if (!file.hasSection(section_name))
		{
			return;
		}

		// create a new entry for symbol
		if (!rule_map_.has(symbol))
		{
			rule_map_.insert(symbol, list<pair<Expression, String> >());
		}

		// iterate over all lines of the respective section
		INIFile::LineIterator it = file.getSectionFirstLine(section_name);
		++it;//skip section line
		for (; +it ; it.getSectionNextLine())
		{
			String line(*it);
			// empty lines or comment lines (starting with ';' or '#') are ignored
			if (line.has('=') && (line[0] != ';') && (line[0] != '#'))
			{
				if (line[0] == '=')
				{
					Log.error() << "RuleEvaluator:: invalid rule in line: " << line << endl;
					continue;
				}

				String value = line.before("=");	
				String expression_string;
				if (line.after("=").isValid())
				{
					expression_string = line.after("=");
				}
				expression_string.trim();
				value.trim();

				// push the expression into the list
				rule_map_[symbol].push_back(pair<Expression, String>(Expression(expression_string), value));
			}
		}
	}


	String RuleEvaluator::operator () (const Atom& atom) const throw()
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
					#ifdef DEBUG_RULEEVALUATOR
						Log.info() << "atom "<< atom.getFullName() << " matches rule " << it->first.getExpression() << endl;
					#endif
					// retrieve the return value
					result = it->second;
					break;
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
					
					// and exit the loop
					break;
				}
			}
		}
				
		// return the value
		return result;
	}


	bool RuleEvaluator::operator == (const RuleEvaluator& evaluator) const
		throw()
	{
		return ((prefix_ == evaluator.prefix_)
			&& (rule_map_ == evaluator.rule_map_)
			&& (valid_ == evaluator.valid_));
	}


	const RuleEvaluator& RuleEvaluator::operator = 
		(const RuleEvaluator& evaluator) throw()
	{
		valid_ = evaluator.valid_;
		prefix_ = evaluator.prefix_;
		rule_map_ = evaluator.rule_map_;

		return *this;
	}


	bool RuleEvaluator::isValid() const throw()
	{
		return valid_;
	}


	void RuleEvaluator::dump(std::ostream& /* s */, Size /* indent_depth */)
		const throw()
	{
		// BAUSTELLE
	}
}
