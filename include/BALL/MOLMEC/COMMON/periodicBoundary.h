// $Id: periodicBoundary.h,v 1.12 2001/06/21 21:28:27 amoll Exp $

#ifndef BALL_MOLMEC_COMMON_PERIODICBOUNDARY_H
#define BALL_MOLMEC_COMMON_PERIODICBOUNDARY_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
#endif

namespace BALL 
{

	class ForceField;

	class PeriodicBoundary;

	/**	Periodic boundary class for force field simulations. 
			{\bf Definition:} \URL{BALL/MOLMEC/COMMON/periodicBoundary.h} \\
			Molecular Mechanics: class representing periodic boundary conditions.
	*/
	class PeriodicBoundary
	{
		public:

		BALL_CREATE(PeriodicBoundary)

		/**	@name	Type definitions 
		*/
		//@{
		
		///
		typedef	pair<Size, Size>				AtomIndexPair;

		///
		typedef vector<AtomIndexPair>		AtomIndexArray;

		//@}		
		/**	@name	Constants used as option keys
		*/
		//@{

		///
		struct Option 
		{
			///	Vector containing the lower corner of the box
			static const char* PERIODIC_BOX_LOWER;

			///	Vector containing the upper corner of the box
			static const char* PERIODIC_BOX_UPPER;

			///	 Bool that indicates whether periodic boundary is enabled or not 
			static const char* PERIODIC_BOX_ENABLED;
		
			///	Minimum distance between solute molecules and the box in Angstrom
			static const char* PERIODIC_BOX_DISTANCE;
			
			///	Flag to initiate the addition of solvent to the box
			static const char* PERIODIC_BOX_ADD_SOLVENT;
			
			/** Name of the file containing the solvent.
					This file should contain an equilibrated box of the solvent
					in the HyperChem format.
			*/
			static const char* PERIODIC_BOX_SOLVENT_FILE;

			/**	Minimum distance between solvent and solute for added solvent.
			*/
			static const char* PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE;

			/**	Filename for the default solvent.
			*/
			static const char* PERIODIC_WATER_FILE;

			};
		
		/// Default values
		struct Default 
		{
			///	Default vector for the lower corner of the box
			static const Vector3 PERIODIC_BOX_LOWER; 

			///	Default vector for the upper corner of the box
			static const Vector3 PERIODIC_BOX_UPPER;

			///	Default bool value for periodic boundary (enabled or not)
			static const bool PERIODIC_BOX_ENABLED;

			///	Default minimum distance between solute molecules and the box in Angstrom
			static const float PERIODIC_BOX_DISTANCE;

			///	Default value of the flag to initiate the addition of solvent to the box
			static const bool PERIODIC_BOX_ADD_SOLVENT;

			/**	Default file for adding solvent molecules into the box.
					This file should contain an equilibrated box of the solvent
					in the HyperChem format.
			*/
			static const char* PERIODIC_BOX_SOLVENT_FILE;

			///	Default minimum distance between solvent and solute for added solvent.
			static const float PERIODIC_BOX_SOLVENT_SOLUTE_DISTANCE;

			///	Default file for default solvent.
			static const char* PERIODIC_WATER_FILE;
		};

		//@}
		/**	@name	Constructors and Destructors	
		*/
		//@{

		/**	Default constructor.
		*/
		PeriodicBoundary();

		/**	Constructor.
		*/
		PeriodicBoundary(const ForceField& force_field);

		/**	Copy constructor
		*/
		PeriodicBoundary(const PeriodicBoundary& periodic_boundary);

		/**	Destructor.
		*/
		virtual ~PeriodicBoundary();

		//@}
		/**	@name	Assignment
		*/
		//@{
			
		/**	Assignment operator.
		*/
		PeriodicBoundary& operator = (const PeriodicBoundary& periodic_boundary);

		/**	Clear method
		*/
		virtual void clear() 
			throw();

		//@}
		/**	@name	Setup Methods 
		*/
		//@{

		/**	Sets up the periodic box
		*/
		bool setup();

		/**	Generate the vector of molecules of the system
		*/
		Size	generateMoleculesVector();

		//@}
		/**	@name	Accessors 
		*/
		//@{
		
		/// Enable periodic boundary conditions
		void enable();

		/// Disable periodic boundary conditions
		void disable();

		/**	Retrieve the box dimensions.
		*/
		Box3 getBox() const;

		/**	Set the box dimensions.
				The box dimensions are additionally stored in options
				using keys \Ref{lower} and \Ref{upper}.
		*/
		void setBox(const Box3& box);

		/** Fill the defined box with a solvent.
				This method fills the defined box with solvent molecules.
				If no box is defined, no solvent is added. The neccessary
				solvent box template is either taken from the options (solvent_file)
				or a default is assumed.\\
				This method replicates the solvent box starting at the box origin in
				all three dimensions and removes all solvent molecules that are within 
				\Ref{solvent_distance} of a solute molecule or outside the periodic box.\\
				The method returns the number of inserted solvent molecules. All solvent 
				molecules are marked as such by setting their property \Ref{Molecule::PROPERTY__PERIODIC_BOX_SOLVENT}.\\
				@see	removeSolvent
		*/
		Size addSolvent(const String& filename) const;
		
		/**	Removes all solvent molecules
				This method removes all molecules that have the property \Ref{Molecule::PROPERTY__PERIODIC_BOX_SOLVENT}
				set from the force field's system.
				The number of removed solvent molecules is returned.
				@see	addSolvent
		*/
		Size removeSolvent() const;

		//@}
		/**	@name	Predicates	
		*/
		//@{
			
		/**	Returns {\bf true} if the periodic boundary is enabled
				@see	enable
				@see	disable
		*/
		bool isEnabled() const;

		//@}
		/**	@name Periodic boundary methods 
		*/
		//@{
		
		/**	Checks which molecules are inside the box.
				If the center of mass of a molecule leaves the box, 
				it is inserted on the other side.
		*/
		void updateMolecules();

		//@}
		/**	@name	Variables
		*/
		//@{
		
		/**	Periodic Boundary options.
				This is usually a pointer to the options of the force field the PeriodicBoundary object 
				is contained in.
		*/
		Options*	options;

		//@}

		protected:

		private:

		/*_	@name Private Attributes	
		*/
		//_@{

		//_ Pointer to the force field of the periodic boundary 
		ForceField*	force_field_;

		//_ Indicates if periodic boundary is enabled or not 
		bool		enabled_;

		//_ The box of the periodic boundary 
		Box3		box_;

		//_ An pair vector with the start and end indices of all molecules (atoms of the molecules)
		AtomIndexArray	molecules_;

		//_@}

	};

} // namespace BALL



#endif // BALL_MOLMEC_COMMON_PERIODICBOUNDARY_H
