// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_UCK_H
#define BALL_STRUCTURE_UCK_H

#ifndef BALL_DATATYPE_STRING_H
	#include <BALL/DATATYPE/string.h>
#endif

#include <vector>

namespace BALL
{
	class Molecule;

	/** Unique Chemical Key.
			Implements the UCK algorithm for computing a unique key for a given chemical structure.
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT UCK
	{
		public:

		/**	@name Type definitions */
		//@{
		typedef std::vector<std::pair<Size, Size> > PairVector;
		typedef std::vector<std::vector<Size> >			SizeVector;
		//@}

		/* default constructor
		*/
		UCK();
		
		/* constructor
		*/
		UCK(const Molecule& mol, Size d=3);
		
		/* constructor added originally in CADDSuite
		 */
		UCK(const Molecule& mol, bool ignore_hydrogens, Size d=3);

		/* copy constructor
		*/
		UCK(UCK& uck);
		
		/* destructor
		*/
		virtual ~UCK();
		
		/* print Uck to std::out
		*/
		void printUCK(std::ostream& outstr);

		Size getDepth();

		const String& getFormula() const;

		const String& getUCK() const;

		const String& getId() const;

		float getWeight();
		
		private:
		
		/* variation of UCK-Algorithm:
		* delete one occurance of the current label from the lambda string
		* this reduces the length of the constructed string
		*/
		String eraseDoubleLabels(const Size d, String x, String label);
		
		/* computes the uck
		*/
		void makeUCK(const Molecule& m);
		
		/* construct graph-representation of the molecule read
		*/
		void getGraph(std::vector<String>& v, PairVector& e, const Molecule& mol);
		
		/* Floyd's Algorithm
		*  find shortest paths between all pairs of nodes
		*/
		void makePathMatrix(const PairVector& e, SizeVector& sp, const Size e_size);
		
		/* compute concatenated strings [lambda(a)nlambda(b)] for every pair of nodes
		*/
		void makePairs(const std::vector<String>& lambda_map, std::vector<String>& pairs, const std::vector<vector<Size> >& sp);
		
		/* compute lambda-map
		*/
		String lambda(String lambda_d, const PairVector& e, const std::vector<String>& v, Size pos, Size d);
		
		/* construct final UCK as follows:
		*  chemical_formula-lexicographically ordered collection of strings pair(a,b)
		*/
		void createFinalString(const std::vector<String>& pairs);

		Size depth_;
		String formula_; 
		String uck_str_; 
		String id_;
		float weight_;
		bool ignore_hydrogens_;
	};

}//namespace
#endif // BALL_STRUCTURE_UCK_H
