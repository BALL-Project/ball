// $Id: nucleotide.C,v 1.8 2001/01/14 21:57:16 amoll Exp $

#include <BALL/KERNEL/nucleotide.h>

#include <BALL/KERNEL/nucleicAcid.h>

using namespace std;

namespace BALL 
{

	Nucleotide::Nucleotide()
		throw()
		:	Fragment(),
			id_(BALL_NUCLEOTIDE_DEFAULT_ID),
			insertion_code_(BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE)
	{
	}
		
	Nucleotide::Nucleotide(const Nucleotide& nucleotide, bool deep)
		throw()
		:	Fragment(nucleotide, deep),
			id_(nucleotide.id_),
			insertion_code_(nucleotide.insertion_code_)
	{
	}
		
	Nucleotide::Nucleotide(const String& name, const String& id, char insertion_code)
		throw()
		:	Fragment(name),
			id_(id),
			insertion_code_(insertion_code)
	{
	}

	Nucleotide::~Nucleotide()
		throw()
	{
		destroy();
	}

	void Nucleotide::clear()
		throw()
	{
		Fragment::clear();
		id_ = BALL_NUCLEOTIDE_DEFAULT_ID;
		insertion_code_ = BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE;
	}
		
	void Nucleotide::destroy()
		throw()
	{
		Fragment::destroy();
		id_ = BALL_NUCLEOTIDE_DEFAULT_ID;
		insertion_code_ = BALL_NUCLEOTIDE_DEFAULT_INSERTION_CODE;
	}

	void Nucleotide::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			Fragment::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
			pm.writePrimitive(insertion_code_, "insertion_code_");
		pm.writeObjectTrailer(name);
	}

	void Nucleotide::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Fragment>());
			Fragment::persistentRead(pm);
		pm.checkObjectTrailer(0);

		pm.readPrimitive(id_, "id_");
		pm.readPrimitive(insertion_code_, "insertion_code_");
	}
		
	void Nucleotide::set(const Nucleotide& nucleotide, bool deep)
		throw()
	{
		Fragment::set(nucleotide, deep);
		id_ = nucleotide.id_;
		insertion_code_ = nucleotide.insertion_code_;
	}
			
	const Nucleotide& Nucleotide::operator =(const Nucleotide& nucleotide)
		throw()
	{
		set(nucleotide);
		return *this;
	}

	void Nucleotide::get(Nucleotide& nucleotide, bool deep) const
		throw()
	{
		nucleotide.set(*this, deep);
	}
			
	void Nucleotide::swap(Nucleotide& nucleotide)
		throw()
	{
		Fragment::swap(nucleotide);

		id_.swap(nucleotide.id_);

		char temp_insertion_code = insertion_code_;
		insertion_code_ = nucleotide.insertion_code_;
		nucleotide.insertion_code_ = temp_insertion_code;
	}

	NucleicAcid* Nucleotide::getNucleicAcid()
		throw()
	{
		NucleicAcid* nucleic_acid_ptr = 0;
		for (Composite::AncestorIterator ancestor_it = beginAncestor(); !ancestor_it.isEnd(); ++ancestor_it)
		{
			nucleic_acid_ptr = dynamic_cast<NucleicAcid*>(&*ancestor_it);
			if (nucleic_acid_ptr != 0)
			{
				break;
			}
		}

		return nucleic_acid_ptr;
	}

	const NucleicAcid* Nucleotide::getNucleicAcid() const
		throw()
	{
		return (const_cast<Nucleotide*>(this)->getNucleicAcid());
	}

	void Nucleotide::setID(const String &id)
		throw()
	{
		id_ = id;
	}

	const String &Nucleotide::getID() const
		throw()
	{
		return id_;
	}

	void Nucleotide::setInsertionCode(char insertion_code)
		throw()
	{
		insertion_code_ = insertion_code;
	}

	char Nucleotide::getInsertionCode() const
		throw()
	{
		return insertion_code_;
	}

	void Nucleotide::prepend(Atom& atom)
		throw()
	{
		Composite::prependChild(atom);
	}

	void Nucleotide::append(Atom &atom)
		throw()
	{
		Composite::appendChild(atom);
	}

	void Nucleotide::insert(Atom &atom)
		throw()
	{
		append(atom);
	}

	void Nucleotide::insertBefore(Atom &atom, Composite& before)
		throw()
	{
		before.Composite::insertBefore(atom);
	}

	void Nucleotide::insertAfter(Atom& atom, Composite &after)
		throw()
	{
		after.Composite::insertAfter(atom);
	}

	bool Nucleotide::remove(Atom& atom)
		throw()
	{
		return Composite::removeChild(atom);
	}

	void Nucleotide::spliceBefore(Nucleotide& nucleotide)
		throw()
	{
		Composite::spliceBefore(nucleotide);
	}

	void Nucleotide::spliceAfter(Nucleotide& nucleotide)
		throw()
	{
		Composite::spliceAfter(nucleotide);
	}

	void Nucleotide::splice(Nucleotide& nucleotide)
		throw()
	{
		Composite::splice(nucleotide);
	}

	bool Nucleotide::isTerminal() const
		throw()
	{
		const NucleicAcid* parent = (*this).getNucleicAcid();
		if (parent != 0)
		{
			if (parent->get3Prime() == this ||
					parent->get5Prime() == this)
			{
				return true;
			}
		}
		return false;
	}

	bool Nucleotide::is5Prime() const
		throw()
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
		throw()
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
		throw()
	{ 
		return (Fragment::isValid() && id_.isValid());
	}

	void Nucleotide::dump(ostream& s, Size depth) const
		throw()
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
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void Nucleotide::write(ostream&  /*s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool Nucleotide::operator == (const Nucleotide& nucleotide) const
		throw()
	{
		return(Fragment::operator == (nucleotide) &&
					 id_ == nucleotide.id_ &&
					 insertion_code_ == nucleotide.insertion_code_);
	}

	bool Nucleotide::operator != (const Nucleotide& nucleotide) const
		throw()
	{
		return ! (*this == nucleotide);
	}

} // namespace BALL
