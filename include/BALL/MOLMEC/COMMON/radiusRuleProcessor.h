// $Id: radiusRuleProcessor.h,v 1.1 2000/05/19 11:12:24 oliver Exp $
// Molecular Mechanics: rule-based assignment of radii

#ifndef BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{

	/**	
	*/
	class RadiusRuleProcessor
		:	public RuleProcessor
	{
		public:

		BALL_CREATE_NODEEP(RadiusRuleProcessor)

		/**	Constructors and Destructors
		*/
		//@{
		/**	Default constructor
		*/
		RadiusRuleProcessor();
			
		/**	Detailed constructor
		*/
		RadiusRuleProcessor(INIFile& file, const String& prefix);
			
		/**	Copy constructor
		*/
		RadiusRuleProcessor(const RadiusRuleProcessor& rule_processor);

		/**	Destructor
		*/
		~RadiusRuleProcessor();

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


#endif // BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
