// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK.h,v 1.1 2004/06/15 09:13:08 bender Exp $
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
		UCK(Molecule* mol, String file, Size d, Size molnumber);
		
		/* destructor
		*/
		~UCK();
		
		/* print Uck to outfile
		*/
		void printUck(std::ofstream& outfile);
		
		/* print Uck to std::out
		*/
		void printUck(std::ostream& outstr);

		String getUck();

		String getFormula();

		String getId();

		float getWeight();
		
		private:
		
		/* variation of UCK-Algorithm:
		* delete one occurance of the current label from the lambda string
		* this reduces the length of the constructed string
		*/
		String eraseDoubleLabels(Size d, String x, String label);
		
		/* computes the uck
		*/
		void makeUck(Molecule* m);
		
		/* construct graph-representation of the molecule read
		*/
		void getGraph(vector<String>& v, vector<std::pair<Size,Size> >& e, Molecule* mol);
		
		/* Floyd's Algorithm
		*  find shortest paths between all pairs of nodes
		*/
		void makePathMatrix(vector<std::pair<Size,Size> >& e, vector<vector<int> >& sp, Size e_size);
		
		/* compute mu-map, i.e. concatenatet string [lambda(a)nlambda(b)]
		*/
		void makePairs(vector<String>& lambda_map, vector<String>& pairs, vector<vector<int> >& sp);
		
		/* compute lambda-map
		*/
		String lambda(String tmp, vector<std::pair<Size,Size> >& e, vector<String>& v, Size pos, Size d);
		
		/* construct final UCK as follows:
		*  chemical_formula-lexicographically ordered collection of strings pair(a,b)
		*/
		String createFinalString(vector<String>& pairs);

		Size depth_, mol_number_;
		String filename_, formula_, uck_str_, id_;
		float weight_;
	};
}//namespace
