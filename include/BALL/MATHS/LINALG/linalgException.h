#ifndef BALL_LINALG_EXCEPTION_H
#define BALL_LINALG_EXCEPTION_H

#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif

#include <iostream>

namespace BALL 
{
	namespace Exception 
	{
		class MatrixHasWrongDimension : public BALL::Exception::GeneralException
		{
			public:
				MatrixHasWrongDimension() 
					throw();
				virtual ~MatrixHasWrongDimension()
					throw();
		};

		class VectorHasWrongDimension : public BALL::Exception::GeneralException
		{
			public:
				VectorHasWrongDimension()
					throw();

				virtual ~VectorHasWrongDimension()
					throw();
		};

		class MatrixNotRegular : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotRegular() 
					throw();

				~MatrixNotRegular()
					throw();
		};

		class MatrixNotQuadratic : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotQuadratic() 
					throw();

				~MatrixNotQuadratic()
					throw();
		};

// 		class OutOfMemory : public std::bad_alloc
// 		{
// 			public:
// 				OutOfMemory(const char* file, int line, size_t n = 0)
// 					throw();
// 
// 				virtual ~OutOfMemory()
// 					throw();
// 
// 				protected:
// 					size_t size_;
// 		};

// 		class OutOfRange 
// 		{
// 			public:
// 
// 			 OutOfRange(const char* file, int line, size_t size, size_t value)
// 				throw();
// 
// 			~OutOfRange()
// 				throw();
// 	
// 			protected:
// 				size_t size_;
// 				size_t index_;
// 		};

		class SingularIterator : public BALL::Exception::GeneralException
		{
			public:
				SingularIterator() 
					throw();

				~SingularIterator()
					throw();
		};

// 		class InvalidIterator
// 		{
// 			public:
// 				InvalidIterator() 
// 					throw();
// 
// 				~InvalidIterator()
// 					throw();
// 		};

// 		class Precondition
// 		{
// 			public:
// 				Precondition() 
// 					throw();
// 
// 				~Precondition()
// 					throw();
// 		};

// 		class IncompatibleIterators
// 		{
// 			public:
// 				IncompatibleIterators() 
// 					throw();
// 
// 				~IncompatibleIterators()
// 					throw();
// 		};

		class Uninitialized : public BALL::Exception::GeneralException
		{
			public:
				Uninitialized() throw();
				~Uninitialized()throw();
		};
	}

}

#endif // BALL_COMMON_EXCEPTION_H
