// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: radiusRuleProcessor.h,v 1.9 2003/03/03 14:17:57 anhi Exp $
// Molecular Mechanics: rule-based assignment of radii

#ifndef BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#	include <BALL/MOLMEC/COMMON/ruleProcessor.h>
#endif

namespace BALL 
{
    /**  \addtogroup  MolmecAssignment
     *  @{
     */
	/**	Radius Rule Processor.
			<b>Definition:</b> BALL/MOLMEC/COMMON/radiusRuleProcessor.h
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
     /** @} */
} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RADIUSRULEPROCESSOR_H
