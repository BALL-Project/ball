// $Id: typeRuleProcessor.h,v 1.4 2001/03/02 00:34:15 amoll Exp $
// Molecular Mechanics: rule-based assignment of types 

#ifndef BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{

	/**	Type Rule Processor.
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/typeRuleProcessor.h}
	*/
	class TypeRuleProcessor
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
