#include <BALL/FORMAT/NMRStarFile.h>
#include <iostream> 
#include <fstream>

using namespace std;

namespace BALL
{

	ostream& operator << (ostream &s, const NMRAtomData& ad)
		throw()
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
		throw()
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
		throw()
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
		throw()
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
		throw()
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
		throw()
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
		throw()
		: name(),
			temperature(),
			pH(),
			pressure()
	{
	}

	NMRAtomDataSet::NMRAtomDataSet()
		throw()
		: name(),
			condition(0),
			reference(0)
	{
	}

	std::vector<String> NMRStarFile::reference_options_;



	NMRStarFile::NMRStarFile()
		throw ()
		:	LineBasedFile(),
			number_of_shifts_(0)
	{
	}

	NMRStarFile::NMRStarFile(const NMRStarFile& f)
		throw (Exception::FileNotFound)
		:	LineBasedFile(f),
			number_of_shifts_		(f.number_of_shifts_),
			atom_data_sets_			(f.atom_data_sets_),
			sample_conditions_  (f.sample_conditions_),
			shift_references_   (f.shift_references_),
			system_name_				(f.system_name_)
	{
	}

	NMRStarFile::NMRStarFile(const String& file_name)
		throw (Exception::FileNotFound, Exception::ParseError)
		:	LineBasedFile(file_name),
			number_of_shifts_(0)
	{
		try 
		{
			readEntryInformation_();
			readMolSystem_();
			readSampleConditions_();
			readShiftReferences_();
			readShifts_();
		}
		catch (Exception::GeneralException e)
		{
			throw Exception::ParseError(e.getFile(), e.getLine(), String("NMRStarFile: ") + e.getMessage(), "");
		}
		catch (...)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "NMRStarFile: caught unexpected exception while reading file.", "");
		}
	}

	NMRStarFile::~NMRStarFile()
		throw()
	{
	}

	const NMRStarFile& NMRStarFile::operator = (const NMRStarFile& f)
		throw ()
	{
		LineBasedFile::operator = (f);

		number_of_shifts_		= f.number_of_shifts_;;
		atom_data_sets_			= f.atom_data_sets_;
		sample_conditions_  = f.sample_conditions_ ;
		shift_references_   = f.shift_references_;
		system_name_				= f.system_name_ ;

		return *this;
	}

	Size NMRStarFile::getNumberOfAtoms() 
		const	throw()
	{
		Size max = 0;
		for (Position pos = 0;  pos < atom_data_sets_.size(); pos++)
		{
			if (atom_data_sets_[pos]->atom_data.size() > max)
			{
				 max = (Size)atom_data_sets_[pos]->atom_data.size();
			}
		}

		return max;
	}

	const std::vector<NMRAtomDataSet*>& NMRStarFile::getData() 
		const	throw()
	{
		return atom_data_sets_;
	}

	void NMRStarFile::readEntryInformation_()
		throw (LineBasedFile::LineBasedFileError, Exception::InvalidFormat)
	{
		try
		{
			rewind();
			test(__FILE__, __LINE__, 
				search("       assigned_chemical_shifts", "#  Molecular system description  #", true),
				"Number of assigned chemical shifts could not be found");

			number_of_shifts_ = getField(1).toUnsignedInt();
		}
		catch (LineBasedFile::LineBasedFileError)
		{
			Log.warn() << "Number of assigned chemical shifts could not be found" << endl;
		}
		catch (Exception::InvalidFormat e)
		{
			Log.warn() << "Number of assigned chemical shifts could not be transformed into a number: " 
								 << e.getMessage() << endl;
		}
	}

	void NMRStarFile::readMolSystem_()
		throw()
	{
		try
		{
			test(__FILE__, __LINE__, 
						search("#  Molecular system description  #", 
									 "#  Sample contents and methodology  #", true),
						"Molecular system description could not be found");

			skipLines(2);

			test(__FILE__, __LINE__,
						search("   _Mol_system_name", "#", true),
						"Molecular system description could not be found");

			// systemname can be enclosed with '' or ""
				system_name_ = getField(1, "'\"");

			if (system_name_ == "")
			{
				Log.warn() << "The name of the molecular system could not be read." << endl;
			}
		}
		catch (LineBasedFile::LineBasedFileError e)
		{
			Log.warn() << e.getMessage() << endl;
		}
	}

	void NMRStarFile::readSampleConditions_()
		throw()
	{
		try
		{
			test(__FILE__, __LINE__, 
						search("#  Sample conditions  #",
									 "#  NMR parameters  #", true),
						"sample conditions could not be found");

			skipLines(2);

			String word;
			float value;

			while (search("save_", "#", false))
			{
				if (line_ == "save_")
				{
					continue;
				}

				SampleCondition* condition = new SampleCondition();
				condition->name.set(getLine(), 5);
				if (!search("      _Variable_value_units", "#"))
				{
					break;
				}
				skipLines(1);

				// reading samples set until empty line
				while (line_.size() > 0)
				{ 
					word = getField(1);
					try
					{
						value = word.toFloat();
					}
					catch (Exception::InvalidFormat)
					{
						Log.warn() << "Sample Condition value could not be transformed to a number: " 
											 << word << endl;
						value = 0;
					}

					word = getField(0);		
					if (word == "pH")
					{
						condition->pH = value;
					}
					else
					{
						if (word == "temperature")
						{
							condition->temperature = value;
						}
						else 
						{
							if (word == "pressure")
							{
									condition->pressure = value;
							}
						}
					}

					readLine();
				}
				sample_conditions_.push_back(condition);
				skipLines(3);  // skip save_

			}
		}
		catch (LineBasedFile::LineBasedFileError e)
		{
			Log.warn() << "Sample Conditions could not be read.\n" 
								 << e.getMessage() << endl;
		}
	}

	void NMRStarFile::initializeReferenceOptions_()
		throw()
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
		throw()
	{
		try
		{
			NMRStarFile::initializeReferenceOptions_();

			test(__FILE__, __LINE__, 
						search("	#  Chemical shift referencing  #",
									 "	#  Assigned chemical shift lists  #", true),
						"Chemical shift referencing could not be found.");

			skipLines(2);

			String word;
			std::vector<int> reference_positions;

			while (search("save_", "#", false))
			{
				if (line_ == "save_")
				{
					continue;
				}

				ShiftReferenceSet* shift_reference = new ShiftReferenceSet();
				shift_reference->name.set(getLine(), 5);

				if (!search("   loop_", "#", false))
				{
					break;
				}
				readLine();
				// read data format of the references set until empty line occurs
				while (line_.size() > 0)
				{ 
					reference_positions.push_back(switchString(reference_options_));
					readLine();
				}

				skipLines(0);

				// read references set until empty line occurs
				while (line_.size() > 0)
				{ 
					ShiftReferenceElement*  reference_element = new ShiftReferenceElement();
					for (Position pos = 0; pos < reference_positions.size(); pos++)
					{
						// _Atom_group may be quoted
						word = getField(pos, "'\"");
						
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
											Log.warn() << "isotope number could not be transformed into a number: " 
																 << word << endl;
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
											Log.warn() << "shift value could not be transformed into a number: " 
																 << word << endl;
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
											Log.warn() << "shift ratio could not be transformed into a number: " 
																 << word << endl;
										}
										break;
							default:
								Log.warn() << "unknown reference option" << endl;
								exit(1);
						}
					}
					readLine();
					shift_reference->elements.push_back(reference_element);
				}

				test(__FILE__, __LINE__, 
							shift_reference->elements.size() > 0,
							"no data for shift references found");
				shift_references_.push_back(shift_reference);
				skipLines(4); // skip save_
			}
		}
		catch (Exception::GeneralException e)
		{
			Log.warn() << "NMRStarFile: unable to read shift references." << e.getMessage() << endl;
		}
	}

	NMRAtomData* NMRStarFile::processShiftLine_()
		throw (LineBasedFile::LineBasedFileError)
	{
		NMRAtomData* ad = new NMRAtomData();

		try
		{
			ad->atom_ID = getField(0).toUnsignedInt();
			ad->residue_seq_code = getField(1).toUnsignedInt();
			ad->residue_label = getField(2);
			ad->atom_name = getField(3);
			ad->atom_type = (getField(4))[0];
			ad->shift_value = getField(5).toFloat();
			try
			{
				ad->error_value = getField(6).toFloat();			
			}
			catch (Exception::InvalidFormat)
			{
				ad->error_value = 0;
			}
			try
			{
				ad->ambiguity_code = getField(7).toUnsignedInt();
			}
			catch (Exception::InvalidFormat)
			{
				ad->ambiguity_code = 0;
			}

		}
		catch (Exception::InvalidFormat e)
		{
			Log.error() << "An error occured while reading shift data:" << endl;
			throw LineBasedFile::LineBasedFileError(__FILE__, __LINE__, this, e.getMessage());
		}
		readLine();
		return ad;
	}

	void NMRStarFile::readShifts_()
		throw (LineBasedFile::LineBasedFileError)
	{
		test(__FILE__, __LINE__, 
				search("#      9             Ambiguous, specific ambiguity not defined    #", false),
				"Assigned chemical shift lists could not be found");

		skipLines(3);

		String word;

		while (search("save_", "#", false))
		{
			if (line_ == "save_")
			{
				continue;
			}
			NMRAtomDataSet* atom_data_sets = new NMRAtomDataSet();
			atom_data_sets->name.set(getLine(), 5);
			try
			{
				test(__FILE__, __LINE__,
							search("   _Sample_conditions_label", "#", false),
							"chemical shift does not contain Sample_conditions_label");

				word = getField(1);
				if (word[0] == '$')
				{
					word.erase(0, 1);
				}

				for (Position pos = 0; pos < sample_conditions_.size(); pos++)
				{
					if (sample_conditions_[pos]->name == word)
					{
						atom_data_sets->condition = sample_conditions_[pos];
					}
				}				
			}
			catch (LineBasedFile::LineBasedFileError)
			{
				rewind();
			}

			try
			{
				test(__FILE__, __LINE__,
							search("   _Chem_shift_reference_set_label", false),
							"chemical shift does not contain _Chem_shift_reference_set_label");

				word = getField(1);
				if (word[0] == '$')
				{
					word.erase(0, 1);
				}

				for (Position pos = 0; pos < shift_references_.size() ; pos++)
				{
					if (shift_references_[pos]->name == word)
					{
						atom_data_sets->reference = shift_references_[pos];
					}
				}			
			}
			catch (LineBasedFile::LineBasedFileError)
			{
				rewind();
			}

			test(__FILE__, __LINE__, 
						search("      _Chem_shift_ambiguity_code", false),
						"chemical shift does not contain _Chem_shift_ambiguity_code");

			skipLines();
			while (line_.size() > 0)
			{
				atom_data_sets->atom_data.push_back(processShiftLine_());
			}
			atom_data_sets_.push_back(atom_data_sets);
		}

		if (number_of_shifts_ > 0)
		{
			test(__FILE__, __LINE__, 
						atom_data_sets_.size() == number_of_shifts_,
						"wrong number of shift sets found");
		}
	}

	bool NMRStarFile::operator == (const NMRStarFile& f)  throw()
	{
		return File::operator == (f);
	}

	bool NMRStarFile::operator != (const NMRStarFile& f)  throw()
	{
		return !(File::operator == (f));
	}

	void NMRStarFile::clear() throw()
	{
		LineBasedFile::clear();
		number_of_shifts_ = 0;
		system_name_ = "";
		reference_options_.clear();

		vector<NMRAtomDataSet*>::iterator data_it = atom_data_sets_.begin();
		for (; data_it != atom_data_sets_.end(); ++data_it)
		{
			delete *data_it;
		}
		atom_data_sets_.clear();

		vector<SampleCondition*>::iterator sample_it = sample_conditions_.begin();
		for (; sample_it != sample_conditions_.end(); ++sample_it)
		{
			delete *sample_it;
		}
		sample_conditions_.clear();

		vector<ShiftReferenceSet*>::iterator references_it = shift_references_.begin();
		for (; references_it != shift_references_.end(); ++references_it)
		{
			delete *references_it;
		}
		shift_references_.clear();

	}

} //namespace
