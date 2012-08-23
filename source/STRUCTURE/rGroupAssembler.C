
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/rGroupAssembler.h>

using namespace std;

namespace BALL
{
	RGroupAssembler::RGroupAssembler(vector<String>& scaffolds, map<String, vector<String> > &rgroups)
	{
		SmilesParser smiles_parser;
		for (Size i = 0; i < scaffolds.size(); i++)
		{
			smiles_parser.parse(scaffolds[i]);
			scaffolds_.push_back( new System(smiles_parser.getSystem()) );
		}
			map<String, vector<String> >::iterator iter = rgroups.begin();
		for (; iter != rgroups.end(); iter++ )
		{
			vector<String> smiles = iter->second;
			for (Size i = 0; i < smiles.size(); i++)
			{
				smiles_parser.parse(smiles[i]);
				System *sys = new System(smiles_parser.getSystem());
				moieties_[iter->first].push_back(sys);
			}
		}
			if (scaffolds_.size() == 0 || moieties_.size() == 0)
			not_finished_ = false;
		else
			not_finished_ = true;
		initCounter_();
	}
		RGroupAssembler::RGroupAssembler(String& scaffoldS, map<String, vector<String> > &rgroups)
	{
		SmilesParser smiles_parser;
		smiles_parser.parse(scaffoldS);
		System* scaffold = new System(smiles_parser.getSystem());
			map<String, vector<String> >::iterator iter = rgroups.begin();
			for (; iter != rgroups.end(); iter++ )
		{
			vector<String> smiles = iter->second;
			for (Size i = 0; i < smiles.size(); i++)
			{
				smiles_parser.parse(smiles[i]);
				System *sys = new System(smiles_parser.getSystem());
				moieties_[iter->first].push_back(sys);
			}
		}
			scaffolds_.push_back(scaffold);
			if (scaffolds_.size() == 0 || moieties_.size() == 0)
			not_finished_ = false;
		else
			not_finished_ = true;
		initCounter_();
	}
		RGroupAssembler::~RGroupAssembler()
	{
		map<String, vector<System*> >::iterator iter = moieties_.begin();
		for (; iter != moieties_.end(); iter++ )
		{
			vector<System*> sys = iter->second;
			for (Size i = 0; i < sys.size(); i++)
			{
				sys[i]->destroy();
				delete sys[i];
			}
		}
		for (Size i = 0; i < scaffolds_.size(); i++)
		{
			scaffolds_[i]->destroy();
			delete scaffolds_[i];
		}
	}
		void RGroupAssembler::initCounter_()
	{
		moiety_counter_.clear();
		maxcounter_.clear();
		scaffold_counter_ = 0;
		for (map < String, vector < System* > > ::iterator it = moieties_.begin(); it != moieties_.end(); it++)
		{
			moiety_counter_[it->first] = 0;
			for (Size i = 0; i < scaffolds_.size(); i++)
			{
				map<String, unsigned int> temp_m;
				temp_m[it->first] = 0;
				maxcounter_.push_back(temp_m);
				System s;
				if (scaffolds_[i] != NULL)
					s = *scaffolds_[i];
				else
				{
					cerr << "scaffold pointer is null" << endl;
					exit(1);
				}
					for (AtomConstIterator atom_it = s.beginAtom(); +atom_it; atom_it++)
				{
					if (isPlacemark_(atom_it->getName()))
					{
						if (atom_it->getName() == it->first)
						{
							maxcounter_[i][it->first] = it->second.size();
						}
					}
				}
			}
		}
	}
		bool RGroupAssembler::incrementCounter_()
	{
		for (map < String, unsigned int > ::iterator it = moiety_counter_.begin(); it != moiety_counter_.end(); it++)
		{
			it->second++;
			if (it->second < maxcounter_[scaffold_counter_][it->first])
			{
				return true;
			}
			else
				it->second = 0;
		}
			scaffold_counter_++;
		if (scaffold_counter_ < scaffolds_.size())
		{
			return true;
		}
		return false;
	}
		bool RGroupAssembler::isPlacemark_(const String& s)
	{
		// Placemarks begin with 'R' followed by a digit
		return (s[0] == 'R' && s.size() >= 2 && isdigit(s[1]));
	}
		int countHeavyAtoms(const AtomContainer* molecule)
	{
		int no = 0;
		for (AtomConstIterator atom_it = molecule->beginAtom(); +atom_it; atom_it++)
		{
			if (atom_it->getElement().getSymbol() != "H") no++;
		}
		return no;
	}
		void RGroupAssembler::appendMoiety_(AtomContainer* molecule, AtomContainer* moiety, String pm_atom)
	{
		AtomContainer* moiety_copy = new AtomContainer;
		*moiety_copy = *moiety;
		molecule->insert(*moiety_copy);
		vector<Atom*> conn_atoms;
			for (AtomIterator atom_it = molecule->beginAtom(); +atom_it; atom_it++)
		{
			if (atom_it->getName() == pm_atom)
			{
				// find placemark atom ..
				conn_atoms.push_back(atom_it->beginBond()->getPartner(*atom_it));
				// ... and select placemark atom's partner
				atom_it->select();
			}
			else atom_it->deselect();
		}
		// remove placemark atoms
		molecule->removeSelected();
			// create bond between both R-atoms
		Bond* bnd = conn_atoms[0]->createBond(*conn_atoms[1]);
		bnd->setOrder(1);
	}
		Molecule* RGroupAssembler::generateNextMolecule()
	{
		if (!not_finished_) return NULL;
			Molecule* mol = new Molecule;
		System* scaffold_copy = new System;
		if (scaffolds_[scaffold_counter_] != NULL)
		{
			*scaffold_copy = *scaffolds_[scaffold_counter_];
		}
		else
		{
			return 0;
		}
		mol->insert(*scaffold_copy);
			for (AtomIterator atom_it = mol->beginAtom(); +atom_it; )
		{
			String atom_name = atom_it->getName();
			if (isPlacemark_(atom_name))
			{
				atom_it++;
				appendMoiety_(mol, moieties_[atom_name][moiety_counter_[atom_name]], atom_name);
			}
			else
			{
				atom_it++;
			}
		}
			not_finished_ = incrementCounter_();
		return mol;
	}
}
