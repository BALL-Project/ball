// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
#define BALL_MATHS_PIECEWISEFUNCTION_H

#ifndef BALL_COMMON_H
# include <BALL/common.h>
#endif

#include <limits>

namespace BALL
{
	/** Every piece of a piecewise function needs a bunch of coefficients 
  \ingroup FunctionClasses
	*/
	typedef std::vector<double> Coefficients;

	/** An interval is defined by its limits 
	*/
	typedef std::pair<double,double> Interval;

	/** Piecewise function object.
			This class provides the interface for piecewise functions needed as
			representation of radial distribution functions (@see RadialDistributionFunction). 
			It implements the <tt>Function</tt> interface. \par
			Note that intervals <b>  must </b> be disjunct and interval limits have to
			meet. We require the intervals to be sorted such that the lowest interval
			limit is the first interval of the vector. \par
			
	 */
	class BALL_EXPORT PiecewiseFunction
	{
		public:

		BALL_CREATE(PiecewiseFunction)

		/** @name Constructors and Destructors 
		*/
		//@{

		/** Default constructor 
		*/
		PiecewiseFunction() ;

		/** Copy constructor 
		*/
		PiecewiseFunction(const PiecewiseFunction& function) ;

		/** Detailed constructor 
		*/
		PiecewiseFunction(const std::vector<Interval>& intervals, 
			const std::vector<Coefficients>& coeffs) ;

		/** Destructor 
		*/
		virtual ~PiecewiseFunction() ;

		//@}
		/** @name Assignment 
		*/
		//@{

		/** Assignemnt operator 
		*/
		PiecewiseFunction& operator = (const PiecewiseFunction& function) ;

		/** Clear function 
		*/
		void clear() ;

		//@}
		/** @name Accessors 
		*/
		//@{

		/** Set the intervals for the piecewise definition.
				Note that this method does <b>not</b> check the definition of the
				intervals for sanity.
		 */
		void setIntervals(const std::vector<Interval>& intervals) ;

		/** Get all the intervals 
		*/
		const std::vector<Interval>& getIntervals() const ;

		/** Get the interval a given x belongs to 
		 *  @throw Exception::OutOfRange if x is out of range
		 */
		const Interval& getInterval(double x) const;

		/** Get interval limits by index 
		 *  @throw Exception::IndexOverflow if index >= intervals_.size()
		 */
		const Interval& getInterval(Position index) const;

		/** Get the interval index for a given x 
		 *  @throw Exception::OutOfRange if x is out of range
		 */
		Position getIntervalIndex(double x) const;

		/** Return the range of the definition 
		*/
		const Interval& getRange() const;

		/** Set the coefficients.
				Note that this method does <b>not</b> check the vector of coefficients
				for sanity.
		 */
		void setCoefficients(const vector<Coefficients>& coefficients) ;

		/** */
		const std::vector<Coefficients>& getCoefficients() const ;

		/** Get the coefficients for a given x 
		 *  @throw Exception::OutOfRange if x is out of range
		 */
		const Coefficients& getCoefficients(double x) const;

		/** Get the coefficients from index 
		 *  @throw Exception::IndexOverflow if index >= intervals_.size()
		 */
		const Coefficients& getCoefficients(Position index) const;
		
		/** compute the value of the piecewise function data for a given x 
		*/
		virtual double operator () (double x) const;

		/** */
		void set(const std::vector<Interval>& intervals,
				const std::vector<Coefficients>& coeffs);

		//@}
		/** @name Predicates 
		*/
		//@{

		/** Check whether a given x is in the range of definition 
		*/
		bool isInRange(double x) const;

		/** check validity of the definition 
		*/
		virtual bool isValid() const;

		/** Equality operator 
		*/
		bool operator == (const PiecewiseFunction& function) const;

		//@}
		/** @name Debugging and Diagnostics 
		*/
		//@{

		/** 	Dumps the whole content of the object 
		*/
		virtual void dump (std::ostream& s = std::cout, Size depth = 0) const;

		//@}

		protected:

		/*_ This vector contains the intervals of the representation 
		*/
		std::vector<Interval> intervals_;

		/*_ This vector stores the coefficients for each interval 
		*/
		std::vector<Coefficients> coefficients_;

		bool valid_;


		private:

		/*_ The range of the defnition, needed for isInRange() and getRange() 
		*/
		Interval range_;

		/*_ Set the internal range fields 
		*/
		void calculateRange();

	};
}

#endif
