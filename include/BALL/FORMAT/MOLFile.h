// $Id: MOLFile.h,v 1.1 2001/12/17 01:43:08 oliver Exp $

#ifndef BALL_FORMAT_MOLFILE_H
#define BALL_FORMAT_MOLFILE_H

#ifndef BALL_FORMAT_GENERICMOLFILE_H
#	include <BALL/FORMAT/genericMolFile.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

namespace BALL 
{
	class System;
	class Atom;
	class Molecule;

	/**	MDL MOL file class.
			This class enables BALL to read and write MDL MOL files. \\
			{\bf Definition:} \URL{BALL/FORMAT/MOLFile.h} \\
	*/
	class MOLFile
		: public GenericMolFile
	{
		public:

		BALL_CREATE(MOLFile)

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor
		*/
		MOLFile()
			throw();

		/** Detailed constructor.
		*/
		MOLFile(const String& filename, File::OpenMode open_mode = File::IN)
			throw(Exception::FileNotFound);

		/**	Copy constructor
		*/
		MOLFile(const MOLFile& file)
			throw(Exception::FileNotFound);

		/// Destructor
		virtual ~MOLFile()
			throw();
		
		//@}

		/**	@name Reading and Writing of Kernel Datastructures
		*/
		//@{
		
		/**	Write a system to the MOL file.
				Note that this changes the properties of atoms in the system.
		*/
		virtual void write(const System& system);
		
		/**	Read a system from the MOL file
		*/
		virtual void read(System&	system)
			throw(Exception::ParseError);

		//@}

		/**	@name	Accessors
		*/
		//@{
				
		//@}

		protected:
		/**	@name Format definitions
		*/
		//@{
		/// The format of the counts line
		static const String counts_format_;

		/// The format of an entry of the atom block
		static const String atom_format_;

		/// The format of an entry of the bond block
		static const String bond_format_;
		//@}

		/// Read the CTAB of a MOL file and construct a Molecule from its data
		Molecule* readCTAB(std::vector<Atom*>& atom_map)
			throw(Exception::ParseError);

		/**	@name	Structs
		*/
		//@{

		/** String constants used for named properties.
				Some of the data read from MOL files cannot be stored immediately in
				the kernel datastructures, so they are stored as \Ref{NamedProperty} objects
				in \Ref{Atom} and \Ref{Bond}. These string constants are used to access
				the corresponding fields of the atom and bond block of the MOL file.
				@see PropertyManager::setProperty
				@see PropertyManager::getProperty
		*/
		struct Property
		{
			///
			static const String ATOM_MASS_DIFFERENCE;
			///
			static const String ATOM_HYDROGEN_COUNT;
			///
			static const String ATOM_STEREO_CARE_BOX;
			///
			static const String ATOM_VALENCE;
			///
			static const String ATOM_H0_DESIGNATOR;
			///
			static const String ATOM_REACTION_COMPONENT_TYPE;
			///
			static const String ATOM_REACTION_COMPONENT_NUMBER;
			///
			static const String ATOM_INVERSION_RETENTION;
			///
			static const String ATOM_EXACT_CHANGE;

			///
			static const String BOND_STEREO;
			///
			static const String BOND_TOPOLOGY;
			///
			static const String BOND_REACTING_CENTER_STATUS;
		};

		///
	  class CountsStruct
		{
			public:
			Size    number_of_atoms;
			Size    number_of_bonds;
			Size    number_of_atom_lists;
			bool    chiral;
			Size    number_of_stext_entries;
			Size    number_of_reaction_components;
			Size    number_of_reactants;
			Size    number_of_products;
			Size    number_of_intermediates;
			String  version;
		};

		///
		class AtomStruct
		{
			public:
			Vector3		position;
			String		symbol;
			Index			mass_difference;
			Index			charge;
			Index			parity;
			Size			hydrogen_count;
			bool			stereo_care_box;
			Size			valence;
			bool			H0_designator;
			Position	reaction_component_type;
			Position	reaction_component_number;
			Position	number;
			Position	inversion_retention;
			bool			exact_change;
		};

		///
		class BondStruct
		{
			public:
			Position	first_atom;
			Position	second_atom;
			Position	type;
			Position	stereo;
			Position	topology;
			Position	reacting_center_status;
		};
		//@}

		void readCountsLine(CountsStruct& counts);

		void readAtomLine(AtomStruct& atom);

		void readBondLine(BondStruct& bond);

		void writeCountsLine(const CountsStruct& counts);

		void writeAtomLine(const AtomStruct& atom);

		void writeBondLine(const BondStruct& bond);
	};

} // namespace BALL

#endif // BALL_FORMAT_MOLFILE_H
