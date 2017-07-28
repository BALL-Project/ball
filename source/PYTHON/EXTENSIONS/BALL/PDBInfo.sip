class PDBInfo
{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBInfo.h>
%End
	public:

	PDBInfo() throw();
	PDBInfo(const PDBInfo&) throw();
	virtual ~PDBInfo() throw();
	virtual void clear();

	bool operator == (const PDBInfo&) const;
	bool operator != (const PDBInfo&) const;

	PDBRecords& getSkippedRecords();
	PDBRecords& getInvalidRecords();
	const String& getFilename() const;
	void setFilename(const String&);
	const String& getID() const;
	void setID(const String&);
	Size getNumberOfRecords() const;
	void setNumberOfRecords(Size);
	Size getNumberOfModels() const;
	void setNumberOfModels(Size);
	Position getCurrentModel() const;
	void setCurrentModel(Position);
	Size getNumberOfATOMRecords() const;
	void setNumberOfATOMRecords(Size);
	Size getNumberOfHETATMRecords() const;
	void setNumberOfHETATMRecords(Size);
	const String& getName() const;
	void setName(const String&);
	PyIndexList getRecordIndices(PDB::RecordType) const;
};
