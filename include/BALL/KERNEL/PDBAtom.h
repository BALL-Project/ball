// $Id: PDBAtom.h,v 1.2 1999/12/04 18:34:16 oliver Exp $

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

	/**
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

		///
		void set(const PDBAtom& pdb_atom, bool deep = true);

		///
		PDBAtom& operator = (const PDBAtom& pdb_atom);

		///
		void get(PDBAtom& pdb_atom, bool deep = true) const;

		///
		void swap(PDBAtom& pdb_atom);
		
		//@}

		/**	@name	Accessors */
		//@{

		///
		Protein* getProtein();

		///
		const Protein* getProtein() const;

		///
		Chain* getChain();

		///
		const Chain* getChain() const;

		///
		Residue* getResidue();

		///
		const Residue* getResidue() const;

		///
		void setBranchDesignator(char branch_designator);

		///
		char getBranchDesignator() const;

		///
		void setRemotenessIndicator(char remoteness_indicator);

		///
		char getRemotenessIndicator() const;

		///
		void setAlternateLocationIndicator(char alternate_location_indicator);

		///
		char getAlternateLocationIndicator() const;

		///
		void setOccupancy(float occupancy);

		///
		float getOccupancy() const;

		///
		void setTemperatureFactor(float temperature_factor);

		///
		float getTemperatureFactor() const;

		//@}

		/**	@name	Debugging and Diagnostics */
		//@{

		///
		virtual bool isValid() const;

		///
		virtual void dump(ostream& s = cout, unsigned long depth = 0) const;
		//@}


		/**	@name	Storers
		*/
		//@{
		///
		virtual void read(istream& s);

		///
		virtual void write(ostream& s) const;
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
