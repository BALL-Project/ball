// $Id: cosineTorsion.h,v 1.6 2000/10/05 17:34:15 anker Exp $
// Molecular Mechanics Parameter: class describing the parameter
// section required for torsions using a cosine type potential function
 
#ifndef BALL_MOLMEC_PARAMETER_COSINETORSION_H
#define BALL_MOLMEC_PARAMETER_COSINETORSION_H

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
	class CosineTorsion 
		:	public ParameterSection
	{
		public:

		enum 
		{
			UNKNOWN
		};

		struct SingleValues
		{
			float phase;
			float V;
			float f;
			float n;

			SingleValues(const SingleValues& v)
				:	phase(v.phase),
					V(v.V),
					f(v.f),
					n(v.n)
			{
			}
				
			SingleValues()
				: phase(0),
					V(0),
					f(0),
					n(0)
			{
			}
		};

		struct Values 
		{
			unsigned char 	n;
			SingleValues*		values;

			Values()
				: n(0),
					values(0)
			{
			}
			
			Values(Size number)
				: n((unsigned char)number),
					values(new SingleValues[number])
			{
			}
			
			Values(const Values& v)
			{
				n = v.n;
				values = new SingleValues[n];
				for (int i = 0; i < n; i++)
				{
					values[i].phase = v.values[i].phase;
					values[i].V = v.values[i].V;
					values[i].f = v.values[i].f;
					values[i].n = v.values[i].n;
				}
			}

			void set(const Values& v)
			{
				delete [] values;

				n = v.n;
				values = new SingleValues[n];
				for (int i = 0; i < n; i++)
				{
					values[i] = v.values[i];
				}
			}

			~Values()
			{
				delete [] values;
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

		struct SingleData
		{
			Atom*					atom1;
			Atom*					atom2;
			Atom*					atom3;
			Atom*					atom4;

			SingleValues	values;
		};

		/**	@name	Constructors and Destructors */	
		//@{

		/**	Default constructor.  */
		CosineTorsion();
		
		/** Copy constructor */
		CosineTorsion(const CosineTorsion& cosine_torsion);

		/**	Destructor.  */
		virtual ~CosineTorsion();

		//@}
		
		/**	@name	 Accessors
		*/
		//@{
		/**	Reads a parameter section from an INI file.
				This method reads the section given in section\_name from ini\_file,
				interprets (if given) a format line, reads the data from this section according to 
				the format, and builds some datastructures for fast and easy acces this data.
		*/
		virtual bool extractSection(ForceFieldParameters& parameters, const String& section_name);
		virtual bool extractSection(Parameters& parameters, const String& section_name);

		/** Queries whether a parameter set is defined for the given atom types.
		*/
		bool hasParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;
		
		/**	Returns the parameters for a given atom type combination.
		*/
		CosineTorsion::Values getParameters(Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;
		
		/**	Assign the parameters for a given atom type combination.
				If no parameters are defined for this combination, false is
				returned and nothing is changed.
		*/
		bool assignParameters(CosineTorsion::Values& parameters, Atom::Type I, Atom::Type J, Atom::Type K, Atom::Type L) const;
		//@}

		/** @name Assignment */
		//@{

		/**	Clear method.  */
		virtual void clear();

		/** Assignment operator */
		const CosineTorsion& operator = (const CosineTorsion& cosine_torsion);
		
		//@}


		/** @name Predicates */
		//@{

		/** Equality operator */
		bool operator == (const CosineTorsion& cosine_torsion) const;

		//@}


		protected:

		Size									number_of_atom_types_;

		vector<Values>				torsions_;
		
		HashMap<Size, Size>		torsion_hash_map_;
	};

} // namespace BALL

#endif // BALL_MOLMEC_PARAMETER_COSINETORSION_H
