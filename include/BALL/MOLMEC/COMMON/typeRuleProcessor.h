// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: typeRuleProcessor.h,v 1.12 2005/12/23 17:01:52 amoll Exp $
//

// Molecular Mechanics: rule-based assignment of types 

#ifndef BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{
	/**	Type Rule Processor.
			
    	\ingroup  MolmecAssignment
	*/
	class BALL_EXPORT TypeRuleProcessor
		:	public RuleProcessor
	{
		public:

		BALL_CREATE(TypeRuleProcessor)

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		TypeRuleProcessor();
			
		/**	Detailed constructor
		*/
		TypeRuleProcessor(INIFile& file, const String& prefix);
			
		/**	Copy constructor
		*/
		TypeRuleProcessor(const TypeRuleProcessor& rule_processor);

		/**	Destructor
		*/
		~TypeRuleProcessor();

		//@}
		/**	@name Processor related methods
		*/
		//@{

		/**
		*/
		virtual Processor::Result operator () (Atom& atom);

		//@}

	};
} // namespace BALL


#endif // BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
