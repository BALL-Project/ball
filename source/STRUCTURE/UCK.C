// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: UCK.C,v 1.1 2004/06/15 09:13:09 bender Exp $
//

#include <BALL/STRUCTURE/UCK.h>
#include <BALL/STRUCTURE/md5.h>

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <BALL/FORMAT/SDFile.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/fragment.h>
#include <BALL/KERNEL/PTE.h>

using namespace std;

namespace BALL
{
	//default constructor
	UCK::UCK()
	{
	}
	
	//constructor
	UCK::UCK(Molecule* mol, String file, Size d, Size molnumber)
	{
		mol_number_ = molnumber;
		filename_ = file;
		depth_ = d;
		makeUck(mol);
	}
	
	UCK::~UCK()
	{
	}
	
	void UCK::makePathMatrix(vector<pair<Size,Size> >& e, vector<vector<int> >& sp, Size e_size)
	{
		vector<int>* line;
		// create bond-matrix, because Floyd's Algorithm requires a reachability matrix
		vector<vector<int> >* bond_matrix;
		line = new vector<int>;
		bond_matrix = new vector<vector<int> >;
		
		for(Size i = 0; i != e_size; ++i)
		{
			line->clear();
			for(Size j = 0; j != e_size; ++j)
				line->push_back(0);
			bond_matrix->push_back(*line);
		}
		for(Size i = 0; i != e.size(); ++i)
			(*bond_matrix)[e[i].first][e[i].second] = 1;
			
		for(Size i = 0; i != bond_matrix->size(); ++i)
		{
			line->clear();
			for(Size j = 0; j != bond_matrix->size(); ++j)
			{
				if (i == j) 
					line->push_back(0);
				else if((*bond_matrix)[i][j] == 1)
					line->push_back(1);
				else
					line->push_back(9999999); // infinity???
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

	void UCK::getGraph(vector<String>& v, vector<pair<Size,Size> >& e, Molecule* mol)
	{
		weight_ = 0.0;
		Size count = 0;
		vector<pair<String, int> >* mol_name;
		mol_name = new vector<pair<String, int> >;
		bool found_atom = false;

		for(AtomIterator atit1 = mol->beginAtom(); atit1 != mol->endAtom(); ++atit1)
		{
			// find chemical formula
			for(Size i = 0; i != mol_name->size(); ++i)
			{
				if((*mol_name)[i].first == atit1->getName())
				{
					(*mol_name)[i].second++;
					found_atom = true;
					break;
				}
			}
			if(!found_atom)
			{
				mol_name->push_back(make_pair(atit1->getName(),1));
				found_atom = false;
			}
			found_atom = false;
			
			weight_ += atit1->getElement().getAtomicWeight();
			v.push_back(atit1->getName());
			Size dest = 0;
			for(AtomIterator atit2 = mol->beginAtom(); atit2 != mol->endAtom(); ++atit2)
			{
				if(atit1->getBond(*atit2)!=0)
					e.push_back(make_pair(count, dest));
				++dest;
			}
			++count;
		}
		sort(mol_name->begin(), mol_name->end());
		for(Size i = 0; i != mol_name->size(); ++i)
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
	
	String UCK::lambda(String tmp, vector<pair<Size,Size> >& e, vector<String>& v, Size i, Size d)
	{
		tmp = v[i]; // fix label
		vector<String>* lam;
		lam = new vector<String>;
		
		if(d==0)
		{
			delete lam;
			return tmp;
		}
		
		else
		{
			for(vector<pair<Size,Size> >::iterator it = e.begin(); it != e.end(); ++it)
				if(it->first!=i)
					continue;
				else
					lam->push_back(eraseDoubleLabels(d, v[i], lambda("", e, v, it->second, d-1)));
			sort(lam->begin(), lam->end());
		}
		for(vector<String>::iterator it = lam->begin(); it != lam->end(); ++it)
			tmp += *it;

		delete lam;
		return tmp;
	}
	
	void UCK::makePairs(vector<String>& lambda_map, vector<String>& pairs, vector<vector<int> >& sp)
	{
		for(Size i = 0; i != lambda_map.size(); ++i)
			for(Size j = 0; j != lambda_map.size(); ++j)
				pairs.push_back(lambda_map[i] + (String)sp[i][j] + lambda_map[j]);

		sort(pairs.begin(), pairs.end());
		return;
	}
	
	String UCK::createFinalString(vector<String>& pairs)
	{
		String uckstr = "";
		uckstr = formula_;
		uckstr += "-";
		for(Size i = 0; i != pairs.size(); ++i)
			uckstr += pairs[i];
			
		uckstr += "\n";
		uckstr = MD5String((char*) uckstr.c_str()); // RSA Data Security, Inc. MD5 Message-Digest Algorithm
		return uckstr;
	}
	
	String UCK::getUck()
	{
		return uck_str_;
	}

	String UCK::getFormula()
	{
		return formula_;
	}

	String UCK::getId()
	{
		return id_;
	}

	float UCK::getWeight()
	{
		return weight_;
	}
	void UCK::printUck(ofstream& outfile)
	{
		outfile<<id_<<formula_<<"	"<<uck_str_<<endl;
		return;
	}

	void UCK::printUck(ostream& outstr)	
	{
		outstr<<id_<<formula_<<"	"<<uck_str_<<endl;
		return;
	}

	void UCK::makeUck(Molecule* m)
	{
		vector<String> *v, *pairs, *lambda_map;
		vector<pair<Size,Size> > *e; // edge set
		vector<vector<int> > *sp;
		v = new vector<String>;
		pairs = new vector<String>;
		lambda_map = new vector<String>;
		e = new vector<pair<Size,Size> >;
		sp = new vector<vector<int> >;
		
		getGraph(*v, *e, m);
		for(Size i=0; i!=v->size(); ++i)
			lambda_map->push_back(lambda("", *e, *v, i, depth_));
		makePathMatrix(*e, *sp, v->size());
		makePairs(*lambda_map, *pairs, *sp);
		uck_str_ = createFinalString(*pairs);
		
		// make sure that tabs are placed correctly when m->getName() is empty
		String m_name(m->getName());
		m_name.trim();
		if(m_name.size() == 0)
			id_ = filename_+":"+(String) mol_number_+":			";
		else
			id_ = filename_+":"+(String) mol_number_+":"+m_name+"		";

		
		++mol_number_;
		delete v;
		delete pairs;
		delete lambda_map;
		delete e;
		delete sp;
		return;
	}
	
} // namespace BALL
