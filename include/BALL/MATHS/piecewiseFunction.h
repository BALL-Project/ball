// $Id: piecewiseFunction.h,v 1.1 2000/08/31 12:19:39 anker Exp $

#ifndef BALL_MATHS_PIECEWISEFUNCTION_H
#define BALL_MATHS_PIECEWISEFUNCTION_H

#ifndef BALL_COMMON_H
#include <BALL/common.h>
#endif

#ifndef BALL_COMMON_LIMITS_H
#include <BALL/COMMON/limits.h>
#endif

namespace BALL
{

	/** Every piece of a piecewise function needs a bunch of coefficients */

	typedef std::vector<double> Coefficients;
	// BAUSTELLE: Nicht schön.
	static const Coefficients INVALID_COEFFICIENTS;

	/** An interval is defined by its limits */
	
	typedef std::pair<double,double> Interval;
	static const Interval INVALID_INTERVAL(0.0,0.0);
	static const double INFINITY = Limits<double>::max();

	/** Piecewise function object.
		This class provides the interface for piecewise functions needed as
		representation of radial distribution functions (@see ...). It
		implements the {\tt Function} interface.\\
		Note that intervals {\em must} be disjunct and interval limits have to
		meet. Argh. Ich kann kein fremdländisch. We require the intervals to be
		sorted such that the lowest interval limit is the first interval of the
		vector.
		{\bf Definition:} \URL{BALL/MATHS/piecewiseFunction.h}
	 */
	
	class PiecewiseFunction
	{
		public:

		// BALL_CREATE_DEEP(PiecewiseFunction);

		/** @name Constructors and Destructors */
		//@{

		/** Default constructor */
		PiecewiseFunction();

		/** Copy constructor */
		PiecewiseFunction(const PiecewiseFunction& function);

		/** Detailed constructor */
		PiecewiseFunction(const std::vector<Interval>& intervals, 
			const std::vector<Coefficients>& coeffs);

		/** Destructor */
		virtual ~PiecewiseFunction();

		/** Clear function */
		void clear();

		/** Destroy function */
		void destroy();

		//@}


		/** */ 
		void set(const PiecewiseFunction& function);

		/** */
		void set(const std::vector<Interval>& intervals,
				const std::vector<Coefficients>& coeffs);

		/** */
		PiecewiseFunction& operator = (const PiecewiseFunction& function);


		/** @name Accessors */
		//@{

		/** Set the intervals for the piecewise definition.
			Note that this method does {\bg not} check the definition of the
			intervals for sanity.
		 */
		void setIntervals(const std::vector<Interval>& intervals);

		/** Get all the intervals */
		const std::vector<Interval>& getIntervals() const;

		/** Get the interval a given x belongs to */
		const Interval& getInterval(double x) const;

		/** Get interval limits by index */
		const Interval& getInterval(Position index) const;

		/** Get the interval index for a given x */
		Position getIntervalIndex(double x) const;

		/** Return the range of the definition */
		Interval getRange() const;

		/** Set the coefficients.
			Note that this method does {\bf not} check the vector of coefficients
			for sanity.
		 */
		void setCoefficients(const vector<Coefficients>& coefficients);

		/** */
		const std::vector<Coefficients>& getCoefficients() const;

		/** Get the coefficients for a given x */
		const Coefficients& getCoefficients(double x) const;

		/** get coefficients from index */
		const Coefficients& getCoefficients(Position index) const;
		
		/** compute the value of the piecewise function ata given x */
		virtual double operator () (double x) const;

		//@}


		/** @name Predicates */
		//@{

		/** Check whether a given x is in the range of definition */
		bool isInRange(double x) const;

		//@}

		/** @name Debugging and Diagnostics */
		//@{

		/** check validity of the definition of a piecewise function */
		virtual bool isValid() const;

		//@}

		protected:

		std::vector<Interval> intervals_;
		std::vector<Coefficients> coefficients_;

		bool valid_;

	};

}

#endif
