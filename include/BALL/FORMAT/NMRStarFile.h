#ifndef BALL_NMR_READ_STAR_H
#define BALL_NMR_READ_STAR_H

#ifndef BALL_FORMAT_READFILE_H
# include<BALL/FORMAT/readFile.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <vector>

namespace BALL 
{
	//using namespace std;

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
		String residueLabel;
		String atomName;
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

		String name;
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

		String molCommonName;
		char atomType;
		Position isotopeNumber;
		String atomGroup;
		String shiftUnits;
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
		String name;
		std::vector<ShiftReferenceElement*> elements;
	};

	struct NMRAtomDataSet
	{
		NMRAtomDataSet();

		String name;
		std::vector<NMRAtomData*> atomData;
		SampleCondition* condition;
		ShiftReferenceSet* reference;
	};

	std::ostream& operator << (std::ostream& s, const NMRAtomData& ad);
	std::ostream& operator << (std::ostream& s, const SampleCondition& sc);
	std::ostream& operator << (std::ostream& s, const ShiftReferenceElement& sre);
	std::ostream& operator << (std::ostream& s, const ShiftReferenceSet& sr);
	std::ostream& operator << (std::ostream& s, const NMRAtomDataSet& set);
	
	//@}

	/** @name	NMRStarFile.
			This class is designed to read a NMR-Star-File.
			To read a file simpy use the NMRStarFile(const String filename)- Konstruktor.
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
			NMRStarFile(const String filename);

			/** Get the maiximum number of atoms in all shift sets
			*/
			Size getNumberOfAtoms() const;

			/** Get the extracted data for the atoms.
			*/
			const std::vector<NMRAtomDataSet*>& getData() const;

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
			std::vector<NMRAtomDataSet*> atomDataSets_;

			/// the data for different sample sets
			std::vector<SampleCondition*> sampleConditions_;

			/// the data for shift references
			std::vector<ShiftReferenceSet*> shiftReferences_;

			/// contains Strings with the used reference options
			std::vector<String> referenceOptions_; // ??? soll static werden

			/// name of the molecular system
			String systemName_;

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

} // Namespace BALL

#endif // BALL_NMR_READ_STAR_H
