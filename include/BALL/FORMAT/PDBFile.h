// $Id: PDBFile.h,v 1.12 2001/02/27 01:58:47 amoll Exp $

#ifndef BALL_FORMAT_PDBFILE_H
#define BALL_FORMAT_PDBFILE_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#include <list>

#ifndef BALL_DATATYPE_HASHMAP_H
#	include <BALL/DATATYPE/hashMap.h>
#endif

#ifndef BALL_DATATYPE_QUADRUPLE_H
#	include <BALL/DATATYPE/quadruple.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#ifndef BALL_FORMAT_GENERICPDBFILE_H
#	include <BALL/FORMAT/genericPDBFile.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_KERNEL_PROTEIN_H
#	include <BALL/KERNEL/protein.h>
#endif

namespace BALL 
{

	inline
	Index Hash(const Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar>& quadruple)
	{
		return (Index)quadruple.third;
	}

	inline
	std::ostream& operator << (std::ostream& s, const Quadruple<String,PDB::Character,PDB::Integer,PDB::AChar>& /* q */)
	{
		return s;
	}

	
	/**	PDB file class
			{\bf Definition:} \URL{BALL/FORMAT/PDBFile.h} \\
	*/
	class PDBFile
		: public GenericPDBFile
	{
		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Default Constructor
		PDBFile();

		/** Copy Constructor.
				Create a new PDBFile object refering to the same
				filename (if already set).
		*/
		PDBFile(const PDBFile& PDB_file);

		/** Detailled constructor.
				The file named {\tt filename} is opened. By default, the file is
				opened for reading. To open it for writing, use {\tt open_mode = File::OUT}.
				@param	filename the name of the file to open
				@param	open_mode the file mode
		*/
		PDBFile(const String& filename, File::OpenMode open_mode = File::IN);

		/// Destructor
		virtual ~PDBFile()
			throw();

		//@}

		virtual bool readUnknownRecord(const char* line);

		virtual bool readInvalidRecord(const char* line);

		virtual bool readRecordATOM
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
			 PDB::LString4 segment_ID,
			 PDB::LString2 element_symbol,
			 PDB::LString2 charge);
	
		virtual bool readRecordCONECT
			(PDB::Integer atom_serial_number,
			 PDB::Integer bonded_atom_serial_number[4],
			 PDB::Integer hydrogen_bonded_atom_serial_number[4],
			 PDB::Integer salt_bridged_atom_serial_number[2]);

		virtual bool readRecordEND();
	
		virtual bool readRecordENDMDL();
	
		virtual bool readRecordHELIX
			(PDB::Integer serial_number,
			 PDB::LString3 helix_ID,
			 PDB::RecordHELIX::InitialResidue& initial_residue,
			 PDB::RecordHELIX::TerminalResidue& terminal_residue,
			 PDB::Integer helix_class,
			 PDB::PDBString comment,
			 PDB::Integer length);
	
		virtual bool readRecordHEADER
			(PDB::String40 classification,
			 PDB::Date deposition_date,
			 PDB::IDcode ID_code);
	
		virtual bool readRecordHETATM
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
			 PDB::LString2 charge);
	
		virtual bool readRecordMODEL
			(PDB::Integer model_serial_number);

		virtual bool readRecordSHEET
			(PDB::Integer strand_number,
			 PDB::LString3 sheet_ID,
			 PDB::Integer number_of_strands,
			 PDB::RecordSHEET::InitialResidue& initial_residue,
			 PDB::RecordSHEET::TerminalResidue& terminal_residue,
			 PDB::Integer sense_of_strand,
			 PDB::Atom atom_name_in_current_strand,
			 PDB::RecordSHEET::ResidueInCurrentStrand& residue_in_current_strand,
			 PDB::Atom atom_name_in_previous_strand,
			 PDB::RecordSHEET::ResidueInPreviousStrand& residue_in_previous_strand);

		virtual bool readRecordSSBOND
			(PDB::Integer serial_number,
			 PDB::RecordSSBOND::PartnerResidue partner_residue[2]);
	
		virtual bool readRecordTER
			(PDB::Integer serial_number,
			 PDB::ResidueName residue_name,
			 PDB::Character chain_ID,
			 PDB::Integer residue_sequence_number,
			 PDB::AChar insertion_code);

		virtual bool readRecordTURN
			(PDB::Integer sequence_number,
			 PDB::LString3 turn_ID,
			 PDB::RecordTURN::InitialResidue& initial_residue,
			 PDB::RecordTURN::TerminalResidue& terminal_residue,
			 PDB::PDBString comment);

		/**	@name	Reading and Writing Files 
		*/
		//@{

		/**	Read a system from the PDBfile
		*/
		virtual void read(System& system);

		/**	Write a system to the PDBfile
		*/
		virtual void write(const System& system);

		/**	Read a system from the PDBfile
		*/
		PDBFile& operator >> (System& system);

		/**	Write a system to the PDB file
		*/
		PDBFile& operator << (const System& system);


		/**	Write a protein to the PDBfile
		*/
    virtual void write(const Protein& protein);

		/**	Read a protein from the PDBfile
		*/
		virtual void read(Protein& protein);

		/**	Read a protein from the PDBfile
		*/
		PDBFile& operator >> (Protein& protein);

		/**	Write a protein to the PDBfile
		*/
    PDBFile& operator << (const Protein& protein);


		virtual void write(const Molecule& molecule);

		PDBFile& operator << (const Molecule& molecule);
  
		//@}

		protected:
		
		void clear_();


		private:

		PDBFile& operator = (const PDBFile& PDB_file);

		void write_(const Composite& composite, bool system = false);

		void postprocessSSBonds_();

		void postprocessHelices_();

		void postprocessSheetsTurns_
			(std::list<Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar> >& secstruc_list,
			 std::list<SecondaryStructure *>& new_secstruc_list);

		void postprocessRandomCoils_();

		typedef HashMap<PDB::Integer,PDBAtom *>																										PDBAtomMap;
		typedef	Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar>								ResidueQuadruple;
		typedef HashMap<ResidueQuadruple, Residue *>																							ResidueMap;
		typedef std::list<Quadruple<String, PDB::Character, PDB::Integer, PDB::AChar> >		QuadrupleList;
		typedef std::list<SecondaryStructure *>																										SecStrucList;

		PDBAtomMap			PDB_atom_map_;
		ResidueMap			residue_map_;

		QuadrupleList		ssbond_list_;
		QuadrupleList		helix_list_;
		QuadrupleList		sheet_list_;
		QuadrupleList		turn_list_;

		SecStrucList		new_helix_secstruc_list_;
		SecStrucList		new_sheet_secstruc_list_;
		SecStrucList		new_turn_secstruc_list_;

		Protein*				current_protein_;
		Chain*					current_chain_;
		Residue*				current_residue_;
		PDBAtom*				current_PDB_atom_;
		String					residue_name_;

		PDB::Character	chain_ID_;
		PDB::Integer		residue_sequence_number_;
		PDB::AChar			insertion_code_;
	};

#	ifndef BALL_NO_INLINE_FUNCTIONS
#		include <BALL/FORMAT/PDBFile.iC>
#	endif

} // namespace BALL

#endif // BALL_FORMAT_PDBFILE_H
