// $Id: fragmentDB.sip,v 1.1 2000/07/18 21:27:37 oliver Exp $

class FragmentDB 
{
  public:
  class AddHydrogensProcessor;
  // friend class FragmentDB::AddHydrogensProcessor;
  enum FragmentTypes 
  {
   TYPE__UNKNOWN = -1,
   TYPE__FRAGMENT,
   TYPE__RESIDUE,
   TYPE__MOLECULE
  };
  typedef short Type;
  // BALL_CREATE(FragmentDB)

  class NoFragmentNode
   : public Exception::GeneralException
  {
   public:
   NoFragmentNode(const char* file, int line, const string& filename);
  };

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
  list<String> getVariantNames(const String& /name/) const;
  const Fragment* getFragment(const String& /fragment_name/) const;
  const Fragment* getReferenceFragment(const Fragment& /fragment/) const;
  const Residue* getResidue(const String& /fragment_name/) const;
  Fragment* getFragmentCopy(const String& /fragment_name/) const;
  Molecule* getMoleculeCopy(const String& /fragment_name/) const;
  Residue* getResidueCopy(const String& /fragment_name/) const;
  const String& getDefaultNamingStandard() const;
  StringHashMap<StringHashMap<String>*>& getNamingStandards();
  bool isValid() const;
  class NormalizeNamesProcessor 
  //  : public UnaryProcessor<Fragment>
		: FragmentProcessor
  {
   public:
   NormalizeNamesProcessor();
   NormalizeNamesProcessor(FragmentDB& /db/);
   virtual ~NormalizeNamesProcessor();
   void setFragmentDB(FragmentDB& /db/);
   void setNamingStandard(const String& /naming_standard/);
   const String& getNamingStandard();
   bool matchName(String&  /res_name/, String&  /atom_name/, const StringHashMap<String>* /map/) const;
   virtual bool start();
   virtual bool finish();
   virtual Processor::Result operator () (Fragment& /fragment/);
  };
  class AddHydrogensProcessor 
  //  : public UnaryProcessor<Fragment> 
		: FragmentProcessor
  {
   public:
   AddHydrogensProcessor();
   AddHydrogensProcessor(const FragmentDB& /db/);
   ~AddHydrogensProcessor();
   virtual bool start();
   virtual bool finish();
   virtual Processor::Result operator()(Fragment&);
   Size getNumberOfInsertedH();
   void setFragmentDB(const FragmentDB& /fragment_db/);
  };

  class BuildBondsProcessor 
  // : public UnaryProcessor<Fragment> 
		: FragmentProcessor
  {
   public:
   BuildBondsProcessor();
   BuildBondsProcessor(const FragmentDB& /db/);
   ~BuildBondsProcessor();
   virtual bool finish();
   virtual bool start();
   virtual Processor::Result operator () (Fragment& /fragment/);
   Size getNumberOfBondsBuilt();
   void setFragmentDB(const FragmentDB& /fragment_db/);
   Size buildFragmentBonds(Fragment& /fragment/) const;
   Size buildInterFragmentBonds(Fragment& /first/, Fragment& /second/) const;
  };

  NormalizeNamesProcessor  normalize_names;
  AddHydrogensProcessor   add_hydrogens;
  BuildBondsProcessor    build_bonds;
  ResourceEntry* tree;
};
