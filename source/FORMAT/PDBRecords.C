// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBRecords.C,v 1.1 2005/02/08 17:32:37 oliver Exp $
//

#include <BALL/FORMAT/PDBRecords.h>

namespace BALL 
{

	PDBRecords::PDBRecords() 
		:	records_(500)
	{
	}

	PDBRecords::PDBRecords(const PDBRecords& pdbr) 
		:	records_(pdbr.records_)
	{
	}

	PDBRecords::~PDBRecords() 
	{
		records_.clear();
	}

	PDBRecords& PDBRecords::operator = (const PDBRecords& rhs)
	{
		if (this != &rhs)
		{
			records_ = rhs.records_;
		}
		return *this;
	}

	void PDBRecords::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this, name);
		pm.writePrimitive((Size)records_.size(), "records_.size");
		for (Position record = 0; record < records_.size(); record++)
		{
			String record_identifier = "records_["+String(record)+"]";
			pm.writePrimitive(records_[record], record_identifier.c_str());
		}
		pm.writeObjectTrailer(name);
	}

	void PDBRecords::persistentRead(PersistenceManager& pm)
	{
		Position record_size;
		pm.readPrimitive(record_size,"records_.size");
		records_.resize(record_size);

		for(Position record = 0; record < record_size; record++)
		{
			String record_identifier = "records_["+String(record)+"]";
			String record_content;
			pm.readPrimitive(record_content, record_identifier.c_str());
			records_[record] = record_content;
		}
	}



} // namespace BALL
