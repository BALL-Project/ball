// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
 
#ifndef BALL_MOLMEC_PARAMETER_QUADRATICANGLEBEND_H
#define BALL_MOLMEC_PARAMETER_QUADRATICANGLEBEND_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

namespace BALL 
{
	/** QuadraticAngleBend.
			Molecular Mechanics Parameter: class describing the parameters required to
			implement a harmonic potential describing angle bends. \par
			
    	\ingroup  MolmecParameters
	*/
	class BALL_EXPORT QuadraticAngleBend 
		:	public ParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct BALL_EXPORT Values 
		{
			float theta0;
			float k;
		};

		struct BALL_EXPORT Data
		{
			Atom* atom1;
			Atom* atom2;
			Atom* atom3;
			Values values;
		};


		/**	Default constructor.
		*/
		QuadraticAngleBend();

		/**	Destructor.
		*/
		virtual ~QuadraticAngleBend() ;

		/**	Clear method. 
		*/
		virtual void clear() ;

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);
		virtual bool extractSection(Parameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J, Atom::Type K) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		QuadraticAngleBend::Values getParameters
			(Atom::Type I, Atom::Type J, Atom::Type K) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters
			(QuadraticAngleBend::Values& parameters, 
			 Atom::Type I, Atom::Type J, Atom::Type K) const;
		
		protected:

		Size						number_of_atom_types_;

		vector<Values>	values_;

		vector<Index>		value_index_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICBONDSTRETCH_H
