// $Id: radiusRuleProcessor.h,v 1.4 2001/02/18 20:51:19 amoll Exp $
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

		BALL_CREATE(RadiusRuleProcessor)

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		RadiusRuleProcessor();
			
		/**	Detailed constructor
		*/
		RadiusRuleProcessor(INIFile& file, const String& prefix = "RadiusRules");
			
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
