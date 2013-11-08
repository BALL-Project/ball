// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//


#ifndef QSARTOOL_EXCEPTION
#define QSARTOOL_EXCEPTION

#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif


namespace BALL 
{
	namespace Exception
	{
		/** Exception for Invalid ModelItems*/
		class InvalidModelItem 
			: public BALL::Exception::GeneralException
		{
			public:
				InvalidModelItem(const char* file, unsigned long line) throw();
		};		
			
		/** Exception for Invalid InputDataItems*/
		class InvalidInputDataItem : public BALL::Exception::GeneralException
		{
			public:
				InvalidInputDataItem(const char* file, unsigned long line) throw();
		};	

		/** Exception for Invalid FeatureSelectionItems*/
		class InvalidFeatureSelectionItem : public BALL::Exception::GeneralException
		{
			public:
				InvalidFeatureSelectionItem(const char* file, unsigned long line) throw();
		};
	
		/** Exception for  Invalid FeatureSelectionItems*/
		class InvalidK : public BALL::Exception::GeneralException
		{
			public:
				InvalidK(const char* file, unsigned long line) throw();
		};

		/** Exception for Invalid ValidationItems*/
		class InvalidValidationItem : public BALL::Exception::GeneralException
		{
			public:
				InvalidValidationItem(const char* file, unsigned long line) throw();
		};

		/** Exception for Invalid PredictionItems*/
		class InvalidPredictionItem : public BALL::Exception::GeneralException
		{
			public:
				InvalidPredictionItem(const char* file, unsigned long line) throw();
		};

		/** Exception for Invalid Pipeline*/
		class InvalidPipeline : public BALL::Exception::GeneralException
		{
			public:
				InvalidPipeline(const char* file, unsigned long line) throw();
		};

		/** Exception for Invalid Pipeline*/
		class ModelTrainingError : public BALL::Exception::GeneralException
		{
			public:
				ModelTrainingError(const char* file, unsigned long line) throw();
		};
	}
}

#endif

