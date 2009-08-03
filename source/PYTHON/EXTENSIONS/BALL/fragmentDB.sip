// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class FragmentDB 
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/fragmentDB.h>
	#include <BALL/STRUCTURE/reconstructFragmentProcessor.h>
	using namespace BALL;
	typedef UnaryProcessor<Fragment> FragmentProcessor;
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

  // BALL_CREATE(FragmentDB)


  FragmentDB();
  FragmentDB(const String& /filename/);
  FragmentDB(const FragmentDB& /db/, bool /deep/ = true); 
  ~FragmentDB();
  void destroy();
  void init();
  void setFilename(const String& /filename/);
  const String& getFilename() const;
  bool has(const String& /fragment_name/) const;
  FragmentDB::Type getFragmentType(const String& /fragment_name/) const;
	// BAUSTELLE
  // list<String> getVariantNames(const String& /name/) const;
  const Fragment* getFragment(const String& /fragment_name/) const;
  const Fragment* getReferenceFragment(const Fragment& /fragment/) const;
  const Residue* getResidue(const String& /fragment_name/) const;
  Fragment* getFragmentCopy(const String& /fragment_name/) const;
  Molecule* getMoleculeCopy(const String& /fragment_name/) const;
  Residue* getResidueCopy(const String& /fragment_name/) const;
	const StringHashMap<String>& getNamingStandard(const String& /std/) const
		throw(IllegalKey);
	vector<String> getAvailableNamingStandards() const;
  const String& getDefaultNamingStandard() const;
	// BAUSTELLE
  // StringHashMap<StringHashMap<String>*>& getNamingStandards();
  bool isValid() const;

  NormalizeNamesProcessor 			normalize_names;
	ReconstructFragmentProcessor  add_hydrogens;
  BuildBondsProcessor   				build_bonds;
  ResourceEntry* tree;
};


//class NoFragmentNode
// : GeneralException
//{
//%TypeHeaderCode
//	#using <BALL/STRUCTURE/fragmentDB.h>
//	using namespace BALL;
//%End
// public:
// NoFragmentNode(const char* /file/, int /line/, const String& /filename/);
//};

class NormalizeNamesProcessor 
//  : public UnaryProcessor<Fragment>
	: FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/fragmentDB.h>
	using namespace BALL;
	typedef FragmentDB::NormalizeNamesProcessor NormalizeNamesProcessor;
	typedef UnaryProcessor<Fragment> FragmentProcessor;
%End
	public:
	NormalizeNamesProcessor();
	NormalizeNamesProcessor(FragmentDB& /db/);
	~NormalizeNamesProcessor();
	void setFragmentDB(FragmentDB& /db/);
	void setNamingStandard(const String& /naming_standard/);
	const String& getNamingStandard();
	// BAUSTELLE
	// bool matchName(String&  /res_name/, String&  /atom_name/, const StringHashMap<String>* /map/) const;
	virtual bool start();
	virtual bool finish();
	Processor::Result operator () (Fragment& /fragment/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
};


class BuildBondsProcessor 
// : public UnaryProcessor<Fragment> 
	: FragmentProcessor
{
%TypeHeaderCode
	#include <BALL/STRUCTURE/fragmentDB.h>
	using namespace BALL;
	typedef FragmentDB::BuildBondsProcessor BuildBondsProcessor;
	typedef UnaryProcessor<Fragment> FragmentProcessor;
%End
	public:
	BuildBondsProcessor();
	BuildBondsProcessor(const FragmentDB& /db/);
	~BuildBondsProcessor();
	virtual bool finish();
	virtual bool start();
	Processor::Result operator () (Fragment& /fragment/);
%MethodCode
  sipRes = sipCpp->operator () (*a0);
%End
	Size getNumberOfBondsBuilt();
	void setFragmentDB(const FragmentDB& /fragment_db/);
	Size buildFragmentBonds(Fragment& /fragment/) const;
	Size buildInterFragmentBonds(Fragment& /first/, Fragment& /second/) const;
};

