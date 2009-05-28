class AtomTypes
	: ParameterSection
{
%TypeHeaderCode
	#include <BALL/KERNEL/atom.h>
	#include <BALL/MOLMEC/PARAMETER/atomTypes.h>
	using namespace BALL;
%End
	public:

	AtomTypes();
	AtomTypes(const AtomTypes& atom_types);
	virtual ~AtomTypes();

	virtual bool extractSection(Parameters& parameters,
			const String& section_name);
	AtomType getType(const String& name) const;
	bool hasType(const String& name) const;
	String getTypeName(AtomType type) const;
	Size getNumberOfTypes() const;
	virtual void clear();

	bool operator == (const AtomTypes& atom_types) const;
};

