// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_KERNEL_PDBATOM_H
#define BALL_KERNEL_PDBATOM_H

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
			This class represents atoms read from PDB files.
			It contains additional attributes to hold the crystallographic properties
			of atoms from a PDB file (e.g., temperature factor, occupancy).
			For details on the PDB format and the detailed meaning of those fields,
			please refer to the PDB documentation at http://www.rcsb.org
			 \par
			
			@see PDBFile
    	
			\ingroup KernelContainers 
	*/
	class BALL_EXPORT PDBAtom
		: public Atom
	{
		public:
		
		friend class Bond;

		BALL_CREATE_DEEP(PDBAtom)

		/**	@name	Enums
		*/
		//@{

		/** Possible values for the PDB remoteness indicator
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

		/** Properties of the PDBAtom.
		*/
		enum Property
		{
			///Property which indicates whether the current atom is a HETATM
			PROPERTY__HETATM = Atom::NUMBER_OF_PROPERTIES + 1,

			///The number of properties in PDBAtom
			NUMBER_OF_PROPERTIES
		};
		//@}

		/**	@name	Constructors and Destructors 
		*/
		//@{

		/// Default constructor
		PDBAtom();

		/// Copy constructor
		PDBAtom(const PDBAtom& pdb_atom, bool deep = true);
	
		/// Constructor
		PDBAtom(const String& name);

		/// Detailed constructor
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
	
		/// Clears the contents of the atom and removes it from all composite
		/// structures.
		virtual void destroy();

		//@}
		/** @name Predicates
		*/
		//@{

		/**	Equality operator.
				@see Object::operator ==
		*/
		bool operator == (const PDBAtom& pdb_atom) const;

		/**	Inequality operator
				@see operator ==
		*/
		bool operator != (const PDBAtom& pdb_atom) const;

		//@}
		/** @name Persistence 
		*/
		//@{

		/**	Writes a PDBAtom object to a persistent stream.
		 *	@param pm the persistence manager
		 *	@param name the instance name
		 */
		void persistentWrite(PersistenceManager& pm, const char* name = 0) const;

		/**	Reads a PDBAtom object from a persistent stream.
		 *	@param pm the persistence manager
		 */
		void persistentRead(PersistenceManager& pm);

		//@}
		/**	@name	Assignment
		*/
		//@{

    /** Assignment with cloning facility.
        The assignment is either deep or shallow (default).
        @param  pdb_atom the PDBAtom to be copied (cloned)
        @param  deep make a deep (=<tt>true</tt>) or shallow (=<tt>false</tt>) copy
    */
    void set(const PDBAtom& pdb_atom, bool deep = true);

    /** Copy this instance to {\em pdb_atom}.
        The assignment is either deep or shallow (default).
        @param  pdb_atom the PDBAtom to be assigned to
    */
    void get(PDBAtom& pdb_atom, bool deep = true) const;

		/** Assignment operator.
				The assignment is deep.
				@param   pdb_atom the PDBAtom to be copied (cloned)
				@return  pdb_atom& - the instance
		*/
		PDBAtom& operator = (const PDBAtom& pdb_atom);

		/** Swapping of PDBAtoms.
				@param  pdb_atom the PDBAtom {\em *this} is being swapped with
		*/
		void swap(PDBAtom& pdb_atom);

		/** Clear the contents of the atom.
		*/
		virtual void clear();
		
		//@}
		/**	@name	Accessors 
		*/
		//@{

		/** Get a pointer to the parent protein.
				The pointer is 0 if the instance does not have a parent protein.
				@return  Protein* - mutable pointer to the parent protein
		*/
		Protein* getProtein();

		/** Get a pointer to the parent protein.
				The pointer is 0 if the instance does not have a parent protein.
				@return  Protein* - constant pointer to the parent protein
		*/
		const Protein* getProtein() const;

		/** Get a pointer to the parent chain.
				The pointer is 0 if the instance does not have a parent chain.
				@return  Chain* - mutable pointer to the parent chain
		*/
		Chain* getChain();

		/** Get a pointer to the parent chain.
				The pointer is 0 if the instance does not have a parent chain.
				@return  Chain* - constant pointer to the parent chain
		*/
		const Chain* getChain() const;

		/** Get a pointer to the parent residue.
				The pointer is 0 if the instance does not have a parent residue.
				@return  Residue* - mutable pointer to the parent residue
		*/
		Residue* getResidue();

		/** Get a pointer to the parent residue.
				The pointer is 0 if the instance does not have a parent residue.
				@return  Residue* - constant pointer to the parent residue
		*/
		const Residue* getResidue() const;

		/** Set the branch designator.
				@param branch_designator the branch designator
		*/
		void setBranchDesignator(char branch_designator);

		/** Get the branch designator.
				@return char the branch designator
		*/
		char getBranchDesignator() const;

		/** Set the remoteness indicator.
				@param remoteness_indicator the remoteness indicator
		*/
		void setRemotenessIndicator(char remoteness_indicator);

		/** Get the remoteness indicator.
				@return char the remoteness indicator
		*/
		char getRemotenessIndicator() const;

		/** Set the alternate location indicator.
				@param alternate_location_indicator the alternate location indicator
		*/
		void setAlternateLocationIndicator(char alternate_location_indicator);

		/** Get the alternate location indicator.
				@return char the alternate location indicator
		*/
		char getAlternateLocationIndicator() const;

		/** Set the occupancy.
				@param occupancy the occupancy
		*/
		void setOccupancy(float occupancy);

		/** Get the occupancy.
				@return float occupancy the occupancy
		*/
		float getOccupancy() const;

		/** Set the temperature factor.
				@param temperature_factor the temperature factor
		*/
		void setTemperatureFactor(float temperature_factor);

		/** Get the temperature factor.
				@return float the temperature factor
		*/
		float getTemperatureFactor() const;

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/** Internal state dump.
				Dump the current internal state of {\em *this} to the 
				output ostream <b>  s </b> with dumping depth <b>  depth </b>.
				@param	s output stream where to output the internal state of the instance
				@param  depth the dumping depth
		*/
		virtual void dump(std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		private:

		void clear_();

		/*_ The PDB branch designator
		*/
		char 	branch_designator_;

		/*_ The PDB remoteness indicator
		*/
		char	remoteness_indicator_;

		/*_ The PDB alternate location indicator
		*/
		char 	alternate_location_indicator_;

		/*_ The PDB occupancy
		*/
		float occupancy_;

		/*_ The PDB temperature factor
		*/
		float temperature_factor_;

	};
} // namespace BALL

#endif // BALL_KERNEL_PDBATOM_H
