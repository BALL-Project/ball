// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK.h,v 1.2 2004/06/24 16:03:26 bender Exp $
//

#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <fstream>
namespace BALL
{
	class UCK
	{
		public:
		
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
		void getGraph(vector<String>& v, vector<std::pair<Size,Size> >& e, const Molecule& mol);
		
		/* Floyd's Algorithm
		*  find shortest paths between all pairs of nodes
		*/
		void makePathMatrix(const vector<std::pair<Size,Size> >& e, vector<vector<Size> >& sp, const Size e_size);
		
		/* compute concatenated strings [lambda(a)nlambda(b)] for every pair of nodes
		*/
		void makePairs(const vector<String>& lambda_map, vector<String>& pairs, const vector<vector<Size> >& sp);
		
		/* compute lambda-map
		*/
		String lambda(String lambda_d, const vector<std::pair<Size,Size> >& e, const vector<String>& v, Size pos, Size d);
		
		/* construct final UCK as follows:
		*  chemical_formula-lexicographically ordered collection of strings pair(a,b)
		*/
		void createFinalString(const vector<String>& pairs);

		Size depth_;
		String formula_, uck_str_, id_;
		float weight_;
	};
}//namespace
