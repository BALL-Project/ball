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
		
		///
		void setAromaticRings(const vector<HashSet<Atom*> >& rings) { aromatic_rings_ = rings;}

		///
		void setRings(const vector<HashSet<Atom*> >& rings) { rings_ = rings;}

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
		vector<HashSet<Atom*> > 					rings_;
		vector<HashSet<Atom*> > 					aromatic_rings_;
		StringHashMap<vector<Position> > 	element_to_rules_;
		HashSet<Atom*> 										atoms_;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_ATOMTYPER_H
