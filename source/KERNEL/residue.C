// $Id: residue.C,v 1.3 1999/12/04 18:34:30 oliver Exp $

#include <BALL/KERNEL/residue.h>

#include <BALL/KERNEL/chain.h>
#include <BALL/KERNEL/protein.h>

namespace BALL 
{

	Residue::Residue()
		:	Fragment(),
			id_(BALL_RESIDUE_DEFAULT_ID),
			insertion_code_(BALL_RESIDUE_DEFAULT_INSERTION_CODE)
	{
	}
		
	Residue::Residue(const Residue& residue, bool deep)
		:	Fragment(residue, deep),
			id_(residue.id_),
			insertion_code_(residue.insertion_code_)
	{
	}
		
	Residue::Residue(const String& name, const String& id, char insertion_code)
		:	Fragment(name),
			id_(id),
			insertion_code_(insertion_code)
	{
	}

	Residue::~Residue()
	{
		destroy();
	}

	void Residue::clear()
	{
		Fragment::clear();

		clear_();
	}
		
	void Residue::destroy()
	{
		Fragment::destroy();

		clear_();
	}

	void Residue::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Fragment::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
			pm.writePrimitive(insertion_code_, "insertion_code_");
		pm.writeObjectTrailer(name);
	}

	void Residue::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI<Fragment>::getStreamName());
			Fragment::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readPrimitive(id_, "id_");
		pm.readPrimitive(insertion_code_, "insertion_code_");
	}
		
	void Residue::set(const Residue& residue, bool deep)
	{
		Fragment::set(residue, deep);

		id_ = residue.id_;
		insertion_code_ = residue.insertion_code_;
	}
			
	Residue& Residue::operator =(const Residue& residue)
	{
		set(residue);

		return *this;
	}

	void Residue::get(Residue& residue, bool deep) const
	{
		residue.set(*this, deep);
	}
			
	void Residue::swap(Residue& residue)
	{
		Fragment::swap(residue);

		id_.swap(residue.id_);

		char temp_insertion_code = insertion_code_;
		insertion_code_ = residue.insertion_code_;
		residue.insertion_code_ = temp_insertion_code;
	}

	Protein* Residue::getProtein()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI<Protein>::isKindOf(*ancestor_it))
			{
				return (Protein *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Protein *Residue::getProtein() const
	{
		return ((Residue *)this)->getProtein();
	}

	Chain* Residue::getChain()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI<Chain>::isKindOf(*ancestor_it))
			{
				return (Chain *)&*ancestor_it;
			}
		}

		return 0;
	}

	const Chain *Residue::getChain() const
	{
		return ((Residue *)this)->getChain();
	}

	PDBAtom *Residue::getPDBAtom(Index index)
	{
		if (index < 0)
			throw Exception::IndexUnderflow(__FILE__, __LINE__, index);

		for (PDBAtomIterator protein_atom_iterator = beginPDBAtom();
				 !protein_atom_iterator.isEnd(); ++protein_atom_iterator)
		{
			if (index-- == 0)
			{
				return &(*protein_atom_iterator);
			}
		}

		return 0;
	}

	const PDBAtom* Residue::getPDBAtom(Index index) const
	{
		return ((Residue *)this)->getPDBAtom(index);
	}

	void Residue::setID(const String &id)
	{
		id_ = id;
	}

	String & Residue::getID()
	{
		return id_;
	}

	const String &Residue::getID() const
	{
		return id_;
	}

	void Residue::setInsertionCode(char insertion_code)
	{
		insertion_code_ = insertion_code;
	}

	char Residue::getInsertionCode() const
	{
		return insertion_code_;
	}

	Size Residue::countPDBAtoms() const
	{
		register Size size = 0;

		for (PDBAtomIterator protein_atom_iterator = beginPDBAtom();
				 !protein_atom_iterator.isEnd(); ++protein_atom_iterator)
		{
			++size;
		}

		return size;
	}

	void Residue::prepend(PDBAtom &protein_atom)
	{
		Fragment::prepend(protein_atom);
	}

	void Residue::append(PDBAtom& protein_atom)
	{
		Fragment::append(protein_atom);
	}

	void 
	Residue::insert(PDBAtom& protein_atom)
	{
		Fragment::insert(protein_atom);
	}

	void Residue::insertBefore(PDBAtom& protein_atom, Composite& before)
	{
		Fragment::insertBefore(protein_atom, before);
	}

	void Residue::insertAfter(PDBAtom& protein_atom, Composite& after)
	{
		Fragment::insertAfter(protein_atom, after);
	}

	bool Residue::remove(PDBAtom& protein_atom)
	{
		return Fragment::remove(protein_atom);
	}

	void Residue::spliceBefore(Residue& residue)
	{
		Fragment::spliceBefore(residue);
	}

	void Residue::spliceAfter(Residue& residue)
	{
		Fragment::spliceAfter(residue);
	}

	void Residue::splice(Residue& residue)
	{
		Fragment::splice(residue);
	}

	bool Residue::isAminoAcid() const
	{
		return hasProperty(PROPERTY__AMINO_ACID);
	}

	bool Residue::isTerminal() const
	{
		return (isNTerminal() || isCTerminal());
	}

	bool Residue::isNTerminal() const
	{
		if (isAminoAcid() == true)
		{
			const Chain* chain = getChain();

			if (chain != 0)
			{
				ResidueConstIterator res_it = chain->beginResidue();
				for (; +res_it && &(*res_it) != this && !res_it->isAminoAcid(); ++res_it);

				return (&(*res_it) == this);
			}
		}

		return false;
	}
		
	bool Residue::isCTerminal() const
	{
		if (isAminoAcid() == true)
		{
			const Chain* chain = getChain();

			if (chain != 0)
			{
				ResidueConstIterator res_it = chain->rbeginResidue();
				for (; +res_it && &(*res_it) != this && !res_it->isAminoAcid(); --res_it);

				return (&(*res_it) == this);
			}
		}

		return false;
	}
		
	bool Residue::isValid() const
	{ 
		if (Fragment::isValid() == false
				|| id_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void Residue::dump(ostream& s, unsigned long depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Fragment::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  insertion code: " << insertion_code_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Residue::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Residue::write(ostream&  /*s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Residue::clear_()
	{
		id_ = BALL_RESIDUE_DEFAULT_ID;
		insertion_code_ = BALL_RESIDUE_DEFAULT_INSERTION_CODE;
	}

} // namespace BALL
