// $Id: ruleEvaluator.h,v 1.5 2000/10/28 15:39:14 anker Exp $
// Molecular Mechanics: rule-based assignment of properties (typenames, charges, radii, etc.)

#ifndef BALL_MOLMEC_COMMON_RULEEVALUATOR_H
#define BALL_MOLMEC_COMMON_RULEEVALUATOR_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_EXPRESSION_H
#	include <BALL/KERNEL/expression.h>
#endif

#include <list>
#include <utility>

namespace BALL 
{

	// forward declarations
	class INIFile;

	/**	Rule evaluator class.
			This class provides means for evaluating rules on Atoms. It is a
			helper class of RuleProcessors. Rules define
			values depending on atom constellations. They are defined in an
			INIFile.
			\\
			@see {RuleProcessor}
			\\
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/ruleEvaluator.h}
	*/
	class RuleEvaluator
	{
		public:

		BALL_CREATE(RuleEvaluator)

		/**	@name Type Definitions */

		//@{
		/** Type definition for a list containing rules.
		*/
		typedef std::list<std::pair<Expression, String> > RuleList;

		/**	Type definition for a hashmap containing the lists of rules.
		*/
		typedef StringHashMap<RuleList> RuleMap;

		//@}


		/**	@name Constructors and Destructor
		*/

		//@{
		/**	Default constructor
		*/
		RuleEvaluator() throw();
			
		/**	Detailed constructor
		*/
		RuleEvaluator(INIFile& file, const String& prefix) throw();
			
		/**	Copy constructor
		*/
		RuleEvaluator(const RuleEvaluator& evaluator) throw();

		/**	Destructor
		*/
		virtual ~RuleEvaluator() throw();

		//@}
		

		/**	@name	Accessors
		*/
		//@{

		/**
		*/
		bool initialize(INIFile& file, const String& prefix) throw();

		/**	Return the prefix of the INI file sections
		*/
		const String& getPrefix() const throw();
			
		/**	Set the prefix of the INI file sections
		*/
		void setPrefix(const String& prefix) throw();
			
		//@}


		/**	@name	Assignment
		*/
		//@{
			
		/** Assignment operator
		*/
		const RuleEvaluator& operator = (const RuleEvaluator& evaluator)
		throw();

		/** Clear method
		*/
		virtual void clear() throw();

		//@}


		/**	@name Predicates
		*/
		//@{
			
		/**	Rule evaluation.
				Evaluate all matching rules (in the correct order) and return
				the corresponding value. If no rule matches, an empty string is
				returned.
		*/
		String operator () (const Atom& atom) const throw();

		/** Equality operator */
		bool operator == (const RuleEvaluator& evaluator) const throw();

		//@}


		/**	@name Debugging and Diagnostics
		*/

		//@{
		/**	
		*/
		bool isValid() const throw();

		/**	
		*/
		void dump(std::ostream& s = std::cout, Size indent_depth = 0) const
		throw();

		//@}


		protected:

		/// parse the section with name predicate_ + ":" + symbol of file
		void extractSection_(INIFile& file, const String& symbol) throw();

		/// The INI file section prefix
		String		prefix_;

		/// The map relating an element name and the corresponding list of expressions
		RuleMap		rule_map_;

		/// 
		bool			valid_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RULEEVALUATOR_H
