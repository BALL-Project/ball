// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_EXPRESSION_H
#define BALL_KERNEL_EXPRESSION_H

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
#	include <BALL/DATATYPE/stringHashMap.h>
#endif

#ifndef BALL_KERNEL_EXPRESSIONPARSER_H
#	include <BALL/KERNEL/expressionParser.h>
#endif

namespace BALL
{
	class Atom;
	class ExpressionTree;

	/**
	 * The class \ref Expression provides a frontend to \ref ExpressionTree.
	 * \par
	 * Expressions may be built from the following modules:
	 * <ul>
	 *   <li><tt>AND</tt> a conjunction
	 *   <li><tt>OR</tt>  a disjunction
	 *   <li><tt>!</tt>   a negation
	 *   <li><tt>predicate(argument) </tt> a predicate class that is derived from
	 *       \ref ExpressionPredicate and provides <tt>operator()(const \ref Atom& atom) const</tt>.
	 * </ul>
	 * Additionally brackets can be used for grouping. At least one bracket
	 * pair must exist which encloses the argument of a predicate.
	 * Empty arguments are allowed.
	 * \par
	 * <b>Example:</b>
	 * \code
	 * !residueID(12) AND (element(N) OR element(C))
	 * \endcode
	 * \par
	 * BALL offers various predefined ExpressionPredicates. These are:
	 * \par
	 * <table>
	 *   <tr><th>Syntax</th><th>Description</th></tr>
	 *   <tr><td><b> true()                  </b></td><td> always true </td></tr>
	 *   <tr><td><b> false()                 </b></td><td> always false </td></tr>
	 *   <tr><td><b> SMARTS(string)          </b></td><td> use a <a href="http://www.daylight.com/dayhtml/doc/theory/theory.smarts.html">SMARTS</a> expression </td></tr>
	 *   <tr><td><b> selected()              </b></td><td> true for already selected atoms </td></tr>
	 *   <tr><td><b> name(string)            </b></td><td> the name of an atom </td></tr>
	 *   <tr><td><b> type(string)            </b></td><td> type name of an atom </td></tr>
	 *   <tr><td><b> element(char)           </b></td><td> element (abbreviated by its symbol) </td></tr>
	 *   <tr><td><b> residue(string)         </b></td><td> name of a residue </td></tr>
	 *   <tr><td><b> residueID(string)       </b></td><td> PDB ID of the residue (usually a number) </td></tr>
	 *   <tr><td><b> protein(string)         </b></td><td> name of protein, the atom is contained in </td></tr>
	 *   <tr><td><b> secondaryStruct(string) </b></td><td> name of the secondary structure, the atom is contained in </td></tr>
	 *   <tr><td><b> chain(string)           </b></td><td> name of the chain, the atom is contained in </td></tr>
	 *   <tr><td><b> nucleotide(string)      </b></td><td> name of the nucleotide the atom is contained in</td></tr>
	 *   <tr><td><b> solvent()               </b></td><td> the atom is a solvent atom added by BALL </td></tr>
	 *   <tr><td><b> backbone()              </b></td><td> backbone atoms </td></tr>
	 *   <tr><td><b> inRing()                </b></td><td> part of a ring</td></tr>
	 *   <tr><td><b> doubleBonds()           </b></td><td> atoms with double bonds </td></tr>
	 *   <tr><td><b> tripleBonds()           </b></td><td> atoms with triple bonds </td></tr>
	 *   <tr><td><b> numberOfBonds(int)      </b></td><td> atoms with a given number of bonds </td></tr>
	 *   <tr><td><b> aromaticBonds()         </b></td><td> atoms with aromatic bonds </td></tr>
	 *   <tr><td><b> connectedTo(char)       </b></td><td> atoms which are connected to an atom (see \ref ConnectedToPredicate)</td></tr>
	 *   <tr><td><b> charge([op][number])    </b></td><td> select by charge, Possible operators are '&lt;'&nbsp; '&lt;='&nbsp; '='&nbsp; '&gt;='&nbsp; '&gt;'  </td></tr>
	 *   <tr><td><b> spHybridized()          </b></td><td> &nbsp; </td></tr>
	 *   <tr><td><b> sp2Hybridized()         </b></td><td> &nbsp; </td></tr>
	 *   <tr><td><b> sp3Hybridized()         </b></td><td> &nbsp; </td></tr>
	 *   <tr><td><b> charge(float)           </b></td><td> atoms with given charge </td></tr>
	 *   <tr><td><b> isAxial()               </b></td><td> &nbsp; </td></tr>
	 *   <tr><td><b> is4C1()                 </b></td><td> &nbsp; </td></tr>
	 * </table>
	 * \par
	 * \see ExpressionTree
	 *
   * \ingroup  Predicates
	 */
	class BALL_EXPORT Expression
	{
		public:

		BALL_CREATE(Expression)

		/**	@name	Type Definitions
		*/
		//@{

		/**	A creation method for predicates.
		*/
		typedef void * (*CreationMethod) ();

		//@}
		/**	@name	Constructors and Destructor
		*/
		//@{

		/** Default Constructor.
		*/
		Expression();

		/** Copy Constructor.
		*/
		Expression(const Expression& expression);

		/** Construct an Expression with a string
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		Expression(const String& expression_string);

		/** Destructor.
		 */
		virtual ~Expression();

		//@}
		/**	@name	Predicates
		*/
		//@{

		/**
		*/
		bool hasPredicate(const String& name) const;

		/** Equality operator 
		 */
		bool operator == (const Expression& expression) const;

		//@}
		/**	@name	Accessors
		*/
		//@{

		/** Evaluate the expression of <tt>atom</tt>
				@param atom
		*/
		virtual bool operator () (const Atom& atom) const;

		/**	Create a new predicate according to the name.
				If the predicate is not known, return 0.
				@param name the name of the predicate
				@param args the optional argument of the predicate
		*/
		ExpressionPredicate* getPredicate(const String& name,
		                                  const String& args = "") const;

		/**	Register a new predicate class.
		*/
		void registerPredicate(const String& name, CreationMethod creation_method);

		/** Set the expression and build a tree for it.
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		void setExpression(const String& expression);

		/** Get the expression string.
		*/
		const String& getExpressionString() const;

		/** Get the expression tree.
		*/
		const ExpressionTree* getExpressionTree() const;

		/** Get the creation methods.
		*/
		const StringHashMap<CreationMethod>& getCreationMethods() const;

		//@}
		/** @name Assignment 
		 */
		//@{ 

		/** Assignment operator 
		 */
		Expression& operator = (const Expression& expression);

		/** Clear method 
		 */
		virtual void clear();

		//@}

		protected:

		/*_ @name Protected methods
		*/
		//@{

		/*_ Construct the expression tree from the SyntaxTree
		 *  @throw Exception::ParseError if a syntax error was encountered
		 */
		ExpressionTree*	constructExpressionTree_(const ExpressionParser::SyntaxTree& tree);

		/*_ Register the predicates defined by default.
				See also: BALL/KERNEL/standardPredicates.h
		*/
		void registerStandardPredicates_();

		//@}
		/*_ @name Protected attributes
		*/
		//@{

		/*_ The methods to create the ExpressionPredicate instances
		*/
		StringHashMap<CreationMethod> create_methods_;

		/*_	The ExpressionTree constructed from the string.
				This tree contains the instances of the predicates.
		*/
		ExpressionTree*								expression_tree_;

		/*_ The string describing the expression.
		*/
		String												expression_string_;

		//@}
	};
}

#endif // BALL_KERNEL_EXPRESSION_H

