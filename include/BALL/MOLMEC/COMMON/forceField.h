// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceField.h,v 1.33 2005/12/23 17:01:51 amoll Exp $
//

#ifndef BALL_MOLMEC_COMMON_FORCEFIELD_H
#define BALL_MOLMEC_COMMON_FORCEFIELD_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_KERNEL_SYSTEM_H
#	include <BALL/KERNEL/system.h>
#endif

#ifndef BALL_DATATYPE_OPTIONS_H
#	include <BALL/DATATYPE/options.h>
#endif

#ifndef BALL_CONCEPT_TIMESTAMP_H
#	include <BALL/CONCEPT/timeStamp.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDPARAMETERS_H
#	include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_MOLMEC_COMMON_PERIODIC_BOUNDARY_H
#	include <BALL/MOLMEC/COMMON/periodicBoundary.h>
#endif

#ifndef BALL_MOLMEC_COMMON_ATOMVECTOR_H
#	include <BALL/MOLMEC/COMMON/atomVector.h>
#endif

#include <vector>

namespace BALL 
{
	class ForceFieldComponent;

	/**	Force field class.
			This class is used to represent a general force field.
			Each force field by itself is composed by several
			different force field components which are represented
			by  \link ForceFieldComponent ForceFieldComponent \endlink  objects. \par
			Each ForceField object provides a list of components
			which may be manipulated by the user to generate the
			force field he needs. \par
			ForceField only represents a baseclass to the specific force field
			implementations (e.g.  \link AMBER AMBER \endlink ) and implements the common interface 
			and the neccessary mechanisms to administer the force field	components. \par
			A typical force field contains a small number of components (e.g. bond stretch,
			bend, torsion and non-bonding interactions). 
			A specialized forcefield has to implement each of these components (by deriving them
			from  \link ForceFieldComponent ForceFieldComponent \endlink  and must be derived from ForceField.
			Basically only the default constructor for the new forcefield has to
			be modified to create an instance of each of the components and 
			register them by calling  \link insertComponent insertComponent \endlink . But in order to implement
			the setup methods of the force field components efficiently, 
			jobs like assigning atom types, reading parameter files and the such should be
			provided for all force field components by the corrseponding ForceField
			object. This should be implemented in the force field-specific  \link specificSetup specificSetup \endlink  
			method. \par
			For an efficient and easy to use implementation of a forcefield
			parameter file, please refer to  \link ForceFieldParameters ForceFieldParameters \endlink  and its
			related objects. \par
			Each force field provides as well energy and its derivatives, the force
			on each atom. A calculation of the energy is done by calling
			 \link updateEnergy updateEnergy \endlink . This method iterates over the list of force field
			components and invokes the updateEnergy method of each of these components.
			The total energy may then be retrieved by calling  \link getEnergy getEnergy \endlink . \par
			Analogously, a force calculation is performed by invoking  \link updateForces updateForces \endlink .
			 \link getForces getForces \endlink  returns a pointer to an array containing the forces
			for each atom. \par
			
    	\ingroup  MolmecCommon
	*/
	class BALL_EXPORT ForceField
	{
		public:

		friend class ForceFieldComponent;

		/**	@name	Type Definitions
		*/
		//@{

		/**	Atom pair vector.
				This type is used to represent "pair lists". In fact, it is
				a <tt>vector</tt> of <tt>pair</tt>s of atom pointers.
		*/
		typedef std::vector<std::pair<Atom*, Atom*> >	PairVector;

		//@}
		/**	@name	Constructors and Destructors	
		*/
		//@{
		
		BALL_CREATE(ForceField)

		/**	Default constructor.
				A default-constructed force field is not valid!
				It becomes valid after a successful call to setup.
		*/
		ForceField();

		/**	Constructor.
				The successful setup of the force field can be verified
				by the  \link isValid isValid \endlink  method.
		*/
		ForceField(System& system);

		/**	Constructor.
				The successful setup of the force field can be verified
				by the  \link isValid isValid \endlink  method.
		*/
		ForceField(System& system, const Options& options);

		/**	Copy constructor
		*/
		ForceField(const ForceField& force_field);

		/**	Destructor.
		*/
		virtual ~ForceField();

		/**	Clear method.
		*/
		virtual void clear()
			;

		//@}
		/**	@name	Assignments 
		*/
		//@{

		/**	Assignment operator
		*/
		ForceField&	operator = (const ForceField& force_field);

		//@}
		/**	@name	Debugging and Diagnostics 
		*/
		//@{

		/**	Check the force field's validity.
				A force field is valid if it is bound to a system
				and  \link setup setup \endlink  was successful.
		*/
		bool isValid() const
			;

		//@}
		/**	@name	Setup methods 
		*/
		//@{

		/**	Sets up the force field and its components.
		*/
		bool setup(System& system);

		/**	Sets up the force field and its components.
		*/
		bool setup(System& system, const Options& options);

		/**	Force field specific setup.
				This method is called by setup.
		*/
		virtual bool specificSetup()
			throw(Exception::TooManyErrors);

		/** Set the number of atoms, for which the setup of the forcefield can
		    fail, until the setup() methods aborts and return false.
				By default, there is no limit set.
		*/
		void setMaximumNumberOfErrors(Size nr);

		/** Get the number of atoms, for which the setup of the forcefield can
		    fail, until the setup() methods aborts and return false.
		*/
		Size getMaximumNumberOfErrors() const;

		/// Get the atoms, for which the force field setup failed.
		HashSet<const Atom*>& getUnassignedAtoms();

		//@}
		/**	@name	Accessors 
		*/
		//@{

		/**	Sets the force field name.
		*/
		void setName(const String& name);
		
		/**	Returns the force field name
		*/
		String getName() const;
		
		/**	Returns the number of atoms stored in the force field
		*/
		Size getNumberOfAtoms() const;

		/**	Returns the number of non-fixed atoms stored in the force field.
				If the option <tt>SELECTION_FIXED</tt> is set to <b>true</b> or
				<tt>SELECTION_MOVABLE</tt> is set to <b>true</b> the atom array
				is split. The first section (indices 0 to  \link getNumberOfMovableAtoms getNumberOfMovableAtoms \endlink )
				contains the atoms that are to be moved, the rest of the array contains
				the fixed atoms.
		*/
		Size getNumberOfMovableAtoms() const;

		/**	Returns a reference to the atom vector
		*/
		BALL_INLINE
		const	AtomVector& getAtoms() const ;

		/**	Returns a pointer to the system
		*/
		BALL_INLINE
		System* getSystem() ;

    /** Returns a const pointer to the system
    */
		BALL_INLINE
    const System* getSystem() const ;

		/**	Return the status of the selection mechanism
		*/
		BALL_INLINE
		bool getUseSelection() const ;

		/**	Set the status of the selection mechanism
		*/
		BALL_INLINE
		void disableSelection() ;

		/**	Set the status of the selection mechanism
		*/
		BALL_INLINE
		void enableSelection() ;

		/**	Return the status of the selection mechanism */
		BALL_INLINE
		bool isSelectionEnabled() const ;

		/**	Returns a pointer to the parameter file
		*/
		ForceFieldParameters& getParameters();

		/**	Returns the number of components registered by the force field.
		*/
		Size countComponents() const;

		/**	Return the point of time of the last call to update.
				ForceField contains a time stamp which is used to determine
				whether the selection or even the topology of the system
				has changed. Every time update is called, the 
				 \link update_time_stamp_ update_time_stamp_ \endlink  is updated. Similarly, all setup methods
				update the  \link setup_time_stamp_ setup_time_stamp_ \endlink 
		*/
		const TimeStamp& getUpdateTime() const
			;
		
		/**	Return the point of time of the last call to setup.
				ForceField contains a time stamp which is used to determine
				whether the selection or even the topology of the system
				has changed. Every time update is called, the 
				 \link update_time_stamp_ update_time_stamp_ \endlink  is updated. Similarly, all setup methods
				update the  \link setup_time_stamp_ setup_time_stamp_ \endlink 
		*/
		const TimeStamp& getSetupTime() const
			;
		
		/**	Insert a new component into the force field's component list.
				Responsability for the destruction of the component is passed on to
				the ForceField instance.
		*/
		void insertComponent(ForceFieldComponent* force_field_component);

		/**	Remove a component from the force field's component list.
				The ForceFieldComponent will be destructed and removed from the component list.
		*/
		void removeComponent(const ForceFieldComponent* force_field_component);

		/**	Remove a component from the force field's component list.
		*/
		void removeComponent(const String& name);

		/**	Return a pointer to the specified force field component.
				If the specified index does not exist, 0 is returned.
				The given index should be smaller than the value returned by
				countComponents. The first component in the list has the index zero.
		*/
		ForceFieldComponent* getComponent(const Size	index) const;

		/**	Return a pointer to the specified force field component.
				If a component with the specified name does not exist, 0 is returned.
		*/
		ForceFieldComponent* getComponent(const String& name) const;

		/**	Return the sum of energies of all registered force field components. 
				No calculation will be performed. This method simply returns the 
				last value for the total energy calculated by updateEnergy.
				@return	double - energy in kJ/mol
		*/
		double getEnergy() const;

		/**	Calculate the sum of energies of all force field components and returns its value.
		*/
		double updateEnergy();

		/**	Calculate the forces caused by each component and updates the current forces.
		*/
		void updateForces();

		/**	Calculates the RMS of the current gradient
		*/
		double getRMSGradient() const;

		/**	Return the update frequency for pair lists etc.
				This method is used by minimizers or the MD simulation to determine the number
				of iterations between two calls to  \link update update \endlink .
		*/
		virtual Size getUpdateFrequency() const;

		/**	Update internal data structures.
				The force field may use this method to update internal data structures
				(e.g. pair lists) periodically. The MD simulation class as well as the minimizer classes
				use  \link getUpdateFrequency getUpdateFrequency \endlink  to determine the number of iterations
				between two calls to update. \par
				The default implementation calls  \link ForceFieldComponent::update ForceFieldComponent::update \endlink  for
				each component in the force field.
		*/
		virtual void update()
			throw(Exception::TooManyErrors);

		/** Get the current results in String form
		 		(Generic function to be overloaded in derived classes.)
		*/
		virtual String getResults() const
			 { return "undefined";}

		//_ Report an error and increase the error counter
		std::ostream& error() throw(Exception::TooManyErrors);

		//@}
		/**	@name	Public Attributes
		*/
		//@{
		
		/**	Force field options
		*/
		Options	options;

		/**	Periodic boundary
		*/
		PeriodicBoundary	periodic_boundary;

		//@}

		protected:
		/*_	Collect all atoms into the atoms_ vector.
		*/
		void collectAtoms_(const System& system);

		/*_	Sort the atom vector wrt selection.
		*/
		void sortSelectedAtomVector_();

		/**
		 * Check whether the force field needs to be updated and perform the necessary steps
		 */
		virtual void performRequiredUpdates_();

		/*_	@name	Protected Attributes
		*/
		//_@{

		/*_	The system the force field is bound to
		*/
		System*	system_;

		/*_	The atoms in the simulated system
		*/
		AtomVector	atoms_;

		/*_ An object containing the force field parameters read from a file
		*/
		ForceFieldParameters	parameters_;	

		/*_	The boolean variable indicates if the setup of the force field was successful
		*/
		bool 	valid_;

		/*_ The force field name
		*/
		String	name_;

		/*_	The total energy
		*/
		double	energy_;
		
		/*_	The components of the force field
		*/
		vector<ForceFieldComponent*>	components_;

		/*_	The number of movable atoms in the force field
		*/
		Size	number_of_movable_atoms_;

		/*_	Do we have to check whether atoms are selected?
		*/
		bool	use_selection_;

		/*_	This flag can temporarily disable the selection.
		*/
		bool selection_enabled_;

		/*_	The time of the last call to update.
		*/
		TimeStamp	update_time_stamp_;

		/*_	The time of the last call to setup.
		*/
		TimeStamp	setup_time_stamp_;

		//_ Atoms, for which the setup of the force field fails
		HashSet<const Atom*> unassigned_atoms_;

		//_ max number of unassigned atoms
		Size max_number_of_errors_;

		Size number_of_errors_;

		//_@}
	};

# ifndef BALL_NO_INLINE_FUNCTIONS
#   include <BALL/MOLMEC/COMMON/forceField.iC>
# endif

} // namespace BALL

#endif // BALL_MOLMEC_COMMON_FORCEFIELD_H
