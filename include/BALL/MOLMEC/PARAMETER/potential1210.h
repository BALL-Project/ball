// $Id: potential1210.h,v 1.7 2000/10/18 10:40:03 anker Exp $
// Molecular Mechanics Parameter: class describing the parameters required
// for a 12-10 (hydrogen bond) potential
 
#ifndef BALL_MOLMEC_PARAMETER_POTENTIAL1210_H
#define BALL_MOLMEC_PARAMETER_POTENTIAL1210_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
		
	/**	
	*/
	class Potential1210 
		:	public ParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct Values 
		{
			float A;
			float B;
		};

		struct Data
		{
			Atom*		atom1;
			Atom*		atom2;
			Values	values;
		};


		/** @name Constructors and Destructor. */
		//@{

		/**	Default constructor.
		*/
		Potential1210() throw();

		/** Copy constructor. */
		Potential1210(const Potential1210& pot1210) throw();

		/**	Destructor.
		*/
		virtual ~Potential1210() throw();
		
		//@}


		/** @name Parameter extraction */
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, 
				const String& section_name) throw();
		virtual bool extractSection(Parameters& parameters, 
				const String& section_name) throw();

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J) const throw();
		
		/**	Returns the parameters for a given atom type combination.
		*/
		Potential1210::Values getParameters
			(Atom::Type I, Atom::Type J) const throw();
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters
			(Potential1210::Values& parameters, 
			 Atom::Type I, Atom::Type J) const throw();

		//@}


		/** @name Assignment */
		//@{

		/** Assignment operator */
		const Potential1210& operator = (const Potential1210& pot1210) throw();

		/**	Clear method. */
		virtual void clear() throw();

		//@}

		
		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const Potential1210& pot1210) const throw();

		//@}


		protected:

		Size									number_of_atom_types_;

		std::vector<float>		A_;
		
		std::vector<float>		B_;

		std::vector<bool>			is_defined_;
			
		std::vector<String>		names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_POTENTIAL1210_H
