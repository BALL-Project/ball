// $Id: ruleProcessor.h,v 1.2 2000/05/24 12:13:43 anker Exp $
// Molecular Mechanics: rule-based assignment of properties (typenames, charges, radii, etc.)

#ifndef BALL_MOLMEC_COMMON_RULEPROCESSOR_H
#define BALL_MOLMEC_COMMON_RULEPROCESSOR_H

#ifndef BALL_MOLMEC_COMMON_RULES_H
#	include <BALL/MOLMEC/COMMON/rules.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

namespace BALL 
{

	/**	
	*/
	class RuleProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		BALL_CREATE_NODEEP(RuleProcessor)

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
		virtual Processor::Result operator () (const Atom& atom);

		/** 
		*/
		String evaluate(const Atom& atom);

		//@}

		/**	@name Debugging and Diagnostics
		*/
		//@{
		bool isValid() const;
		void dump(std::ostream& s = std::cout) const;
		//@}

		protected:

		///
		RuleEvaluator	evaluator_;

		/// 
		bool					valid_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RULEPROCESSOR_H
