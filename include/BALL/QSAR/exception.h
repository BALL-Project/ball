// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#ifndef BALL_QSAR_EXCEPTION_H
#define BALL_QSAR_EXCEPTION_H

#ifndef BALL_COMMON_EXCEPTION_H
#include <BALL/COMMON/exception.h>
#endif


namespace BALL
{
	namespace QSAR
	{
		namespace Exception
		{
			/** Exception for the case of given activity values <0 or activity values that are larger than the number of properties in the current input file */
			class BALL_EXPORT InvalidActivityID : public BALL::Exception::GeneralException
			{
				public:
					InvalidActivityID(const char* file, unsigned long line, int act, int m) throw();

					InvalidActivityID(const char* file, unsigned long line) throw();
			};

			/** Exception for wrong number of properties or wrong format of properties within structure input files */
			class BALL_EXPORT PropertyError : public BALL::Exception::GeneralException
			{
				public:
					PropertyError(const char* file, unsigned long line, const char* sd_file, int mol, const char* mess="") throw();
			};


			/** Exception for singular, i.e. non-invertible, matrices */
			class BALL_EXPORT SingularMatrixError : public BALL::Exception::GeneralException
			{
				public:
					SingularMatrixError(const char* file, unsigned long line, const char* mess="") throw();
			};


			/** class for errors that are due to inconsistent use of methods by the user; e.g. requesting training of a model before some data has been read ... */
			class BALL_EXPORT InconsistentUsage : public BALL::Exception::GeneralException
			{
				public:
					InconsistentUsage(const char* file, unsigned long line, const char* mess="") throw();
			};

			
			/** Exception for creation of a kernel with wrong parameters */
			class BALL_EXPORT KernelParameterError : public BALL::Exception::GeneralException
			{
				public:
					KernelParameterError(const char* file, unsigned long line, const char* mess) throw();
			};

			
			/** Wrong type of input data for a model, e.g. non-discrete y-values for a classification model */
			class BALL_EXPORT WrongDataType : public BALL::Exception::GeneralException
			{
				public:
					WrongDataType(const char* file, unsigned long line, const char* mess) throw();
			};
			
			/** No variance present to be explained by PCA, so that PCR can not be done */
			class BALL_EXPORT NoPCAVariance : public BALL::Exception::GeneralException
			{
				public:
					NoPCAVariance(const char* file, unsigned long line, const char* mess) throw();
			};

			/** exception for creation of a model with wrong number of parameters or with wrong types of parameters */
			class BALL_EXPORT ModelParameterError : public BALL::Exception::GeneralException
			{
				public:
					ModelParameterError(const char* file, unsigned long line, const char* mess) throw();
			};
			
			/** There may not be more PLS components than features */
			class TooManyPLSComponents : public BALL::Exception::GeneralException
			{
				public:
					TooManyPLSComponents(const char* file, unsigned long line, int comp, int features) throw();
			};

			/** File format observed for an input data file (sdf-file) does not fit the expected file format */
			class BALL_EXPORT WrongFileFormat : public BALL::Exception::GeneralException
			{
				public:
					WrongFileFormat(const char* file, unsigned long line, const char* input_file) throw();
			};
			
			/** exception for specification of an invalid parameters for feature selection */
			class BALL_EXPORT FeatureSelectionParameterError : public BALL::Exception::GeneralException
			{
				public:
					FeatureSelectionParameterError(const char* file, unsigned long line, const char* mess) throw();
			};
			
			/** exception that is thrown when parameters are missing or incorrect within an config-file section */
			class BALL_EXPORT ConfigurationReadingError : public BALL::Exception::GeneralException
			{
				public: 
					ConfigurationReadingError(const char* file, unsigned long line, const char* message) throw();
			};


		}
	}
}


#endif // QSAR_EXCEPTION

