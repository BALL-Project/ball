// $Id: lennardJones.h,v 1.2 1999/12/04 18:34:23 oliver Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file
 
#ifndef BALL_MOLMEC_PARAMETER_LENNARDJONES_H
#define BALL_MOLMEC_PARAMETER_LENNARDJONES_H

#ifndef BALL_MOLMEC_PARAMETER_FFPARAMETERSECTION_H
#	include <BALL/MOLMEC/PARAMETER/FFParameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL {
		
	class FFPSLennardJones 
		:	public FFParameterSection
	{
		public:

		enum FormatType
		{
			A_B_FORMAT,
			EPSILON_R_FORMAT
		};

		struct Values 
		{
			float A;
			float B;
		};

		struct LennardJones
		{
			Atom*		atom1;
			Atom*		atom2;
			Values	values;
		};


		/**	Default constructor.
		*/
		FFPSLennardJones();

		/**	Destructor.
		*/
		virtual ~FFPSLennardJones();

		/**	Destroy method.
		*/
		virtual void destroy();
		
		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		FFPSLennardJones::Values getParameters(Atom::Type I, Atom::Type J) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(FFPSLennardJones::Values& parameters, Atom::Type I, Atom::Type J) const;


		protected:

		Size									number_of_atom_types_;

		float*								A_;
		
		float*								B_;

		float*								Aij_;
		
		float*								Bij_;

		bool*									is_defined_;

		FormatType						format_;
			
		String*								names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_LENNARDJONES_H
