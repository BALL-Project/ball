// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ATOMTYPER_H
#define BALL_STRUCTURE_ATOMTYPER_H

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

#ifndef BALL_DATATYPE_STRINGHASHMAP_H
# include <BALL/DATATYPE/stringHashMap.h>
#endif

#include <vector>
#include <BALL/KERNEL/moleculeStore.h>     // v2.2 H3d.A: StableId-keyed

namespace BALL 
{
	using std::vector;

	class Molecule;
	class System;
	class Atom;

	/** Base class for a programable Atom typer.
	 		The types are assigned by using a line based file with SMARTS expressions.
			For usage have a look at the derived class MMFF94AtomTyper.\par
			<br><br>
			Usage:<br>
			\code
			AtomTyper typer;
			typer.setRings(...);
			typer.setAromaticRings(...);
			typer.setup(parameter_file_name);
			typer.assignTo(system);
			\endcode
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT AtomTyper
	{
		public:

		BALL_CREATE(AtomTyper)

		///
		AtomTyper();

		///
		AtomTyper(const AtomTyper& t);

		///
		virtual ~AtomTyper() {};

		/// Setup with the given line based file with SMARTS expressions
		bool setup(const String& filename);
		
		///
		virtual void assignTo(System& s);
		
		// v2.2 H3d.A: public API stays vector<HashSet<Atom*>>; impl
		// translates to sid form at the boundary (defined in
		// atomTyper.C). Forward-stable across reparent / recycle.
		void setAromaticRings(const vector<HashSet<Atom*> >& rings);
		void setRings(const vector<HashSet<Atom*> >& rings);

		// just for debugging: computational times for the individual SMARTS expressions
		static StringHashMap<float> rule_times;

		protected:
		
		///
		void assignTo(Molecule& mol);

		/** To be overloaded in derived classes 
				to add support for additional properties e.g. charges
		*/
		virtual void assignSpecificValues_(Atom&) {};

		/** To be overloaded in derived classes 
				to add support for additional properties e.g. charges
		*/
		virtual bool specificSetup_() { return true;}

		vector<String> 		names_;
		vector<String> 		rules_;
		vector<Index> 		types_;
		// entries in the file for specificSetup_() :
		vector<vector<String> > fields_;
		// number for fields per line in the config file
		Size 															number_expected_fields_;
		// v2.2 H3d.A (D-H3.8): internal SSSR + working sets keyed on
		// StableId. setRings / setAromaticRings (public API) translate
		// from the caller-provided vector<HashSet<Atom*>> at the boundary;
		// store identity is captured at first push (single-store
		// discipline; foreign-store atoms skipped). operator() walks
		// the input atom container, stamps each atom's sid into
		// atoms_, then uses the SmartsMatcher sid-keyed match overload.
		vector<HashSet<MoleculeStore::StableId> > rings_;
		vector<HashSet<MoleculeStore::StableId> > aromatic_rings_;
		StringHashMap<vector<Position> >          element_to_rules_;
		HashSet<MoleculeStore::StableId>          atoms_;
		MoleculeStore*                            atom_typer_store_ = nullptr;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_ATOMTYPER_H
