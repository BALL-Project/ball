// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// This file contains the more or less implementation specific portion of PDBFile.
// It covers the methods doing the specific parsing of all parsed records.
// More general methods are contained in PDBFileGeneral.C

#include <BALL/FORMAT/PDBFile.h>

#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/forEach.h>
#include <BALL/COMMON/logStream.h>
#include <BALL/DATATYPE/regularExpression.h>
#include <BALL/XRAY/crystalInfo.h>

#include <ctime> // time, asctime
#include <cctype>
#include <cstdarg>
#include <cstdlib>

using namespace std;

namespace BALL
{

	const char* PDBFile::getAtomElementSymbol
		(const PDB::Atom atom_name, PDB::Atom element_symbol)
	{
		// if the element symbol starts with a blank,
		// drop the blank
		if (element_symbol[0] == ' ')
		{
			element_symbol[0] = element_symbol[1];
			element_symbol[1] = '\0';
		}
		// If the element_symbol entry is valid, it has precedence,
		// otherwise, we try to extract it from the atom name.
		if (PTE[element_symbol] == Element::UNKNOWN)
		{
			// Otherwise, we try to reconstruct the element
			// from the atom name (which is dangerous if non-PDB names are
		  // present, e.g. names like HE12 which is read as He!)
			if (atom_name[0] == ' ' || isdigit(atom_name[0]))
			{
				if (atom_name[1] == ' ')
				{
					element_symbol[0] = '\0';
				}
				else
				{
					element_symbol[0] = atom_name[1];
					element_symbol[1] = '\0';
				}
			}
			else
			{
				element_symbol[0] = atom_name[0];
				element_symbol[1] = atom_name[1];
				element_symbol[2] = '\0';
			}
		}

		return (const char*)element_symbol;
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

				AtomIterator a_it = first->second->beginAtom();
				for(; +a_it; ++a_it)
				{
					if(a_it->getElement().getAtomicNumber() == 16) {
						break;
					}
				}

				AtomIterator b_it = second->second->beginAtom();
				for(; +b_it; ++b_it)
				{
					if(b_it->getElement().getAtomicNumber() == 16) {
						break;
					}
				}

				if(+a_it && +b_it) {
					Bond* bond = a_it->getBond(*b_it);
					if(bond) bond->setType(Bond::TYPE__DISULPHIDE_BRIDGE);
				}
			}
		}
	}

	void  PDBFile::postprocessHelices_()
	{
		QuadrupleList::iterator res_it = helix_list_.begin();
		SecStructList::iterator helix_it = new_helix_secstruc_list_.begin();
		
		for (; (res_it != helix_list_.end()) && (helix_it != new_helix_secstruc_list_.end()); 
				 ++res_it, ++helix_it)
		{
			// Check whether the two residues exist in the structure
			// (i.e. a residue has been constructed).
			if (!residue_map_.has(*res_it))
			{
				delete *helix_it;
				++res_it;
				continue;
			}
			Residue* initial(residue_map_[*res_it]);
				
			++res_it;

			// Find the last residue. Caveat: under strange circumstances,
			// res_it mitght be invalid, so test for it first.
			if ((res_it == helix_list_.end()) || !residue_map_.has(*res_it))
			{
				delete *helix_it;
				continue;
			}
			Residue* terminal(residue_map_[*res_it]);

			// Delete the constructed helix object if
			// it was not valid and could not be inserted correctly.
			// Otherwise, responsability for the memory is taken over by
			// the System/Chain/Protein it is contained in.
			bool delete_helix = true;
			if ((initial->getParent() == terminal->getParent())
					&& (initial->getParent() != 0))
			{
				const Residue* ptr = initial;
				while (ptr != 0 && ptr != terminal) 
				{
					ptr = dynamic_cast<const Residue*>(ptr->getNext(RTTI::getDefault<Residue>()));
				}
				if (ptr == 0)
				{
					std::swap(initial, terminal);
				}
				ptr = initial;
				while (ptr != 0 && ptr != terminal)
				{
					ptr = dynamic_cast<const Residue*>(ptr->getNext(RTTI::getDefault<Residue>()));
				}
				if (ptr == 0)
				{
					continue;
				}
				
				delete_helix = !Composite::insertParent(**helix_it, *initial, *terminal, false);
			}
			if (delete_helix)
			{
				delete (*helix_it);
			}			
		}
	}

	void  PDBFile::postprocessSheetsTurns_
		(PDBFile::QuadrupleList& secstruc_list,
		 PDBFile::SecStructList& new_secstruc_list)
	{
		ResidueIterator protein_res_it;
		ResidueMap::Iterator initial;
		ResidueMap::Iterator terminal;
		QuadrupleList::iterator res_it = secstruc_list.begin();
		SecStructList::iterator secstruc_it = new_secstruc_list.begin();
		
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
		if (current_protein_ == 0)
		{
			return;
		}

		ResidueIterator protein_res_it;
		Residue* initial_residue = 0;
		Residue* terminal_residue = 0;
		SecondaryStructure* sec_struc = 0;
		
		for (ChainIterator chain_it = current_protein_->beginChain();
				 +chain_it; ++chain_it)
		{
			for (protein_res_it = chain_it->beginResidue();
					 +protein_res_it; ++protein_res_it)
			{
				if ((*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID)
						&& !(*protein_res_it).Composite::hasAncestor(RTTI::getDefault<SecondaryStructure>()))
				{
					terminal_residue = initial_residue = &(*protein_res_it);
		
					for (; !protein_res_it.isEnd()
							 && (*protein_res_it).hasProperty(Residue::PROPERTY__AMINO_ACID)
							 && !(*protein_res_it).hasAncestor(RTTI::getDefault<SecondaryStructure>());
							 ++protein_res_it)
					{
						terminal_residue = &(*protein_res_it);
					}
		
					sec_struc = new SecondaryStructure;
					sec_struc->setType(SecondaryStructure::COIL);
		
					Composite::insertParent(*sec_struc, *initial_residue, *terminal_residue, false);
		
					if (protein_res_it.isEnd())
					{
						break;
					}
				}
			}
		}
	}


	bool PDBFile::interpretRecord(const PDB::RecordATOM& record)
	{
		if (record.residue.chain_ID != chain_ID_)
		{
			current_chain_ = new Chain;
			current_protein_->insert(*current_chain_);
			
			residue_name_ = "";
			chain_ID_ = record.residue.chain_ID;
			
			current_chain_->setName(chain_ID_);
		}
		
		// make sure we read only the first location if alternate
		// locations are present to avoid invalid structures due
		// to duplicate atoms
		if(   (record.alternate_location_indicator != ' ' )
		   && (record.alternate_location_indicator != alternate_location_indicator_))
		{
			return true;
		}

		if (residue_name_ != (const char*)record.residue.name
				|| (record.residue.sequence_number != residue_sequence_number_)
				|| (record.residue.insertion_code != insertion_code_))
		{
			ResidueQuadruple unique_residue(record.residue.name, record.residue.chain_ID, 
																			record.residue.sequence_number, record.residue.insertion_code);

			current_residue_ = new Residue;
			current_chain_->insert(*current_residue_);
			residue_map_[unique_residue] = current_residue_;
			
			residue_name_ = record.residue.name;
			residue_sequence_number_ = record.residue.sequence_number;
			insertion_code_ = record.residue.insertion_code;
			
			if (!current_residue_->hasProperty(Residue::PROPERTY__NON_STANDARD))
			{
				current_residue_->setProperty(Residue::PROPERTY__AMINO_ACID);
			}
			String trimmed_name = residue_name_;
			current_residue_->setName(trimmed_name.trim());
			current_residue_->setID(residue_sequence_number_);
			current_residue_->setInsertionCode(insertion_code_);
		}

		current_PDB_atom_ = new PDBAtom;
		current_residue_->insert(*current_PDB_atom_);
		PDB_atom_map_[record.serial_number] = current_PDB_atom_;

		current_PDB_atom_->setName(getAtomName(record.atom_name));
		current_PDB_atom_->setRemotenessIndicator(getAtomRemotenessIndicator(record.atom_name));
		current_PDB_atom_->setBranchDesignator(getAtomBranchDesignator(record.atom_name));
		current_PDB_atom_->setAlternateLocationIndicator(record.alternate_location_indicator);
		current_PDB_atom_->setOccupancy(record.occupancy);
		current_PDB_atom_->setTemperatureFactor(record.temperature_factor);
		current_PDB_atom_->setRadius(current_PDB_atom_->getElement().getVanDerWaalsRadius());
		current_PDB_atom_->setPosition(Vector3(record.orthogonal_vector[0], record.orthogonal_vector[1], record.orthogonal_vector[2]));

		// Figuring out the element is nto entirely trivial: it *should* be in columns 77-78.
		// However, some codes abuse cols. 77-80 for partial charges. We support this format w/ the
		// option PARTIAL_CHARGES for reading and writing.
		if (parse_partial_charges_)
		{
			// ????
		}
		else
		{
			char element_symbol[3];
			element_symbol[0] = record.element_symbol[0];
			element_symbol[1] = record.element_symbol[1];
			element_symbol[2] = '\0';
			current_PDB_atom_->setElement(PTE[PDBFile::getAtomElementSymbol(record.atom_name, element_symbol)]);
			current_PDB_atom_->setFormalCharge(0);
			try
			{
				if (record.charge[1] == '+' || record.charge[1] == '-')
				{
					current_PDB_atom_->setFormalCharge(String(record.charge).toInt());
				}
			}
			catch (Exception::InvalidFormat&)
			{
			}
			current_PDB_atom_->setCharge((float)current_PDB_atom_->getFormalCharge());
		}
		
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordATOM& record)
	{
		record.element_symbol[0] = '\0';
		record.occupancy = 1.0;
		record.temperature_factor = 0.0;
		record.segment_ID[0] = '\0';
		record.charge[0] = '\0';
		record.partial_charge[0] = '\0';

		if (parse_partial_charges_)
		{
			return parseLine
				(line, size, 
				 PDB::FORMAT_ATOM_PARTIAL_CRG,
				 record.record_name,
				 &record.serial_number,
				 record.atom_name,
				 &record.alternate_location_indicator,
				 record.residue.name,
				 &record.residue.chain_ID,
				 &record.residue.sequence_number,
				 &record.residue.insertion_code,
				 &record.orthogonal_vector[0],
				 &record.orthogonal_vector[1],
				 &record.orthogonal_vector[2],
				 &record.occupancy,
				 &record.temperature_factor,
				 record.segment_ID,
				 record.partial_charge);			
		}
		else	
		{
			return parseLine
				(line, size, 
				 PDB::FORMAT_ATOM,
				 record.record_name,
				 &record.serial_number,
				 record.atom_name,
				 &record.alternate_location_indicator,
				 record.residue.name,
				 &record.residue.chain_ID,
				 &record.residue.sequence_number,
				 &record.residue.insertion_code,
				 &record.orthogonal_vector[0],
				 &record.orthogonal_vector[1],
				 &record.orthogonal_vector[2],
				 &record.occupancy,
				 &record.temperature_factor,
				 record.segment_ID,
				 record.element_symbol,
				 record.charge,
				 record.partial_charge);
		}
	}
		
	bool PDBFile::parseRecordATOM(const char* line, Size size)
	{
		// Parse only those records belonging to the correct model!
		if ((selected_model_ != -1) && (selected_model_ != current_model_))
		{
			return true;
		}
		static PDB::RecordATOM record;
		if (!fillRecord(line, size, record))
		{
			return false;
		}
		if (ignore_xplor_pseudo_atoms_
				&& record.orthogonal_vector[0] >= 9998.0
				&& record.orthogonal_vector[1] >= 9998.0
				&& record.orthogonal_vector[2] >= 9998.0)
		{ // ignore XPLOR pseudo atoms (see Rasmol2.6 source 'molecule.c/ReadPDBAtom')
			return true;
		}
		return interpretRecord(record);
	}
	
	bool PDBFile::parseRecordCONECT(const char* line, Size size)
	{
		static PDB::RecordCONECT record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCONECT& record)
	{
		record.clear();
		return parseLine(line, size, PDB::FORMAT_CONECT,
					 record.record_name, &record.atom_serial_number,
					 &record.bond_atom[0], &record.bond_atom[1],
					 &record.bond_atom[2], &record.bond_atom[3],
					 &record.hbond_atom[0],&record.hbond_atom[1],
					 &record.salt_bridge_atom[0],
					 &record.hbond_atom[2], &record.hbond_atom[3],
					 &record.salt_bridge_atom[1]);
	}

	bool PDBFile::interpretRecord(const PDB::RecordCONECT& record)
	{
		PDBAtom* PDB_atom = 0;
		Bond*	bond = 0;
		unsigned short i = 0;

		// extract a pointer to the atom, if it was found in the hash map
		PDBAtomMap::Iterator atom_map_it = PDB_atom_map_.find(record.atom_serial_number);
		if (atom_map_it == PDB_atom_map_.end())
		{
			return false;  // illegal atom number, abort
		}
		else
		{
			PDB_atom = atom_map_it->second; // retrieve the pointer
		}

		try
		{
			//Metal knows no bo(u)nds
			if(!PDB_atom->getElement().isMetal())
			{
				// read the entries for the bonds
				for (i = 0; i < 4; ++i)
				{
					if (record.bond_atom[i] == 0) continue;
					
					// retrieve the second atom via the hash table
					atom_map_it = PDB_atom_map_.find(record.bond_atom[i]);				
					if (atom_map_it == PDB_atom_map_.end()) continue;

					//Metal knows no bo(u)nds. See above
					if(atom_map_it->second->getElement().isMetal()) {
						continue;
					}

					// create the new bond
					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__COVALENT);
						bond->setOrder(Bond::ORDER__SINGLE);
					}
				}
						
				for (i = 0; i < 4; ++i)
				{
					if (record.hbond_atom[i] == 0) continue;
					
					atom_map_it = PDB_atom_map_.find(record.hbond_atom[i]);
					if (atom_map_it == PDB_atom_map_.end()) continue;

					//Metal knows no bo(u)nds. See above
					if(atom_map_it->second->getElement().isMetal()) {
						continue;
					}

					bond = PDB_atom->createBond(*atom_map_it->second);
		
					if (bond != 0)
					{
						bond->setType(Bond::TYPE__HYDROGEN);
						bond->setOrder(Bond::ORDER__SINGLE);
					}
				}
			}
			
			for (i = 0; i < 2; ++i)
			{
				if (record.salt_bridge_atom[i] == 0) continue;
				
				atom_map_it = PDB_atom_map_.find(record.salt_bridge_atom[i]);
				if (atom_map_it == PDB_atom_map_.end()) continue;
			
				bond = PDB_atom->createBond(*atom_map_it->second);
	
				if (bond != 0)
				{
					bond->setType(Bond::TYPE__SALT_BRIDGE);
					bond->setOrder(Bond::ORDER__SINGLE);
				}
			}
		}
		catch (Exception::TooManyBonds& e)
		{
			Log.error() << e << std::endl;
			return false;
		}
		
		return true;
	}


  bool PDBFile::parseRecordCRYST1(const char*  line, Size size)
	{
		static PDB::RecordCRYST1 record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordCRYST1& record)
	{
		return parseLine(line, size, PDB::FORMAT_CRYST1,
					 record.record_name, &record.unit_cell.a,
					 &record.unit_cell.b, &record.unit_cell.c,
					 &record.unit_cell.alpha, &record.unit_cell.beta,
					 &record.unit_cell.gamma, record.unit_cell.space_group,
					 &record.unit_cell.z_value);
	}
		
	bool PDBFile::interpretRecord(const PDB::RecordCRYST1& record )
	{
		if (!(current_protein_->hasProperty("CRYSTALINFO")))
		{
			boost::shared_ptr<PersistentObject> temp_ptr(new CrystalInfo());
			current_protein_->setProperty(NamedProperty("CRYSTALINFO", temp_ptr));
		}

		boost::shared_ptr<CrystalInfo> ci_ptr;
		ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(current_protein_->getProperty("CRYSTALINFO").getSmartObject());
		
		ci_ptr->setSpaceGroup(String(record.unit_cell.space_group));
		ci_ptr->setCellEdgeLengthA(record.unit_cell.a);
		ci_ptr->setCellEdgeLengthB(record.unit_cell.b);
		ci_ptr->setCellEdgeLengthC(record.unit_cell.c);
		ci_ptr->setCellAngleAlpha(Angle(record.unit_cell.alpha, false));
		ci_ptr->setCellAngleBeta(Angle(record.unit_cell.beta, false));
		ci_ptr->setCellAngleGamma(Angle(record.unit_cell.gamma, false));
		ci_ptr->setZScore(record.unit_cell.z_value);

    return true;
	}


	bool PDBFile::parseRecordENDMDL(const char* line, Size size)
	{
		static PDB::RecordENDMDL record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordENDMDL& record)
	{
		return parseLine(line, size, PDB::FORMAT_ENDMDL,record.record_name);
	}

	bool PDBFile::interpretRecord(const PDB::RecordENDMDL& /* record */)
	{
		// We are now outside a model...
		current_model_ = INVALID_INDEX;
		return true;
	}
																												

	bool PDBFile::parseRecordHEADER(const char* line, Size size)
	{
		static PDB::RecordHEADER record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHEADER& record)
	{
		return parseLine(line, size, PDB::FORMAT_HEADER,
										 record.record_name, record.classification, record.deposition_date, record.ID_code);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHEADER& record)
	{
		current_protein_->setName(record.classification);	
		current_protein_->setID(record.ID_code);
		info.setName(record.classification);
		info.setID(record.ID_code);

		return true;
	}

	bool PDBFile::parseRecordHELIX(const char* line, Size size)
	{
		static PDB::RecordHELIX record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordHELIX& record)
	{
		ResidueQuadruple partner_residue
			(record.initial_residue.name, record.initial_residue.chain_ID,
			 record.initial_residue.sequence_number, record.initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(record.helix_ID);
		
		new_helix_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::HELIX);
		sec_struc->setProperty("HELIX_CLASS", (Index)record.helix_class);
		
		helix_list_.push_back(partner_residue);
		
		partner_residue.set(record.terminal_residue.name, record.terminal_residue.chain_ID,
												record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);
		
		helix_list_.push_back(partner_residue);
		
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHELIX& record)
	{
		return parseLine(line, size, PDB::FORMAT_HELIX,
										 record.record_name, &record.serial_number,
										 record.helix_ID, record.initial_residue.name,
										 &record.initial_residue.chain_ID, &record.initial_residue.sequence_number,
										 &record.initial_residue.insertion_code, record.terminal_residue.name,
										 &record.terminal_residue.chain_ID, &record.terminal_residue.sequence_number,
										 &record.terminal_residue.insertion_code, &record.helix_class,
										 record.comment, &record.length);
	}

	bool PDBFile::parseRecordHETATM(const char* line, Size size)
	{
		if ((selected_model_ != -1) && (selected_model_ != current_model_))
		{
			return true;
		}
		parseRecordATOM(line, size); 
		
		current_residue_->clearProperty(Residue::PROPERTY__AMINO_ACID);
		current_residue_->setProperty(Residue::PROPERTY__NON_STANDARD);
		current_PDB_atom_->setProperty(PDBAtom::PROPERTY__HETATM);
		
		static RegularExpression regular_expression("^OHH|HOH|HHO|H2O|2HO|OH2|SOL|TIP|TIP2|TIP3|TIP4|WAT|D2O$");
		if (regular_expression.match(current_residue_->getName()))
		{
			current_residue_->setProperty(Residue::PROPERTY__WATER);
		}
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHETATM& record)
	{
		return parseLine(line, size, PDB::FORMAT_HETATM,
										 record.record_name, &record.serial_number,
										 record.atom_name, &record.alternate_location_indicator,
										 record.residue.name, &record.residue.chain_ID, &record.residue.sequence_number,
										 &record.residue.insertion_code, &record.orthogonal_vector[0],
										 &record.orthogonal_vector[1], &record.orthogonal_vector[2],
										 &record.occupancy, &record.temperature_factor,
										 record.segment_ID, record.element_symbol, record.charge);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHETATM& /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordHYDBND(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordHYDBND& record)
	{
		return parseLine(line, size, PDB::FORMAT_HYDBND,
										 record.record_name, record.hydrogen_partner_atom[0].atom_name,
										 &record.hydrogen_partner_atom[0].alternate_location_indicator,
										 record.hydrogen_partner_atom[0].residue.name,
										 &record.hydrogen_partner_atom[0].residue.chain_ID,
										 &record.hydrogen_partner_atom[0].residue.sequence_number,
										 &record.hydrogen_partner_atom[0].residue.insertion_code,
										 record.hydrogen_atom.atom_name,
										 &record.hydrogen_atom.alternate_location_indicator,
										 &record.hydrogen_atom.residue.chain_ID,
										 &record.hydrogen_atom.residue.sequence_number,
										 &record.hydrogen_atom.residue.insertion_code,
										 record.hydrogen_partner_atom[1].atom_name,
										 &record.hydrogen_partner_atom[1].alternate_location_indicator,
										 record.hydrogen_partner_atom[1].residue.name,
										 &record.hydrogen_partner_atom[1].residue.chain_ID,
										 &record.hydrogen_partner_atom[1].residue.sequence_number,
										 &record.hydrogen_partner_atom[1].residue.insertion_code,
										 &record.first_non_hydrogen_atom,
										 &record.second_non_hydrogen_atom);
	}

	bool PDBFile::interpretRecord(const PDB::RecordHYDBND& /* record */)
	{
		return true;
	}

	bool PDBFile::parseRecordMTRIX1(const char* line, Size size)
	{
		static PDB::RecordMTRIX1 record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX1& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX1,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX1& record)
	{
		//CrystalInfo* ci_ptr;
		if (!(current_protein_->hasProperty("CRYSTALINFO")))
		{
			boost::shared_ptr<PersistentObject> temp_ptr(new CrystalInfo());
			current_protein_->setProperty(NamedProperty("CRYSTALINFO", temp_ptr));
		}

		boost::shared_ptr<CrystalInfo> ci_ptr;
		ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(current_protein_->getProperty("CRYSTALINFO").getSmartObject());
		
		if (!(ci_ptr->getNumberOfNCSSymOps() >= (Size)record.serial_number))
		{
			Matrix4x4 new_mtrix;
			new_mtrix.setIdentity();
			ci_ptr->pushbackNCS(new_mtrix, record.is_given);
		}
		
		Matrix4x4& curr_mtrx = ci_ptr->getNCS(record.serial_number-1);
		curr_mtrx(0,0) = record.transformation_matrix[0];
		curr_mtrx(0,1) = record.transformation_matrix[1];
		curr_mtrx(0,2) = record.transformation_matrix[2];
		curr_mtrx(0,3) = record.transformation_matrix[3];
		
		return true;
	}


	bool PDBFile::parseRecordMTRIX2(const char* line, Size size)
	{
		static PDB::RecordMTRIX2 record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX2& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX2,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX2& record)
	{
		if (!(current_protein_->hasProperty("CRYSTALINFO")))
		{
			boost::shared_ptr<PersistentObject> temp_ptr(new CrystalInfo());
			current_protein_->setProperty(NamedProperty("CRYSTALINFO", temp_ptr));
		}

		boost::shared_ptr<CrystalInfo> ci_ptr;
		ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(current_protein_->getProperty("CRYSTALINFO").getSmartObject());
		
		if (!(ci_ptr->getNumberOfNCSSymOps() >= (Size)record.serial_number))
		{
			Matrix4x4 new_mtrix;
			new_mtrix.setIdentity();
			ci_ptr->pushbackNCS(new_mtrix, record.is_given);
		}
		
		Matrix4x4& curr_mtrx = ci_ptr->getNCS(record.serial_number-1);
		curr_mtrx(1,0) = record.transformation_matrix[0];
		curr_mtrx(1,1) = record.transformation_matrix[1];
		curr_mtrx(1,2) = record.transformation_matrix[2];
		curr_mtrx(1,3) = record.transformation_matrix[3];

		return true;
	}


	bool PDBFile::parseRecordMTRIX3(const char* line, Size size)
	{
		static PDB::RecordMTRIX3 record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMTRIX3& record)
	{
		return parseLine(line, size, PDB::FORMAT_MTRIX3,
										 record.record_name, &record.serial_number,
										 &record.transformation_matrix[0],
										 &record.transformation_matrix[1],
										 &record.transformation_matrix[2],
										 &record.transformation_matrix[3],
										 &record.is_given);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMTRIX3& record)
	{
		if (!(current_protein_->hasProperty("CRYSTALINFO")))
		{
			boost::shared_ptr<PersistentObject> temp_ptr(new CrystalInfo());
			current_protein_->setProperty(NamedProperty("CRYSTALINFO", temp_ptr));
		}

		boost::shared_ptr<CrystalInfo> ci_ptr;
		ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(current_protein_->getProperty("CRYSTALINFO").getSmartObject());
		
		if (!(ci_ptr->getNumberOfNCSSymOps() >= (Size)record.serial_number))
		{
			Matrix4x4 new_mtrix;
			new_mtrix.setIdentity();
			ci_ptr->pushbackNCS(new_mtrix, record.is_given);
		}
		
		Matrix4x4& curr_mtrx = ci_ptr->getNCS(record.serial_number-1);
		curr_mtrx(2,0) = record.transformation_matrix[0];
		curr_mtrx(2,1) = record.transformation_matrix[1];
		curr_mtrx(2,2) = record.transformation_matrix[2];
		curr_mtrx(2,3) = record.transformation_matrix[3];

		return true;
	}



	bool PDBFile::parseRecordMODEL(const char* line, Size size)
	{
		static PDB::RecordMODEL record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
		
	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordMODEL& record)
	{
		return parseLine(line, size, PDB::FORMAT_MODEL,
										 record.record_name,	&record.model_serial_number);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordMODEL& record)
	{
		current_model_ = (Index)record.model_serial_number;
		return true;
	}


	bool PDBFile::parseRecordSHEET(const char* line, Size size)
	{
		static PDB::RecordSHEET record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSHEET& record)
	{
		return parseLine(line, size, PDB::FORMAT_SHEET,
										 record.record_name, &record.strand_number,
										 record.sheet_ID, &record.number_of_strands,
										 record.initial_residue.name,
										 &record.initial_residue.chain_ID,
										 &record.initial_residue.sequence_number,
										 &record.initial_residue.insertion_code,
										 record.terminal_residue.name,
										 &record.terminal_residue.chain_ID,
										 &record.terminal_residue.sequence_number,
										 &record.terminal_residue.insertion_code,
										 &record.sense_of_strand,
										 record.atom_name_in_current_strand,
										 record.residue_in_current_strand.name,
										 &record.residue_in_current_strand.chain_ID,
										 &record.residue_in_current_strand.sequence_number,
										 &record.residue_in_current_strand.insertion_code,
										 record.atom_name_in_previous_strand,
										 record.residue_in_previous_strand.name,
										 &record.residue_in_previous_strand.chain_ID,
										 &record.residue_in_previous_strand.sequence_number,
										 &record.residue_in_previous_strand.insertion_code);
	}

	bool PDBFile::interpretRecord(const PDB::RecordSHEET& record)
	{
    ResidueQuadruple partner_residue
      (record.initial_residue.name, record.initial_residue.chain_ID,
       record.initial_residue.sequence_number, record.initial_residue.insertion_code);

		// Store the sheet id + ":" + strand_id as name. Example: "BS1:5".
    SecondaryStructure* sec_struc = new SecondaryStructure(String(record.sheet_ID) + ":" + String(record.strand_number));

    new_sheet_secstruc_list_.push_back(sec_struc);
    sec_struc->setType(SecondaryStructure::STRAND);
    sec_struc->setProperty("STRAND_SENSE", (record.sense_of_strand != 0));

    sheet_list_.push_back(partner_residue);

    partner_residue.set(record.terminal_residue.name, record.terminal_residue.chain_ID,
                        record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);

    sheet_list_.push_back(partner_residue);

    return true;
	}

	bool PDBFile::parseRecordSLTBRG(const char* /* line */, Size /* size */)
	{
		return skipCurrentRecord();
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSLTBRG& record)
	{
		return parseLine(line, size, PDB::FORMAT_SLTBRG,
										 record.record_name,
										 record.partner_atom[0].atom_name,
										 &record.partner_atom[0].alternate_location_indicator,
										 record.partner_atom[0].residue.name,
										 &record.partner_atom[0].residue.chain_ID,
										 &record.partner_atom[0].residue.sequence_number,
										 &record.partner_atom[0].residue.insertion_code,
										 record.partner_atom[1].atom_name,
										 &record.partner_atom[1].alternate_location_indicator,
										 record.partner_atom[1].residue.name,
										 &record.partner_atom[1].residue.chain_ID,
										 &record.partner_atom[1].residue.sequence_number,
										 &record.partner_atom[1].residue.insertion_code,
										 &record.first_atom,
										 &record.second_atom);
	}
	
	bool PDBFile::interpretRecord(const PDB::RecordSLTBRG&  /* record */)
	{
		return true;
	}


	bool PDBFile::parseRecordSSBOND(const char* line, Size size)
	{
		static PDB::RecordSSBOND record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordSSBOND& record)
	{
		return parseLine(line, size, PDB::FORMAT_SSBOND,
										 record.record_name,
                    &record.serial_number,
                    record.partner_residue[0].name,
                    &record.partner_residue[0].chain_ID,
                    &record.partner_residue[0].sequence_number,
                    &record.partner_residue[0].insertion_code,
                    record.partner_residue[1].name,
                    &record.partner_residue[1].chain_ID,
                    &record.partner_residue[1].sequence_number,
                    &record.partner_residue[1].insertion_code,
                    &record.symmetry_operator[0],
                    &record.symmetry_operator[1]);
	}

	bool PDBFile::interpretRecord(const PDB::RecordSSBOND& record)
	{
		ResidueQuadruple partner(record.partner_residue[0].name, record.partner_residue[0].chain_ID,
														 record.partner_residue[0].sequence_number, record.partner_residue[0].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		partner.set(record.partner_residue[1].name, record.partner_residue[1].chain_ID,
								record.partner_residue[1].sequence_number, record.partner_residue[1].insertion_code);
		
		ssbond_list_.push_back(partner);
		
		return true;
	}


	bool PDBFile::parseRecordTER(const char* line, Size size)
	{
		if ((selected_model_ != -1) && (selected_model_ != current_model_))
		{
			return true;
		}
		static PDB::RecordTER record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTER& record)
	{
		return parseLine(line, size, PDB::FORMAT_TER, 
										 record.record_name,
										 &record.serial_number,
										 record.residue.name,
										 &record.residue.chain_ID,
										 &record.residue.sequence_number,
										 &record.residue.insertion_code);

	}
	
	bool PDBFile::interpretRecord(const PDB::RecordTER& /* record */)
	{
		chain_ID_ = 0;
		return true;
	}

	bool PDBFile::fillRecord(const char* line, Size size, PDB::RecordTURN& record)
	{
    return parseLine(line, size, PDB::FORMAT_TURN,
                     record.record_name, &record.sequence_number,
                     record.turn_ID, record.initial_residue.name,
                     &record.initial_residue.chain_ID,
                     &record.initial_residue.sequence_number,
                     &record.initial_residue.insertion_code,
                     record.terminal_residue.name,
                     &record.terminal_residue.chain_ID,
                     &record.terminal_residue.sequence_number,
                     &record.terminal_residue.insertion_code,
                     record.comment);
	}

	bool PDBFile::parseRecordTURN(const char* line, Size size)
	{
		static PDB::RecordTURN record;
		return (fillRecord(line, size, record) && interpretRecord(record));
	}
	
	
	bool PDBFile::interpretRecord(const PDB::RecordTURN& record)
	{
		ResidueQuadruple partner_residue(record.initial_residue.name, record.initial_residue.chain_ID,
																		 record.initial_residue.sequence_number, record.initial_residue.insertion_code);
		
		SecondaryStructure* sec_struc = new SecondaryStructure(record.turn_ID);
		
		new_turn_secstruc_list_.push_back(sec_struc);
		sec_struc->setType(SecondaryStructure::TURN);
		
		turn_list_.push_back(partner_residue);
		
		partner_residue.set(record.terminal_residue.name, record.terminal_residue.chain_ID,
												record.terminal_residue.sequence_number, record.terminal_residue.insertion_code);
		
		turn_list_.push_back(partner_residue);
		
		return true;
	}


	bool PDBFile::read(Protein& protein)
	{
		clear();
		protein.destroy();
		current_protein_ = &protein;
		readRecords();
		postprocessSSBonds_();
		postprocessHelices_();
		postprocessSheetsTurns_(sheet_list_, new_sheet_secstruc_list_);
		postprocessSheetsTurns_(turn_list_, new_turn_secstruc_list_);
		postprocessRandomCoils_();
		
		return true;
	}

	bool PDBFile::read(Molecule& molecule)
	{
		Protein* protein = new Protein;
		bool result = read(*protein);
		if (result)
		{
			molecule = *protein;
		}
		delete protein;
		
		return result;
	}

	bool PDBFile::read(System& system)
	{
		Protein* protein = new Protein;
		bool result = read(*protein);
		if (!result)
		{
			delete protein;
		}
		else
		{	
			system.insert(*protein);
		}

		if (!system.hasProperty("PDBINFO")) {
			// create a new storage PDBInfo
			boost::shared_ptr<PersistentObject> pdbi_ptr(new PDBInfo());
			system.setProperty(NamedProperty("PDBINFO",pdbi_ptr));
		}

		boost::shared_ptr<PDBInfo> pdbi;
		pdbi = boost::dynamic_pointer_cast<PDBInfo>(system.getProperty("PDBINFO").getSmartObject());
		*pdbi = info;

		return result;
	}

	Molecule* PDBFile::read()
	{
		if(read_done_)
		{
			return 0;
		}
		Molecule* protein = new Molecule;
		bool result = read(*protein);

		read_done_ = 1;

		if(result)
		{
			return protein;
		}
		else
		{
			delete protein;
			return 0;
		}

	}

	bool PDBFile::write(const System& system)
	{
		PDBInfo pdbi;

		if (system.hasProperty("PDBINFO")) {
			// retrieve the stored PDBInfo record from the system.
			boost::shared_ptr<PDBInfo> pdbi_ptr = boost::dynamic_pointer_cast<PDBInfo>(system.getProperty("PDBINFO").getSmartObject());
			pdbi = *pdbi_ptr;
		}

		return write(system, pdbi);
	}

	bool PDBFile::write(const System& system, const PDBInfo& info)
	{	
		write_pdbformat_1996_ = options.getBool(Option::WRITE_PDBFORMAT_1996);
			
		if (!isOpen() || getOpenMode() != std::ios::out)
		{
			throw (File::CannotWrite(__FILE__, __LINE__, name_));
		}
		if (info.getName() == "")
		{
			// If info doesn't provide a name, but the system/protein does,
			// we'll take it from there.
			String name = system.getName();
			if ((name == "") && (system.beginMolecule() != system.endMolecule()))
			{
				name = system.beginMolecule()->getName();
			}
			PDBInfo new_info(info);
			new_info.setName(name);
			write_(system, new_info);
		}
		else
		{
			write_(system, info);
		}
		
		return true;
	}

	bool PDBFile::isHeteroAtom_(const Atom& atom)
	{
		const Residue* res(atom.getResidue());
		if (res == 0)
		{
			// If the atom is not inside a residue, it cannot be
			// a standard amino acid/nucleotide acid atom
			return true;
		}
		return (!res->hasProperty(Residue::PROPERTY__AMINO_ACID));
	}
	
	void PDBFile::extractStructure_(const AtomContainer& ac, PDB::Structure& structure)
	{
		// Paranoia: remove old crap from structure.
		structure.clear();
		

		// Make sure we have at least one atom:
		if (ac.beginAtom() == ac.endAtom())
		{
			return;
		}
		
		// check if the parent atomcontainer has crystal information
		if (ac.countAtomContainers() != 0)
		{
			if (ac.getAtomContainer(0)->hasProperty("CRYSTALINFO"))
			{
				//CrystalInfo* ci_ptr = dynamic_cast<CrystalInfo*>(ac.getAtomContainer(0)->getProperty("CRYSTALINFO").getObject());
				boost::shared_ptr<CrystalInfo> ci_ptr = boost::dynamic_pointer_cast<CrystalInfo>(ac.getAtomContainer(0)->getProperty("CRYSTALINFO").getSmartObject());
				if (ci_ptr != 0)
				{
					// extract all crystal informations to the structure
					strcpy(structure.unitcell_info.space_group, ci_ptr->getSpaceGroup().c_str());		
					structure.unitcell_info.a = ci_ptr->getCellEdgeLengthA(); 
					structure.unitcell_info.b = ci_ptr->getCellEdgeLengthB(); 
					structure.unitcell_info.c = ci_ptr->getCellEdgeLengthC(); 
					structure.unitcell_info.alpha = ci_ptr->getCellAngleAlpha().toDegree(); 
					structure.unitcell_info.beta = ci_ptr->getCellAngleBeta().toDegree(); 
					structure.unitcell_info.gamma = ci_ptr->getCellAngleGamma().toDegree(); 
					structure.unitcell_info.z_value = ci_ptr->getZScore(); 
					if(ci_ptr->getNumberOfNCSSymOps() != 0)
					{
						for(Position i = 0; i < ci_ptr->getNumberOfNCSSymOps(); i++)
						{
							structure.ncs_matrices.push_back(PDB::Structure::NCSMatrix(ci_ptr->getNCS(i), ci_ptr->isgivenNCS(i)));
						}
					}
				}
			}
		}
		
		// Walk over all atoms and collect all chains, bonds, etc.
		Position index = 0; // index of the current atom in the atoms vector
		for (AtomConstIterator it(ac.beginAtom()); +it; ++it, ++index)
		{
			// Store all atom ptrs in a vector and use indices to refer to them.
			structure.atoms.push_back(PDB::Structure::AtomEntry(*it));

			// Store all bonds
			for (Atom::BondConstIterator bi(it->beginBond()); +bi; ++bi)
			{
				// Make sure we consider each bond just once!
				if ((bi->getFirstAtom() == &*it) && (bi->getFirstAtom() != 0) && (bi->getSecondAtom() != 0))
				{
					switch (bi->getType())
					{
						case Bond::TYPE__COVALENT:
						case Bond::TYPE__UNKNOWN:
						case Bond::TYPE__PEPTIDE:
						case Bond::TYPE__DISULPHIDE_BRIDGE:
							if (isHeteroAtom_(*bi->getFirstAtom()) || isHeteroAtom_(*bi->getSecondAtom()))
							{
								structure.conect_atoms.insert(bi->getFirstAtom());
								structure.conect_atoms.insert(bi->getSecondAtom());
							}
							if ((bi->getFirstAtom()->getElement() == PTE[Element::S])
									&& (bi->getSecondAtom()->getElement() == PTE[Element::S])
									&& (bi->getFirstAtom()->getResidue() != 0) 
									&& (bi->getSecondAtom()->getResidue() != 0)
									&& (bi->getFirstAtom()->getResidue() != bi->getSecondAtom()->getResidue()))
							{
								structure.ssbonds.push_back(&*bi);
								structure.conect_atoms.insert(bi->getFirstAtom());
								structure.conect_atoms.insert(bi->getSecondAtom());
							}
							break;

						case Bond::TYPE__HYDROGEN:
							structure.hydrogen_bonds.push_back(&*bi);
							structure.conect_atoms.insert(bi->getFirstAtom());
							structure.conect_atoms.insert(bi->getSecondAtom());
							break;

						case Bond::TYPE__SALT_BRIDGE:
							structure.saltbridges.push_back(&*bi);
							structure.conect_atoms.insert(bi->getFirstAtom());
							structure.conect_atoms.insert(bi->getSecondAtom());
							break;

						default:
							break;
					}
				}
			}
		}

		structure.chain_residue_names.clear();
		structure.sec_structs.clear();
		const Residue* res = 0;
		const SecondaryStructure* ss = 0;
		for (Position i = 0; i < structure.atoms.size(); ++i)
		{
			// Ignore all hetero residues.
			if ((structure.atoms[i].residue == 0) || (structure.atoms[i].residue->hasProperty(Residue::PROPERTY__NON_STANDARD)))
			{
				continue;
			}

			// If the residue pointer changes, we have reached the end of a residue
			// and the (potential) start of a new one. We remember the residue and its
			// chain ID in a vector to extract its sequence afterwards (SEQRES).
			if (structure.atoms[i].residue != res)
			{
				res = structure.atoms[i].residue;
				if ((res != 0) && (res->getName() != ""))
				{
					char chain_name(structure.atoms[i].chain == 0 ? ' ' : structure.atoms[i].chain->getName()[0]);
					structure.chain_residue_names.push_back(make_pair(chain_name, res->getName()));
				}
			}

			// To a similar thing for secondary structures: whenever a secondary structure
			// ends/starts, remember its indices and push it into an appropriate vector.
			if (structure.atoms[i].sec_struct != ss)
			{
				ss = structure.atoms[i].sec_struct;
				// Is this a new secondary structure?
				if (ss != 0) 
				{
					structure.sec_structs.push_back(ss);
				}
			}
		}
	}

	void PDBFile::write_(const AtomContainer& ac, const PDBInfo& info)
	{
		// Decompose the atom container into chains,
		// hetero groups and AA/NA residues.
		PDB::Structure structure;
		extractStructure_(ac, structure);

		// Reset the bookkeeping entries (counting the number of
		// specific records written for the MASTER record).
		book_keeping_.clear();

		// Write the individual sections of the PDB file one by one.
		writeTitleSection_(structure, info);
		writePrimaryStructureSection_(structure, info);
		writeHeterogenSection_(structure, info);
		writeSecondaryStructureSection_(structure, info);
		writeConnectivityAnnotationSection_(structure, info);
		writeMiscellaneousFeaturesSection_(structure, info);
		writeCrystallographicSection_(structure, info);
		writeCoordinateSection_(structure, info);
		writeConnectivitySection_(structure, info);
		writeBookKeepingSection_(structure, info);
	}
	
	void PDBFile::updateAdditionalAtomInfo_
		(const PDB::Structure::AtomEntry& atom, PDB::AdditionalAtomInfo& aai)
	{
		if (atom.chain != aai.current_chain)
		{
			// Remember we have entered a new chain, so we don't have to 
			// do this every time we write an atom.
			aai.current_chain = atom.chain;

			// If there is a chain, extract its ID, use a default otherwise.
			if (atom.chain != 0)
			{
				aai.chain_id = atom.chain->getName().c_str()[0];
				if (aai.chain_id == '\0')
				{
					aai.chain_id = BALL_CHAIN_DEFAULT_NAME;
				}
			}
		}

		// Now do the same trick for the residue.
		if (atom.residue != aai.current_residue)
		{
			aai.current_residue = atom.residue;
			if (atom.residue != 0)
			{
				strncpy(aai.residue_name, atom.residue->getName().c_str(), 4);
				if (atom.residue->getID().toInt() == 0)
				{
					aai.residue_id++;
				}
				else
				{
					aai.residue_id = atom.residue->getID().toInt();
				}
				aai.residue_insertion_code = atom.residue->getInsertionCode();
			}
			else
			{
				strncpy(aai.residue_name, "UNK", 4);
			}
		}
	}

	void PDBFile::writeAtom_(const PDB::Structure::AtomEntry& atom, PDB::AdditionalAtomInfo& aai, bool hetatm)
	{					
		// get the element symbol
		char element_symbol[3];
		strcpy(element_symbol, atom.atom->getElement().getSymbol().c_str());

		// normalize the atom name:
		//  if the atom name starts with the element name and the element
		//	name is a single character (e.g. C, N, O, H and the name is not
		//  prefixed by a number) then the name should pe prefixed by
		//  a blank to distinguish CA (carbon alpha) from CA (calcium)
		String name(atom.atom->getName());
		name.trim();
		Index offset;
		char PDB_atom_name[5];
		if ((name.size() < 4) && name.hasPrefix(element_symbol) && (strlen(element_symbol) == 1))
		{
			offset = 1;
			PDB_atom_name[0] = ' ';
		} 
		else 
		{
			offset = 0;
		}
		name.get(&(PDB_atom_name[offset]), 0, 4 - offset);
		
		// Update the chain name.
		updateAdditionalAtomInfo_(atom, aai);

		// Define a few defaults for the case that
		// we don't have a PDBAtom, but only an Atom
		char alternate_location = ' ';
		float occupancy = 1.0;
		float temperature_factor = 20.0;

		// Check whether the atom is a PDB atom. If so, retrieve the 
		// additional content.
		const PDBAtom* pdb_atom = dynamic_cast<const PDBAtom*>(atom.atom);
		if (pdb_atom != 0)
		{
			alternate_location = pdb_atom->getAlternateLocationIndicator();
			occupancy = pdb_atom->getOccupancy();
			temperature_factor = pdb_atom->getTemperatureFactor();
		}
	
		// Formal charge ????
		char formal_charge[3] = "  ";
							
		// Write the record
		writeRecord_((hetatm ? PDB::RECORD_TYPE__HETATM : PDB::RECORD_TYPE__ATOM),
								 aai.number++, PDB_atom_name,
								 alternate_location, aai.residue_name,
								 aai.chain_id, aai.residue_id, aai.residue_insertion_code,
								 atom.atom->getPosition().x,
								 atom.atom->getPosition().y,
								 atom.atom->getPosition().z,
								 occupancy,	temperature_factor,
								 aai.pdb_id, element_symbol, formal_charge); 
	}
	
	void PDBFile::writeCoordinateSection_
		(const PDB::Structure& structure, const PDBInfo& /* info */)
	{
		// --- MODEL ---
		// We write single model files only at this time!
		// writeRecord_(PDB::RECORD_TYPE__MODEL, 1L);

		// --- ATOM/HETATM/TER ---
		PDB::AdditionalAtomInfo atom_info;
		if (!structure.atoms.empty())
		{
			atom_info.current_residue = structure.atoms[0].residue;
			atom_info.current_chain = structure.atoms[0].chain;
			if (atom_info.current_residue != 0)
			{
				strncpy(atom_info.residue_name, atom_info.current_residue->getName().c_str(), 4);
				try
				{
					atom_info.residue_id = atom_info.current_residue->getID().toInt();
				}
				catch(...)
				{
				}
				atom_info.residue_insertion_code = atom_info.current_residue->getInsertionCode();
			}
			if ((atom_info.current_chain != 0) && (atom_info.current_chain->getName() != ""))
			{
				atom_info.chain_id = atom_info.current_chain->getName()[0];
			}
		}
		atom_map_.clear(); // here, we store the indices of the atoms. This is required to
  										 // resolve connectivities later on.
		for (Position index = 0; index < structure.atoms.size(); ++index)
		{
			// --- ATOM/HETATM ---
			atom_map_[structure.atoms[index].atom] = atom_info.number;
			writeAtom_(structure.atoms[index], atom_info, isHeteroAtom_(*structure.atoms[index].atom));
	
			// --- TER ---
			// Is this the last atom of the structure or the last atom of a chain?
			if ((index == structure.atoms.size() - 1) || (structure.atoms[index + 1].chain != structure.atoms[index].chain))
			{
				writeRecord_(PDB::RECORD_TYPE__TER,
										 atom_info.number++, atom_info.residue_name,
										 atom_info.chain_id, atom_info.residue_id,
										 atom_info.residue_insertion_code);
			}
		}

		// --- ENDMDL ---
		// writeRecord_(PDB::RECORD_TYPE__ENDMDL);	
  }

	bool atomPairSortPred(const std::pair<Position, const Atom*>& left, const std::pair<Position, const Atom*>& right)
	{
		return left.first < right.first;
	}

	void PDBFile::writeConnectivitySection_
		(const PDB::Structure& structure, const PDBInfo& /* info */)
	{
		// --- CONECT ---
		// Extract the atoms w/ relevant bonds from structure and sort them by their index.
		std::vector<std::pair<Index, const Atom*> > relevant_atoms;
		for (HashSet<const Atom*>::ConstIterator it = structure.conect_atoms.begin();
				 +it; ++it)
		{
			relevant_atoms.push_back(std::make_pair(atom_map_[*it], *it));
		}
		std::sort(relevant_atoms.begin(), relevant_atoms.end(), atomPairSortPred);;

		// Walk over all relevant atoms and create the CONECT records.
		PDB::Structure::ConectAtomList cl;
		for (Position i = 0; i < relevant_atoms.size(); ++i)
		{
			cl.serial_number = relevant_atoms[i].first;
			const Atom& atom = *relevant_atoms[i].second;
			for (Atom::BondConstIterator it = atom.beginBond(); +it; ++it)
			{
				const Atom* atom2 = it->getPartner(atom);
				if (atom_map_.has(atom2))
				{
					Position pos = atom_map_[atom2];
					switch (it->getType())
					{
						case Bond::TYPE__SALT_BRIDGE:
							cl.saltbridges.push_back(pos);
							break;

						case Bond::TYPE__HYDROGEN:
							cl.hbonds.push_back(pos);
							break;

						case Bond::TYPE__DISULPHIDE_BRIDGE:
						case Bond::TYPE__COVALENT:
						case Bond::TYPE__PEPTIDE:
						case Bond::TYPE__UNKNOWN:
							cl.bonds.push_back(pos);
							break;

						default:
							break;
					}
				}
			}
			writeCONECTRecords_(cl);
		}
	}
	

	void PDBFile::writeTitleSection_
		(const PDB::Structure& /* structure */, const PDBInfo& info)
	{
		// --- HEADER ---
		// Retrieve the current date...
		time_t current_time = time(NULL);
		char* date = asctime(localtime(&current_time));
		char PDB_date_format[10];
		
		sprintf(PDB_date_format, "%2.2s-%c%c%c-%2.2s",
						date + 8, toupper(date[4]), toupper(date[5]),
						toupper(date[6]), date + 22);
		writeRecord_(PDB::RECORD_TYPE__HEADER, info.getName().c_str(),
								 PDB_date_format, info.getID().c_str());

		// --- OBSLTE ---
		addAllRecords_(info, PDB::RECORD_TYPE__OBSLTE);
		// --- TITLE  ---
		addAllRecords_(info, PDB::RECORD_TYPE__TITLE);
		// --- CAVEAT ---
		addAllRecords_(info, PDB::RECORD_TYPE__CAVEAT);
		// --- COMPND ---
		addAllRecords_(info, PDB::RECORD_TYPE__COMPND);
		// --- SOURCE ---
		addAllRecords_(info, PDB::RECORD_TYPE__SOURCE);
		// --- KEYWDS ---
		addAllRecords_(info, PDB::RECORD_TYPE__KEYWDS);
		// --- EXPDTA ---
		addAllRecords_(info, PDB::RECORD_TYPE__EXPDTA);
		// --- AUTHOR ---
		addAllRecords_(info, PDB::RECORD_TYPE__AUTHOR);
		// --- REVDAT ---
		addAllRecords_(info, PDB::RECORD_TYPE__REVDAT);
		// --- SPRSDE ---
		addAllRecords_(info, PDB::RECORD_TYPE__SPRSDE);	
		// --- JRNL   ---
		addAllRecords_(info, PDB::RECORD_TYPE__JRNL);
		// --- REMARK ---
		addAllRecords_(info, PDB::RECORD_TYPE__REMARK);
	}

	void PDBFile::writeSEQRESSection_(const std::vector<std::pair<char, String> >& chain_residues)
	{
		// Iterate over all residues and count the residues per chain.
		std::vector<Size> chain_size;
		char current_chain = '\1';
		for (Position i = 0; i < chain_residues.size(); ++i)
		{
			if (chain_residues[i].first != current_chain)
			{
				chain_size.push_back(1);
			}
			else
			{
				if (!chain_size.empty())
				{
					chain_size.back()++;
				}
			}
			current_chain = chain_residues[i].first;
		}

		// Iterate over all residues and assemble them to SEQRES records (13 per line).
		Position ser_num = 0;			 // The line counter. Reset to 1 for each new chain.
		current_chain = '\0'; // The zeroth chain gets an invalid id.
		PDB::RecordSEQRES sr;
		Index chain_counter = -1;
		Position res_counter = 0;  // number of residues in sr;
		sr.serial_number = 1;
		if (chain_size.size() > 0)
		{
			sr.number_of_residues_in_chain = chain_size[0];
		}
		else
		{
			sr.number_of_residues_in_chain = 0;
		}

		for (Position i = 0; i < chain_residues.size(); ++i)
		{
			// Just some shorthands...
			const char& chain_id = chain_residues[i].first;
			const String& res_name = chain_residues[i].second;
			
			// Is this a new chain or is the line full?
			if ((chain_id != current_chain) || (res_counter == 13))
			{
				// Write the old stuff if this is not the first record to be written.
				if (ser_num > 0)
				{
					 writeRecord_(sr);
				}
				sr.clear();

				if (res_counter == 13)
				{
					// Line full.
					sr.serial_number = ++ser_num;
				}

				if((res_counter != 13) || (chain_id != current_chain))
				{
					// New chain.
					chain_counter++;
					sr.serial_number = 1;
					ser_num = 1;
				}
				res_counter = 0;
				if (chain_size.size() > static_cast<Size>(chain_counter))
				{
					sr.number_of_residues_in_chain = chain_size[chain_counter];
				}
				else
				{
					sr.number_of_residues_in_chain = 0;
				}
				sr.chain_ID = chain_id;
			}
			current_chain = chain_id;
			
			// Replace the residue names for nucleotides:
			PDB::ResidueName seq;
			seq[0] = '\0';
			const char* original_name[] = {"DA", "DC", "DG", "DT", "DU", "DI"};
			const char* seqres_name[]   = { "A",  "C",  "G",  "T",  "U",  "I"};
			for (Position i = 0; i < 6; ++i)
			{
				if (res_name == original_name[i])
				{
					strncpy(seq, seqres_name[i], 3);
					break;
				}
			}
			if (seq[0] == '\0')
			{
				strncpy(seq, res_name.c_str(), 3);
			}
			strncpy(sr.residue_name[res_counter++], seq, 3);
		}
		if (res_counter > 0)
		{
			writeRecord_(sr);
		}
	}

	void PDBFile::writeHELIXSection_(const PDB::Structure& structure)
	{
		PDB::RecordHELIX hr;
		for (Position i = 0; i < structure.sec_structs.size(); ++i)
		{
			if (structure.sec_structs[i]->getType() == SecondaryStructure::HELIX)
			{
				const SecondaryStructure& helix(*structure.sec_structs[i]);
				hr.serial_number++;
				strncpy(hr.helix_ID, helix.getName().c_str(), 3);
				hr.initial_residue.set(*helix.beginResidue());
				hr.terminal_residue.set(*helix.rbeginResidue());
				hr.comment[0] = '\0';
				hr.length = hr.terminal_residue.sequence_number - hr.initial_residue.sequence_number + 1;
				hr.helix_class = 1; // default: \alpha-helix
				if (helix.hasProperty("HELIX_CLASS"))
				{
					hr.helix_class = helix.getProperty("HELIX_CLASS").getInt();
				}

				writeRecord_(hr);
			}
		}
	}
  void PDBFile::writeSHEETSection_(const PDB::Structure& structure)
	{
		// First, count the number of strands. Not exactly what 
		// we need, but it'll do for now. We would have to count
		// the strands per sheet for every distinct sheet ID and
		// account for barrels (closed sheets: last strand = first strand)
		// as well. / OK ????
		PDB::RecordSHEET sr;
		for (Position i = 0; i < structure.sec_structs.size(); ++i)
		{
			if (structure.sec_structs[i]->getType() == SecondaryStructure::STRAND)
			{
				sr.number_of_strands++;
			}
		}

		for (Position i = 0; i < structure.sec_structs.size(); ++i)
		{
			if (structure.sec_structs[i]->getType() == SecondaryStructure::STRAND)
			{
				const SecondaryStructure& strand(*structure.sec_structs[i]);
				const String& name = strand.getName();
				if (name.has(':'))
				{
					String sheet_id = name.before(":");
					strncpy(sr.sheet_ID, sheet_id.c_str(), 3);
					sr.strand_number = name.after(":").toString().toInt();
				}
				else	
				{
					strncpy(sr.sheet_ID, name.c_str(), 3);
					sr.strand_number = 1;
				}
				sr.initial_residue.set(*strand.beginResidue());
				sr.terminal_residue.set(*strand.rbeginResidue());

				writeRecord_(sr);
			}
		}
	}

	void PDBFile::writeTURNSection_(const PDB::Structure& structure)
	{
		PDB::RecordTURN tr;
		for (Position i = 0; i < structure.sec_structs.size(); ++i)
		{
			if (structure.sec_structs[i]->getType() == SecondaryStructure::TURN)
			{
				const SecondaryStructure& turn(*structure.sec_structs[i]);
				tr.sequence_number++;
				strncpy(tr.turn_ID, turn.getName().c_str(), 3);
				tr.initial_residue.set(*turn.beginResidue());
				tr.terminal_residue.set(*turn.rbeginResidue());
				tr.comment[0] = '\0';

				writeRecord_(tr);
			}
		}
	}

  void PDBFile::writeSSBONDSection_(const PDB::Structure& structure)
	{
		PDB::RecordSSBOND sr;
		for (Position i = 0; i < structure.ssbonds.size(); ++i)
		{
			const Bond* bond = structure.ssbonds[i];
			sr.serial_number = i + 1;
			if (bond->getFirstAtom()->getResidue()->getID().toInt() > bond->getSecondAtom()->getResidue()->getID().toInt())
			{
				sr.partner_residue[1].set(*bond->getFirstAtom()->getResidue());
				sr.partner_residue[0].set(*bond->getSecondAtom()->getResidue());	
			}
			else	
			{
				sr.partner_residue[0].set(*bond->getFirstAtom()->getResidue());
				sr.partner_residue[1].set(*bond->getSecondAtom()->getResidue());	
			}

			writeRecord_(sr);
		}
	}
  void PDBFile::writeHYDBNDSection_(const PDB::Structure& /* structure */)
	{
		// ????
	}
  void PDBFile::writeSLTBRGSection_(const PDB::Structure& /* structure */)
	{
		// ????
	}

  void PDBFile::writeCRYST1Section_(const PDB::Structure& structure)
  {
		if (structure.unitcell_info.z_value != -1)
		{
			PDB::RecordCRYST1 cr;
			cr.unit_cell.a = structure.unitcell_info.a;
			cr.unit_cell.b = structure.unitcell_info.b;
			cr.unit_cell.c = structure.unitcell_info.c;
			cr.unit_cell.alpha = structure.unitcell_info.alpha;
			cr.unit_cell.beta = structure.unitcell_info.beta;
			cr.unit_cell.gamma = structure.unitcell_info.gamma;
			strcpy(cr.unit_cell.space_group, structure.unitcell_info.space_group);
			cr.unit_cell.z_value = structure.unitcell_info.z_value;

			writeRecord_(cr);
		}
	}
  
	void PDBFile::writeMTRIXnSection_(const PDB::Structure& structure)
	{
		if (structure.ncs_matrices.size() != 0)
		{
			for (Position i = 0; i < structure.ncs_matrices.size(); i++)
			{
				// write all MTRIXn records at once, as they only make sense when they are complete
				PDB::RecordMTRIX1 mr1;
				PDB::RecordMTRIX2 mr2;
				PDB::RecordMTRIX3 mr3;
				mr1.serial_number = i+1;
				mr2.serial_number = i+1;
				mr3.serial_number = i+1;
				mr1.transformation_matrix[0] = structure.ncs_matrices[i].matrix[0]; 
				mr1.transformation_matrix[1] = structure.ncs_matrices[i].matrix[1]; 
				mr1.transformation_matrix[2] = structure.ncs_matrices[i].matrix[2]; 
				mr1.transformation_matrix[3] = structure.ncs_matrices[i].matrix[3]; 
				mr2.transformation_matrix[0] = structure.ncs_matrices[i].matrix[4]; 
				mr2.transformation_matrix[1] = structure.ncs_matrices[i].matrix[5]; 
				mr2.transformation_matrix[2] = structure.ncs_matrices[i].matrix[6]; 
				mr2.transformation_matrix[3] = structure.ncs_matrices[i].matrix[7]; 
				mr3.transformation_matrix[0] = structure.ncs_matrices[i].matrix[8]; 
				mr3.transformation_matrix[1] = structure.ncs_matrices[i].matrix[9]; 
				mr3.transformation_matrix[2] = structure.ncs_matrices[i].matrix[10]; 
				mr3.transformation_matrix[3] = structure.ncs_matrices[i].matrix[11]; 
				mr1.is_given = structure.ncs_matrices[i].isgiven;
				mr2.is_given = structure.ncs_matrices[i].isgiven;
				mr3.is_given = structure.ncs_matrices[i].isgiven;
				writeRecord_(mr1);
				writeRecord_(mr2);
				writeRecord_(mr3);
			}
		}
  }

	void PDBFile::writeConnectivityAnnotationSection_
		(const PDB::Structure& structure, const BALL::PDBInfo& info)
	{
		// --- SSBOND ---
		writeSSBONDSection_(structure);
			
		// --- LINK   ---
		// Add LINK records read from the fie before
		addAllRecords_(info, PDB::RECORD_TYPE__LINK);

		// --- HYDBND ---
		writeHYDBNDSection_(structure);

		// --- SLTBRG ---
		writeSLTBRGSection_(structure);
		
		// --- CISPEP ---
		// Add CISPEP records skipped when reading the file.
		addAllRecords_(info, PDB::RECORD_TYPE__CISPEP);
	}

	void PDBFile::writeSecondaryStructureSection_
		(const PDB::Structure& structure, const BALL::PDBInfo& /* info */)
	{
		// --- HELIX ---
		writeHELIXSection_(structure);
		// --- SHEET ---
		writeSHEETSection_(structure);
		// --- TURN ---
		writeTURNSection_(structure);
	}

	void BALL::PDBFile::writeCrystallographicSection_
		(const PDB::Structure& structure, const BALL::PDBInfo& info)
	{
		// --- CRYST1 ---
		writeCRYST1Section_(structure);
		// --- ORIGX1 ---
		addAllRecords_(info, PDB::RECORD_TYPE__ORIGX1);
		// --- ORIGX2 ---
		addAllRecords_(info, PDB::RECORD_TYPE__ORIGX2);
		// --- ORIGX3 ---
		addAllRecords_(info, PDB::RECORD_TYPE__ORIGX3);
		// --- SCALE1 ---
		addAllRecords_(info, PDB::RECORD_TYPE__SCALE1);
		// --- SCALE2 ---
		addAllRecords_(info, PDB::RECORD_TYPE__SCALE2);
		// --- SCALE3 ---
		addAllRecords_(info, PDB::RECORD_TYPE__SCALE3);
		// --- MTRIX1, MTRIX2, MTRIX3 ---
		writeMTRIXnSection_(structure);
		// --- TVECT ---
		addAllRecords_(info, PDB::RECORD_TYPE__TVECT);
	}

	void BALL::PDBFile::writePrimaryStructureSection_
		(const PDB::Structure& structure, const BALL::PDBInfo& info)
	{
		// --- DBREF  ---
		addAllRecords_(info, PDB::RECORD_TYPE__DBREF);
		// --- SEQADV ---
		addAllRecords_(info, PDB::RECORD_TYPE__SEQADV);		
		// --- SEQRES ---
		writeSEQRESSection_(structure.chain_residue_names);
		// --- MODRES ---
		addAllRecords_(info, PDB::RECORD_TYPE__MODRES);
	}
	void PDBFile::writeMiscellaneousFeaturesSection_
		(const PDB::Structure& /* structure */, const BALL::PDBInfo& info)
	{
		// --- SITE ---
		addAllRecords_(info, PDB::RECORD_TYPE__SITE);
	}

	void PDBFile::writeHeterogenSection_
		(const PDB::Structure& /* structure */, const BALL::PDBInfo& info)
	{
		// --- HET    ---
		addAllRecords_(info, PDB::RECORD_TYPE__HET);
		// --- HETNAM ---
		addAllRecords_(info, PDB::RECORD_TYPE__HETNAM);
		// --- HETSYN ---
		addAllRecords_(info, PDB::RECORD_TYPE__HETSYN);
		// --- FORMUL ---
		addAllRecords_(info, PDB::RECORD_TYPE__FORMUL);
	}

	void PDBFile::writeBookKeepingSection_
		(const PDB::Structure& /* structure */, const PDBInfo& /* info */)
	{
		// ---- MASTER ----
		// Write bookkeeping information about the number of records of each type.
		writeRecord_(PDB::RECORD_TYPE__MASTER,
								 book_keeping_.remark_records, 0L,
								 book_keeping_.het_records, book_keeping_.helix_records,
								 book_keeping_.sheet_records, book_keeping_.turn_records,
								 book_keeping_.site_records,	book_keeping_.coordinate_transformation_records,
								 book_keeping_.atomic_coordinate_records, book_keeping_.ter_records,
								 book_keeping_.conect_records,	book_keeping_.seqres_records);

		// ---- END ----
		writeRecord_(PDB::RECORD_TYPE__END);
	}
	

	void PDBFile::writeRecord_(const PDB::RecordSEQRES& seqres)
	{
		writeRecord_(PDB::RECORD_TYPE__SEQRES, 
								 seqres.serial_number, seqres.chain_ID,
								 seqres.number_of_residues_in_chain,
								 seqres.residue_name[0], seqres.residue_name[1], seqres.residue_name[2],
								 seqres.residue_name[3], seqres.residue_name[4], seqres.residue_name[5],
								 seqres.residue_name[6], seqres.residue_name[7], seqres.residue_name[8],
								 seqres.residue_name[9], seqres.residue_name[10], seqres.residue_name[11],
								 seqres.residue_name[12]);
	}

	void PDBFile::writeRecord_(const PDB::RecordHELIX& helix)
	{
		writeRecord_(PDB::RECORD_TYPE__HELIX, 
								 helix.serial_number, helix.helix_ID,
								 helix.initial_residue.name, helix.initial_residue.chain_ID,
								 helix.initial_residue.sequence_number, helix.initial_residue.insertion_code,
								 helix.terminal_residue.name, helix.terminal_residue.chain_ID,
								 helix.terminal_residue.sequence_number, helix.terminal_residue.insertion_code,
								 helix.helix_class, helix.comment, helix.length);
	}

	void PDBFile::writeRecord_(const PDB::RecordSHEET& sheet)
	{
		writeRecord_(PDB::RECORD_TYPE__SHEET, 
								 sheet.strand_number, sheet.sheet_ID, sheet.number_of_strands,
								 sheet.initial_residue.name, sheet.initial_residue.chain_ID,
								 sheet.initial_residue.sequence_number, sheet.initial_residue.insertion_code,
								 sheet.terminal_residue.name, sheet.terminal_residue.chain_ID,
								 sheet.terminal_residue.sequence_number, sheet.terminal_residue.insertion_code,
								 sheet.sense_of_strand, 
								 sheet.atom_name_in_current_strand, 
								 sheet.residue_in_current_strand.name, sheet.residue_in_current_strand.chain_ID,
								 sheet.residue_in_current_strand.sequence_number, sheet.residue_in_current_strand.insertion_code,
								 sheet.atom_name_in_previous_strand, 
								 sheet.residue_in_previous_strand.name, sheet.residue_in_previous_strand.chain_ID,
								 sheet.residue_in_previous_strand.sequence_number, sheet.residue_in_previous_strand.insertion_code);
	}

	void PDBFile::writeRecord_(const PDB::RecordTURN& turn)
	{
		writeRecord_(PDB::RECORD_TYPE__HELIX, 
								 turn.sequence_number, turn.turn_ID,
								 turn.initial_residue.name, turn.initial_residue.chain_ID,
								 turn.initial_residue.sequence_number, turn.initial_residue.insertion_code,
								 turn.terminal_residue.name, turn.terminal_residue.chain_ID,
								 turn.terminal_residue.sequence_number, turn.terminal_residue.insertion_code,
								 turn.comment);
	}


	void PDBFile::writeRecord_(const PDB::RecordSSBOND& ssb)
	{
		writeRecord_(PDB::RECORD_TYPE__SSBOND, 
								 ssb.serial_number, 
								 ssb.partner_residue[0].name, ssb.partner_residue[0].chain_ID,
								 ssb.partner_residue[0].sequence_number, ssb.partner_residue[0].insertion_code,
								 ssb.partner_residue[1].name, ssb.partner_residue[1].chain_ID,
								 ssb.partner_residue[1].sequence_number, ssb.partner_residue[1].insertion_code,
								 ssb.symmetry_operator[0], ssb.symmetry_operator[1]);
	}

	void PDBFile::writeCONECTRecords_(PDB::Structure::ConectAtomList& cl)
	{
		if (write_pdbformat_1996_)
		{
			cl.saltbridges.sort();
			cl.bonds.sort();
			cl.hbonds.sort();

			while (!cl.bonds.empty()|| !cl.saltbridges.empty() || !cl.hbonds.empty() || !cl.bonds.empty())
			{
				PDB::RecordCONECT cr;
				cr.clear();
				cr.atom_serial_number = cl.serial_number;
				Size i = 0;
				while (!cl.bonds.empty() && (i < 4))
				{
				cr.bond_atom[i++] = cl.bonds.front();
				cl.bonds.pop_front();
				}
				i = 0;

				while (!cl.hbonds.empty() && (i < 4))
				{
					cr.hbond_atom[i++] = cl.hbonds.front();
					cl.hbonds.pop_front();
				}
				i = 0;
				while (!cl.saltbridges.empty() && (i < 2))
				{
					cr.salt_bridge_atom[i++] = cl.saltbridges.front();
					cl.saltbridges.pop_front();
				}
			
				writeRecord_(cr);
			}
		}
		else 
		{
			cl.bonds.sort();
			while (!cl.bonds.empty())
			{
				PDB::RecordCONECT cr;
				cr.clear();
				cr.atom_serial_number = cl.serial_number;
				Size i = 0;
				while (!cl.bonds.empty() && (i < 4))
				{
					cr.bond_atom[i++] = cl.bonds.front();
					cl.bonds.pop_front();
				}
				writeRecord_(cr);
			}
		}
	}

	void PDBFile::writeRecord_(const PDB::RecordCONECT& cr)
	{
		// ???? We should not write undefined atom numbers as 0 but as "" instead! // OK
		if (write_pdbformat_1996_)
		{
			writeRecord_(PDB::RECORD_TYPE__CONECT,
			             cr.atom_serial_number,
			             cr.bond_atom[0], cr.bond_atom[1],
			             cr.bond_atom[2], cr.bond_atom[3],
			             cr.hbond_atom[0], cr.hbond_atom[1],
			             cr.salt_bridge_atom[0],
			             cr.hbond_atom[2], cr.hbond_atom[3],
			             cr.salt_bridge_atom[1]);
		}
		else
		{
			if (cr.bond_atom[0]==0)
			{
				writeRecord_(PDB::RECORD_TYPE__CON061,
				             cr.atom_serial_number);
			}
			else
			{
				if (cr.bond_atom[1]==0)
				{
					writeRecord_(PDB::RECORD_TYPE__CON062,
					             cr.atom_serial_number,
					             cr.bond_atom[0]);
				}
				else
				{
					if (cr.bond_atom[2]==0)
					{
						writeRecord_(PDB::RECORD_TYPE__CON063,
						             cr.atom_serial_number,
						             cr.bond_atom[0], 
					                     cr.bond_atom[1]);
					}
					else
					{
						if (cr.bond_atom[3]==0)
						{
							writeRecord_(PDB::RECORD_TYPE__CON064,
							             cr.atom_serial_number,
							             cr.bond_atom[0], 
							             cr.bond_atom[1],
							             cr.bond_atom[2]);
						}
						else
						{
							writeRecord_(PDB::RECORD_TYPE__CON06,
							             cr.atom_serial_number,
							             cr.bond_atom[0], cr.bond_atom[1],
							             cr.bond_atom[2], cr.bond_atom[3]);
						}					
					}
				}
			}

		}

	}

	void PDBFile::writeRecord_(const PDB::RecordCRYST1& cr)
	{
		writeRecord_(PDB::RECORD_TYPE__CRYST1,
								 cr.unit_cell.a,
								 cr.unit_cell.b,
								 cr.unit_cell.c,
								 cr.unit_cell.alpha,
								 cr.unit_cell.beta,
								 cr.unit_cell.gamma,
								 cr.unit_cell.space_group,
								 cr.unit_cell.z_value);
	}
	
	void PDBFile::writeRecord_(const PDB::RecordMTRIX1& mr1)
	{
		writeRecord_(PDB::RECORD_TYPE__MTRIX1,
								 mr1.serial_number,
								 mr1.transformation_matrix[0],
								 mr1.transformation_matrix[1],
								 mr1.transformation_matrix[2],
								 mr1.transformation_matrix[3],
								 mr1.is_given);
	}
	
	void PDBFile::writeRecord_(const PDB::RecordMTRIX2& mr2)
	{
		writeRecord_(PDB::RECORD_TYPE__MTRIX2,
								 mr2.serial_number,
								 mr2.transformation_matrix[0],
								 mr2.transformation_matrix[1],
								 mr2.transformation_matrix[2],
								 mr2.transformation_matrix[3],
								 mr2.is_given);
	}
	
	void PDBFile::writeRecord_(const PDB::RecordMTRIX3& mr3)
	{
		writeRecord_(PDB::RECORD_TYPE__MTRIX3,
								 mr3.serial_number,
								 mr3.transformation_matrix[0],
								 mr3.transformation_matrix[1],
								 mr3.transformation_matrix[2],
								 mr3.transformation_matrix[3],
								 mr3.is_given);
	}
} // namespace BALL
