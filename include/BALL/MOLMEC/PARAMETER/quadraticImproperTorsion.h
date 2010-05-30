// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

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
    /**  \addtogroup  MolmecParameters
     *  @{
     */		
	/** QuadraticImproperTorsion.
			
	*/
	class BALL_EXPORT QuadraticImproperTorsion 
		:	public ParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct BALL_EXPORT Values
		{
			float phase;
			float k;

			bool operator == (const Values& values) const 
			{
				return ((phase == values.phase) && (k == values.k));
			}

			bool operator != (const Values& values) const 
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

		/** @name Constructors and destructor 
		*/
		//@{

		/**	Default constructor.
		*/
		QuadraticImproperTorsion() ;

		/** Copy constructor. 
		*/
		QuadraticImproperTorsion(const QuadraticImproperTorsion& torsion)
			;

		/**	Destructor.
		*/
		virtual ~QuadraticImproperTorsion() ;

		/**	Clear method. 
		*/
		virtual void clear() ;

		//@}
		/** @name Parameter extraction 
		*/
		//@{

		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this
				section according to the format, and builds some datastructures for
				fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters,
				const String& section_name) ;

		///
		virtual bool extractSection(Parameters& parameters,
				const String& section_name) ;

		//@}

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) 
			const ;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		QuadraticImproperTorsion::Values getParameters(Atom::Type I, Atom::Type J,
				Atom::Type K, Atom::Type L) const ;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(QuadraticImproperTorsion::Values& parameters,
				Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const
			;

		/** @name Assignment 
		*/
		//@{

		/** Assignment operator 
		*/
		const QuadraticImproperTorsion& operator = 
			(const QuadraticImproperTorsion& torsion) ;

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Equality operator 
		*/
		bool operator == (const QuadraticImproperTorsion& torsion) const
			;

		//@}

		protected:

		Size									number_of_atom_types_;

		vector<Values>				torsions_;
		
		HashMap<Size, Size>		torsion_hash_map_;
	};
  /** @} */
} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_QUADRATICIMPROPERTORSION_H
