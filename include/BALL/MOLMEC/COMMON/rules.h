// $Id: rules.h,v 1.4 2000/05/19 15:59:31 oliver Exp $
// Molecular Mechanics: rule-based assignment of properties (typenames, charges, radii, etc.)

#ifndef BALL_MOLMEC_COMMON_RULES_H
#define BALL_MOLMEC_COMMON_RULES_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_FORMAT_INIFILE_H
#	include <BALL/FORMAT/INIFile.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

#include <list>
#include <utility>

namespace BALL 
{


	/**	
	*/
	class RuleEvaluator
	{
		public:

		BALL_CREATE_NODEEP(RuleEvaluator)

		/**	Type Definitions
		*/
		//@{
		/** Type definition for a list containing rules.
		*/
		typedef std::list<std::pair<Expression, String> > RuleList;
		/**	Type definition for a hashmap containing the lists of rules.
		*/
		typedef StringHashMap<RuleList> RuleMap;
		//@}

		/**	Constructors and Destructors
		*/
		//@{
		/**	Default constructor
		*/
		RuleEvaluator();
			
		/**	Detailed constructor
		*/
		RuleEvaluator(INIFile& file, const String& prefix);
			
		/**	Copy constructor
		*/
		RuleEvaluator(const RuleEvaluator& evaluator);

		/**	Destructor
		*/
		virtual ~RuleEvaluator();

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

		/**	Return the prefix of the INI file sections
		*/
		const String& getPrefix() const;
			
		//@}

		/**	@name	Assignment
		*/
		//@{
			
		/**
		*/
		const RuleEvaluator& operator = (const RuleEvaluator& evaluator);

		/**	
		*/
		void set(const RuleEvaluator& evaluator);
		//@}

		/**	@name Predicates
		*/
		//@{
			
		/**	Rule evaluation.
				Evaluate all matching rules (in the correct order) and return
				the corresponding value. If no rule matches, an empty string i
				returned.
		*/
		String operator () (const Atom& atom) const;
		//@}

		/**	@name Debugging and Diagnostics
		*/
		//@{
		/**	
		*/
		bool isValid() const;

		/**	
		*/
		void dump(std::ostream& s = std::cout, Size indent_depth = 0) const;
		//@}

		protected:

		/// parse the section with name predicate_ + ":" + symbol of file
		void extractSection_(INIFile& file, const String& symbol);

		/// The INI file section prefix
		String		prefix_;

		/// The map relating an element name and the corresponding list of expressions
		RuleMap		rule_map_;

		/// 
		bool			valid_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RULES_H
