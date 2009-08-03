struct Data
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
	using namespace BALL;
	typedef CosineTorsion::Data Data;
%End
};

struct SingleData
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
	using namespace BALL;
	typedef CosineTorsion::SingleData SingleData;
%End
};

struct SingleValues
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
	using namespace BALL;
	typedef CosineTorsion::SingleValues SingleValues;
%End
	SingleValues(const SingleValues& v);
	SingleValues();

	bool operator == (const SingleValues& rhs);
};

struct Values
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
	using namespace BALL;
	typedef CosineTorsion::Values Values;
%End
	Values();
	Values(Size number);
	Values(const Values& v);
	void set(const Values& v);
	~Values();
	bool operator == (const Values& rhs) const;
	bool operator != (const Values& rhs) const;
};

class CosineTorsion
	: ParameterSection
{
%TypeHeaderCode
	#include <BALL/MOLMEC/PARAMETER/cosineTorsion.h>
	using namespace BALL;
%End
	public:

	CosineTorsion();
	CosineTorsion(const CosineTorsion& cosine_torsion);
	virtual ~CosineTorsion();

	virtual void clear();
	virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);
	virtual bool extractSection(Parameters& parameters, const String& section_name);
	bool hasParameters(AtomType I, AtomType J, AtomType K, AtomType L) const;
	Values getParameters(AtomType I, AtomType J, AtomType K, AtomType L) const;
	bool assignParameters(Values& parameters, AtomType I,
												AtomType J, AtomType K, AtomType L) const;

	bool operator == (const CosineTorsion& cosine_torsion) const;
};

