// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/NMRStarFile.h>
#include <iostream> 
#include <fstream>

using namespace std;

namespace BALL
{
	const float    NMRStarFile::FLOAT_VALUE_NA    = Limits<float>::max();
	const Position NMRStarFile::POSITION_VALUE_NA = Limits<Position>::max();

	NMRStarFile::NMRAtomData::NMRAtomData()
		: atom_ID(0),
			residue_seq_code(0),
			residue_label(""),
			atom_name(""),
			atom_type(0),
			shift_value(0),
			error_value(0),
			ambiguity_code(0)
	{
	}


	NMRStarFile::ShiftReferenceSet::ShiftReferenceSet()
		: name(),
			elements()
	{
	}

	ostream& NMRStarFile::NMRAtomData::operator >> (ostream &s)	
	{
		s << "atom_ID: "					  << atom_ID;
		s << " residue_seq_code: "  << residue_seq_code;
		s << " residue_label: "		  << residue_label;
		s << " atom_name: "					<< atom_name;
		s << " shift_value: "				<< shift_value;
		s << " error_value: "				<< error_value;
		s << " ambiguity_code: "		<< ambiguity_code << endl;
		return s;
	}

	ostream& NMRStarFile::SampleCondition::operator >> (std::ostream& s)		
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name "				<< name << endl;;
		s << "temperature "	<< temperature << endl;;
		s << "pH "					<< pH << endl;;
		s << "pressure "		<< pressure << endl;;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& NMRStarFile::ShiftReferenceElement::operator >> (std::ostream& s)		
	{
		s << endl;
		s << "mol_common_name "		<< mol_common_name << endl;
		s << "atom_type "					<< atom_type << endl;
		s << "isotope_number "		<< isotope_number << endl;
		s << "atom_group "				<< atom_group << endl;
		s << "shift_units "				<< shift_units << endl;
		s << "shift_value "				<< shift_value << endl;
		s << "reference_method ";
		switch (reference_method)
		{
			case NMRStarFile::INTERNAL_REFERENCE:	cout << "internal";	break;
			case NMRStarFile::EXTERNAL_REFERENCE:	cout << "external";	break;
			case NMRStarFile::UNKNOWN_REFERENCE:	cout << "unknown";	break;
			case NMRStarFile::UNSET_REFERENCE:		cout << "unset";		break;
			default:	cout << "?";
		}
		s << endl << "reference_type ";
		switch (reference_type)
		{
			case NMRStarFile::DIRECT_TYPE:		cout << "direct";		break;
			case NMRStarFile::INDIRECT_TYPE:	cout << "indirect";	break;
			case NMRStarFile::UNKNOWN_TYPE:		cout << "unknown";	break;
			case NMRStarFile::UNSET_TYPE:			cout << "unset";		break;
			default:	cout << "?";
		}
		s << endl << "indirect_shift_ratio " <<  indirect_shift_ratio << endl << endl;
		return s;
	}
	
	ostream& NMRStarFile::ShiftReferenceSet::operator >> (std::ostream& s)	
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name " << name << endl << endl;
		for (Position pos = 0; pos < elements.size() ; pos++ )
		{
			elements[pos] >> s;
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& NMRStarFile::NMRAtomDataSet::operator >> (std::ostream& s)		
	{
		s << endl << "name " << name << endl << endl;

		for (Position pos = 0; pos < atom_data.size() ; pos++)
		{
			atom_data[pos] >> s;
		}
		
		s << endl;
		condition >> s;
		reference >> s;
		s << endl;
		return s;
	}

	NMRStarFile::ShiftReferenceElement::ShiftReferenceElement()	
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

	NMRStarFile::SampleCondition::SampleCondition()	
		: name(""),
			temperature(0.0),
			pH(7.0),
			pressure(0.0)
	{
	}

	NMRStarFile::NMRAtomDataSet::NMRAtomDataSet()	
		: name(),
			condition(),
			reference()
	{
	}
	
	NMRStarFile::EntryInformation::EntryInformation() 
		: entry_type(),
		  BMRB_accession_code(),
		  NMR_STAR_version(),
		  experimental_method()
	{
	}

	NMRStarFile::EntryInformation::~EntryInformation() 
	{
	}
	ostream& NMRStarFile::EntryInformation::operator >> (std::ostream& s)
	{
		s << "BMRB_accession_code:" << BMRB_accession_code;
 		s << " entry_type: "  			<< entry_type;
		s << " NMR_STAR_version: "	<< NMR_STAR_version;
		s << " experimental_method: "<< experimental_method;
		return s;
	}

	std::vector<String> NMRStarFile::reference_options_;

////////////////////////////////////////////////////////////////

	NMRStarFile::NMRStarFile()
		:	CIFFile(),
			number_of_shifts_(0),
			entry_information_(),
			atom_data_sets_(),
			sample_conditions_(),
			shift_references_(),
			system_name_()
	{
	}

		:	CIFFile(f),
			entry_information_  (f.entry_information_),
	NMRStarFile::NMRStarFile(const String& file_name, File::OpenMode open_mode)
		throw (Exception::FileNotFound)
		:	CIFFile(file_name, open_mode),
			number_of_shifts_(0),
			entry_information_(),
			atom_data_sets_(),
			sample_conditions_(),
			shift_references_(),
			system_name_()
	{
	}

	NMRStarFile::~NMRStarFile()		
	{
	}

	const NMRStarFile& NMRStarFile::operator = (const NMRStarFile& f)		
	{
		CIFFile::operator = (f);

		number_of_shifts_		= f.number_of_shifts_;;
		atom_data_sets_			= f.atom_data_sets_;
		sample_conditions_  = f.sample_conditions_ ;
		shift_references_   = f.shift_references_;
		system_name_				= f.system_name_ ;

		return *this;
	}

	bool NMRStarFile::read() 
		throw(Exception::ParseError)
	{
		CIFFile::read();
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
		return true;
	}

	{
		Size max = 0;
		/*
		for (Position pos = 0;  pos < atom_data_sets_.size(); pos++)
		{
			if (atom_data_sets_[pos].atom_data.size() > max)
			{
				max = (Size)atom_data_sets_[pos].atom_data.size();
			}
		}
		*/
		return max;
	}

	/*CIFFile::SaveFrame& NMRStarFile::getSaveframe(String category, Position db_pos) 
	{
		if (datablocks_.size() < (Size) db_pos)
		{
			Log.error() << "An error occured while searching a saveframe: index out of range" <<  std::endl;
			return dummy_saveframe_;
		}

		if (datablocks_[db_pos].hasSaveframeCategory(category))
		{
			return datablocks_[db_pos].getSaveframeByCategory(category);
		}
		else
		{
			Log.error() << "No such saveframe category in datablock " << db_pos << std::endl;
			return dummy_saveframe_;
		}
	}*/

	/* const CIFFile::Item* NMRStarFile::hasColumn(const SaveFrame* sf, String name) 
	{
		if (!sf)
			return 0;
	
		// look in all items of the current saveframe
		for (Size i=0; i < sf->items.size(); i++)
		{
			if (sf->items[i].is_loop)
			{
				// look in all columns of the current item
				for (Size col=0; col < sf->items[i].keys.size(); col++)
				{
					if ( sf->items[i].keys[col]== name)
					{
						return &(sf->items[i]);
					}
				}
			}
		}
		return 0;
	}*/

	const std::vector<NMRStarFile::NMRAtomDataSet>& NMRStarFile::getData() const
	{
		return atom_data_sets_;
	}

	void NMRStarFile::readEntryInformation_()
		throw (Exception::ParseError, Exception::InvalidFormat) // TODO::
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("entry_information"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("entry_information");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one entry information saveframe! " << std::endl; 
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					if (saveframes[sf].hasItem("_Entry_type"))
						entry_information_.entry_type = saveframes[sf].getItemValue("_Entry_type");	
					if (saveframes[sf].hasItem("_NMR_STAR_version"))
						entry_information_.NMR_STAR_version = saveframes[sf].getItemValue("_NMR_STAR_version");	
					if (saveframes[sf].hasItem("_BMRB_accession_number"))
						entry_information_.BMRB_accession_code = saveframes[sf].getItemValue("_BMRB_accession_number");	
					if (saveframes[sf].hasItem("_Experimental_method"))
						entry_information_.experimental_method = saveframes[sf].getItemValue("_Experimental_method");	
				}

			}

		}
		
		/*try
		{
			rewind();
			test(__FILE__, __LINE__, 
				search("       assigned_chemical_shifts", "#  Molecular system description  #", true),
				"Number of assigned chemical shifts could not be found");

			number_of_shifts_ = getField(1).toUnsignedInt();
		}
		catch (Exception::ParseError&)
		{
			Log.warn() << "Number of assigned chemical shifts could not be found" << endl;
		}
		catch (Exception::InvalidFormat& e)
		{
			Log.warn() << "Number of assigned chemical shifts could not be transformed into a number: " 
								 << e.getMessage() << endl;
		}*/
	}

	void NMRStarFile::readMolSystem_()	
	{
		/*try
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
		catch (Exception::ParseError& e)
		{
			Log.warn() << e.getMessage() << endl;
		}*/
	}

	void NMRStarFile::readSampleConditions_()		
	{
	/*	try
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

				SampleCondition condition;
				condition.name.set(getLine(), 5);
				if (!search("      _Variable_value_units", "#", true))
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
					catch (Exception::InvalidFormat&)
					{
						Log.warn() << "Sample Condition value could not be transformed to a number: " 
											 << word << endl;
						value = 0;
					}

					word = getField(0);		
					if (word == "pH")
					{
						condition.pH = value;
					}
					else
					{
						if (word == "temperature")
						{
							condition.temperature = value;
						}
						else 
						{
							if (word == "pressure")
							{
								condition.pressure = value;
							}
						}
					}

					readLine();
				}
				sample_conditions_.push_back(condition);
				skipLines(3);  // skip save_

			}
		}
		catch (Exception::ParseError& e)
		{
			Log.warn() << "Sample Conditions could not be read.\n" 
								 << e.getMessage() << endl;
		}*/
	}

	void NMRStarFile::initializeReferenceOptions_()		
	{
		/*if (reference_options_.size() > 0)
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
		*/
	}

	void NMRStarFile::readShiftReferences_()		
	{
		/*try
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

				ShiftReferenceSet shift_reference;
				shift_reference.name.set(getLine(), 5);

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
					ShiftReferenceElement reference_element;
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
										reference_element.mol_common_name = word;
										break;
							case ATOM_TYPE:
										reference_element.atom_type = word[0];
										break;
							case ISOTOPE_NUMBER:
										try
										{
											reference_element.isotope_number = word.toUnsignedInt();
										}
										catch (Exception::InvalidFormat&)
										{
											reference_element.isotope_number = 0;
											Log.warn() << "isotope number could not be transformed into a number: " 
																 << word << endl;
										}
										break;
							case ATOM_GROUP:
										reference_element.atom_group = word;
										break;
							case SHIFT_UNITS:
										reference_element.shift_units = word;
										break;
							case SHIFT_VALUE:
										try
										{
											reference_element.shift_value = word.toFloat();
										}
										catch (Exception::InvalidFormat&)
										{
											reference_element.shift_value = 0;
											Log.warn() << "shift value could not be transformed into a number: " 
																 << word << endl;
										}
										break;
							case REFERENCE_METHOD:
										if (word == "internal")
										{
											reference_element.reference_method = INTERNAL_REFERENCE;
										}
										else
										{
											if (word == "external")
											{
												reference_element.reference_method = EXTERNAL_REFERENCE;
											}
											else
											{
												reference_element.reference_method = UNKNOWN_REFERENCE;
											}
										}
										break;
							case REFERENCE_TYPE:
										if (word == "direct")
										{
											reference_element.reference_type = DIRECT_TYPE;
										}
										else
										{
											if (word == "indirect")
											{
												reference_element.reference_type = INDIRECT_TYPE;
											}
											else
											{
												reference_element.reference_type = UNKNOWN_TYPE;
											}
										}
										break;
							case INDIRECT_SHIFT_RATIO:
										try
										{
											reference_element.indirect_shift_ratio = word.toFloat();
										}
										catch (Exception::InvalidFormat&)
										{
											reference_element.indirect_shift_ratio = 0;
											Log.warn() << "shift ratio could not be transformed into a number: " 
																 << word << endl;
										}
										break;
							default:
								throw Exception::ParseError(__FILE__,__LINE__, word, "Unknown reference option");
						}
					}
					readLine();
					shift_reference.elements.push_back(reference_element);
				}

				test(__FILE__, __LINE__, 
							shift_reference.elements.size() > 0,
							"no data for shift references found");
				shift_references_.push_back(shift_reference);
				skipLines(4); // skip save_
			}
		}
		catch (Exception::GeneralException& e)
		{
			Log.warn() << "NMRStarFile: unable to read shift references." << e.getMessage() << endl;
		}*/
	}

/*	NMRStarFile::NMRAtomData NMRStarFile::processShiftLine_()
		throw (Exception::ParseError)
	{
		NMRAtomData ad;

		try
		{
			ad.atom_ID = getField(0).toUnsignedInt();
			ad.residue_seq_code = getField(1).toUnsignedInt();
			ad.residue_label = getField(2);
			ad.atom_name = getField(3);
			ad.atom_type = (getField(4))[0];
			ad.shift_value = getField(5).toFloat();
			try
			{
				ad.error_value = getField(6).toFloat();			
			}
			catch (Exception::InvalidFormat&)
			{
				ad.error_value = 0;
			}
			try
			{
				ad.ambiguity_code = getField(7).toUnsignedInt();
			}
			catch (Exception::InvalidFormat&)
			{
				ad.ambiguity_code = 0;
			}

		}
		catch (Exception::InvalidFormat& e)
		{
			Log.error() << "An error occured while reading shift data:" << endl;
			throw Exception::ParseError(__FILE__, __LINE__,  String("error while reading shift data from line ") 
																	+ String(getLineNumber()) + " from " + getName(), e.getMessage());
		}
		readLine();
		return ad;
	}*/

	void NMRStarFile::readShifts_()
		throw (Exception::ParseError) //??? wo wird die geworfen? TODO??
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("assigned_chemical_shifts"))
			{

				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("assigned_chemical_shifts");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one assigned_chemical_shifts saveframe! " << std::endl; 

				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					// look for the correct loop structure...
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0]== "_Atom_shift_assign_ID")
							{
								// we have found the shift values :-)
								// store the data
								Item* current_loop = &saveframes[sf].items[loop];

								NMRStarFile::NMRAtomDataSet atom_data_set;

								for (Size line = 0; line < current_loop->values.size(); line++ )
								{	
									NMRAtomData atom_data;

									// empty values are denoted by '.' want shall we do?

									atom_data.atom_ID = ((current_loop->values[line][0]=="." )? POSITION_VALUE_NA : current_loop->values[line][0].toUnsignedInt());
									atom_data.residue_seq_code = ((current_loop->values[line][1]=="." )? POSITION_VALUE_NA : current_loop->values[line][1].toUnsignedInt());
									// current_loop->values[line][1].toUnsignedInt();
									atom_data.residue_label = current_loop->values[line][2];
									atom_data.atom_name = current_loop->values[line][3];
									atom_data.atom_type = current_loop->values[line][4].toChar();
									atom_data.shift_value = ((current_loop->values[line][5]=="." )? POSITION_VALUE_NA : current_loop->values[line][5].toFloat());
									//current_loop->values[line][5].toFloat();
									atom_data.error_value = ((current_loop->values[line][6]=="." )? POSITION_VALUE_NA : current_loop->values[line][6].toFloat());

									//current_loop->values[line][6].toFloat();
									atom_data.ambiguity_code = current_loop->values[line][7].toUnsignedInt();

									atom_data_set.atom_data.push_back(atom_data);
								}
								atom_data_sets_.push_back(atom_data_set);
std::cout << "added a data_set with " << atom_data_set.atom_data.size() << std::endl;
							}
						}
					}
				}
			}
		}
		/*		test(__FILE__, __LINE__, 
					search("#    
					This class ... TODO!!9             Ambiguous, specific ambiguity not defined    #", false),
					"Assigned chemical shift lists could not be found");

					skipLines(3);

					String word;

					while (search("save_", "#", false))
					{
					if (line_ == "save_")
					{
					continue;
					}
					NMRStarFile::NMRAtomDataSet atom_data_sets;
					atom_data_sets.name.set(getLine(), 5);
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
					if (sample_conditions_[pos].name == word)
					{
					atom_data_sets.condition = sample_conditions_[pos];
					}
					}				
					}
			catch (Exception::ParseError&)
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
					if (shift_references_[pos].name == word)
					{
					atom_data_sets.reference = shift_references_[pos];
					}
					}			
					}
			catch (Exception::ParseError&)
					{
					rewind();
					}

					test(__FILE__, __LINE__, 
					search("      _Chem_shift_ambiguity_code", false),
					"chemical shift does not contain _Chem_shift_ambiguity_code");

		skipLines();
		while (line_.size() > 0)
		{
			atom_data_sets.atom_data.push_back(processShiftLine_());
		}
		atom_data_sets_.push_back(atom_data_sets);
	}

	if (number_of_shifts_ > 0)
	{
		test(__FILE__, __LINE__, 
				atom_data_sets_.size() == number_of_shifts_,
				"wrong number of shift sets found");
	}*/
	}

	bool NMRStarFile::operator == (const NMRStarFile& f)  
	{
		return File::operator == (f);
	}

	bool NMRStarFile::operator != (const NMRStarFile& f)  
	{
		return !(File::operator == (f));
	}

	void NMRStarFile::clear() 
	{
		CIFFile::clear();
		number_of_shifts_ = 0;
		system_name_ = "";
		reference_options_.clear();

		atom_data_sets_.clear();
		sample_conditions_.clear();
		shift_references_.clear();
	}

} //namespace
