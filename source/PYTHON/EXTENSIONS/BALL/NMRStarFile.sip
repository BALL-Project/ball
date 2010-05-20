// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//


class NMRStarFile
//	: CIFFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/CIFFile.h>
  #include <BALL/FORMAT/NMRStarFile.h>
  using namespace BALL;
%End
	public:

			static const float FLOAT_VALUE_NA;
			static const int INT_VALUE_NA;
			static const int POSITION_VALUE_NA;
			
			class SampleCondition
			{
				public:
					SampleCondition(); 
	
					String	name;
					vector<String> types;
					StringHashMap<float>  values;
					StringHashMap<float>  errors;
					StringHashMap<String> units;
				
	//				void clear(); 
//					ostream& operator >> (ostream& s);
			};
	
			class Sample
			{
				public:
				
					class Component
					{
						public:
							Component();
			//				void clear(); 
			//				ostream& operator >> (ostream& s);
							
							String label;
							float concentration_value;
							String value_unit;
							float concentration_min;
							float concentration_max;
							String isotopic_labeling;
					};

					Sample(); 
//					void clear(); 
//					ostream& operator >> (ostream& s);

					String label;
					String type;
					String details;

//					vector <NMRStarFile::Sample::Component> components;

			};

			class ShiftReferenceElement
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

		//			void clear(); 

			//		ostream& operator >> (ostream& s);
			};

			class ShiftReferenceSet
			{
				public:	
					ShiftReferenceSet();
				
					String name;  
//					vector<NMRStarFile::ShiftReferenceElement> elements;

			//		void clear(); 

				//	ostream& operator >> (ostream& s);
			};

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

			//		void clear(); 

		//			ostream& operator >> (ostream& s);
			};

						
			class NMRAtomDataSet
			{
				public:
					NMRAtomDataSet(NMRStarFile* parent);

					String										name;
//					vector<NMRStarFile::NMRAtomData> 	atom_data;
					String 										condition;  
					String										reference;  
					vector<String>						samples;   

			//		void clear(); 
			//		ostream& operator >> (ostream& s);
			};
			
			class EntryInformation
			{
				public:
					EntryInformation();
					~EntryInformation();
			//		ostream& operator >> (ostream& s);
		  //			void clear(); 

					String entry_type;
					String BMRB_accession_code;
					String NMR_STAR_version;
					String experimental_method;
			};

			class MonomericPolymer
			{
				public:
					class HomologDB 
					{
						public:
							HomologDB();
			//				ostream& operator >> (ostream& s); 
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
//						ostream& operator >> (ostream& s); 
						void clear(); 

						String label_name;
						String type;
						String polymer_class;
						String common_name;
						String name_variant;
						float  molecular_mass;
						String details;  	
						int number_of_residues;
						String residue_sequence; 
						StringHashMap<String> residues_by_index;

//						vector<NMRStarFile::MonomericPolymer::HomologDB> homolog_database_entries; 
			};


			class MolecularSystem
			{	
				public:
					class RelatedDB
					{
						public:
							RelatedDB();
			//				ostream& operator >> (ostream& s); 
							void clear(); 

							String name;
							String accession_code;
							String entry_mol_name;
							String relation_type;
							String details;
					};
					
													
					class ChemicalUnit
					{
						public:
							ChemicalUnit();  
					//		ostream& operator >> (ostream& s);  
							void clear();  

							String component_name;
							String label;
//							MonomericPolymer* monomeric_polymer; 
//							NMRAtomDataSet* shifts;             
					};


					MolecularSystem(); 
					~MolecularSystem(); 

					String system_name;
					String abbreviation_common;
//					vector<NMRStarFile::MolecularSystem::ChemicalUnit> chemical_units;  
					String system_physical_state;
					String system_oligomer_state;
					String system_paramagnetic;
					String system_thiol_state;
					float system_molecular_weight;	

//					vector<NMRStarFile::MolecularSystem::RelatedDB> related_database_entries;
					//ostream& operator >> (ostream& s); 
					void clear(); 
			};


			class NMRSpectrometer
			{
				public:
					NMRSpectrometer();
					String manufacturer;
					String model;
					float  field_strength;
//					ostream& operator >> (ostream& s);
			};
			
			class BALLToBMRBMapping
			{
				public:
						
					BALLToBMRBMapping(Protein* /protein/, const NMRStarFile* /nmr_data/);
					BALLToBMRBMapping(Chain* /chain/, const NMRStarFile* /nmr_data/);

					bool hasAtom(Atom* /atom/);
				
					pair<Position, Position>& operator() (Atom* /atom/); 

					Protein* getProtein();
					void setProtein(Protein* /protein/); 
					Chain* getChain();
					void setChain(Chain* /chain/);

					const Atom* findNMRAtomInProtein(NMRStarFile::NMRAtomData& /atom/);

					bool createTrivialMapping();
					bool createMapping(	const String& /aligned_ball_sequence/,
															const String& /aligned_nmrstar_sequence/);
					int getNumberOfMismatches();

			};

			NMRStarFile();
			
			NMRStarFile(const NMRStarFile& f)
				throw(FileNotFound);

			NMRStarFile(const String& file_name, OpenMode open_mode = ios::in)
				throw(FileNotFound);

			~NMRStarFile()
				throw();

//			const NMRStarFile& operator = (const NMRStarFile& /f/)  
//				throw();
			
			bool read()
				throw();

			bool read(AtomContainer& /ac/);

			bool assignShifts(BALLToBMRBMapping& /pdb_to_bmrb_mapping/);
		
			bool assignShifts(AtomContainer& /ac/, const String& /aligned_ball_sequence/,
												const String& /aligned_nmrstar_sequence/);

			void clear() 
				throw();

			Size getNumberOfAtoms()    
				const throw();
			Size getNumberOfShiftsAssigned();
			
//			const vector<NMRStarFile::NMRAtomDataSet>& getNMRData() const throw();

			const EntryInformation& getEntryInformation()  const throw();

			const MolecularSystem& getMolecularInformation() const throw();

			SampleCondition& getSampleConditionByName(String name) throw();
//			const SampleCondition& getSampleConditionByName(String name) const throw();
			SampleCondition& getSampleCondition(Position i) throw ();
//			const SampleCondition& getSampleCondition(Position i) const throw ();
			const Size getNumberOfSampleConditions() const throw();
//			const vector<SampleCondition>& getSampleConditions() const throw();
//			vector<NMRStarFile::SampleCondition>& getSampleConditions() throw();

//			vector<NMRStarFile::Sample> getSamples()  const throw();

			Size getNumberOfSamples() const throw();
			
			bool hasSample(String label) const throw();
			
			Sample getSample(Index i) const throw();
			
			Sample getSample(String label) const throw();

//			vector<NMRStarFile::ShiftReferenceSet>& getShiftReferenceSets() throw(); 
			//const vector<ShiftReferenceSet>& getShiftReferenceSets() const throw();

			const Size getNumberOfShiftReferenceSets() const throw();

			//const ShiftReferenceSet& getShiftReferenceSet(Position i) const throw();
			ShiftReferenceSet& getShiftReferenceSet(Position i) throw(); 

//			const ShiftReferenceSet& getShiftReferenceSetByName(String name) const throw();
			ShiftReferenceSet& getShiftReferenceSetByName(String name) throw();
			NMRSpectrometer& getNMRSpectrometer() throw();
//			const NMRSpectrometer& getNMRSpectrometer() const throw();
			
			String getNMRSpectrometerManufacturer() throw();
			float getNMRSpectrometerFieldStrength() throw();
		
			Size getNumberOfMonomericPolymers() const throw();

			MonomericPolymer getMonomericPolymer(String name) throw();
		
//			vector<NMRStarFile::MonomericPolymer> getMonomericPolymers() const throw();
			
			bool hasMonomericPolymer(String name) throw(); 
		
			bool isMonomericPolymer(String chemical_unit_label) throw(); 

			void addMonomericPolymer(MonomericPolymer mp) throw();
			
			String getResidueSequence();

			bool hasHshifts();
			bool hasCshifts();
			bool hasNshifts();

			bool operator == (const NMRStarFile& f)  throw();
			bool operator != (const NMRStarFile& f)  throw();

};
