class FragmentDB
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/fragmentDB.h>
	#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
	using StringHashMapString = StringHashMap<String>;
%End

	public:

	enum FragmentTypes
	{
		TYPE__UNKNOWN = -1,
		TYPE__FRAGMENT,
		TYPE__RESIDUE,
		TYPE__MOLECULE
	};
	typedef short Type;

	FragmentDB();
	FragmentDB(const String&);
	FragmentDB(const FragmentDB&, bool deep = true);
	~FragmentDB();
	void destroy();
	void init();
	void setFilename(const String&);
	const String& getFilename() const;
	bool has(const String&) const;
	FragmentDB::Type getFragmentType(const String&) const;

	const Fragment* getFragment(const String&) const;
	const Fragment* getReferenceFragment(const Fragment&) const;
	const Residue* getResidue(const String&) const;
	Fragment* getFragmentCopy(const String&) const;
	Molecule* getMoleculeCopy(const String&) const;
	Residue* getResidueCopy(const String&) const;
	const StringHashMap<String>& getNamingStandard(const String&) const
		throw(StringHashMapString::IllegalKey);
	vector<String> getAvailableNamingStandards() const;
	const String& getDefaultNamingStandard() const;
	bool isValid() const;

	NormalizeNamesProcessor normalize_names;
	ReconstructFragmentProcessor  add_hydrogens;
	BuildBondsProcessor build_bonds;
	ResourceEntry* tree;
};

class NormalizeNamesProcessor
	: FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/fragmentDB.h>
	using NormalizeNamesProcessor = FragmentDB::NormalizeNamesProcessor;
%End
	public:
	NormalizeNamesProcessor();
	NormalizeNamesProcessor(FragmentDB&);
	~NormalizeNamesProcessor();
	void setFragmentDB(FragmentDB&);
	void setNamingStandard(const String&);
	const String& getNamingStandard();
	virtual bool start();
	virtual bool finish();
	Processor::Result operator () (Fragment&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
};

class BuildBondsProcessor
	: FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/PYTHON/pyUnaryProcessors.h>
	#include <BALL/STRUCTURE/fragmentDB.h>
	using BuildBondsProcessor = FragmentDB::BuildBondsProcessor;
%End
	public:
	BuildBondsProcessor();
	BuildBondsProcessor(const FragmentDB&);
	~BuildBondsProcessor();
	virtual bool finish();
	virtual bool start();
	Processor::Result operator () (Fragment&);
%MethodCode
	sipRes = sipCpp->operator () (*a0);
%End
	Size getNumberOfBondsBuilt();
	void setFragmentDB(const FragmentDB&);
	Size buildFragmentBonds(Fragment&) const;
	Size buildInterFragmentBonds(Fragment&, Fragment&) const;
};
