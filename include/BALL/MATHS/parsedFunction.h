// $Id: parsedFunction.h,v 1.1.2.2 2002/11/26 22:59:36 oliver Exp $

#ifndef BALL_MATHS_PARSEDFUNCTION_H
#define BALL_MATHS_PARSEDFUNCTION_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif

#include <numeric>

using std::unary_function;

namespace BALL
{

	extern HashMap<String, double*> *parsedFunctionConstants;
	extern HashMap<String, double (*)(double)> *parsedFunctionFunctions;
	
	/** @name parsedFunction.
	 		This function type contains a double - valued function string like
			$var1=atan(.5); sin(cos(atan(asin(exp(X+var1)))))$.
			When the operator () is called, $X$ is replaced with
			the argument of the operator call and the function string
			is parsed and evaluated.
	*/
	template <typename arg> 
	class ParsedFunction 
		: public unary_function<arg, double> 
	{
		public:
			
		/** @name Constructors and Destructors
		 */
		//@{

		/// Default constructor
		ParsedFunction()
			throw();

		/// Detailed constructor.
		ParsedFunction(String expression)
			throw();
		
		/// Copy constructor
		ParsedFunction(const ParsedFunction& func)
			throw();
		
		/// Destructor
		~ParsedFunction()
			throw();
		
		//@}
		
		/** @name Accessors
		 */
		//@{
		/** Evaluate the function at point $p$.
		 */
		double operator () (arg p)
			throw(Exception::ParseError);
		
		/** @name Parsing
		 */
		//@{
		/** This function initializes the function table and the constant table
		    of our parser.
			*/
		void initTable()
			throw();
		//@}

		/** The table of constants
		 */
		HashMap<String, double*> constants_;

		/** The table of functions
		 */
		HashMap<String, double (*)(double)> functions_;
		
		protected:
			String expression_;
	};
}

#endif // BALL_MATHS_PARSEDFUNCTION_H
