// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/STRUCTURE/rotamerLibrary.h>
#include <BALL/FORMAT/SCWRLRotamerFile.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/MATHS/common.h>

#include <algorithm>

using namespace std;

namespace BALL
{
  const String RotamerLibrary::DEFAULT_LIBRARY = "rotamers/bbdep02.May.sortlib";
  const String RotamerLibrary::DEFAULT_FRAGMENT_DB = "fragments/Fragments.db";

	RotamerLibrary::RotamerLibrary() 
		: step_width_(0),
			fragment_db_(DEFAULT_FRAGMENT_DB),
			backbone_dependent_(false)
	{
		Path path;
		String absolute_filename = path.find(DEFAULT_LIBRARY);
		SCWRLRotamerFile file(absolute_filename);
		file >> *this;
	}

	RotamerLibrary::RotamerLibrary(const RotamerLibrary& library) 
		:	bb_dep_sets_(library.bb_dep_sets_),
			bb_indep_sets_(library.bb_indep_sets_),
			step_width_(library.step_width_),
			fragment_db_(library.fragment_db_),
			backbone_dependent_(library.backbone_dependent_)
	{
	}

	RotamerLibrary::RotamerLibrary(const String& filename, const FragmentDB& fragment_db) 
		:	step_width_(0),
			fragment_db_(fragment_db),
			backbone_dependent_(false)
	{
		Path path;
		String absolute_filename = path.find(filename);
		SCWRLRotamerFile file(absolute_filename);
		file >> *this;
	}

	RotamerLibrary::RotamerLibrary(const FragmentDB& fragment_db) 
		: step_width_(0),
			fragment_db_(fragment_db),
			backbone_dependent_(false)
	{
		
	}

	RotamerLibrary& RotamerLibrary::operator = (const RotamerLibrary& rhs)
	{
		// Avoid self assignment...
		if (&rhs != this)
		{ // ...and copy all attributes.
			bb_dep_sets_ = rhs.bb_dep_sets_;
			bb_indep_sets_ = rhs.bb_indep_sets_;
			step_width_ = rhs.step_width_;
			fragment_db_ = rhs.fragment_db_;
			backbone_dependent_ = rhs.backbone_dependent_;
		}
		return *this;
	}

	RotamerLibrary::~RotamerLibrary()
	{
	}

	ResidueRotamerSet* RotamerLibrary::getRotamerSet(const String& name)
	{
		if (backbone_dependent_)
		{
			Log.error() << "RotamerLibrary: getRotamerSet, no torsions given for name '" << name << "'" << endl;
			return 0;
		}
		
		if (bb_indep_sets_.has(name))
		{
			return &bb_indep_sets_[name];
		}
		return 0;
	}

	ResidueRotamerSet* RotamerLibrary::getRotamerSet(const String& name, float phi, float psi)
	{
		Index bin_phi = getNearestBackboneTorsion_(phi);
		Index bin_psi = getNearestBackboneTorsion_(psi);
		if (bb_dep_sets_.has(bin_phi) && bb_dep_sets_[bin_phi].has(bin_psi) && bb_dep_sets_[bin_phi][bin_psi].has(name))
		{
			return &bb_dep_sets_[bin_phi][bin_psi][name];
		}

		return 0;
	}

	ResidueRotamerSet* RotamerLibrary::getRotamerSet(const Residue& residue)
	{
		if (backbone_dependent_)
		{
			Index phi(0), psi(0);

			phi = getNearestBackboneTorsion_(residue.getTorsionPhi().toDegree());
			psi = getNearestBackboneTorsion_(residue.getTorsionPsi().toDegree());

			
			if (bb_dep_sets_.has(phi) && bb_dep_sets_[phi].has(psi) && bb_dep_sets_[phi][psi].has(residue.getName()))
			{
				return &bb_dep_sets_[phi][psi][residue.getName()];
			}
		}
		else
		{
			if (bb_indep_sets_.has(residue.getName()))
			{
				return &bb_indep_sets_[residue.getName()];
			}
		}

		return 0;
	}

	Size RotamerLibrary::getNumberOfRotamerSets() const
	{
		Size number(0);
		if (backbone_dependent_)
		{
			HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >::ConstIterator it1 = bb_dep_sets_.begin();
			for (; it1 != bb_dep_sets_.end(); ++it1)
			{
				for (HashMap<Index, HashMap<String, ResidueRotamerSet> >::ConstIterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
				{
					number += it2->second.size();
				}
			}
		}
		else
		{
			number = bb_indep_sets_.size();
		}
		return number;
	}

	Size RotamerLibrary::getNumberOfRotamers() const 
	{
		Size number = 0;
		if (backbone_dependent_)
		{
			HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >::ConstIterator it1 = bb_dep_sets_.begin();
			for (; it1 != bb_dep_sets_.end(); ++it1)
			{
				for (HashMap<Index, HashMap<String, ResidueRotamerSet> >::ConstIterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
				{
					for (HashMap<String, ResidueRotamerSet>::ConstIterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
					{
						number += it3->second.getNumberOfRotamers();
					}
				}
			}
		}
		else
		{
			for (HashMap<String, ResidueRotamerSet>::ConstIterator it = bb_indep_sets_.begin(); it != bb_indep_sets_.end(); ++it)
			{
				number += it->second.getNumberOfRotamers();
			}
		}

		return number;
	}

	Size RotamerLibrary::getNumberOfRotamers(const String& name) const
	{
		Size number(0);
		if (backbone_dependent_)
		{
			HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >::ConstIterator it1 = bb_dep_sets_.begin();
    	for (; it1 != bb_dep_sets_.end(); ++it1)
    	{
      	for (HashMap<Index, HashMap<String, ResidueRotamerSet> >::ConstIterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
      	{
					if (it2->second.has(name))
					{
      	    number += (it2->second)[name].getNumberOfRotamers();
    	    }
  	    }
	    }
		}
		else
		{
			if (bb_indep_sets_.has(name))
			{
				number += bb_indep_sets_[name].getNumberOfRotamers();
			}
		}
		return number;
	}

	void RotamerLibrary::addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions, Index phi, Index psi)
	{
		if (bb_dep_sets_[phi][psi].has(name))
		{
			bb_dep_sets_[phi][psi][name].addRotamer(rotamer);
		}
		else
		{
			bb_dep_sets_[phi][psi].insert(make_pair(name, ResidueRotamerSet(*fragment_db_.getResidue(name), number_of_torsions)));
			bb_dep_sets_[phi][psi][name].setTorsionPhi(Angle(phi, false));
			bb_dep_sets_[phi][psi][name].setTorsionPsi(Angle(psi, false));
			bb_dep_sets_[phi][psi][name].addRotamer(rotamer);
		}
	}

	void RotamerLibrary::addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions)
	{
		if (bb_indep_sets_.has(name))
		{
			bb_indep_sets_[name].addRotamer(rotamer);
		}
		else
		{
			bb_indep_sets_.insert(make_pair(name, ResidueRotamerSet(*fragment_db_.getResidue(name), number_of_torsions)));
			bb_indep_sets_[name].addRotamer(rotamer);
		}
		return;
	}

	void RotamerLibrary::setBackboneDependent(bool backbone_dependent)
	{
		backbone_dependent_ = backbone_dependent;
	}
	
	bool RotamerLibrary::isBackboneDependent() const
	{
		return backbone_dependent_;
	}

	Index RotamerLibrary::getNearestBackboneTorsion_(double torsion) const
	{
		double bin = Maths::round(torsion / (double)step_width_);
		
		Index rounded_torsion = (Index)bin * step_width_;

		// now check if the angle is wrong (e.g. -180 -> 180)
		if (bb_dep_sets_.size() != 0 && !bb_dep_sets_.has(rounded_torsion))
		{
			// TODO try to correct the angle 
			Log.error() << "Angle of " << rounded_torsion << " is not contained in the rotamer library!"  << endl;
		}
		return rounded_torsion;
	}

	bool RotamerLibrary::validate()
	{
		if (!backbone_dependent_)
		{
			return true;
		}
		Size number_of_phis(bb_dep_sets_.size());
		vector<Index> torsions;
		HashSet<Index> unique_torsion;
		for (HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >::ConstIterator it1 = bb_dep_sets_.begin(); it1 != bb_dep_sets_.end(); ++it1)
		{
			// same number of discrete angels of phi and psi?
			if (number_of_phis != it1->second.size())
			{
				return false;
			}

			if (!unique_torsion.has(it1->first))
			{
				torsions.push_back(it1->first);
				unique_torsion.insert(it1->first);
			}

			for (HashMap<Index, HashMap<String, ResidueRotamerSet> >::ConstIterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
			{
				if (!unique_torsion.has(it1->first))
				{
					torsions.push_back(it1->first);
					unique_torsion.insert(it1->first);
				}
			}
		}

		Size diff = 0;
		if (torsions.size() >= 2)
		{
			std::sort(torsions.begin(), torsions.end());
			diff = Maths::abs(torsions[0] - torsions[1]);
			for (Size i = 2; i < torsions.size(); ++i)
			{
				Size tmp = Maths::abs(torsions[i] - torsions[i - 1]);
				if (tmp != diff)
				{
					return false;
				}
			}
		}

		// set the step width of the rotamer torsions
		step_width_ = diff;
		return true;
	}

	void RotamerLibrary::sort()
	{
		if (backbone_dependent_)
		{
			for (HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >::Iterator it1 = bb_dep_sets_.begin(); it1 != bb_dep_sets_.end(); ++it1)
			{
				for (HashMap<Index, HashMap<String, ResidueRotamerSet> >::Iterator it2 = it1->second.begin(); it2 != it1->second.end(); ++it2)
				{
					for (HashMap<String, ResidueRotamerSet>::Iterator it3 = it2->second.begin(); it3 != it2->second.end(); ++it3)
					{
						it3->second.sort();
					}
				}
			}
		}
		else
		{
			for (HashMap<String, ResidueRotamerSet>::Iterator it = bb_indep_sets_.begin(); it != bb_indep_sets_.end(); ++it)
			{
				it->second.sort();
			}
		}
		return;
	}

  void RotamerLibrary::clear()
  {
    backbone_dependent_ = false;
    HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > >().swap(bb_dep_sets_);
    HashMap<String, ResidueRotamerSet>().swap(bb_indep_sets_);
  }

	bool RotamerLibrary::hasRotamers(const String& name) const
	{
		return getNumberOfRotamers(name) != 0;
	}
} // namespace BALL

