#include <BALL/FORMAT/NMRStarFile.h>
#include <iostream> 
#include <fstream>

using namespace std;

namespace BALL
{

	ostream& operator << (ostream &s, const NMRAtomData& ad)
	{
		s << "atom_ID: "					  << ad.atom_ID;
		s << " residue_seq_code: "  << ad.residue_seq_code;
		s << " residue_label: "		  << ad.residue_label;
		s << " atom_name: "					<< ad.atom_name;
		s << " shift_value: "				<< ad.shift_value;
		s << " error_value: "				<< ad.error_value;
		s << " ambiguity_code: "		<< ad.ambiguity_code << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const SampleCondition& sc)
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name "				<< sc.name << endl;;
		s << "temperature "	<< sc.temperature << endl;;
		s << "pH "					<< sc.pH << endl;;
		s << "pressure "		<< sc.pressure << endl;;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const ShiftReferenceElement& sre)
	{
		s << endl;
		s << "mol_common_name "		<< sre.mol_common_name << endl;
		s << "atom_type "					<< sre.atom_type << endl;
		s << "isotope_number "		<< sre.isotope_number << endl;
		s << "atom_group "				<< sre.atom_group << endl;
		s << "shift_units "				<< sre.shift_units << endl;
		s << "shift_value "				<< sre.shift_value << endl;
		s << "reference_method ";
		switch (sre.reference_method)
		{
		case NMRStarFile::INTERNAL_REFERENCE:	cout << "internal";	break;
		case NMRStarFile::EXTERNAL_REFERENCE:	cout << "external";	break;
		case NMRStarFile::UNKNOWN_REFERENCE:	cout << "unknown";	break;
		case NMRStarFile::UNSET_REFERENCE:		cout << "unset";		break;
		default:	cout << "?";
		}
		s << endl << "reference_type ";
		switch (sre.reference_type)
		{
			case NMRStarFile::DIRECT_TYPE:		cout << "direct";		break;
			case NMRStarFile::INDIRECT_TYPE:	cout << "indirect";	break;
			case NMRStarFile::UNKNOWN_TYPE:		cout << "unknown";	break;
			case NMRStarFile::UNSET_TYPE:			cout << "unset";		break;
			default:	cout << "?";
		}
		s << endl << "indirect_shift_ratio " <<  sre.indirect_shift_ratio << endl << endl;
		return s;
	}
	
	ostream& operator << (::std::ostream& s, const ShiftReferenceSet& sr)
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name " << sr.name << endl << endl;
		for (Position pos = 0; pos < sr.elements.size() ; pos++ )
		{
			s << *(sr.elements[pos]);
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const NMRAtomDataSet& set)
	{
		s << endl << "name " << set.name << endl << endl;

		for (Position pos = 0; pos < set.atom_data.size() ; pos++)
		{
			if (set.atom_data[pos] == 0)
			{
				s << "0 - pointer for atomDataItem" << endl;			
			}
			else
			{
				s << *set.atom_data[pos];
			}
		}
		
		s << endl;

		if (set.condition == 0)
		{
			s << "0 - pointer for condition" << endl;
		}
		else
		{
			s << *set.condition;
		}
		if (set.reference == 0)
		{
			s << "0 - pointer for reference" << endl;
		}
		else
		{
			s << *set.reference;
		}
		s << endl;
		return s;
	}

	ShiftReferenceElement::ShiftReferenceElement()
		: atom_type(),
			isotope_number(0),
			atom_group(),
			shift_units(),
			shift_value(),
			reference_method(NMRStarFile::UNSET_REFERENCE),
		  reference_type	(NMRStarFile::UNSET_TYPE),
			indirect_shift_ratio()
	{
	}

	SampleCondition::SampleCondition()
		: name(),
			temperature(),
			pH(),
			pressure()
	{
	}

	NMRAtomDataSet::NMRAtomDataSet()
		: name(),
			condition(0),
			reference(0)
	{
	}

	std::vector<String> NMRStarFile::reference_options_;

	Size NMRStarFile::getNumberOfAtoms() const
	{
		Size max = 0;
		for (Position pos = 0;  pos < atom_data_sets_.size(); pos++)
		{
			if (atom_data_sets_[pos]->atom_data.size() > max)
			{
				 max = atom_data_sets_[pos]->atom_data.size();
			}
		}

		return max;
	}

	const std::vector<NMRAtomDataSet*>& NMRStarFile::getData() const
	{
		return atom_data_sets_;
	}

	void NMRStarFile::readEntryInformation_()
	{
		try
		{
			rewind_();
			test_(__FILE__, __LINE__, 
				search_("       assigned_chemical_shifts"),
				"Number of assigned chemical shifts could not be found");

			Position _position_ = 31;
			number_of_shifts_ = getToken_(_position_).toUnsignedInt();
		}
		catch (ReadFile::ReadFileError)
		{
			Log.warn() << "Number of assigned chemical shifts could not be read" << endl;
		}
	}

	void NMRStarFile::readMolSystem_()
	{
		try
		{
			rewind_();
			test_(__FILE__, __LINE__, 
						search_("#  Molecular system description  #"),
						"Molecular system description could not be found");

			skipLines_();

			test_(__FILE__, __LINE__,
						search_("   _Mol_system_name", "#"),
						"Molecular system description could not be found");

			Position _position_ = 21;

			// systemname can be enclosed with ' '
			String word = getToken_(_position_);
			if (word[0] == '\'')
			{
				_position_ = 21;
				word = getToken_(_position_, '\'');
				word.erase(word.size()), 1;
			}
			system_name_ = word;
		}
		catch (ReadFile::ReadFileError)
		{
			Log.warn() << "The name of the molecular system could not be read." << endl;
		}
	}

	void NMRStarFile::readSampleConditions_()
	{
		try
		{
			rewind_();
			test_(__FILE__, __LINE__, 
						search_("#  Sample conditions  #"),
						"sample conditions could not be found");

			skipLines_();

			Position _position_;
			String temp;
			float value;

			while (search_("save_", "#"))
			{
				if (line_ == "save_")
				{
					continue;
				}

				SampleCondition* condition = new SampleCondition();
				condition->name = copyString_(5);
				if (!search_("      _Variable_value_units", "#"))
				{
					break;
				}
				skipLines_(1);

				// reading samples set until empty line
				while (line_.size() > 0)
				{ 
					_position_ = 0;
					temp = getToken_(_position_);
					try
					{
						value = getToken_(_position_).toFloat();
					}
					catch (Exception::InvalidFormat)
					{
						value = 0;
					}
					
					if (temp == "pH")
					{
						condition->pH = value;
					}
					else
					{
						if (temp == "temperature")
						{
							condition->temperature = value;
						}
						else 
						{
							if (temp == "pressure")
							{
									condition->pressure = value;
							}
						}
					}

					readLine_();
				}

				sample_conditions_.push_back(condition);
			}
		}
		catch (ReadFile::ReadFileError)
		{
			Log.warn() << "Sample Conditions could not be read." << endl;
		}
	}

	void NMRStarFile::initializeReferenceOptions_()
	{
		if (reference_options_.size() > 0)
		{
			return;
		}
		reference_options_.push_back("      _Mol_common_name");
		reference_options_.push_back("      _Atom_type");
		reference_options_.push_back("      _Atom_isotope_number");
		reference_options_.push_back("      _Atom_group");
		reference_options_.push_back("      _Chem_shift_units");
		reference_options_.push_back("      _Chem_shift_value");
		reference_options_.push_back("      _Reference_method");
		reference_options_.push_back("      _Reference_type");
		reference_options_.push_back("      _Indirect_shift_ratio");
		// .... more referenceOptions can be added here
	}

	void NMRStarFile::readShiftReferences_()
	{
		try
		{
			rewind_();
			NMRStarFile::initializeReferenceOptions_();

			test_(__FILE__, __LINE__, 
						search_("	#  Chemical shift referencing  #"),
						"Chemical shift referencing could not be found.");

			skipLines_();

			Position _position_;
			String word;
			std::vector<int> reference_positions;

			while (search_("save_", "#"))
			{
				if (line_ == "save_")
				{
					continue;
				}

				ShiftReferenceSet* shift_reference = new ShiftReferenceSet();
				shift_reference->name = copyString_(5);
				if (!search_("   loop_", "#"))
				{
					break;
				}
				readLine_();
				// data format of the references set until empty line occurs
				while (line_.size() > 0)
				{ 
					reference_positions.push_back(switch_(reference_options_));
					readLine_();
				}

				skipLines_();
				int lastpos;

				// read references set until empty line occurs
				while (line_.size() > 0)
				{ 
					ShiftReferenceElement*  reference_element = new ShiftReferenceElement();
					_position_ = 0;
					for (Position pos = 0; pos < reference_positions.size(); pos++)
					{
						lastpos = _position_;
						word = getToken_(_position_);
						if (word[0] == '\'')
						{
							_position_ = lastpos;
							word = getToken_(_position_, '\'');
							reference_element->atom_group = word;
						}
						
						if (reference_positions[pos] == -1)
						{
							continue;
						}
						switch (reference_positions[pos])
						{
							case MOL_COMMON_NAME:
										reference_element->mol_common_name = word;
										break;
							case ATOM_TYPE:
										reference_element->atom_type = word[0];
										break;
							case ISOTOPE_NUMBER:
										try
										{
											reference_element->isotope_number = word.toUnsignedInt();
										}
										catch (Exception::InvalidFormat)
										{
											reference_element->isotope_number = 0;
										}
										break;
							case ATOM_GROUP:
										reference_element->atom_group = word;
										break;
							case SHIFT_UNITS:
										reference_element->shift_units = word;
										break;
							case SHIFT_VALUE:
										try
										{
											reference_element->shift_value = word.toFloat();
										}
										catch (Exception::InvalidFormat)
										{
											reference_element->shift_value = 0;
										}
										break;
							case REFERENCE_METHOD:
										if (word == "internal")
										{
											reference_element->reference_method = INTERNAL_REFERENCE;
										}
										else
										{
											if (word == "external")
											{
												reference_element->reference_method = EXTERNAL_REFERENCE;
											}
											else
											{
												reference_element->reference_method = UNKNOWN_REFERENCE;
											}
										}
										break;
							case REFERENCE_TYPE:
										if (word == "direct")
										{
											reference_element->reference_type = DIRECT_TYPE;
										}
										else
										{
											if (word == "indirect")
											{
												reference_element->reference_type = INDIRECT_TYPE;
											}
											else
											{
												reference_element->reference_type = UNKNOWN_TYPE;
											}
										}
										break;
							case INDIRECT_SHIFT_RATIO:
										try
										{
											reference_element->indirect_shift_ratio = word.toFloat();
										}
										catch (Exception::InvalidFormat)
										{
											reference_element->indirect_shift_ratio = 0;
										}
										break;
							default:
								Log.warn() << "unknown reference option" << endl;
								exit(1);
						}
					}
					readLine_();
					shift_reference->elements.push_back(reference_element);
				}

				test_(__FILE__, __LINE__, 
							shift_reference->elements.size() > 0,
							"no data for shift references found");

				shift_references_.push_back(shift_reference);
				skipLines_(4); // skip save_
			}
		}
		catch (...)
		{
			Log.warn() << "Shift References could not be read." << endl;
		}
	}

	NMRAtomData* NMRStarFile::processShiftLine_()
	{
		NMRAtomData* ad = new NMRAtomData();
		Position pos = 0;

		try
		{
			ad->atom_ID = getToken_(pos).toUnsignedInt();
			ad->residue_seq_code = getToken_(pos).toUnsignedInt();
			ad->residue_label = getToken_(pos);
			ad->atom_name = getToken_(pos);
			ad->atom_type = (getToken_(pos))[0];
			ad->shift_value = getToken_(pos).toFloat();
			try
			{
				ad->error_value = getToken_(pos).toFloat();			
			}
			catch (Exception::InvalidFormat)
			{
				ad->error_value = 0;
			}
			try
			{
				ad->ambiguity_code = getToken_(pos).toUnsignedInt();
			}
			catch (Exception::InvalidFormat)
			{
				ad->ambiguity_code = 0;
			}

		}
		catch (Exception::GeneralException e)
		{
			Log.error() << "An error occured while reading shift data:" << endl;
			throw e;
		}
		readLine_();
		return ad;
	}

	void NMRStarFile::readShifts_()
	{
		rewind_();

		test_(__FILE__, __LINE__, 
					search_("#      9             Ambiguous, specific ambiguity not defined    #"),
					"Assigned chemical shift lists could not be found");

		skipLines_(2);

		String word;
		Position _position_;

		while (search_("save_", "#"))
		{
			if (line_ == "save_")
			{
				continue;
			}
			NMRAtomDataSet* atom_data_sets = new NMRAtomDataSet();
			atom_data_sets->name = copyString_(5);
			_position_ = 35;
			
			try
			{
				test_(__FILE__, __LINE__,
							search_("   _Sample_conditions_label", "#"),
							"chemical shift does not contain Sample_conditions_label");

				word = getToken_(_position_);
				word.erase(0, 1);

				for (Position pos = 0; pos < sample_conditions_.size(); pos++)
				{
					if (sample_conditions_[pos]->name == word)
					{
						atom_data_sets->condition = sample_conditions_[pos];
					}
				}				
			}
			catch (ReadFile::ReadFileError)
			{
				rewind_();
			}

			try
			{
				test_(__FILE__, __LINE__,
							search_("   _Chem_shift_reference_set_label"),
							"chemical shift does not contain _Chem_shift_reference_set_label");

				_position_ = 35;
				word = getToken_(_position_);
				word.erase(0, 1);

				for (Position pos = 0; pos < shift_references_.size() ; pos++)
				{
					if (shift_references_[pos]->name == word)
					{
						atom_data_sets->reference = shift_references_[pos];
					}
				}			
			}
			catch (ReadFile::ReadFileError)
			{
				rewind_();
			}

			test_(__FILE__, __LINE__, 
						search_("      _Chem_shift_ambiguity_code"),
						"chemical shift does not contain _Chem_shift_ambiguity_code");

			skipLines_(1);
			while (line_.size() > 0)
			{
				atom_data_sets->atom_data.push_back(processShiftLine_());
			}
			atom_data_sets_.push_back(atom_data_sets);
		}

		if (number_of_shifts_ > 0)
		{
			test_(__FILE__, __LINE__, 
						atom_data_sets_.size() == number_of_shifts_,
						"wrong number of shift sets found");
		}
	}

	NMRStarFile::NMRStarFile()
		:	ReadFile(),
			number_of_shifts_(0)
	{
	}

	NMRStarFile::NMRStarFile(const NMRStarFile& f)
		:	ReadFile(f),
			number_of_shifts_		(f.number_of_shifts_),
			atom_data_sets_			(f.atom_data_sets_),
			sample_conditions_  (f.sample_conditions_),
			shift_references_   (f.shift_references_),
			system_name_				(f.system_name_)
	{
	}

	NMRStarFile::NMRStarFile(const String& file_name)
		:	ReadFile(file_name),
			number_of_shifts_(0)
	{
		readEntryInformation_();
		readMolSystem_();
		readSampleConditions_();
		readShiftReferences_();
		readShifts_();
	}

	NMRStarFile& NMRStarFile::operator = (const NMRStarFile& f)
	{
		file_name_ = f.file_name_;
		rewind_();

		number_of_shifts_		= f.number_of_shifts_;;
		atom_data_sets_			= f.atom_data_sets_;
		sample_conditions_  = f.sample_conditions_ ;
		shift_references_   = f.shift_references_;
		system_name_				= f.system_name_ ;
		return *this;
	}

} //namespace
