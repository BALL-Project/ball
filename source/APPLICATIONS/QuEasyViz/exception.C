/* exception.C
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

#include <exception.h>
#include <BALL/DATATYPE/string.h>

namespace BALL
{
	namespace Exception
	{

		InvalidModelItem::InvalidModelItem(const char* file, unsigned long line) throw() 
			: BALL::Exception::GeneralException(file,line)
		{
			name_= "InvalidModelItem";
			message_ = "The creation of the model item failed";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidInputDataItem ::InvalidInputDataItem (const char* file, unsigned long line) throw() : GeneralException(file,line)
		{
			name_= "InvalidInputDataItem";
			message_ = "The creation of the input data item failed";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidFeatureSelectionItem::InvalidFeatureSelectionItem(const char* file, unsigned long line) throw() : GeneralException(file,line)
		{
			name_= "InvalidFeatureSelectionItem";
			message_ = "The creation of the feature selection item failed";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidValidationItem::InvalidValidationItem(const char* file, unsigned long line) throw(): GeneralException(file,line)
		{
			name_= "InvalidValidationItem";
			message_ = "The creation of the validation item failed";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidPredictionItem::InvalidPredictionItem(const char* file, unsigned long line) throw(): GeneralException(file,line)
		{
			name_= "InvalidPrecitionItem";
			message_ = "The creation of the prediction item failed";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidPipeline::InvalidPipeline(const char* file, unsigned long line) throw(): GeneralException(file,line)
		{
			name_= "InvalidPipeline";
			message_ = "Could not restore the pipeline from file";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));	
		}

		ModelTrainingError::ModelTrainingError(const char* file, unsigned long line) throw(): GeneralException(file,line)
		{
			name_= "ModelTrainingError";
			message_ = "The model could not be trained because of a singular matrix";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}

		InvalidK::InvalidK(const char* file, unsigned long line) throw(): GeneralException(file,line)
		{
			name_= "InvalidK";
			message_ = "The k value given is invalid";
			BALL::Exception::globalHandler.setName(String(name_));
			BALL::Exception::globalHandler.setMessage(String(message_));
		}
	}
}
