// $Id: nucleotide.C,v 1.4 2000/05/15 16:01:01 amoll Exp $

#include <BALL/KERNEL/nucleotide.h>

#include <BALL/KERNEL/nucleicAcid.h>

using namespace std;

namespace BALL 
{

	Nucleotide::Nucleotide()
		:	Fragment(),
			id_(BALL_NUCLEOTIDE_DEFAULT_ID),
			insertion_code_(BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE)
	{
	}
		
	Nucleotide::Nucleotide(const Nucleotide& nucleotide, bool deep)
		:	Fragment(nucleotide, deep),
			id_(nucleotide.id_),
			insertion_code_(nucleotide.insertion_code_)
	{
	}
		
	Nucleotide::Nucleotide(const String& name, const String& id, char insertion_code)
		:	Fragment(name),
			id_(id),
			insertion_code_(insertion_code)
	{
	}

	Nucleotide::~Nucleotide()
	{
		destroy();
	}

	void Nucleotide::clear()
	{
		Fragment::clear();

		clear_();
	}
		
	void Nucleotide::destroy()
	{
		Fragment::destroy();

		clear_();
	}

	void Nucleotide::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Fragment::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
			pm.writePrimitive(insertion_code_, "insertion_code_");
		pm.writeObjectTrailer(name);
	}

	void Nucleotide::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Fragment>());
			Fragment::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readPrimitive(id_, "id_");
		pm.readPrimitive(insertion_code_, "insertion_code_");
	}
		
	void Nucleotide::set(const Nucleotide& nucleotide, bool deep)
	{
		Fragment::set(nucleotide, deep);

		id_ = nucleotide.id_;
		insertion_code_ = nucleotide.insertion_code_;
	}
			
	Nucleotide& Nucleotide::operator =(const Nucleotide& nucleotide)
	{
		set(nucleotide);

		return *this;
	}

	void Nucleotide::get(Nucleotide& nucleotide, bool deep) const
	{
		nucleotide.set(*this, deep);
	}
			
	void Nucleotide::swap(Nucleotide& nucleotide)
	{
		Fragment::swap(nucleotide);

		id_.swap(nucleotide.id_);

		char temp_insertion_code = insertion_code_;
		insertion_code_ = nucleotide.insertion_code_;
		nucleotide.insertion_code_ = temp_insertion_code;
	}

	NucleicAcid* Nucleotide::getNucleicAcid()
	{
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			if (RTTI::isKindOf<NucleicAcid>(*ancestor_it))
			{
				return (NucleicAcid*)&*ancestor_it;
			}
		}

		return 0;
	}

	const NucleicAcid* Nucleotide::getNucleicAcid() const
	{
		return (const_cast<const Nucleotide*>(this)->getNucleicAcid());
	}

	void Nucleotide::setID(const String &id)
	{
		id_ = id;
	}

	const String &Nucleotide::getID() const
	{
		return id_;
	}

	void Nucleotide::setInsertionCode(char insertion_code)
	{
		insertion_code_ = insertion_code;
	}

	char Nucleotide::getInsertionCode() const
	{
		return insertion_code_;
	}

	void Nucleotide::prepend(Atom& atom)
	{
		Composite::prependChild(atom);
	}

	void Nucleotide::append(Atom &atom)
	{
		Composite::appendChild(atom);
	}

	void Nucleotide::insert(Atom &atom)
	{
		append(atom);
	}

	void Nucleotide::insertBefore(Atom &atom, Composite& before)
	{
		before.Composite::insertBefore(atom);
	}

	void Nucleotide::insertAfter(Atom& atom, Composite &after)
	{
		after.Composite::insertAfter(atom);
	}

	bool Nucleotide::remove(Atom& atom)
	{
		return Composite::removeChild(atom);
	}

	void Nucleotide::spliceBefore(Nucleotide& nucleotide)
	{
		Composite::spliceBefore(nucleotide);
	}

	void Nucleotide::spliceAfter(Nucleotide& nucleotide)
	{
		Composite::spliceAfter(nucleotide);
	}

	void Nucleotide::splice(Nucleotide& nucleotide)
	{
		Composite::splice(nucleotide);
	}

	bool Nucleotide::isTerminal() const
	{
		const NucleicAcid* parent = (*this).getNucleicAcid();
		/*if (parent != 0)
		{
			if (parent->get3Prime() == this ||
					parent->get5Prime() == this)
			{
				return true;
			}
		}*/
		return false;
	}

	bool Nucleotide::is5Prime() const
	{
		const NucleicAcid* parent = getNucleicAcid();
		if (parent != 0)
		{
			if (parent->get5Prime() == this)
			{
				return true;
			}
		}
		return false;
	}
		
	bool Nucleotide::is3Prime() const
	{
		const NucleicAcid* parent = getNucleicAcid();
		if (parent != 0)
		{
			if (parent->get3Prime() == this)
			{
				return true;
			}
		}
		return false;
	}

	bool Nucleotide::isValid() const
	{ 
		if (Fragment::isValid() == false
				|| id_.isValid() == false)
		{
			return false;
		}

		return true;
	}

	void Nucleotide::dump(ostream& s, Size depth) const
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Fragment::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_DEPTH(s, depth);
		s << "  insertion code: " << insertion_code_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void Nucleotide::read(istream&  /* s */)
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Nucleotide::write(ostream&  /*s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Nucleotide::clear_()
	{
		id_ = BALL_NUCLEOTIDE_DEFAULT_ID;
		insertion_code_ = BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE;
	}

} // namespace BALL
