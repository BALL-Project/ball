// $Id: NMRStarFile.h,v 1.14 2001/06/21 22:07:50 amoll Exp $

#ifndef BALL_NMR_READ_STAR_H
#define BALL_NMR_READ_STAR_H

#ifndef BALL_FORMAT_LineBasedFile_H
# include<BALL/FORMAT/lineBasedFile.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
# include <BALL/DATATYPE/string.h>
#endif

#include <vector>

namespace BALL 
{
	/** @name Data classes and structures.
			These classes represent the data extracted from NMR-Star-Files.
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	//@{

	/**	NMRAtomData
			This structure includes all information for one atom 
			delivered by a NMR-Star-File.
			All members are public for easy access.
	*/
	struct NMRAtomData
	{
		Position	atom_ID;
		Position	residue_seq_code;
		String		residue_label;
		String		atom_name;
		char			atom_type;
		float			shift_value;
		float			error_value;
		Position	ambiguity_code;
	};

	/** SampleConditions.
			This structure includes all information for a set of samples
			delivered by a NMR-Star-File.
			All members are public for easy access.
	*/
	struct SampleCondition
	{
		SampleCondition() throw();

		String	name;
		float		temperature;
		float		pH;
		float		pressure;
	};

	/** ShiftReferenceElement.
			This structure includes all information for a shift reference element
			for one atom type delivered by a NMR-Star-File.
			All members are public for easy access.
	*/
	struct ShiftReferenceElement
	{
		ShiftReferenceElement() throw();

		String		mol_common_name;
		char			atom_type;
		Position	isotope_number;
		String		atom_group;
		String		shift_units;
		float			shift_value;
		char			reference_method;
		char			reference_type;
		float			indirect_shift_ratio;
	};

	/** ShiftReferenceSet.
			This structure includes all information for all sets of shift references
			for several atom types delivered by a NMR-Star-File.
			All members are public for easy access.
	*/
	struct ShiftReferenceSet
	{
		String name;
		std::vector<ShiftReferenceElement*> elements;
	};

	struct NMRAtomDataSet
	{
		NMRAtomDataSet() throw();

		String										name;
		std::vector<NMRAtomData*> atom_data;
		SampleCondition*					condition;
		ShiftReferenceSet*				reference;
	};

	std::ostream& operator << (std::ostream& s, const NMRAtomData&						ad)	throw();
	std::ostream& operator << (std::ostream& s, const SampleCondition&				sc)	throw();
	std::ostream& operator << (std::ostream& s, const ShiftReferenceElement& sre) throw();
	std::ostream& operator << (std::ostream& s, const ShiftReferenceSet&			sr)	throw();
	std::ostream& operator << (std::ostream& s, const NMRAtomDataSet&				 set)	throw();
	
	//@}

	/** NMRStarFile.
			This class is designed to read a NMR-Star-File.
			To read a file simpy use the NMRStarFile(char* filename)- Constructor.
			All useful data are extracted and stored.
			To get access to the data use getData().
			{\bf Definition:} \URL{BALL/NMR/NMRStarFile.h}
	*/
	//@{
	class NMRStarFile
		: public LineBasedFile
	{
		public:

			BALL_CREATE(NMRStarFile)

			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor
			*/
			NMRStarFile()
				throw();
			
			/** Copy constructor.
					If the file was open, it is closed.
					Then the file of f will be opend.
					All data already extracted in f is copied to this instance.
			*/
			NMRStarFile(const NMRStarFile& f)
				throw();

			/** Detailled constuctor.
					Opens the given file and extracts all usefull data.
			*/
			NMRStarFile(const String& file_name)
				throw(Exception::FileNotFound);

			/** Assignment operator.
					@see NMRStarFile(const NMRStarFile& f)
			*/
			const NMRStarFile& operator = (const NMRStarFile& f)  throw();

			/** Clear the object.
			*/
			void clear() throw();

			//@}
			/**	@name	Access methods
			*/
			//@{

			/** Get the maiximum number of atoms in all shift sets
			*/
			Size getNumberOfAtoms() 
				const throw();

			/** Get the extracted data for the atoms.
			*/
			const std::vector<NMRAtomDataSet*>& getData()
				const throw();

			//@}
			/**	@name	Equality
			*/
			//@{

			/** Equality operator.
					Test if both instances point to the same file.
			*/
			bool operator == (const NMRStarFile& f)  throw();

			/** Inequality operator
					Test if both instances point to different files.
			*/
			bool operator != (const NMRStarFile& f)  throw();

			//@}
			/**	@name	Enums
			*/
			//@{

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

			//@}

		private:

			/*_	@name	NMR-Star specific Help-Methods
			*/
			//_@{

			/// function to extract the data from a chemical shift line
			NMRAtomData* processShiftLine_()
				throw(LineBasedFile::LineBasedFileError);

			/// reads the number of chemical shifts
			void readEntryInformation_()
				throw(LineBasedFile::LineBasedFileError, Exception::InvalidFormat);

			/// reads the molecular system name
			void readMolSystem_()
				throw();

			/// reads the sample conditions
			void readSampleConditions_()
				throw();

			/// reads the shift references
			void readShiftReferences_()
				throw();

			/// reads the shift datas
			void readShifts_()
				throw (LineBasedFile::LineBasedFileError);

			/// initialize the referenceOptions
			static void initializeReferenceOptions_()
				throw ();

			//_@}
			/*_	@name	NMR-Star specific attributes
			*/
			//_@{

			/// the number of shift data
			Size number_of_shifts_;

			/// the data for the atoms is stored here
			std::vector<NMRAtomDataSet*> atom_data_sets_;

			/// the data for different sample sets
			std::vector<SampleCondition*> sample_conditions_;

			/// the data for shift references
			std::vector<ShiftReferenceSet*> shift_references_;

			/// name of the molecular system
			String system_name_;

			/// contains Strings with the used reference options
			static std::vector<String> reference_options_;

			/// internal enumeration used in readShiftReferences_()
			enum ShiftReferenceTypes_
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

	};

	//@}

} // Namespace BALL

#endif // BALL_NMR_READ_STAR_H
