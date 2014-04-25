// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_FORMAT_NMRSTARFILE_H
#define BALL_FORMAT_NMRSTARFILE_H

#ifndef BALL_FORMAT_CIFFILE_H
# include <BALL/FORMAT/CIFFile.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
# include <BALL/KERNEL/protein.h>
#endif

#ifndef BALL_STRUCTURE_PEPTIDES_H
# include <BALL/STRUCTURE/peptides.h>
#endif

#include <vector>

namespace BALL
{
	/** NMRStarFile class. 
	    
			\brief This class provides methods for reading and assigning chemical shifts.
		 
			This class provides methods for reading and assigning experimental 
			chemical shifts from NMRStar files to peptides. 
			The nested class BALLToBMRBMapper creates a mapping between NMRStar file atoms and
		 	atoms of a chain either based on a trivial mapping @see createTrivialMapping()
			or based on a given alignment @see createMapping().
			Access to information other than the shifts is provided as well.
		
	    <br>
	    Example code: <br> 
	 	  \code  
	 		  System system;
				pdb_file >> system;
				Chain& chain = *(system.beginChain());
				
				NMRStarFile nmr_file("NMRStarFile_test.bmr");
				cout << "File contains "      << nmr_file.getNumberOfAtoms()   << " chemical shifts" << endl;
				cout << "Number of samples: " << nmr_file.getNumberOfSamples() << endl;
				
				NMRStarFile::MolecularSystem nmr_mol_sys = nmr_file.getMolecularInformation();
				cout << "Molecular systems name: " <<  nmr_mol_sys.system_name << endl;
				cout << "Number of possible chemical units: " <<  nmr_mol_sys.getNumberOfChemicalUnits() << endl;
	      
	      // the chemical units of this file
				for (Size k=0; k <  nmr_mol_sys.getNumberOfChemicalUnits(); k++)
				{
				   NMRStarFile::MolecularSystem::ChemicalUnit cu = nmr_mol_sys.getChemicalUnit(k);
				   cout << " " << k << " : " <<  cu.component_name 
				        << " " <<  cu.label << endl;
				}
				
	      // get the shift data		
				// NOTE: nmr_file.atom_data_sets_ equals  nmr_data !
				std::vector<NMRStarFile::NMRAtomDataSet> const& nmr_data = nmr_file.getNMRData();
				cout << "Number of shift sets: " <<  nmr_data.size() << endl;
				
				for (Size k=0; k < nmr_data.size(); k++)
				{
				   cout << " " << k << " shift set name: " << nmr_data[k].name << "   -- condition name: " << nmr_data[k].condition << endl;
				}
				
				// decide for a chemical unit	
				NMRStarFile::MolecularSystem::ChemicalUnit cu = nmr_mol_sys.getChemicalUnit(0);
				String chemical_unit_label = cu.label;
				cout << Peptides::GetSequence(chain)  << endl;
				cout << nmr_file.getResidueSequence() << endl;
        
	      // create a mapping based on an alignment
				cout << "Create a mapping... " ;
				NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file, chemical_unit_label);
				mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", 
				                     "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT");
				
		    cout << "Number of mismatches: " << mapper.getNumberOfMismatches() << endl;
				cout << "Number of gaps: "       << mapper.getNumberOfGaps() << endl;
	      
	      // assign the shifts
				nmr_file.assignShifts(mapper);
			  cout << "Assigned "      <<	nmr_file.getNumberOfShiftsAssigned() 
				     << " shifts given " <<	nmr_file.getNumberOfAtoms() << " chemical shifts." << endl << endl;

		 		...
				   cout << atom->getProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT).getFloat() << endl;
			  ...

				// get the ChemicalUnit by label
				NMRStarFile::MolecularSystem::ChemicalUnit chemical_unit = getChemicalUnitByLabel(chemical_unit_label); 
				
				// find the corresponding SampleCondition
				if (chemical_unit.shifts && nmr_file.hasSampleCondition(chemical_unit.shifts->condition))
				{
					NMRStarFile::SampleCondition& condition = nmr_file.getSampleConditionByName(chemical_unit.shifts->condition);			
					if (condition.hasType("pH"))
						pH = condition.values["pH"];  
					if (condition.hasType("pH*"))
						pH = condition.values["pH*"]; 
					if (condition.hasType("temperature"))
						temperature  = condition.values["temperature"];
					if (condition.hasType("pressure"))
						pressure  = condition.values["pressure"];
				
					cout << "NMR conditions: " << pH << ", " << temperature << ", " << pressure << endl; 
				}

	    \endcode
	*/
	class BALL_EXPORT NMRStarFile
		: public CIFFile
	{
		public:

			// constant variables to denote unknown values
			static const float    FLOAT_VALUE_NA;
			static const int      INT_VALUE_NA;

			static const Position POSITION_VALUE_NA;

			/** @name Nested classes
				  These classes represent the data extracted from NMRStar files.
			*/

			//@{

			/** SampleConditions.
			 	  This class includes all information for a set of samples
				  delivered by a NMRStar file.
				  All members are public for easy access.
			*/
			class BALL_EXPORT SampleCondition
			{
				public:
					SampleCondition();

					// saveframe name -- referenced in the sample_condition_label 
					// of  the saveframe "assigned_chemical_shifts"   
					String                name;
					// Accoring to the NMRStarFile 2.1 documentation 
					// the first entry MUST BE "_Variable_type".
					// This is why we are allowed to map per type :-)
					vector<String>         types;
					StringHashMap<float>   values;
					StringHashMap<float>   errors;
					StringHashMap<String>  units;

					bool hasType(String type) {return values.has(type);}
					std::ostream& operator >> (std::ostream& s);
			};
	
			/** Samples.
					This class includes all information of a NMRStar file sample.
			*/
			class BALL_EXPORT Sample
			{
				public:
				
					/** Component.
					    This class includes all information of a sample component.
				  	  All members are public for easy access.
					*/
					class BALL_EXPORT Component
					{
						public:
							Component();
							void clear();

							String   label;
							float    concentration_value;
							String   value_unit;
							float    concentration_min;
							float    concentration_max;
							String   isotopic_labeling;

							std::ostream& operator >> (std::ostream& s);
					};

					Sample();
					void clear();

					String label;
					String type;
					String details;
					vector <Component> components;

					std::ostream& operator >> (std::ostream& s);
			};

			/** ShiftReferenceElement.
			  	This class includes all information for a shift reference element
				  for one atom type delivered by a NMRStar file.
				  All members are public for easy access.
			*/
			class BALL_EXPORT ShiftReferenceElement
			{
				public:
					ShiftReferenceElement();

					String      mol_common_name;
					String      atom_type;
					Position    isotope_number;
					String      atom_group;
					String      shift_units;
					float       shift_value;
					String      reference_method;
					String      reference_type;
					float       indirect_shift_ratio;

					std::ostream& operator >> (std::ostream& s);
			};

			/** ShiftReferenceSet.
				  This class includes all information for all sets of shift references
				  for several atom types delivered by a NMRStar file.
				  All members are public for easy access.
			*/
			class BALL_EXPORT ShiftReferenceSet
			{
				public:
					ShiftReferenceSet();
				
					// The saveframe's name = set name
					// can be referenced in the saveframe "assigned_chemical_shifts"
					String                             name;
					std::vector<ShiftReferenceElement> elements;

					std::ostream& operator >> (std::ostream& s);
			};

			/**	NMRAtomData
					This class includes all information for one atom 
					delivered by a NMRStar file.
					All members are public for easy access.
			*/
			class BALL_EXPORT NMRAtomData
			{
				public:
					NMRAtomData();

					Position   atom_ID;
					Position   residue_seq_code;
					String     residue_label;
					String     atom_name;
					char       atom_type;
					float      shift_value;
					float      error_value;
					Position   ambiguity_code;

					bool operator == (const NMRAtomData& atom) const;
					std::ostream& operator >> (std::ostream& s);
			};


			/** NMRAtomDataSet
				  This class includes all NMR information concerning a 
				  dataset of atoms delivered by a NMRStar file.
					The corresponding condition, reference set, and samples can be looked 
				  up with getSampleConditionByName(), 
				          getShiftReferenceSetByName(), and 
						  		getSample(). 
				  All members are public for easy access.
			*/
			class BALL_EXPORT NMRAtomDataSet // _Saveframe_category  assigned_chemical_shifts

			{
				public:
					NMRAtomDataSet(NMRStarFile* parent);

					String                     name;
					String                     label;
					std::vector<NMRAtomData>   atom_data;
					String                     condition;
					String                     reference;
					std::vector<String>        samples;

					std::ostream& operator >> (std::ostream& s);

				protected:
					NMRStarFile*                    parent_;
			};

			/** EntryInformation
				 This class includes all "entry" information of
				 a NMRStar file datablock.
				 All members are public for easy access.
			*/
			class BALL_EXPORT EntryInformation
			{
				public:
					EntryInformation();
					~EntryInformation();

					std::ostream& operator >> (std::ostream& s);
					void clear();

					String   entry_type;
					String   BMRB_accession_code;
					String   NMR_STAR_version;
					String   experimental_method;
					String   submission_date;

			};

			/** Monomeric Polymer
			 * 	This class includes all Monomeric Polymers occuring in this file.
			 * 	All members are public for easy access.
			 */
			class BALL_EXPORT MonomericPolymer
			{
				public:
					/// Database with homolog entries
					class BALL_EXPORT HomologDB
					{
						public:
							HomologDB();

							std::ostream& operator >> (std::ostream& s);
							void clear();

							String  name;
							String  accession_code;
							String  entry_mol_name;
							float   seq_to_submitted_percentage;
							float   subject_length;
							float   seq_identity;
							float   seq_positive;
							float   homology_expectation_value;
					};


					MonomericPolymer();

					String   label_name;
					String   type;
					String   polymer_class;
					String   common_name;
					String   name_variant;
					float    molecular_mass;
					String   details;
					//	polymer residue sequence information
					int      number_of_residues;
					String   residue_sequence;
					// we want to allow things like resid 137A, so we cannot use Index
					// key: index -- value: aminoacidname
					StringHashMap<String>   residues_by_index;
					vector<HomologDB>       homolog_database_entries;

					std::ostream& operator >> (std::ostream& s);
					void clear();
			};


			/** Molecular System
			 		This class includes all system relevant information.
					All members are public for easy access.
			 */
			class BALL_EXPORT MolecularSystem
			{
				// System related information
				public:
					class BALL_EXPORT RelatedDB
					{
						public:
							RelatedDB();

							std::ostream& operator >> (std::ostream& s);
							void clear();

							String    name;
							String    accession_code;
							String    entry_mol_name;
							String    relation_type;
							String    details;
					};


					// Central class for convenience
					class BALL_EXPORT ChemicalUnit
					{
						public:
							ChemicalUnit();
							std::ostream& operator >> (std::ostream& s);
							void clear();

							String            component_name;
							String            label;
							MonomericPolymer* monomeric_polymer;
							NMRAtomDataSet*   shifts;
					};


					MolecularSystem();
					~MolecularSystem();

					ChemicalUnit const&  getChemicalUnit(Position i) const { return chemical_units[i]; }
					ChemicalUnit&  getChemicalUnit(Position i) { return chemical_units[i]; }

					Size getNumberOfChemicalUnits() const {return chemical_units.size(); }

					// Name of the molecular system
					String                system_name;
					String                abbreviation_common;
					vector<ChemicalUnit>  chemical_units;
					String                system_physical_state;
					String                system_oligomer_state;
					String                system_paramagnetic;
					String                system_thiol_state;
					/// The systems molecular weigth in dalton
					float                 system_molecular_weight;
					// related entries in various DB's
					vector<RelatedDB>     related_database_entries;

					/// ligand information //TODO

					std::ostream& operator >> (std::ostream& s);
					void clear();
			};


			/** NMRSpectrometer
			 * 	This class includes all NMR spectrometer information.
			 * 	All members are public for easy access.
			 */
			class BALL_EXPORT NMRSpectrometer
			{
				public:
					String    name;
					String    manufacturer;
					String    model;
					float     field_strength;

					std::ostream& operator >> (std::ostream& s);
			};


			/** Mapper class between BALL atoms and NMRStar file atom entries.
			 *  
			 *  The main reason for the existence of this class (instead of maps to atompointer,position,position)
			 *  is the python interface, which is greatly simplified.
			 */
			class BALL_EXPORT BALLToBMRBMapper
			{
				public:

					/** @name Type definitions 
          */
          //@{
					/** BMRMIndex stores the savaframe id and 
					    the index of the atom in the NMRAtomDataSet. 	
					*/
					//<saveframe_id, atom_id_in_nmr_atom_data_set>
					typedef std::pair<Position, Position>             BMRBIndex;
					typedef std::map<Atom const* , BMRBIndex>         BALLToBMRBMapping;
					typedef std::map<const NMRAtomData*, Atom const*> BMRBToBALLMapping;
					//TODO: Dont use pointer but something more sophisticated!

					//@}

					/**	@name	Constructors and Destructors 
					*/
					//@{
					BALLToBMRBMapper();

					/** Detailed constructor
					
					    @param  chain            the chain to be mapped to a shift reference set 
					    @param  nmr_data         the NMRStarFile provinding the chemical shifts for chain
				 	    @param  chemical_unit    the name of the molecular system (chemical unit) storing the atom shifts 
					*/
					BALLToBMRBMapper(Chain const& chain, const NMRStarFile& nmr_data, const String& chemical_unit);

					/// Destructor
					virtual ~BALLToBMRBMapper() {}

					//@}
					/**	@name	Access methods
					*/
					//@{

					/// Get the chain
					const Chain* getChain() const {return chain_;}

					/// Set the chain
					void setChain(Chain const& chain) { chain_ = &chain;
					                                    num_mismatches_ = -1;
					                                    num_gaps_ = -1;}

					/// Set the NMRStar file
					void setNMRStarFile(NMRStarFile const& nmrfile) {nmr_data_ = &nmrfile;
					                                                 num_mismatches_ = -1;
					                                                 num_gaps_ = -1;}

					/// Get the NMRStar file
					const NMRStarFile* getNMRStarFile() const {return nmr_data_;}

					/// Set the NMRAtomDataSet
					void setNMRAtomDataSet(NMRAtomDataSet const& nmr_atom_data_set){nmr_atom_data_set_= &nmr_atom_data_set;}

					/// Set the NMRAtomDataSet by chemical unit name
					bool setNMRAtomDataSetByName(String const& chemical_unit_name);

					// Get the NMRAtomDataSet 
					const NMRAtomDataSet* getNMRAtomDataSet() const{return nmr_atom_data_set_;}

					/// Return the mapping	
					BALLToBMRBMapping& getBALLToBMRBMapping() {return ball_to_bmrb_map_;}

					/// Return the mapping	
					const BALLToBMRBMapping& getBALLToBMRBMapping() const {return ball_to_bmrb_map_;}

					/// Return the mapping	
					BMRBToBALLMapping& getBMRBToBALLMapping() {return bmrb_to_ball_map_;}

					/// Return the mapping	
					const BMRBToBALLMapping& getBMRBToBALLMapping() const {return bmrb_to_ball_map_;}

					/// Return the number of mismatches in the current mapping
					int getNumberOfMismatches(){return num_mismatches_;}

					/// Return the number of mismatches in the current mapping
					int getNumberOfGaps(){return num_gaps_;}

					/// Test whether the given nmr atom data is mapped to a structure atom 
					bool isMapped(const NMRAtomData& nmr_atom) const;

					/** Return the atom mapped to the given NMRAtom.

						  @param  atom the NMRAtom 
						  @return the mapped BALL atom. If no atom can be matched a NULL pointer is returned.
					*/
					const Atom* getBALLAtom(const NMRAtomData& nmr_atom) const;

					/** Test, whether an atom is matched to NMRAtom data entry

						 @return true if the given atom is mapped to an atom in the NMRStar file,else otherwise
					*/
					bool isMapped(Atom const* atom) const;

					///	Return the mapping of the given atom
					BMRBIndex operator () (const Atom* atom);

					/** Create a trivial mapping between the given chain and the NMRStar file atoms.
					 		
						NOTE: this mapping only works for very simple cases as we assume no gaps.

						@return bool - <tt>true</tt> if creating the mapping was possible 
					*/
					bool createTrivialMapping();

					/** Create a mapping between the given chain and the molecular system named chemical_unit 
					    of the given NMRStar file atoms based on the given alignment.
					    The alignmed sequences should be given in OneLetterCode, where '-' denotes a gap.

						  @param  aligned_ball_sequence    the aligned protein or chain aminoacid sequence 
						  @param  aligned_nmrstar_sequence the aligned aminoacid sequence of the NMRStar atoms  
						
						  @return bool - <tt>true</tt> if creating the mapping was possible 
					*/
					bool createMapping(const String& aligned_ball_sequence,
					                   const String& aligned_nmrstar_sequence);

					/** Clear the object.
					*/
					void clear();

					//@}

				protected:

					Peptides::NameConverter name_converter_;

					/**     @name Atributes
					*/
					//@{

					/// map BALL atoms to NMR atom data
					BALLToBMRBMapping       ball_to_bmrb_map_;

					/// map NMR atom data to BALL atoms
					BMRBToBALLMapping       bmrb_to_ball_map_;

					// NOTE: do *not* attempt to delete these pointers!
					const Chain*            chain_;
					const NMRStarFile*      nmr_data_;
					const NMRAtomDataSet*   nmr_atom_data_set_;
					Position                nmr_atom_data_set_index_;
					int                     num_mismatches_;
					int                     num_gaps_;
					bool                    valid_;
					//@}

				private:
					const Atom* findNMRAtom_(const NMRAtomData& atom) const;

			};

			//@}
			/**	@name	Constructors and Destructors
			*/
			//@{

			/** Standard constructor
			*/
			NMRStarFile();

			/** Detailed constuctor.
			 *	Opens the given file and extracts all usefull data (Calls  \link read read \endlink). 
			 *  	@throw Exception::FileNotFound if the file could not be opened
			 */
			NMRStarFile(const String& file_name, File::OpenMode open_mode = std::ios::in);

			/// Destructor.
			~NMRStarFile();
			//@}


			/**	@name	Access methods
			*/
			//@{

			/** Read an NMRStarFile.
			 *  @throw Exception::ParseError if a syntax error was encountered
			 */
			bool read();

			/*  Read an NMRStarFile and assign the shifts to the
			    given AtomContainer using a trivial standard mapping.
			    If the AtomContainer is a system, the first chain in chosen.  
			 
			 	  @param  ac AtomContainer to which the NMRStarfile's shift should be assigned.
				  @return bool - <tt>true</tt> if reading the file was successful 
			 */
			//TODO to be able to use this function, further functions getMapping() and assign() are needed.
			bool read(AtomContainer& ac);

			/** Assign the shifts to the given AtomContainer as 
			    denoted in the given {\b BALLToBMRBMapper}.
					
				  @return bool - <tt>true</tt> if reading the file was successful 
				  @see   NMRStarFile::BALLToBMRBMapper
			 */
			bool assignShifts(BALLToBMRBMapper& ball_to_bmrb_mapping);

			/** Assign the shifts to the given
			    AtomContainer as pointed out in the alignment.
			    The alignmed sequences should be given in 
			    OneLetterCode, where '-' denotes a gap. 		
					
				  @param  ac              AtomContainer to which the NMRStarfile's shift should be assigned.		
				  @param  chemical_unit   the name of the molecular system (chemical unit) storing the atom shifts 
				  @param  aligned_ball_sequence    the aligned AtomContainers aminoacid sequence 
				  @param  aligned_nmrstar_sequence the aligned aminoacid sequence of the NMRStar file atoms 
				  @return bool - <tt>true</tt> if reading the file was successful 
			 */
			bool assignShifts(AtomContainer& ac,
			                  const String& chemical_unit,
			                  const String& aligned_ball_sequence,
			                  const String& aligned_nmrstar_sequence);

			/** Return the maximum number of atoms in all shift sets
			*/
			Size getNumberOfAtoms() const;

			/** Return the number of atoms assigned a chemical shift
			*/
			Size getNumberOfShiftsAssigned() const {return number_of_assigned_shifts_;};

			/** Get the extracted data for the atoms
			*/
			const std::vector<NMRAtomDataSet>& getNMRData() const;

			/** Get the entry information
			 */
			const EntryInformation& getEntryInformation() const {return entry_information_;};

			/** Get the molecular information
			 */
			const MolecularSystem& getMolecularInformation() const {return molecular_system_;};

			/** Get the molecular information
			 */
			MolecularSystem& getMolecularInformation() {return molecular_system_;};


			/** Get a chemical unit by name
			 */
			const MolecularSystem::ChemicalUnit& getChemicalUnitByLabel(String const& label) const;

			/** Get a chemical unit by name
			 */
			MolecularSystem::ChemicalUnit& getChemicalUnitByLabel(String const& label);


			/**  Return true if the file contains a SampleCondition named name, false otherwise
			 */
			bool hasSampleCondition(String name);

			/**  Return true if the file contains a SampleCondition named name, false otherwise
			 */
			bool hasSampleCondition(String name) const;

			/** Get a SampleCondition by its SaveFrame name
			 */
			SampleCondition& getSampleConditionByName(String name);

			/** Get a SampleCondition by its SaveFrame name 
			 */
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

			/// Get the samples
			std::vector<Sample> getSamples()  const {return samples_;};
			//const std::vector<Sample>& getSamples() const {return samples_;};

			/// Get the number of samples
			Size getNumberOfSamples() const {return samples_.size();};

			/// Return true if the file contains a sample named label, false otherwise
			bool hasSample(String label) const;

			/** Get the i-th sample. 
			    If i is out of size a dummy sample is returned.
			*/
			Sample getSample(Position i) const;

			/** Get the sample with name label. 
			    If no sample with name label exist a dummy sample is returned.
			*/
			Sample getSample(String label) const;

			/// Get the shift reference sets
			std::vector<ShiftReferenceSet>& getShiftReferenceSets() {return shift_references_;};
			/// Get the shift reference sets
			const std::vector<ShiftReferenceSet>& getShiftReferenceSets() const  {return shift_references_;};

			/// Get the number of shift reference sets
			Size getNumberOfShiftReferenceSets() const {return shift_references_.size();};

			/// Check if there is a ShiftReferenceSet named name
			bool hasShiftReferenceSet(String name);

			/// Get the i-th shift reference set
			ShiftReferenceSet& getShiftReferenceSet(Position i) {return shift_references_[i];};
			/// Get the i-th shift reference set
			const ShiftReferenceSet& getShiftReferenceSet(Position i) const {return shift_references_[i];};

			/// Get a ShiftReferenceSet by its SaveFrame name
			const ShiftReferenceSet& getShiftReferenceSetByName(String name) const;
			/// Get a ShiftReferenceSet by its SaveFrame name
			ShiftReferenceSet& getShiftReferenceSetByName(String name);


			/// Get the spectrometers 
			std::vector<NMRSpectrometer>& getNMRSpectrometers() {return nmr_spectrometers_;};
			/// Get the spectrometers 
			const std::vector<NMRSpectrometer>& getNMRSpectrometers() const {return nmr_spectrometers_;};

			/// Get the number of nmr spectrometers
			Size getNumberOfNMRSpectrometers() const  {return nmr_spectrometers_.size();};

			/// Get the i-th spectrometer
			NMRSpectrometer& getNMRSpectrometer(Position i);
			/// Get the i-th spectrometer
			const NMRSpectrometer& getNMRSpectrometer(Position i) const;

			/// Get the spectrometer by its SaveFrame name
			NMRSpectrometer& getNMRSpectrometerByName(String name);
			/// Get the spectrometer by its SaveFrame name
			const NMRSpectrometer& getNMRSpectrometerByName(String name) const;

			/// Get the spectrometer manufacturer
			String getNMRSpectrometerManufacturer(Position i) const;

			/// Get the spectrometer field strength 
			float getNMRSpectrometerFieldStrength(Position i) const;


			/** Get a mutable reference to the MonomericPolymer-information by index
			 *	@throw Exception::OutOfRange if a polymer with this index could not be found
			 */
			NMRStarFile::MonomericPolymer& getMonomericPolymer(Position i);

			/** Get the MonomericPolymer-information by index
			 *	@throw Exception::OutOfRange if a polymer with this index could not be found
			 */
			const NMRStarFile::MonomericPolymer& getMonomericPolymer(Position i) const;

			/** Get a mutable reference to the MonomericPolymer-information by name
			 *	@throw Exception::OutOfRange if a polymer with this name could not be found
			 */
			NMRStarFile::MonomericPolymer& getMonomericPolymer(const String& name);

			/** Get the MonomericPolymer-information by name
			 *	@throw Exception::OutOfRange if a polymer with this name could not be found
			 */
			const NMRStarFile::MonomericPolymer& getMonomericPolymer(const String& name) const;

			/// Get the number of monomeric polymers in the file
			Size getNumberOfMonomericPolymers() const {return monomeric_polymers_.size();};

			/// Get all Monomeric Polymers
			vector<MonomericPolymer> getMonomericPolymers() const {return monomeric_polymers_;};

			///	Check if polymer name is already stored is a monomeric polymer
			bool hasMonomericPolymer(String name) const;

			/** Check if label is a monomeric polymer.
			 
			    Returns true if a monomer with name chemical_unit_label exists,
			    false otherwise.
			*/
			bool isMonomericPolymer(String chemical_unit_label);

			/** Add a Monomeric Polymer
			    NOTE: if a poymer with the same name already exists, it will be overwritten! 
			*/
			//TODO: Store changes/additions as Saveframes also in CIFFile
			void addMonomericPolymer(MonomericPolymer mp);


			/** Get the sequence of residues of the i-th monomeric polymer in the file.
					
			    This method automatically compares the i-th monomeric_polymers' residue sequence 
			    with the actual stored NMR shift data. If there is a mismatch, a warning is given!
			    If there is a mismatch or there is no monomeric_polymers residue sequence an empty. 
					
				@return String amino acid sequence
			*/
			String getResidueSequence(Position i=0) const;

			/// Check, whether this NMRFile provides hydrogen shifts 
			bool hasHshifts() const {return has_H_shifts_;};

			/// Check, whether this NMRFile provides carbon shifts 
			bool hasCshifts() const {return has_C_shifts_;};

			/// Check, whether this NMRFile provides nitrogen shifts 
			bool hasNshifts() const {return has_N_shifts_;};

			//@}


			/**	@name	Equality
			*/
			//@{

			/** Equality operator.
			    Test if both instances point to the same file.
			 */
			bool operator == (const NMRStarFile& f) const;

			/** Inequality operator
			    Test if both instances point to different files.
			 */
			bool operator != (const NMRStarFile& f) const;

			/** Clear the object.
			 */
			void clear();

			//@}

		private:

			/*_	@name	NMRStar file specific Help-Methods
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

			/// read the samples
			void readSamples_();

			/// reads the NMR spectrometer data
			void readNMRSpectrometer_();

			/// find dependencies for the ChemicalUnits
			void findDependiencies_();

			/// set the characters denoting single non-available values
			void setSpecialCharacters_(String characters);

			/// check whether the given String denotes a non-available value
			bool isValidSingleValue_(String value);

			/// returns the value as float, if it is a valid one, or FLOAT_VALUE_NA
			float valueToFloat_(String value);

			/// returns the value as int, if it is a valid one, or INT_VALUE_NA
			int valueToInt_(String value);
			/** Apply the shifts read into the AtomContainer as denoted in the mapping.
			 * 	We assume, that the file was already read!
			 * 	The shifts are stored as a property under the key 
			 * 	{\b BALL::ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT}
			 */
			bool assignShifts_(BALLToBMRBMapper& pdb_to_bmrb_mapping);

			//_@}
			/*_	@name	NMRStar file specific attributes
			*/
			//_@{

			/*_ A flag indicating validity of this instance. A sole NMRStarFile
				instance cannot be valid, because it does not have any information.
			*/
			bool valid_;

			/// the number of shift data sets  
			Size number_of_shift_sets_;

			/// the number of assigned shifts during the last call of assignShifts()
			Size number_of_assigned_shifts_;

			/// the general entry data 
			EntryInformation entry_information_;

			/// the system information
			MolecularSystem molecular_system_;

			/// the data for the atoms is stored here
			std::vector<NMRAtomDataSet> atom_data_sets_;

			/// the data for different sample sets
			std::vector<SampleCondition> sample_conditions_;

			/// the samples
			std::vector<Sample> samples_;

			/// the data for shift references
			std::vector<ShiftReferenceSet> shift_references_;

			/// the data of nmr spectrometers 
			std::vector<NMRSpectrometer> nmr_spectrometers_;

			/// Monomeric Polymer information 
			vector<MonomericPolymer> monomeric_polymers_;

			/// stores, which shifts are given in the file
			bool has_H_shifts_;
			bool has_C_shifts_;
			bool has_N_shifts_;

			// a dummy saveframe
			SaveFrame dummy_saveframe_;

			// a dummy sample condition
			SampleCondition dummy_sample_condition_;

			// a dummy sample
			Sample dummy_sample_;

			// a dummy shift reference set
			ShiftReferenceSet dummy_shift_reference_set_;

			// a dummy nmr spectrometer
			NMRSpectrometer dummy_NMR_spectrometer_;

			// a dummy nmr spectrometer
			MonomericPolymer dummy_monomeric_polymer_;

			/// characters, that denote non-available values
			String special_characters_;
			//_@}
	};

	//@}
} // Namespace BALL

#endif // BALL_FORMAT_NMRSTARFILE_H
