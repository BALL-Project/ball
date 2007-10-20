// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// 
// not checked in

#ifndef BALL_QSAR_PARTIALCHARGEDESCRIPTORS_H
#define BALL_QSAR_PARTIALCHARGEDESCRIPTORS_H

#ifndef BALL_QSAR_PARTIALCHARGEBASE_H
# include <BALL/QSAR/partialChargeBase.h>
#endif

namespace BALL
{
	// 4 partial charge descriptors
	
	/** This descriptor adds up all positive partial charges
			over all atoms.
	*/
	class BALL_EXPORT TotalPositivePartialCharge
		: public PartialChargeBase
	{
		public:

		BALL_CREATE(TotalPositivePartialCharge)
		
		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/	
		TotalPositivePartialCharge();

		/** Copy constructor
		*/
		TotalPositivePartialCharge(const TotalPositivePartialCharge& tppc);

		/** Destructor
		*/
		virtual ~TotalPositivePartialCharge();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		TotalPositivePartialCharge& operator = (const TotalPositivePartialCharge& tppc);
		//@}
	};

	/** This descriptor adds up all negative partial charges
			over all atoms.
	*/
	class BALL_EXPORT TotalNegativePartialCharge
		:	public PartialChargeBase
	{
		public:

		BALL_CREATE(TotalNegativePartialCharge)
		
		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		TotalNegativePartialCharge();

		/** Copy constructor
		*/
		TotalNegativePartialCharge(const TotalNegativePartialCharge& tnpc);

		/** Destructor
		*/
		virtual ~TotalNegativePartialCharge();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		TotalNegativePartialCharge& operator = (const TotalNegativePartialCharge& tnpc);
		//@}
	};

	/** This descriptor returns the fraction of positive
			partial charges, which is calculated by deviding 
			the biggest partial charges value by the sum of 
			positive partial charges.
	*/
	class BALL_EXPORT RelPositivePartialCharge
		:	public PartialChargeBase
	{
		public:

		BALL_CREATE(RelPositivePartialCharge)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelPositivePartialCharge();

		/** Copy constructor
		*/
		RelPositivePartialCharge(const RelPositivePartialCharge& rppc);

		/** Destructor
		*/
		virtual ~RelPositivePartialCharge();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelPositivePartialCharge& operator = (const RelPositivePartialCharge& rppc);
		//@}
	};

	/** This descriptor returns the fraction of negative
			partial charges, which is calculated by deviding
			the most negative partial charge value by the sum
			of negative partial charges.
	*/
	class BALL_EXPORT RelNegativePartialCharge
		:	public PartialChargeBase
	{
		public:

		BALL_CREATE(RelNegativePartialCharge)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/
		RelNegativePartialCharge();

		/** Copy constructor
		*/
		RelNegativePartialCharge(const RelNegativePartialCharge& rppc);

		/** Destructor
		*/
		virtual ~RelNegativePartialCharge();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		RelNegativePartialCharge& operator = (const RelNegativePartialCharge& rnpc);
		//@}
	};

} // namespace BALL

#endif // BALL_QSAR_PARTIALCHARGEDESCRIPTORS_H
