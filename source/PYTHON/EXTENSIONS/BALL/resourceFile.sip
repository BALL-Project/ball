class ResourceEntry
{
%TypeHeaderCode
	#include <BALL/FORMAT/resourceFile.h>
%End
	public:
	ResourceEntry();
	ResourceEntry(const ResourceEntry&, bool deep = true);
	ResourceEntry(const String&, const String&, ResourceEntry* parent = 0);
	~ResourceEntry();
	void clear();
	void destroy();
	void set(const ResourceEntry&, bool deep = true);
	void get(ResourceEntry&, bool deep = true) const;
	ResourceEntry& getRoot();
	ResourceEntry* getParent();
	ResourceEntry* getChild(Position);
	ResourceEntry* getEntry(const String&);
	const String& getKey() const;
	void setValue(const String&);
	String& getValue();
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
	bool removeKey(const String&);
	ResourceEntry* findChild(const String&);
	ResourceEntry* findDescendant(const String&);
	ResourceEntry* findEntry(const String&);
	bool hasChild(const String&) const;
	bool isEmpty() const;
	bool isParentOf(const ResourceEntry&) const;
	bool isChildOf(const ResourceEntry&) const;
	bool isAncestorOf(const ResourceEntry&) const;
	bool isDescendantOf(const ResourceEntry&) const;
	bool isRelatedWith(const ResourceEntry&) const;
	bool isRoot() const;
	bool isValid() const;
};

class ResourceFile
	: File
{
%TypeHeaderCode
	#include <BALL/FORMAT/resourceFile.h>
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
	void saveAs(const String&);
	void save(const Entry&);
	void save();
	Size getSize() const;
	Entry& getRoot();
	Entry* getEntry(const String&);
	String* getValue(const String&);
	Entry* insert(const String&, const String&);
	bool removeKey(const String&);
	bool hasKey(const String&) const;
	bool isEmpty() const;
	bool isValid() const;

	private:
	ResourceFile(const ResourceFile&);
};
