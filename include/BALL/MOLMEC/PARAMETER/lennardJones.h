// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

// Molecular Mechanics Parameter: class describing the atom type section of a parameter file
 
#ifndef BALL_MOLMEC_PARAMETER_LENNARDJONES_H
#define BALL_MOLMEC_PARAMETER_LENNARDJONES_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
	/**	Lennard Jones parameter section.
			This section reads parameters for a Lennard Jones potental (usually a
			6-12 Potential). Parameters may be given in three different formats
			(no mixing of formats is allowed).
				- <b> A </b> and <b> B </b> are given directly ( \link A_B_FORMAT A_B_FORMAT \endlink )
				- well depth and minimum radii ( \link EPSILON_R_FORMAT EPSILON_R_FORMAT \endlink )
				- Slater-Kirkwood parameters ( \link SLATER_KIRKWOOD_FORMAT SLATER_KIRKWOOD_FORMAT \endlink )

			If the Slater-Kirkwood format is used, the values for \f$A_{ij}\f$ and \f$B_{ij}\f$ are calculated
			as follows (see e.g. Brooks et al., J. Comput. Chem, 4(2):187-217 (1983))
			\f[
				B_{ij} = \frac{3}{2} \left(\frac{1}{4 \pi \varepsilon_0}\right)^\frac{1}{2}
						\frac{e \hbar \sqrt{m_e} \alpha_i \alpha_j}{\sqrt{\frac{\alpha_i}{N_i} + \sqrt{\frac{\alpha_j}{N_j}}}} 
			\f]
			\par
			\f[
				A_{ij} = \frac{1}2{} B_{ij} (R_i + R_j)^6
			\f]
			 \par
			
    	\ingroup  MolmecParameters
	*/
	class BALL_EXPORT LennardJones 
		:	public ParameterSection
	{
		public:
	
		/**	@name	Enums
		*/
		//@{

		enum FormatType
		{
			A_B_FORMAT,
			EPSILON_R_FORMAT,
			SLATER_KIRKWOOD_FORMAT
		};
		
		//@}
		/**	@name	Type definitions
		*/
		//@{

		/**
		*/
		struct BALL_EXPORT Values 
		{
			float A;
			float B;
		};

		struct BALL_EXPORT Data
		{
			Atom* atom1;
			Atom* atom2;
			Values	 values;
		};

		//@}


		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		LennardJones() ;

		/** Copy constructor. 
		*/
		LennardJones(const LennardJones& lj) ;

		/**	Destructor.
		*/
		virtual ~LennardJones() ;

		/**	Clear method. 
		*/
		virtual void clear() ;

		//@}
		
		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from
				ini\_file, interprets (if given) a format line, reads the data from
				this section according to the format, and builds some
				datastructures for fast and easy access to this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, 
				const String& section_name) ;

		///
		virtual bool extractSection(Parameters& parameters, 
				const String& section_name) ;

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J) const ;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		Values getParameters(Atom::Type I, Atom::Type J) const ;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(Values& parameters, Atom::Type I, Atom::Type J)
			const ;

		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const LennardJones& operator = (const LennardJones& lj) ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const LennardJones& lj) const ;

		//@}

		protected:

		Size									number_of_atom_types_;

		std::vector<float>		A_;
		
		std::vector<float>		B_;

		std::vector<float>		N_;

		std::vector<float>		Aij_;
		
		std::vector<float>		Bij_;

		std::vector<bool>			is_defined_;

		FormatType						format_;
			
		std::vector<String>		names_;
	};
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_LENNARDJONES_H
