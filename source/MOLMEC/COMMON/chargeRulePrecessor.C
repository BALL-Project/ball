// $Id: chargeRulePrecessor.C,v 1.1 2000/05/24 11:48:32 anker Exp $

#include <BALL/MOLMEC/COMMON/chargeRuleProcessor.h>

namespace BALL
{
	ChargeRuleProcessor::ChargeRuleProcessor()
		: evaluator_.prefix("")
	{
		// BAUSTELLE: Muss hier der Praefix gesetzt werden?
	}

	ChargeRuleProcessor::ChargeRuleProcessor(INIFile& file,
			const String prefix)
		: evaluator_.prefix_(prefix)
	{
		// BAUSTELLE
	}

	ChargeRuleProcessor::ChargeRuleProcessor(
			const ChargeRuleProcessor& rule_processor)
		: evaluator_(rule_processor.evaluator_)
	{
	}

	ChargeRuleProcessor::~ChargeRuleProcessor()
	{
	}

	Processor::Result ChargeRuleProcessor::operator () (Atom& atom)
	{
		atom.setCharge(evaluate(atom));
	}
}
