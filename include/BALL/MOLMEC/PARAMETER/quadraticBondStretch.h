// $Id: quadraticBondStretch.h,v 1.1 1999/08/26 07:53:21 oliver Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file
 
#ifndef BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
#define BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H

#ifndef BALL_MOLMEC_PARAMETER_FFPARAMETERSECTION_H
#	include <BALL/MOLMEC/PARAMETER/FFParameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL {
		
	class FFPSQuadraticBondStretch 
		:	public FFParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct Values 
		{
			float r0;
			float k;
		};

		struct QuadraticStretch
		{
			Atom*		atom1;
			Atom*		atom2;
			Values	values;
		};


		/**	Default constructor.
		*/
		FFPSQuadraticBondStretch(void);

		/**	Destructor.
		*/
		virtual ~FFPSQuadraticBondStretch(void);

		/**	Destroy method.
		*/
		virtual void destroy(void);
		
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
		FFPSQuadraticBondStretch::Values getParameters
			(Atom::Type I, Atom::Type J) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters
			(FFPSQuadraticBondStretch::Values& parameters, 
			 Atom::Type I, Atom::Type J) const;

		protected:

		Size									number_of_atom_types_;

		float*								k_;
		
		float*								r0_;

		bool*									is_defined_;
			
		String*								names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
