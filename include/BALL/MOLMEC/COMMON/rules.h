// $Id: rules.h,v 1.1 2000/05/19 11:12:25 oliver Exp $
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
#include <pair>

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
		/**	Type definition for a hashmap containing the lists of rules.
		*/
		typedef StringHashMap<std::list<std::pair<Expression, String> > > RuleMap;
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
		~RuleEvaluator();

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

		/**	@name Debugging and Diagnostics
		*/
		//@{
		bool isValid() const;
		void dump(std::ostream& s = std::cout) const;
		//@}

		protected:

		/// The INI file section prefix
		String		prefix_;

		/// The map relating an element name and the corresponding list of expressions
		RuleMap		rule_map_;

		/// 
		bool			valid_;
	};

} // namespace BALL


#endif // BALL_MOLMEC_COMMON_RULES_H
