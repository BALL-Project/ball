/* exception.h
 * 
 * Copyright (C) 2009 Marcel Schumann
 * 
 * This file is part of QuEasy -- A Toolbox for Automated QSAR Model
 * Construction and Validation.
 * QuEasy is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or (at
 * your option) any later version.
 * 
 * QuEasy is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

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
	namespace VIEW
	{
		namespace Exception
		{
			/** Exception for Invalid ModelItems*/
			class InvalidModelItem : public BALL::Exception::GeneralException
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
}


#endif

