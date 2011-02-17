// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: PDBInfo.C,v 1.3 2005/02/16 14:09:40 oliver Exp $
//

#include <BALL/FORMAT/PDBInfo.h>

namespace BALL 
{
	PDBInfo::PDBInfo() 
		:	skipped_records_(),
			invalid_records_(),
			filename_(""),
			id_(""),
			name_(""),
			number_of_records_(0),
			number_of_models_(0),
			model_read_(0),
			number_of_atom_records_(0),
			number_of_hetatm_records_(0)
	{
	}

	PDBInfo::PDBInfo(const PDBInfo& pdbi) 
		:	skipped_records_(pdbi.skipped_records_),
			invalid_records_(pdbi.invalid_records_),
			filename_(pdbi.filename_),
			id_(pdbi.id_),
			name_(pdbi.name_),
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
		name_ = pdbi.name_;
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
			&& (name_ == rhs.name_)
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

	PDBInfo::~PDBInfo() 
	{
		clear();
	}

	void PDBInfo::clear()
	{
		skipped_records_.clear();
		invalid_records_.clear();
		filename_.clear();
		id_.clear();
		name_.clear();
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

	void PDBInfo::persistentWrite(PersistenceManager& pm, const char* name) const
	{
		pm.writeObjectHeader(this,name);

		skipped_records_.persistentWrite(pm,"skipped_records_");
		invalid_records_.persistentWrite(pm, "invalid_records_");

		BALL_WRITE_PRIMITIVE_MEMBER(pm,filename_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,id_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,name_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,number_of_records_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,number_of_models_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,model_read_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,number_of_atom_records_);
		BALL_WRITE_PRIMITIVE_MEMBER(pm,number_of_hetatm_records_);

		pm.writeObjectTrailer(name);
	}

	void PDBInfo::persistentRead(PersistenceManager& pm)
	{
		pm.checkObjectHeader(skipped_records_,"skipped_records_");
		skipped_records_.persistentRead(pm);
		pm.checkObjectTrailer("skipped_records_");

		pm.checkObjectHeader(invalid_records_,"invalid_records_");
		invalid_records_.persistentRead(pm);
		pm.checkObjectTrailer("invalid_records_");

		BALL_READ_PRIMITIVE_MEMBER(pm,filename_);
		BALL_READ_PRIMITIVE_MEMBER(pm,id_);
		BALL_READ_PRIMITIVE_MEMBER(pm,name_);
		BALL_READ_PRIMITIVE_MEMBER(pm,number_of_records_);
		BALL_READ_PRIMITIVE_MEMBER(pm,number_of_models_);
		BALL_READ_PRIMITIVE_MEMBER(pm,model_read_);
		BALL_READ_PRIMITIVE_MEMBER(pm,number_of_atom_records_);
		BALL_READ_PRIMITIVE_MEMBER(pm,number_of_hetatm_records_);
	}


} // namespace BALL
