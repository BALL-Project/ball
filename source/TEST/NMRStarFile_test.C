// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/CONCEPT/classTest.h>
#include <BALLTestConfig.h>

///////////////////////////
#include <BALL/STRUCTURE/fragmentDB.h>
#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/FORMAT/PDBFile.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/NMR/shiftModule.h>
///////////////////////////

using namespace BALL;

START_TEST(String)

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

NMRStarFile* f;

CHECK(NMRStarFile() )
	f = new NMRStarFile;
	TEST_NOT_EQUAL(f, 0)
RESULT

CHECK(~NMRStarFile() )
	delete(f);
RESULT

CHECK(NMRStarFile(const String& file_name) throw(Exception::FileNotFound, Exception::ParseError))
	PRECISION(1e-3)
 	NMRStarFile rs(BALL_TEST_DATA_PATH(NMRStarFile_test.bmr));
RESULT


NMRStarFile rs(BALL_TEST_DATA_PATH(NMRStarFile_test.bmr));


CHECK(EntryInformation)
	const NMRStarFile::EntryInformation& entry_info = rs.getEntryInformation();
	TEST_EQUAL(entry_info.entry_type, "new")
	TEST_EQUAL(entry_info.BMRB_accession_code, "3322")
	TEST_EQUAL(entry_info.NMR_STAR_version, "2.1.1")
	TEST_EQUAL(entry_info.experimental_method, "NMR")
RESULT


CHECK(MolecularSystem)
	const NMRStarFile::MolecularSystem& mol_sys = rs.getMolecularInformation();
	
	TEST_EQUAL(mol_sys.system_name, "\"endoglucanase Cel45A\"") //TODO 
	TEST_EQUAL(mol_sys.abbreviation_common, "dockerin")
	TEST_EQUAL(mol_sys.chemical_units.size(), 1)
	TEST_EQUAL(mol_sys.getNumberOfChemicalUnits(), 1)	
	if (mol_sys.chemical_units.size() > 0)
	{	
		TEST_EQUAL(mol_sys.chemical_units[0].component_name, "dockerin")
		TEST_EQUAL(mol_sys.getChemicalUnit(0).component_name, "dockerin")
		TEST_EQUAL(mol_sys.chemical_units[0].label, "dockerin")
		TEST_EQUAL(mol_sys.getChemicalUnit(0).label, "dockerin")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer != NULL, true)  //!= NULL

		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->label_name, "dockerin")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->type, "polymer")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->polymer_class, "protein")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->common_name, "dockerin")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->name_variant, ".")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->molecular_mass, 0.f)
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->details,
				"For structure calculations, only first 46 residues are considered.  \nAlso the N-terminal residue Gly is replaced by Ala.")
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->number_of_residues, 52)
		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->residue_sequence,"GSCWAQSQGYNCCNNPSSTKVEYTDASGQWGVQNGQWCGIDYSYGQNQGNES")
		// StringHashMap<String> residues_by_index; //TODO

		TEST_EQUAL(mol_sys.chemical_units[0].monomeric_polymer->homolog_database_entries.size(), 3)

		TEST_EQUAL(mol_sys.chemical_units[0].shifts != NULL, true) 
		if (mol_sys.chemical_units[0].shifts != NULL)	
			TEST_EQUAL(mol_sys.chemical_units[0].shifts->name, rs.getNMRData()[0].name)   
	}

	TEST_EQUAL(mol_sys.system_physical_state,	"native" )
	TEST_EQUAL(mol_sys.system_oligomer_state,	"monomer")
	TEST_EQUAL(mol_sys.system_paramagnetic	,	"no")
	TEST_EQUAL(mol_sys.system_thiol_state		, "'all disulfide bound'")
	TEST_REAL_EQUAL(mol_sys.system_molecular_weight, NMRStarFile::FLOAT_VALUE_NA);
  TEST_EQUAL(mol_sys.related_database_entries.size(), 0)
/*	
	if (mol_sys.related_database_entries.size() > 0)
	{
		std::cout << mol_sys.related_database_entries[0].name << std::endl; 	
		std::cout << mol_sys.related_database_entries[0].accession_code << std::endl; 	
		std::cout << mol_sys.related_database_entries[0].entry_mol_name << std::endl; 	
		std::cout << mol_sys.related_database_entries[0].relation_type  << std::endl; 	
		std::cout << mol_sys.related_database_entries[0].details			  << std::endl;
	}*/
RESULT


CHECK(MonomericPolymer)
	PRECISION(1e-3)
	TEST_EQUAL(rs.getNumberOfMonomericPolymers(), 1)
	TEST_EQUAL(rs.getMonomericPolymer("dockerin").label_name, "dockerin")
	TEST_EQUAL(rs.hasMonomericPolymer("dockerin"), true) 
	TEST_EQUAL(rs.hasMonomericPolymer("dockerin_any"), false) 
	TEST_EQUAL(rs.isMonomericPolymer("dockerin"), true) 
	TEST_EQUAL(rs.getResidueSequence(), "GSCWAQSQGYNCCNNPSSTKVEYTDASGQWGVQNGQWCGIDYSYGQNQGNES")

	const vector<NMRStarFile::MonomericPolymer>& mono_polymers = rs.getMonomericPolymers();
	TEST_EQUAL(rs.getNumberOfMonomericPolymers(), mono_polymers.size())

	if (mono_polymers.size()>0)
	{
		TEST_EQUAL(mono_polymers[0].label_name, "dockerin")
		TEST_EQUAL(mono_polymers[0].type,				"polymer")
		TEST_EQUAL(mono_polymers[0].polymer_class,"protein")
		TEST_EQUAL(mono_polymers[0].common_name, "dockerin")
		TEST_EQUAL(mono_polymers[0].name_variant,".")
		TEST_REAL_EQUAL(mono_polymers[0].molecular_mass, 0.0)
		TEST_EQUAL(mono_polymers[0].details,"For structure calculations, only first 46 residues are considered.  \nAlso the N-terminal residue Gly is replaced by Ala.")
		TEST_EQUAL(mono_polymers[0].number_of_residues, 52)
		TEST_EQUAL(mono_polymers[0].residue_sequence, "GSCWAQSQGYNCCNNPSSTKVEYTDASGQWGVQNGQWCGIDYSYGQNQGNES")
		//StringHashMap<String> residues_by_index; //TODO
		TEST_EQUAL(mono_polymers[0].homolog_database_entries.size(), 3)
		if (mono_polymers[0].homolog_database_entries.size() == 3)
		{
			TEST_EQUAL(mono_polymers[0].homolog_database_entries.back().name, "PDB")
			TEST_EQUAL(mono_polymers[0].homolog_database_entries.back().accession_code, "2J4N") 
			TEST_EQUAL(mono_polymers[0].homolog_database_entries.back().entry_mol_name, "\"Double Dockerin From Piromyces Equi Cel45a\"") //TODO"
			TEST_REAL_EQUAL(mono_polymers[0].homolog_database_entries.back().seq_to_submitted_percentage, 98.08)
			TEST_REAL_EQUAL(mono_polymers[0].homolog_database_entries.back().subject_length, 100)
			TEST_REAL_EQUAL(mono_polymers[0].homolog_database_entries.back().seq_identity, 98.04)
			TEST_REAL_EQUAL(mono_polymers[0].homolog_database_entries.back().seq_positive, 100.00)
			TEST_REAL_EQUAL(mono_polymers[0].homolog_database_entries.back().homology_expectation_value, 9.29e-20f)  
		}
	}

RESULT


CHECK(NMRAtomDataSet)
	TEST_EQUAL(rs.getNMRData()[0].condition, "condition_1")
	TEST_EQUAL(rs.getNMRData()[0].reference, "chemical_shift_reference")
	TEST_EQUAL(rs.getNumberOfSamples(), 1)
	TEST_EQUAL(rs.getNMRData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 343)
	TEST_EQUAL(rs.getSampleConditionByName("condition_1").types[0], "pH") 
	TEST_EQUAL(rs.getShiftReferenceSetByName("chemical_shift_reference").elements[0].mol_common_name , "TSP") 
	TEST_EQUAL(rs.getSample("Sample_1").label, rs.getNMRData()[0].samples[0]) 
	TEST_EQUAL(rs.hasSample("Sample_1"), true)
RESULT


CHECK(NMRAtomData) //NMRStarFile(const String& file_name) throw(Exception::FileNotFound, Exception::ParseError))
	PRECISION(1e-3)
 	//NMRStarFile rs(BALL_TEST_DATA_PATH(NMRStarFile_test.bmr));

	TEST_EQUAL(rs.getNMRData().size(), 1)
	TEST_EQUAL(rs.getNumberOfAtoms(), 343)
	if (rs.getNMRData().size() == 1 && rs.getNumberOfAtoms() == 343)
	{
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().atom_ID, 343)
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().residue_seq_code, 52)
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().residue_label, "SER")
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().atom_type, 'N')
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().atom_name, "N")
		TEST_REAL_EQUAL(rs.getNMRData()[0].atom_data.back().shift_value, 122.3)
		TEST_REAL_EQUAL(rs.getNMRData()[0].atom_data.back().error_value, 0.1)
		TEST_EQUAL(rs.getNMRData()[0].atom_data.back().ambiguity_code, 1)
	}
	TEST_EQUAL(rs.getNMRData()[0].name, "dockerin") 
RESULT


CHECK(SampleCondition) 
	PRECISION(1e-3)
	TEST_EQUAL(rs.getNMRData()[0].condition, "condition_1")
	TEST_EQUAL(rs.getNumberOfSampleConditions(), 1) 
	TEST_EQUAL(rs.getSampleConditions().size(), 1)
	NMRStarFile::SampleCondition& condition = rs.getSampleConditionByName(rs.getNMRData()[0].condition);

	TEST_REAL_EQUAL(condition.values["temperature"], 298)
	TEST_REAL_EQUAL(condition.errors["temperature"], 1)
	TEST_EQUAL(condition.units["temperature"], "K")

	TEST_REAL_EQUAL(condition.values["pH"], 6.5)
	TEST_REAL_EQUAL(condition.errors["pH"], 0.1)
	TEST_EQUAL(condition.units["pH"], "n/a")
RESULT


CHECK(ShiftReferenceElement/ShiftReferenceSet)
	TEST_EQUAL(rs.getNMRData()[0].reference, "chemical_shift_reference")
	NMRStarFile::ShiftReferenceSet& reference = rs.getShiftReferenceSetByName(rs.getNMRData()[0].reference);
	TEST_EQUAL(rs.getNumberOfShiftReferenceSets(), 1)
  TEST_EQUAL(reference.name, "chemical_shift_reference")
	TEST_EQUAL(reference.elements.size(), 2)
	if (reference.elements.size()>0)
	{
		TEST_EQUAL(reference.elements[0].mol_common_name, "TSP") 
	  TEST_EQUAL(reference.elements[0].atom_type, "H" )
	  TEST_EQUAL(reference.elements[0].isotope_number, 1)
	  TEST_EQUAL(reference.elements[0].atom_group, "'methyl protons'")
	  TEST_EQUAL(reference.elements[0].shift_units, "ppm")
	  TEST_REAL_EQUAL(reference.elements[0].shift_value, 0.0)
	  TEST_EQUAL(reference.elements[0].reference_method, "internal")
	  TEST_EQUAL(reference.elements[0].reference_type, "direct")
	  TEST_REAL_EQUAL(reference.elements[0].indirect_shift_ratio, 1.0)
	}
	if (reference.elements.size()>1)
	{
		TEST_EQUAL(reference.elements[1].mol_common_name, "TSP") 
	  TEST_EQUAL(reference.elements[1].atom_type, "N" )
		TEST_EQUAL(reference.elements[1].isotope_number, 15)
	  TEST_EQUAL(reference.elements[1].atom_group, "'methyl protons'")
	  TEST_EQUAL(reference.elements[1].shift_units, "ppm")
	  TEST_REAL_EQUAL(reference.elements[1].shift_value, 0.0)
	  TEST_EQUAL(reference.elements[1].reference_method, ".")
	  TEST_EQUAL(reference.elements[1].reference_type, "indirect")
	  TEST_REAL_EQUAL(reference.elements[1].indirect_shift_ratio, 0.101329118)
	}
	/*
	for (Position i = 0;  i < rs.getNMRData()[0].atom_data.size(); i++)
	{
		cout << *(rs.getNMRData()[0].atom_data[i]) << endl;
	}
	*/	
RESULT


CHECK(Sample)
	TEST_EQUAL(rs.getNMRData()[0].samples.size(), rs.getNumberOfSamples())
	TEST_EQUAL(rs.getSample("Sample_1").label, "Sample_1")
	TEST_EQUAL(rs.getSample("Sample_1").type, "solution" )
	TEST_EQUAL(rs.getSample("Sample_1").details, "" )
	TEST_EQUAL(rs.getSample("Sample_1").components.size(), 1) 

	const std::vector<NMRStarFile::Sample>& samples =  rs.getSamples();
	TEST_EQUAL( samples.size(), rs.getNumberOfSamples())
	if (rs.getNumberOfSamples()>0)
	{	
		const NMRStarFile::Sample& sample = rs.getSample(0);	
		TEST_EQUAL(sample.label, "Sample_1")
		TEST_EQUAL(sample.type, "solution" )
		TEST_EQUAL(sample.details, "" )

		const vector<NMRStarFile::Sample::Component>& components = sample.components;
		TEST_EQUAL(components.size(),1) 	
		if (components.size()>0)
		{
			TEST_EQUAL(components[0].label, "dockerin")
			TEST_REAL_EQUAL(components[0].concentration_value, 1)
			TEST_EQUAL(components[0].value_unit, "mM")
			TEST_REAL_EQUAL(components[0].concentration_min, 0.f)
      TEST_REAL_EQUAL(components[0].concentration_max, 0.f)
      TEST_EQUAL(components[0].isotopic_labeling, "\"[U-15N]\"") //TODO /.toChar(); ??
		}
	}
RESULT


CHECK(NMRSpectrometer)
	TEST_EQUAL(rs.getNumberOfNMRSpectrometers(), 2)
	TEST_EQUAL(rs.getNMRSpectrometerManufacturer(1), "Bruker")
	TEST_REAL_EQUAL(rs.getNMRSpectrometerFieldStrength(0), 500 )
	vector<NMRStarFile::NMRSpectrometer>& spectrometers = rs.getNMRSpectrometers();
	TEST_EQUAL(spectrometers.size(), 2)
	if (spectrometers.size()> 1)
	{
		TEST_EQUAL(spectrometers[1].name, "NMR_spectrometer_2")	
		TEST_EQUAL(spectrometers[1].model,"DRX")
	}
RESULT


NMRStarFile f2(BALL_TEST_DATA_PATH(NMRStarFile_test.bmr));

CHECK(bool operator == (const NMRStarFile& f) )
	NMRStarFile f1;
	NMRStarFile f3;
	TEST_EQUAL(f1 == f3, true)
	TEST_EQUAL(f1 == f2, false)
RESULT


CHECK(bool operator != (const NMRStarFile& f) )
	NMRStarFile f1;
	TEST_EQUAL(f1 != f1, false)
	TEST_EQUAL(f1 != f2, true)
RESULT


NMRStarFile empty;
CHECK(Size getNumberOfAtoms() const )
	TEST_EQUAL(f2.getNumberOfAtoms(), 343)
	TEST_EQUAL(empty.getNumberOfAtoms(), 0)
RESULT


CHECK(const std::vector<NMRAtomDataSet>& getNMRData() const)
	TEST_EQUAL(f2.getNMRData().size(), 1)
	TEST_EQUAL(empty.getNMRData().size(), 0)
RESULT


CHECK(void clear() )
	TEST_EQUAL(f2.getNMRData().size(), 1)
	TEST_EQUAL(f2.getNumberOfAtoms(), 343)
	f2.clear();
	TEST_EQUAL(f2.getNMRData().size(), 0)
	TEST_EQUAL(f2.getNumberOfAtoms(), 0)
RESULT
	

CHECK(String getResidueSequence())
	TEST_EQUAL(rs.getResidueSequence(), "GSCWAQSQGYNCCNNPSSTKVEYTDASGQWGVQNGQWCGIDYSYGQNQGNES")
RESULT
	
CHECK(bool hasHshifts())	
	TEST_EQUAL(rs.hasHshifts(), true)  
RESULT

CHECK(bool hasCshifts())
	TEST_EQUAL(rs.hasCshifts(), false)
RESULT

CHECK(bool hasNshifts())
	TEST_EQUAL(rs.hasNshifts(), true)
RESULT



CHECK(BALLToBMRBMapper)
	PDBFile pdb(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.pdb));

	BALL::System S;
	pdb >> S;
	Chain& chain = *(S.beginChain());

	NMRStarFile nmr_file(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.bmr));
	
	NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file, "AbrBN");
//	NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file, "'AbrBN subunit 1'");
	TEST_EQUAL(*mapper.getChain() == chain, true)
	TEST_EQUAL(*mapper.getNMRStarFile() == nmr_file, true)
	TEST_EQUAL(mapper.getNMRAtomDataSet() != NULL, true)
	const NMRStarFile::NMRAtomDataSet* nmr_atom_data_set = mapper.getNMRAtomDataSet();

	NMRStarFile nmr_file2(BALL_TEST_DATA_PATH(NMRStarFile_test.bmr));

	Chain& chain2 = *(S.beginChain()++);
	NMRStarFile::BALLToBMRBMapper mapper2(chain2, nmr_file2, "dockerin");
	
	TEST_EQUAL(*mapper2.getChain() == chain2, true)	
	TEST_EQUAL(*mapper2.getNMRStarFile() == nmr_file2, true)
	TEST_EQUAL(mapper2.getNMRAtomDataSet() != NULL, true)

	mapper2.setChain(chain);
	mapper2.setNMRStarFile(nmr_file);
	mapper2.setNMRAtomDataSet(*nmr_atom_data_set);
	TEST_EQUAL(*(mapper2.getChain()) == *(mapper.getChain()), true)
	TEST_EQUAL(mapper2.getNMRAtomDataSet() == nmr_atom_data_set, true)
	TEST_EQUAL(*(mapper2.getNMRStarFile()) == nmr_file, true)

	/*
	String ball_seq = Peptides::GetSequence(chain);
		std::cout << std::endl << ball_seq << std::endl;
		std::cout << nmr_file.getResidueSequence() << std::endl;
	*/

	TEST_EQUAL(mapper.createTrivialMapping(), true)
	TEST_EQUAL(mapper.getNumberOfMismatches(), 0)
	TEST_EQUAL(mapper.getNumberOfGaps(), 0)
	
	NMRStarFile:: BALLToBMRBMapper::BALLToBMRBMapping& all_mappings = mapper.getBALLToBMRBMapping();
	TEST_EQUAL(all_mappings.size(), 617)
	
	TEST_EQUAL(mapper.createMapping("MKSTGIVRKVDELGRVVIPIELRRTLGIAEKDALEIYVDDEKIIL-KKYKPNMT", "AKSTGIVRKVDELGRVVIPIELRRTLGIAEKDALEIYVDDEKIILKK-YKPNMT"), true)		
	TEST_EQUAL(mapper.getNumberOfMismatches(), 1)
	TEST_EQUAL(mapper.getNumberOfGaps(),2)

	NMRStarFile:: BALLToBMRBMapper::BMRBToBALLMapping& all_mappings2 = mapper.getBMRBToBALLMapping();
	TEST_EQUAL(all_mappings2.size(), 617)

	NMRStarFile::NMRAtomData const& nmr_atom = nmr_file.getNMRData()[0].atom_data[18];
	TEST_EQUAL(mapper.isMapped(nmr_atom), true)
	
	Atom const* atom = mapper.getBALLAtom(nmr_atom);
	TEST_EQUAL(atom != NULL, true)

	if (atom)
	{
		TEST_EQUAL(atom == mapper.getBALLAtom(nmr_atom), true)
		NMRStarFile::BALLToBMRBMapper::BMRBIndex index_mapping = mapper(atom);
		TEST_EQUAL(index_mapping.first, 0) 
		TEST_EQUAL(index_mapping.second, 18)
	}

	NMRStarFile::BALLToBMRBMapper::BMRBIndex mapping = all_mappings.find(atom)->second;
	NMRStarFile::NMRAtomData nmr_atom2 = (nmr_file.getNMRData()[mapping.first]).atom_data[mapping.second]; 
	TEST_EQUAL(nmr_atom == nmr_atom2, true)
	
RESULT



CHECK(read(AtomContainer& ac))	
	PDBFile pdb(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.pdb));
	BALL::System S;
	pdb >> S;

	NMRStarFile nmr_file(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.bmr));
	TEST_EQUAL(nmr_file.read(S), true)
	
	NMRStarFile::NMRAtomData const& nmr_atom = nmr_file.getNMRData()[0].atom_data[18];
	TEST_EQUAL(nmr_atom.atom_ID, 19)
	TEST_EQUAL(nmr_atom.residue_seq_code, 3)
	TEST_EQUAL(nmr_atom.residue_label, "SER")
	TEST_EQUAL(nmr_atom.atom_name, "HB3")
	TEST_EQUAL(nmr_atom.atom_type, 'H')
	TEST_REAL_EQUAL(nmr_atom.shift_value, 3.709)
RESULT
 


 
CHECK(bool assignShifts(BALLToBMRBMapper& ball_to_bmrb_mapping))
	PDBFile pdb(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.pdb));
	BALL::System S;
	pdb >> S;

	FragmentDB fdb("");
	S.apply(fdb.normalize_names);
	
	Chain& chain = *(S.beginChain());
	NMRStarFile nmr_file(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.bmr));
	NMRStarFile::BALLToBMRBMapper mapper(chain, nmr_file, "AbrBN");

	// NOTE: for testing, we artificially create a gap. this leads to a lower number of assigned shifts later on
	//       (606 instead of 617)
	bool worked = mapper.createMapping("MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", 
																		 "MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT");
	TEST_EQUAL (worked, true)
	if (worked)
	{
 		nmr_file.assignShifts(mapper);
		TEST_EQUAL(nmr_file.getNumberOfShiftsAssigned(), 606)  

		NMRStarFile::NMRAtomData const& nmr_atom = nmr_file.getNMRData()[0].atom_data[0];
		Atom const* atom = mapper.getBALLAtom(nmr_atom); 

		TEST_NOT_EQUAL(atom, NULL)
		TEST_EQUAL(atom->getName(), "HA")

		if (atom)
		{
			TEST_EQUAL(atom->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT), true)

			if (atom->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT))
				TEST_REAL_EQUAL(atom->getProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT).getFloat(), 4.804f)
		}
		
		NMRStarFile::NMRAtomData const& nmr_atom2 = nmr_file.getNMRData()[0].atom_data[22];
		Atom const* atom2 = mapper.getBALLAtom(nmr_atom2);

		TEST_NOT_EQUAL(atom2, NULL)
		TEST_EQUAL(atom2->getName(), "N")

		if (atom2)
		{
			TEST_EQUAL(atom2->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT), true)

			if (atom2->hasProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT))
				TEST_REAL_EQUAL(atom2->getProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT).getFloat(), 118.691f)
		}
		
	}
RESULT


CHECK(assignShifts(AtomContainer& ac, const String& aligned_ball_sequence,
												const String& aligned_nmrstar_sequence))
	PDBFile pdb(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.pdb));
	BALL::System S;
	pdb >> S;

	FragmentDB fdb("");
	S.apply(fdb.normalize_names);
	
	NMRStarFile nmr_file(BALL_TEST_DATA_PATH(NMRStarFile_test_1z0r.bmr));
	
	bool worked = nmr_file.assignShifts(S, "AbrBN",
			"MKSTGIVRKVDELGR-VVIPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT", 
			"MKSTGIVRKVDELGRVV-IPIELRRTLGIAEKDALEIYVDDEKIILKKYKPNMT"  );
	TEST_EQUAL (worked, true)
	if (worked)
	{
		TEST_EQUAL(nmr_file.getNumberOfShiftsAssigned(), 606)  
	}
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
