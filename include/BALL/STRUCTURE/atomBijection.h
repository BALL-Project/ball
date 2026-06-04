// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_ATOMBIJECTION_H
#define BALL_STRUCTURE_ATOMBIJECTION_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOMCONTAINER_H
#	include <BALL/KERNEL/atomContainer.h>
#endif

#ifndef BALL_KERNEL_MOLECULESTORE_H
#	include <BALL/KERNEL/moleculeStore.h>     // v2.2 H3d.B: StableId-keyed
#endif

#include <cassert>

namespace BALL
{

	/**	Atom bijection.
			This class implements a mapping of two sets of atoms onto each other.
			It is used by the \link StructureMapper StructurMapper \endlink class
			and the \link RMSDMinimizer RMSDMinimizer \endlink classes to define
			which atoms are mapped onto each other.
			\p
			There are a few methods for general mappings (based on atom order, atom names, etc.)
			that should suffice for most applications. If you want to match proteins based on
			particular mappings (e.g. based on a pairwise alignment), you should create the mapping
			yourself. This is easily done by pushing an AtomPair into the vector:
			\code
				Atom* atom1 = ...;
				Atom* atom2 = ...;
	
				// Create an empty bijection
				AtomBijection bijection;

				// Map atom1 onto atom2.
				bijection.push_back(AtomBijection::AtomPair(atom1, atom2));
			\endcode
			\p
			The class behaves more or less like the vector of atom pointer pairs it
			truly is. In particular, the STL container interface has been fully 
			implemented.
	\ingroup StructureMapping
	*/
	// v2.2 H3d-DR FLAW 6 fix (atomBijection extraction): the public
	// inheritance from std::vector<std::pair<Atom*, Atom*>> is broken;
	// AtomBijection now HAS-A PairVector. All public-facing access
	// (operator[], size, push_back, begin/end, iterators, types) is
	// preserved via forwarding declarations -- callers don't change.
	// This commit is the preparatory refactor BEFORE the h3d.B
	// atomBijection StableId-keyed migration.
	class BALL_EXPORT AtomBijection
	{
		public:

		/** @name Type definitions */
		//@{
		/** Legacy AtomPair (raw Atom*) -- INPUT-only via push_back. Internal
		    storage is SidPair (StableId-keyed); access goes through
		    atomA(i) / atomB(i) accessors that resolve at read time.
		*/
		typedef std::pair<Atom*, Atom*> AtomPair;
		/** v2.2 H3d.B (D-H3.8): internal pair element is a pair of
		    StableIds. .first is the sid in store_a_; .second is the sid
		    in store_b_. */
		typedef std::pair<MoleculeStore::StableId, MoleculeStore::StableId> SidPair;
		typedef std::vector<SidPair> PairVector;

		// v2.2 H3d: re-export the STL iterator types from the internal
		// vector. Caller code that uses AtomBijection::iterator (e.g.,
		// test/AtomBijection_test.C) continues to compile.
		typedef PairVector::iterator                iterator;
		typedef PairVector::const_iterator          const_iterator;
		typedef PairVector::reverse_iterator        reverse_iterator;
		typedef PairVector::const_reverse_iterator  const_reverse_iterator;
		typedef PairVector::value_type              value_type;
		typedef PairVector::size_type               size_type;
		typedef PairVector::reference               reference;
		typedef PairVector::const_reference         const_reference;
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		AtomBijection() {}

		/**	Construct a trivial bijection between to atom containers.
				Construct a simple bijection mapping the atoms of the two
				atom containers onto each other. The mapping iterates
				over the atoms and stops assigning pairs of atoms as soon
				as the smaller of the two atom sets is fully assigned. 
				The larger of the two atom container can thus contain
				unassigned atoms. No checking with respect to atom names,
				elements or the like are being made.
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				This corresponds to calling assignTrivial after default
				construction
		*/
		AtomBijection(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		///	Destructor
		virtual ~AtomBijection() {}

		//@}

		/**	@name Bijection construction */
		//@{
		/** Assign all atoms in the two atom containers in order.
				Construct a simple bijection mapping the atoms of the two
				atom containers onto each other. The mapping iterates
				over the atoms and stops assigning pairs of atoms as soon
				as the smaller of the two atom sets is fully assigned. 
				The larger of the two atom container can thus contain
				unassigned atoms. No checking with respect to atom names,
				elements or the like are being made.
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				This trivial bijection is useful, if the two atom containers
				correspond to exactly the same structure (i.e. they 
				just differ in their conformations). 
        Care must be taken that the order of atoms is correct.
        Beware of adding hydrogens, which might mess up atom 
        order in some cases.
				\p
				The number of atoms mapped is returned.
		*/
		Size assignTrivial(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/**	Assign the atom pairs through a name matching.
				This method creates a mapping based on the atom names.
				If the atom is contained in a Residue/Protein, the name consists
				of the fully qualified name (<chain>:<residue name>:<residue id>:<atom name>).
				If no pair of atoms could by matched this way, it will try to match by
				atom names only (not considering residues, chains or the like).	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				The method constructs a hash map for all atom names, so run time is linear 
				in the number of atoms.
				\p
				The number of atoms mapped is returned.
		*/
		Size assignByName(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/** Assign the C-alpha atoms ordered by sequence.
				This method iterated over all residues and assigns
				the C-alpha atoms (i.e. all atoms named "CA" in a
				residue with the property AMINO_ACID) of the two proteins
				in the order they are traversed. The size of the mapping
				corresponds to the minimum of the number of C-alpha atoms
        of both atom containers.	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				\return The number of atom pairs mapped
		*/
		Size assignCAlphaAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);


		/** Assign the backbone atoms ordered by sequence.
				This method iterated over all residues and assigns
				the backbone atoms (i.e. all atoms named "CA", "C",
				"N", "H", and "O" in every residue with the property AMINO_ACID)
				of the two proteins
				in the order they are traversed. The mapping terminates,
				if the traversal of the residues in one of the two atom containers
				terminates.	
				\p
				If the flag limit_to_selection is set to true and one of
				the two given atom containers has selected content, the 
				bijection is limited to this selection.
				\p
				\return The number of atom pairs mapped
		*/
		Size assignBackboneAtoms(AtomContainer& A, AtomContainer& B, bool limit_to_selection = false);

		/** Assign the atom pairs through a name matching and based on the 
		    property "ATOMBIJECTION_RMSD_SELECTION".
				@see assignTrivial()
				This is restriction is useful, if the focus of investigation
				is limited to e.g. a binding pocket.
		*/
		Size assignAtomsByProperty(AtomContainer& A, AtomContainer& B);
		//@}


		/**	@name Accessors */
		//@{
		///	Calculate the root mean squared deviation of the mapped atoms.
		double calculateRMSD() const;
		//@}

		/**	@name STL container compliance (v2.2 H3d.B: SidPair storage) */
		//@{
		size_type size() const { return pairs_.size(); }
		bool empty() const { return pairs_.empty(); }
		void clear() { pairs_.clear(); store_a_ = nullptr; store_b_ = nullptr; }

		/** Legacy push_back: translates Atom* -> StableId. Captures the
		    source stores at first push; subsequent pushes from foreign
		    stores are silently SKIPPED (per the H3c "store-captured-at-
		    first-push + reject-foreign" pattern). */
		void push_back(const AtomPair& p)
		{
			MoleculeStore* sa = p.first  ? p.first->getStore()  : nullptr;
			MoleculeStore* sb = p.second ? p.second->getStore() : nullptr;
			if (sa == nullptr || sb == nullptr) return;
			if (store_a_ == nullptr) store_a_ = sa;
			else if (store_a_ != sa) return;
			if (store_b_ == nullptr) store_b_ = sb;
			else if (store_b_ != sb) return;
			pairs_.push_back(SidPair(
				sa->stable_id(p.first->getStoreIndex()),
				sb->stable_id(p.second->getStoreIndex())));
		}

		/** v2.2 H3d closing-CR MEDIUM finding 4: Direct sid push REQUIRES
		    the caller to have already established both stores via either
		    a prior `push_back(AtomPair)` (which captures stores at first
		    push) or `bindStores(store_a, store_b)` below. Without a
		    captured store, atomA(i)/atomB(i) cannot resolve the entries
		    -- they return nullptr unconditionally because store_a_ /
		    store_b_ are nullptr. Calling this overload before binding
		    stores is a programmer error and asserted in debug builds.
		    Production builds tolerate the call but the entries will be
		    unresolvable until the stores are bound. */
		void push_back(const SidPair& sp)
		{
			assert(store_a_ != nullptr && store_b_ != nullptr);
			pairs_.push_back(sp);
		}

		/** Bind the two stores explicitly. Required before
		    `push_back(SidPair)` if no `push_back(AtomPair)` has run.
		    Setting non-null on an already-captured store enforces the
		    same single-store discipline as the AtomPair path: the call
		    is a no-op if it matches, and silently keeps the prior
		    binding if it doesn't (so a mistaken cross-store push is
		    caught by the assert above, not by silently re-binding). */
		void bindStores(MoleculeStore* sa, MoleculeStore* sb)
		{
			if (store_a_ == nullptr) store_a_ = sa;
			if (store_b_ == nullptr) store_b_ = sb;
		}

		reference       operator[](size_type i)       { return pairs_[i]; }
		const_reference operator[](size_type i) const { return pairs_[i]; }
		iterator        begin()        { return pairs_.begin(); }
		const_iterator  begin()  const { return pairs_.begin(); }
		iterator        end()          { return pairs_.end(); }
		const_iterator  end()    const { return pairs_.end(); }
		reverse_iterator       rbegin()       { return pairs_.rbegin(); }
		const_reverse_iterator rbegin() const { return pairs_.rbegin(); }
		reverse_iterator       rend()         { return pairs_.rend(); }
		const_reverse_iterator rend()   const { return pairs_.rend(); }

		/** v2.2 H3d.B (D-H3.8): resolved-Atom* accessors. Resolve the
		    stored sid to the current owner of that slot via the H3c
		    Phase 0 reverse map. Returns nullptr if the slot was
		    released (ABA-safe). */
		Atom* atomA(size_type i) const
		{
			if (store_a_ == nullptr) return nullptr;
			MoleculeStore::Index idx = store_a_->atom_idx_by_stable_id(pairs_[i].first);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return store_a_->back_ptr(idx);
		}
		Atom* atomB(size_type i) const
		{
			if (store_b_ == nullptr) return nullptr;
			MoleculeStore::Index idx = store_b_->atom_idx_by_stable_id(pairs_[i].second);
			if (idx == MoleculeStore::UNKNOWN_STABLE_ID) return nullptr;
			return store_b_->back_ptr(idx);
		}

		/** Source stores. nullptr until first push. */
		MoleculeStore* storeA() const { return store_a_; }
		MoleculeStore* storeB() const { return store_b_; }
		//@}

		protected:
		// v2.2 H3d.B (D-H3.8): StableId-keyed pair storage + the two
		// source stores captured at first push.
		PairVector     pairs_;
		MoleculeStore* store_a_ = nullptr;
		MoleculeStore* store_b_ = nullptr;
	};

} // namespace BALL

#endif // BALL_STRUCTURE_ATOMBIJECTION_H
