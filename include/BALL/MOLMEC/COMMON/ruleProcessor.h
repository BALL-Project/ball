// $Id: ruleProcessor.h,v 1.8 2001/07/11 23:07:56 amoll Exp $
// Molecular Mechanics: rule-based assignment of properties (typenames, charges, radii, etc.)

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_RULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULEEVALUATOR_H
#	include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

namespace BALL 
{

	/**	Rule Processor class.
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/ruleProcessor.h}
	*/
	class RuleProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		BALL_CREATE(RuleProcessor)

		/**	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		RuleProcessor();
			
		/**	Detailed constructor
		*/
		RuleProcessor(INIFile& file, const String& prefix);
			
		/**	Copy constructor
		*/
		RuleProcessor(const RuleProcessor& rule_processor);

		/**	Destructor
		*/
		~RuleProcessor();

		/**
		*/
		void clear();
			
		/**
		*/
		void destroy();			

		//@}
		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		bool initialize(INIFile& file, const String& prefix);

		//@}
		/**	@name	Assignment
		*/
		//@{
			
		/**
		*/
		const RuleProcessor& operator = (const RuleProcessor& rule_processor);

		/**	
		*/
		void set(const RuleProcessor& rule_processor);

		//@}
		/**	@name Processor related methods
		*/
		//@{

		/**
		*/
		virtual bool start();

		/**
		*/
		virtual bool finish();

		/** 
		*/
		virtual Processor::Result operator () (Atom& atom);

		/** 
		*/
		String evaluate(const Atom& atom);

		//@}
		/**	@name Debugging and Diagnostics
		*/
		//@{

		///
		bool isValid() const;
		
		///
		void dump(std::ostream& s = std::cout) const;

		//@}

		protected:

		//_
		RuleEvaluator	evaluator_;

		//_ 
		bool					valid_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RULEPROCESSOR_H
