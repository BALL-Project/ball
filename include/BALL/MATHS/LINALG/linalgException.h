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
		class BALL_EXPORT MatrixHasWrongDimension : public BALL::Exception::GeneralException
		{
			public:
				MatrixHasWrongDimension() 
					throw();
				
				MatrixHasWrongDimension(const char* file, unsigned long line)
					throw();
				
				virtual ~MatrixHasWrongDimension()
					throw();
		};

		class BALL_EXPORT VectorHasWrongDimension : public BALL::Exception::GeneralException
		{
			public:
				VectorHasWrongDimension()
					throw();
				
				VectorHasWrongDimension(const char* file, unsigned long line)
					throw();

				virtual ~VectorHasWrongDimension()
					throw();
		};

		class BALL_EXPORT MatrixNotRegular : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotRegular() 
					throw();

				~MatrixNotRegular()
					throw();
		};

		class BALL_EXPORT MatrixNotQuadratic : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotQuadratic() 
					throw();

				~MatrixNotQuadratic()
					throw();
		};
		
		class BALL_EXPORT MatrixIsSingular : public BALL::Exception::GeneralException
		{
			public:
				MatrixIsSingular(const char* file, unsigned long line) 
						throw();

				~MatrixIsSingular()
						throw();
		};		

// 		class BALL_EXPORT OutOfMemory : public std::bad_alloc
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

// 		class BALL_EXPORT OutOfRange 
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

		class BALL_EXPORT SingularIterator : public BALL::Exception::GeneralException
		{
			public:
				SingularIterator() 
					throw();

				~SingularIterator()
					throw();
		};

// 		class BALL_EXPORT InvalidIterator
// 		{
// 			public:
// 				InvalidIterator() 
// 					throw();
// 
// 				~InvalidIterator()
// 					throw();
// 		};

// 		class BALL_EXPORT Precondition
// 		{
// 			public:
// 				Precondition() 
// 					throw();
// 
// 				~Precondition()
// 					throw();
// 		};

// 		class BALL_EXPORT IncompatibleIterators
// 		{
// 			public:
// 				IncompatibleIterators() 
// 					throw();
// 
// 				~IncompatibleIterators()
// 					throw();
// 		};

		class BALL_EXPORT Uninitialized : public BALL::Exception::GeneralException
		{
			public:
				Uninitialized() throw();
				~Uninitialized()throw();
		};
	}

}

#endif // BALL_COMMON_EXCEPTION_H
