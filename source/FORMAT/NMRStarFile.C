#include <BALL/FORMAT/NMRStarFile.h>
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>


#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif

using namespace std;

namespace BALL
{

	ostream& operator << (ostream &s, const NMRAtomData& ad)
	{
		s << "atomID: " << ad.atomID;
		s << " residueSeqCode: " << ad.residueSeqCode;
		s << " residueLabel: " << ad.residueLabel;
		s << " atomName: " << ad.atomName;
		s << " shiftValue: " << ad.shiftValue;
		s << " errorValue: " << ad.errorValue;
		s << " ambiguityCode: " << ad.ambiguityCode;
		s << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const SampleCondition& sc)
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name " << sc.name << endl;;
		s << "temperature "<< sc.temperature << endl;;
		s << "pH "<< sc.pH << endl;;
		s << "pressure "<< sc.pressure << endl;;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const ShiftReferenceElement& sre)
	{
		s << endl;
		s << "molCommonName " <<	sre.molCommonName << endl;
		s << "atomType " <<  sre.atomType << endl;
		s << "isotopeNumber " <<  sre.isotopeNumber << endl;
		s << "atomGroup " <<  sre.atomGroup << endl;
		s << "shiftUnits " <<  sre.shiftUnits << endl;
		s << "shiftValue " <<  sre.shiftValue << endl;
		s << "referenceMethod ";
			switch (sre.referenceMethod)
			{
			case NMRStarFile::INTERNAL_REFERENCE:
				cout << "internal";
				break;
			case NMRStarFile::EXTERNAL_REFERENCE:
				cout << "external";
				break;
			case NMRStarFile::UNKNOWN_REFERENCE:
				cout << "unknown";
				break;
			case NMRStarFile::UNSET_REFERENCE:
				cout << "unset";
				break;
			default:
				cout << "?????????????????????????";
			}
		s << endl;
		s << "referenceType ";
			switch (sre.referenceType)
			{
			case NMRStarFile::DIRECT_TYPE:
				cout << "direct";
				break;
			case NMRStarFile::INDIRECT_TYPE:
				cout << "indirect";
				break;
			case NMRStarFile::UNKNOWN_TYPE:
				cout << "unknown";
				break;
			case NMRStarFile::UNSET_TYPE:
				cout << "unset";
				break;
			default:
				cout << "?????????????????????????";
			}
		s << endl;
		s << "indirectShiftRatio " <<  sre.indirectShiftRatio << endl << endl;
		return s;
	}
	
	ostream& operator << (::std::ostream& s, const ShiftReferenceSet& sr)
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name " << sr.name << endl << endl;
		for (Position i = 0; i < sr.elements.size() ; i++ )
		{
			s << *(sr.elements[i]);
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	ostream& operator << (::std::ostream& s, const NMRAtomDataSet& set)
	{
		s << endl << "name " << set.name << endl << endl;

		for (Position i = 0; i < set.atomData.size() ; i++)
		{
			if (set.atomData[i] == 0)
			{
				s << "0 - pointer for atomDataItem" << endl;			
			}
			else
			{
				s << *set.atomData[i];
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
		: atomType(),
			isotopeNumber(0),
			atomGroup(),
			shiftUnits(),
			shiftValue(),
			referenceMethod(NMRStarFile::UNSET_REFERENCE),
		  referenceType(NMRStarFile::UNSET_TYPE),
			indirectShiftRatio()
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

	Size NMRStarFile::getNumberOfAtoms() const
	{
		Size max = 0;
		for (Position i = 0;  i < atomDataSets_.size(); i++)
		{
			if (atomDataSets_[i]->atomData.size() > max)
			{
				 max = atomDataSets_[i]->atomData.size();
			}
		}

		return max;
	}

	const vector<NMRAtomDataSet*>& NMRStarFile::getData() const
	{
		return atomDataSets_;
	}

	void NMRStarFile::readEntryInformation_()
	{
		try
		{
			test_(search_("       assigned_chemical_shifts"),
					 "Number of assigned chemical shifts could not be found");

			Position _position_ = 31;
			numberOfShifts = atoi(getToken_(_position_));
		}
		catch (ReadFileError)
		{
			Log.warn() << "Number of assigned chemical shifts = 0";
		}
	}

	void NMRStarFile::readMolSystem_()
	{
		string word;
		try
		{
			test_(search_("#  Molecular system description  #"),
					 "Molecular system description could not be found");

			skipLines_();

			test_(search_("   _Mol_system_name", "#"),
					 "Molecular system description could not be found");

			Position _position_ = 21;
				// systemname kann mit ' ' umschlossen sein
				word = getToken_(_position_);
				if (word[0] == '\'')
				{
					_position_ = 21;
					word = getToken_(_position_, '\'');
					word.erase(word.size()), 1;
				}
				systemName_ = word;
		}
		catch (ReadFileError)
		{
			Log.warn() << "Molecular system description could not be found";
		}
	}

	void NMRStarFile::readSampleConditions_()
	{
		try
		{
			Position _position_;
			string temp;

			test_(search_("#  Sample conditions  #"),
				"sample conditions could not be found");

			skipLines_();

			while (search_("save_", "#"))
			{
				if (strcmp(line_, "save_") == 0)
				{
					continue;
				}

				SampleCondition* sampleCondition = new SampleCondition();
				sampleCondition->name = copyString_(5, strlen(line_)-1);
				if (!search_("      _Variable_value_units", "#"))
				{
					break;
				}
				skipLines_(1);

				// daten des samples set einlesen bis leerzeile
				while (strlen(line_) > 0)
				{ 
					_position_ = 0;
					temp = getToken_(_position_);

					if (temp == "pH")
					{
						sampleCondition->pH = atof(getToken_(_position_));
					}
					else
					{
						if (temp == "temperature")
						{
							sampleCondition->temperature = atof(getToken_(_position_));
						}
						else 
						{
							if (temp == "pressure")
							{
								sampleCondition->pressure = atof(getToken_(_position_));
							}
						}
					}

					readLine_();
				}

				sampleConditions_.push_back(sampleCondition);
			}
		}
		catch (ReadFileError)
		{
		}
	}

	void NMRStarFile::initializeReferenceOptions_()
	{
		if (referenceOptions_.size() > 0)
		{
			return;
		}
		referenceOptions_.push_back("      _Mol_common_name");				//molCommonName
		referenceOptions_.push_back("      _Atom_type");							//atomType
		referenceOptions_.push_back("      _Atom_isotope_number");		//isotopeNumber
		referenceOptions_.push_back("      _Atom_group");							//atomGroup
		referenceOptions_.push_back("      _Chem_shift_units");				//shiftUnits
		referenceOptions_.push_back("      _Chem_shift_value");				//shiftValue
		referenceOptions_.push_back("      _Reference_method");				//internalReferenceMethod
		referenceOptions_.push_back("      _Reference_type");					//directReferenceType
		referenceOptions_.push_back("      _Indirect_shift_ratio");		//indirectShiftRatio
		// .... more referenceOptions can be added here
	}

	void NMRStarFile::readShiftReferences_()
	{
		Position _position_;
		string word;
		vector<int> referencePositions;
		NMRStarFile::initializeReferenceOptions_();

		if(!search_("	#  Chemical shift referencing  #"))
		{
			return;
		}

		skipLines_();

		while (search_("save_", "#"))
		{
			if (strcmp(line_, "save_") == 0)
			{
				continue;
			}

			ShiftReferenceSet* shiftReference = new ShiftReferenceSet();
			shiftReference->name = copyString_(5, strlen(line_)-1);
			if (!search_("   loop_", "#"))
			{
				break;
			}
			readLine_();
			// datenformat des reference set einlesen bis leerzeile
			while (strlen(line_) > 0)
			{ 
				referencePositions.push_back(switch_(referenceOptions_));

				readLine_();
			}

			skipLines_();
			int lastpos;
			// das references set einlesen bis leerzeile
			while (strlen(line_) > 0)
			{ 
				ShiftReferenceElement*  shiftReferenceElement = new ShiftReferenceElement();
				_position_ = 0;
				for (Position i = 0; i < referencePositions.size(); i++)
				{
					lastpos = _position_;
					word = getToken_(_position_);
					if (word[0] == '\'')
					{
						_position_ = lastpos;
						word = getToken_(_position_, '\'');
						shiftReferenceElement->atomGroup = string(word, 1, word.size());
					}
					
					if (referencePositions[i] == -1)
					{
						continue;
					}
					switch (referencePositions[i])
					{
					case MOL_COMMON_NAME:
								shiftReferenceElement->molCommonName = word;
								break;
					case ATOM_TYPE:
								shiftReferenceElement->atomType = word[0];
								break;
					case ISOTOPE_NUMBER:
								shiftReferenceElement->isotopeNumber = atoi(word.c_str());
								break;
					case ATOM_GROUP:
								shiftReferenceElement->atomGroup = word;
								break;
					case SHIFT_UNITS:
								shiftReferenceElement->shiftUnits = word;
								break;
					case SHIFT_VALUE:
								shiftReferenceElement->shiftValue = atof(word.c_str());
								break;
					case REFERENCE_METHOD:
								if (word == "internal")
								{
									shiftReferenceElement->referenceMethod = INTERNAL_REFERENCE;
								}
								else
								{
									if (word == "external")
									{
										shiftReferenceElement->referenceMethod = EXTERNAL_REFERENCE;
									}
									else
									{
										shiftReferenceElement->referenceMethod = UNKNOWN_REFERENCE;
									}
								}
								break;
					case REFERENCE_TYPE:
								if (word == "direct")
								{
									shiftReferenceElement->referenceType = DIRECT_TYPE;
								}
								else
								{
									if (word == "indirect")
									{
										shiftReferenceElement->referenceType = INDIRECT_TYPE;
									}
									else
									{
										shiftReferenceElement->referenceType = UNKNOWN_TYPE;
									}
								}
								break;
					case INDIRECT_SHIFT_RATIO:
								shiftReferenceElement->indirectShiftRatio = atof(word.c_str());
								break;
					default:
						cerr << "unknown referenceOption";
						exit(1);
					}
				}
				readLine_();
				shiftReference->elements.push_back(shiftReferenceElement);
			}

			test_(shiftReference->elements.size() > 0,
						"no data for shift references found");

			shiftReferences_.push_back(shiftReference);
			skipLines_(4); // ueberspringt save_
		}
	}

	NMRAtomData* NMRStarFile::processShiftLine_()
	{
		NMRAtomData* ad = new NMRAtomData();
		Position pos = 0;

		try
		{
			ad->atomID = atoi(getToken_(pos));
			ad->residueSeqCode = atoi(getToken_(pos));
			ad->residueLabel = getToken_(pos);
			ad->atomName = getToken_(pos);
			ad->atomType = (getToken_(pos))[0];
			ad->shiftValue = atof(getToken_(pos));
			ad->errorValue = atof(getToken_(pos));
			ad->ambiguityCode = atoi(getToken_(pos));
		}
		catch (ReadFileError)
		{
			Log.error() << "An error occured while reading shift data.";
			throw ReadFileError();
		}
		readLine_();
		return ad;
	}

	void NMRStarFile::readShifts_()
	{
		Position _position_;
		string word;
		test_(search_("#      9             Ambiguous, specific ambiguity not defined    #"),
					"Assigned chemical shift lists could not be found");
		skipLines_(2);

		while (search_("save_", "#"))
		{
			if (strcmp(line_, "save_") == 0)
			{
				continue;
			}
			NMRAtomDataSet* atomDataSet = new NMRAtomDataSet();
			atomDataSet->name = copyString_(5, strlen(line_)-1);
			_position_ = 35;

			test_(search_("   _Sample_conditions_label", "#"),
						"chemical shift does not contain Sample_conditions_label");

			word = getToken_(_position_);
			word.erase(0, 1);

			for (Position i = 0; i < sampleConditions_.size() ; i++)
			{
				if (sampleConditions_[i]->name == word)
				{
					atomDataSet->condition = sampleConditions_[i];
				}
			}

			test_(search_("   _Chem_shift_reference_set_label", "   loop_"),
						"chemical shift does not contain _Chem_shift_reference_set_label");

			_position_ = 35;
			word = getToken_(_position_);
			word.erase(0, 1);

			for (Position i = 0; i < shiftReferences_.size() ; i++)
			{
				if (shiftReferences_[i]->name == word)
				{
					atomDataSet->reference = shiftReferences_[i];
				}
			}
			test_(search_("      _Chem_shift_ambiguity_code", "#"),
						"chemical shift does not contain _Chem_shift_ambiguity_code");

			skipLines_(1);
			while (strlen(line_) > 0)
			{
				atomDataSet->atomData.push_back(processShiftLine_());
			}
			atomDataSets_.push_back(atomDataSet);
		}

		if (numberOfShifts > 0)
		{
			test_(atomDataSets_.size() == numberOfShifts,
						"wrong number of shift sets found");
		}
	}

	NMRStarFile::NMRStarFile(char* filename)
		:	numberOfShifts(0)
	{
		in.open(filename);
		test_(in,	"File could not be opend:");
		if (!in)
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, filename);
		}

		readEntryInformation_();
		readMolSystem_();
		readSampleConditions_();
		readShiftReferences_();
		readShifts_();
	}

} //namespace
