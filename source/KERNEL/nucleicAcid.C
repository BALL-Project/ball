// $Id: nucleicAcid.C,v 1.5 2000/12/11 21:14:49 oliver Exp $

#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/global.h>

using namespace std;

namespace BALL 
{

	NucleicAcid::NucleicAcid()
		:	Molecule(),
			id_(BALL_NUCLEICACID_DEFAULT_ID)
	{
	}

	NucleicAcid::NucleicAcid(const NucleicAcid& nucleic_acid, bool deep)
		:	Molecule(nucleic_acid, deep),
			id_(nucleic_acid.id_)
	{
	}
		
	NucleicAcid::NucleicAcid(const String& name,const String& id)
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

		clear_();
	}
		
	void NucleicAcid::destroy()
		throw()
	{
		Molecule::destroy();

		clear_();
	}
		
	void NucleicAcid::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
			Molecule::persistentWrite(pm);
			pm.writePrimitive(id_, "id_");
		pm.writeObjectTrailer(name);
	}

	void NucleicAcid::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(RTTI::getStreamName<Molecule>());
			Molecule::persistentRead(pm);
		pm.checkObjectTrailer(0);
		pm.readPrimitive(id_, "id_");
	}


	void NucleicAcid::set(const NucleicAcid& nucleic_acid, bool deep)
	{
		Molecule::set(nucleic_acid, deep);

		id_ = nucleic_acid.id_;
	}
			
	NucleicAcid& NucleicAcid::operator =(const NucleicAcid &nucleic_acid)
	{
		set(nucleic_acid);

		return *this;
	}

	void NucleicAcid::get(NucleicAcid &nucleic_acid, bool deep) const
	{
		nucleic_acid.set(*this, deep);
	}
			
	void NucleicAcid::swap(NucleicAcid& nucleic_acid)
	{
		Molecule::swap(nucleic_acid);

		id_.swap(nucleic_acid.id_);
	}

	Nucleotide* NucleicAcid::getNucleotide(Position position)
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
	{
		return ((NucleicAcid *)this)->getNucleotide(position);
	}
		

	Nucleotide* NucleicAcid::get3Prime()
	{
		return getNucleotide(0);
	}
		
	const Nucleotide* NucleicAcid::get3Prime() const
	{
		return getNucleotide(0);
	}

	Nucleotide* NucleicAcid::get5Prime()
	{
		return getNucleotide(countNucleotides()-1);
	}
		
	const Nucleotide* NucleicAcid::get5Prime() const
	{
		return getNucleotide(countNucleotides()-1);
	}

	void NucleicAcid::setID(const String& id)
	{
		id_ = id;
	}

	const String& NucleicAcid::getID() const
	{
		return id_;
	}

	Size NucleicAcid::countNucleotides() const
	{
		Size size = 0;

		for (NucleotideIterator res_it = beginNucleotide(); !res_it.isEnd(); ++res_it)
		{
			++size;
		}

		return size;
	}

	bool NucleicAcid::isValid() const
	{ 
		if (Molecule::isValid() == false
				|| id_.isValid() == false)
		{
			return false;
		}

		return true;
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
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void NucleicAcid::write(ostream&  /* s */) const
	{
		throw Exception::NotImplemented(__FILE__, __LINE__);
	}

	void NucleicAcid::clear_()
	{
		id_ = BALL_NUCLEICACID_DEFAULT_ID;
	}

} // namespace BALL
