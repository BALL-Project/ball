// $Id: protein.C,v 1.10 2001/01/21 21:07:22 amoll Exp $

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/global.h>

using namespace std;

namespace BALL 
{

	Protein::Protein()
		throw()
		:	Molecule(),
			id_(BALL_PROTEIN_DEFAULT_ID)
	{
	}

	Protein::Protein(const Protein& protein, bool deep)
		throw()
		:	Molecule(protein, deep),
			id_(protein.id_)
	{
	}
		
	Protein::Protein(const String& name,const String& id)
		throw()
		:	Molecule(name),
			id_(id)
	{
	}

	Protein::~Protein()
		throw()
	{
		destroy();
	}

	void Protein::clear()
		throw()
	{
		Molecule::clear();
		id_ = BALL_PROTEIN_DEFAULT_ID;
	}
		
	void Protein::destroy()
		throw()
	{
		Molecule::destroy();
		id_ = BALL_PROTEIN_DEFAULT_ID;
	}
		
	void Protein::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			Molecule::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
		pm.writeObjectTrailer(name);
	}

	void Protein::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Molecule>());
			Molecule::persistentRead(pm);
		pm.checkObjectTrailer(0);
		pm.readPrimitive(id_, "id_");
	}

	void Protein::set(const Protein& protein, bool deep)
		throw()
	{
		Molecule::set(protein, deep);
		id_ = protein.id_;
	}
			
	const Protein& Protein::operator =(const Protein &protein)
		throw()
	{
		set(protein);
		return *this;
	}

	void Protein::get(Protein &protein, bool deep) const
		throw()
	{
		protein.set(*this, deep);
	}
			
	void Protein::swap(Protein& protein)
		throw()
	{
		Molecule::swap(protein);
		id_.swap(protein.id_);
	}
		
	Chain* Protein::getChain(Position position)
		throw()
	{
		for (ChainIterator chain_it = beginChain(); !chain_it.isEnd(); ++chain_it)
		{
			if (position-- == 0)
			{
				return &(*chain_it);
			}
		}

		return 0;
	}

	const Chain* Protein::getChain(Position position) const
		throw()
	{
		return ((Protein *)this)->getChain(position);
	}

	SecondaryStructure* Protein::getSecondaryStructure(Position position)
		throw()
	{
		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd(); ++secondary_structure_it)
		{
			if (position-- == 0)
			{
				return &(*secondary_structure_it);
			}
		}

		return 0;
	}

	const SecondaryStructure* Protein::getSecondaryStructure(Position position) const
		throw()
	{
		return ((Protein *)this)->getSecondaryStructure(position);
	}

	Residue* Protein::getResidue(Position position)
		throw()
	{
		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			if (position-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Residue* Protein::getResidue(Position position) const
		throw()
	{
		return ((Protein *)this)->getResidue(position);
	}

	Residue* Protein::getNTerminal()
		throw()
	{
		return (Residue *)::BALL::getNTerminal(*this);
	}
		
	const Residue* Protein::getNTerminal() const
		throw()
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue* Protein::getCTerminal()
		throw()
	{
		return (Residue *)::BALL::getCTerminal(*this);
	}
		
	const Residue* Protein::getCTerminal() const
		throw()
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom* Protein::getPDBAtom(Position position)
		throw()
	{
		for (PDBAtomIterator protein_atom_it = beginPDBAtom();
				 !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (position-- == 0)
			{
				return &(*protein_atom_it);
			}
		}

		return 0;
	}

	const PDBAtom* Protein::getPDBAtom(Position position) const
		throw()
	{
		return ((Protein *)this)->getPDBAtom(position);
	}

	void Protein::setID(const String& id)
		throw()
	{
		id_ = id;
	}

	const String& Protein::getID() const
		throw()
	{
		return id_;
	}

	Size Protein::countChains() const
		throw()
	{
		Size size = 0;

		for (ChainIterator chain_it = beginChain(); !chain_it.isEnd(); ++chain_it)
		{
			++size;
		}

		return size;
	}

	Size Protein::countSecondaryStructures() const
		throw()
	{
		Size size = 0;

		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd(); ++secondary_structure_it)
		{
			++size;
		}

		return size;
	}

	Size Protein::countResidues() const
		throw()
	{
		Size size = 0;

		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	Size Protein::countPDBAtoms() const
		throw()
	{
		Size size = 0;

		for (PDBAtomIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			++size;
		}

		return size;
	}

	bool Protein::isValid() const
		throw()
	{ 
		return(Molecule::isValid() && id_.isValid());
	}

	void	Protein::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Molecule::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Protein::read(istream& /* s */)
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Protein::write(ostream&  /* s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool Protein::operator == (const Protein& protein) const
		throw()
	{
		return(Object::operator == (protein));
	}

	bool Protein::operator != (const Protein& protein) const
		throw()
	{
		return ! (*this == protein);
	}


} // namespace BALL
