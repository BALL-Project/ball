// $Id: charmmEEF1.h,v 1.9 2001/09/25 11:43:47 anker Exp $
// Molecular Mechanics Parameter: class describing parameters needed 
// for the EEF1 solvation component in the CHARMM force field
 
#ifndef BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
#define BALL_MOLMEC_PARAMETER_CHARMMEEF1_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{

	/**	Force Field Parameter Section for CHARMM EEF1.
			This class reads and administers a parameter set of solvation
			parameters for the CHARMM/EEF1 force field (Lazaridis, Karplus, ???). \\
			{\bf Definition:} \URL{BALL/MOLMEC/PARAMETER/charmmEEF1.h}
	*/
	class CharmmEEF1 
		:	public ParameterSection
	{
		public:

		/**	BAUSTELLE
		*/
		struct Values 
		{
			/// BAUSTELLE
			float V;
			/// BAUSTELLE
			float dG_ref;
			/// BAUSTELLE
			float dG_free;
			/// BAUSTELLE
			float r_min;
			/// BAUSTELLE
			float sig_w;
			/// BAUSTELLE
			float dH_ref;
			/// BAUSTELLE
			float Cp_ref;
		};

		/** BAUSTELLE
		*/
		struct Data
		{
			/// BAUSTELLE
			Atom*		atom;
			/// BAUSTELLE
			Values	values;
		};


		/** @name Constructors and Destructor 
		*/
		//@{

		/**	Default constructor.  
		*/
		CharmmEEF1() throw();

		/** Copy constructor 
		*/
		CharmmEEF1(const CharmmEEF1& charmm_EEF1) throw();

		/**	Destructor.  
		*/
		virtual ~CharmmEEF1() throw();

		/**	Clear method.  
		*/
		virtual void clear() throw();

		//@}

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section_name from ini_file,
				interprets (if given) a format line, reads the data from this
				section according to the format, and builds some datastructures for
				fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, 
				const String& section_name) throw();

		/** BAUSTELLE
		*/
		virtual bool extractSection(Parameters& parameters, 
				const String& section_name) throw();

		/** Queries whether a parameter set is defined for the given atom type.
		*/
		bool hasParameters(Atom::Type I) const throw();
		
		/**	Returns the parameters for a given atom type combination.
		*/
		CharmmEEF1::Values getParameters(Atom::Type I) const throw();
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(CharmmEEF1::Values& parameters, Atom::Type I)
			const throw();

		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const CharmmEEF1& operator = (const CharmmEEF1& charmm_EEF1) throw();

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const CharmmEEF1& charmm_EEF1) const throw();

		//@}

		protected:

		//_ BAUSTELLE
		Size					number_of_atom_types_;

		//_ BAUSTELLE
		float*				V_;
		
		//_ BAUSTELLE
		float*				dG_ref_;
		
		//_ BAUSTELLE
		float*				dG_free_;

		//_ BAUSTELLE
		float*				dH_ref_;

		//_ BAUSTELLE
		float*				Cp_ref_;

		//_ BAUSTELLE
		float*				sig_w_;

		//_ BAUSTELLE
		float*				R_min_;
		
		//_ BAUSTELLE
		bool*					is_defined_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
