class ResourceEntry
	// : AutoDeletable
{
%HeaderCode
	#include <BALL/FORMAT/resourceFile.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(ResourceEntry)
  ResourceEntry();
  ResourceEntry(const ResourceEntry& /entry/, bool /deep/ = true);
  ResourceEntry(const String& /key/, const String& /value/, ResourceEntry* /parent/ = 0);
  ~ResourceEntry();
  void clear();
  void destroy();
  void set(const ResourceEntry& /entry/, bool /deep/ = true);
  // ResourceEntry& operator = (const ResourceEntry& /entry/);
  void get(ResourceEntry& /entry/, bool /deep/ = true) const;
  ResourceEntry& getRoot();
  const ResourceEntry& getRoot() const;
  ResourceEntry* getParent();
  const ResourceEntry* getParent() const;
  ResourceEntry* getChild(Position /index/);
  const ResourceEntry* getChild(Position /index/) const;
  ResourceEntry* getEntry(const String& /key_path/);
  const ResourceEntry* getEntry(const String& /key_path/) const;
  const String& getKey() const;
  void setValue(const String& /value/);
  String& getValue();
  const String& getValue() const;
  String getPath() const;
  Size countChildren() const;
  Size countDescendants() const;
  Size getSize() const;
  Size getDepth() const;
  ResourceEntry* insertChild(const String& /key/, const String& /value/, bool /replace_value/ = true);
  ResourceEntry* insertSibling(const String& /key/, const String& /value/, bool /replace_value/ = true);
  ResourceEntry* insertChild(ResourceEntry& /entry/, bool /replace_value/ = true);
  ResourceEntry* insert(const String& /key_path/, const String& /name/);
  bool mergeChildrenOf(ResourceEntry& /entry/, bool /replace_value/ = true);
  // bool removeChild(const String& /key/, ResourceEntry** /removed/ = 0);
  bool removeKey(const String& /key_path/);
  ResourceEntry* findChild(const String& /key/);
  const ResourceEntry* findChild(const String& /key/) const;
  ResourceEntry* findDescendant(const String& /key/);
  const ResourceEntry* findDescendant(const String& /key/) const;
  ResourceEntry* findEntry(const String& /key/);
  const ResourceEntry* findEntry(const String& /key/) const;
  bool hasChild(const String& /key/) const;
  bool isEmpty() const;
  bool isParentOf(const ResourceEntry& /entry/) const;
  bool isChildOf(const ResourceEntry& /entry/) const;
  bool isAncestorOf(const ResourceEntry& /entry/) const;
  bool isDescendantOf(const ResourceEntry& /entry/) const;
  bool isRelatedWith(const ResourceEntry& /entry/) const;
  bool isRoot() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // void host(Visitor<ResourceEntry>& visitor);
  // bool apply(UnaryProcessor<ResourceEntry>& processor);
  // bool applyChildren(UnaryProcessor<ResourceEntry>& processor);
  // typedef ResourceEntry* IteratorPosition;
};


class ResourceFile
 :  File
{
%HeaderCode
	#include <BALL/FORMAT/resourceFile.h>
	using namespace BALL;
%End
  public:
  static char ENTRY_BEGIN;
  static char ENTRY_END;
  static char SEPARATOR;
  typedef ResourceEntry Entry;
  ResourceFile();
  ResourceFile(const String& /name/);
	ResourceFile(const ResourceFile& /resource_file/);
  ~ResourceFile();
  void destroy();
  bool open(const String& /name/);
  void close();
  // static void saveAs(const Entry& /entry/, const String& /name/);
  void saveAs(const String& /name/);
  void save(const Entry& /entry/);
  void save();
  Size getSize() const;
  Entry& getRoot();
  // const Entry& getRoot() const;
  Entry* getEntry(const String& /key_path/);
  const Entry* getEntry(const String& /key_path/) const;
  String* getValue(const String& /key_path/);
  const String* getValue(const String& /key_path/) const;
  Entry* insert(const String& /key_path/, const String& /name/);
  bool removeKey(const String& /key_path/);
  bool hasKey(const String& /key_path/) const;
  bool isEmpty() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // friend std::istream& operator >> (std::istream& s, ResourceFile& resource_file);
  // void host(Visitor<ResourceFile>& visitor);
  // bool apply(UnaryProcessor<Entry>& processor);
  // static bool applyChildren(Entry& entry, UnaryProcessor<Entry>& processor);
  typedef Entry::Iterator Iterator;
};
  
