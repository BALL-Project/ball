// $Id: typeRuleProcessor.h,v 1.1 2000/05/29 11:17:20 oliver Exp $
// Molecular Mechanics: rule-based assignment of types 

#ifndef BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_TYPERULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{

	/**	
	*/
	class TypeRuleProcessor
		:	public RuleProcessor
	{
		public:

		BALL_CREATE_NODEEP(TypeRuleProcessor)

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
