// $Id: typenameRuleProcessor.h,v 1.1 2000/05/19 11:12:26 oliver Exp $
// Molecular Mechanics: rule-based assignment of typenames 

#ifndef BALL_MOLMEC_COMMON_TYPENAMERULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_TYPENAMERULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{

	/**	
	*/
	class TypenameRuleProcessor
		:	public RuleProcessor
	{
		public:

		BALL_CREATE_NODEEP(TypenameRuleProcessor)

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
