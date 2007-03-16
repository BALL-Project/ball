// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#ifndef BALL_QSAR_CONNECTIVITYDESCRIPTORS_H
#define BALL_QSAR_CONNECTIVITYDESCRIPTORS_H

#ifndef BALL_QSAR_CONNECTIVITYBASE_H
# include <BALL/QSAR/connectivityBase.h>
#endif

namespace BALL
{
	// 2 connectivity descriptors

	/** This descriptor returns the Zagreb Index, which is calculated
			as: $\sum_i d_i^2$, over all heavy atoms
	*/
	class BALL_EXPORT ZagrebIndex
		: public ConnectivityBase
	{
		public:

		BALL_CREATE(ZagrebIndex)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		ZagrebIndex();

		/** Copy constructor
		*/
		ZagrebIndex(const ZagrebIndex& zi);
		
		/** Destructor
		*/
		virtual ~ZagrebIndex();
		//@}
		
		/** @name Assignment
		*/
		//@{
		/** Assignment operator
		*/
		ZagrebIndex& operator = (const ZagrebIndex& zi);
		//@}
		
		/** @name Accessors
		*/
		//@{
		double compute(AtomContainer& ac);
		//@}
	};

	/** The Balaban index J is topological index. It is calculated using
			the distance matrix with weighted edges of the molecular graph:
			1 for single bonds, $\frac{1}{2}$ for double bonds, $\frac{1}{3}$
			for triple bonds and $\frac{2}{3}$ for aromatic bonds.  
			\( J = \frac{q}{(q-n+2)} \cdot \sum_{\text{adjac.}i,j} (s_i,s_j)^{-\frac{1}{2}} \),
			$s_i$ and $s_j$ denotes the row sum of the distance matrix of atom $i$ and $j$.
	*/
	class BALL_EXPORT BalabanIndexJ
		:	public ConnectivityBase
	{
		public:

		BALL_CREATE(BalabanIndexJ)

		/** @name Constructors and Destructors
		*/
		//@{
		/** Default constructor
		*/		
		BalabanIndexJ();

		/** Copy constructor
		*/
		BalabanIndexJ(const BalabanIndexJ& bij);

		/** Destructor
		*/
		virtual ~BalabanIndexJ();
		//@}

		/** @name Assignment
		*/
		//@{
		/** Assignment Operator
		*/
		BalabanIndexJ& operator = (const BalabanIndexJ& bij);
		//@}
	};

} // namespace BALL

#endif // BALL_QSAR_CONNECTIVITYDESCRIPTORS_H

