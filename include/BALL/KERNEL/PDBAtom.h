// $Id: PDBAtom.h,v 1.6 2000/04/17 13:32:27 amoll Exp $

#ifndef BALL_KERNEL_PDBATOM_H
#define BALL_KERNEL_PDBATOM_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif


#define BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR              ' '
#define BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR           ' '
#define BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR   ' '
#define BALL_PDBATOM_DEFAULT_OCCUPANCY                      1.0
#define BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR             0.0


namespace BALL 
{

	class Chain;
	class Protein;
	class Residue;

	/** Atom class with additional attributes.
			This class is represents atoms read from PDB files.
			It contains additional attributes to hold the crystallographic properties
			of atoms from a PDB file (e.g., temperature factor, occupancy).\\
			{\bf Definition:}\URL{BALL/KERNEL/PDBAtom.h}
			@see PDBFile
	*/
	class PDBAtom
		: public Atom
	{
		public:
		
		friend class Bond;

		BALL_CREATE(PDBAtom)

		/**	@name	Enums
		*/
		//@{

		/**
		*/
		enum RemotenessIndicator
		{
			REMOTENESS_INDICATOR__UNKNOWN = BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR,
			REMOTENESS_INDICATOR__ALPHA   = 'A',
			REMOTENESS_INDICATOR__alpha   = 'a',
			REMOTENESS_INDICATOR__BETA    = 'B',
			REMOTENESS_INDICATOR__beta    = 'b',
			REMOTENESS_INDICATOR__GAMMA   = 'G',
			REMOTENESS_INDICATOR__gamma   = 'g',
			REMOTENESS_INDICATOR__DELTA   = 'D',
			REMOTENESS_INDICATOR__delta   = 'd',
			REMOTENESS_INDICATOR__EPSILON = 'E',
			REMOTENESS_INDICATOR__epsilon = 'e',
			REMOTENESS_INDICATOR__ZETA    = 'Z',
			REMOTENESS_INDICATOR__zeta    = 'z',
			REMOTENESS_INDICATOR__ETA     = 'H',
			REMOTENESS_INDICATOR__eta     = 'h',

			NUMBER_OF_REMOTENESS_INDICATORS
		};

		/**
		*/
		enum Property
		{
			NUMBER_OF_PROPERTIES = Atom::NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors */
		//@{

		/// Default constructor
		PDBAtom();

		/// Copy constructor
		PDBAtom(const PDBAtom& pdb_atom, bool deep = true);
	
		/// Detailled constructor
		PDBAtom
			(Element& element,
			 const String& name,
			 const String& type_name = BALL_ATOM_DEFAULT_TYPE_NAME,
			 Atom::Type atom_type = BALL_ATOM_DEFAULT_TYPE,
			 const Vector3& position = Vector3(BALL_ATOM_DEFAULT_POSITION),
			 const Vector3& velocity = Vector3(BALL_ATOM_DEFAULT_VELOCITY),
			 const Vector3& force = Vector3(BALL_ATOM_DEFAULT_FORCE),
			 float charge = BALL_ATOM_DEFAULT_CHARGE,
			 float radius = BALL_ATOM_DEFAULT_RADIUS,
			 char branch_designator =BALL_PDBATOM_DEFAULT_BRANCH_DESIGNATOR,
			 char remoteness_indicator =BALL_PDBATOM_DEFAULT_REMOTENESS_INDICATOR,
			 char alternate_location_indicator = BALL_PDBATOM_DEFAULT_ALTERNATE_LOCATION_INDICATOR,
			 float occupancy = BALL_PDBATOM_DEFAULT_OCCUPANCY,
			 float temperature_factor = BALL_PDBATOM_DEFAULT_TEMPERATURE_FACTOR);

		/// Destructor
		virtual ~PDBAtom();

		/// Clears the contents of the atom
		virtual void clear();
	
		/// Clears the contents of the atom and removes it from all composite structures
		virtual void destroy();

		//@}

		/** @name Persistence */
		//@{

		/// Persistent writing of PDBAtoms
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/// Persistent reading of PDBAtoms
		void persistentRead(PersistenceManager& pm);

		//@}


		/**	@name	*/
		//@{

		/** Assignment with cloning facility.
				Assign the PDB-atom {\em pdb_atom} to {\em *this} PDB-atom.
				The assignment is either deep or shallow (default).
				@param  pdb_atom the PDB-atom to be copied (cloned)
				@param  deep make a deep (={\tt true}) or shallow (={\tt false}) copy of {\em pdb_atom}
		*/
		void set(const PDBAtom& pdb_atom, bool deep = true);

		/** Assignment operator.
				Assign the PDB-atom {\em pdb_atom} to {\em *this} pdb_atom.
				The assignment is either deep or shallow (default).
				@param   pdb_atom the PDB-atom to be copied (cloned)
				@return  pdb_atom& - {\em *this} PDB-atom
		*/
		PDBAtom& operator = (const PDBAtom& pdb_atom);

		/** Copying with cloning facility.
				Copy {\em *this} PDB-atom to the PDB-atom {\em pdb_atom}.
				The assignment is either deep or shallow (default).
				@param  pdb_atom the PDB-atom to be assigned to
		*/
		void get(PDBAtom& pdb_atom, bool deep = true) const;

		/** Swapping of PDB-atom.
				Swap the states of {\em *this} PDB-atom with the PDB-atom {\em pdb_atom}.
				@param  pdb_atom the PDB-atom {\em *this} PDB-atom is being swapped with
		*/
		void swap(PDBAtom& pdb_atom);
		
		//@}

		/**	@name	Accessors */
		//@{

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} PDB-atom does not have a parent protein.
				@return  Protein* -
								 mutable reference to the parent protein of {\em *this} PDB-atom,
		*/
		Protein* getProtein();

		/** Get a pointer to the parent protein.
				The reference is 0 if {\em *this} PDB-atom does not have a parent protein.
				@return  Protein* -
								 constant reference to the parent protein of {\em *this} PDB-atom,
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the parent chain.
				The reference is 0 if {\em *this} PDB-atom does not have a parent chain.
				@return  Chain* -
								 mutable reference to the parent chain of {\em *this} PDB-atom,
		*/
		Chain* getChain();

		/** Get a pointer to the parent chain.
				The reference is 0 if {\em *this} PDB-atom does not have a parent chain.
				@return  Chain* -
								 constant reference to the parent chain of {\em *this} PDB-atom,
		*/
		const Chain* getChain() const;

		/** Get a pointer to the parent residue.
				The reference is 0 if {\em *this} PDB-atom does not have a parent residue.
				@return  Residue* -
								 mutable reference to the parent residue of {\em *this} PDB-atom,
		*/
		Residue* getResidue();

		/** Get a pointer to the parent residue.
				The reference is 0 if {\em *this} PDB-atom does not have a parent residue.
				@return  Residue* -
								 constant reference to the parent residue of {\em *this} PDB-atom,
		*/
		const Residue* getResidue() const;

		/** Set the branch designator of this PDB-atom.
				@param branch_designator the branch designator
		*/
		void setBranchDesignator(char branch_designator);

		/** Get the branch designator of this PDB-atom.
				@return char the branch designator
		*/
		char getBranchDesignator() const;

		/** Set the remoteness indicator of this PDB-atom.
				@param remoteness_indicator the remoteness indicator
		*/
		void setRemotenessIndicator(char remoteness_indicator);

		/** Get the remoteness indicator of this PDB-atom.
				@return char the remoteness indicator
		*/
		char getRemotenessIndicator() const;

		/** Set the alternate location indicator of this PDB-atom.
				@param alternate_location_indicator the alternate location indicator
		*/
		void setAlternateLocationIndicator(char alternate_location_indicator);

		/** Get the alternate location indicator of this PDB-atom.
				@return char the alternate location indicator
		*/
		char getAlternateLocationIndicator() const;

		/** Set the occupancy of this PDB-atom.
				@param occupancy the occupancy
		*/
		void setOccupancy(float occupancy);

		/** Get the occupancy of this PDB-atom.
				@return float occupancy the occupancy
		*/
		float getOccupancy() const;

		/** Set the temperature factor of this PDB-atom.
				@param temperature_factor the temperature factor
		*/
		void setTemperatureFactor(float temperature_factor);

		/** Get the temperature factor of this PDB-atom.
				@return float the temperature factor
		*/
		float getTemperatureFactor() const;

		//@}

		/**	@name	Debugging and Diagnostics */
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} PDB-atom to the output ostream {\em s} with dumping depth {\em depth}.
	
				@param	s output stream where to output the internal state of {\em *this} PDB-atom
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;
		//@}


		/**	@name	Storers
		*/
		//@{

		/* Persistent stream input and state restorage.
				Read persistent bond data from the input stream {\em s} and restore the state of {\em *this} PDB-atom.
				\\
				{\bf Note:} Not yet implemented.
				@param  s input stream from where to restore the internal state of {\em *this} PDB-atom
		*/
		virtual void read(std::istream& s);

		/* Persistent stream output and state storage.
				Write persistent bond data to the output stream {\em s} and store the state of {\em *this} PDB-atom.
				\\
				{\bf Note:} Not yet implemented.	
				@param  s input stream from where to restore the internal state of {\em *this} PDB-atom
		*/
		virtual void write(std::ostream& s) const;
		//@}

		private:

		void clear_();

		char 	branch_designator_;
		char	remoteness_indicator_;
		char 	alternate_location_indicator_;
		float occupancy_;
		float temperature_factor_;
	};

} // namespace BALL

#endif // BALL_KERNEL_PDBATOM_H
