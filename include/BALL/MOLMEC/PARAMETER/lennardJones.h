// $Id: lennardJones.h,v 1.12 2000/10/05 08:31:00 oliver Exp $
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
			This section read parameters for a Lennard Jones potental (usually a 6-12 Potential).
			Parameters may we given in three different formats (no mixing of formats is allowed).
			\begin{itemize}
				\item {\em A} and {\em B} are given directly (\Ref{A_B_FORMAT})
				\item well depth and minimum radii (\Ref{EPSILON_R_FORMAT})
				\item Slater-Kirkwood parameters (\Ref{SLATER_KIRKWOOD_FORMAT})
			\end{itemize}

			If the Slater-Kirkwood format is used, the values for $A_{ij}$ and $B_{ij}$ are calculated
			as follows (see e.g. Brooks et al., J. Comput. Chem, 4(2):187-217 (1983))
			\[
				B_{ij} & = & \frac{3}{2} \left(\frac{1}{4 \pi \varepsilon_0}\right)^\frac{1}{2}
						\frac{e \hbar \sqrt{m_e} \alpha_i \alpha_j}{\sqrt{\frac{\alpha_i}{N_i} + \sqrt{\frac{\alpha_j}{N_j}}}}
			\]
			\[
				A_{ij} & = & \frac{1}2{} B_{ij} (R_i + R_j)^6
			\]
			\\
			{\bf Definition:}\URL{BALL/MOLMEC/PARAMETER/lennardJones.h}	
			\\
	*/
	class LennardJones 
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
		//@}

		/**	@name	Constructors and Destructors
		*/
		//@{

		/**	Default constructor.
		*/
		LennardJones();

		/**	Destructor.
		*/
		virtual ~LennardJones();

		/**	Destroy method.
		*/
		virtual void destroy();
		//@}
		
		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from
				ini\_file, interprets (if given) a format line, reads the data from
				this section according to the format, and builds some
				datastructures for fast and easy access to this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);
		virtual bool extractSection(Parameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		Values getParameters(Atom::Type I, Atom::Type J) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(Values& parameters, Atom::Type I, Atom::Type J) const;


		protected:

		Size									number_of_atom_types_;

		float*								A_;
		
		float*								B_;

		float*								N_;

		float*								Aij_;
		
		float*								Bij_;

		bool*									is_defined_;

		FormatType						format_;
			
		String*								names_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_LENNARDJONES_H
