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
				MatrixHasWrongDimension();
				
				MatrixHasWrongDimension(const char* file, unsigned long line);
				
				virtual ~MatrixHasWrongDimension() throw();
		};

		class BALL_EXPORT VectorHasWrongDimension : public BALL::Exception::GeneralException
		{
			public:
				VectorHasWrongDimension();
				
				VectorHasWrongDimension(const char* file, unsigned long line);

				virtual ~VectorHasWrongDimension() throw();
		};

		class BALL_EXPORT MatrixNotRegular : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotRegular();

				~MatrixNotRegular() throw();
		};

		class BALL_EXPORT MatrixNotQuadratic : public BALL::Exception::GeneralException
		{
			public:
				MatrixNotQuadratic();

				~MatrixNotQuadratic() throw();
		};
		
		class BALL_EXPORT MatrixIsSingular : public BALL::Exception::GeneralException
		{
			public:
				MatrixIsSingular(const char* file, unsigned long line);

				~MatrixIsSingular() throw();
		};		

		class BALL_EXPORT SingularIterator : public BALL::Exception::GeneralException
		{
			public:
				SingularIterator();

				~SingularIterator() throw();
		};

		class BALL_EXPORT Uninitialized : public BALL::Exception::GeneralException
		{
			public:
				Uninitialized();
				~Uninitialized() throw();
		};
	}

}

#endif // BALL_COMMON_EXCEPTION_H
