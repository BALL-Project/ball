// $Id: PDBFile.C,v 1.11 2000/02/10 15:12:01 oliver Exp $

#include <BALL/FORMAT/PDBFile.h>

#include <time.h> // time, asctime

#include <BALL/KERNEL/PSE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/CONCEPT/composite.h>
#include <BALL/COMMON/logStream.h>

using namespace std;

namespace BALL 
{

	PDBFile::PDBFile()
		:	PDB_atom_map_(),
			residue_map_(),
			ssbond_list_(),
			helix_list_(),
			sheet_list_(),
			turn_list_(),
			new_helix_secstruc_list_(),
			new_sheet_secstruc_list_(),
			new_turn_secstruc_list_(),
			current_protein_(0),
			current_chain_(0),
			current_residue_(0),
			current_PDB_atom_(0),
			residue_name_(),
			chain_ID_(0),
			residue_sequence_number_(-1),
			insertion_code_(0)
	{
	}

	PDBFile::PDBFile(const String& filename, File::OpenMode open_mode)
		:	PDB_atom_map_(),
			residue_map_(),
			ssbond_list_(),
			helix_list_(),
			sheet_list_(),
			turn_list_(),
			new_helix_secstruc_list_(),
			new_sheet_secstruc_list_(),
			new_turn_secstruc_list_(),
			current_protein_(0),
			current_chain_(0),
			current_residue_(0),
			current_PDB_atom_(0),
			residue_name_(),
			chain_ID_(0),
			residue_sequence_number_(-1),
			insertion_code_(0)
	{
		open(filename, open_mode);
	}


	PDBFile::~PDBFile()
	{
		clear_();
	}

	bool PDBFile::readRecordATOM
		(PDB::Integer serial_number,
		 PDB::Atom atom_name,
		 PDB::Character alternate_location_indicator,
		 PDB::ResidueName residue_name,
		 PDB::Character chain_ID,
		 PDB::Integer residue_sequence_number,
		 PDB::AChar insertion_code,
		 PDB::Real orthogonal_vector[3],
		 PDB::Real occupancy,
		 PDB::Real temperature_factor,
		 PDB::LString4 /* segment_ID */,
		 PDB::LString2 /* element_symbol */,
		 PDB::LString2 /* charge */)
	{
		if (chain_ID != chain_ID_)
		{
			current_chain_ = new Chain();
			current_protein_->insert(*current_chain_);
			
			residue_name_ = "";
			chain_ID_ = chain_ID;
			
			current_chain_->setName(chain_ID_);
		}
		
		if (residue_name_ != (const char *)residue_name
				|| residue_sequence_number != residue_sequence_number_
				|| insertion_code != insertion_code_)
		{
			ResidueQuadruple unique_residue(residue_name, chain_ID, residue_sequence_number, insertion_code);

			current_residue_ = new Residue();
			current_chain_->insert(*current_residue_);
			residue_map_[unique_residue] = current_residue_;
			
			residue_name_ = residue_name;
			residue_sequence_number_ = residue_sequence_number;
			insertion_code_ = insertion_code;
			
			if (current_residue_->hasProperty(Residue::PROPERTY__NON_STANDARD) == false)
			{
				current_residue_->setProperty(Residue::PROPERTY__AMINO_ACID);
			}
			current_residue_->setName(residue_name_);
			current_residue_->setID(residue_sequence_number_);
			current_residue_->setInsertionCode(insertion_code_);
		}

		current_PDB_atom_ = new PDBAtom();
		current_residue_->insert(*current_PDB_atom_);
		PDB_atom_map_[serial_number] = current_PDB_atom_;
		
		current_PDB_atom_->setName(getAtomName(atom_name));
		current_PDB_atom_->setRemotenessIndicator(getAtomRemotenessIndicator(atom_name));
		current_PDB_atom_->setBranchDesignator(getAtomBranchDesignator(atom_name));
		current_PDB_atom_->setAlternateLocationIndicator(alternate_location_indicator);
		current_PDB_atom_->setOccupancy(occupancy);
		current_PDB_atom_->setTemperatureFactor(temperature_factor);
		current_PDB_atom_->setElement(PSE[GenericPDBFile::getAtomElementSymbol(atom_name, atom_name)]);
		//  current_PDB_atom_->setElement(Element::UNKNOWN);
		current_PDB_atom_->setRadius(current_PDB_atom_->getElement().getVanDerWaalsRadius());
		current_PDB_atom_->setPosition(Vector3(orthogonal_vector[0], orthogonal_vector[1], orthogonal_vector[2]));
		
		return true;
	}
		
	bool PDBFile::readRecordCONECT
		(PDB::Integer atom_serial_number,
		 PDB::Integer bonded_atom_serial_number[4],
		 PDB::Integer hydrogen_bonded_atom_serial_number[4],
		 PDB::Integer salt_bridged_atom_serial_number[2])
	{
		PDBAtomMap::Iterator	atom_map_it = PDB_atom_map_.find(atom_serial_number);

		PDBAtom*				PDB_atom = 0;
		Bond*						bond = 0;
		unsigned short	i = 0;

		// extract a pointer to the atom, if it was found in the hash map
		if (atom_map_it == PDB_atom_map_.end())
		{
			return true;  // illegal atom number, abort
		}
		else
		{
			PDB_atom = atom_map_it->second; // retrieve the pointer
		}
		
		// read the entries for the bonds
		for (i = 0; i < 4; ++i)
		{
			if (bonded_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(bonded_atom_serial_number[i]);
				
				// retrieve the second atom via the hash table
				if (atom_map_it != PDB_atom_map_.end())
				{
					// create the new bond
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__COVALENT);
					}
				}
			}
		}
				
		for (i = 0; i < 4; ++i)
		{
			if (hydrogen_bonded_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(hydrogen_bonded_atom_serial_number[i]);
				
				if (atom_map_it != PDB_atom_map_.end())
				{
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__HYDROGEN);
					}
				}
			}
		}
		
		for (i = 0; i < 2; ++i)
		{
			if (salt_bridged_atom_serial_number[i] != 0)
			{
				atom_map_it = PDB_atom_map_.find(salt_bridged_atom_serial_number[i]);
				
				if (atom_map_it != PDB_atom_map_.end())
				{
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__SALT_BRIDGE);
					}
				}
			}
		}
		
		return true;
	}

	bool PDBFile::readRecordHEADER
		(PDB::String40 classification,
		 PDB::Date /* deposition_date */,
		 PDB::IDcode ID_code)
	{
		current_protein_->setName(classification);
		current_protein_->setID(ID_code);

		return true;
	}
		
	bool PDBFile::readRecordHELIX
		(PDB::Integer /* serial_number */,
		 PDB::LString3 helix_ID,
		 PDB::RecordHELIX::InitialResidue& initial_residue,
		 PDB::RecordHELIX::TerminalResidue& terminal_residue,
		 PDB::Integer helix_class,
		 PDB::PDBString /* comment */,
		 PDB::Integer /* length */)
	{
		ResidueQuadruple partner_residue
			(initial_residue.residue_name, initial_residue.chain_ID,
			 initial_residue.sequence_number, initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(helix_ID);
		
		new_helix_secstruc_list_.push_back(sec_struc);
		sec_struc->setProperty(SecondaryStructure::PROPERTY__HELIX);
		sec_struc->setProperty("HELIX_CLASS", (unsigned int)helix_class);
		
		helix_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		helix_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::readRecordHETATM
		(PDB::Integer serial_number,
		 PDB::Atom atom_name,
		 PDB::Character alternate_location_indicator,
		 PDB::ResidueName residue_name,
		 PDB::Character chain_ID,
		 PDB::Integer residue_sequence_number,
		 PDB::AChar insertion_code,
		 PDB::Real orthogonal_vector[3],
		 PDB::Real occupancy,
		 PDB::Real temperature,
		 PDB::LString4 segment_ID,
		 PDB::LString2 element_symbol,
		 PDB::LString2 charge)
	{
		readRecordATOM(serial_number, atom_name, alternate_location_indicator, residue_name,
									 chain_ID, residue_sequence_number, insertion_code, orthogonal_vector,
									 occupancy, temperature, segment_ID, element_symbol, charge); 
		
		current_residue_->clearProperty(Residue::PROPERTY__AMINO_ACID);
		current_residue_->setProperty(Residue::PROPERTY__NON_STANDARD);
		
		RegularExpression regular_expression("^OHH|HOH|HHO|H2O|2HO|OH2|SOL|TIP|TIP2|TIP3|TIP4|WAT|D2O$");
		if (regular_expression.match(current_residue_->getName()) == true)
		{
			current_residue_->setProperty(Residue::PROPERTY__WATER);
		}
		
		return true;
	}

	bool PDBFile::readRecordSHEET
		(PDB::Integer /* strand_number */,
		 PDB::LString3 sheet_ID,
		 PDB::Integer /* number_of_strands */,
		 PDB::RecordSHEET::InitialResidue& initial_residue,
		 PDB::RecordSHEET::TerminalResidue& terminal_residue,
		 PDB::Integer sense_of_strand,
		 PDB::Atom /* atom_name_in_current_strand */,
		 PDB::RecordSHEET::ResidueInCurrentStrand & /* residue_in_current_strand */,
		 PDB::Atom /* atom_name_in_previous_strand */,
		 PDB::RecordSHEET::ResidueInPreviousStrand & /* residue_in_previous_strand */)
	{
		ResidueQuadruple partner_residue
			(initial_residue.residue_name, initial_residue.chain_ID,
			 initial_residue.sequence_number, initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(sheet_ID);
		
		new_sheet_secstruc_list_.push_back(sec_struc);
		sec_struc->setProperty(SecondaryStructure::PROPERTY__STRAND);
		sec_struc->setProperty("STRAND_SENSE", (void *)sense_of_strand);
		
		sheet_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		sheet_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::readRecordSSBOND
		(PDB::Integer /* serial_number */,
		 PDB::RecordSSBOND::PartnerResidue partner_residue[2])
	{
		ResidueQuadruple partner(partner_residue[0].residue_name, partner_residue[0].chain_ID,
														 partner_residue[0].sequence_number, partner_residue[0].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		partner.set(partner_residue[1].residue_name, partner_residue[1].chain_ID,
								partner_residue[1].sequence_number,	partner_residue[1].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		return true;
	}

	bool PDBFile::readRecordTER
		(PDB::Integer /* serial_number */,
		 PDB::ResidueName /* residue_name */,
		 PDB::Character /* chain_ID */,
		 PDB::Integer /* residue_sequence_number */,
		 PDB::AChar /* insertion_code */)
	{
		chain_ID_ = 0;
		
		return true;
	}

	bool  
	PDBFile::readRecordTURN
		(PDB::Integer /* sequence_number */,
		 PDB::LString3 turn_ID,
		 PDB::RecordTURN::InitialResidue &initial_residue,
		 PDB::RecordTURN::TerminalResidue &terminal_residue,
		 PDB::PDBString /* comment */)
	{
		ResidueQuadruple partner_residue(initial_residue.residue_name, initial_residue.chain_ID,
																		 initial_residue.sequence_number,	initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(turn_ID);
		
		new_turn_secstruc_list_.push_back(sec_struc);
		sec_struc->setProperty(SecondaryStructure::PROPERTY__TURN);
		
		turn_list_.push_back(partner_residue);
		
		partner_residue.set(terminal_residue.residue_name, terminal_residue.chain_ID,
												terminal_residue.sequence_number, terminal_residue.insertion_code);
		
		turn_list_.push_back(partner_residue);
		
		return true;
	}

	void PDBFile::read(Protein& protein)
	{
		clear_();
		protein.destroy();
		current_protein_ = &protein;
		readRecords();
		postprocessSSBonds_();
		postprocessHelices_();
		postprocessSheetsTurns_(sheet_list_, new_sheet_secstruc_list_);
		postprocessSheetsTurns_(turn_list_, new_turn_secstruc_list_);
		postprocessRandomCoils_();
	}

	void PDBFile::read(System& system)
	{
		Protein* protein = new Protein();
		read(*protein);
		system.insert(*protein);
	}

	void PDBFile::write(const System& system)
	{
		Size number_of_proteins = system.count(RTTI::getDefault<KernelPredicate<Protein> >());
		if (number_of_proteins > 1)
		{
			Log.error() << "PDBFile::write: cannot write a system with multiple proteins to a PDB file." << endl;
			return;
		}
		
		if (number_of_proteins == 0)
		{
			Protein p;
			((Composite&)p).splice((Composite&)system);
			write_(p, true);
			((Composite&)system).splice((Composite &)p);
		} else {
			Composite::SubcompositeIterator it = system.beginSubcomposite();
			for (; +it && !RTTI::isKindOf<Protein>(*it); ++it);
			if (+it)
			{
				write_(*it, true);
			} else {
				Log.error() << "PDBFile::write: cannot find a protein in the current system." << endl;
			}
		}
	}
	
	void PDBFile::write_(const Composite& composite, bool system)
	{
		const Protein* protein = RTTI::castTo<Protein>(composite);
		const Molecule& molecule = *RTTI::castTo<Molecule>(composite);
  
		char line_buffer[PDB::SIZE_OF_PDB_LINE_BUFFER];
		ChainIterator chain_it;
		SecondaryStructureIterator sec_struc_it;
		SecondaryStructureIterator sheet_it;
		ResidueIterator residue_it;
		ResidueReverseIterator reverse_residue_it;
		PDBAtomIterator atom_iterator;
		AtomIterator atom_it;
		Atom::BondIterator bond_it;
		unsigned long secstruc_serial_number = 0;
		unsigned long residue_sequence_number = 0;
		char chain_name = BALL_CHAIN_DEFAULT_NAME;
		unsigned long atom_serial_number = 0;
		unsigned long ssbond_serial_number = 0;
		char PDB_code[5] = { 0, 0, 0, 0, 0 };
		char PDB_secstruc_name[4] = { 0, 0, 0, 0 };
		char PDB_residue_name[13][4] = { "", "", "", "", "", "", "", "", "", "", "", "", "" };
		char PDB_atom_name[5] = { 0, 0, 0, 0, 0 };
		char element_symbol[3] = { 0, 0, 0 };
		unsigned long number_of_residues_in_chain = 0;
		unsigned long number_of_remark_records = 0;
		unsigned long number_of_het_records = 0;
		unsigned long number_of_helix_records = 0;
		unsigned long number_of_sheet_records = 0;
		unsigned long number_of_turn_records = 0;
		unsigned long number_of_site_records = 0;
		unsigned long number_of_coordinate_transformation_records = 0;
		Fragment *current_fragment = 0;
		unsigned long number_of_atomic_coordinate_records = 0;
		unsigned long number_of_ter_records = 0;
		unsigned long number_of_conect_records = 0;
		unsigned long number_of_seqres_records = 0;
		PDB::RecordType record_type = PDB::RECORD_TYPE__UNKNOWN;
		Residue*							residue[13] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
		SecondaryStructure*		current_sec_struc = 0;
		Atom*									current_atom = 0;
		Size									length_of_secstruc = 0;
		String								temp_string;
		HashMap<void*, PDB::Integer> atom_map;
		Atom* covalent_bonded_atom[4];
		Atom* hydrogen_bonded_atom[4];
		Atom* saltbridge_bonded_atom[2];
		unsigned short covalent_bond = 0;
		unsigned short hydrogen_bond = 0;
		unsigned short saltbridge_bond = 0;

		// --- HEADER ---
		
		/* prepare the current date ... */
		time_t current_time = time(NULL);
		char* date = asctime(localtime(&current_time));
		char PDB_date_format[10];
		
		sprintf(PDB_date_format, "%2.2s-%c%c%c-%2.2s",
						date + 8, toupper(date[4]), toupper(date[5]),
						toupper(date[6]), date + 22);

		if (protein != 0)
		{
			protein->getID().get(PDB_code, 0, 5);
		}
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__HEADER].format_string,
						record_type_format_[PDB::RECORD_TYPE__HEADER].string,
						molecule.getName().c_str(),
						PDB_date_format,
						PDB_code);

		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;

		// --- SEQRES ---

		if (protein != 0)
		{
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_chain_ = &(*chain_it);
				number_of_residues_in_chain = current_chain_->countResidues();
				residue_sequence_number = 0;
				
				BALL_FOREACH_RESIDUE((*chain_it), residue_it)
				{
					current_residue_ = &(*residue_it);

					if (current_residue_->hasProperty(Residue::PROPERTY__NON_STANDARD) == true)
					{
						continue;
					}
				
					residue[residue_sequence_number] = current_residue_;
					residue[residue_sequence_number]->getName().get(PDB_residue_name[residue_sequence_number], 0, 4);
			
					if (residue_sequence_number++ == 12)
					{
						chain_name = current_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

		    
						sprintf(line_buffer, 
										record_type_format_[PDB::RECORD_TYPE__SEQRES].format_string,
										record_type_format_[PDB::RECORD_TYPE__SEQRES].string,
										++number_of_seqres_records,
										chain_name,
										number_of_residues_in_chain,
										PDB_residue_name[0],
										PDB_residue_name[1],
										PDB_residue_name[2],
										PDB_residue_name[3],
										PDB_residue_name[4],
										PDB_residue_name[5],
										PDB_residue_name[6],
										PDB_residue_name[7],
										PDB_residue_name[8],
										PDB_residue_name[9],
										PDB_residue_name[10],
										PDB_residue_name[11],
										PDB_residue_name[12]);
	
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;

						residue_sequence_number = 0;

						PDB_residue_name[0][0]
							= PDB_residue_name[1][0]
							= PDB_residue_name[2][0]
							= PDB_residue_name[3][0]
							= PDB_residue_name[4][0]
							= PDB_residue_name[5][0]
							= PDB_residue_name[6][0]
							= PDB_residue_name[7][0]
							= PDB_residue_name[8][0]
							= PDB_residue_name[9][0]
							= PDB_residue_name[10][0]
							= PDB_residue_name[11][0]
							= PDB_residue_name[12][0]
							= '\0';
					}
				}

				if (residue_sequence_number > 0)
				{
					chain_name = current_chain_->getName().c_str()[0];
					if (chain_name == 0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}

					sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__SEQRES].format_string,
						record_type_format_[PDB::RECORD_TYPE__SEQRES].string,
						++number_of_seqres_records,
						chain_name,
						number_of_residues_in_chain,
						PDB_residue_name[0],
						PDB_residue_name[1],
						PDB_residue_name[2],
						PDB_residue_name[3],
						PDB_residue_name[4],
						PDB_residue_name[5],
						PDB_residue_name[6],
						PDB_residue_name[7],
						PDB_residue_name[8],
						PDB_residue_name[9],
						PDB_residue_name[10],
						PDB_residue_name[11],
						PDB_residue_name[12]);
			
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
				}
			}
		

			// --- HELIX ---

			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);
					
					if (current_sec_struc->hasProperty(SecondaryStructure::PROPERTY__HELIX) == true)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = current_sec_struc->rbeginResidue();

						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						for (length_of_secstruc = 1; !residue_it.isEnd() && residue_it != reverse_residue_it;
								 ++length_of_secstruc, ++residue_it)
						{
						}
			
						chain_name = current_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}
			
						sprintf(line_buffer, 
										record_type_format_[PDB::RECORD_TYPE__HELIX].format_string,
										record_type_format_[PDB::RECORD_TYPE__HELIX].string,
										++number_of_helix_records,
										PDB_secstruc_name,
										PDB_residue_name[0],
										chain_name,
										residue[0]->getID().toLong(),
										residue[0]->getInsertionCode(),
										PDB_residue_name[1],
										chain_name,
										residue[1]->getID().toLong(),
										residue[1]->getInsertionCode(),
										(unsigned long)current_sec_struc->getProperty("HELIX_CLASS").getUnsignedInt(),
										"", // comment not supported, yet
										length_of_secstruc);
					
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}
		

			// --- SHEET ---

			temp_string = "12345";
			
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);

					if (current_sec_struc->hasProperty(SecondaryStructure::PROPERTY__STRAND) == true)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = current_sec_struc->rbeginResidue();

						++number_of_sheet_records;
		
						if (temp_string != current_sec_struc->getName())
						{
							secstruc_serial_number = 0;
							temp_string = current_sec_struc->getName();

							for (length_of_secstruc = 0, sheet_it = sec_struc_it;
									 !sheet_it.isEnd(); ++sheet_it)
							{
								if ((*sheet_it).hasProperty(SecondaryStructure::PROPERTY__STRAND) == true
										&& (*sheet_it).getName() == temp_string)
								{
									++length_of_secstruc;
								}
							}
						}
			
						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						chain_name = current_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{	
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}
						
						sprintf(line_buffer, 
							record_type_format_[PDB::RECORD_TYPE__SHEET].format_string,
							record_type_format_[PDB::RECORD_TYPE__SHEET].string,
							++secstruc_serial_number,
							PDB_secstruc_name,
							length_of_secstruc,
							PDB_residue_name[0],
							chain_name,
							residue[0]->getID().toLong(),
							residue[0]->getInsertionCode(),
							PDB_residue_name[1],
							chain_name,
							residue[1]->getID().toLong(),
							residue[1]->getInsertionCode(),
							(long)current_sec_struc->getProperty("STRAND_SENSE").getUnsignedInt(),
							"",  // Registration. Atom name in current strand not supported, yet
							"",  // Registration. Residue name in current strand not supported, yet
							' ', // Registration. Chain identifier in current strand not supported, yet
							0L,   // Registration. Residue sequence number in current strand not supported, yet
							' ', // Registration. Insertion code in current strand not supported, yet
							"",  // Registration. Atom name in previous strand not supported, yet
							"",  // Registration. Residue name in previous strand not supported, yet
							' ', // Registration. Chain identifier in previous strand not supported, yet
							0L,   // Registration. Residue sequence number in previous strand not supported, yet
							' ');// Registration. Insertion code in previous strand not supported, yet
							
							
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}

			// --- TURN ---

			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_chain_ = &(*chain_it);
				
				BALL_FOREACH_SECONDARYSTRUCTURE(*chain_it, sec_struc_it)
				{
					current_sec_struc = &(*sec_struc_it);
					
					if (current_sec_struc->hasProperty(SecondaryStructure::PROPERTY__TURN) == true)
					{
						residue_it = current_sec_struc->beginResidue();
						if (residue_it.isEnd())
						{
							continue;
						}
						reverse_residue_it = current_sec_struc->rbeginResidue();

						current_sec_struc->getName().get(PDB_secstruc_name, 0, 4);

						residue[0] = &(*residue_it);
						residue[1] = &(*reverse_residue_it);
						residue[0]->getName().get(PDB_residue_name[0], 0, 4);
						residue[1]->getName().get(PDB_residue_name[1], 0, 4);

						for (length_of_secstruc = 1;
								 !residue_it.isEnd() && residue_it != reverse_residue_it;
								 ++length_of_secstruc, ++residue_it)
						{
						}
						
						chain_name = current_chain_->getName().c_str()[0];
						if (chain_name == 0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

						
						sprintf(line_buffer, 
							record_type_format_[PDB::RECORD_TYPE__TURN].format_string,
							record_type_format_[PDB::RECORD_TYPE__TURN].string,
							++number_of_turn_records,
							PDB_secstruc_name,
							PDB_residue_name[0],
							chain_name,
							residue[0]->getID().toLong(),
							residue[0]->getInsertionCode(),
							PDB_residue_name[1],
							chain_name,
							residue[1]->getID().toLong(),
							residue[1]->getInsertionCode(),
							""); // comment not supported, yet
								
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;
					}
				}
			}

			// --- SSBOND ---

			ssbond_serial_number = 0;

			BALL_FOREACH_BOND(*protein, atom_it, bond_it)
			{
				residue[0] = (*bond_it).getFirstAtom()->getAncestor(RTTI::getDefault<Residue>());
				residue[1] = (*bond_it).getSecondAtom()->getAncestor(RTTI::getDefault<Residue>());

				if (residue[0] == 0 || residue[1] == 0
						|| residue[0] == residue[1]
						|| residue[0]->hasProperty(Residue::PROPERTY__HAS_SSBOND) == false
						|| residue[1]->hasProperty(Residue::PROPERTY__HAS_SSBOND) == false)
				{
					continue;
				}
				
				residue[0]->getName().get(PDB_residue_name[0], 0, 4);
				residue[1]->getName().get(PDB_residue_name[1], 0, 4);

				if (residue[0]->getChain() != 0)
				{
					chain_name = residue[0]->getChain()->getName().c_str()[0];
					if (chain_name == (char)0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}
				} else {
					chain_name = BALL_CHAIN_DEFAULT_NAME;
				}
				char chain_name_B;
				if (residue[1]->getChain() != 0)
				{
					chain_name_B = residue[1]->getChain()->getName().c_str()[0];
					if (chain_name_B == (char)0)
					{
						chain_name_B = BALL_CHAIN_DEFAULT_NAME;
					}
				} else {
					chain_name_B = BALL_CHAIN_DEFAULT_NAME;
				}

				sprintf(line_buffer, 
					record_type_format_[PDB::RECORD_TYPE__SSBOND].format_string,
					record_type_format_[PDB::RECORD_TYPE__SSBOND].string,
					++ssbond_serial_number,
					PDB_residue_name[0],
					chain_name,
					residue[0]->getID().toLong(),
					residue[0]->getInsertionCode(),
					PDB_residue_name[1],
					chain_name_B,
					residue[1]->getID().toLong(),
					residue[1]->getInsertionCode(),
					0L,  // Symmetry operator for 1st residue not supported, yet
					0L); // Symmetry operator for 2nd residue not supported, yet
					
				line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
				File::getFileStream() << line_buffer << endl;
			}
			
			// --- HYDBND ---

			// --- SLTBRG ---

		}

		// --- MODEL ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__MODEL].format_string,
						record_type_format_[PDB::RECORD_TYPE__MODEL].string,
						1L);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;

		// --- ATOM/HETATM ---

		atom_serial_number = 0;
		
		if (protein != 0)
		{
			BALL_FOREACH_CHAIN(*protein, chain_it)
			{
				current_chain_ = &(*chain_it);
				current_residue_ = 0;
					
				BALL_FOREACH_RESIDUE((*chain_it), residue_it)
				{
					current_residue_ = &(*residue_it);
					current_residue_->getName().get(PDB_residue_name[0], 0, 4);

					if (current_residue_->hasProperty(Residue::PROPERTY__AMINO_ACID) == true)
					{
						record_type = PDB::RECORD_TYPE__ATOM;
					}	else {
						record_type = PDB::RECORD_TYPE__HETATM;
					}

					BALL_FOREACH_PDBATOM(*current_residue_, atom_iterator)
					{
						++number_of_atomic_coordinate_records;
						
						current_PDB_atom_ = &(*atom_iterator);

						// get the element symbol
						strcpy(element_symbol, current_PDB_atom_->getElement().getSymbol().c_str());

						// normalize the atom name:
						//  if the atom name starts with the element name and the element
						//	name is a single character (e.g. C, N, O, H and the name is not
						//  prefixed by a number) then the name should pe prefixed by
						//  a blank to distinguish CA (carbon alpha) from CA (calcium)
						String name = current_PDB_atom_->getName();
						name.trim();
						Index offset;
						if ((name.size() < 4) && name.hasPrefix(element_symbol) && (strlen(element_symbol) == 1))
						{
							offset = 1;
							PDB_atom_name[0] = ' ';
						} else {
							offset = 0;
						}

						name.get(&(PDB_atom_name[offset]), 0, 4 - offset);
							
						chain_name = current_chain_->getName().c_str()[0];
						if (chain_name == (char)0)
						{
							chain_name = BALL_CHAIN_DEFAULT_NAME;
						}

						sprintf(line_buffer, 
							record_type_format_[record_type].format_string,
							record_type_format_[record_type].string,
							++atom_serial_number,
							PDB_atom_name,
							current_PDB_atom_->getAlternateLocationIndicator(),
							PDB_residue_name[0],
							chain_name,
							current_residue_->getID().toLong(),
							current_residue_->getInsertionCode(),
							current_PDB_atom_->getPosition().x,
							current_PDB_atom_->getPosition().y,
							current_PDB_atom_->getPosition().z,
							current_PDB_atom_->getOccupancy(),
							current_PDB_atom_->getTemperatureFactor(),
							PDB_code,
							element_symbol,
							""); // CHARGE NOT YET SUPPORTED
								
						line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
						File::getFileStream() << line_buffer << endl;

						atom_map[(void *)current_PDB_atom_] =  (long)atom_serial_number;
					}
				}
			

				// --- TER ---

				if (current_residue_ != 0)
				{
					++number_of_ter_records;
					
					chain_name = current_chain_->getName().c_str()[0];

					if (chain_name == 0)
					{
						chain_name = BALL_CHAIN_DEFAULT_NAME;
					}

					sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__TER].format_string,
						record_type_format_[PDB::RECORD_TYPE__TER].string,
						++atom_serial_number,
						PDB_residue_name[0],
						chain_name,
						current_residue_->getID().toLong(),
						current_residue_->getInsertionCode());
					
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
				}

				// ---
				
				if (system == true)
				{
					BaseFragmentIterator frag_it;
		
					BALL_FOREACH_BASEFRAGMENT(molecule, frag_it)
					{	
						if (RTTI::isKindOf<Molecule>(*frag_it) == false
								|| RTTI::isKindOf<Protein>(*frag_it) == true)
						{
							continue;
						}

						// invariant: *frag_it is molecule
						BALL_FOREACH_ATOM(*frag_it, atom_it)
						{
							++number_of_atomic_coordinate_records;
						
							current_atom = &(*atom_it);
							current_atom->getName().get(PDB_atom_name, 0, 3);
							PDB_atom_name[2] = BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
							PDB_atom_name[3] = BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
							PDB_atom_name[4] = '\0';
							strcpy(element_symbol, current_atom->getElement().getSymbol().c_str());
						
							current_fragment = current_atom->getAncestor(RTTI::getDefault<Fragment>());
							if (current_fragment != 0)
							{
								current_fragment->getName().get(PDB_residue_name[0], 0, 3);
							} else {
								PDB_residue_name[0][0] = '\0';
							}

							sprintf(line_buffer, 
									record_type_format_[PDB::RECORD_TYPE__HETATM].format_string,
									record_type_format_[PDB::RECORD_TYPE__HETATM].string,
									++atom_serial_number,
									PDB_atom_name,
									BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR,
									PDB_residue_name[0],
									BALL_CHAIN_DEFAULT_NAME,
									0L,
									BALL_RESIDUE_DEFAULT_INSERTION_CODE,
									current_atom->getPosition().x,
									current_atom->getPosition().y,
									current_atom->getPosition().z,
									0.0f,
									0.0f,
									"",
									"",
									""); // CHARGE NOT YET SUPPORTED
								
							line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
							File::getFileStream() << line_buffer << endl;
								
							atom_map[(void *)current_atom] = (long)atom_serial_number;
						}
					}
				
					/** unnecessary TER record!
					// --- TER ---

					++number_of_ter_records;
							
					sprintf(line_buffer, 
									record_type_format_[PDB::RECORD_TYPE__TER].format_string,
									record_type_format_[PDB::RECORD_TYPE__TER].string,
									++atom_serial_number,
									"",
									BALL_CHAIN_DEFAULT_NAME,
									0L,
									' ');
							
					line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
					File::getFileStream() << line_buffer << endl;
					*/
				}
			}
		} else {
			BALL_FOREACH_ATOM(molecule, atom_it)
    {
      ++number_of_atomic_coordinate_records;

      current_atom = &(*atom_it);
      current_atom->getName().get(PDB_atom_name, 0, 3);
      PDB_atom_name[2] = BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR;
      PDB_atom_name[3] = BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR;
      PDB_atom_name[4] = '\0';
      strcpy(element_symbol, current_atom->getElement().getSymbol().c_str());
      
      current_fragment = current_atom->getAncestor(RTTI::getDefault<Fragment>());
      if (current_fragment != 0)
      {
				current_fragment->getName().get(PDB_residue_name[0], 0, 4);
      } else {
				PDB_residue_name[0][0] = '\0';
      }

      sprintf(line_buffer, 
	      record_type_format_[PDB::RECORD_TYPE__HETATM].format_string,
	      record_type_format_[PDB::RECORD_TYPE__HETATM].string,
	      ++atom_serial_number,
	      PDB_atom_name,
	      BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR,
	      PDB_residue_name[0],
	      BALL_CHAIN_DEFAULT_NAME,
	      0L,
	      BALL_RESIDUE_DEFAULT_INSERTION_CODE,
	      current_atom->getPosition().x,
	      current_atom->getPosition().y,
	      current_atom->getPosition().z,
	      0.0f,
	      0.0f,
	      "",
	      "",
	      ""); // CHARGE NOT YET SUPPORTED
      
      line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
      File::getFileStream() << line_buffer << endl;
      
      atom_map[(void *)current_atom] = (long)atom_serial_number;
    }
  }

  // --- ENDMDL ---
  
  sprintf(line_buffer, 
					record_type_format_[PDB::RECORD_TYPE__ENDMDL].format_string,
					record_type_format_[PDB::RECORD_TYPE__ENDMDL].string);
				
	line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
	File::getFileStream() << line_buffer << endl;

	// --- CONECT ---

  for (atom_it = molecule.beginAtom(); 
			 !atom_it.isEnd(); ++atom_it)
	{
		current_atom = &(*atom_it);

		covalent_bond
			= hydrogen_bond
			= saltbridge_bond
			= 0;

		covalent_bonded_atom[0]
			= covalent_bonded_atom[1]
			= covalent_bonded_atom[2]
			= covalent_bonded_atom[3]
			= 0;
				
		hydrogen_bonded_atom[0]
			= hydrogen_bonded_atom[1]
			= hydrogen_bonded_atom[2]
			= hydrogen_bonded_atom[3]
			= 0;
				
		saltbridge_bonded_atom[0]
			= saltbridge_bonded_atom[1]
			= 0;

		for (bond_it = current_atom->beginBond();
				 !bond_it.isEnd(); ++bond_it)
		{
			switch ((*bond_it).getType())
			{
				case Bond::TYPE__HYDROGEN:

					if (hydrogen_bond < 4)
					{
						hydrogen_bonded_atom[hydrogen_bond]
							= (*bond_it).getBondedAtomOf(*current_atom);

						if (*hydrogen_bonded_atom[hydrogen_bond] >= *current_atom)
						{
							hydrogen_bonded_atom[hydrogen_bond] = 0;
						} else {
							++hydrogen_bond;
						}
					}
					
					break;
			
				case Bond::TYPE__SALT_BRIDGE:

					if (saltbridge_bond < 2)
					{
						saltbridge_bonded_atom[saltbridge_bond]
							= (*bond_it).getBondedAtomOf(*current_atom);

						if (*saltbridge_bonded_atom[saltbridge_bond] >= *current_atom)
						{
							saltbridge_bonded_atom[saltbridge_bond] = 0;
						} else {
							++saltbridge_bond;
						}
					}
					
					break;


				default:

					if (covalent_bond < 4)
					{
						covalent_bonded_atom[covalent_bond]
							= (*bond_it).getBondedAtomOf(*current_atom);

						if (*covalent_bonded_atom[covalent_bond] >= *current_atom)
						{
							covalent_bonded_atom[covalent_bond] = 0;
						} else {
							++covalent_bond;
						}
					}
			}
		}

		if (covalent_bond == 0 && hydrogen_bond == 0 && saltbridge_bond == 0)
		{
			continue;
		}

		++number_of_conect_records;

		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__CONECT].format_string,
						record_type_format_[PDB::RECORD_TYPE__CONECT].string,
						((*atom_map.find((void *)current_atom)).second),
						(covalent_bonded_atom[0] != 0) ? ((*atom_map.find((void *)covalent_bonded_atom[0])).second) : 0,
						(covalent_bonded_atom[1] != 0) ? ((*atom_map.find((void *)covalent_bonded_atom[1])).second) : 0,
						(covalent_bonded_atom[2] != 0) ? ((*atom_map.find((void *)covalent_bonded_atom[2])).second) : 0,
						(covalent_bonded_atom[3] != 0) ? ((*atom_map.find((void *)covalent_bonded_atom[3])).second) : 0,
						(hydrogen_bonded_atom[0] != 0) ? ((*atom_map.find((void *)hydrogen_bonded_atom[0])).second) : 0,
						(hydrogen_bonded_atom[1] != 0) ? ((*atom_map.find((void *)hydrogen_bonded_atom[1])).second) : 0,
						(saltbridge_bonded_atom[0] != 0) ? ((*atom_map.find((void *)saltbridge_bonded_atom[0])).second) : 0,
						(hydrogen_bonded_atom[2] != 0) ? ((*atom_map.find((void *)hydrogen_bonded_atom[2])).second) : 0,
						(hydrogen_bonded_atom[3] != 0) ? ((*atom_map.find((void *)hydrogen_bonded_atom[3])).second) : 0,
						(saltbridge_bonded_atom[1] != 0) ? ((*atom_map.find((void *)saltbridge_bonded_atom[1])).second) : 0);

		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
	}
		
		// --- MASTER ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__MASTER].format_string,
						record_type_format_[PDB::RECORD_TYPE__MASTER].string,
						number_of_remark_records,
						0L,
						number_of_het_records,
						number_of_helix_records,
						number_of_sheet_records,
						number_of_turn_records,
						number_of_site_records,
						number_of_coordinate_transformation_records,
						number_of_atomic_coordinate_records,
						number_of_ter_records,
						number_of_conect_records,
						number_of_seqres_records);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
		
		// --- END ---
		
		sprintf(line_buffer, 
						record_type_format_[PDB::RECORD_TYPE__END].format_string,
						record_type_format_[PDB::RECORD_TYPE__END].string);
				
		line_buffer[PDB::SIZE_OF_PDB_RECORD_LINE + 1] = '\0';
		File::getFileStream() << line_buffer << endl;
	}

	void PDBFile::clear_()
	{
		GenericPDBFile::clear_();
		
		PDB_atom_map_.destroy();
		residue_map_.destroy();
		ssbond_list_.clear();
		helix_list_.clear();
		sheet_list_.clear();
		turn_list_.clear();
		new_helix_secstruc_list_.clear();
		new_sheet_secstruc_list_.clear();
		new_turn_secstruc_list_.clear();
		current_protein_ = 0;
		current_chain_ = 0;
		current_residue_ = 0;
		current_PDB_atom_ = 0;
		residue_name_ = "";
		chain_ID_ = 0;
		residue_sequence_number_ = -1;
		insertion_code_ = 0;
	}

	void PDBFile::postprocessSSBonds_()
	{
		ResidueMap::Iterator first;
		ResidueMap::Iterator second;
		
		QuadrupleList::iterator it;
		for (it = ssbond_list_.begin(); it != ssbond_list_.end(); ++it)
		{
			first = residue_map_.find(*it);
			++it;
			second = residue_map_.find(*it);
			
			if (first != residue_map_.end() && second != residue_map_.end())
			{
				first->second->setProperty(Residue::PROPERTY__HAS_SSBOND);
				second->second->setProperty(Residue::PROPERTY__HAS_SSBOND);
			} else {
				Log.warn() << "PDBFile::postprocessSSBonds_: could not assign SSBOND for " << *it << endl;
			}
		}
	}

	void  PDBFile::postprocessHelices_()
	{
		ResidueMap::Iterator initial;
		ResidueMap::Iterator terminal;
		QuadrupleList::iterator res_it = helix_list_.begin();
		SecStrucList::iterator helix_it = new_helix_secstruc_list_.begin();
		
		for (; res_it != helix_list_.end(); ++res_it, ++helix_it)
		{
			initial = residue_map_.find(*res_it);
			++res_it;
			terminal = residue_map_.find(*res_it);
			
			if (!(initial != residue_map_.end() && terminal != residue_map_.end()
						&& initial->second->getChain() == terminal->second->getChain()
						&& Composite::insertParent(*(*helix_it), *initial->second, *terminal->second, false) == true))
			{
				delete (*helix_it);
			}
		}
	}

	void  PDBFile::postprocessSheetsTurns_
		(PDBFile::QuadrupleList& secstruc_list,
		 PDBFile::SecStrucList& new_secstruc_list)
	{
		ResidueIterator protein_res_it;
		ResidueMap::Iterator initial;
		ResidueMap::Iterator terminal;
		QuadrupleList::iterator res_it = secstruc_list.begin();
		SecStrucList::iterator secstruc_it = new_secstruc_list.begin();
		
		for (; res_it != secstruc_list.end(); ++res_it, ++secstruc_it)
		{
			initial = residue_map_.find(*res_it);
			++res_it;
			terminal = residue_map_.find(*res_it);
			
			if (initial != residue_map_.end() && terminal != residue_map_.end())
			{
				Residue* initial_residue = initial->second;
				Residue* terminal_residue = terminal->second;
			
				for (protein_res_it = current_protein_->beginResidue(); !protein_res_it.isEnd();	++protein_res_it)
				{
					if (*protein_res_it == *terminal_residue)
					{
						Residue *temp_residue = terminal_residue;
						terminal_residue = initial_residue;
						initial_residue = temp_residue;
					}
		
					if (*protein_res_it == *initial_residue)
					{
						for (; !protein_res_it.isEnd() && (*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>());
								 ++protein_res_it)
						{	
							if (*protein_res_it == *terminal_residue)
							{
								initial_residue = terminal_residue = 0;
					
								break;
							}
						}

						if (initial_residue == 0 || protein_res_it.isEnd())
						{
							break;
						}

						if (!protein_res_it.isEnd())
						{
							initial_residue = &(*protein_res_it);

							for (; !protein_res_it.isEnd(); ++protein_res_it)
							{
								if ((*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>()))
								{
									--protein_res_it;

									break;
								}

								if (*protein_res_it == *terminal_residue)
								{
									break;
								}
							}

							if (protein_res_it.isValid())
							{
								terminal_residue = &(*protein_res_it);
							}
							else
							{
								initial_residue = terminal_residue = 0;
							}
						}
						else
						{
							initial_residue = terminal_residue = 0;
						}
			
						break;
					}
				}

				if (initial_residue == 0 || terminal_residue == 0
						|| Composite::insertParent(*(*secstruc_it), *initial_residue, *terminal_residue, false) == false)
				{
					delete (*secstruc_it);
				}
			}
			else
			{
				delete (*secstruc_it);
			}
		}
	}

	void PDBFile::postprocessRandomCoils_()
	{
		ResidueIterator protein_res_it;
		Residue *initial_residue = 0;
		Residue *terminal_residue = 0;
		SecondaryStructure *sec_struc = 0;
		
		for (ChainIterator chain_it = current_protein_->beginChain();
				 !chain_it.isEnd();
				 ++chain_it)
		{
			for (protein_res_it = (*chain_it).beginResidue();
					 !protein_res_it.isEnd(); ++protein_res_it)
			{
				if ((*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID) == true
						&& (*protein_res_it).Composite::hasAncestor(RTTI::getDefault<SecondaryStructure>()) == false)
				{
					terminal_residue = initial_residue = &(*protein_res_it);
		
					for (; !protein_res_it.isEnd()
							 && (*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID) == true
							 && (*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>()) == false;
							 ++protein_res_it)
					{
						terminal_residue = &(*protein_res_it);
					}
		
					sec_struc = new SecondaryStructure();
					sec_struc->setProperty(SecondaryStructure::PROPERTY__RANDOM_COIL);
		
					Composite::insertParent(*sec_struc, *initial_residue, *terminal_residue, false);
		
					if (protein_res_it.isEnd() == true)
					{
						break;
					}
				}
			}
		}
	}

#	ifdef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL
