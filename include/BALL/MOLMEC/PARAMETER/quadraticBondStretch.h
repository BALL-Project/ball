// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: quadraticBondStretch.h,v 1.18 2005/12/23 17:01:54 amoll Exp $
//
 
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
	/** QuadraticBondStretch.
			
			Molecular Mechanics Parameter: class describing the parameters
			required for a harmonic stretch potential.
    	
			\ingroup  MolmecParameters
	*/
	class BALL_EXPORT QuadraticBondStretch 
		:	public ParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct BALL_EXPORT Values 
		{
			float r0;
			float k;
		};

		struct BALL_EXPORT Data
		{
			Atom::StaticAtomAttributes*	atom1;
			Atom::StaticAtomAttributes*	atom2;
			Values	values;
		};


		/**	Default constructor.
		*/
		QuadraticBondStretch();

		/**	Destructor.
		*/
		virtual ~QuadraticBondStretch() ;

		/**	Clear method  
		*/
		virtual void clear() ;

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);

		///
		virtual bool extractSection(Parameters& parameters, const String& section_name);
	
		/** Write the parameters in Parm-file format
		 */
		virtual bool exportParmFile(File& outfile) const;

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
			
		ForceFieldParameters* force_field_parameters_; 

		String* 							comment_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
