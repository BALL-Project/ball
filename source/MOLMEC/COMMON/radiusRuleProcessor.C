// $Id: radiusRuleProcessor.C,v 1.1 2000/05/24 11:48:32 anker Exp $

#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>

namespace BALL
{
	RadiusRuleProcessor::RadiusRuleProcessor()
		: evaluator_.prefix("")
	{
		// BAUSTELLE: Muss hier der Praefix gesetzt werden?
	}

	RadiusRuleProcessor::RadiusRuleProcessor(INIFile& file,
			const String prefix)
		: evaluator_.prefix_(prefix)
	{
		// BAUSTELLE
	}

	RadiusRuleProcessor::RadiusRuleProcessor(
			const RadiusRuleProcessor& rule_processor)
		: evaluator_(rule_processor.evaluator_)
	{
	}

	RadiusRuleProcessor::~RadiusRuleProcessor()
	{
	}

	Processor::Result RadiusRuleProcessor::operator () (Atom& atom)
	{
		atom.setRadius(evaluate(atom));
	}
}
