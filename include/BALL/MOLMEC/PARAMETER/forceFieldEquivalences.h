// Molecular Mechanics: general force field parameter class

#ifndef BALL_MOLMEC_PARAMETER_FORCEFIELDEQUIVALENCES_H
#define BALL_MOLMEC_PARAMETER_FORCEFIELDEQUIVALENCES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_FORMAT_PARAMETERS_H
# include <BALL/FORMAT/parameters.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMEQUIVALENCES_H
# include <BALL/MOLMEC/PARAMETER/atomEquivalences.h>
#endif

#ifndef BALL_SYSTEM_FILE_H
# include <BALL/SYSTEM/file.h>
#endif


namespace BALL 
{
	class AtomEquivalences;
	
	/**	Force field equivalence class.
			
			\ingroup  MolmecParameters
	*/
	class BALL_EXPORT ForceFieldEquivalences
		:	public Parameters
	{
		public:
		BALL_CREATE(ForceFieldEquivalences)

		friend class ForceField;

		/**@name	Constructors and destructor	
		*/
		//@{

		/**	Default constructor.
		*/
		ForceFieldEquivalences(ForceField* force_field=0);

		/**	Constructor.
		*/
		ForceFieldEquivalences(const String& filename, ForceField* force_field=0);

		/**	Copy constructor
		*/
		ForceFieldEquivalences(const ForceFieldEquivalences& force_field_parameter);

		/**	Destructor.
		*/
		virtual ~ForceFieldEquivalences();

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Clear method 
		*/
		virtual void clear();

		/** Assignment operator 
		*/
		const ForceFieldEquivalences& operator = (const ForceFieldEquivalences& param);
		
		//@}
		/**@name	Accessors 	
		*/
		//@{

		/**	Return a reference to the atom type parameter section
		*/
		AtomEquivalences&	getAtomEquivalences();

		/** Return a pointer to the forcefield
		 */
		ForceField* getForceField();

		/** Write the force field parameter in Parm-file format
		 */
		virtual bool exportParmFile(File& outfile) const;

		/**	Read the contents of the INI file and interpret them.
		*/
		virtual bool init();

		//@}
		/**	@name	Predicates
		*/
		//@{
			
		/**	Valididty predicate.
				Return <b>true</b> if the force field parameters were correctly
				initialized, the internal INI file is valid and the internal atom types		
				object is valid.
				@return bool - <tt>valid_ && parameter_file_.isValid() && atom_types_.isValid()</tt>
		*/
		virtual bool isValid() const;

		/** Equality operator 
		*/
		bool operator == (const ForceFieldEquivalences& param) const;

		//@}

		protected:

		/*_@name	Protected Members 
		*/
		//_@{ 
	
		/*_	the equivalent atom types section
		*/
		AtomEquivalences	atom_type_equivalences_;

		/*_ the forcefield defined by these parameters
		 */
		ForceField*  force_field_;

		//_@} 
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_FORCEFIELDEQUIVALENCES_H

