// $Id: forceFieldComponent.h,v 1.4 2000/03/26 12:44:20 oliver Exp $
// Molecular Mechanics: general force field component class

#ifndef BALL_MOLMEC_FORCEFIELDCOMPONENT_H
#define BALL_MOLMEC_FORCEFIELDCOMPONENT_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include	<BALL/DATATYPE/string.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include	<BALL/KERNEL/atom.h>
#endif


namespace BALL 
{

	class ForceField;

	/**	Generic force field component class
	*/
	class ForceFieldComponent 
	{

		friend class ForceField;

		public:

		/**	@name	Constructors and Destructors 
		*/
		//@{

		public:

		BALL_CREATE(ForceFieldComponent)


		/**	Default constructor.
		*/
		ForceFieldComponent();

		/**	Constructor.
		*/
		ForceFieldComponent(ForceField*	force_field);

		/**	Copy constructor.
			Creates a new instance of the given ForceFieldComponent.
			The created object will not be assigned to an existing force field.
		*/
		ForceFieldComponent(const ForceFieldComponent&	force_field_component, bool clone_deep = true);

		/**	Destructor.
		*/
		virtual ~ForceFieldComponent();

		//@} 

		/**	@name Setup method
		*/
		//@{

		/**	Setup method.
		*/
		virtual bool setup();

		//@}
		

		/**	@name	Accessors	
		*/
		//@{

		/**	Returns the parent force field.
			0 is returned, if no force field is assigned.
		*/	
		ForceField*	getForceField() const;

		/**	Sets the force field.
		*/
		void	setForceField(ForceField* force_field);

		/**	Sets the component's name.
		*/
		void	setName(const String& name);


		/**	Returns the component's name 
		*/
		String	getName() const;

		//@}

		
		/**	@name	Force field calculations
		*/
		//@{

		/**	Returns the energy of the force field component
		*/
		virtual double getEnergy() const;

		/**	Returns the energy of the force field component.
				The current energy for this force field component is being
				calculated and returned in units of kJ/mol.\\
				If the component isn't assigned to a ForceField object,
				zero is returned.
		*/
		virtual double	updateEnergy();
		

		/**	Updates the atomic forces in the force field.
				The forces created by this ForceFieldComponent are
				calculated for each atom and updated in the corresponding 
				array (forces) of the ForceField instance this component 
				is assigned to.
		*/
		virtual void updateForces();
		
		//@}


		protected:

		/*_	@name Protected Attributes
		*/
		//_@{

		/*_	The force field this component is registered in
		*/
		ForceField*	force_field_;

		/*_	The energy of the component
		*/
		double	energy_;

		//_@}

			
		private:
		
		/*_	@ name Private Attributes
		*/
		//_@{

		/*_	The force field component name
		*/
		String	name_;
		//_@}

	};

} // namespace BALL

#endif // BALL_MOLMEC_FORCEFIELDCOMPONENT_H
