// $Id: forceFieldParameters.h,v 1.8 2000/10/05 17:34:16 anker Exp $
// Molecular Mechanics: general force field parameter class

#ifndef BALL_MOLMEC_FORCEFIELDPARAMETERS_H
#define BALL_MOLMEC_FORCEFIELDPARAMETERS_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
# include <BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
# include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
	class AtomTypes;
	
	/**	Force field parameter class.
	*/
	class ForceFieldParameters
		:	public Parameters
	{
		public:

		BALL_CREATE(ForceFieldParameters)

		friend class ForceField;

		/**@name	Constructors and destructor	*/
		//@{

		/**	Default constructor.
		*/
		ForceFieldParameters();

		/**	Constructor.
		*/
		ForceFieldParameters(const String& filename);

		/**	Copy constructor
		*/
		ForceFieldParameters(const ForceFieldParameters& force_field_parameter);

		/**	Destructor.
		*/
		virtual ~ForceFieldParameters();

		//@}


		/** @name Assignment */
		//@{

		/** Clear method */
		virtual void clear();

		/** Assignment operator */
		const ForceFieldParameters& operator = (const ForceFieldParameters& param);
		
		//@}


		/**@name	Accessors 	*/
		//@{

		/**	Return a reference to the atom type parameter section
		*/
		AtomTypes&	getAtomTypes();

		/**	Read the contents of the INI file and interpret them.
		*/
		virtual bool init();
		//@}

		/**	@name	Predicates
		*/
		//@{
			
		/**	Valididty predicate.
				Return {\bf true} if the force field parameters were correctly
				initialized, the internal INI file is valid and the internal atom types		
				object is valid.
				@return bool - {\tt valid_ && parameter_file_.isValid() && atom_types_.isValid()}
		*/
		virtual bool isValid() const;

		/** Equality operator */
		bool operator == (const ForceFieldParameters& param) const;

		//@}


		protected:

		/*_@name	Protected Members */
		//_@{ 

		/*_	the atom types section
		*/
		AtomTypes	atom_types_;
		//_@} 
	};

} // namespace BALL

#endif // BALL_MOLMEC_FORCEFIELDPARAMETERS_H
