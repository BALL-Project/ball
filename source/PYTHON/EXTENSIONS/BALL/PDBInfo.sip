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
		PDBInfo(const PDBInfo& /pdbi/) throw();
		virtual ~PDBInfo() throw();
		virtual void clear();

		// PDBInfo& operator = (const PDBInfo& rhs);
		bool operator == (const PDBInfo& /rhs/) const;
		bool operator != (const PDBInfo& /rhs/) const;

		PDBRecords& getSkippedRecords();
		// const PDBRecords& getSkippedRecords() const;

		PDBRecords& getInvalidRecords();
		// const PDBRecords& getInvalidRecords() const;

		const String& getFilename() const;
		void setFilename(const String& /filename/);
		const String& getID() const;
		void setID(const String& /id/);
		Size getNumberOfRecords() const;
		void setNumberOfRecords(Size /n/);
		Size getNumberOfModels() const;
		void setNumberOfModels(Size /n/);
		Position getCurrentModel() const;
		void setCurrentModel(Position /n/);
		Size getNumberOfATOMRecords() const;
		void setNumberOfATOMRecords(Size /n/);
		Size getNumberOfHETATMRecords() const;
		void setNumberOfHETATMRecords(Size /n/);
		const String& getName() const;
		void setName(const String& /name/);
		PyIndexList getRecordIndices(PDB::RecordType /record/) const;
	};
