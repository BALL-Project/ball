# 1 "nucleotide.h"
namespace BALL 
{
 class NucleicAcid;
 class Protein;
 class Nucleotide
  : public Fragment
 {
  public:
  BALL_CREATE(Nucleotide)
  enum Property
  {
   PROPERTY__5_PRIME Fragment::NUMBER_OF_PROPERTIES + 1,
   PROPERTY__3_PRIME,
   NUMBER_OF_PROPERTIES
  };
  Nucleotide();
  Nucleotide(const Nucleotide& nucleotide, bool deep = true);
  Nucleotide
   (const String& name,
    const String& id = "" ,
    char insertion_code = ' ' );
  virtual ~Nucleotide();
  virtual void clear();
  virtual void destroy();
  void persistentWrite(PersistenceManager& pm, const char* name = 0) const;
  void persistentRead(PersistenceManager& pm);
  void set(const Nucleotide& nucleotide, bool deep = true);
  Nucleotide &operator = (const Nucleotide& nucleotide);
  void get(Nucleotide& nucleotide, bool deep = true) const;
  void swap(Nucleotide& nucleotide);
  Protein* getProtein();
  const Protein* getProtein() const;
  NucleicAcid* getNucleicAcid();
  const NucleicAcid* getNucleicAcid() const;
  void setID(const String& id);
  String& getID();
  const String& getID() const;
  void setInsertionCode(char insertion_code);
  char getInsertionCode() const;
  void prepend(Atom& atom);
  void append(Atom& atom);
  void insert(Atom& atom);
  void insertBefore(Atom& atom, Composite& before);
  void insertAfter(Atom& atom, Composite& after);
  bool remove(Atom& atom);
  void spliceBefore(Nucleotide& nucleotide);
  void spliceAfter(Nucleotide& nucleotide);
  void splice(Nucleotide& nucleotide);
  bool isTerminal() const;
  bool is3Prime() const;
  bool is5Prime() const;
  virtual bool isValid() const;
  virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
  virtual void read(istream& s);
  virtual void write(ostream& s) const;
  private:
  BaseFragment* getBaseFragment(Index index);
  const BaseFragment* getBaseFragment(Index index) const;
  Size countBaseFragments() const;
  void prepend(BaseFragment& base_fragment);
  void append(BaseFragment& base_fragment);
  void insert(BaseFragment& base_fragment);
  void insertBefore(BaseFragment& base_fragment, Composite& composite);
  void insertAfter(BaseFragment& base_fragment, Composite& composite);
  void spliceBefore(BaseFragment& base_fragment);
  void spliceAfter(BaseFragment& base_ragment);
  void splice(BaseFragment& BaseFragment);
  bool remove(BaseFragment& BaseFragment);
  bool isSuperBaseFragmentOf(const BaseFragment& base_fragment) const;
  BALL_KERNEL_DEFINE_ITERATOR_CREATORS(BaseFragment)
  void clear_();
  String  id_;
  char   insertion_code_;
 };
  template <class NucleotideContainerType>
  const Nucleotide* get5Prime(const NucleotideContainerType& nucleotide_container)
  {
  NucleotideConstIterator res_it;
    for ( res_it = nucleotide_container.beginNucleotide(); !res_it.isEnd(); ++res_it)
   return &(*res_it);
    return 0;
  }
  template <class NucleotideContainerType>
  const Nucleotide* get3Prime(const NucleotideContainerType& nucleotide_container)
  {
    for (NucleotideConstIterator res_it = nucleotide_container.rbeginNucleotide(); !res_it.isREnd(); ++res_it)
   return &(*res_it);
    return 0;
  }
}  
