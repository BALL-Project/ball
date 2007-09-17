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

			// constant variables to denote unknown values
			static const float FLOAT_VALUE_NA;
			static const int INT_VALUE_NA;

			static const Position POSITION_VALUE_NA;
			
			/** name nested Data classes.
				These classes represent the data extracted from NMR-Star-Files.
			*/
			
			//@{

		/** SampleConditions.
				This class includes all information for a set of samples
				delivered by a NMR-Star-File.
			*/
			class BALL_EXPORT SampleCondition
			{
				public:
					SampleCondition();
	
					// saveframe name -- referenced in the sample_condition_label 
					// of saveFrame assigned_chemical_shifts   
					String	name;
					// Accoring to the NMRStarFile 2.1 documentation 
					// the first entry MUST BE "_Variable_type".
					// This is why we are allowed to map per type :-)
					vector<String> types;
					StringHashMap<float>  values;
					StringHashMap<float>  errors;
					StringHashMap<String> units;
					
					std::ostream& operator >> (std::ostream& s);
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

					std::ostream& operator >> (std::ostream& s);
			};

			/** ShiftReferenceSet.
				This class includes all information for all sets of shift references
				for several atom types delivered by a NMR-Star-File.
			*/
			class BALL_EXPORT ShiftReferenceSet
			{
				public:	
					ShiftReferenceSet();
				
					// The saveFrames name = set name
					// can be referenced in SaveFrame assigned_chemical_shifts
					String name;  
					std::vector<ShiftReferenceElement> elements;

					std::ostream& operator >> (std::ostream& s);
			};

			/**	NMRAtomData
					This class includes all information for one atom 
					delivered by a NMR-Star-File.
					All members are public for easy access.
			*/
			class NMRAtomData
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

					std::ostream& operator >> (std::ostream& s);
			};

						
			/** NMRAtomDataSet
				This class includes all NMR information concerning a 
				dataset of atoms delivered by a NMR-Star-File.
				All members are public for easy access.
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
				This class includes all "entry" information of
				a NMRStar-File datablock.
				All members are public for easy access.
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
	/** Monomeric Polymer
			 * 	This class includes all Monomeric Polymers occuring in this file.
			 * 	All members are public for easy access.
			 */
			class MonomericPolymer
			{
				public:
					/// Database with homolog entry
					class HomologDB 
					{
						public:
							HomologDB();
							std::ostream& operator >> (std::ostream& s); 
							void clear(); 

							String name;
							String accession_code;
							String entry_mol_name;
							float  seq_to_submitted_percentage;
							float  subject_length;
							float  seq_identity;
							float  seq_positive;
							float  homology_expectation_value;  
					};


						MonomericPolymer(); 
						std::ostream& operator >> (std::ostream& s); 
						void clear(); 

						String label_name;
						String type;
						String polymer_class;
						String common_name;
						String name_variant;
						float  molecular_mass;
						String details;  	
						//	polymer residue sequence information
						int number_of_residues;
						String residue_sequence; 
						// we want to allow things like resid 137A, so we cannot use Index
						// key: index -- value: aminoacidname
						StringHashMap<String> residues_by_index;

						vector<HomologDB> homolog_database_entries; 
			};


			/** Molecular System
			 		This class includes all system relevant information.
					All members are public for easy access.
			 */
			class MolecularSystem
			{	
				/// System related information
				public:
					class RelatedDB
					{
						public:
							RelatedDB();
							std::ostream& operator >> (std::ostream& s); 
							void clear(); 

							String name;
							String accession_code;
							String entry_mol_name;
							String relation_type;
							String details;
					};
					
													
					/// Central class for convenience
					class ChemicalUnit
					{
						public:
							ChemicalUnit();  
							std::ostream& operator >> (std::ostream& s);  //TODO check
							void clear();  

							String component_name;
							String label;
							NMRStarFile::MonomericPolymer* monomeric_polymer; 
							NMRAtomDataSet* shifts;             
					};


					MolecularSystem(); 
					~MolecularSystem(); 

									/// Name of the molecular system
					String system_name;
					String abbreviation_common;
					// first: Molecular system component name, 
					// second: label
					//vector < std::pair<String, String> > chemical_units; // TODO!! 
					vector<ChemicalUnit> chemical_units; // TODO!! 
					String system_physical_state;
					String system_oligomer_state;
					String system_paramagnetic;
					String system_thiol_state;
					/// The systems molecular weigth in dalton
					float system_molecular_weight;	

					// related entries in various DB's
					vector<RelatedDB> related_database_entries;
					
					/// ligand information
					
					std::ostream& operator >> (std::ostream& s); 
					void clear(); 
			};


			/** NMRSpectrometer
			 * 	This class includes all NMR-Spectrometer information.
			 * 	All members are public for easy access.
			 */
			class NMRSpectrometer
			{
				public:
					String manufacturer;
					String model;
					float  field_strength;
					std::ostream& operator >> (std::ostream& s);
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
			Size getNumberOfAtoms()    

			/** Get the extracted data for the atoms.
			*/
			const std::vector<NMRAtomDataSet>& getNMRData()	const;

			const EntryInformation& getEntryInformation() const {return entry_information_;};

			const MolecularSystem& getMolecularInformation() const {return molecular_system_;};

			/// Get a SampleCondition by its SaveFrame name
			SampleCondition& getSampleConditionByName(String name);
			/// Get a SampleCondition by its SaveFrame name 
			const SampleCondition& getSampleConditionByName(String name) const;
			/// Get the i-th  SampleCondition 
			SampleCondition& getSampleCondition(Position i) {return sample_conditions_[i];};
			/// Get the i-th  SampleCondition 
			const SampleCondition& getSampleCondition(Position i) const {return sample_conditions_[i];};
			/// Get the number of sample conditions
			Size getNumberOfSampleConditions() const {return sample_conditions_.size();};
			/// Get the sample conditions
			const std::vector<SampleCondition>& getSampleConditions() const {return sample_conditions_;};
			/// Get the sample conditions
			std::vector<SampleCondition>& getSampleConditions() {return sample_conditions_;};

			// addSampleCondition TODO!!
			
			/// Get the shift reference sets
			std::vector<ShiftReferenceSet>& getShiftReferenceSets() {return shift_references_;}; 
			const std::vector<ShiftReferenceSet>& getShiftReferenceSets() const  {return shift_references_;}; 

			/// Get the number of shift reference sets
			Size getNumberOfShiftReferenceSets() const  {return shift_references_.size();};

			/// Get the i-th shift reference set
			const ShiftReferenceSet& getShiftReferenceSet(Position i) const {return shift_references_[i];}; 
			/// Get the i-th shift reference set
			ShiftReferenceSet& getShiftReferenceSet(Position i) {return shift_references_[i];}; 

			/// Get a ShiftReferenceSet by its SaveFrame name
			const ShiftReferenceSet& getShiftReferenceSetByName(String name) const;
			ShiftReferenceSet& getShiftReferenceSetByName(String name);
			NMRSpectrometer& getNMRSpectrometer() {return nmr_spectrometer_;};
			const NMRSpectrometer& getNMRSpectrometer() const {return nmr_spectrometer_;};
			
			String getNMRSpectrometerManufacturer() {return  nmr_spectrometer_.manufacturer;};
			float getNMRSpectrometerFieldStrength() {return nmr_spectrometer_.field_strength;};
		
			/// Get the MonomericPolymer-information of name.
			/// Throws exception IllegalKey 
			Size getNumberOfMonomericPolymers() const {return monomeric_polymers_.size();};

			NMRStarFile::MonomericPolymer getMonomericPolymer(String name) { return monomeric_polymers_[monomeric_polymer_indices_[name]];}
		
			// Get all Monomeric Polymers
			vector<NMRStarFile::MonomericPolymer> getMonomericPolymers() const {return monomeric_polymers_;};
			
			//TODO:
			//const vector<NMRStarFile::MonomericPolymer> & getMonomericPolymers() const {return monomeric_polymers_;};


			//TODO: sollte wir nicht auch einen  oder gibt das Krach mit vetor und StringHashmap?	
			//NMRStarFile::MonomericPolymer& getMonomericPolymer(String name) { return monomeric_polymers_[name];}

			bool hasMonomericPolymer(String name) { return monomeric_polymer_indices_.has(name);}; 
		
			/// returns true if a monomer with name chemical_unit_label exists,
			/// false otherwise
			bool isMonomericPolymer(String chemical_unit_label); 

			//TODO: Änderungen/Hinzufügungen als Saveframes auch im CIFFile vermerken!
			/// adds a Monomeric Polymer
			/// if a poymer with the same name already exists, it will be overwritten! 
			void addMonomericPolymer(NMRStarFile::MonomericPolymer mp);

			
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


			//@}

		private:

			/*_	@name	NMR-Star specific Help-Methods
			 */
			//_@{

	
			/// reads the number of chemical shifts
			void readEntryInformation_();

			/// reads the molecular system name
			void readMolSystem_();
			
			/// read the MonomericPolymers
			void readMonomericPolymers_();
			
			/// reads the sample conditions
			void readSampleConditions_();

			/// reads the shift references
			void readShiftReferences_();

			/// reads the shift datas
			void readShifts_();
			
			/// reads the NMR spectrometer data
			void readNMRSpectrometer_();
			
			/// find dependencies for the ChemicalUnits
			void findDependiencies_();

			//_@}
			/*_	@name	NMR-Star specific attributes
			*/
			//_@{

			/// the number of shift data sets  
			Size number_of_shift_sets_; 

			/// the general entry data 
			EntryInformation entry_information_;
			
			/// the system information
			MolecularSystem molecular_system_;

			/// the data for the atoms is stored here
			std::vector<NMRAtomDataSet> atom_data_sets_;

			/// the data for different sample sets
			std::vector<SampleCondition> sample_conditions_;

			/// the data for shift references
			std::vector<ShiftReferenceSet> shift_references_;

			/// the data of nmr spectrometer 
			NMRSpectrometer nmr_spectrometer_; 

			/// Monomeric Polymer information 
			// stored per label_name
			// a MonomericPolymer can be referenced in chemical_units
			StringHashMap< Index> monomeric_polymer_indices_; //TODO >>!
			vector<MonomericPolymer> monomeric_polymers_; //TODO: setzen
			/// name of the molecular system
		//	String system_name_;  // TODO wo kommt der her?

			// a dummy saveframe
			SaveFrame dummy_saveframe_; 
			
			SampleCondition dummy_sample_condition_;
			
			ShiftReferenceSet dummy_shift_reference_set_; 

			//_@}
	};

	//@}
} // Namespace BALL

#endif // BALL_FORMAT_NMRSTARFILE_H
