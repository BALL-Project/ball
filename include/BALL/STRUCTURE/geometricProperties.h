// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#define BALL_STRUCTURE_GEOMETRICPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_SIMPLEBOX3_H
#	include <BALL/MATHS/simpleBox3.h>
#endif

#ifndef BALL_KERNEL_ATOM_H
#	include <BALL/KERNEL/atom.h>
#endif

#ifndef BALL_KERNEL_FRAGMENT_H
#	include <BALL/KERNEL/fragment.h>
#endif

#ifndef BALL_CONCEPT_PROCESSOR_H
#	include <BALL/CONCEPT/processor.h>
#endif

#ifndef BALL_DATATYPE_STRING_H
#	include <BALL/DATATYPE/string.h>
#endif

#include <vector>

namespace BALL 
{

	/**	Bounding box creating processor.
			This class iterates over all atoms of a given molecular object and
			determines the lowest and the highest coordinates occuring. It returns
			two coordinates ( \link getLower getLower \endlink ,  \link getUpper getUpper \endlink ) describing the smallest
			cuboid (whose sides are parallel to the planes defined by the corrdinate
			axes) enclosing all atoms of the molecular object. \par
			This processor is useful to determine the extent of a molecular object
			if you want to define a  \link THashGrid THashGrid \endlink  or alike objects. \par
			The coordinates returned by  \link getLower getLower \endlink  and  \link getUpper getUpper \endlink  are only
			valid, if the processor has been applied to a molecular object containing
			atoms. \par
			
			 \par
			\ingroup StructureGeometric
	*/
	class BALL_EXPORT BoundingBoxProcessor
		: public ConstUnaryProcessor<Atom>
	{
		public:

		/** @name Processor related methods.
		*/
		//@{
			
		/**
		 * Initialize the processor. Resets lower and upper vectors to lower
		 * and upper numeric float limits.
		 *
		 * \see UnaryProcessor::start()
		 *
		 * \return true in all cases
		*/
		virtual bool start();

		/**
		 * Checks if bounding box has been calculated.
		 *
		 * \see UnaryProcessor::finish()
		 *
		 * \return true if bounding box has been calculated, otherwise reset lower and upper to (0,0,0) and return false
		*/
		virtual bool finish();

		/**
		 * Contributes the coordinates of provided \ref Atom to the bounding box.
		 * The function internally calls the more generic \ref operator() with
		 * \ref Vector3 argument derived from atom position.
		*/
		virtual Processor::Result operator () (const Atom& atom)
			 { return operator() (atom.getPosition());}

		/**
		 * Contributes coordinates of \ref Vector3 argument to the bounding box.
		*/
		virtual Processor::Result operator () (const Vector3& v);


		//@}
		/**	@name Accessors
		*/
		//@{

		/** Return the bounding box
		*/
		SimpleBox3 getBox() const;

		/**	Returns the lower corner of the bounding box
		*/
		const Vector3& getLower() const;

		/**	Returns the upper corner of the bounding box
		*/
		const Vector3& getUpper() const;

		//@}
			
		private:

		Vector3	lower_;
		Vector3	upper_;
	};

	/**	Calculates the geometric center of a given Composite object.
			This processor calculates the geometric center of the atom coordinates
			of a given molecular object. \par
			The geometric center is calculated as follows: \par
			\[
				\vec{C} = \frac{1}{N} \sum_{i}{N} \vec{r_i}
			\]
			Where $\vec{r_i}$ represents the coordinates of the ith atom. \par
			\ingroup StructureGeometric
	*/
	class BALL_EXPORT GeometricCenterProcessor
		:	public ConstUnaryProcessor<Atom> 
	{
		public:

		/**	@name Processor related methods
		*/
		//@{

		/**
		*/
		virtual bool start()
			;

		/**
		*/
		virtual bool finish()
			;

		/**
		*/
		virtual Processor::Result operator()(const Atom& atom)
			 { return operator()(atom.getPosition());}

		/**
		*/
		virtual Processor::Result operator()(const Vector3& v)
			;

		//@}
		/**@name	Accessors
		*/
		//@{

		/**	Returns the center of the object
		*/
		Vector3& getCenter()
			;

		//@}

		private:

		Vector3	center_;
		Size		n_;
	};


	/**	Collects all MolecularFragments that are close enough to another 
			molecular fragment.
			This processor examines the distances between every atom of a given fragment
			(further referred to as the reference fragment) and all other atoms in a molecular 
			object he is applied to. If any atom of a fragment is closer to any atom of the
			reference fragment, the whole fragment is collected in an array. \par
			The reference fragment itself is also contained in this array, if it is part
			of the molecular object the collector is applied to. \par
			The array only contains pointers to the fragments, the fragments are neither 
			changed, nor removed from the molecular object. \par
			The reference fragment may either be given by a specialized constructor (also
			together with the distance) or using  \link setFragment setFragment \endlink . \par
			The fragment array is emptied prior to each collection run. \par
			
			 \par
	\ingroup StructureGeometric
	*/
	class BALL_EXPORT FragmentDistanceCollector
		: public ConstUnaryProcessor<Composite>
	{
		public:

		/** @name Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		FragmentDistanceCollector()
			;

		/**	Constructor.
				Creates a new collector and sets the reference composite
				@param	composite the reference composite
		*/
		FragmentDistanceCollector(const Composite& composite)
			;

		/**	Constructor.
				Creates a new collector and sets the reference composite and the distance.
				@param	composite the reference composite
				@param	distance the maximum distance between any two atoms
		*/
		FragmentDistanceCollector(const Composite& composite, float distance)
			;
			
		virtual ~FragmentDistanceCollector()
			
		{}	
			
		//@}
		/**	@name	Processor related methods
		*/
		//@{

		/**
		*/
		virtual bool start()
			;

		/**
		*/
		virtual bool finish()
			;

		/**
		*/
		virtual Processor::Result operator()(const Composite& composite);

		//@}
		/**	@name Accessors
		*/
		//@{

		/**	Returns the number of molecular fragments found
				@return	the number of fragments in the array
		*/
		Size getNumberOfFragments()
			;

		/**	Sets the reference composite
				@param	composite the new reference composite
		*/
		void setComposite(const Composite& composite)
			;

		/**	Gets the reference composite
				@return a const pointer to the reference composite
		*/
		const Composite* getComposite() const
			;

		/**	Gets the maximum distance
				@return the maximum distance
		*/
		float getDistance() const
			;
		
		/**	Sets the maximum distance
				@param	distance the new maximum distance 
		*/
		void setDistance(float distance)
			;

		//@}
		
		/**	The array containing all molecular fragments collected
		*/
		std::vector<const Fragment*> fragments;


		protected:

		std::vector<const Fragment*>	all_fragments_;
		const Composite*	reference_composite_;
		float							squared_distance_;
	};


	//@}
	/**	@name	Angle Calculation
	\ingroup StructureGeometric
	*/
	//@{
		
	/**	Calculate the torsion angle between four atoms
	*/
	BALL_EXPORT Angle calculateTorsionAngle(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4)
		throw(Exception::IllegalPosition);

	/**
	 * Set the torsion angle defined by a1, a2, a3, a4. The atoms should be connected by bonds
	 * The position of a1 and a2 is kept fix, while the connected component
	 * containing a4 is rotated accordingly.
	 *
	 * @return false if the bond is not rotatable or if the molecule graph is not divided
	 *         into two connected components upon removal of the bond (a2,a3)
	 */
	BALL_EXPORT bool setTorsionAngle(const Atom& a1, const Atom& a2, Atom& a3, const Atom& a4, Angle angle);

	/**	Calculate the bond angle between three atoms
	*/
	BALL_EXPORT Angle calculateBondAngle(const Atom& a1, const Atom& a2, const Atom& a3)
		throw(Exception::IllegalPosition);

	//@}
} // namespace BALL

#endif // BALL_STRUCTURE_GEOMETRICPROPERTIES_H
