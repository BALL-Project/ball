// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK.C,v 1.3 2004/06/26 20:17:16 oliver Exp $
//

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/md5.h>
#include <BALL/COMMON/limits.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <BALL/COMMON/limits.h>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{

	typedef vector<pair<Size,Size> > pair_vec;
	typedef vector<vector<Size> > vec_size;

	//default constructor
	UCK::UCK()
	{
	}
	
	//constructor
	UCK::UCK(const Molecule& mol, Size d)
	{
		id_ = mol.getName();
		id_.trim();
		depth_ = d;
		makeUCK(mol);
	}
	
	// copy constructor
	UCK::UCK(UCK::UCK& uck)
	{
		depth_		= uck.getDepth();
		formula_	= uck.getFormula();
		uck_str_	= uck.getUCK();
		id_				= uck.getId();
		weight_		= uck.getWeight();
	}
	
	UCK::~UCK()
	{
	}
	
	void UCK::makePathMatrix(const pair_vec& e, vec_size& sp, const Size e_size)
	{
		vector<Size>* line;
		// create bond-matrix, because Floyd's Algorithm requires a reachability matrix
		vec_size* bond_matrix;
		line = new vector<Size>;
		bond_matrix = new vec_size;
		
		// initialize bond-matrix with 0 at every position
		for(Size i = 0; i != e_size; ++i)
		{
			line->clear();
			for(Size j = 0; j != e_size; ++j)
				line->push_back(0);
			bond_matrix->push_back(*line);
		}
		// proceed all edges and set corresponding position in bond_matrix to 1
		for(Size i = 0; i != e.size(); ++i)
			(*bond_matrix)[e[i].first][e[i].second] = 1;
		
		// initialize sp-matrix
		for(Size i = 0; i != bond_matrix->size(); ++i)
		{
			line->clear();
			for(Size j = 0; j != bond_matrix->size(); ++j)
			{
				if (i == j) // distance from a node to itself = 0
					line->push_back(0);
				else if((*bond_matrix)[i][j] == 1)	// if an edge exists between node i and j,
					line->push_back(1);								// the distance between them is 1
				else
						line->push_back(Limits<Index>::max()); // otherwise the distance is set to infinity
			}
			sp.push_back(*line);
		}
		
		// Floyd's Algorithm
		for(Size i = 0; i != sp.size(); ++i)
			for(Size j = 0; j != sp.size(); ++j)
				for(Size k = 0; k != sp.size(); k++)
					if(sp[j][k] > (sp[j][i] + sp[i][k]))
						sp[j][k] = (sp[j][i] + sp[i][k]);
		
		delete bond_matrix;
		delete line;
		return;
	}

	void UCK::getGraph(vector<String>& v, pair_vec& e, const Molecule& mol)
	{
		weight_ = 0.0;
		Size count = 0;
		vector<pair<String, Size> >* mol_name;
		mol_name = new vector<pair<String, Size> >;
		bool found_atom = false;

		for(AtomConstIterator atit1 = mol.beginAtom(); atit1 != mol.endAtom(); ++atit1)
		{
			// find chemical formula
			for(Size i = 0; i != mol_name->size(); ++i)
			{
				if((*mol_name)[i].first == atit1->getName())	// increase number of already existing molecules
				{
					(*mol_name)[i].second++;
					found_atom = true;
					break;
				}
			}
			
			if(!found_atom)	// add current atom to formula, if it doesn't exist
			{
				mol_name->push_back(make_pair(atit1->getName(),1));
				found_atom = false;
			}
			found_atom = false;
			
			weight_ += atit1->getElement().getAtomicWeight();
			v.push_back(atit1->getName());	// add atom-name to label-list
			Size dest = 0;
			// find bonds from current atom to all other atoms and store them in e
			for(AtomConstIterator atit2 = mol.beginAtom(); atit2 != mol.endAtom(); ++atit2)
			{
				if(atit1->getBond(*atit2)!=0)
					e.push_back(make_pair(count, dest));
				++dest;
			}
			++count;
		}
		sort(mol_name->begin(), mol_name->end());		// sort vector mol_name in order to get the lexicographically ordered
		for(Size i = 0; i != mol_name->size(); ++i)	// chemical formula
			formula_ += ((*mol_name)[i].first)+(String)(*mol_name)[i].second;
			
		delete mol_name;
		return;
	}
	
	String UCK::eraseDoubleLabels(Size d, String label, String x)
	{
		if(d>=2)
			if(x.find(label)!=string::npos)
				x = x.substr(0,x.find(label)) + x.substr(x.find(label)+label.size());

		return x;
	}
	
	String UCK::lambda(String lambda_d, const pair_vec& e, const vector<String>& v, Size i, Size d)
	{
		lambda_d = v[i]; // fix label
		vector<String>* lam;
		lam = new vector<String>;
		
		if(d==0) // depth 0 is reached, return the label written in new_label
		{
			delete lam;
			return lambda_d;
		}
		
		else	// d!=0
		{
			// compute lambda_d-1_labels for all children
			for(pair_vec::const_iterator it = e.begin(); it != e.end(); ++it)
				if(it->first!=i)	// if source node in e is not equal to the current position i, then skip this edge
					continue;
				else	// an edge to another node is found, so compute lambda_d-1 of the child and store the resulting string
							// in vector lam
					lam->push_back(eraseDoubleLabels(d, v[i], lambda("", e, v, it->second, d-1)));
			sort(lam->begin(), lam->end()); // lexicographically order the lambda_d-1 -labels
		}
		// concatenate lambda_d-1 -labels and produce lambda_d -label
		for(vector<String>::iterator it = lam->begin(); it != lam->end(); ++it)
			lambda_d += *it;

		delete lam;
		return lambda_d;
	}
	
	void UCK::makePairs(const vector<String>& lambda_map, vector<String>& pairs, const vec_size& sp)
	{
		for(Size i = 0; i != lambda_map.size(); ++i)
			for(Size j = 0; j != lambda_map.size(); ++j)
				pairs.push_back(lambda_map[i] + (String)sp[i][j] + lambda_map[j]);

		sort(pairs.begin(), pairs.end());
		return;
	}
	
	void UCK::createFinalString(const vector<String>& pairs)
	{
		String uckstr = "";
		uck_str_ = formula_;
		uck_str_ += "-";
		for(Size i = 0; i != pairs.size(); ++i)
			uck_str_ += pairs[i];
			
		uck_str_ += "\n";
		uck_str_ = MD5String((char*) uck_str_.c_str()); // RSA Data Security, Inc. MD5 Message-Digest Algorithm
		return;
	}
	
	Size UCK::getDepth()
	{
		return depth_;
	}

	String UCK::getFormula()
	{
		return formula_;
	}

	String UCK::getUCK()
	{
		return uck_str_;
	}

	String UCK::getId()
	{
		return id_;
	}

	float UCK::getWeight()
	{
		return weight_;
	}

	void UCK::printUCK(ostream& outstr)	
	{
		outstr<<id_<<formula_<<"	"<<uck_str_<<endl;
		return;
	}

	void UCK::makeUCK(const Molecule& m)
	{
		vector<String> v, pairs, lambda_map;
		pair_vec e; // edge set
		vec_size sp;
		
		getGraph(v, e, m);
		
		for(Size i=0; i!=v.size(); ++i)
			lambda_map.push_back(lambda("", e, v, i, depth_));
		
		makePathMatrix(e, sp, v.size());
		makePairs(lambda_map, pairs, sp);
		createFinalString(pairs);
		return;
	}
} // namespace BALL
