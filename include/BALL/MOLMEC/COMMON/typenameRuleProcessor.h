// $Id: typenameRuleProcessor.h,v 1.4 2001/03/02 00:34:16 amoll Exp $
// Molecular Mechanics: rule-based assignment of typenames 

#ifndef BALL_MOLMEC_COMMON_TYPENAMERULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_TYPENAMERULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{

	/**	Typename Rule Processor.
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/typenameRuleProcessor.h}
	*/
	class TypenameRuleProcessor
		:	public RuleProcessor
	{
		public:

		BALL_CREATE(TypenameRuleProcessor)

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		TypenameRuleProcessor();
			
		/**	Detailed constructor
		*/
		TypenameRuleProcessor(INIFile& file, const String& prefix);
			
		/**	Copy constructor
		*/
		TypenameRuleProcessor(const TypenameRuleProcessor& rule_processor);

		/**	Destructor
		*/
		~TypenameRuleProcessor();

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


#endif // BALL_MOLMEC_COMMON_TYPENAMERULEPROCESSOR_H
