class INIFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/INIFile.h>
%End
	public:

	enum
	{
		MAX_LINE_LENGTH = 1024
	};

	static const String UNDEFINED;
	static const String HEADER;

	INIFile();
	INIFile(const String&);
	~INIFile();
	void clear();

	bool read();
	bool write();
	const String& getFilename() const;
	void setFilename(const String&);
	bool isValid() const;
	Size getNumberOfLines() const;
	bool hasSection(const String&) const;
	Size getNumberOfSections() const;
	Size getSectionLength(const String&) const;
	bool deleteSection(const String&);
	bool appendSection(const String&);
	bool hasEntry(const String&, const String&) const;
	String getValue(const String&, const String&) const;
	bool insertValue(const String&, const String&, const String&);
	bool setValue(const String&, const String&, const String&);
	bool operator == (const INIFile&) const;
	void setDuplicateKeyCheck(bool);
};
