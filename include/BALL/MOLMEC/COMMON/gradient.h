// $Id: gradient.h,v 1.10 2001/12/30 13:28:39 sturm Exp $ 
// A conjugate gradient minimizer for geometry optimisation

#ifndef BALL_MOLMEC_COMMON_GRADIENT_H 
#define BALL_MOLMEC_COMMON_GRADIENT_H 

#ifndef BALL_COMMON_H
#	include <BALL/common.h>
#endif

#ifndef BALL_MATHS_VECTOR3_H
#	include <BALL/MATHS/vector3.h>
#endif

#include <vector>

namespace BALL 
{ 

	class AtomVector;

	/**	Gradient class.
			This class is used in the Minimizers to represent gradients and
			directions. 
			These gradients are extracted from a vector of atom pointers, if the
			forces of the atoms are correctly set (e.g. using ForceField::updateForces()).
			The gradient contains the negative forces from each atom. Forces are converted from Newton (N)
			to units of $kJ/(mol \AA)$.
			A gradient may be invalidated by calling \Ref{invalidate}. This method only
			flags the gradient as invalid, the data is untouched. This may be used to 
			initiate automatic recalculations of the gradient.
			This class is used by all minimizer classes. \\
			{\bf Definition:}\URL{BALL/MOLMEC/COMMON/gradient.h}
	*/
	class Gradient
		:	private std::vector<Vector3>
	{
    public:

    BALL_CREATE_DEEP(Gradient)

		/**	Type definitions
		*/
		//@{

		/**	Iterator type
		*/
		typedef std::vector<Vector3>::iterator Iterator;

		/**	Const iterator type
		*/
		typedef std::vector<Vector3>::const_iterator ConstIterator;

		//@}
    /**	@name	Constructors and Destructors	
    */
    //@{

    /**	Default constructor.
    */
    Gradient();

    /**	Construct from atom forces
    */
    Gradient(const AtomVector& atoms);

    /**	Copy constructor
    */
    Gradient(const Gradient& gradient, bool deep = true);

    /**	Destructor.
    */
    virtual ~Gradient();

    //@}
    /**	@name	Assignments 
    */
    //@{

    /**	Assignment operator
    */
    Gradient& operator = (const Gradient& rhs);

		/**	Assign from an array of atom pointers.
		*/
    Gradient& operator = (const AtomVector& rhs);

    /**	Assign from another gradient
    */
    void set(const Gradient& gradient);

		/**	Assign from an array of atom pointers.
		*/
    void set(const AtomVector& atoms);

		/**	Negate the gradient.
				Iterate over all force vectors of the gradient and
				invert the signs.
		*/
		void negate();

    /** Normalize the gradient.
				Rescale to unity length
    */
    void normalize();

		/**	Dot product operator
				@exception InvalidRange if the two gradients have different sizes
		*/
		double operator * (const Gradient& gradient) const;

    //@}
    /**	@name	Accessors 
    */
    //@{

		/*	Return the number of components of the gradient.
		*/
		using std::vector<Vector3>::size;

    /* Return the component for an atom.
    */
		//?????: GCC3 using std::vector<Vector3>::operator [];
		const Vector3& operator [] (int i) const { return std::vector<Vector3>::operator [] (i); }
		Vector3& operator [] (int i) { return std::vector<Vector3>::operator [] (i); }

		/**	Invalidate the gradient.
		*/	
		void invalidate();

		/**	Return the validity flag.
		*/
		bool isValid() const;

    //@}
		/**	@name	Iteration
		*/
		//@{
			
		/**	Return an iterator to the begining of the vector
		*/
		//?????: GCC3 using std::vector<Vector3>::begin;
		Iterator begin() { return vector<Vector3>::begin(); }
		ConstIterator begin() const { return vector<Vector3>::begin(); }

		/**	Return a past-the-end vector.
		*/
		//?????: GCC3 using std::vector<Vector3>::end;
		Iterator end() { return vector<Vector3>::end(); }
		ConstIterator end() const { return vector<Vector3>::end(); }

		//@}
    /**	@name	Public Attributes
    */
    //@{

		/**	The gradient norm.
		*/
		double norm;

		/**	The inverse of the gradient norm.
		*/
		double inv_norm;

		/**	The root mean square of the gradient.
		*/
		double rms;

    //@}

		protected:
			
		/*_	The validity flag.
		*/
		bool valid_;
		
    };

  } // end of namespace BALL

#endif // BALL_MOLMEC_COMMON_GRADIENT_H 
