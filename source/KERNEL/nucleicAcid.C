// $Id: nucleicAcid.C,v 1.7 2001/01/14 21:57:16 amoll Exp $

#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/global.h>

using namespace std;

namespace BALL 
{

	NucleicAcid::NucleicAcid()
		throw()
		:	Molecule(),
			id_(BALL_NUCLEICACID_DEFAULT_ID)
	{
	}

	NucleicAcid::NucleicAcid(const NucleicAcid& nucleic_acid, bool deep)
		throw()
		:	Molecule(nucleic_acid, deep),
			id_(nucleic_acid.id_)
	{
	}
		
	NucleicAcid::NucleicAcid(const String& name,const String& id)
		throw()
		:	Molecule(name),
			id_(id)
	{
	}

	NucleicAcid::~NucleicAcid()
		throw()
	{
		destroy();
	}

	void NucleicAcid::clear()
		throw()
	{
		Molecule::clear();
		id_ = BALL_NUCLEICACID_DEFAULT_ID;
	}
		
	void NucleicAcid::destroy()
		throw()
	{
		Molecule::destroy();
		id_ = BALL_NUCLEICACID_DEFAULT_ID;
	}
		
	void NucleicAcid::persistentWrite(PersistenceManager& pm, const char* name) const
		throw()
	{
		pm.writeObjectHeader(this, name);
			Molecule::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
		pm.writeObjectTrailer(name);
	}

	void NucleicAcid::persistentRead(PersistenceManager& pm)
		throw()
	{
		pm.checkObjectHeader(RTTI::getStreamName<Molecule>());
			Molecule::persistentRead(pm);
		pm.checkObjectTrailer(0);
		pm.readPrimitive(id_, "id_");
	}

	void NucleicAcid::set(const NucleicAcid& nucleic_acid, bool deep)
		throw()
	{
		Molecule::set(nucleic_acid, deep);
		id_ = nucleic_acid.id_;
	}
			
	const NucleicAcid& NucleicAcid::operator =(const NucleicAcid &nucleic_acid)
		throw()
	{
		set(nucleic_acid);
		return *this;
	}

	void NucleicAcid::get(NucleicAcid &nucleic_acid, bool deep) const
		throw()
	{
		nucleic_acid.set(*this, deep);
	}
			
	void NucleicAcid::swap(NucleicAcid& nucleic_acid)
		throw()
	{
		Molecule::swap(nucleic_acid);
		id_.swap(nucleic_acid.id_);
	}

	Nucleotide* NucleicAcid::getNucleotide(Position position)
		throw()
	{
		for (NucleotideIterator Nucleotide_it = beginNucleotide(); !Nucleotide_it.isEnd(); ++Nucleotide_it)
		{
			if (position-- == 0)
			{
				return &(*Nucleotide_it);
			}
		}

		return 0;
	}

	const Nucleotide* NucleicAcid::getNucleotide(Position position) const
		throw()
	{
		return ((NucleicAcid *)this)->getNucleotide(position);
	}
		

	Nucleotide* NucleicAcid::get3Prime()
		throw()
	{
		return getNucleotide(0);
	}
		
	const Nucleotide* NucleicAcid::get3Prime() const
		throw()
	{
		return getNucleotide(0);
	}

	Nucleotide* NucleicAcid::get5Prime()
		throw()
	{
		return getNucleotide(countNucleotides()-1);
	}
		
	const Nucleotide* NucleicAcid::get5Prime() const
		throw()
	{
		return getNucleotide(countNucleotides()-1);
	}

	void NucleicAcid::setID(const String& id)
		throw()
	{
		id_ = id;
	}

	const String& NucleicAcid::getID() const
		throw()
	{
		return id_;
	}

	Size NucleicAcid::countNucleotides() const
		throw()
	{
		Size size = 0;

		for (NucleotideIterator res_it = beginNucleotide(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	bool NucleicAcid::isValid() const
		throw()
	{ 
		return (Molecule::isValid() && id_.isValid());
	}

	void	NucleicAcid::dump(ostream& s, Size depth) const
		throw()
	{
		BALL_DUMP_STREAM_PREFIX(s);
		
		Molecule::dump(s, depth);
		
		BALL_DUMP_DEPTH(s, depth);
		s << "  id: " << id_ << endl;

		BALL_DUMP_STREAM_SUFFIX(s);
	}

	void NucleicAcid::read(istream& /* s */)
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void NucleicAcid::write(ostream&  /* s */) const
		throw()
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	bool NucleicAcid::operator == (const NucleicAcid& nucleic_acid) const
		throw()
	{
		return(Molecule::operator == (nucleic_acid) &&
					 id_ == nucleic_acid.id_);
	}

	bool NucleicAcid::operator != (const NucleicAcid& nucleic_acid) const
		throw()
	{
		return ! (*this == nucleic_acid);
	}


} // namespace BALL
