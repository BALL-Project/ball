// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// BALL 2.0 KERNEL replacement (K0.2).
// See .planning/v2.x/KERNEL-V2-DECISIONS.md for the design contract.
//

#ifndef BALL_KERNEL_MOLECULESTORE_H
#define BALL_KERNEL_MOLECULESTORE_H

#ifndef BALL_COMMON_GLOBAL_H
# include <BALL/COMMON/global.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
# include <BALL/MATHS/vector3.h>
#endif

#include <cstdint>
#include <string>
#include <vector>

namespace BALL
{
	class Atom;
	class Bond;

	/**	Bond record (struct-of-arrays bond table row).
			12 bytes; first/second atom indices (uint32) + bond order (uint8) +
			bond type (uint8) + flags (uint16). Compact replacement for the
			heavyweight v1.x Bond object.
	*/
	struct BALL_EXPORT BondRecord
	{
		std::uint32_t a;
		std::uint32_t b;
		std::uint8_t  order;
		std::uint8_t  type;
		std::uint16_t flags;
	};

	/**	MoleculeStore — ground truth for per-atom and per-bond data.

			SoA columns (positions, velocities, forces, charges, element,
			selection bitmap, name offsets into a string pool, residue index,
			stable id, and a back-pointer to the owning Atom handle).

			Owned per System (D5 in KERNEL-V2-DECISIONS.md). Atom and Bond are
			handle objects holding (store*, idx, generation); their getters
			dispatch through the store. Reference-return getters
			(Vector3& Atom::getPosition()) bind to a column row and stay valid
			until the next store reallocation, which is detected via the
			generation counter (D7).

			@see Atom
			@see Bond
			@see System

			\ingroup  KernelContainers
	*/
	class BALL_EXPORT MoleculeStore
	{
		public:

		typedef std::uint32_t Index;
		typedef std::uint64_t Generation;
		typedef std::uint64_t StableId;

		/**	@name Construction
		*/
		//@{

		MoleculeStore();
		~MoleculeStore();

		// MoleculeStore is non-copyable for now; cloning copies are a v2.0.x
		// follow-on (D12 persistence-format work).
		MoleculeStore(const MoleculeStore&)            = delete;
		MoleculeStore& operator=(const MoleculeStore&) = delete;

		//@}
		/**	@name Atom column accessors (used by Atom handle)
		*/
		//@{

		Vector3&       position(Index i)             { return positions_[i]; }
		const Vector3& position(Index i) const       { return positions_[i]; }

		Vector3&       velocity(Index i)             { return velocities_[i]; }
		const Vector3& velocity(Index i) const       { return velocities_[i]; }

		Vector3&       force(Index i)                { return forces_[i]; }
		const Vector3& force(Index i) const          { return forces_[i]; }

		float&         charge(Index i)               { return charges_[i]; }
		float          charge(Index i) const         { return charges_[i]; }

		float&         radius(Index i)               { return radii_[i]; }
		float          radius(Index i) const         { return radii_[i]; }

		short&         atom_type(Index i)            { return atom_types_[i]; }
		short          atom_type(Index i) const      { return atom_types_[i]; }

		short&         formal_charge(Index i)        { return formal_charges_[i]; }
		short          formal_charge(Index i) const  { return formal_charges_[i]; }

		std::uint8_t&  element_index(Index i)        { return element_indices_[i]; }
		std::uint8_t   element_index(Index i) const  { return element_indices_[i]; }

		bool           selected(Index i) const       { return selection_[i] != 0; }
		void           set_selected(Index i, bool v) { selection_[i] = v ? 1 : 0; }

		StableId       stable_id(Index i) const      { return stable_ids_[i]; }

		Atom*          back_ptr(Index i) const       { return back_ptr_[i]; }
		void           set_back_ptr(Index i, Atom* p){ back_ptr_[i] = p; }

		// String pool for atom & type names. Each atom has two name_offset
		// values (name, type_name) into the pool.
		const std::string& string_pool() const       { return string_pool_; }
		std::uint32_t      name_offset(Index i) const{ return name_offsets_[i]; }
		std::uint32_t      type_name_offset(Index i) const { return type_name_offsets_[i]; }

		void               set_name(Index i, const std::string& s);
		void               set_type_name(Index i, const std::string& s);
		std::string        get_name(Index i) const;
		std::string        get_type_name(Index i) const;

		//@}
		/**	@name Lifecycle
		*/
		//@{

		// Allocate a new atom slot. Returns its index. Generation advances if
		// the column vectors had to reallocate.
		Index allocate_atom();

		// Reserve N slots up front so subsequent allocate_atom() calls don't
		// reallocate (the "reserve discipline" of the D7 reference contract).
		void  reserve(std::size_t n);

		// Shrink columns to fit. Always advances generation (drops references).
		void  compact();

		// Current logical size + capacity.
		std::size_t size() const     { return positions_.size(); }
		std::size_t capacity() const { return positions_.capacity(); }

		//@}
		/**	@name Bond table
		*/
		//@{

		std::uint32_t add_bond(Index a, Index b,
		                       std::uint8_t order = 1,
		                       std::uint8_t type = 0);

		std::size_t bond_count() const               { return bonds_.size(); }
		BondRecord& bond(std::uint32_t i)            { return bonds_[i]; }
		const BondRecord& bond(std::uint32_t i) const{ return bonds_[i]; }

		// Bond handle back-pointers (parallel to bonds_).
		Bond*       bond_back_ptr(std::uint32_t i) const   { return bond_back_ptr_[i]; }
		void        set_bond_back_ptr(std::uint32_t i, Bond* p) { bond_back_ptr_[i] = p; }

		// Returns all bond-record indices that touch atom i. Linear scan in
		// the K0.2 skeleton; CSR adjacency lands in K0.3.
		std::vector<std::uint32_t> bonds_of(Index i) const;

		//@}
		/**	@name Generation + dirty-tracking
		*/
		//@{

		Generation generation() const                { return generation_; }
		Generation selection_generation() const      { return selection_generation_; }

		// Mark the selection bitmap dirty (any selection_-mutating call should
		// invoke this). Independent of structural generation.
		void mark_selection_dirty()                  { ++selection_generation_; }

		//@}

		private:

		void bump_generation_if_reallocated_(std::size_t old_cap);

		// Atom columns. All vectors are sized to size().
		std::vector<Vector3>      positions_;
		std::vector<Vector3>      velocities_;
		std::vector<Vector3>      forces_;
		std::vector<float>        charges_;
		std::vector<float>        radii_;
		std::vector<short>        atom_types_;
		std::vector<short>        formal_charges_;
		std::vector<std::uint8_t> element_indices_;
		std::vector<std::uint8_t> selection_;
		std::vector<std::uint32_t> name_offsets_;
		std::vector<std::uint32_t> type_name_offsets_;
		std::vector<StableId>     stable_ids_;
		std::vector<Atom*>        back_ptr_;

		std::string               string_pool_;

		// Bond table.
		std::vector<BondRecord>   bonds_;
		std::vector<Bond*>        bond_back_ptr_;

		Generation generation_           = 0;
		Generation selection_generation_ = 0;
		StableId   next_stable_id_       = 1;
	};

} // namespace BALL

#endif // BALL_KERNEL_MOLECULESTORE_H
