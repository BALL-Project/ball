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
	using namespace std;
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
					
					bool hasType(String );	
//					void clear(); 
//					ostream& operator >> (ostream& s);
			};
	
			class Sample
			{
				public:
				
					class Component
					{
						public:
							Component();
//							void clear(); 
//							ostream& operator >> (ostream& s);
							
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

//					vector <Component> components;
			};

			class ShiftReferenceElement
			{
				public:
					ShiftReferenceElement();

					String		mol_common_name;
					String		atom_type;
					Position	isotope_number;
					String		atom_group;
					String		shift_units;
					float			shift_value;
					String		reference_method;
					String		reference_type;
					float			indirect_shift_ratio;

//					void clear(); 
//					ostream& operator >> (ostream& s);
			};

			class ShiftReferenceSet
			{
				public:	
					ShiftReferenceSet();
				
					String name;  
//					vector<NMRStarFile::ShiftReferenceElement> elements;
//					void clear(); 
//					ostream& operator >> (ostream& s);
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
		
//					bool operator == (const NMRAtomData& ) const;

//					void clear(); 
//					ostream& operator >> (ostream& s);
			};

						
			class NMRAtomDataSet
			{
				public:
					NMRAtomDataSet(NMRStarFile* parent);

					String										name;
//					vector<NMRStarFile::NMRAtomData> 	atom_data; //TODO!!
					String 										condition;  
					String										reference;  
					vector<String>						samples;   
//					void clear(); 
//					ostream& operator >> (ostream& s);
			};
			
			class EntryInformation
			{
				public:
					EntryInformation();
					~EntryInformation();
//					ostream& operator >> (ostream& s);
					void clear(); 

					String entry_type;
					String BMRB_accession_code;
					String NMR_STAR_version;
					String experimental_method;
					String submission_date;
			};

			class MonomericPolymer
			{
				public:
					class HomologDB 
					{
						public:
							HomologDB();
//							ostream& operator >> (ostream& s); 
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
	
					ChemicalUnit&  getChemicalUnit(Position i);
					Size getNumberOfChemicalUnits();					

					String system_name;
					String abbreviation_common;
					//vector<NMRStarFile::MolecularSystem::ChemicalUnit> chemical_units;  
					String system_physical_state;
					String system_oligomer_state;
					String system_paramagnetic;
					String system_thiol_state;
					float system_molecular_weight;	

//					vector<NMRStarFile::MolecularSystem::RelatedDB> related_database_entries;
//					ostream& operator >> (ostream& s); 
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
			
			class BALLToBMRBMapper
			{
%TypeHeaderCode
					#include <BALL/KERNEL/chain.h>
  				#include <BALL/COMMON/exception.h>
					
					using namespace BALL;
					using namespace BALL::Exception;
					using namespace std;
					typedef pair<Position, Position>             BMRBIndex;
					typedef map<Atom const* , BMRBIndex>         BALLToBMRBMapping; 
					typedef map<const NMRStarFile::NMRAtomData*, Atom const*> BMRBToBALLMapping;
%End
				public:
				
					BALLToBMRBMapper();	

					BALLToBMRBMapper(const Chain& , const NMRStarFile&, const String&);	
					virtual ~BALLToBMRBMapper();

					const Chain* getChain() const;
					void setChain(const Chain&);
					
					void setNMRStarFile(const NMRStarFile&);
					const NMRStarFile* getNMRStarFile() const;
					
					// void setNMRAtomDataSet(const NMRAtomDataSet& ); 
					bool setNMRAtomDataSetByName(const String& );
					//const NMRAtomDataSet* getNMRAtomDataSet();

					// TODO!
//					map<const Atom*, BMRBIndex>& getBALLToBMRBMapping();
//					const BALLToBMRBMapping& getBALLToBMRBMapping() const;
					
					// TODO!
//					BMRBToBALLMapping& getBMRBToBALLMapping();
//					const BMRBToBALLMapping& getBMRBToBALLMapping() const;

					int getNumberOfMismatches();
					int getNumberOfGaps();
					bool isMapped(const NMRStarFile::NMRAtomData&) const; // TODO!
					const Atom* getBALLAtom(const NMRStarFile::NMRAtomData&) const;

					bool isMapped(const Atom*) const;
					pair<Position, Position> operator () (Atom*);

					bool createTrivialMapping();
					bool createMapping(const String&, const String&);	
					void clear();

			};

			NMRStarFile();
			
			NMRStarFile(const String&_name, OpenMode)
				throw(FileNotFound);

			~NMRStarFile();

			bool read();
			void close();

		//	bool read(AtomContainer&);

			bool assignShifts(BALLToBMRBMapper&);
			bool assignShifts(AtomContainer&, const String&,  const String&, const String&);

			
			Size getNumberOfAtoms() const;
			Size getNumberOfShiftsAssigned();
	
			// TODO!		
//			const vector<NMRStarFile::NMRAtomDataSet>& getNMRData() const;

			const EntryInformation& getEntryInformation() const;

			MolecularSystem& getMolecularInformation();
			MolecularSystem::ChemicalUnit& getChemicalUnitByLabel(const String& );
	
			bool hasSampleCondition(String );

			SampleCondition& getSampleConditionByName(String );
//			const SampleCondition& getSampleConditionByName(String ) const;
			SampleCondition& getSampleCondition(Position );
//			const SampleCondition& getSampleCondition(Position ) const;
			const Size getNumberOfSampleConditions() const;
//			const vector<SampleCondition>& getSampleConditions() const;
//			vector<NMRStarFile::SampleCondition>& getSampleConditions();
//			vector<NMRStarFile::Sample> getSamples() const; //TODO

			Size getNumberOfSamples() const;
			bool hasSample(String label) const;
			Sample getSample(Index ) const;
			Sample getSample(String ) const;


//			vector<NMRStarFile::ShiftReferenceSet>& getShiftReferenceSets(); 
//			const vector<ShiftReferenceSet>& getShiftReferenceSets() const;

			Size getNumberOfShiftReferenceSets() const;
			bool hasShiftReferenceSet(String );	

//			const ShiftReferenceSet& getShiftReferenceSet(Position ) const;
			ShiftReferenceSet& getShiftReferenceSet(Position ); 
//			const ShiftReferenceSet& getShiftReferenceSetByName(String ) const;
			ShiftReferenceSet& getShiftReferenceSetByName(String );
			
//			vector<NMRSpectrometer>& getNMRSpectrometers();
			Size getNumberOfNMRSpectrometers() const;
			NMRStarFile::NMRSpectrometer& getNMRSpectrometer(Position );	
			NMRSpectrometer& getNMRSpectrometerByName(String );

			
			String getNMRSpectrometerManufacturer(Position );
			float getNMRSpectrometerFieldStrength(Position );
		
			Size getNumberOfMonomericPolymers() const;

			MonomericPolymer getMonomericPolymer(Position);
			MonomericPolymer getMonomericPolymer(String name);
		
//			vector<NMRStarFile::MonomericPolymer> getMonomericPolymers() const;
			
			bool hasMonomericPolymer(String name) const; 
		
			bool isMonomericPolymer(String ); 

			void addMonomericPolymer(MonomericPolymer );
			
			String getResidueSequence(Position i) const;

			bool hasHshifts();
			bool hasCshifts();
			bool hasNshifts();

			bool operator == (const NMRStarFile& ) const;
			bool operator != (const NMRStarFile& ) const;
	
			void clear(); 

	private:
			NMRStarFile(const NMRStarFile& f);


};
