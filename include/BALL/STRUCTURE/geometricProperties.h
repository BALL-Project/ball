// $Id: geometricProperties.h,v 1.10 2000/12/19 12:50:59 amoll Exp $

#ifndef BALL_STRUCTURE_GEOMETRICPROPERTIES_H
#define BALL_STRUCTURE_GEOMETRICPROPERTIES_H

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#ifndef BALL_MATHS_BOX3_H
#	include <BALL/MATHS/box3.h>
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

using std::vector;

namespace BALL 
{

	/**	@name	Geometric property processors
			@nonames
			The applicators, processors, and collectors described in 
			this chapter are used to extract geometric properties out
			of a given molecular object or to extract parts of these
			objects according to their geometric properties.\\
			Using the \Ref{BoundingBoxProcessor}, the bounding box 
			of a given molecular object can be calculated. THe bounding box is
			represented by the lowest and highest coordinates occuring in the 
			molecular object, i.e. the bounding box is the smallest rectangular
			box (with sides parallel to the coordinate axes) that encloses all
			atoms in the molecular object.\\
			The \Ref{GeometricCenterProcessor} calculates the geometric 
			center of all atoms contained in the molecular object it is applied to.\\
			With the aid of the \Ref{FragmentDistanceCollector} it is possible
			to collect all molecular fragments that are within a given distance
			from a certain fragment. This is useful to extract the relevant molecular
			environment (e.g. to examin a binding site).\\
	*/
	//@{

	/**	Bounding box creating processor.
			This class iterates over all atoms of a given molecular object and
			determines the lowest and the highest coordinates occuring. It returns
			Two coordinates (\Ref{getLower}, \Ref{getUpper}) describing the smallest
			cuboid (whose sides are parallel to the planes defined by the corrdinate
			axes) enclosing all atoms of the molecular object.\\
			This processor is useful to determine the extent of a molecular object
			if you want to define a \Ref{THashGrid} or alike objects.\\
			The coordinates returned by \Ref{getLower} and \Ref{getUpper} are only
			valid, if the processor has been applied to a molecular object containing
			atoms.\\
			{\bf Definition:} \URL{BALL/STRUCTURE/geometricProperties.h}
			\\
	*/
	class BoundingBoxProcessor
		:	public UnaryProcessor<Atom>
	{
		public:

		/** @name Processor related methods.
		*/
		//@{
			
		/**
		*/
		virtual bool start()
			throw();

		/**
		*/
		virtual bool finish()
			throw();

		/**
		*/
		virtual Processor::Result operator () (Atom& atom)
			throw();
		//@}

		/**	@name Accessors
		*/
		//@{
		/** Return the bounding box
		*/
		Box3 getBox() const
			throw();

		/**	Returns the lower corner of the bounding box
		*/
		const Vector3& getLower() const
			throw();

		/**	Returns the upper corner of the bounding box
		*/
		const Vector3& getUpper() const
			throw();
		//@}
			
		private:

		Vector3	lower_;
		Vector3	upper_;
	};

	/**	Calculates the geometric center of a given Composite object.
			This processor calculates the geometric center of the atom coordinates
			of a given molecular object.\\
			The geometric center is calculated as follows:\\
			\[
				\vec{C} = \frac{1}{N} \sum_{i}{N} \vec{r_i}
			\]
			Where $\vec{r_i}$ represents the coordinates of the ith atom.\\
			{\bf Definition:} \URL{BALL/STRUCTURE/geometricProperties.h}
			\\
	*/
	class GeometricCenterProcessor
		:	public UnaryProcessor<Atom> 
	{
		public:

		/**	@name Processor related methods
		*/
		//@{

		/**
		*/
		virtual bool start()
			throw();

		/**
		*/
		virtual bool finish()
			throw();

		/**
		*/
		virtual Processor::Result operator()(Atom& atom)
			throw();
		//@}

		/**@name	Accessors
		*/
		//@{
		/**	Returns the center of the object
		*/
		Vector3& getCenter()
			throw();
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
			reference fragment, the whole fragment is collected in an array.\\
			The reference fragment itself is also contained in this array, if it is part
			of the molecular object the collector is applied to.\\
			The array only contains pointers to the fragments, the fragments are neither 
			changed, nor remove from the molecular object.\\
			The reference fragment may either be given by a specialized constructor (also
			together with the distance) or using \Ref{setFragment}.\\
			The fragment array is emptied prior to each collection run.\\
			{\bf Definition:} \URL{BALL/STRUCTURE/geometricProperties.h}
			\\
	*/
	class FragmentDistanceCollector
		: public UnaryProcessor<Composite> 
	{		
		public:

		/** @name Constructors and Destructors 
		*/
		//@{

		/**	Default constructor
		*/
		FragmentDistanceCollector()
			throw();

		/**	Constructor.
				Creates a new collector and sets the reference composite/
				@param	composite the reference composite
		*/
		FragmentDistanceCollector(const Composite& composite)
			throw();

		/**	Constructor.
				Creates a new collector and sets the reference composite and the distance.
				@param	composite the reference composite
				@param	distance the maximum distance between any two atoms
		*/
		FragmentDistanceCollector(const Composite& composite, float distance)
			throw();
			
		virtual ~FragmentDistanceCollector()
			throw()
		{}	
			
		//@}

		/**	@name	Processor related methods
		*/
		//@{

		/**
		*/
		virtual bool start()
			throw();

		/**
		*/
		virtual bool finish()
			throw();

		/**
		*/
		virtual Processor::Result operator()(Composite& composite)
			throw();
		//@}
		
		/**	@name Accessors
		*/
		//@{
		/**	Returns the number of molecular fragments found
				@return	the number of fragments in the array
		*/
		Size getNumberOfFragments()
			throw();

		/**	Sets the reference composite
				@param	composite the new reference composite
		*/
		void setComposite(const Composite& composite)
			throw();

		/**	Gets the reference composite
				@return a const pointer to the reference composite
		*/
		const Composite* getComposite() const
			throw();

		/**	Gets the maximum distance
				@return the maximum distance
		*/
		float getDistance() const
			throw();
		
		/**	Sets the maximum distance
				@param	distance the new maximum distance 
		*/
		void setDistance(float distance)
			throw();
		//@}
		

		/**	The array containing all molecular fragments collected
		*/
		vector<Fragment*>	fragments;


		protected:

			vector<Fragment*>	all_fragments_;
			const Composite*	reference_composite_;
			float							squared_distance_;
	};


	//@}


	/**	@name	
	*/
	//@{
		
	/**	Calculate the torsion angle between four atoms
	*/
	Angle calculateTorsionAngle(const Atom& a1, const Atom& a2, const Atom& a3, const Atom& a4)
		throw(Exception::DivisionByZero);

	/**	Calculate the bond angle between three atoms
	*/
	Angle calculateBondAngle(const Atom& a1, const Atom& a2, const Atom& a3)
		throw(Exception::DivisionByZero);
	//@}

} // namespace BALL

#endif // BALL_STRUCTURE_GEOMETRICPROPERTIES_H
