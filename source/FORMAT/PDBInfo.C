// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBInfo.C,v 1.2 2005/02/13 22:38:49 oliver Exp $
//

#include <BALL/FORMAT/PDBInfo.h>

namespace BALL 
{
	PDBInfo::PDBInfo() throw()
		:	skipped_records_(),
			invalid_records_(),
			filename_(""),
			id_(""),
			number_of_records_(0),
			number_of_models_(0),
			model_read_(0),
			number_of_atom_records_(0),
			number_of_hetatm_records_(0)
	{
	}

	PDBInfo::PDBInfo(const PDBInfo& pdbi) throw()
		:	skipped_records_(pdbi.skipped_records_),
			invalid_records_(pdbi.invalid_records_),
			filename_(pdbi.filename_),
			id_(pdbi.id_),
			number_of_records_(pdbi.number_of_records_),
			number_of_models_(pdbi.number_of_models_),
			model_read_(pdbi.model_read_),
			number_of_atom_records_(pdbi.number_of_atom_records_),
			number_of_hetatm_records_(pdbi.number_of_hetatm_records_)
	{
	}
	
	PDBInfo& PDBInfo::operator = (const PDBInfo& pdbi)
	{
		skipped_records_ = pdbi.skipped_records_;
		invalid_records_ = pdbi.invalid_records_;
		filename_ = pdbi.filename_;
		id_ = pdbi.id_;
		number_of_records_ = pdbi.number_of_records_;
		number_of_models_ = pdbi.number_of_models_;
		model_read_ = pdbi.model_read_;
		number_of_atom_records_ = pdbi.number_of_atom_records_;
		number_of_hetatm_records_ = pdbi.number_of_hetatm_records_;
		return *this;
	}

	bool PDBInfo::operator == (const PDBInfo& rhs) const
	{
		return 
			(skipped_records_ == rhs.skipped_records_)
			&& (invalid_records_ == rhs.invalid_records_)
			&& (filename_ == rhs.filename_)
			&& (id_ == rhs.id_)
			&& (number_of_records_ == rhs.number_of_records_)
			&& (number_of_models_ == rhs.number_of_models_)
			&& (model_read_ == rhs.model_read_)
			&& (number_of_atom_records_ == rhs.number_of_atom_records_)
			&& (number_of_hetatm_records_ == rhs.number_of_hetatm_records_);
	}
 
	bool PDBInfo::operator != (const PDBInfo& rhs) const
	{
		return ! operator == (rhs);
	}

	PDBInfo::~PDBInfo() throw()
	{
		clear();
	}

	void PDBInfo::clear()
	{
		skipped_records_.clear();
		invalid_records_.clear();
		filename_.clear();
		id_.clear();
		number_of_records_ = 0;
		number_of_models_ = 0;
		model_read_ = 0;
		number_of_atom_records_ = 0;
		number_of_hetatm_records_ = 0;
	}

	std::list<Position> PDBInfo::getRecordIndices(PDB::RecordType record) const
	{
		std::list<Position> indices;

		String tag(PDB::RECORD_TYPE_FORMAT[record].string);
		Position i = 0;
		for (PDBRecords::ConstIterator it(skipped_records_.begin()); it != skipped_records_.end(); ++it, ++i)
		{
			if (it->hasPrefix(tag))
			{
				indices.push_back(i);
			}
		}
		
		return indices;
	}

} // namespace BALL
