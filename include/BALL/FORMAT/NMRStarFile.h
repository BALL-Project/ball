// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_NMRSTARFILE_H
#define BALL_FORMAT_NMRSTARFILE_H

#ifndef BALL_FORMAT_CIFFILE_H
# include<BALL/FORMAT/CIFFile.h>
#endif

#ifndef BALL_COMMON_LIMITS_H
# include<BALL/COMMON/limits.h>
#endif

#include <vector>

namespace BALL 
{
	
	class BALL_EXPORT NMRStarFile
		: public CIFFile
	{
		public:
			BALL_CREATE(NMRStarFile)

			static const float FLOAT_VALUE_NA;
			static const Position POSITION_VALUE_NA;

			/** name nested Data classes.
				These classes represent the data extracted from NMR-Star-Files.
			*/
			//@{
		
			/**	NMRAtomData
			This class includes all information for one atom 
			delivered by a NMR-Star-File.
			All members are public for easy access.
			*/
			class BALL_EXPORT NMRAtomData
			{
				public:
					NMRAtomData();

					Position	atom_ID;
					Position	residue_seq_code;
					String		residue_label;
					String		atom_name;
					char			atom_type;
					float			shift_value;
					float			error_value;
					Position	ambiguity_code;

					std::ostream& operator >> (std::ostream& s)	throw();
			};

			/** SampleConditions.
				This class includes all information for a set of samples
				delivered by a NMR-Star-File.
			*/
			class BALL_EXPORT SampleCondition
			{
				public:
					SampleCondition();
	
					String	name;
					float		temperature;
					float		pH;
					float		pressure;
	
					std::ostream& operator >> (std::ostream& s)	throw();
			};

			/** ShiftReferenceElement.
				This class includes all information for a shift reference element
				for one atom type delivered by a NMR-Star-File.
			*/
			class BALL_EXPORT ShiftReferenceElement
			{
				public:
					ShiftReferenceElement();

					String		mol_common_name;
					char			atom_type;
					Position	isotope_number;
					String		atom_group;
					String		shift_units;
					float			shift_value;
					char			reference_method;
					char			reference_type;
					float			indirect_shift_ratio;

					std::ostream& operator >> (std::ostream& s) throw();
			};

			/** ShiftReferenceSet.
				This class includes all information for all sets of shift references
				for several atom types delivered by a NMR-Star-File.
			*/
			class BALL_EXPORT ShiftReferenceSet
			{
				public:	
					ShiftReferenceSet();
				
					String name;
					std::vector<ShiftReferenceElement> elements;

					std::ostream& operator >> (std::ostream& s);
			};
			
			/** NMRAtomDataSet
				This class ... TODO!!
			*/
			class BALL_EXPORT NMRAtomDataSet
			{
				public:
					NMRAtomDataSet();

					String										name;
					std::vector<NMRAtomData> 	atom_data;
					SampleCondition						condition;
					ShiftReferenceSet					reference;

					std::ostream& operator >> (std::ostream& s);
			};

			/** EntryInformation
				This class ... TODO!!
			*/
			class BALL_EXPORT EntryInformation
			{
				public:
					EntryInformation();
					~EntryInformation();
					std::ostream& operator >> (std::ostream& s);

					String entry_type;
					String BMRB_accession_code;
					String NMR_STAR_version;
					String experimental_method;
			};
	
			//@}
			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor
			*/
			NMRStarFile();
					Then the file of f will be openeed.
			
			/** Detailed constuctor.
					Opens the given file and extracts all usefull data.
			*/
			NMRStarFile(const String& file_name, File::OpenMode open_mode = std::ios::in)
				throw(Exception::FileNotFound);

			~NMRStarFile();

			/** Assignment operator.
					@see NMRStarFile(const NMRStarFile& f)
			*/
			const NMRStarFile& operator = (const NMRStarFile& f);
			
			/** Read a NMRStarFile.
			 */
			bool read()
				throw(Exception::ParseError);

			/** Clear the object.
			*/
			void clear();

			//@}


			/**	@name	Access methods
			*/
			//@{

			/** Get the maiximum number of atoms in all shift sets
			*/
			Size getNumberOfAtoms()  // TODO!!!  

			/** Get the extracted data for the atoms.
			*/
			const std::vector<NMRAtomDataSet>& getData() const;

			const EntryInformation& getEntryInformation() const {return entry_information_;};

			/** Get the saveframe category within datablock. 
			  	The datablockenumeration starts at 0!
			 */
		//	SaveFrame& getSaveframe(String category, Position db_pos = 0);
		//	const SaveFrame& getSaveframe(String category, Position db_pos = 0) const;

			/** Get the Item with coloumnname name within saveframe 
			 */
	//		const CIFFile::Item* hasColumn(const SaveFrame* sf, String name);
	// TODO ;; bool hasColumn(const SaveFrame* sf, String name);

	//@}
			/**	@name	Equality
			 */
			//@{

			/** Equality operator.
				Test if both instances point to the same file.
			 */
			bool operator == (const NMRStarFile& f);

			/** Inequality operator
				Test if both instances point to different files.
			 */
			bool operator != (const NMRStarFile& f);

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
			//		NMRAtomData processShiftLine_()
			//			throw(Exception::ParseError);

			/// reads the number of chemical shifts
			void readEntryInformation_()
				throw(Exception::ParseError, Exception::InvalidFormat);

			/// reads the molecular system name
			void readMolSystem_();

			/// reads the sample conditions
			void readSampleConditions_();

			/// reads the shift references
			void readShiftReferences_();

			/// reads the shift datas
			void readShifts_()
				throw (Exception::ParseError);

			/// initialize the referenceOptions
			static void initializeReferenceOptions_();

			//_@}
			/*_	@name	NMR-Star specific attributes
			*/
			//_@{

			/// the number of shift data -> todo: not sure what is ment with this! 
			Size number_of_shifts_;
 
			//
			EntryInformation entry_information_;

			/// the data for the atoms is stored here
			std::vector<NMRAtomDataSet> atom_data_sets_;

			/// the data for different sample sets
			std::vector<SampleCondition> sample_conditions_;

			/// the data for shift references
			std::vector<ShiftReferenceSet> shift_references_;

			/// name of the molecular system
			String system_name_;

			/// contains Strings with the used reference options
			static std::vector<String> reference_options_;
	
			// a dummy saveframe
			SaveFrame dummy_saveframe_; // TODO : add to clear... Constructor..

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

#endif // BALL_FORMAT_NMRSTARFILE_H
