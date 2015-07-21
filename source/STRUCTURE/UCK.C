// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/UCK.h>

#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/atom.h>

#include <QtCore/QCryptographicHash>

#include <BALL/QSAR/aromaticityProcessor.h>
#include <BALL/STRUCTURE/molecularSimilarity.h>

#include <openbabel/mol.h>
#include <openbabel/canon.h>
#include <openbabel/graphsym.h>


using namespace std;

namespace BALL
{
	
	// ????
	typedef vector<pair<Size,Size> > PairVector;
	typedef vector<vector<Size> > SizeVector;

	//default constructor
	UCK::UCK()
		: 	depth_(0),
			weight_(0.0),
			ignore_hydrogens_(false)
	{
	}
	
	//constructor
	UCK::UCK(const Molecule& mol, Size d)
		:	depth_(d),
			weight_(0.0),
			ignore_hydrogens_(false)
	{
		id_ = mol.getName();
		id_.trim();
		depth_ = d;
		makeUCK(mol);
	}

	//constructor (originally included in CADDSuite)
	UCK::UCK(const Molecule& mol, bool ignore_hydrogens, Size d)
		:	depth_(d),
			weight_(0.0)
	{
		id_ = mol.getName();
		id_.trim();
		depth_ = d;
		ignore_hydrogens_ = ignore_hydrogens;
		makeUCK(mol);
	}
	
	// copy constructor
	UCK::UCK(UCK& uck)
		:	depth_(uck.depth_),
			formula_(uck.formula_),
			uck_str_(uck.uck_str_),
			id_(uck.id_),
			weight_(uck.weight_)
	{
	}
	
	UCK::~UCK()
	{
	}
	
	void UCK::makePathMatrix(const PairVector& e, SizeVector& sp, const Size e_size)
	{
		std::vector<Size>* line;
		// create bond-matrix, because Floyd's Algorithm requires a reachability matrix
		SizeVector* bond_matrix;
		line = new vector<Size>;
		bond_matrix = new SizeVector;
		
		// initialize bond-matrix with 0 at every position
		for (Size i = 0; i != e_size; ++i)
		{
			line->clear();
			for(Size j = 0; j != e_size; ++j)
			{
				line->push_back(0);
			}
			bond_matrix->push_back(*line);
		}
		// proceed all edges and set corresponding position in bond_matrix to 1
		for (Size i = 0; i != e.size(); ++i)
		{
			(*bond_matrix)[e[i].first][e[i].second] = 1;
		}
		
		// initialize sp-matrix
		for(Size i = 0; i != bond_matrix->size(); ++i)
		{
			line->clear();
			for(Size j = 0; j != bond_matrix->size(); ++j)
			{
				if (i == j) // distance from a node to itself = 0
				{
					line->push_back(0);
				}
				else if((*bond_matrix)[i][j] == 1)	// if an edge exists between node i and j,
				{
					line->push_back(1);								// the distance between them is 1
				}
				else
				{
					line->push_back(std::numeric_limits<Index>::max()); // otherwise the distance is set to infinity
				}
			}
			sp.push_back(*line);
		}
		
		// Floyd's Algorithm
		for(Size i = 0; i != sp.size(); ++i)
		{
			for(Size j = 0; j != sp.size(); ++j)
			{
				for(Size k = 0; k != sp.size(); k++)
				{
					if(sp[j][k] > (sp[j][i] + sp[i][k]))
					{
						sp[j][k] = (sp[j][i] + sp[i][k]);
					}
				}
			}
		}
		
		delete bond_matrix;
		delete line;
		return;
	}
	
	void UCK::getGraph(vector<String>& v, PairVector& e, const Molecule& mol)
	{
		weight_ = 0.0;
		Size count = 0;
		vector<pair<String, Size> >* mol_name;
		mol_name = new vector<pair<String, Size> >;
		bool found_atom = false;

		for(AtomConstIterator atit1 = mol.beginAtom(); atit1 != mol.endAtom(); ++atit1)
		{
			if(ignore_hydrogens_ && atit1->getElement()==PTE[1]) continue;

			// find chemical formula
			for(Size i = 0; i != mol_name->size(); ++i)
			{
				if((*mol_name)[i].first == atit1->getElement().getSymbol())	// increase number of already existing molecules
				{
					(*mol_name)[i].second++;
					found_atom = true;
					break;
				}
			}
			
			if(!found_atom)	// add current atom to formula, if it doesn't exist
			{
				mol_name->push_back(make_pair(atit1->getElement().getSymbol(),1));
				found_atom = false;
			}
			found_atom = false;
			
			weight_ += atit1->getElement().getAtomicWeight();
			v.push_back(atit1->getElement().getSymbol());	// add atom-name to label-list
			Size dest = 0;
			// find bonds from current atom to all other atoms and store them in e
			for(AtomConstIterator atit2 = mol.beginAtom(); atit2 != mol.endAtom(); ++atit2)
			{
				if(ignore_hydrogens_ && atit2->getElement()==PTE[1]) continue;

				if(atit1->getBond(*atit2) != 0)
				{
					e.push_back(make_pair(count, dest));
				}
				++dest;
			}
			++count;
		}

		sort(mol_name->begin(), mol_name->end());		// sort vector mol_name in order to get the lexicographically ordered
		for(Size i = 0; i != mol_name->size(); ++i)	// chemical formula
		{
			formula_ += ((*mol_name)[i].first)+(String)(*mol_name)[i].second;
		}
			
		delete mol_name;
		return;
	}
	
	String UCK::eraseDoubleLabels(Size d, String label, String x)
	{
		if(d >= 2)
		{
			if(x.find(label) != string::npos)
			{
				x = x.substr(0,x.find(label)) + x.substr(x.find(label)+label.size());
			}
		}

		return x;
	}
	
	String UCK::lambda(String lambda_d, const PairVector& e, const vector<String>& v, Size i, Size d, const Molecule& m)
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
			for(PairVector::const_iterator it = e.begin(); it != e.end(); ++it)
			{
				if(it->first!=i)	// if source node in e is not equal to the current position i, then skip this edge
				{
					continue;
				}
				else	// an edge to another node is found, so compute lambda_d-1 of the child and store the resulting string
							// in vector lam
				{
					/// modified by pbrach, Nov.2014, take bond-order into account
					const Atom* atm = m.getAtom(it->second);
					const Bond* bnd = m.getAtom(i)->getBond(*atm);
					String lam_str="";
					if (bnd)
					{
						String bo_str= "";
						
						if (bnd->getOrder()==2)
							bo_str="=";
						else if(bnd->getOrder()==3)
							bo_str="#";
						else if(bnd->getOrder()==4)
							bo_str="-";
						else if(bnd->getOrder()==5)
							bo_str="*";
	
						lam_str = lambda("", e, v, it->second, d-1, m)+bo_str;
					}
					else
					{
//						std::cout<< "WARNING: UCK running on molecule "<< m.getName();
//						std::cout<<"did not get bond for: "<< (i+1) << " "<< (it->second+1);
//						std::cout<<endl;
						lam_str = lambda("", e, v, it->second, d-1, m); /// original call!
					}
					lam->push_back(eraseDoubleLabels(d, v[i], lam_str));
				}
			}
			sort(lam->begin(), lam->end()); // lexicographically order the lambda_d-1 -labels
		}
		// concatenate lambda_d-1 -labels and produce lambda_d -label
		for(vector<String>::iterator it = lam->begin(); it != lam->end(); ++it)
		{
			lambda_d += *it;
		}

		delete lam;
		return lambda_d;
	}
	
	void UCK::makePairs(const vector<String>& lambda_map, vector<String>& pairs, const SizeVector& sp)
	{
		for(Size i = 0; i != lambda_map.size(); ++i)
		{
			for(Size j = 0; j != lambda_map.size(); ++j)
			{
				pairs.push_back(lambda_map[i] + (String)sp[i][j] + lambda_map[j]);
			}
		}

		sort(pairs.begin(), pairs.end());
		return;
	}
	
	void UCK::createFinalString(const vector<String>& pairs)
	{
		uck_str_ = formula_ + "-";
		for (Size i = 0; i != pairs.size(); ++i)
		{
			uck_str_ += pairs[i];
		}	
		uck_str_ += "\n";

		// TODO: Do we want to use MD5? In CADDSuite this was SHA1!
		uck_str_ = QCryptographicHash::hash(QByteArray(uck_str_.c_str()), 
		                                    QCryptographicHash::Md5).toHex().constData(); 

		return;
	}
	
	Size UCK::getDepth()
	{
		return depth_;
	}

	const String& UCK::getFormula() const
	{
		return formula_;
	}

	const String& UCK::getUCK() const
	{
		return uck_str_;
	}

	const String& UCK::getId() const
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
		PairVector e; // edge set
		SizeVector sp;
		
		getGraph(v, e, m);
		
		for(Size i=0; i!=v.size(); ++i)
		{
			lambda_map.push_back(lambda("", e, v, i, depth_, m));
		}
		
		makePathMatrix(e, sp, v.size());
		makePairs(lambda_map, pairs, sp);
		createFinalString(pairs);
		return;
	}
	
	
	/// C l a s s   C a n o n i c a l i z e r
	/// ############################################################################
	Canonicalizer::Canonicalizer(){}
	
	Canonicalizer::~Canonicalizer(){}
	
	void Canonicalizer::canonicalize(AtomContainer& molecule)
	{
		using namespace OpenBabel;
		
		// aromatize:
		AromaticityProcessor aroma;
		molecule.apply(aroma);
		
		OBMol* temp = MolecularSimilarity::createOBMol( molecule);
		int num_atoms = temp->NumAtoms();
		
		// get canonical labels:
		vector<unsigned int> sym;
		vector<unsigned int> clabels;
		
		OBGraphSym grsym(temp);
		grsym.GetSymmetry(sym);
		
		CanonicalLabels(temp, sym, clabels, OBBitVec(), 30, false);
		
		// resort atom list by inserting into a new empty molecule
		AtomContainer* new_frag = new AtomContainer;
		
		vector <Atom*> aList(num_atoms);
		for( int i = 0; i < clabels.size(); i++)
			aList[ clabels[i]-1 ] = molecule.getAtom(i);
		
		// insert in correct order into temporary atomContainer
		for( int i = 0; i < clabels.size(); i++)
			new_frag->append( *aList[i] );
		
		// swap the temp with the original and delete the temp
		molecule.swap(*new_frag);
		delete new_frag;
		delete temp;
	}
} // namespace BALL
