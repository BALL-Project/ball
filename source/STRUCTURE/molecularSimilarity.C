// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/STRUCTURE/molecularSimilarity.h>
#include <BALL/KERNEL/molecule.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/DATATYPE/string.h>
#include <BALL/SYSTEM/path.h>
#include <fstream>
#include <sstream>

#ifdef BALL_HAS_OPENBABEL
	#include <openbabel/obconversion.h>
	#include <openbabel/parsmart.h>
	#include <BALL/FORMAT/SDFile.h>
	#include <openbabel/atom.h>
	using namespace OpenBabel;
#endif

using namespace BALL;
using namespace std;


MolecularSimilarity::MolecularSimilarity(String smarts_file)
{
	Path path;
	String file = path.find(smarts_file);
	if(file=="")
	{
		throw BALL::Exception::FileNotFound(__FILE__,__LINE__,smarts_file);
	}
	std::ifstream smart_input(file.c_str());

	// read SMARTS-expression and names for those SMARTS from the specified file
	for(Size i=0; smart_input; i++)
	{
		if(i%300==0) // prevent frequent resizing
		{
			int a = (i/300)+1;
			smarts_.reserve(a*300);
			smart_names_.reserve(a*300);
		}

		String line;
		getline(smart_input,line);
		line.trim();
		if(line!="")
		{
			stringstream lstream(line);
			string s;
			lstream >> s;  // read first word but ignore the following comment (name/description of functional group)
			if(s!="") smarts_.push_back(s);

			if(line.hasSubstring("\t")) smart_names_.push_back(String(line.after("\t")).trim());
			else
			{
				throw BALL::Exception::GeneralException(__FILE__,__LINE__,"MolecularSimilarity error","SMARTS file has wrong format! Maybe tabs are missing.");
			}
		}
	}
}



#ifdef BALL_HAS_OPENBABEL
OBMol* MolecularSimilarity::createOBMol(const Molecule& mol, bool ignore_hydrogen, bool suppress_warning)
{
	OBMol* obmol = new OBMol;
	HashMap<const Atom*,OBAtom*> atoms;
	HashMap<OBAtom*,HashSet<OBAtom*> > bonds;
	HashMap<OBAtom*,HashSet<OBAtom*> >::iterator s_it;
	HashSet<const Atom*> fixed_bonds;
	bool warning_shown = false;

	for(AtomConstIterator a_it=mol.beginAtom(); +a_it; a_it++)
	{
		if(ignore_hydrogen && a_it->getElement().getSymbol()=="H") continue;
		OBAtom* atom = obmol->NewAtom();
		atom->SetAtomicNum(a_it->getElement().getAtomicNumber());
		const Vector3& pos = a_it->getPosition();
		atom->SetVector(pos.x, pos.y, pos.z);
		atoms.insert(make_pair(&*a_it,atom));
	}
	for(AtomConstIterator a_it=mol.beginAtom(); +a_it; a_it++)
	{
		if(ignore_hydrogen && a_it->getElement().getSymbol()=="H") continue;

		for(Atom::BondConstIterator b_it=a_it->beginBond(); +b_it; b_it++)
		{
			if(ignore_hydrogen && b_it->getPartner(*a_it)->getElement().getSymbol()=="H") continue;

			OBAtom* atom1 = atoms.find(b_it->getFirstAtom())->second;
			OBAtom* atom2 = atoms.find(b_it->getSecondAtom())->second;

			// do not add any bond twice ...
			s_it=bonds.find(atom1);
			if(s_it!=bonds.end() && s_it->second.find(atom2)!=s_it->second.end())
			{
				continue;
			}
			s_it=bonds.find(atom2);
			if(s_it!=bonds.end() && s_it->second.find(atom1)!=s_it->second.end())
			{
				continue;
			}

			int order = b_it->getOrder();
			OBBond* bond = obmol->NewBond();
			bond->SetBegin(atom1);
			bond->SetEnd(atom2);
			if (b_it->isAromatic())
			{
				bond->SetBondOrder(5);

				/// Correct bond-orders of carboxyl- and guanidinium-groups (as e.g. in glutamic/aspartic acid or arginine): OpenBabel does not like those bonds to be declared as aromatic (it would add too many hydrogens or place them incorrectly), so we have to fix this here!
				const Atom* base = b_it->getFirstAtom();
				const Atom* terminal = b_it->getSecondAtom();
				if (base->getElement().getSymbol()!="C")
				{
					base = b_it->getSecondAtom();
					terminal = b_it->getFirstAtom();
				}
				if (base->getElement().getSymbol()=="C")
				{
					// Find out whether the 'terminal' atom is not bonded to any other heavy atoms
					bool is_terminal = true;
					for (Atom::BondConstIterator b_it=terminal->beginBond(); +b_it; b_it++)
					{
						Atom* a = b_it->getPartner(*terminal);
						if (a != base && a->getElement().getSymbol() != "H")
						{
							is_terminal = false;
							break;
						}
					}

					// Check whether the base atom has the correct number of neighboring nitrogen resp. oxygen atoms
					Size no_nitrogens_partners = 0;
					Size no_oxygen_partners = 0;
					Size no_other_partners = 0;
					Atom* nonterminal_N = 0;
					for (Atom::BondConstIterator b_it=base->beginBond(); +b_it; b_it++)
					{
						Atom* a = b_it->getPartner(*base);
						String element = a->getElement().getSymbol();
						if (element == "O")
						{
							no_oxygen_partners++;
						}
						else if (element == "N")
						{
							no_nitrogens_partners++;
							bool is_terminal_N = true;
							for (Atom::BondConstIterator b_it2=a->beginBond(); +b_it2; b_it2++)
							{
								Atom* a2 = b_it2->getPartner(*a);
								if (a2 != base && a2->getElement().getSymbol() != "H")
								{
									is_terminal_N = false;
									break;
								}
							}
							if (!is_terminal_N)
							{
								nonterminal_N = a;
							}
						}
						else if (element != "H")
						{
							no_other_partners++;
						}
					}
					bool is_carboxyl = ((no_other_partners==1) && (no_oxygen_partners==2) && (no_nitrogens_partners==0));
					bool is_guanidinium = ((no_other_partners==0) && (no_oxygen_partners==0) && (no_nitrogens_partners==3) && nonterminal_N);

					if (is_terminal && (is_carboxyl || is_guanidinium))
					{
						// Check whether we need to set the order of the current bond to one or to two
						HashSet<const Atom*>::iterator search_it = fixed_bonds.find(base);
						if (search_it == fixed_bonds.end())
						{
							bond->SetBondOrder(1);
							fixed_bonds.insert(base);

							if (is_carboxyl)
							{
								atoms.find(terminal)->second->SetFormalCharge(-1);
							}

							if (!warning_shown && !suppress_warning)
							{
								Log.warn() << "[Warning:] Carboxyl- or guanidinium-groups with bonds labeled as aromatic detected. For the created OpenBabel molecule, these bonds will be de-aromatized, since otherwise OpenBabel would produce errors when trying to protonate molecules." << endl << endl;
								warning_shown = true;
							}
						}
						else
						{
							bond->SetBondOrder(2);
							if (is_guanidinium)
							{
								atoms.find(terminal)->second->SetFormalCharge(1);
							}
						}
					}
				}
			}
			else if (order==0)
			{
				bond->SetBondOrder(1);
			}
			else if(order<=5) // order=5 is interpreted as aromatic by openbabel as well ...
			{
				bond->SetBondOrder(order);
			}
			else if(order==6)
			{
				bond->SetBondOrder(1);
			}
			obmol->AddBond(*bond);

			s_it=bonds.find(atom1);
			if(s_it==bonds.end())
			{
				HashSet<OBAtom*> set;
				set.insert(atom2);
				bonds.insert(make_pair(atom1,set));
			}
			else
			{
				s_it->second.insert(atom2);
			}
		}
	}

	obmol->SetAromaticPerceived(); // make sure that detection of aromatic atoms/bonds will work later when using smarts-matcher

	return obmol;
}


Molecule* MolecularSimilarity::createMolecule(OpenBabel::OBMol& obmol, bool ignore_hydrogen)
{
	Molecule* mol = new Molecule;
	HashMap<const OBAtom*,Atom*> atoms;
	HashMap<Atom*,HashSet<Atom*> > bonds;
	HashMap<Atom*,HashSet<Atom*> >::iterator s_it;

	for(OBAtomIterator a_it=obmol.BeginAtoms(); a_it!=obmol.EndAtoms(); a_it++)
	{
		OBAtom* obatom = *a_it;
		if(ignore_hydrogen && obatom->GetAtomicNum()==1) continue;
		Atom* atom = new Atom;
		atom->setElement(PTE_::getElement(obatom->GetAtomicNum()));
		
		atom->setCharge(obatom->GetPartialCharge());
		atom->setFormalCharge(obatom->GetFormalCharge());
		
		Vector3 pos(obatom->GetX(), obatom->GetY(), obatom->GetZ());
		atom->setPosition(pos);
		atoms.insert(make_pair(obatom,atom));
		mol->insert(*atom);
	}
	
	for(OBAtomIterator a_it=obmol.BeginAtoms(); a_it!=obmol.EndAtoms(); a_it++)
	{
		if(ignore_hydrogen && (*a_it)->GetAtomicNum()==1) continue;

		for (OBBondIterator b_it = (*a_it)->BeginBonds();
			b_it !=  (*a_it)->EndBonds(); b_it++)
		{
			OBBond* obbond = *b_it;

			if (ignore_hydrogen && (obbond->GetBeginAtom()->GetAtomicNum()==1 || obbond->GetEndAtom()->GetAtomicNum()==1))
			{
				continue;
			}

			Atom* atom1 = atoms.find(obbond->GetBeginAtom())->second;
			Atom* atom2 = atoms.find(obbond->GetEndAtom())->second;

			// do not add any bond twice ...
			s_it=bonds.find(atom1);
			if(s_it!=bonds.end() && s_it->second.find(atom2)!=s_it->second.end())
			{
				continue;
			}
			s_it=bonds.find(atom2);
			if(s_it!=bonds.end() && s_it->second.find(atom1)!=s_it->second.end())
			{
				continue;
			}

			int order = obbond->GetBondOrder();
			Bond* bond = atom1->createBond(*atom2);
			if(order<=5) // order=5 is interpreted as aromatic by BALL as well ...
			{
				bond->setOrder(order);
			}
			else if(order==6)
			{
				bond->setOrder(1);
			}

			s_it=bonds.find(atom1);
			if(s_it==bonds.end())
			{
				HashSet<Atom*> set;
				set.insert(atom2);
				bonds.insert(make_pair(atom1,set));
			}
			else
			{
				s_it->second.insert(atom2);
			}
		}
	}

	return mol;
}


void MolecularSimilarity::generateCanSmile(const Molecule& molecule, String& cansmile, OpenBabel::OBMol** output_obmol, bool ignore_hydrogen)
{
	OBMol* obmol = createOBMol(molecule,ignore_hydrogen);
	if(output_obmol) *output_obmol = obmol;

	OBConversion conv;
	conv.SetOutFormat("can"); // canonical smiles
	cansmile = conv.WriteString(obmol);
	// remove the stupid ID that openbabel always attaches to the generated canonical smile
	cansmile = cansmile.substr(0,cansmile.find_first_of('\t'));

	if(!output_obmol) delete obmol;
}
#endif



void MolecularSimilarity::generateFingerprints(System& molecules, vector<vector<Size> >& fingerprints)
{
	list<Molecule*> molecule_list;
	for(MoleculeIterator it=molecules.beginMolecule(); +it; it++)
	{
		molecule_list.push_back(&*it);
	}
	generateFingerprints(molecule_list,fingerprints);
}


void MolecularSimilarity::generateFingerprints(const list<Molecule*>& molecules, vector<vector<Size> >& fingerprints)
{
	Size no_smarts = smarts_.size();
	fingerprints.clear();
	fingerprints.resize(molecules.size());

	Size no_mols = molecules.size();
	Size i=0;
	cout<<"Generating fingerprints: "<<endl;
	for(list<Molecule*>::const_iterator it=molecules.begin(); it!=molecules.end(); it++, i++)
	{
		cout<<"\r  molecule "<<i<<"/"<<no_mols<<flush;
		fingerprints[i].resize(no_smarts,0);
		generateFingerprint(**it,fingerprints[i]);
	}
	cout<<endl;
}


void MolecularSimilarity::generateFingerprint(Molecule& molecule, vector<Size>& fingerprint)
{
	Size no_smarts = smarts_.size();
	fingerprint.resize(no_smarts);

#ifdef BALL_HAS_OPENBABEL
	OBMol* obmol = createOBMol(molecule, false);

	for(Size i=0; i<no_smarts;i++)
	{
		try
		{
			OBSmartsPattern pattern;
			pattern.Init(smarts_[i]);
			pattern.Match(*obmol);
			fingerprint[i] = pattern.GetUMapList().size();
		}
		catch(...)
		{
			Log.error() << "Error while trying to match OB SMARTS for fingerprint generation." << endl;
		}
	}

	delete obmol;
#else
	for(Size i=0; i<no_smarts;i++)
	{
		try
		{
			SmartsMatcher::Match match;
			matcher_.match(match,molecule,smarts_[i]);
			fingerprint[i] = (Size) match.size();
		}
		catch(BALL::Exception::GeneralException)
		{
			Log.error() << "Error while trying to match SMARTS for fingerprint generation." << endl;
		}
	}
#endif
}


#ifdef BALL_HAS_OPENBABEL
void MolecularSimilarity::matchSmarts(const String& usmile, const String& smarts, Size& no_matches, Size max_matches)
{
	OBMol mol;
	OBConversion conv;
	conv.SetInFormat("smi");
	conv.ReadString(&mol,usmile);

	OBSmartsPattern pattern;
	pattern.Init(smarts);
	bool single_match = (max_matches==1);
	pattern.Match(mol,single_match);
	no_matches=pattern.GetUMapList().size();
}
#endif


void MolecularSimilarity::generatePathFingerprint(Molecule& mol, vector<bool>& fingerprint)
{
	fingerprint.resize(1024,0);

	// enumerate all pathes up to length 7 (=14 characters)
	for(AtomConstIterator a_it=mol.beginAtom(); +a_it; a_it++)
	{
		vector<Size> path;
		set<const Bond*> path_bonds;
		bool processed_path = generatePathFingerprint_(&*a_it,path,path_bonds,fingerprint);

		if(!processed_path)  // single unconnected atoms
		{
			// generate boolean hash-key for current 'path' string
			Size hash;  // range will be [0:1020]
			generatePathHash_(path,hash);

			// OR boolean hash-key with current 'fingerprint'
			if(!fingerprint[hash]) fingerprint[hash] = true;
		}
	}
}


bool MolecularSimilarity::generatePathFingerprint_(const Atom* atom, vector<Size>& path, set<const Bond*>& path_bonds, vector<bool>& fingerprint)
{
	bool processed_path = 0;
	path.push_back(atom->getElement().getAtomicNumber());

	for(Atom::BondConstIterator b_it=atom->beginBond(); +b_it; b_it++)
	{
		if(path_bonds.find(&*b_it)!=path_bonds.end()) continue;

		if(path.size()>14) break;

		processed_path=true;
		vector<Size> path_i = path; // new subpath for current bond
		set<const Bond*> path_i_bonds = path_bonds;
		path_i_bonds.insert(&*b_it);

		int order=b_it->getOrder();
		if(order==0) order=1;  // 'unknown' bond -> single bond
		else if(order==6) order=1; // 'any' bond -> single bond
		path_i.push_back(b_it->getOrder());

		const Atom* atom1 = b_it->getFirstAtom();
		const Atom* partner;
		if(atom1==atom) partner=atom1;
		else partner=b_it->getSecondAtom();
		generatePathFingerprint_(partner,path_i,path_i_bonds,fingerprint);

		// generate boolean hash-key for current 'path' string
		Size hash;  // range will be [0:1020]
		generatePathHash_(path_i,hash);

		// OR boolean hash-key with current 'fingerprint'
		if(!fingerprint[hash]) fingerprint[hash] = true;
	}

	return processed_path;
}


void MolecularSimilarity::generatePathHash_(vector<Size>& path, Size& hash)
{
	// whole path treated as a binary number mod 1021
	const int MODINT = 108; // 2^32 % 1021
	hash=0;
	for(unsigned i=0;i<path.size();++i)
	{
		hash = (hash*MODINT + (path[i] % 1021)) % 1021;
	}
}



void MolecularSimilarity::filterRedundantMolecules(System& molecules, float similarity_threshold)
{
	list<Molecule*> molecule_list;
	for(MoleculeIterator it=molecules.beginMolecule(); +it; it++)
	{
		molecule_list.push_back(&*it);
	}
	filterRedundantMolecules(molecule_list,similarity_threshold);
}


void MolecularSimilarity::filterRedundantMolecules(const list<Molecule*>& molecules, float similarity_threshold)
{
	// make sure that no molecules are selected
	for(list<Molecule*>::const_iterator it=molecules.begin(); it!=molecules.end(); it++)
	{
		(*it)->deselect();
	}

	vector<vector<Size> > all_fingerprints;
	generateFingerprints(molecules,all_fingerprints);

	// calculate mean of each functional group count
	Size no_smarts = smarts_.size();
	Size no_mols = all_fingerprints.size();
	vector<float> mean(no_smarts,0);
	for(Size j=0; j<no_smarts; j++)
	{
		for(Size i=0; i<no_mols; i++)
		{
			mean[j]+=all_fingerprints[i][j];
		}
		mean[j] /= no_mols;
	}

	// calculate stddev of each functional group count
	vector<float> stddev(no_smarts,0);
	for(Size j=0; j<no_smarts; j++)
	{
		for(Size i=0; i<no_mols; i++)
		{
			stddev[j]+=pow(all_fingerprints[i][j]-mean[j],2);
		}
		stddev[j] = sqrt(stddev[j]/no_mols);
	}

	Size a=0;
	for(list<Molecule*>::const_iterator it1=molecules.begin(); it1!=molecules.end(); it1++, a++)
	{
		float max_sim_to_previous_mols = 0;
		Size similar_molecule=0;
		Size b=0;
		for(list<Molecule*>::const_iterator it2=molecules.begin(); it2!=it1; it2++, b++)
		{
			if((*it2)->isSelected()) continue; // ignore redundant molecules

			float sim = calculateSimilarity(all_fingerprints[a],all_fingerprints[b],&stddev);
			if(sim>max_sim_to_previous_mols)
			{
				max_sim_to_previous_mols = sim;
			}
			if(max_sim_to_previous_mols>similarity_threshold)
			{
				cout<<(a+1)<<","<<(b+1)<<" : "<<sim<<endl;
				similar_molecule=b+1;
				break;
			}
		}

		if(max_sim_to_previous_mols>similarity_threshold)
		{
			// select the redundant molecules
			(*it1)->select();
			(*it1)->setProperty("similarity",max_sim_to_previous_mols);
			(*it1)->setProperty("similar_mol",similar_molecule);
		}
	}
}


float MolecularSimilarity::calculateSimilarity(vector<bool>& fingerprint1, vector<bool>& fingerprint2)
{
	Size AND_bits=0;
	Size OR_bits=0;

	for(Size i=0; i<fingerprint1.size(); i++)
	{
		if(fingerprint1[i]!=0)
		{
			OR_bits++;

			if(fingerprint2[i]!=0)
			{
				AND_bits++;
			}
		}
		else if(fingerprint2[i]!=0)
		{
			OR_bits++;
		}
	}

	return ((float)AND_bits)/OR_bits;
}


float MolecularSimilarity::calculateSimilarity(vector<Size>& fingerprint1, vector<Size>& fingerprint2, vector<float>* stddev)
{
	double sim=0;
	Size no=0;
	for(Size i=0; i<fingerprint1.size(); i++)
	{
		float dist_i = fingerprint1[i]-fingerprint2[i];
		if(fingerprint1[i]!=0 || fingerprint2[i]!=0)
		{
			if(stddev && (*stddev)[i]>1e-10) dist_i /= (*stddev)[i];
			float sim_i = 1-fabs(dist_i);
			if(sim_i<0) sim_i=0;

			sim += sim_i;
			no++;
		}
	}

	if(no>0) sim /= no;
	return sim;
}


const vector<String>& MolecularSimilarity::getFunctionalGroupNames()
{
	return smart_names_;
}
