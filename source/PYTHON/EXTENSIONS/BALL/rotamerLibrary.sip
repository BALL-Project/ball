// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
%ModuleHeaderCode
	#include <BALL/STRUCTURE/rotamerLibrary.h>
%End

	class RotamerLibrary
	{
%TypeHeaderCode
	#include <BALL/STRUCTURE/rotamerLibrary.h>
	using namespace BALL;
%End
		public:
    static const String DEFAULT_LIBRARY;
    static const String DEFAULT_FRAGMENT_DB;
		RotamerLibrary() throw();
		RotamerLibrary(const String& filename, const FragmentDB& fragment_db) throw();
		RotamerLibrary(const FragmentDB& fragment_db) throw();
		RotamerLibrary(const RotamerLibrary& library) throw();
		virtual ~RotamerLibrary();
		//RotamerLibrary& operator = (const RotamerLibrary& rhs);
		ResidueRotamerSet* getRotamerSet(const String& name);
		ResidueRotamerSet* getRotamerSet(const String& name, float phi, float psi);
		ResidueRotamerSet* getRotamerSet(const Residue& residue);
		Size getNumberOfRotamers() const;
		Size getNumberOfRotamers(const String& name) const;
		Size getNumberOfRotamerSets() const;
		void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions, Index phi, Index psi);
		void addRotamer(const String& name, const Rotamer& rotamer, Size number_of_torsions);
		bool isBackboneDependent() const;
		void setBackboneDependent(bool dependent);
		bool hasRotamers(const String& name) const;
		bool validate();
		void sort();
    void clear();
		// HashMap<Index, HashMap<Index, HashMap<String, ResidueRotamerSet> > > bb_dep_sets_;
		// HashMap<String, ResidueRotamerSet> bb_indep_sets_;
		//Index getNearestBackboneTorsion_(double angle) const;
		//Size step_width_;
		//FragmentDB fragment_db_;
		//bool backbone_dependent_;
	};
