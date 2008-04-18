// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/NMRStarFile.h>
#include <iostream> 
#include <fstream>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/NMR/shiftModule.h>

//#define NMRSTAR_DEBUG 1
#undef NMRSTAR_DEBUG

using namespace std;

namespace BALL
{
	const float    NMRStarFile::FLOAT_VALUE_NA    = Limits<float>::max();
	const int      NMRStarFile::INT_VALUE_NA    	= Limits<int>::max();
	const Position NMRStarFile::POSITION_VALUE_NA = Limits<Position>::max();

	NMRStarFile::NMRAtomData::NMRAtomData()
		: atom_ID(0),
			residue_seq_code(0),
			residue_label(""),
			atom_name(""),
			atom_type(0),
			shift_value(0),
			error_value(0),
			ambiguity_code(0)
	{
	}


	NMRStarFile::ShiftReferenceSet::ShiftReferenceSet()
		: name(),
			elements()
	{
	}

	ostream& NMRStarFile::NMRAtomData::operator >> (ostream &s)	
	{
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << "atom_ID: "					  << atom_ID ;
		s << " residue_seq_code: "  << residue_seq_code;
		s << " residue_label: "		  << residue_label;
		s << " atom_name: "					<< atom_name;
		s << " shift_value: "				<< shift_value;
		s << " error_value: "				<< error_value;
		s << " ambiguity_code: "		<< ambiguity_code << endl;
		return s;
	}

	ostream& NMRStarFile::SampleCondition::operator >> (std::ostream& s)		
	{
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		if (values.size() != errors.size() && errors.size() != units.size())
		{
			Log.warn() << "Missing values in Sample Conditions." << std::endl;
			return s;
		}
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "set " << name  << std::endl;
		for (Size i=0; i < types.size(); i++)
		{
					s << types[i] << " = " << values[types[i]] << " " << units[types[i]] << " with error " << errors[types[i]] << std::endl;  
		}

		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	NMRStarFile::Sample::Component::Component()
		: label(""),
		concentration_value(0.),
		value_unit(""),
		concentration_min(0.),
		concentration_max(0.),
		isotopic_labeling("")
	{
	}

	void  NMRStarFile::Sample::Component::clear()
	{
		label = "";
		concentration_value = 0.;
		value_unit = "";
		concentration_min = 0.;
		concentration_max = 0.;
		isotopic_labeling = "";
	}

	ostream& NMRStarFile::Sample::Component::operator >> (std::ostream& s)
	{	
		s << "   " << label << " :\t value: " << concentration_value 
			<< " unit: " << value_unit 
			<< " concentration_min: " << concentration_min
			<< " concentration_max: " << concentration_max
			<< " isotopic labeling: " << isotopic_labeling << std::endl;
		return s;
	}

	NMRStarFile::Sample::Sample()
		: label(""),
			type(""),
			details(""),
			components()
	{
	}

	void  NMRStarFile::Sample::clear()
	{
		label = "";
		type = "";
		details = "";
		components.clear();
	}


	ostream& NMRStarFile::Sample::operator >> (std::ostream& s)
	{	
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "label: " << label
			<< " type: " << type
			<< " details : " << details << endl;
		s	<< " components: " << endl;
		for (Size i=0; i< components.size(); i++)
		{
			if (i!=0)
				s << "------------------------------------" << endl;
			components[i] >> s;
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}



	ostream& NMRStarFile::ShiftReferenceElement::operator >> (std::ostream& s)		
	{
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << endl;
		s << "mol_common_name: "		<< mol_common_name << endl;
		s << "atom_type: "					<< atom_type << endl;
		s << "isotope_number: "		<< isotope_number << endl;
		s << "atom_group: "				<< atom_group << endl;
		s << "shift_units: "				<< shift_units << endl;
		s << "shift_value: "				<< shift_value << endl;
		s << "reference_method: "  << reference_method << endl;
		s << "reference_type: " 		<< reference_type << endl;
		s << "indirect_shift_ratio: " <<  indirect_shift_ratio << endl << endl;
		return s;
	}
	
	ostream& NMRStarFile::ShiftReferenceSet::operator >> (std::ostream& s)	
	{
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name " << name << endl << endl;
		for (Position pos = 0; pos < elements.size() ; pos++ )
		{
			elements[pos] >> s;
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	NMRStarFile::NMRAtomDataSet::NMRAtomDataSet(NMRStarFile* parent)
		: name(),
			condition(),
			reference(),
			samples(),
			parent_(parent)
	{
	}

	ostream& NMRStarFile::NMRAtomDataSet::operator >> (std::ostream& s)		
	{
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << endl << "name " << name << endl << endl;

		for (Position pos = 0; pos < atom_data.size() ; pos++)
		{
			atom_data[pos] >> s;
		}
		
		s << endl;
		if (parent_)
		{
			parent_->getSampleConditionByName(condition) >> s;
			parent_->getShiftReferenceSetByName(reference) >> s;
			for (Size i=0; i< samples.size(); i++)
				parent_->getSample(samples[i]) >> s;
			s << endl;
		}
		return s;
	}

	NMRStarFile::ShiftReferenceElement::ShiftReferenceElement()	
		: atom_type(),
			isotope_number(0),
			atom_group(),
			shift_units(),
			shift_value(),
			reference_method(),
		  reference_type	(),
			indirect_shift_ratio()
	{
	}

	NMRStarFile::SampleCondition::SampleCondition()	
		: types(),
			values(),
			errors(),
			units()
	{
	}

	
	NMRStarFile::EntryInformation::EntryInformation() 
		: entry_type(),
		  BMRB_accession_code(),
		  NMR_STAR_version(),
		  experimental_method()
	{
	}

	NMRStarFile::EntryInformation::~EntryInformation() 
	{
	}
	ostream& NMRStarFile::EntryInformation::operator >> (std::ostream& s)
	{	
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"  << endl;
		s << "BMRB_accession_code:" << BMRB_accession_code  << endl;
 		s << "entry_type: "  			<< entry_type						<< endl;
		s << "NMR_STAR_version: "	<< NMR_STAR_version			<< endl;
		s << "experimental_method: "<< experimental_method << std::endl;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	NMRStarFile::MolecularSystem::ChemicalUnit::ChemicalUnit()
		: component_name(),
			label(),
			monomeric_polymer(NULL),
			shifts(NULL)
	{}
	ostream& NMRStarFile::MolecularSystem::ChemicalUnit::operator >> (std::ostream& s)
	{
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"  << endl;
		s << "component_name:" << component_name  << endl;
 		s << "label: "  			<< label						<< endl;
		if (monomeric_polymer)
			*monomeric_polymer >> s;
		if (shifts)
			*(shifts) >> s ;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	void NMRStarFile::MolecularSystem::ChemicalUnit::clear()
	{
		component_name = ""; 
		label = "";
		monomeric_polymer = NULL;
		shifts = NULL;
	}

		
	NMRStarFile::MolecularSystem::RelatedDB::RelatedDB() 
		: name(), 
			accession_code(), 
			entry_mol_name(),
			relation_type(), 
			details()
	{}
	
	void NMRStarFile::MolecularSystem::RelatedDB::clear() 
	{
		name = "";
		accession_code = "";
    entry_mol_name = "";
    relation_type = "";
		details = "";
	}

	ostream& NMRStarFile::MolecularSystem::RelatedDB::operator >> (std::ostream& s)
	{	
		s << name << " " << accession_code << " " << entry_mol_name << " " <<  relation_type << " " <<  details << endl;
		return s;
	}

	
	NMRStarFile::MonomericPolymer::HomologDB::HomologDB()
		: name(),
      accession_code(),
      entry_mol_name(),
      seq_to_submitted_percentage(0.),
      subject_length(0.),
      seq_identity(0.),
      seq_positive(0.),
			homology_expectation_value(0.)
	{}
	
	void NMRStarFile::MonomericPolymer::HomologDB::clear() 
	{
		name = "";
    accession_code = "";
    entry_mol_name = "";
    seq_to_submitted_percentage = 0.;
    subject_length = 0.;
    seq_identity = 0.;
    seq_positive = 0.;
    homology_expectation_value = 0.;  
	}

	ostream& NMRStarFile::MonomericPolymer::HomologDB::operator >> (std::ostream& s)
	{
		s << "  " << name << " " 
			<< accession_code << " " 
			<< entry_mol_name << " " 
			<< seq_to_submitted_percentage << " " 
			<< subject_length << " " 
			<< seq_identity << " " 
			<< seq_positive << " " 
			<< homology_expectation_value 
			<< endl; 
		return s;
	}
	
	NMRStarFile::MonomericPolymer::MonomericPolymer() 
		: label_name(),    
			type(),          
			polymer_class(), 
			common_name(),   
			name_variant(),  
			molecular_mass(0.),
      details(),
			number_of_residues(0),
			residue_sequence(),
			residues_by_index(),
			homolog_database_entries()
	{}
	
	void NMRStarFile::MonomericPolymer::clear() 
	{	
		label_name = "";    
    type = "";          
    polymer_class = ""; 
    common_name = "";   
    name_variant = "";  
    molecular_mass = 0.,
    details = "";
    number_of_residues = 0;
    residue_sequence = "";
    residues_by_index.clear();
    homolog_database_entries.clear();
	}


	ostream& NMRStarFile::MonomericPolymer::operator >> (std::ostream& s)
	{	
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"  << endl;
		s << "Monomeric Polymer:" << label_name << endl;
 		s << "type: "  						<< type						<< endl;
		s << "polymer_class: "		<< polymer_class			<< endl;
		s << "common_name: " 			<< common_name << std::endl;
		s << "name_variant: " 		<< name_variant << std::endl;
		s << "molecular_mass: " 	<< molecular_mass << std::endl;
		s << "details " 					<< details << std::endl;
		s << "number_of_residues: " << number_of_residues << std::endl;
		s << "residue_sequence " << residue_sequence << std::endl;
		//s << "residues per index :" << endl;	
		// TODO residue per index
		s << "other databases with homologs :" << endl;	
		for (Size i=0; i < homolog_database_entries.size(); i++)
		{
			homolog_database_entries[i] >> s;
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;	
		return s;
	}

	NMRStarFile::MolecularSystem::MolecularSystem() 
		: system_name(),
			abbreviation_common(),
			chemical_units(),
			system_physical_state(),
      system_oligomer_state(),
      system_paramagnetic(),
      system_thiol_state(),
      system_molecular_weight(),
			related_database_entries()
	{
	}

	NMRStarFile::MolecularSystem::~MolecularSystem() 
	{
	}

	bool NMRStarFile::isMonomericPolymer(String chemical_unit_label) 
	{
		return monomeric_polymer_indices_.has(chemical_unit_label); 
	}
 
	void NMRStarFile::addMonomericPolymer(NMRStarFile::MonomericPolymer mp) 
	{
		if (hasMonomericPolymer(mp.label_name))
		{
			Log.warn() << "A Monomer with that name was overwritten!" << std::endl;  
			// delete the old entry
			vector<NMRStarFile::MonomericPolymer>::iterator mpi = monomeric_polymers_.begin() + monomeric_polymer_indices_[mp.label_name] ;
			monomeric_polymers_.erase(mpi);
			//monomeric_polymers_.erase(monomeric_polymer_indices_[mp.label_name]);
		}	
		monomeric_polymers_.push_back(mp);
		monomeric_polymer_indices_[mp.label_name] = monomeric_polymers_.size()-1;
	}

	void NMRStarFile::MolecularSystem::clear()
	{
		system_name = "";
		abbreviation_common = "";
		chemical_units.clear();
		system_physical_state = "";
    system_oligomer_state = "";
    system_paramagnetic = "";
    system_thiol_state = "";

		system_molecular_weight = 0;
		related_database_entries.clear();
	}
	
// TODO :	monomeric_polymers
	ostream& NMRStarFile::MolecularSystem::operator >> (std::ostream& s)
	{	
		// TODO: if POSITION_VALUE_NA, FLOAT_VALUE_NA ... -> NA ausgeben
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "system_name:" << system_name												<< endl;
 		s << "abbreviation_common: "  	<< abbreviation_common 		<< endl;
		s << "system_physical_state: "	<< system_physical_state	<< endl;
		s << "system_oligomer_state: "	<< system_oligomer_state	<< endl;
		s << "system_paramagnetic: "		<< system_paramagnetic		<< endl;
		s << "system_thiol_state: " 		<< system_thiol_state			<< endl;
		s << "system_molecular_weight: "<< system_molecular_weight << std::endl;

		s << "chemical units:" << std::endl; 
		for (Size i=0; i < chemical_units.size(); i++)
		{
			s << "  " << chemical_units[i].component_name << " : " << chemical_units[i].label << std::endl;
			*(chemical_units[i].monomeric_polymer) >> s;
		}
		s << "related database entries:" << std::endl;
		for (Size i=0; i < related_database_entries.size(); i++)
		{
			if (i!=0)
				s << "   -------------------------------------------" << endl;

			s << "   DB name: "				<< related_database_entries[i].name 						<< endl;
			s << "   accesion code: " 	<< related_database_entries[i].accession_code 	<< std::endl; 
			s << "   molecule name: " 	<< related_database_entries[i].entry_mol_name 	<< std::endl; 
			s << "   relation type: " 	<< related_database_entries[i].relation_type 	<< std::endl; 
			s << "   details : " 			<< related_database_entries[i].details 				<< std::endl; 	
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	ostream& NMRStarFile::NMRSpectrometer::operator >> (std::ostream& s)
	{	
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "manufacturer: " 	<< manufacturer 	<< endl;
		s << "model: " 					<< model 					<< endl;
		s << "field strength: " << field_strength << endl; 
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}


////////////////////////////////////////////////////////////////

	NMRStarFile::NMRStarFile()
		:	CIFFile(),
			number_of_shift_sets_(0),
			entry_information_(),
			molecular_system_(),
			atom_data_sets_(),
			sample_conditions_(),
			samples_(),
			shift_references_(), 
			nmr_spectrometer_(),
			monomeric_polymer_indices_(),
			monomeric_polymers_(),
			dummy_saveframe_(),
			dummy_sample_condition_(),
			dummy_sample_(),
			dummy_shift_reference_set_(),
			special_characters_(".?@")
	{
	}

		:	CIFFile(f),
			number_of_shift_sets_		(f.number_of_shift_sets_),
			entry_information_  (f.entry_information_),	
			molecular_system_   (f.	molecular_system_),
			samples_						(f.samples_),
			nmr_spectrometer_		(f.nmr_spectrometer_),
			monomeric_polymer_indices_(f.monomeric_polymer_indices_),
			monomeric_polymers_	(f.monomeric_polymers_),
			dummy_saveframe_		(f.dummy_saveframe_),
			dummy_sample_condition_(f.dummy_sample_condition_),
			dummy_sample_				(f.dummy_sample_),
			dummy_shift_reference_set_(f.dummy_shift_reference_set_),
			special_characters_	(f.special_characters_)
	NMRStarFile::NMRStarFile(const String& file_name, File::OpenMode open_mode)
		throw (Exception::FileNotFound)
		:	CIFFile(file_name, open_mode),
			number_of_shift_sets_(0),
			entry_information_(),
			molecular_system_(),
			atom_data_sets_(),
			sample_conditions_(),
			samples_(),
			shift_references_(),
			nmr_spectrometer_(),	
			monomeric_polymer_indices_(),
			monomeric_polymers_(),
			dummy_saveframe_(),
			dummy_sample_condition_(),
			dummy_sample_(),
			dummy_shift_reference_set_(),
			special_characters_(".?@")
	{
	}

	NMRStarFile::~NMRStarFile()		
	{
	}

	const NMRStarFile& NMRStarFile::operator = (const NMRStarFile& f)		
	{
		CIFFile::operator = (f);

		number_of_shift_sets_		= f.number_of_shift_sets_;
		entry_information_  = f.entry_information_;
		molecular_system_   = f.molecular_system_; 
		atom_data_sets_			= f.atom_data_sets_;
		sample_conditions_  = f.sample_conditions_ ;
		samples_						= f.samples_;
		shift_references_   = f.shift_references_;
		nmr_spectrometer_		= f.nmr_spectrometer_;	
		monomeric_polymer_indices_ 	= f.monomeric_polymer_indices_;
		monomeric_polymers_ 				= f.monomeric_polymers_;
		dummy_saveframe_						= f.dummy_saveframe_;
		dummy_sample_condition_			= f.dummy_sample_condition_;
		dummy_sample_								= f.dummy_sample_;
		dummy_shift_reference_set_	= f.dummy_shift_reference_set_;	
		special_characters_	 				= f.special_characters_;
	
		return *this;
	}

	bool NMRStarFile::read() 
		throw(Exception::ParseError)
	{

#ifdef NMRSTAR_DEBUG
	std::cout << "CIFFile::read()" << std::endl;
#endif
		CIFFile::read();
		//try 
		//{
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readEntryInformation_()" << std::endl;
#endif
			readEntryInformation_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readMolSystem_()" << std::endl;
#endif
			readMolSystem_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readMonomericPolymers_()" << std::endl;
#endif
			readMonomericPolymers_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readSampleConditions_()" << std::endl;
#endif
			readSampleConditions_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readShiftReferences_()" << std::endl;
#endif
			readShiftReferences_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readSamples_()" << std::endl;
#endif
			readSamples_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::readNMRSpectrometer_()" << std::endl;
#endif
			readNMRSpectrometer_();
#ifdef NMRSTAR_DEBUG	
	std::cout << "NMRStarFile::readShifts_()" << std::endl;
#endif
			readShifts_();
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::findDependiencies_()" << std::endl;	
#endif
			findDependiencies_();
		//}
		/*catch (Exception::GeneralException e)
		{
			throw Exception::ParseError(e.getFile(), e.getLine(), String("NMRStarFile: ") + e.getMessage(), "");
		}
		catch (...)
		{
			throw Exception::ParseError(__FILE__, __LINE__, "NMRStarFile: caught unexpected exception while reading file.", "");
		}*/
		return true;
	}

	bool NMRStarFile::read(AtomContainer& ac)
	{
		bool result = read();
		if (result)
		{
			// first check, if the given AtomContainer is a valid protein
			Protein* protein;
   		if (RTTI::isKindOf<Protein>(ac))
    	{
      	protein = RTTI::castTo<Protein>(ac);
    	}
    	else if (RTTI::isKindOf<System>(ac))
    	{
				System* system =  RTTI::castTo<System>(ac);
      	if (system->countProteins() > 0)
      	{	 
        	ProteinIterator pit = system->beginProtein();
        	// we take the first protein of the system
        	protein = RTTI::castTo<Protein>(*pit);
     		}
				else
				{
					Log.error() << "NMRStarfile::read(): No protein found in the system." << endl;
					return false;
				}
    	} 
    	else
    	{
      	Log.error() << "NMRStarfile::read(): Cannot assign shifts to a non-protein." << endl;
      	return false;
    	}
			
			// Since no explicit mapping AtomContainer-->BMRB
			// is given, we try to find a trivial mapping 
			// by exactly matching residues and atom names
			BALLToBMRBMapping pdb_to_bmrb_mapping;
			Position i=1;
					
			// create the mapping: take the 
			for (ResidueIterator res_it = protein->beginResidue(); +res_it; res_it++)
			{
				for (AtomIterator at_it = res_it->beginAtom(); +at_it; at_it++)
				{
					// TODO: we should move the argument of the BALLTo...Mapping to Position instead of String
					pdb_to_bmrb_mapping.mapTo(&*at_it, String(i), at_it->getName());
				}
				++i;
			}

			// now assign the shifts via the mapping
			result = assignShifts_(ac, pdb_to_bmrb_mapping);
		}
		return result;
	}

	bool NMRStarFile::read(	AtomContainer& ac,  
													BALLToBMRBMapping& pdb_to_bmrb_mapping)
	{
		bool result = read();
		if (result)
		{
			result = assignShifts_(ac, pdb_to_bmrb_mapping);
		}
		return result;
	}


  //  Apply the shifts read into to the AtomContainer as denoted in the mapping.
	//  We assume, that the file was already read!
	bool NMRStarFile::assignShifts_(	AtomContainer& ac, 
																		BALLToBMRBMapping& pdb_to_bmrb_mapping)
	{
    Protein* protein;
    if (RTTI::isKindOf<Protein>(ac))
    {
      protein = RTTI::castTo<Protein>(ac);
    }
    else if (RTTI::isKindOf<System>(ac))
    {
			System* system =  RTTI::castTo<System>(ac);
      if (system->countProteins() > 0)
      { 
        ProteinIterator pit = system->beginProtein();
        // we take the first protein of the system
        protein = RTTI::castTo<Protein>(*pit);
      }
			else
			{
				Log.error() << "NMRStarfile::assignShifts: No protein found in the system." << endl;
				return false;
			}
    } 
    else
    {
      Log.error() << "NMRStarfile::assignShifts: Cannot assign shifts to a non-protein." << endl;
      return false;
    }

		// map the shifts via the pdb_bmrb_mapping into the given AtomContainer
		for (ResidueIterator r_it = protein->beginResidue(); +r_it; r_it++)
		{
			for (AtomIterator a_it = r_it->beginAtom(); +a_it; a_it++)
			{
				if (pdb_to_bmrb_mapping.hasAtom(&*a_it))
				{
					BALLToBMRBMapping::BMRBIndex& bindex = pdb_to_bmrb_mapping(&*a_it);

					std::pair<String, String> index(bindex.residue_id, bindex.atom_name);
					if (bmrb_to_shifts_mapping_.find(index) != bmrb_to_shifts_mapping_.end())
					{
						a_it->setProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT,	
															bmrb_to_shifts_mapping_[index]);
					}
				}
			}
		}
    
		return true;
	}




	{
		Size max = 0;
		
		for (Position pos = 0;  pos < atom_data_sets_.size(); pos++)
		{
			if (atom_data_sets_[pos].atom_data.size() > max)
			{
				max = (Size)atom_data_sets_[pos].atom_data.size();
			}
		}
		
		return max;
	}

	
	const std::vector<NMRStarFile::NMRAtomDataSet>& NMRStarFile::getNMRData() const
	{
		return atom_data_sets_;
	}

	NMRStarFile::SampleCondition&	NMRStarFile::getSampleConditionByName(String condition) 
	{
		for (Size i=0; i < sample_conditions_.size(); i++)
		{
			if (sample_conditions_[i].name == condition)
			{
				return sample_conditions_[i]; 
			}
		}
		Log.warn() << "Returned a dummy sample condition!" << std::endl;
		return dummy_sample_condition_;
	}

	const NMRStarFile::SampleCondition&	NMRStarFile::getSampleConditionByName(String condition) const
	{
		for (Size i=0; i < sample_conditions_.size(); i++)
		{
			if (sample_conditions_[i].name == condition)
			{
				return sample_conditions_[i]; 
			}
		}
		Log.warn() << "Returned a dummy sample condition!" << std::endl;
		return dummy_sample_condition_;
	}
	
	
	bool  NMRStarFile::hasSample(String label) const 
	{
		for (Size i=0; i < samples_.size(); i++)
		{
			if (samples_[i].label == label)
			{
				return true;		
			}
		}
		return false;
	}

	NMRStarFile::Sample NMRStarFile::getSample(Index i) const 
	{
		if (i < (Index)samples_.size())
			return samples_[i];
		else
		{	
			Log.warn() << "Returned a dummy sample!" << std::endl;
			return dummy_sample_;
		}
	}	
	
	NMRStarFile::Sample NMRStarFile::getSample(String label) const 
	{
		for (Size i=0; i < samples_.size(); i++)
		{
			if (samples_[i].label == label)
			{
				return samples_[i];		
			}
		}
		
		Log.warn() << "Returned a dummy sample!" << std::endl;
		return  dummy_sample_;
	}


	const  NMRStarFile::ShiftReferenceSet& NMRStarFile::getShiftReferenceSetByName(String name) const 
	{
		for (Size i=0; i < shift_references_.size(); i++)
		{
			if (shift_references_[i].name == name)
			{
				return shift_references_[i]; 
			}
		}
		Log.warn() << "Returned a dummy shift reference set!" << std::endl;

		return dummy_shift_reference_set_;
	}
	
	NMRStarFile::ShiftReferenceSet&  NMRStarFile::getShiftReferenceSetByName(String name) 
	{
		for (Size i=0; i < shift_references_.size(); i++)
		{
			if (shift_references_[i].name == name)
			{
				return shift_references_[i]; 
			}
		}
		Log.warn() << "Returned a dummy shift reference set!" << std::endl;

		return dummy_shift_reference_set_;
	}


	void NMRStarFile::readEntryInformation_()
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("entry_information"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("entry_information");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one entry information saveframe! " << std::endl; 
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					if (saveframes[sf].hasItem("_Entry_type"))
						entry_information_.entry_type = saveframes[sf].getItemValue("_Entry_type");
					else  entry_information_.entry_type = "";
					if (saveframes[sf].hasItem("_NMR_STAR_version"))
						entry_information_.NMR_STAR_version = saveframes[sf].getItemValue("_NMR_STAR_version");		
					else  entry_information_.NMR_STAR_version = "";

					if (saveframes[sf].hasItem("_BMRB_accession_number"))
						entry_information_.BMRB_accession_code = saveframes[sf].getItemValue("_BMRB_accession_number");	
					else  entry_information_.BMRB_accession_code = "";

					if (saveframes[sf].hasItem("_Experimental_method"))
						entry_information_.experimental_method = saveframes[sf].getItemValue("_Experimental_method");	
					else  entry_information_.experimental_method = "";
				}

			}

		}
	}

	void NMRStarFile::readMolSystem_()	
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("molecular_system"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("molecular_system");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one molecular system saveframe! " << std::endl; 
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					// read the paired entries

					if (saveframes[sf].hasItem("_Mol_system_name"))
						molecular_system_.system_name = saveframes[sf].getItemValue("_Mol_system_name");
					else  molecular_system_.system_name  = "";
					
					if (saveframes[sf].hasItem("_Abbreviation_common"))
						molecular_system_.abbreviation_common = saveframes[sf].getItemValue("_Abbreviation_common");
					else  molecular_system_.abbreviation_common = "";
									
					if (saveframes[sf].hasItem("_System_physical_state"))
						molecular_system_.system_physical_state = saveframes[sf].getItemValue("_System_physical_state");
					else  molecular_system_.system_physical_state = "";

					if (saveframes[sf].hasItem("_System_oligomer_state"))
						molecular_system_.system_oligomer_state = saveframes[sf].getItemValue("_System_oligomer_state");
					else  molecular_system_.system_oligomer_state = "";

					if (saveframes[sf].hasItem("_System_paramagnetic"))
						molecular_system_.system_paramagnetic = saveframes[sf].getItemValue("_System_paramagnetic");
					else  molecular_system_.system_paramagnetic = "";

					if (saveframes[sf].hasItem("_System_thiol_state"))
						molecular_system_.system_thiol_state = saveframes[sf].getItemValue("_System_thiol_state");
					else  molecular_system_.system_thiol_state = "";

					// in dalton
					if (saveframes[sf].hasItem("_System_molecular_weight"))
						molecular_system_.system_molecular_weight = 
							( valueIsValid(saveframes[sf].getItemValue("_System_molecular_weight")) 
								? saveframes[sf].getItemValue("_System_molecular_weight").toFloat()
								:  FLOAT_VALUE_NA  );
					else  molecular_system_.system_molecular_weight = FLOAT_VALUE_NA;

					//
					// read the loop entries
					//
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{	
							Item* current_loop = &saveframes[sf].items[loop];

							// read the chemical units!
							// first: Molecular system component name, 
							// second: label

							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0]== "_Mol_system_component_name")
							{
								// we have found the component name table :-)
								// store the data
								MolecularSystem::ChemicalUnit cu;
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{	
									cu.component_name  = current_loop->values[line][0];
									cu.label = current_loop->values[line][1];
									molecular_system_.chemical_units.push_back(cu);
								}
							}

							// read the related DB entries

							if (saveframes[sf].items[loop].keys[0]== "_Database_name")
							{
								NMRStarFile::MolecularSystem::RelatedDB db;
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{	
									db.name = current_loop->values[line][0];
									Index pos = current_loop->getKeyIndex("_Database_accession_code");
									if ( pos > -1) db.accession_code = current_loop->values[line][pos];
									
									pos = current_loop->getKeyIndex("_Database_entry_mol_name");
									if ( pos > -1) db.entry_mol_name = current_loop->values[line][pos];
									
									pos = current_loop->getKeyIndex("_Database_entry_relation_type");
									if ( pos > -1) db.relation_type = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Database_entry_details");
									if ( pos > -1) db.details = current_loop->values[line][pos];
								}
									molecular_system_.related_database_entries.push_back(db);
							}

						} // if loop
					} // end for all loops

				} // end of for all saveframes
			}
		}
	}

	void NMRStarFile::readMonomericPolymers_()
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("monomeric_polymer"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("monomeric_polymer");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one monomeric polymer saveframe! " << std::endl; 
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					NMRStarFile::MonomericPolymer mp;
					mp.label_name = saveframes[sf].framename;

					// read the paired entries	
				
					if (saveframes[sf].hasItem("_Mol_type"))
						mp.type = saveframes[sf].getItemValue("_Mol_type");
					else   mp.type = "";
				
					if (saveframes[sf].hasItem("_Mol_polymer_class"))
						mp.polymer_class = saveframes[sf].getItemValue("_Mol_polymer_class");
					else   mp.polymer_class = "";
				
					if (saveframes[sf].hasItem("_Name_common"))
						mp.common_name = saveframes[sf].getItemValue("_Name_common");
					else   mp.common_name = "";
					
					if (saveframes[sf].hasItem("_Name_variant"))
						mp.name_variant = saveframes[sf].getItemValue("_Name_variant");
					else   mp.name_variant = "";
					
					if (saveframes[sf].hasItem("_Molecular_mass"))
						mp.molecular_mass = 
							( valueIsValid(saveframes[sf].getItemValue("_Molecular_mass")) 
								? saveframes[sf].getItemValue("_Molecular_mass").toFloat() :  FLOAT_VALUE_NA );
					else  mp.molecular_mass = 0;
					
					if (saveframes[sf].hasItem("_Details"))
						mp.details = saveframes[sf].getItemValue("_Details");
					else   mp.details = "";

					if (saveframes[sf].hasItem("_Residue_count"))
						mp.number_of_residues = 
							( valueIsValid(saveframes[sf].getItemValue("_Residue_count")) 
								? saveframes[sf].getItemValue("_Residue_count").toInt() : INT_VALUE_NA );
					else  mp.number_of_residues  = 0;

					if (saveframes[sf].hasItem("_Mol_residue_sequence"))
					{
						mp.residue_sequence = saveframes[sf].getItemValue("_Mol_residue_sequence").trim("; \n");
						while ( mp.residue_sequence.hasSubstring("\n"))
						{
							mp.residue_sequence.substitute("\n", "");
						}
					}
					else { mp.residue_sequence = "";}

					// read the loops
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							Item* current_loop = &saveframes[sf].items[loop];

							// read the residues per sequence code
							if (   (saveframes[sf].items[loop].keys[0]== "_Residue_seq_code")
									&& (saveframes[sf].items[loop].keys[1]== "_Residue_label"))
							{
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									String idx =  current_loop->values[line][0];
									String res =  current_loop->values[line][1];
									mp.residues_by_index[idx] = res;
								}
							}
							
							// read the table of databases with homologs
							if (saveframes[sf].items[loop].keys[0]== "_Database_name")
							{
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									NMRStarFile::MonomericPolymer::HomologDB hdb; 
									hdb.name =  current_loop->values[line][0];
	
									Index pos = current_loop->getKeyIndex("_Database_accession_code");
									if ( pos > -1) hdb.accession_code = current_loop->values[line][pos];
									
									pos = current_loop->getKeyIndex("_Database_entry_mol_name");
									if ( pos > -1) hdb.entry_mol_name = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Sequence_query_to_submitted_percentage");
									hdb.seq_to_submitted_percentage  = (((pos>-1 ) && valueIsValid(current_loop->values[line][pos])) 
																				? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_subject_length");
									hdb.subject_length  = (((pos>-1 ) && valueIsValid(current_loop->values[line][pos])) 
																				? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
			
									pos = current_loop->getKeyIndex("_Sequence_identity");
									hdb.seq_identity  = (((pos>-1 ) && valueIsValid(current_loop->values[line][pos])) 
																				? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_positive");
									hdb.seq_positive  = (((pos>-1 ) && valueIsValid(current_loop->values[line][pos])) 
																				? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_homology_eypectation_value");
									hdb.homology_expectation_value  = (((pos>-1 ) && valueIsValid(current_loop->values[line][pos])) 
																				? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
									
									mp.homolog_database_entries.push_back(hdb);
								}
							}
						}
					}// end of for all items
					// store this Monomer
					addMonomericPolymer(mp);
					
				/*	// check, to which chemical unit this monomer belongs
					for (Size j=0; j < chemical_units.size(); j++)
					{
						if (chemical_units[j].label.trim("$") = mp.label_name)
							chemical_units[j].monomeric_polymer = &(monomeric_polymer[mp.label_name]);
					}*/

				} // endo for all polymeric polymers
			} // end of has polymeric monomer
		}
	}
	
	void NMRStarFile::readSampleConditions_()
	{	
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("sample_conditions"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("sample_conditions");
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					// look for the correct loop structure...
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0]== "_Variable_type")
							{
								SampleCondition tmp;
								tmp.name =  saveframes[sf].framename;
								// we have found the sample condition values :-)
								// store the data
								Item* current_loop = &saveframes[sf].items[loop];

								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									// accoring to the NMRStarFile 2.1 documentation, 
									// naming the first entry "_Variable_type" is mandatory
									// this is why we are allowed to map per type :-)
									tmp.types.push_back(current_loop->values[line][0]);
									if ( current_loop->keys[1] == "_Variable_value")
									{
										tmp.values[current_loop->values[line][0]] = 
											( valueIsValid(current_loop->values[line][1])
											 ? current_loop->values[line][1].toFloat() : FLOAT_VALUE_NA );
									}
									if ( current_loop->keys[2] == "_Variable_value_error")
									{
										tmp.errors[current_loop->values[line][0]] = 
											( valueIsValid(current_loop->values[line][2])
												? current_loop->values[line][2].toFloat() : FLOAT_VALUE_NA );
									}
									if ( current_loop->keys[3] == "_Variable_value_units")
									{
										tmp.units[current_loop->values[line][0]] = current_loop->values[line][3];
									}
								}
								sample_conditions_.push_back(tmp);
							}

						}
					}
				}
			}
		}
	}


	void NMRStarFile::readShiftReferences_()
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("chemical_shift_reference"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("chemical_shift_reference");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one chemical_shift_reference saveframe! " << std::endl; 

				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					// look for the correct loop structure...
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0]== "_Mol_common_name")
							{
								// we have found the shift references :-)
								// store the data
								Item* current_loop = &saveframes[sf].items[loop];

								NMRStarFile::ShiftReferenceSet reference_set;
								reference_set.name = saveframes[sf].framename;							
						
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{	
									NMRStarFile::ShiftReferenceElement ref_element;
									Index pos = current_loop->getKeyIndex("_Mol_common_name");
									if ( pos > -1)
									{
										ref_element.mol_common_name =  current_loop->values[line][pos];
									}
									
									pos = current_loop->getKeyIndex("_Atom_type");
									if ( pos > -1)
									{
										ref_element.atom_type  =  current_loop->values[line][pos].toChar();
									}
									
									pos = current_loop->getKeyIndex("_Atom_isotope_number");
									if ( pos > 1)
									{
											ref_element.isotope_number  = ( valueIsValid(current_loop->values[line][pos]) 
													                         ? (Position)current_loop->values[line][pos].toInt() : INT_VALUE_NA );
									}
									
									pos = current_loop->getKeyIndex("_Atom_group");
									if ( pos > 1)
									{
										ref_element.atom_group   =  current_loop->values[line][pos];
									}
									pos = current_loop->getKeyIndex("_Chem_shift_units");
									if ( pos > -1)
									{
										ref_element.shift_units  =  current_loop->values[line][pos];
									}
									
									pos = current_loop->getKeyIndex("_Chem_shift_value");
									if ( pos > -1)
									{
											ref_element.shift_value  = ( valueIsValid(current_loop->values[line][pos]) 
												 												 ?  current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);	
									}
									
									pos = current_loop->getKeyIndex("_Reference_method");
									if ( pos > -1)
									{
										ref_element.reference_method  = current_loop->values[line][pos].toChar();
									}

									pos = current_loop->getKeyIndex("_Reference_type");
									if ( pos > -1)
									{
										ref_element.reference_type  = current_loop->values[line][pos].toChar();
									}
									
									pos = current_loop->getKeyIndex("_Indirect_shift_ratio");
									if ( pos > -1)
									{
										ref_element.indirect_shift_ratio  = 
											(valueIsValid(current_loop->values[line][pos]) 
											 ? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA); 
									}
									
									reference_set.elements.push_back(ref_element);
								}
								
								shift_references_.push_back(reference_set);
							}
						}
					}
				}
			}
		}
	}

	void NMRStarFile::readShifts_()
	{
		// clear the bmrb_to_shifts map
		bmrb_to_shifts_mapping_.clear();

		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("assigned_chemical_shifts"))
			{

				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("assigned_chemical_shifts");
				if (saveframes.size() > 1)
					Log.warn() << "NMRStarfile::readShifts(): File has more than one assigned_chemical_shifts saveframe! " << std::endl; 
				number_of_shift_sets_ =  saveframes.size();
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					// look for the correct loop structure...
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0]== "_Atom_shift_assign_ID")
							{
								// we have found the shift values :-)
								// store the data
								Item* current_loop = &saveframes[sf].items[loop];

								NMRStarFile::NMRAtomDataSet atom_data_set(this);

								for (Size line = 0; line < current_loop->values.size(); line++ )
								{	
									NMRAtomData atom_data;

									// empty values are denoted by '.' what shall we do?
									Index pos = current_loop->getKeyIndex("_Atom_shift_assign_ID");
									atom_data.atom_ID = (((pos > -1) && valueIsValid(current_loop->values[line][0]) )
																			?  current_loop->values[line][0].toUnsignedInt() : POSITION_VALUE_NA);
									pos = current_loop->getKeyIndex("_Residue_seq_code");
									atom_data.residue_seq_code = (((pos > -1) && valueIsValid(current_loop->values[line][pos]))
																								? current_loop->values[line][1].toUnsignedInt() : POSITION_VALUE_NA);
									pos = current_loop->getKeyIndex("_Residue_label");
									if (pos > -1) atom_data.residue_label = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Atom_name");
									if (pos > -1) atom_data.atom_name = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Atom_type");
									if (pos > -1) atom_data.atom_type = current_loop->values[line][pos].toChar();
							
									pos = current_loop->getKeyIndex("_Chem_shift_value");
									atom_data.shift_value = ( (pos > -1) && valueIsValid(current_loop->values[line][pos]) 
										 												? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
									
									pos = current_loop->getKeyIndex("_Chem_shift_value_error");
									atom_data.error_value = ( (pos > -1) && valueIsValid(current_loop->values[line][pos])
																						?  current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Chem_shift_ambiguity_code");
									atom_data.ambiguity_code = ( (pos > -1) && valueIsValid(current_loop->values[line][pos])
																							?  current_loop->values[line][pos].toUnsignedInt() : INT_VALUE_NA);
									// store in the NMRdataset
									atom_data_set.atom_data.push_back(atom_data);

									//store in the bmrb_to_shifts map
									std::pair<String, String> tuple(atom_data.residue_seq_code, atom_data.atom_name);
									bmrb_to_shifts_mapping_[tuple] = atom_data.shift_value;	
								}

								// look for the sample conditions
								if (saveframes[sf].hasItem("_Sample_conditions_label"))
								{	
									//store the name
									atom_data_set.condition = (saveframes[sf].getItemValue("_Sample_conditions_label")).trim("$");
									//atom_data_set.condition = getSampleConditionByName(condition);
		
								}

								// look for the chemical shift reference
								if (saveframes[sf].hasItem("_Chem_shift_reference_set_label"))
								{		
									//store the name
									atom_data_set.reference = (saveframes[sf].getItemValue("_Chem_shift_reference_set_label")).trim("$");
									
									//atom_data_set.reference = getShiftReferenceSetByName(reference);
								}
							
								// look for the samples
								for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
								{
									if (saveframes[sf].items[loop].is_loop)
									{	
										Item* current_loop = &saveframes[sf].items[loop];

										if (saveframes[sf].items[loop].keys[0]== "_Sample_label")
										{
											for (Size line = 0; line < current_loop->values.size(); line++ )
											{	
												//store the labels
												atom_data_set.samples.push_back(current_loop->values[line][0]);
											}
										}	
									}	

								}
								// store this set 
								atom_data_sets_.push_back(atom_data_set);
								
							} // end of a shift loop
						}// end of a loop
					
					}// for all items
				} // end of for all saveFrames 
			}
		}
	}

	void  NMRStarFile::readSamples_() 
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("sample"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("sample");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one sample saveframe! " << std::endl; 

				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					NMRStarFile::Sample sample;
					sample.label = saveframes[sf].framename;
					if (saveframes[sf].hasItem("_Sample_type"))
						sample.type = (saveframes[sf].getDataItemValue("_Sample_type"));
					if (saveframes[sf].hasItem("_Details"))
						sample.details = (saveframes[sf].getDataItemValue("_Details"));
					
					// read the loops
					for (Size loop=0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{
							Item* current_loop = &saveframes[sf].items[loop];
							if ( saveframes[sf].items[loop].keys[0]== "_Mol_label")
							{
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									NMRStarFile::Sample::Component component;
									component.label = current_loop->values[line][0];
										
									Index pos = current_loop->getKeyIndex("_Concentration_value");
									if ( pos > -1) 
									{ 
										component.concentration_value =  (valueIsValid(current_loop->values[line][pos]) 
																										? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
									}	

									pos = current_loop->getKeyIndex("_Concentration_value_units");
									if ( pos > -1) component.value_unit  = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Concentration_min_value");
									if ( pos > -1)
									{
										component.concentration_min =  (valueIsValid(current_loop->values[line][pos]) 
																									? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
									}
									
									pos = current_loop->getKeyIndex("_Concentration_max_value");
									if ( pos > -1) 
									{
										component.concentration_max  =  (valueIsValid(current_loop->values[line][pos]) 
																									? current_loop->values[line][pos].toFloat() : FLOAT_VALUE_NA);
									}

									pos = current_loop->getKeyIndex("_Isotopic_labeling");
									if ( pos > -1) component.isotopic_labeling  = current_loop->values[line][pos];

									sample.components.push_back(component);
								}
							}
						}
					}

					// store the sample
					samples_.push_back(sample);
				}
			}
		}
	}

	void NMRStarFile::readNMRSpectrometer_()
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("NMR_spectrometer"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("NMR_spectrometer");
				if (saveframes.size() > 1)
					Log.warn() << "NMRFile has more than one NMR spectrometer  saveframe! " << std::endl; 

				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					if (saveframes[sf].hasItem("_Manufacturer"))
						nmr_spectrometer_.manufacturer = (saveframes[sf].getDataItemValue("_Manufacturer"));
					if (saveframes[sf].hasItem("_Model"))
						nmr_spectrometer_.model = saveframes[sf].getDataItemValue("_Model");
					if (saveframes[sf].hasItem("_Field_strength"))
						nmr_spectrometer_.field_strength = (valueIsValid(saveframes[sf].getDataItemValue("_Field_strength")) 
											?  saveframes[sf].getDataItemValue("_Field_strength").toFloat() : FLOAT_VALUE_NA);
				}
			}
		}
	}
	

	void NMRStarFile::findDependiencies_() 
	{
#ifdef NMRSTAR_DEBUG
	std::cout << "NMRStarFile::findDependiencies_()" << std::endl;	
#endif

		// set the MonomericPolymer
		// check all chemical units, to which monomer they belong 
		for (Size j=0; j < molecular_system_.chemical_units.size(); j++)
		{
			if ( monomeric_polymer_indices_.has(molecular_system_.chemical_units[j].label.trim("$")) )
			{
				molecular_system_.chemical_units[j].monomeric_polymer = &(monomeric_polymers_[monomeric_polymer_indices_[molecular_system_.chemical_units[j].label.trim("$")]]);
			}
		}
		// set the shifts
		for (Size j=0; j < 	molecular_system_.chemical_units.size(); j++)
		{
			for (Size i=0; i < atom_data_sets_.size(); i++)
			{
				if (atom_data_sets_[i].name == molecular_system_.chemical_units[j].component_name)
				{
					molecular_system_.chemical_units[j].shifts = &atom_data_sets_[i];
				}
			}
		}

#ifdef NMRSTAR_DEBUG
	std::cout << "    End of NMRStarFile::findDependiencies_()" << std::endl;	
#endif

	}

	void NMRStarFile::setSpecialCharacters_(String characters)
	{
		special_characters_ = characters;
	}

	bool NMRStarFile::valueIsValid(String value)
	{
		if (value.size()==1)
		{
			for (Size i=0; i < special_characters_.size(); i ++)
			{
				if (value == special_characters_[i])
					return false;
			}
		}
		return true;
	}

	bool NMRStarFile::operator == (const NMRStarFile& f)  
	{
		// TODO: reicht das??
		return File::operator == (f);
	}

	bool NMRStarFile::operator != (const NMRStarFile& f)  
	{
		// TODO: reicht das??
		return !(File::operator == (f));
	}

	void NMRStarFile::clear() // TODO
	{
		CIFFile::clear();
		number_of_shift_sets_ = 0;
		//entry_information_.clear();
		//molecular_system_.clear();
		atom_data_sets_.clear();
		sample_conditions_.clear();
		shift_references_.clear();
		//nmr_spectrometer_.clear();
		monomeric_polymers_.clear();
		dummy_saveframe_.clear();	
		//dummy_sample_condition_.clear();
		//dummy_shift_reference_set_.clear();
	}
	
	
} //namespace
