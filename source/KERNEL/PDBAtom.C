// $Id: PDBAtom.C,v 1.6 2000/05/09 16:13:15 amoll Exp $

#include <BALL/KERNEL/PDBAtom.h>

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/residue.h>

using namespace std;

namespace BALL 
{

	PDBAtom::PDBAtom()
		:	Atom(),
			branch_designator_(BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR),
			remoteness_indicator_(BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR),
			alternate_location_indicator_(BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR),
			occupancy_(BALL_PDBATOM_DEFAULT_OCCUPANCY),
			temperature_factor_(BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR)
	{
	}
		
	PDBAtom::PDBAtom(const PDBAtom& pdb_atom, bool deep)
		:	Atom(pdb_atom, deep),
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
		:	Atom(element, name, type_name, atom_type,
				 position, velocity, force, charge, radius),
			branch_designator_(branch_designator),
			remoteness_indicator_(remoteness_indicator),
			alternate_location_indicator_(alternate_location_indicator),
			occupancy_(occupancy),
			temperature_factor_(temperature_factor)
	{
	}
		
	PDBAtom::PDBAtom(const String& name)
		:	Atom(),
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

	void PDBAtom::persistentWrite(PersistenceManager& pm, const char* name) const	
	{
		pm.writeObjectHeader(this, name);
			Atom::persistentWrite(pm);
			pm.writePrimitive(branch_designator_, "branch_designator_");
			pm.writePrimitive(remoteness_indicator_, "remoteness_indicator_");
			pm.writePrimitive(alternate_location_indicator_, "alternate_location_indicator_");
			pm.writePrimitive(occupancy_, "occupancy_");
			pm.writePrimitive(temperature_factor_, "temperature_factor_");		
		pm.writeObjectTrailer(name);
	}

	void PDBAtom::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Atom>());
			Atom::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readPrimitive(branch_designator_, "branch_designator_");
		pm.readPrimitive(remoteness_indicator_, "remoteness_indicator_");
		pm.readPrimitive(alternate_location_indicator_, "alternate_location_indicator_");
		pm.readPrimitive(occupancy_, "occupancy_");
		pm.readPrimitive(temperature_factor_, "temperature_factor_");		
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
			
	PDBAtom& PDBAtom::operator =(const PDBAtom& pdb_atom)
	{
		set(pdb_atom);

		return *this;
	}

	void PDBAtom::get(PDBAtom& pdb_atom, bool deep) const
	{
		pdb_atom.set(*this, deep);
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
			if (RTTI::isKindOf<Protein>(*ancestor_it))
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
			if (RTTI::isKindOf<Chain>(*ancestor_it))
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
			if (RTTI::isKindOf<Residue>(*ancestor_it))
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
		BALL_DUMP_STREAM_PREFIX(s);
		
		Atom::dump(s, depth);
		// just to avoid these damned compiler warnings
		// (dump_indent_depth_ was declared but never referenced)
		if (dump_indent_depth_ == 0) ;
	 
		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void PDBAtom::read(istream & /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}


	void PDBAtom::write(ostream& /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void PDBAtom::clear_()
	{
		branch_designator_						= BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
		remoteness_indicator_					= BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
		alternate_location_indicator_ = BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR;
		occupancy_										= BALL_PDBATOM_DEFAULT_OCCUPANCY;
		temperature_factor_						= BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR;
	}

} // namespace BALL
