// $Id: protein.C,v 1.4 1999/12/30 18:05:34 oliver Exp $

#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/global.h>

using namespace std;

namespace BALL 
{

	Protein::Protein()
		:	Molecule(),
			id_(BALL_PROTEIN_DEFAULT_ID)
	{
	}

	Protein::Protein(const Protein& protein, bool deep)
		:	Molecule(protein, deep),
			id_(protein.id_)
	{
	}
		
	Protein::Protein(const String& name,const String& id)
		:	Molecule(name),
			id_(id)
	{
	}

	Protein::~Protein()
	{
		destroy();
	}

	void Protein::clear()
	{
		Molecule::clear();

		clear_();
	}
		
	void Protein::destroy()
	{
		Molecule::destroy();

		clear_();
	}
		
	void Protein::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Molecule::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
		pm.writeObjectTrailer(name);
	}

	void Protein::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Molecule>());
			Molecule::persistentRead(pm);
		pm.checkObjectTrailer(0);
		pm.readPrimitive(id_, "id_");
	}


	void Protein::set(const Protein& protein, bool deep)
	{
		Molecule::set(protein, deep);

		id_ = protein.id_;
	}
			
	Protein& Protein::operator =(const Protein &protein)
	{
		set(protein);

		return *this;
	}

	void Protein::get(Protein &protein, bool deep) const
	{
		protein.set(*this, deep);
	}
			
	void Protein::swap(Protein& protein)
	{
		Molecule::swap(protein);

		id_.swap(protein.id_);
	}
		
	Chain *Protein::getChain(Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (ChainIterator chain_it = beginChain(); !chain_it.isEnd(); ++chain_it)
		{
			if (index-- == 0)
			{
				return &(*chain_it);
			}
		}

		return 0;
	}

	const Chain *
	Protein::getChain(Index index) const
	{
		return ((Protein *)this)->getChain(index);
	}

	SecondaryStructure* Protein::getSecondaryStructure(Index index)
	{
    if (index < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, index);
 
		for (SecondaryStructureIterator secondary_structure_it = beginSecondaryStructure();
				 !secondary_structure_it.isEnd(); ++secondary_structure_it)
		{
			if (index-- == 0)
			{
				return &(*secondary_structure_it);
			}
		}

		return 0;
	}

	const SecondaryStructure* Protein::getSecondaryStructure(Index index) const
	{
		return ((Protein *)this)->getSecondaryStructure(index);
	}

	Residue* Protein::getResidue(Index index)
	{
    if (index < 0)
      throw Exception::IndexUnderflow(__FILE__, __LINE__, index);
 
		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			if (index-- == 0)
			{
				return &(*res_it);
			}
		}

		return 0;
	}

	const Residue* Protein::getResidue(Index index) const
	{
		return ((Protein *)this)->getResidue(index);
	}

	Residue* Protein::getNTerminal()
	{
		return (Residue *)::BALL::getNTerminal(*this);
	}
		
	const Residue* Protein::getNTerminal() const
	{
		return ::BALL::getNTerminal(*this);
	}

	Residue* Protein::getCTerminal()
	{
		return (Residue *)::BALL::getCTerminal(*this);
	}
		
	const Residue* Protein::getCTerminal() const
	{
		return ::BALL::getCTerminal(*this);
	}

	PDBAtom* Protein::getPDBAtom(Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (PDBAtomIterator protein_atom_it = beginPDBAtom();
				 !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			if (index-- == 0)
			{
				return &(*protein_atom_it);
			}
		}

		return 0;
	}

	const PDBAtom* Protein::getPDBAtom(Index index) const
	{
		return ((Protein *)this)->getPDBAtom(index);
	}

	void Protein::setID(const String& id)
	{
		id_ = id;
	}

	String&  Protein::getID()
	{
		return id_;
	}

	const String& Protein::getID() const
	{
		return id_;
	}

	Size Protein::countChains() const
	{
		Size size = 0;

		for (ChainIterator chain_it = beginChain(); !chain_it.isEnd(); ++chain_it)
		{
			++size;
		}

		return size;
	}

	Size Protein::countSecondaryStructures() const
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
	{
		Size size = 0;

		for (ResidueIterator res_it = beginResidue(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	Size Protein::countPDBAtoms() const
	{
		Size size = 0;

		for (PDBAtomIterator protein_atom_it = beginPDBAtom(); !protein_atom_it.isEnd(); ++protein_atom_it)
		{
			++size;
		}

		return size;
	}

	bool Protein::isValid() const
	{ 
		if (Molecule::isValid() == false
				|| id_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void	Protein::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Molecule::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Protein::read(istream& /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Protein::write(ostream&  /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Protein::clear_()
	{
		id_ = BALL_PROTEIN_DEFAULT_ID;
	}

} // namespace BALL
