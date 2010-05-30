// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_KEKULIZER_H
#define BALL_STRUCTURE_KEKULIZER_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_HASHMAP_H
# include <BALL/DATATYPE/hashMap.h>
#endif 

#include <set>

namespace BALL
{
	class Atom;
	class Molecule;
	class Bond;

	/** Class to transform bonds with type "aromatic" to 
	 		conjugated single and double bonds.
			<br>
			Useage:<br>
			\code
			Kekulizer k;
			k.setAromaticRings(...);
			k.setRings(...);
			k.setup(Molecule& m);
			k.clear();
			\endcode
			\ingroup StructureMiscellaneous
	*/
	class BALL_EXPORT Kekuliser
	{
		///
		struct BALL_EXPORT AtomInfo
		{
			// needed for sorting:
			bool operator < (const AtomInfo& info) const;
			AtomInfo& operator = (const AtomInfo& ai) ;

			// THE atom 
			Atom* atom;

			// bond to be set to a double bond
			Bond* double_bond;

			// aromatic bonds
			std::vector<Bond*> abonds;

			// position of the partner atoms in the vector of AtomInfos
			std::vector<Position> partner_id;

			// current number of double bonds for this atom
			Index curr_double;

			// minumum possible number of double bonds for this atom
			Index min_double;
			
			// maximum possible number of double bonds for this atom
			Index max_double;
			
			// number of double bonds for this atom to be uncharged
			Index uncharged_double;
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
		void setAromaticRings(const std::vector<std::set<Atom*> >& rings) { aromatic_rings_ = rings;}
		
		///
		void setRings(const std::vector<std::set<Atom*> >& rings) { rings_ = rings;}
		
		///
		const std::vector<Bond*>& getUnassignedBonds() const { return unassigned_bonds_; }

		///
		void clear();

		///
		void dump();

		///
		void setUseFormalCharges(bool state) { use_formal_charges_ = state;}

		///
		bool useFormalCharges() const { return use_formal_charges_;}

		protected:

		bool fixAromaticRings_();
		void fixAromaticSystem_(Position it);
		virtual Size getPenalty_(Atom& atom, Index charge);

		void getMaximumValence_();

		// merge aromatic rings:
		void calculateAromaticSystems_();
		void collectSystems_(Atom& atom);
		void collectAromaticAtoms_();
		bool hasAromaticBonds_(Atom& atom);
		void applySolution_(Position pos);
		Position calculateDistanceScores_();

		bool use_formal_charges_;

		std::vector<std::set<Atom*> > aromatic_systems_;
		std::vector<std::set<Atom*> > aromatic_rings_;
		std::vector<std::set<Atom*> > rings_;
		std::vector<Bond*> 					unassigned_bonds_;

		// atoms that take part in an aromatic bond:
		std::set<const Atom*> 		aromatic_atoms_;
		std::set<const Atom*> 		all_aromatic_atoms_;
		HashMap<Atom*, Index> 	max_valence_;

		std::set<Atom*> 					current_aromatic_system_;

		// current aromatic system:
		std::vector<AtomInfo> 				atom_infos_;
		Molecule*								molecule_;
		Size 										lowest_penalty_;
		Size 										current_penalty_;

		std::vector<std::vector<AtomInfo> > solutions_;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_KEKULIZER_H
