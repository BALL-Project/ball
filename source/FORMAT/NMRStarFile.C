// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/FORMAT/NMRStarFile.h>
#include <BALL/FORMAT/lineBasedFile.h>
#include <iostream>
#include <fstream>

#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/protein.h>
#include <BALL/STRUCTURE/peptides.h>
#include <BALL/NMR/shiftModule.h>
#include <BALL/SYSTEM/path.h>

//#define NMRSTAR_DEBUG 1
#undef NMRSTAR_DEBUG

//#define NMRSTAR_DEBUG_MAPPING 1
#undef NMRSTAR_DEBUG_MAPPING

using namespace std;

namespace BALL
{
	const float    NMRStarFile::FLOAT_VALUE_NA    = std::numeric_limits<float>::max();
	const int      NMRStarFile::INT_VALUE_NA      = std::numeric_limits<Position>::max();
	const Position NMRStarFile::POSITION_VALUE_NA = std::numeric_limits<Position>::max();

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

	bool NMRStarFile::NMRAtomData::operator == (const NMRStarFile::NMRAtomData& atom) const
	{
		bool ret_value = true;
		ret_value &= (atom_ID          == atom.atom_ID          );
		ret_value &= (residue_seq_code == atom.residue_seq_code );
		ret_value &= (residue_label    == atom.residue_label    );
		ret_value &= (atom_name        == atom.atom_name        );
		ret_value &= (atom_type        == atom.atom_type        );
		ret_value &= (shift_value      == atom.shift_value      );
		ret_value &= (error_value      == atom.error_value      );
		ret_value &= (ambiguity_code   == atom.ambiguity_code   );

		return ret_value;
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
			Log.warn() << "NMRStarFile::SampleCondition: Warning: Missing values in Sample Conditions." << std::endl;
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
		s << " components: " << endl;
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
		s << "isotope_number: "		  << isotope_number << endl;
		s << "atom_group: "				  << atom_group << endl;
		s << "shift_units: "				<< shift_units << endl;
		s << "shift_value: "				<< shift_value << endl;
		s << "reference_method: "   << reference_method << endl;
		s << "reference_type: "     << reference_type << endl;
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
		  reference_type(),
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
    s << "entry_type: "         << entry_type           << endl;
		s << "NMR_STAR_version: "	  << NMR_STAR_version     << endl;
		s << "experimental_method: "<< experimental_method << std::endl;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	void NMRStarFile::EntryInformation::clear()
	{
		entry_type.clear();
		BMRB_accession_code.clear();
		NMR_STAR_version.clear();
		experimental_method.clear();
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
    s << "label: "         << label << endl;
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


	ostream& NMRStarFile::MonomericPolymer::operator >> (ostream& s)
	{	
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<"  << endl;
		s << "Monomeric Polymer:" << label_name << endl;
 		s << "type: "             << type << endl;
		s << "polymer_class: "		<< polymer_class << endl;
		s << "common_name: "			<< common_name << endl;
		s << "name_variant: "     << name_variant << endl;
		s << "molecular_mass: "   << molecular_mass << endl;
		s << "details "           << details << endl;
		s << "number_of_residues: " << number_of_residues << endl;
		s << "residue_sequence " << residue_sequence << endl;
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

	NMRStarFile::MonomericPolymer& NMRStarFile::getMonomericPolymer(Position i)
	{	
		if (i >= monomeric_polymers_.size())
				throw(Exception::OutOfRange(__FILE__, __LINE__));

		return monomeric_polymers_[i];
	}

	const NMRStarFile::MonomericPolymer& NMRStarFile::getMonomericPolymer(Position i) const
	{	
		if (i >= monomeric_polymers_.size())
				throw(Exception::OutOfRange(__FILE__, __LINE__));

		return monomeric_polymers_[i];
	}

	NMRStarFile::MonomericPolymer& NMRStarFile::getMonomericPolymer(const String& name)
	{	
		for (Size i=0; i<monomeric_polymers_.size(); i++)
		{
			if (name == monomeric_polymers_[i].label_name)
				return monomeric_polymers_[i];
		}	

		throw(Exception::OutOfRange(__FILE__, __LINE__));
	}
	
	const NMRStarFile::MonomericPolymer& NMRStarFile::getMonomericPolymer(const String& name) const
	{	
		for (Size i=0; i<monomeric_polymers_.size(); i++)
		{
			if (name == monomeric_polymers_[i].label_name)
				return monomeric_polymers_[i];
		}	

		throw(Exception::OutOfRange(__FILE__, __LINE__));
	}

	bool NMRStarFile::hasMonomericPolymer(String name) const
	{	
		bool found = false;
		for (Size i=0; i<monomeric_polymers_.size(); i++)
		{
			if (name == monomeric_polymers_[i].label_name)
				found = true;
		}
		return found;
	}


	bool NMRStarFile::isMonomericPolymer(String chemical_unit_label) 
	{	
		bool found = false;
		for (Size i=0; i<monomeric_polymers_.size(); i++)
		{
			if (chemical_unit_label == monomeric_polymers_[i].label_name)
				found = true;
		}
		return found;
	}
 
	void NMRStarFile::addMonomericPolymer(NMRStarFile::MonomericPolymer mp) 
	{
		if (hasMonomericPolymer(mp.label_name))
		{
			Log.warn() << "NMRStarFile::addMonomericPolymer(): Warning: A monomer with name " << mp.label_name << " was overwritten!" << endl;  
		}	
		monomeric_polymers_.push_back(mp);
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
		s << "system_name:" << system_name << endl;
 		s << "abbreviation_common: "	  << abbreviation_common << endl;
		s << "system_physical_state: "	<< system_physical_state << endl;
		s << "system_oligomer_state: "	<< system_oligomer_state << endl;
		s << "system_paramagnetic: "		<< system_paramagnetic << endl;
		s << "system_thiol_state: "     << system_thiol_state << endl;
		s << "system_molecular_weight: "<< system_molecular_weight << endl;

		s << "chemical units:" << endl;
		for (Size i=0; i < chemical_units.size(); i++)
		{
			s << "  " << chemical_units[i].component_name << " : " << chemical_units[i].label << endl;
			*(chemical_units[i].monomeric_polymer) >> s;
		}
		s << "related database entries:" << endl;
		for (Size i=0; i < related_database_entries.size(); i++)
		{
			if (i!=0)
				s << "   -------------------------------------------" << endl;

			s << "   DB name: "				<< related_database_entries[i].name << endl;
			s << "   accesion code: "	<< related_database_entries[i].accession_code << endl;
			s << "   molecule name: "	<< related_database_entries[i].entry_mol_name << endl;
			s << "   relation type: "	<< related_database_entries[i].relation_type << endl;
			s << "   details : "			<< related_database_entries[i].details << endl;
		}
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}
	
	ostream& NMRStarFile::NMRSpectrometer::operator >> (std::ostream& s)
	{	
		s << endl<< "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<" << endl;
		s << "name:	"						<< name << endl;
		s << "manufacturer: "   << manufacturer << endl;
		s << "model: "          << model << endl;
		s << "field strength: " << field_strength << endl;
		s << ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> " << endl << endl;
		return s;
	}

	NMRStarFile::BALLToBMRBMapper::BALLToBMRBMapper()
		: name_converter_(),
			ball_to_bmrb_map_(),
			bmrb_to_ball_map_(),
			chain_(NULL),
			nmr_data_(NULL),
			num_mismatches_(0),
			num_gaps_(0),
			valid_(true)
	{
	}


	NMRStarFile::BALLToBMRBMapper::BALLToBMRBMapper(Chain const& chain,
		                                              const NMRStarFile& nmr_data,
	                                                const String& chemical_unit)
		: name_converter_(),
			ball_to_bmrb_map_(),
			bmrb_to_ball_map_(),
			chain_(&chain),
			nmr_data_(&nmr_data),
			nmr_atom_data_set_(NULL),
			nmr_atom_data_set_index_(0),
			num_mismatches_(0),
			num_gaps_(0),
			valid_(true)
	{
		valid_ = setNMRAtomDataSetByName(chemical_unit);
	}

	bool NMRStarFile::BALLToBMRBMapper::setNMRAtomDataSetByName(String const& chemical_unit_label)
	{
		bool found_chemical_unit = false;
		String chemical_unit_name;

		try {
			MolecularSystem::ChemicalUnit const& cu = nmr_data_->getChemicalUnitByLabel(chemical_unit_label);
			chemical_unit_name = cu.component_name;


			// first, try to identify a data set with the same *name*
			for (Size k=0; k < nmr_data_->atom_data_sets_.size(); k++)
			{
				if (nmr_data_->atom_data_sets_[k].name == chemical_unit_name)
				{
					found_chemical_unit = true;
					nmr_atom_data_set_  = &nmr_data_->atom_data_sets_[k];
					nmr_atom_data_set_index_ = k;
				}
			}
		} catch (...)
		{
			found_chemical_unit = false;
		}

		if (chemical_unit_name=="")
		{
			Log.warn() << "BALLToBMRBMapper: No chemical unit given!" << std::endl;
				valid_ = false;
		}
		else if (!found_chemical_unit)
		{
			Log.warn() << "BALLToBMRBMapper: " << chemical_unit_name << " not found!" << std::endl;
				valid_ = false;
		}
		return valid_;
	}

	bool NMRStarFile::BALLToBMRBMapper::isMapped(Atom const* atom) const
	{
		return (ball_to_bmrb_map_.find(atom) != ball_to_bmrb_map_.end());
	}


	bool NMRStarFile::BALLToBMRBMapper::isMapped(const NMRAtomData& nmr_atom) const
	{
		return (bmrb_to_ball_map_.find(&nmr_atom) != bmrb_to_ball_map_.end());
	}

	const Atom* NMRStarFile::BALLToBMRBMapper::getBALLAtom(const NMRAtomData& nmr_atom) const
	{
		const Atom* atom = NULL;
		if (isMapped(nmr_atom))
		{
			atom = bmrb_to_ball_map_.find(&nmr_atom)->second;
		}
		return atom;
	}

	NMRStarFile::BALLToBMRBMapper::BMRBIndex NMRStarFile::BALLToBMRBMapper::operator() (const Atom* atom)
	{
		NMRStarFile::BALLToBMRBMapper::BMRBIndex mapping(0,0);
		if (isMapped(atom))
		{
			mapping = ball_to_bmrb_map_.find(atom)->second;
		}
		return mapping;
	}

	Atom const* NMRStarFile::BALLToBMRBMapper::findNMRAtom_(const NMRAtomData& atom) const
	{
		// get the atom corresponding to the given NMRFile atom
		if (atom.residue_seq_code <= chain_->countResidues())
		{
			// first find the residue
			Residue const* residue = chain_->getResidue(atom.residue_seq_code-1);
			
			if (!residue)
			{
				Log.warn() << "NMRStarFile::findNMRAtom_(): Warning: Residue no " << atom.residue_seq_code << " not found." << std::endl;
				return NULL;
			}	

			// TODO: decide whether to map in such a case anyway using a class option
			if (residue->getName() != atom.residue_label)
			{
				Log.warn() << "NMRStarFile::findNMRAtom_(): Warning: Residues have different types in NMRStarFile and BALL sequence!" << std::endl;
				Log.warn() << "             NMRStarFile: " << atom.residue_label << std::endl;
				Log.warn() << "             BALL:        " << residue->getName() << std::endl;
			}

			// then find the atom
			for (AtomConstIterator at_it = residue->beginAtom(); +at_it; ++at_it)
			{
				// does the name converter supports NMRSTAR and PDB?
				//	if (at_it->getName() == name_converter_.convertName(atom.residue_label, atom.atom_name, "NMRSTAR", "PDB"))
				if (name_converter_.matches(atom.residue_label, atom.atom_name, "NMRSTAR", at_it->getName(), "PDB"))
				{
					return &*at_it;
				}
			}
		}	
		else
		{		
			Log.warn() << "NMRStarFile::findNMRAtom_(): Warning: Given atom belongs to residue " << atom.residue_seq_code 
								 << " but the corresponding chain has only " << chain_->countResidues()  << " residues." << std::endl;
		}
		return NULL;
	}


	// TODO: this currently works only with one NMRAtomDataSet
	bool  NMRStarFile::BALLToBMRBMapper::createTrivialMapping()
	{
		if (!valid_)
		{
			return false;
		}

		num_mismatches_  = -1;
		num_gaps_        = -1;

		// walk over the NMRAtomData sets of the nmr data file
		// NOTE: currently, we walk only over the first of those data sets
		const vector<NMRAtomDataSet>& nmr_data_sets = nmr_data_->getNMRData();

		if (nmr_data_sets.empty())
		{
			Log.error() << "NMRStarFile::BALLToBMRBMapper::createTrivialMapping(): no atom data sets found!" << std::endl;

			return false;
		}

		if (nmr_data_sets.size() > 1)
		{
			Log.error() << "NMRStarFile::BALLToBMRBMapper::createTrivialMapping(): can't compute mapping for more than one data set!" << std::endl;

			return false;
		}

		num_mismatches_  = 0;
		num_gaps_        = 0;

		const NMRAtomDataSet* nmr_atom_data_set = nmr_atom_data_set_;
		if (!nmr_atom_data_set_)
		{
			Log.warn() << "NMRStarFile::BALLToBMRBMapper::createTrivialMapping(): no chemical unit specified, try the first!" << std::endl;

			// if nothing was specified, try the first atom data set	
			nmr_atom_data_set = &nmr_data_sets[0];
		}

		// TODO: insert the data from the NMRAtomDataSet (like conditions) as properties!
		for (Position current_nmr_atom = 0;
				current_nmr_atom < nmr_atom_data_set->atom_data.size();
				++current_nmr_atom)
		{
			const Atom* ball_atom = findNMRAtom_(nmr_atom_data_set->atom_data[current_nmr_atom]);

			BMRBIndex b_index(0, current_nmr_atom);

			if (ball_atom)
			{
				ball_to_bmrb_map_[ball_atom] = b_index;
				bmrb_to_ball_map_[&(nmr_atom_data_set->atom_data[current_nmr_atom])] = ball_atom;
			}
		}
		return true;
	}


	bool NMRStarFile::BALLToBMRBMapper::createMapping(const String& aligned_ball_sequence,
																										const String& aligned_nmrstar_sequence)
	{
		if (!valid_)
		{
			Log.error() << "BALLToBMRBMapper::createMapping(): Warning: invalid state!" << std::endl;
			return false;
		}

		int  matches     =  0;
		num_mismatches_  = -1;
		num_gaps_        = -1;

		String chain_seq = Peptides::GetSequence(*chain_);

		String squeezed_align_seq(aligned_ball_sequence);
		while(squeezed_align_seq.substitute("-", "")!=String::EndPos)
		{
			;
		}

		if (chain_seq != squeezed_align_seq)
		{
			Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: Alignment sequence cannnot be matched to chosen chain!" << endl;
			Log.warn() << chain_seq << std::endl;
			Log.warn() << squeezed_align_seq << std::endl;
//			TODO: what should we do here?
//			return false;
		}

		// check whether the alignment is valid
		if ((aligned_ball_sequence.size()==0) || (aligned_nmrstar_sequence.size()==0) )
		{
			Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: One of the aligned strings has length zero!" << endl;
			return false;
		}

		if (aligned_ball_sequence.size() != aligned_nmrstar_sequence.size())
		{
			Log.error() << "BALLToBMRBMapper::createMapping(): Warning: Incorrect alignment. Aligned sequences have different length!" << std::endl;
			return false;
		}
		// get the NMR data
		// NOTE: we take the zero's dataset!
		if (nmr_data_ && nmr_atom_data_set_ && nmr_data_->atom_data_sets_.empty())
		{
			Log.error() << "BALLToBMRBMapper::createMapping(): Warning: no atom data present!" << std::endl;
			return false;
		}

		num_mismatches_  = 0;
		num_gaps_        = 0;

		std::vector<NMRAtomData> const& nmr_data = nmr_atom_data_set_->atom_data;

		// get the actual length of the nmr sequence
		// NOTE: unfortunately this information is not provided by getResidueSequence().size !!!
		Size len_sequence = 0;
		for (Position i=0; i<aligned_nmrstar_sequence.size(); ++i)
		{
			if (aligned_nmrstar_sequence[i] != '-')
			{
				++len_sequence;
			}
		}
		std::vector<std::list<Position> > atoms_per_nmr_residue(len_sequence);
		for (Position i=0; i<nmr_data.size(); i++)
		{
			if (nmr_data[i].residue_seq_code-1 < len_sequence)
			{
				atoms_per_nmr_residue[nmr_data[i].residue_seq_code-1].push_back(i);
			}
			else
			{
				Log.error() << "BALLToBMRBMapper::createMapping(): Warning: corrupt index!" << std::endl;
			}
		}

		ResidueConstIterator res_it = chain_->beginResidue();
		Position num_nmr_alig_gaps  = 0;

		// now create BALLToBMRBMappers for the given alignment
		for (Position i=0; (i < aligned_nmrstar_sequence.size()) && (res_it != chain_->endResidue()); i++)
		{
			if (   (aligned_ball_sequence[i]    == '-')
					&& (aligned_nmrstar_sequence[i] == '-'))
			{
				Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: Gaps in both aligned sequences!" << endl;
				num_gaps_++;
			}
			else
			{
				if (aligned_ball_sequence[i] == '-')
				{
					// this NMR residue has no assigned BALL residue 
					// nothing to do!

#if defined NMRSTAR_DEBUG || defined NMRSTAR_DEBUG_MAPPING
					Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: NMR residue no" << i << " "
						         << aligned_nmrstar_sequence[i] << " has no assigned structure residue!" << endl;
#endif
					num_gaps_++;
				}
				else if (aligned_nmrstar_sequence[i] == '-')
				{
					// this BALL residue has no assigned NMR residue
					// the current residue cannot be assigned any shifts!
					res_it++;
					num_nmr_alig_gaps++;
					num_gaps_++;

#if defined NMRSTAR_DEBUG || defined NMRSTAR_DEBUG_MAPPING					
					Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: Structure residue no " << i << " "
						<< aligned_ball_sequence[i] << " has no assigned NMR residue!" << endl;
#endif
				}
				else //if (aligned_ball_sequence[i] == aligned_nmrstar_sequence[i])
				{
					// there is a correspondence found between nmr and ball 
					if (Peptides::OneLetterCode(res_it->getName()) != aligned_nmrstar_sequence[i])
					{
						Log.warn() << "NMRStarFile::BALLToBMRBMapper::createMapping(): Warning: Incorrect mapping "
											 << Peptides::OneLetterCode(res_it->getName()) << " to " << aligned_nmrstar_sequence[i]  << "." << std::endl;

						num_mismatches_++;
					}
					else if (aligned_ball_sequence[i] != aligned_nmrstar_sequence[i])
					{
						num_mismatches_++;
#if defined NMRSTAR_DEBUG || defined NMRSTAR_DEBUG_MAPPING
						Log.warn() << "BALLToBMRBMapper::createMapping(): Warning: Assigned structure residue "
							<< aligned_ball_sequence[i] << " to NMR residue " << aligned_nmrstar_sequence[i] << "." << endl;
#endif
					}
					else // we got a match
					{
						matches++;

						//try to match the atoms
						std::list<Position>& nmr_atoms = atoms_per_nmr_residue[i-num_nmr_alig_gaps];

						for (std::list<Position>::iterator nmr_atom = nmr_atoms.begin(); nmr_atom != nmr_atoms.end(); ++nmr_atom)
						{
							bool mapped_me=false;
							for (AtomConstIterator at_it = res_it->beginAtom(); +at_it && !mapped_me; ++at_it)
							{
								String full_name = at_it->getName();

								if (name_converter_.matches(nmr_data[*nmr_atom].residue_label, nmr_data[*nmr_atom].atom_name,
								                           "NMRSTAR", full_name, "PDB"))
								{
									// and store in the mappings
									BALLToBMRBMapper::BMRBIndex b_index(nmr_atom_data_set_index_, *nmr_atom);
									ball_to_bmrb_map_[&*at_it] = b_index;
									bmrb_to_ball_map_[&(nmr_data[*nmr_atom])] = &*at_it;
									mapped_me = true;
								}
							}
						}
					}
					// get the next residue 
					res_it++;

				} // end of else no gaps
			}// end of else not '-''-'
		} // end of for all alignment positions

#if defined NMRSTAR_DEBUG || defined NMRSTAR_DEBUG_MAPPING
	Log.info() << " Mapping matches:  " << matches << endl;
#endif
		return true;
	}

	void NMRStarFile::BALLToBMRBMapper::clear()
	{
		ball_to_bmrb_map_.clear();
		bmrb_to_ball_map_.clear();
		chain_    = NULL;
		nmr_data_ = NULL;
		nmr_atom_data_set_ = NULL;
		nmr_atom_data_set_index_ = 0;
		num_mismatches_    = -1;
		num_gaps_ = -1;
		valid_    = false;
	}


////////////////////////////////////////////////////////////////

	NMRStarFile::NMRStarFile()
		: CIFFile(),
			valid_(false),
			number_of_shift_sets_(0),
			number_of_assigned_shifts_(0),
			entry_information_(),
			molecular_system_(),
			atom_data_sets_(),
			sample_conditions_(),
			samples_(),
			shift_references_(),
			nmr_spectrometers_(),
			monomeric_polymers_(),
			has_H_shifts_(false),
			has_C_shifts_(false),
			has_N_shifts_(false),
			dummy_saveframe_(),
			dummy_sample_condition_(),
			dummy_sample_(),
			dummy_shift_reference_set_(),
			dummy_NMR_spectrometer_(),
			dummy_monomeric_polymer_(),
			special_characters_(".?@")
	{
	}

	NMRStarFile::NMRStarFile(const String& file_name, File::OpenMode open_mode)
		: CIFFile(file_name, open_mode),
			valid_(false),
			number_of_shift_sets_(0),
			number_of_assigned_shifts_(0),
			entry_information_(),
			molecular_system_(),
			atom_data_sets_(),
			sample_conditions_(),
			samples_(),
			shift_references_(),
			nmr_spectrometers_(),
			monomeric_polymers_(),
			has_H_shifts_(false),
			has_C_shifts_(false),
			has_N_shifts_(false),
			dummy_saveframe_(),
			dummy_sample_condition_(),
			dummy_sample_(),
			dummy_shift_reference_set_(),
			dummy_NMR_spectrometer_(),
			dummy_monomeric_polymer_(),
			special_characters_(".?@")
	{
		read();
	}

	NMRStarFile::~NMRStarFile()
	{
	}

	bool NMRStarFile::read()
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

			valid_ = true;

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
		bool result = true;
		// try to assign to the AtomContainer's atoms 

		Chain* chain;

		// first check, if the given AtomContainer is a valid chain
        if (RTTI::isKindOf<Chain>(&ac))
		{
			chain = RTTI::castTo<Chain>(ac);
		}

        if (RTTI::isKindOf<System>(&ac))
		{
			System* system =  RTTI::castTo<System>(ac);
			if (system->countChains() > 0)
			{
				ChainIterator cit = system->beginChain();

				// NOTE: we take the first chain of the system
				chain = RTTI::castTo<Chain>(*cit);
			}
			else
			{
				Log.error() << "NMRStarfile::read(): No chain found in the given AtomContainer." << endl;
				return false;
			}
		}
		else
		{
			Log.error() << "NMRStarfile::read(): Cannot assign shifts to a non-chain." << endl;
			return false;
		}

		String chemical_unit = "";
		// get always the first chemical_unit
		if (!atom_data_sets_.empty())
		{
			Log.warn() << "NMRStarfile::read(): Warning: chemical unit ambiguous!" << endl;
			// return false;
		}
		chemical_unit = atom_data_sets_[0].name;

		// Since no explicit mapping AtomContainer-->BMRB
		// is given, we try to find a trivial mapping 
		// by exactly matching residues and atom names
		BALLToBMRBMapper pdb_to_bmrb_mapping(*chain, *this, chemical_unit);
		pdb_to_bmrb_mapping.createTrivialMapping();

		// now assign the shifts via the mapping
		result = assignShifts_(pdb_to_bmrb_mapping);
		return result;
	} 
	
	bool NMRStarFile::assignShifts(AtomContainer& ac, 
	                        const String& chemical_unit,
	                        const String& aligned_ball_sequence,
	                        const String& aligned_nmrstar_sequence)
	{
		// read the NMRStarFile's information
		bool result = valid_;
		
		Chain* chain = 0;	
		if (result)
		{	
			// check, if the AtomContainer is a chain
			// first check, if the given AtomContainer is a valid chain
            if (RTTI::isKindOf<Chain>(&ac))
			{
				chain = RTTI::castTo<Chain>(ac);
			}
            else if (RTTI::isKindOf<System>(&ac))
			{
				System* system =  RTTI::castTo<System>(ac);
				if (system->countChains() > 0)
				{	 
					ChainIterator cit = system->beginChain();

					// NOTE: we take the first chain of the system
					chain = RTTI::castTo<Chain>(*cit);
				}
				else
				{
					Log.error() << "NMRStarfile::assignShifts(): No chain found in the given AtomContainer." << endl;
					result = false;
				}
			} 
			else
			{
				Log.error() << "NMRStarfile::assignShifts(): Cannot assign shifts to a non-chain." << endl;
				result = false;
			}
		}		
		// ... if valid try to assign to the AtomContainer's atoms 
		if (result && chain)
		{
			// create a mapping
			BALLToBMRBMapper pdb_to_bmrb_mapping(*chain, *this, chemical_unit);
			pdb_to_bmrb_mapping.createMapping(aligned_ball_sequence, aligned_nmrstar_sequence);

#if defined NMRSTAR_DEBUG_MAPPING
Log.info()  << "NMRStarfile::assignShifts(): number of mismatched residues: " 
						<< pdb_to_bmrb_mapping.getNumberOfMismatches()<< endl;
#endif
			// now assign the shifts via the mapping
			result = assignShifts_(pdb_to_bmrb_mapping);
		}
		return result;
	}

	bool NMRStarFile::assignShifts(BALLToBMRBMapper& ball_to_bmrb_mapping)
	{	
		// do we have a valid NMRStarFile's instance?
		bool result = valid_;

		// .. and assign the shifts via the given mapping
		if (result)
		{
			result = assignShifts_(ball_to_bmrb_mapping);
		}
		return result;
	}


	//  Apply the shifts read into to the AtomContainer as denoted in the mapping.
	//  We assume, that the file was already read!
	bool NMRStarFile::assignShifts_(BALLToBMRBMapper& ball_to_bmrb_mapping)
	{
		number_of_assigned_shifts_ = 0;
		ResidueIterator r_it;
		if (ball_to_bmrb_mapping.getChain())
			r_it = const_cast<Chain*>(ball_to_bmrb_mapping.getChain())->beginResidue();
		else
		{
			Log.warn() << "NMRStarFile: could not find chain" << std::endl;
			return false;
		}

		// map the shifts via the pdb_bmrb_mapping into the given AtomContainer
		for ( ; +r_it; r_it++)
		{
			for (AtomIterator a_it = r_it->beginAtom(); +a_it; a_it++)
			{
				if (ball_to_bmrb_mapping.isMapped(&*a_it))
				{
					BALLToBMRBMapper::BMRBIndex bindex = ball_to_bmrb_mapping(&*a_it);
					NMRAtomData& nmr_atom = atom_data_sets_[bindex.first].atom_data[bindex.second];

					a_it->setProperty(ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT,	nmr_atom.shift_value);
					number_of_assigned_shifts_++;
				}
			}
		}
		return true;
	}


	Size NMRStarFile::getNumberOfAtoms() const
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

	String NMRStarFile::getResidueSequence(Position i) const
	{	
		if (monomeric_polymers_.size() > i)
		{			
			if (atom_data_sets_.size() > i)
			{	
				bool identical_sequences = true;
				std::vector<NMRAtomData> const& atom_data = atom_data_sets_[i].atom_data;
				String residue_sequence = monomeric_polymers_[i].residue_sequence;
				bool is_dna = monomeric_polymers_[i].polymer_class == "DNA";	
				bool is_rna = monomeric_polymers_[i].polymer_class == "RNA";	

				for (Position j=0; identical_sequences && (j<atom_data.size()); j++)
				{
					if (atom_data[j].residue_seq_code == POSITION_VALUE_NA)
					{
						identical_sequences = false;
						Log.warn() << "NMRStarFile::getResidueSequence(): Warning: invalid residue sequence code" << std::endl;
					}

					if (atom_data[j].residue_seq_code - 1 >= residue_sequence.size())
					{
						identical_sequences = false;
						Log.warn() << "NMRStarFile::getResidueSequence(): Warning: residue sequence code " << atom_data[j].residue_seq_code 
						           << " is larger than the sequence length (" << residue_sequence.size() << ")" << std::endl;
					}

					// now check if the residue information is consistent
					char current_letter = residue_sequence[atom_data[j].residue_seq_code - 1];

					if (current_letter == 'X') // this case is handled like a wildcard character
						continue;

					// currently, we treat polymers of type DNA or RNA differently. all else is considered a protein
					if (is_dna || is_rna)
					{
						if (current_letter != atom_data[j].residue_label)
						{
							identical_sequences = false;
							Log.warn() << "NMRStarFile::getResidueSequence(): Warning: Inconsistent residue sequence information." 
								         << residue_sequence[atom_data[j].residue_seq_code - 1] 
											   << " != "  << Peptides::OneLetterCode(atom_data[j].residue_label) << endl;
						}
					}
					else
					{
						if (current_letter != Peptides::OneLetterCode(atom_data[j].residue_label))
						{
							identical_sequences = false;
							Log.warn() << "NMRStarFile::getResidueSequence(): Warning: Inconsistent residue sequence information." 
								         << residue_sequence[atom_data[j].residue_seq_code - 1] 
											   << " != "  << Peptides::OneLetterCode(atom_data[j].residue_label) << endl;
						}
					}
				}
		
				if (identical_sequences)
				{
					return monomeric_polymers_[i].residue_sequence;
				}
			}
		}
		return String("");
		
	}


	const std::vector<NMRStarFile::NMRAtomDataSet>& NMRStarFile::getNMRData() const
	{
		return atom_data_sets_;
	}
		
	const NMRStarFile::MolecularSystem::ChemicalUnit& NMRStarFile::getChemicalUnitByLabel(String const& label) const
	{
		NMRStarFile::MolecularSystem const& mol_sys = getMolecularInformation();

		for (Position i=0; i<mol_sys.chemical_units.size(); ++i)
		{
			if (mol_sys.chemical_units[i].label == label)
			{
				return mol_sys.chemical_units[i];
			}
		}

		throw (Exception::OutOfRange(__FILE__, __LINE__));
	}

	NMRStarFile::MolecularSystem::ChemicalUnit& NMRStarFile::getChemicalUnitByLabel(String const& label)
	{
		NMRStarFile::MolecularSystem& mol_sys = getMolecularInformation();

		for (Position i=0; i<mol_sys.chemical_units.size(); ++i)
		{
			if (mol_sys.chemical_units[i].label == label)
				return mol_sys.chemical_units[i];
		}

		throw (Exception::OutOfRange(__FILE__, __LINE__));
	}

	bool NMRStarFile::hasSampleCondition(String name)
	{
		for (Size i=0; i < sample_conditions_.size(); i++)
		{
			if (sample_conditions_[i].name == name)
			{
				return true; 
			}
		}
		return false;
	}

	bool NMRStarFile::hasSampleCondition(String name) const
	{
		for (Size i=0; i < sample_conditions_.size(); i++)
		{
			if (sample_conditions_[i].name == name)
			{
				return true; 
			}
		}
		return false;
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
		Log.warn() << "NMRStarFile::getSampleConditionByName(): Warning: Returned a dummy sample condition!" << std::endl;
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
		Log.warn() << "NMRStarFile::getSampleConditionByName(): Warning: Returned a dummy sample condition!" << std::endl;
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

	NMRStarFile::Sample NMRStarFile::getSample(Position i) const 
	{
		if (i < samples_.size())
			return samples_[i];
		else
		{	
			Log.warn() << "NMRStarFile::getSample(): Warning: Returned a dummy sample!" << std::endl;
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
		
		Log.warn() << "NMRStarFile::getSample(): Warning: Returned a dummy sample!" << std::endl;
		return  dummy_sample_;
	}

  
	bool  NMRStarFile::hasShiftReferenceSet(String name)
	{
		for (Size i=0; i < shift_references_.size(); i++)
		{
			if (shift_references_[i].name == name)
			{
				return true; 
			}
		}
		return false;
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
		Log.warn() << "NMRStarFile::getShiftReferenceSetByName(): Warning: Returned a dummy shift reference set!" << std::endl;

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
		Log.warn() << "NMRStarFile::getShiftReferenceSetByName(): Warning: Returned a dummy shift reference set!" << std::endl;
		return dummy_shift_reference_set_;
	}

	NMRStarFile::NMRSpectrometer& NMRStarFile::getNMRSpectrometer(Position i) 
	{
		if (i < nmr_spectrometers_.size())
		{
			return nmr_spectrometers_[i];
		}
		else
		{	
			Log.warn() << "NMRStarFile::getNMRSpectrometer(): Warning: Returned a dummy spectrometer!" << std::endl;
			return dummy_NMR_spectrometer_;
		}
	}
	
	const NMRStarFile::NMRSpectrometer& NMRStarFile::getNMRSpectrometer(Position i) const 
	{	
		if (i < nmr_spectrometers_.size())
		{
			return nmr_spectrometers_[i];
		}
		else
		{
			Log.warn() << "NMRStarFile::getNMRSpectrometer(): Warning: Returned a dummy spectrometer!" << std::endl;
			return dummy_NMR_spectrometer_;
		}
	}

	NMRStarFile::NMRSpectrometer& NMRStarFile::getNMRSpectrometerByName(String name)
	{
		for (Size i=0; i < nmr_spectrometers_.size(); i++)
		{
			if (nmr_spectrometers_[i].name == name)
			{
				return nmr_spectrometers_[i]; 
			}
		}
		Log.warn() << "NMRStarFile::gettNMRSpectrometerByName(): Warning: Returned a dummy spectrometer!" << std::endl;
		return dummy_NMR_spectrometer_;
	}
	
	const NMRStarFile::NMRSpectrometer& NMRStarFile::getNMRSpectrometerByName(String name) const
	{
		for (Size i=0; i < nmr_spectrometers_.size(); i++)
		{
			if (nmr_spectrometers_[i].name == name)
			{
				return nmr_spectrometers_[i]; 
			}
		}
		Log.warn() << "NMRStarFile::gettNMRSpectrometerByName(): Warning: Returned a dummy spectrometer!" << std::endl;
		return dummy_NMR_spectrometer_;
	}
	
	String NMRStarFile::getNMRSpectrometerManufacturer(Position i) const 
	{	
		if (i < nmr_spectrometers_.size())
		{
			return nmr_spectrometers_[i].manufacturer;
		}
		else 
			return "";
	}
			
	float NMRStarFile::getNMRSpectrometerFieldStrength(Position i) const 
	{
		if (i < nmr_spectrometers_.size())
		{
			return nmr_spectrometers_[i].field_strength;
		}
		else
			return FLOAT_VALUE_NA;
	}


	void NMRStarFile::readEntryInformation_()
	{
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if (datablocks_[db].hasSaveframeCategory("entry_information"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("entry_information");
//				if (saveframes.size() > 1)
//					Log.warn() << "NMRStarFile::readEntryInformation_(): Warning: NMRFile has more than one entry information saveframe! " << std::endl; 
				for (Size sf=0; sf<saveframes.size(); sf++)
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

					if (saveframes[sf].hasItem("_Submission_date"))
						entry_information_.submission_date = saveframes[sf].getItemValue("_Submission_date");
					else entry_information_.submission_date = "";	
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
			if (datablocks_[db].hasSaveframeCategory("molecular_system"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("molecular_system");
//				if (saveframes.size() > 1)
//					Log.warn() << "NMRStarFile::readMolSystem_(): Warning: NMRFile has more than one molecular system saveframe! " << std::endl; 
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
						molecular_system_.system_molecular_weight = valueToFloat_(saveframes[sf].getItemValue("_System_molecular_weight"));
					else
						molecular_system_.system_molecular_weight = FLOAT_VALUE_NA;

					//
					// read the loop entries
					//
					for (Size loop = 0; loop < saveframes[sf].items.size(); loop++)
					{
						if (saveframes[sf].items[loop].is_loop)
						{	
							Item* current_loop = &saveframes[sf].items[loop];

							// read the chemical units!
							// first: Molecular system component name, 
							// second: label

							// we check the first key :-)
							if (saveframes[sf].items[loop].keys[0] == "_Mol_system_component_name")
							{
								// we have found the component name table :-)
								// store the data
								MolecularSystem::ChemicalUnit cu;
								for (Size line = 0; line < current_loop->values.size(); line++)
								{
									cu.component_name  = current_loop->values[line][0];
									cu.label = current_loop->values[line][1];
									molecular_system_.chemical_units.push_back(cu);
								}
							}

							// read the related DB entries
							if (saveframes[sf].items[loop].keys[0] == "_Database_name")
							{
								NMRStarFile::MolecularSystem::RelatedDB db;
								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									db.name = current_loop->values[line][0];
									Index pos = current_loop->getKeyIndex("_Database_accession_code");
									if ( pos > -1) db.accession_code = current_loop->values[line][pos];
									
									pos = current_loop->getKeyIndex("_Database_entry_mol_name");
									if ( pos > -1) db.entry_mol_name = current_loop->values[line][pos].trim("; \n");
									
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
			if (datablocks_[db].hasSaveframeCategory("monomeric_polymer"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("monomeric_polymer");
//				if (saveframes.size() > 1)
//					Log.warn() << "NMRStarFile::readMonomericPolymers_(): Warning: NMRFile has more than one monomeric polymer saveframe!" << std::endl; 
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
						mp.molecular_mass = valueToFloat_(saveframes[sf].getItemValue("_Molecular_mass"));
					else  
						mp.molecular_mass = 0.f;
					
					if (saveframes[sf].hasItem("_Details"))
						mp.details = saveframes[sf].getItemValue("_Details").trim(";\n");
					else   mp.details = "";

					if (saveframes[sf].hasItem("_Residue_count"))
						mp.number_of_residues = valueToInt_(saveframes[sf].getItemValue("_Residue_count"));
					else  
						mp.number_of_residues  = 0;

					if (saveframes[sf].hasItem("_Mol_residue_sequence"))
					{
						mp.residue_sequence = saveframes[sf].getItemValue("_Mol_residue_sequence").trim("; \n");
						while ( mp.residue_sequence.hasSubstring("\n"))
						{
							mp.residue_sequence.substitute("\n", "");
						}
						// some nmr files seem to have carriage return
						while ( mp.residue_sequence.hasSubstring("\r"))
						{
							mp.residue_sequence.substitute("\r", "");
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
									hdb.seq_to_submitted_percentage  = ((pos>-1 ) ? valueToFloat_(current_loop->values[line][pos]) 
																											          : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_subject_length");
									hdb.subject_length  = ((pos>-1 ) ? valueToFloat_(current_loop->values[line][pos])
																				           : FLOAT_VALUE_NA);
			
									pos = current_loop->getKeyIndex("_Sequence_identity");
									hdb.seq_identity  = ((pos>-1 ) ? valueToFloat_(current_loop->values[line][pos])
																				         : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_positive");
									hdb.seq_positive  = ((pos>-1 ) ? valueToFloat_(current_loop->values[line][pos])
																				         : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Sequence_homology_expectation_value");
									hdb.homology_expectation_value  = ((pos>-1 ) ? valueToFloat_(current_loop->values[line][pos])
																															 : FLOAT_VALUE_NA);
									mp.homolog_database_entries.push_back(hdb);
								}
							}
						}
					}// end of for all items
					// store this Monomer
					addMonomericPolymer(mp);
					
				/*	// check, to which chemical unit this monomer belongs TODO!
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
							if (saveframes[sf].items[loop].keys[0] == "_Variable_type")
							{
								SampleCondition tmp;
								tmp.name =  saveframes[sf].framename;
								// we have found the sample condition values :-)
								// store the data
								Item* current_loop = &saveframes[sf].items[loop];

								for (Size line = 0; line < current_loop->values.size(); line++ )
								{
									// according to the NMRStarFile 2.1 documentation, 
									// naming the first entry "_Variable_type" is mandatory
									// this is why we are allowed to map per type :-)
									tmp.types.push_back(current_loop->values[line][0]);
									String insert_pos=current_loop->values[line][0];

									Index pos = current_loop->getKeyIndex("_Variable_value");
									if ( pos > -1) 
									{
										tmp.values[insert_pos] = valueToFloat_(current_loop->values[line][pos]);
									}
									pos = current_loop->getKeyIndex("_Variable_value_error");
									if ( pos > -1) 
									{
										tmp.errors[insert_pos] = valueToFloat_(current_loop->values[line][pos]);
									}
									pos = current_loop->getKeyIndex("_Variable_value_units");
									if ( pos > -1) 
									{
										tmp.units[insert_pos] = current_loop->values[line][pos];
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
//				if (saveframes.size() > 1)
//					Log.warn() << "NMRStarFile::readShiftReferences_(): Warning: NMRFile has more than one chemical_shift_reference saveframe! " << std::endl; 

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
						
								for (Size line = 0; line < current_loop->values.size(); line++)
								{	
									NMRStarFile::ShiftReferenceElement ref_element;
									Index pos = current_loop->getKeyIndex("_Mol_common_name");
									if (pos > -1)
									{
										ref_element.mol_common_name =  current_loop->values[line][pos];
									}
									
									pos = current_loop->getKeyIndex("_Atom_type");
									if (pos > -1)
									{
										ref_element.atom_type  =  current_loop->values[line][pos];//.toChar();
									}
									
									pos = current_loop->getKeyIndex("_Atom_isotope_number");
									if (pos > 1)
									{
											ref_element.isotope_number = (Position)valueToInt_(current_loop->values[line][pos]);
									}
									
									pos = current_loop->getKeyIndex("_Atom_group");
									if (pos > 1)
									{
										ref_element.atom_group   =  current_loop->values[line][pos];
									}

									pos = current_loop->getKeyIndex("_Chem_shift_units");
									if (pos > -1)
									{
										ref_element.shift_units  =  current_loop->values[line][pos];
									}
									
									pos = current_loop->getKeyIndex("_Chem_shift_value");
									if (pos > -1)
									{
											ref_element.shift_value  = valueToFloat_(current_loop->values[line][pos]);
									}
									
									pos = current_loop->getKeyIndex("_Reference_method");
									if (pos > -1)
									{
										ref_element.reference_method  = current_loop->values[line][pos];//.toChar();
									}

									pos = current_loop->getKeyIndex("_Reference_type");
									if (pos > -1)
									{
										ref_element.reference_type  = current_loop->values[line][pos];//.toChar();
									}
									
									pos = current_loop->getKeyIndex("_Indirect_shift_ratio");
									if (pos > -1)
									{
										ref_element.indirect_shift_ratio = valueToFloat_(current_loop->values[line][pos]);
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
		// in most cases we just have one datablock ...
		for (Size db=0; db < datablocks_.size(); db++)
		{
			// find the category
			if  (datablocks_[db].hasSaveframeCategory("assigned_chemical_shifts"))
			{
				vector<CIFFile::SaveFrame> saveframes = datablocks_[db].getSaveframesByCategory("assigned_chemical_shifts");
//				if (saveframes.size() > 1)
//					Log.warn() << "NMRStarfile::readShifts(): Warning: File has more than one assigned_chemical_shifts saveframe! " << std::endl; 
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
									atom_data.atom_ID = ((pos > -1) ? (Position)valueToInt_(current_loop->values[line][pos]) 
																			            : POSITION_VALUE_NA);
									
									pos = current_loop->getKeyIndex("_Residue_seq_code");
									atom_data.residue_seq_code = ((pos > -1) ? (Position)valueToInt_(current_loop->values[line][pos])
																								           : POSITION_VALUE_NA);
									
									pos = current_loop->getKeyIndex("_Residue_label");
									if (pos > -1) atom_data.residue_label = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Atom_name");
									if (pos > -1) atom_data.atom_name = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Atom_type");
									if (pos > -1) 
									{
										atom_data.atom_type = current_loop->values[line][pos].toChar();
										if (!has_H_shifts_ && (String(atom_data.atom_type) == "H"))
										{
											has_H_shifts_ = true;
										}
										if (!has_C_shifts_ && (String(atom_data.atom_type) == "C"))
										{	
											has_C_shifts_ = true;
										}
										if (!has_N_shifts_ && (String(atom_data.atom_type) == "N"))
										{		
											has_N_shifts_ = true;	
										}
									}

									pos = current_loop->getKeyIndex("_Chem_shift_value");
									atom_data.shift_value = ( (pos > -1) ? valueToFloat_(current_loop->values[line][pos]) 
										 												           : FLOAT_VALUE_NA);
								
									pos = current_loop->getKeyIndex("_Chem_shift_value_error");
									atom_data.error_value = ( (pos > -1) ? valueToFloat_(current_loop->values[line][pos])
									                                     : FLOAT_VALUE_NA);

									pos = current_loop->getKeyIndex("_Chem_shift_ambiguity_code");
									atom_data.ambiguity_code = ( (pos > -1) ? valueToInt_(current_loop->values[line][pos])
																							            : INT_VALUE_NA);
									// store in the NMRDataSet
									atom_data_set.atom_data.push_back(atom_data);
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
						
								// look for the molecular systems component name
								if (saveframes[sf].hasItem("_Mol_system_component_name"))
								{		
									//store the name
									atom_data_set.name = (saveframes[sf].getItemValue("_Mol_system_component_name")).trim("$");
									atom_data_set.label = saveframes[sf].framename;
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
												atom_data_set.samples.push_back(current_loop->values[line][0].trim("$"));
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
				for (Size sf = 0; sf < saveframes.size(); sf++)
				{
					NMRStarFile::Sample sample;
					sample.label = saveframes[sf].framename.trim("$");
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
									component.label = current_loop->values[line][0].trim("$");
										
									Index pos = current_loop->getKeyIndex("_Concentration_value");
									if ( pos > -1) 
									{ 
										component.concentration_value = valueToFloat_(current_loop->values[line][pos]); 
									}	

									pos = current_loop->getKeyIndex("_Concentration_value_units");
									if ( pos > -1) component.value_unit  = current_loop->values[line][pos];

									pos = current_loop->getKeyIndex("_Concentration_min_value");
									if ( pos > -1)
									{
										component.concentration_min = valueToFloat_(current_loop->values[line][pos]);
									}
									
									pos = current_loop->getKeyIndex("_Concentration_max_value");
									if ( pos > -1) 
									{
										component.concentration_max = valueToFloat_(current_loop->values[line][pos]);
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
				for (Size sf=0; sf < saveframes.size(); sf++)
				{
					NMRStarFile::NMRSpectrometer spectrometer;
					spectrometer.name = saveframes[sf].framename;		
						
					if (saveframes[sf].hasItem("_Manufacturer"))
						spectrometer.manufacturer = (saveframes[sf].getDataItemValue("_Manufacturer"));
					if (saveframes[sf].hasItem("_Model"))
						spectrometer.model = saveframes[sf].getDataItemValue("_Model");
					if (saveframes[sf].hasItem("_Field_strength"))
						spectrometer.field_strength = valueToFloat_(saveframes[sf].getDataItemValue("_Field_strength")); 
					
					// store
					nmr_spectrometers_.push_back(spectrometer);
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
			for (Size i=0; i < monomeric_polymers_.size(); i++)
			{
				if (molecular_system_.chemical_units[j].label.trim("$") == monomeric_polymers_[i].label_name)
					molecular_system_.chemical_units[j].monomeric_polymer = &(monomeric_polymers_[i]);
			}
		}

		// set the shifts
		for (Size j=0; j < molecular_system_.chemical_units.size(); j++)
		{
			for (Size i=0; i < atom_data_sets_.size(); i++)
			{
				if (   atom_data_sets_[i].name == molecular_system_.chemical_units[j].component_name
				    || atom_data_sets_[i].name == molecular_system_.chemical_units[j].label)
				{
					molecular_system_.chemical_units[j].shifts = &atom_data_sets_[i];
					break;
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

	bool NMRStarFile::isValidSingleValue_(String value)
	{
		if (value.size()==1)
		{
			for (Size i=0; i<special_characters_.size(); i++)
			{
				if (value == special_characters_[i])
					return false;
			}
		}
		return true;
	}

	float NMRStarFile::valueToFloat_(String value)
	{
		if (isValidSingleValue_(value) && value.isFloat())
			return value.toFloat();	
		else
			return FLOAT_VALUE_NA;
	}

	int NMRStarFile::valueToInt_(String value)
	{
		int result = INT_VALUE_NA;
		
		if (isValidSingleValue_(value))
		{
			try {
				result = value.toInt();
			} catch (Exception::InvalidFormat) {
				// invalid format => INT_VALUE_NA
			}
		}
		
		return result;
	}

	bool NMRStarFile::operator == (const NMRStarFile& f) const 
	{
		return File::operator == (f);
	}

	bool NMRStarFile::operator != (const NMRStarFile& f) const 
	{
		return !(*this == f);
	}

	void NMRStarFile::clear() 
	{
		CIFFile::clear();
		valid_ = true;
		number_of_shift_sets_ = 0;
		number_of_assigned_shifts_ = 0;
		entry_information_.clear();
		molecular_system_.clear();
		atom_data_sets_.clear();
		sample_conditions_.clear(); 
		samples_.clear();
		shift_references_.clear();
		nmr_spectrometers_.clear();
		monomeric_polymers_.clear();
		has_H_shifts_ = false;
	 	has_C_shifts_ = false;
	 	has_N_shifts_ = false;
	}
	
	
} //namespace
