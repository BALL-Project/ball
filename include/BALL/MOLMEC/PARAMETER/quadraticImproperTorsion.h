// $Id: quadraticImproperTorsion.h,v 1.1 2000/02/06 20:00:10 oliver Exp $
// Molecular Mechanics Parameter: class describing the atom type section of a parameter file
 
#ifndef BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
#define BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H

#ifndef BALL_MOLMEC_PARAMETER_FFPARAMETERSECTION_H
#	include <BALL/MOLMEC/PARAMETER/FFParameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
		
	class FFPSQuadraticImproperTorsion 
		:	public FFParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct Values
		{
			float phase;
			float k;
		};

		struct Torsion
		{
			Atom*		atom1;
			Atom*		atom2;
			Atom*		atom3;
			Atom*		atom4;

			Values	values;
		};

		/**	Default constructor.
		*/
		FFPSQuadraticImproperTorsion();

		/**	Destructor.
		*/
		virtual ~FFPSQuadraticImproperTorsion();

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
		bool hasParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		FFPSQuadraticImproperTorsion::Values getParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(FFPSQuadraticImproperTorsion::Values& parameters, Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;

		protected:

		Size									number_of_atom_types_;

		vector<Values>				torsions_;
		
		HashMap<Size, Size>		torsion_hash_map_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
