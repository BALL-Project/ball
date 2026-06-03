// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/KERNEL/PDBAtom.h>
#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>

using namespace::std;

namespace BALL 
{

	// v2.2 H3a.3b step 5 (D-H3.10-R5 / D-H3.6): all 4 PDBAtom ctors now call
	// the tagged Atom overloads with AtomCtor::Origin{0x01}. The hint flows
	// through Atom::origin_hint_ -> bindToStore_ -> allocate_atom(this,
	// origin_hint_); the slot's origin_flags_ column is born marked as
	// PDB-origin in the same atomic critical section that publishes
	// back_ptr_. No (back_ptr=PDBAtom, origin_flags=0) tearing window.

	PDBAtom::PDBAtom()
		:	Atom(AtomCtor::Origin{0x01}),
			branch_designator_(BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR),
			remoteness_indicator_(BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR),
			alternate_location_indicator_(BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR),
			occupancy_(BALL_PDBATOM_DEFAULT_OCCUPANCY),
			temperature_factor_(BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR)
	{
	}

	PDBAtom::PDBAtom(const PDBAtom& pdb_atom, bool deep)
		:	Atom(pdb_atom, deep, AtomCtor::Origin{0x01}),
			branch_designator_(pdb_atom.branch_designator_),
			remoteness_indicator_(pdb_atom.remoteness_indicator_),
			alternate_location_indicator_(pdb_atom.alternate_location_indicator_),
			occupancy_(pdb_atom.occupancy_),
			temperature_factor_(pdb_atom.temperature_factor_)
	{
	}

	PDBAtom::PDBAtom
		(Element& element,
		 const String& name,
		 const String& type_name,
		 Atom::Type atom_type,
		 const Vector3& position,
		 const Vector3& velocity,
		 const Vector3& force,
		 float charge,
		 float radius,
		 char branch_designator,
		 char remoteness_indicator,
		 char alternate_location_indicator,
		 float occupancy,
		 float temperature_factor)
		// Tagged detailed Atom ctor is fully non-defaulted (DR4 Fix 1). Pass
		// formal_charge = BALL_ATOM_DEFAULT_FORMAL_CHARGE explicitly because the
		// v0 PDBAtom detailed ctor never exposed formal_charge to its callers --
		// preserves the prior behavior (Atom-detailed-ctor's default).
		:	Atom(element, name, type_name, atom_type, position, velocity, force,
			     charge, radius, BALL_ATOM_DEFAULT_FORMAL_CHARGE, AtomCtor::Origin{0x01}),
			branch_designator_(branch_designator),
			remoteness_indicator_(remoteness_indicator),
			alternate_location_indicator_(alternate_location_indicator),
			occupancy_(occupancy),
			temperature_factor_(temperature_factor)
	{
	}

	PDBAtom::PDBAtom(const String& name)
		:	Atom(AtomCtor::Origin{0x01}),
			branch_designator_(BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR),
			remoteness_indicator_(BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR),
			alternate_location_indicator_(BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR),
			occupancy_(BALL_PDBATOM_DEFAULT_OCCUPANCY),
			temperature_factor_(BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR)
	{
		Atom::setName(name);
	}

	PDBAtom::~PDBAtom()
	{
		destroy();
	}

	void PDBAtom::clear()
	{
		Atom::clear();
		clear_();
	}
		
	void PDBAtom::destroy()
	{
		Atom::destroy();
		clear_();
	}


  void PDBAtom::set(const PDBAtom& pdb_atom, bool deep)
  {
    Atom::set(pdb_atom, deep);

    branch_designator_ = pdb_atom.branch_designator_;
    remoteness_indicator_ = pdb_atom.remoteness_indicator_;
    alternate_location_indicator_ = pdb_atom.alternate_location_indicator_;
    occupancy_ = pdb_atom.occupancy_;
    temperature_factor_ = pdb_atom.temperature_factor_;
  }			

  void PDBAtom::get(PDBAtom& pdb_atom, bool deep) const
  {
    pdb_atom.set(*this, deep);
  }

	PDBAtom& PDBAtom::operator = (const PDBAtom& pdb_atom)
	{
		Atom::operator =(pdb_atom);

		branch_designator_ = pdb_atom.branch_designator_;
		remoteness_indicator_ = pdb_atom.remoteness_indicator_;
		alternate_location_indicator_ = pdb_atom.alternate_location_indicator_;
		occupancy_ = pdb_atom.occupancy_;
		temperature_factor_ = pdb_atom.temperature_factor_;

		return *this;
	}
			
	void PDBAtom::swap(PDBAtom& pdb_atom)
	{
		Atom::swap(pdb_atom);

		char temp = branch_designator_;
		branch_designator_ = pdb_atom.branch_designator_;
		pdb_atom.branch_designator_ = temp;

		temp = remoteness_indicator_;
		remoteness_indicator_ = pdb_atom.remoteness_indicator_;
		pdb_atom.remoteness_indicator_ = temp;

		temp = alternate_location_indicator_;
		alternate_location_indicator_ = pdb_atom.alternate_location_indicator_;
		pdb_atom.alternate_location_indicator_ = temp;
		
		float temp_float = occupancy_;
		occupancy_ = pdb_atom.occupancy_;
		pdb_atom.occupancy_ = temp_float;

		temp_float = temperature_factor_;
		temperature_factor_ = pdb_atom.temperature_factor_;
		pdb_atom.temperature_factor_ = temp_float;
	}
		
	Protein* PDBAtom::getProtein()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<Protein>(&*ancestor_it))
			{
				return (Protein *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Protein* PDBAtom::getProtein() const
	{
		return ((PDBAtom *)this)->getProtein();
	}

	Chain* PDBAtom::getChain()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<Chain>(&*ancestor_it))
			{
				return (Chain *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Chain* PDBAtom::getChain() const
	{
		return ((PDBAtom *)this)->getChain();
	}

	Residue* PDBAtom::getResidue()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor();
				 !ancestor_it.isEnd(); ++ancestor_it)
		{
            if (RTTI::isKindOf<Residue>(&*ancestor_it))
			{
				return (Residue *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Residue* PDBAtom::getResidue() const
	{
		return ((PDBAtom *)this)->getResidue();
	}

	void PDBAtom::setBranchDesignator(char branch_designator)
	{
		branch_designator_ = branch_designator;
	}

	char PDBAtom::getBranchDesignator() const
	{
		return branch_designator_;
	}

	void PDBAtom::setRemotenessIndicator(char remoteness_indicator)
	{
		remoteness_indicator_ = remoteness_indicator;
	}

	char PDBAtom::getRemotenessIndicator() const
	{
		return remoteness_indicator_;
	}

	void PDBAtom::setAlternateLocationIndicator(char alternate_location_indicator)
	{
		alternate_location_indicator_ = alternate_location_indicator;
	}

	char PDBAtom::getAlternateLocationIndicator() const
	{
		return alternate_location_indicator_;
	}

	void PDBAtom::setOccupancy(float occupancy)
	{
		occupancy_ = occupancy;
	}

	float PDBAtom::getOccupancy() const
	{
		return occupancy_;
	}

	void PDBAtom::setTemperatureFactor(float temperature_factor)
	{
		temperature_factor_ = temperature_factor;
	}

	float PDBAtom::getTemperatureFactor() const
	{
		return temperature_factor_;
	}

	void PDBAtom::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s)
		
		Atom::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) {};
	 
		BALL_DUMP_STREAM_SUFFIX(s)
	}

	void PDBAtom::clear_()
	{
		branch_designator_						= BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
		remoteness_indicator_					= BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
		alternate_location_indicator_ = BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR;
		occupancy_										= BALL_PDBATOM_DEFAULT_OCCUPANCY;
		temperature_factor_						= BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR;
	}

	bool PDBAtom::operator == (const PDBAtom& pdb_atom) const
	{
		return(Object::operator == (pdb_atom));
	}

	bool PDBAtom::operator != (const PDBAtom& pdb_atom) const
	{
		return ! (*this == pdb_atom);
	}


} // namespace BALL
