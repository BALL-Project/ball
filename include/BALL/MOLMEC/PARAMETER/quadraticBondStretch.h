// $Id: quadraticBondStretch.h,v 1.8 2001/03/02 22:10:27 amoll Exp $
// Molecular Mechanics Parameter: class describing the parameters
// required for a harmonic stretch potential
 
#ifndef BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
#define BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
		
	/** QuadraticBondStretch.\\
			{\bf Definition:}\URL{BALL/MOLMEC/PARAMETER/quadraticBondStretch.h}	
	*/
	class QuadraticBondStretch 
		:	public ParameterSection
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

		struct Data
		{
			Atom*		atom1;
			Atom*		atom2;
			Values	values;
		};


		/**	Default constructor.
		*/
		QuadraticBondStretch();

		/**	Destructor.
		*/
		virtual ~QuadraticBondStretch() throw();

		/**	Clear method  
		*/
		virtual void clear() throw();

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);

		///
		virtual bool extractSection(Parameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		QuadraticBondStretch::Values getParameters
			(Atom::Type I, Atom::Type J) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters
			(QuadraticBondStretch::Values& parameters, 
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
