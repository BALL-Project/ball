#ifndef BALL_LINALG_MATRIXTRAITS_H
#define BALL_LINALG_MATRIXTRAITS_H

namespace BALL {

	enum MatrixTraitTypes
	{
		STANDARD_TRAITS = 1,
		UPPER_TRIANGULAR_TRAITS,
		LOWER_TRIANGULAR_TRAITS,
		SYMMETRIC_TRAITS
	};

	class StandardTraits
	{
		public:		
			const static int traits_id = STANDARD_TRAITS; 
	};

	class UpperTriangularTraits
	{
		public:
			const static int traits_id = UPPER_TRIANGULAR_TRAITS; 
	};

	class LowerTriangularTraits
	{
		public:
			const static int traits_id = LOWER_TRIANGULAR_TRAITS; 
	};

	class SymmetricTraits
	{
		public:
			const static int traits_id = SYMMETRIC_TRAITS;
	};
}

#endif // BALL_KERNEL_MATRIXTRAITS_H
