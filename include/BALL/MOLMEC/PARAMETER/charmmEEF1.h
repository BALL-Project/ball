// $Id: charmmEEF1.h,v 1.3 2000/02/06 19:49:03 oliver Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file
 
#ifndef BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
#define BALL_MOLMEC_PARAMETER_CHARMMEEF1_H

#ifndef BALL_MOLMEC_PARAMETER_FFPARAMETERSECTION_H
#	include <BALL/MOLMEC/PARAMETER/FFParameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{

	/**	Force Field Parameter Section for CHARMm EEF1.
			This class reads and administers a aparameter set of solvation
			parameters for the CHARMm/EEF1 force field (Lazaridis, Karplus, ???).
	*/
	class FFPSCharmmEEF1 
		:	public FFParameterSection
	{
		public:

		/**	
		*/
		struct Values 
		{
			float V;
			float dG_ref;
			float dG_free;
			float r_min;
			float sig_w;
			float dH_ref;
			float Cp_ref;
		};

		/**
		*/
		struct EEF1
		{
			Atom*		atom;
			Values	values;
		};


		/**	Default constructor.
		*/
		FFPSCharmmEEF1();

		/**	Destructor.
		*/
		virtual ~FFPSCharmmEEF1();

		/**	Destroy method.
		*/
		virtual void destroy();
		
		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom type.
		*/
		bool hasParameters(Atom::Type I) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		FFPSCharmmEEF1::Values getParameters(Atom::Type I) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(FFPSCharmmEEF1::Values& parameters, Atom::Type I) const;


		protected:

		Size									number_of_atom_types_;

		float*								V_;
		
		float*								dG_ref_;
		
		float*								dG_free_;

		float*								dH_ref_;

		float*								Cp_ref_;

		float*								sig_w_;

		float*								R_min_;
		
		bool*									is_defined_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_CHARMMEEF1_H
