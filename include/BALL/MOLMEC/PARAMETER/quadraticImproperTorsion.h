// $Id: quadraticImproperTorsion.h,v 1.5 2000/10/18 10:40:03 anker Exp $
// Molecular Mechanics Parameter: class describing the parameters required to
// describe a harmonic improper torsion (out-of-plane) potential
 
#ifndef BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
#define BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H

#ifndef BALL_FORMAT_PARAMETERSECTION_H
#	include <BALL/FORMAT/parameterSection.h>
#endif

#ifndef BALL_MOLMEC_PARAMETER_ATOMTYPES_H
#	include <BALL/MOLMEC/PARAMETER/atomTypes.h>
#endif

namespace BALL 
{
		
	/** missing documentation for QuadraticImproperTorsion
			\\
			{\bf Definition:} \URL{include/BALL/MOLMEC/PARAMETER/quadraticImproperTorsion.h}
	*/
	class QuadraticImproperTorsion 
		:	public ParameterSection
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

			bool operator == (const Values& values) const throw()
			{
				return ((phase == values.phase) && (k == values.k));
			}

			bool operator != (const Values& values) const throw()
			{
				return !(operator == (values));
			}

		};

		struct Data
		{
			Atom*		atom1;
			Atom*		atom2;
			Atom*		atom3;
			Atom*		atom4;

			Values	values;
		};

		BALL_CREATE(QuadraticImproperTorsion)

		/** @name Constructors and destructor */
		//@{

		/**	Default constructor.
		*/
		QuadraticImproperTorsion() throw();

		/** Copy constructor. */
		QuadraticImproperTorsion(const QuadraticImproperTorsion& torsion)
			throw();

		/**	Destructor.
		*/
		virtual ~QuadraticImproperTorsion() throw();

		//@}


		/** @name Parameter extraction */
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this
				section according to the format, and builds some datastructures for
				fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name) throw();
		virtual bool extractSection(Parameters& parameters,
				const String& section_name) throw();

		//@}


		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) 
			const throw();
		
		/**	Returns the parameters for a given atom type combination.
		*/
		QuadraticImproperTorsion::Values getParameters(Atom::Type I, Atom::Type J,
				Atom::Type K, Atom::Type L) const throw();
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(QuadraticImproperTorsion::Values& parameters,
				Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const
			throw();


		/** @name Assignment */
		//@{

		/**	Clear method. */
		virtual void clear() throw();

		/** Assignment operator */
		const QuadraticImproperTorsion& operator = 
			(const QuadraticImproperTorsion& torsion) throw();

		//@}
		

		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const QuadraticImproperTorsion& torsion) const
			throw();

		//@}


		protected:

		Size									number_of_atom_types_;

		vector<Values>				torsions_;
		
		HashMap<Size, Size>		torsion_hash_map_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
