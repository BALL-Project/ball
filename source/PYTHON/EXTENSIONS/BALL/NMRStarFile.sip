class NMRStarFile
{
%TypeHeaderCode
	#include <BALL/FORMAT/NMRStarFile.h>
%End
	public:

	static const float FLOAT_VALUE_NA;
	static const int INT_VALUE_NA;
	static const int POSITION_VALUE_NA;

	class SampleCondition
	{
		public:
		SampleCondition();
		bool hasType(String );

		String name;
		vector<String> types;
		StringHashMap<float> values;
		StringHashMap<float> errors;
		StringHashMap<String> units;
	};

	class Sample
	{
		public:

		class Component
		{
			public:
			Component();

			String label;
			float concentration_value;
			String value_unit;
			float concentration_min;
			float concentration_max;
			String isotopic_labeling;
		};

		Sample();

		String label;
		String type;
		String details;
	};

	class ShiftReferenceElement
	{
		public:
		ShiftReferenceElement();

		String   mol_common_name;
		String   atom_type;
		Position isotope_number;
		String   atom_group;
		String   shift_units;
		float    shift_value;
		String   reference_method;
		String   reference_type;
		float    indirect_shift_ratio;
	};

	class ShiftReferenceSet
	{
		public:
		ShiftReferenceSet();

		String name;
	};

	class NMRAtomData
	{
		public:
		NMRAtomData();

		Position atom_ID;
		Position residue_seq_code;
		String   residue_label;
		String   atom_name;
		char     atom_type;
		float    shift_value;
		float    error_value;
		Position ambiguity_code;
	};

	class NMRAtomDataSet
	{
		public:
		NMRAtomDataSet(NMRStarFile* parent);

		String name;
		String condition;
		String reference;
		vector<String> samples;
	};

	class EntryInformation
	{
		public:
		EntryInformation();
		~EntryInformation();
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
	};


	class MolecularSystem
	{
		public:

		class RelatedDB
		{
			public:
			RelatedDB();
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
			void clear();

			String component_name;
			String label;
		};

		MolecularSystem();
		~MolecularSystem();

		ChemicalUnit&  getChemicalUnit(Position i);
		Size getNumberOfChemicalUnits();
		void clear();

		String system_name;
		String abbreviation_common;
		String system_physical_state;
		String system_oligomer_state;
		String system_paramagnetic;
		String system_thiol_state;
		float system_molecular_weight;
	};

	class NMRSpectrometer
	{
		public:
		NMRSpectrometer();
		String manufacturer;
		String model;
		float  field_strength;
	};

	class BALLToBMRBMapper
	{
%TypeHeaderCode
		#include <BALL/KERNEL/chain.h>
		using BMRBIndex = std::pair<Position, Position>;
		using BALLToBMRBMapping = std::map<Atom* const, BMRBIndex>;
		using BMRBToBALLMapping = std::map<const NMRStarFile::NMRAtomData*, Atom* const>;
%End
		public:

		BALLToBMRBMapper();

		BALLToBMRBMapper(const Chain& , const NMRStarFile&, const String&);
		virtual ~BALLToBMRBMapper();

		const Chain* getChain() const;
		void setChain(const Chain&);

		void setNMRStarFile(const NMRStarFile&);
		const NMRStarFile* getNMRStarFile() const;

		bool setNMRAtomDataSetByName(const String& );

		int getNumberOfMismatches();
		int getNumberOfGaps();
		bool isMapped(const NMRStarFile::NMRAtomData&) const;
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

	bool assignShifts(BALLToBMRBMapper&);
	bool assignShifts(AtomContainer&, const String&,  const String&, const String&);


	Size getNumberOfAtoms() const;
	Size getNumberOfShiftsAssigned();

	const EntryInformation& getEntryInformation() const;

	MolecularSystem& getMolecularInformation();
	MolecularSystem::ChemicalUnit& getChemicalUnitByLabel(const String& );

	bool hasSampleCondition(String );

	SampleCondition& getSampleConditionByName(String );
	SampleCondition& getSampleCondition(Position );
	const Size getNumberOfSampleConditions() const;

	Size getNumberOfSamples() const;
	bool hasSample(String label) const;
	Sample getSample(Index ) const;
	Sample getSample(String ) const;

	Size getNumberOfShiftReferenceSets() const;
	bool hasShiftReferenceSet(String );

	ShiftReferenceSet& getShiftReferenceSet(Position );
	ShiftReferenceSet& getShiftReferenceSetByName(String );

	Size getNumberOfNMRSpectrometers() const;
	NMRStarFile::NMRSpectrometer& getNMRSpectrometer(Position );
	NMRSpectrometer& getNMRSpectrometerByName(String );


	String getNMRSpectrometerManufacturer(Position );
	float getNMRSpectrometerFieldStrength(Position );

	Size getNumberOfMonomericPolymers() const;

	MonomericPolymer getMonomericPolymer(Position);
	MonomericPolymer getMonomericPolymer(String name);

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
