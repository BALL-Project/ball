// $Id: genericPDBFile.sip,v 1.1 2001/09/03 16:53:08 oliver Exp $

class GenericPDBFile
				: File, PropertyManager
{
%HeaderCode
	#include <BALL/FORMAT/genericPDBFile.h>
	using namespace BALL;
%End
	public:
	GenericPDBFile();
	~GenericPDBFile()
					throw();
	virtual void clear(int /state/ = 0);
	virtual float getVersion() const;
	void selectModel(Index /index/);
	void selectAllModels();
	Index getSelectedModel() const;
	Index getCurrentModel() const;
	char* getRecordString();
	const char* getRecordString() const;
	Index getRecordNumber() const;
	// PDB::RecordType getRecordType() const;
	//virtual const char* getAtomElementSymbol
	//        (const PDB::Atom /atom_name/,
	//         PDB::Atom /element_symbol/);
	//virtual const char* getAtomName
	//        (const PDB::Atom /atom_name/);
	//virtual char getAtomRemotenessIndicator
	//        (const PDB::Atom /atom_name/);
	//virtual short getAtomBranchDesignator
	//        (const PDB::Atom /atom_name/);
	//Size countRecordFields() const;
	//Size countRecord
	//        (PDB::RecordType /record_type/,
	//         bool /from_begin_of_file/ = true);
	//Size countRecords
	//        (bool /from_begin_of_file/ = true);
	// bool parseLine(const char* /line/, Size /size/, const char* format_string, ...);
	bool readLine(char* /line/, Size /size/, bool /extract_values/);
	bool readFirstRecord(bool /read_values/ = true);
	bool readNextRecord(bool /read_values/ = true);
	bool readRecords();
};
