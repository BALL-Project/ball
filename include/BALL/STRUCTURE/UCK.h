// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK.h,v 1.3 2005/02/23 13:46:57 oliver Exp $
//

#ifndef BALL_STRUCTURE_UCK_H
#define BALL_STRUCTURE_UCK_H

#ifndef BALL_FORMAT_SDFILE_H
#	include <BALL/FORMAT/SDFile.h>
#endif
#ifndef BALL_KERNEL_MOLECULE_H
#include <BALL/KERNEL/molecule.h>
#endif
#include <fstream>
#include <vector>

namespace BALL
{
	/** Unique Chemical Key.
			Implements the UCK algorithm for computing a unique key for a given chemical structure.
	*/
	class UCK
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
		
		/* copy constructor
		*/
		UCK(UCK& uck);
		
		/* destructor
		*/
		~UCK();
		
		/* print Uck to std::out
		*/
		void printUCK(std::ostream& outstr);

		Size getDepth();

		String getFormula();

		String getUCK();

		String getId();

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
		String formula_, uck_str_, id_;
		float weight_;
	};

}//namespace
#endif // BALL_STRUCTURE_UCK_H
