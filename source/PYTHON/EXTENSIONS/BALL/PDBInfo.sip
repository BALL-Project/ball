// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id$
//
	class PDBInfo
	{
%TypeHeaderCode
	#include <BALL/FORMAT/PDBInfo.h>
	using namespace BALL;
%End
		public:
		
		PDBInfo() throw();
		PDBInfo(const PDBInfo&) throw();
		virtual ~PDBInfo() throw();
		virtual void clear();

		// PDBInfo& operator = (const PDBInfo& rhs);
		bool operator == (const PDBInfo&) const;
		bool operator != (const PDBInfo&) const;

		PDBRecords& getSkippedRecords();
		// const PDBRecords& getSkippedRecords() const;

		PDBRecords& getInvalidRecords();
		// const PDBRecords& getInvalidRecords() const;

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
