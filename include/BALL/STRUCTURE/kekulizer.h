// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: kekulizer.h,v 1.1.2.3 2006/10/08 15:34:30 amoll Exp $
//

#ifndef BALL_STRUCTURE_KEKULIZER_H
#define BALL_STRUCTURE_KEKULIZER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif 

#ifndef BALL_DATATYPE_HASHSET_H
# include <BALL/DATATYPE/hashSet.h>
#endif

namespace BALL
{
	class Atom;
	class Molecule;
	class Bond;

	/** Class to transform bonds with type "aromatic" to 
	 		conjugated single and double bonds.
			Useage:\\
			Kekulizer k;\\
			k.setAromaticRings(...);\\
			k.setRings(...);\\
			k.setup(Molecule& m);
			k.clear();
	*/
	class BALL_EXPORT Kekuliser
	{
		struct AtomInfo
		{
			Atom* atom;

			// needed for sorting:
			bool operator < (const AtomInfo& info) const;

			vector<Bond*> abonds;
			vector<Position> partner_id;

		 	Index current_charge;

			Index curr_double;

			Index min_double;
			Index max_double;

			Index min_double_charged;
			Index max_double_charged;
		};

		public:

		BALL_CREATE(Kekuliser)

		///
		Kekuliser();

		///
		virtual ~Kekuliser() {}

		///
		bool setup(Molecule& ac);

		///
		void setAromaticRings(const vector<HashSet<Atom*> >& rings) { aromatic_rings_ = rings;}
		
		///
		void setRings(const vector<HashSet<Atom*> >& rings) { rings_ = rings;}
		
		///
		const vector<Bond*>& getUnassignedBonds() const { return unassigned_bonds_; }

		///
		void clear();

		///
		void dump();

		protected:

		bool fixAromaticRings_();
		bool fixAromaticSystem_(Position it);
		inline bool buildConjugatedSystem_(Position it);
 		bool idealValenceAchieved_();

		void getMaximumValence_();

		// merge aromatic rings:
		void calculateAromaticSystems_();
		void collectSystems_(Atom& atom);
		void collectAromaticAtoms_();

		vector<HashSet<Atom*> > aromatic_systems_;
		vector<HashSet<Atom*> > aromatic_rings_;
		vector<HashSet<Atom*> > rings_;
		vector<Bond*> 					unassigned_bonds_;

		// atoms that take part in an aromatic bond:
		HashSet<const Atom*> 		aromatic_atoms_;
		HashMap<Atom*, Index> 	max_valence_;

		HashSet<Atom*> 					current_aromatic_system_;

		// current aromatic system:
		vector<AtomInfo> 				atom_infos_;
		bool 										try_charge_;
		bool 										protonate_;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_KEKULIZER_H
