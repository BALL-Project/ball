// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//

class ResourceEntry
	// : AutoDeletable
{
%TypeHeaderCode
	#include <BALL/FORMAT/resourceFile.h>
	using namespace BALL;
%End
  public:
  // BALL_CREATE(ResourceEntry)
  ResourceEntry();
  ResourceEntry(const ResourceEntry&, bool deep = true);
  ResourceEntry(const String&, const String&, ResourceEntry* parent = 0);
  ~ResourceEntry();
  void clear();
  void destroy();
  void set(const ResourceEntry&, bool deep = true);
  // ResourceEntry& operator = (const ResourceEntry&);
  void get(ResourceEntry&, bool deep = true) const;
  ResourceEntry& getRoot();
  // const ResourceEntry& getRoot() const;

  ResourceEntry* getParent();
  // const ResourceEntry* getParent() const;

  ResourceEntry* getChild(Position);
  //const ResourceEntry* getChild(Position) const;

  ResourceEntry* getEntry(const String&);
  // const ResourceEntry* getEntry(const String&) const;

  const String& getKey() const;
  void setValue(const String&);

  String& getValue();
  // const String& getValue() const;

  String getPath() const;
  Size countChildren() const;
  Size countDescendants() const;
  Size getSize() const;
  Size getDepth() const;

  ResourceEntry* insertChild(const String&, const String&, bool replace_value = true);
  ResourceEntry* insertSibling(const String&, const String&, bool replace_value = true);
  ResourceEntry* insertChild(ResourceEntry&, bool replace_value = true);
  ResourceEntry* insert(const String&, const String&);
  bool mergeChildrenOf(ResourceEntry&, bool replace_value = true);
  // bool removeChild(const String&, ResourceEntry** removed = 0);
  bool removeKey(const String&);
  ResourceEntry* findChild(const String&);
  // const ResourceEntry* findChild(const String&) const;

  ResourceEntry* findDescendant(const String&);
  // const ResourceEntry* findDescendant(const String&) const;

  ResourceEntry* findEntry(const String&);
  // const ResourceEntry* findEntry(const String&) const;
  bool hasChild(const String&) const;
  bool isEmpty() const;
  bool isParentOf(const ResourceEntry&) const;
  bool isChildOf(const ResourceEntry&) const;
  bool isAncestorOf(const ResourceEntry&) const;
  bool isDescendantOf(const ResourceEntry&) const;
  bool isRelatedWith(const ResourceEntry&) const;
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
%TypeHeaderCode
	#include <BALL/FORMAT/resourceFile.h>
	using namespace BALL;
%End
  public:
  static char ENTRY_BEGIN;
  static char ENTRY_END;
  static char SEPARATOR;
  typedef ResourceEntry Entry;
  ResourceFile();
  ResourceFile(const String&);
  ~ResourceFile();
  void destroy();
  bool open(const String&);
  void close();
  // static void saveAs(const Entry&, const String&);
  void saveAs(const String&);
  void save(const Entry&);
  void save();
  Size getSize() const;
  Entry& getRoot();
  // const Entry& getRoot() const;

  Entry* getEntry(const String&);
  // const Entry* getEntry(const String&) const;

  String* getValue(const String&);
  // const String* getValue(const String&) const;

  Entry* insert(const String&, const String&);
  bool removeKey(const String&);
  bool hasKey(const String&) const;
  bool isEmpty() const;
  bool isValid() const;
  // void dump(std::ostream& s = std::cout, Size depth = 0) const;
  // friend std::istream& operator >> (std::istream& s, ResourceFile& resource_file);
  // void host(Visitor<ResourceFile>& visitor);
  // bool apply(UnaryProcessor<Entry>& processor);
  // static bool applyChildren(Entry& entry, UnaryProcessor<Entry>& processor);
  // typedef Entry::Iterator Iterator;
	private:
	ResourceFile(const ResourceFile&);
};
  
