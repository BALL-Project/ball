#ifndef BALL_NMR_READ_STAR_H
#define BALL_NMR_READ_STAR_H

#include <stdio.h>
#include <iostream> 
#include <fstream>
#include <string>
#include <vector>
#include "readFile.h"

namespace BALL 
{
	using namespace std;

	/** @name	NMRAtomData.
			This structure includes all information for one atom 
			delivered by a NMR-Star-File.
			All members are public for easy access.
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	//@{
	struct NMRAtomData
	{
		Position atomID;
		Position residueSeqCode;
		string residueLabel;
		string atomName;
		char atomType;
		float shiftValue;
		float errorValue;
		Position ambiguityCode;
	};

	/** @name	SampleConditions.
			This structure includes all information for a set of samples
			delivered by a NMR-Star-File.
			All members are public for easy access.
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	struct SampleCondition
	{
		SampleCondition();

		string name;
		float temperature;
		float pH;
		float pressure;
	};

	/** @name	ShiftReferenceElement.
			This structure includes all information for a shift reference element
			for one atom type delivered by a NMR-Star-File.
			All members are public for easy access.
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	struct ShiftReferenceElement
	{
		ShiftReferenceElement();

		string molCommonName;
		char atomType;
		Position isotopeNumber;
		string atomGroup;
		string shiftUnits;
		float shiftValue;
		char referenceMethod;
		char referenceType;
		float indirectShiftRatio;
	};

	/** @name	ShiftReferenceSet.
			This structure includes all information for all sets of shift references
			for several atom types delivered by a NMR-Star-File.
			All members are public for easy access.
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	struct ShiftReferenceSet
	{
		string name;
		vector<ShiftReferenceElement*> elements;
	};

	struct NMRAtomDataSet
	{
		NMRAtomDataSet();

		string name;
		vector<NMRAtomData*> atomData;
		SampleCondition* condition;
		ShiftReferenceSet* reference;
	};

	//@}

	/** @name	NMRStarFile.
			This class is designed to read a NMR-Star-File.
			To read a file simpy use the NMRStarFile(char* filename)- Konstruktor.
			All usefall data are extracted and stored.
			To get access to the data use getData().
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	//@{
	class NMRStarFile
		: public ReadFile
	{
		public:

			/** Detailled constuctor.
					Opens the given file and extracts all usefull data.
			*/
			NMRStarFile(char* filename);

			/** Get the maiximum number of atoms in all shift sets
			*/
			Size getNumberOfAtoms() const;

			/** Get the extracted data for the atoms.
			*/
			const vector<NMRAtomDataSet*>& getData() const;

			enum ReferenceMethod
			{
				INTERNAL_REFERENCE,
				EXTERNAL_REFERENCE,
				UNKNOWN_REFERENCE,
				UNSET_REFERENCE
			};

			enum ReferenceType
			{
				DIRECT_TYPE,
				INDIRECT_TYPE,
				UNKNOWN_TYPE,
				UNSET_TYPE
			};

		private:

			/*_	@name	NMR-Star specific Help-Methods
			*/
			//_@{

			/// function to extract the data from a chemical shift line
			NMRAtomData* processShiftLine_();

			/// reads the number of chemical shifts
			void readEntryInformation_();

			/// reads the molecular system name
			void readMolSystem_();

			/// reads the sample conditions
			void readSampleConditions_();

			/// reads the shift references
			void readShiftReferences_();

			/// reads the shift datas
			void readShifts_();

			/// initialize the referenceOptions
			void initializeReferenceOptions_(); // ??? soll static werden
			//_@}

			/*_	@name	NMR-Star specific elements
			*/
			//_@{

			/// the number of shift data
			Size numberOfShifts;

			/// the data for the atoms is stored here
			vector<NMRAtomDataSet*> atomDataSets_;

			/// the data for different sample sets
			vector<SampleCondition*> sampleConditions_;

			/// the data for shift references
			vector<ShiftReferenceSet*> shiftReferences_;

			/// contains strings with the used reference options
			vector<string> referenceOptions_; // ??? soll static werden

			/// name of the molecular system
			string systemName_;

			enum ShiftReferenceTypes
			{
				MOL_COMMON_NAME,
				ATOM_TYPE,
				ISOTOPE_NUMBER,
				ATOM_GROUP,
				SHIFT_UNITS,
				SHIFT_VALUE,
				REFERENCE_METHOD,
				REFERENCE_TYPE,
				INDIRECT_SHIFT_RATIO
			};

			//_@}

			/// default constuctor made private to prevent use
			NMRStarFile();
	};
	//@}


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

	SampleCondition::SampleCondition()
		: name(),
			temperature(),
			pH(),
			pressure()
	{
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
		s << endl<< "--------------------------------------" << endl;
		s << "name " << set.name << endl << endl;

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
		s << "--------------------------------------"  << endl << endl;
		return s;
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
		test_(search_("       assigned_chemical_shifts"),
				 "Number of assigned chemical shifts could not be found");

		Position _position_ = 31;

		try
		{
			numberOfShifts = atoi(getToken_(_position_));
		}
		catch (ReadFile_ProcessError)
		{
			cerr << "Number of assigned chemical shifts = 0";
			throw ReadFile_ProcessError();
		}
	}

	void NMRStarFile::readMolSystem_()
	{
		string word;
		test_(search_("#  Molecular system description  #"),
				 "Molecular system description could not be found");
		skipLines_();
		test_(search_("   _Mol_system_name", "#"),
				 "Molecular system description could not be found");
		Position _position_ = 21;
		try
		{
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
		catch (ReadFile_ProcessError)
		{
			cerr << "Molecular system description could not be found";
			throw ReadFile_ProcessError();
		}
	}

	void NMRStarFile::readSampleConditions_()
	{
		Position _position_;
		string temp;

		test_(search_("#  Sample conditions  #"),	"Sample conditions could not be found");
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
//cout << temp << endl;//////////////

				if (temp == "pH")
				{
					sampleCondition->pH = atof(getToken_(_position_));
//cout << "pH: "<<sampleCondition->pH << endl;///////////
				}
				else
				{
					if (temp == "temperature")
					{
						sampleCondition->temperature = atof(getToken_(_position_));
//cout << "temperature: "<< sampleCondition->temperature << endl;/////////
					}
					else 
					{
						if (temp == "pressure")
						{
							sampleCondition->pressure = atof(getToken_(_position_));
//cout << "pressure: " << sampleCondition->pressure << endl;////////////
						}
					}
				}

				readLine_();
			}

			sampleConditions_.push_back(sampleCondition);
		}

		test_(sampleConditions_.size() > 0,
					"no sample conditions could be found");
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

		test_(search_("	#  Chemical shift referencing  #"),
					"Chemical shift referencing could not be found");

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
//			Position pos = 0;
	//		Position _position_;
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
//cout << endl<< referencePositions[i] << " " << word << endl;
					switch (referencePositions[i])
					{
					case MOL_COMMON_NAME:
								shiftReferenceElement->molCommonName = word;
//cout <<"molCommonName " <<shiftReferenceElement->molCommonName << endl;
								break;
					case ATOM_TYPE:
								shiftReferenceElement->atomType = word[0];
//cout <<"atomType " <<shiftReferenceElement->atomType << endl;
								break;
					case ISOTOPE_NUMBER:
								shiftReferenceElement->isotopeNumber = atoi(word.c_str());
//cout <<"isotopeNumber " <<shiftReferenceElement->isotopeNumber << endl;
								break;
					case ATOM_GROUP:
								shiftReferenceElement->atomGroup = word;
//cout <<"atomGroup " <<shiftReferenceElement->atomGroup << endl;
								break;
					case SHIFT_UNITS:
								shiftReferenceElement->shiftUnits = word;
//cout <<"shiftUnits " <<shiftReferenceElement->shiftUnits << endl;
								break;
					case SHIFT_VALUE:
								shiftReferenceElement->shiftValue = atof(word.c_str());
//cout <<"shiftValue " <<shiftReferenceElement->shiftValue << endl;
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
//cout <<"referenceMethod " <<shiftReferenceElement->referenceMethod << endl;
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
//cout <<"referenceType " <<shiftReferenceElement->referenceType << endl;
								break;
					case INDIRECT_SHIFT_RATIO:
								shiftReferenceElement->indirectShiftRatio = atof(word.c_str());
//cout <<"indirectShiftRatio " <<shiftReferenceElement->indirectShiftRatio << endl;
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
						"no data for a shift references found");

			shiftReferences_.push_back(shiftReference);
			skipLines_(4); // ueberspringt save_
		}

		test_(shiftReferences_.size() > 0,	"no shift references could be found");
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
		catch (ReadFile_ProcessError)
		{
			cerr << "An error occured while reading shift data.";
			throw ReadFile_ProcessError();
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
//cout << "condition nr. " << i << endl;
				}
			}/*
cout << line_ << endl;		
skipLines_();
cout << line_ << endl;	*/	

			test_(search_("   _Chem_shift_reference_set_label", "   loop_"),
						"chemical shift does not contain _Chem_shift_reference_set_label");




			_position_ = 35;
			word = getToken_(_position_);
			word.erase(0, 1);
//cout << word << endl;		
			for (Position i = 0; i < shiftReferences_.size() ; i++)
			{
				if (shiftReferences_[i]->name == word)
				{
					atomDataSet->reference = shiftReferences_[i];
//cout << "reference nr. " << i << endl;
				}
			}


	/*		test_(search_("   _Mol_system_component_name"),
						"chemical shift does not contain _Mol_system_component_name");*/


			test_(search_("      _Chem_shift_ambiguity_code", "#"),
						"chemical shift does not contain _Chem_shift_ambiguity_code");

			skipLines_(1);
			while (strlen(line_) > 0)
			{
//cout << line_ << endl;

					atomDataSet->atomData.push_back(processShiftLine_());
//					cout << *atomDataSet->atomData.end() << endl;
			}

//cout << *atomDataSet;
			atomDataSets_.push_back(atomDataSet);
		}

//cout << "atomDataSets_.size " << atomDataSets_.size()<< endl;
		test_(atomDataSets_.size() == numberOfShifts,
					"wrong number of shift sets found");
//cout << atomDataSets_[0];
	}

	NMRStarFile::NMRStarFile(char* filename)
	{
		in.open(filename);

		test_(in,	"File could not be opend:");
		
		readEntryInformation_();


//cout << numberOfShifts << endl; //////////



		readMolSystem_();


//cout << systemName_ << endl; //////////



		readSampleConditions_();


/*
for (Position i = 0; i < sampleConditions_.size(); i++)
{
	cout << *sampleConditions_[i];
}*/



		readShiftReferences_();


/*
for (Position i = 0; i < shiftReferences_.size(); i++)
{
	cout << *shiftReferences_[i];
}*/

		readShifts_();
/*
for (Position i = 0; i < atomDataSets_.size(); i++)
{
	cout << *atomDataSets_[i];
}*/

	}

} // Namespace BALL

#endif // BALL_NMR_READ_STAR_H
