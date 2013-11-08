// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
//

#include <BALL/QSAR/exception.h>
#include <BALL/DATATYPE/string.h>

using namespace BALL::QSAR::Exception;


InvalidActivityID::InvalidActivityID(const char* file, unsigned long line, int i, int m) throw() : GeneralException(file, line)
{
	name_ = "InvalidActivityID";
	message_ = "Invalid activity id! \nGiven: '";
	message_ += String(i) + "', for molecules containing "+String(m)+" properties"; 
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

InvalidActivityID::InvalidActivityID(const char* file, unsigned long line) throw() : GeneralException(file, line)
{
	name_ = "InvalidActivityID";
	message_ = "No activity IDs are specified! "; 
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}


PropertyError::PropertyError(const char* file, unsigned long line, const char* sd_file, int mol, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "PropertyError";
	message_ = mess;
	message_ = message_ + "\nIn input-file "+sd_file +", molecule ";
	message_ += String(mol);
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}


SingularMatrixError::SingularMatrixError(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "SingularMatrixError";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));	
}


InconsistentUsage::InconsistentUsage(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "InconsistentUsage";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));	
}

KernelParameterError::KernelParameterError(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "KernelParameterError";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));	
}


WrongDataType::WrongDataType(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "WrongDataType";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

NoPCAVariance::NoPCAVariance(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "NoPCAVariance";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

ModelParameterError::ModelParameterError(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "ModelParameterError";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}


FeatureSelectionParameterError::FeatureSelectionParameterError(const char* file, unsigned long line, const char* mess) throw() : GeneralException(file, line)
{
	name_ = "FeatureSelectionParameterError";
	message_ = mess;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

TooManyPLSComponents::TooManyPLSComponents(const char* file, unsigned long line, int comp, int features) throw() : GeneralException(file, line)
{
	name_ = "TooManyPLSComponents";
	message_ = "Building a model with more PLS components than features requested! \n number of features: "+String(features)+" , number of desired PLS components: "+String(comp);
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

WrongFileFormat::WrongFileFormat(const char* file, unsigned long line, const char* input_file) throw() : GeneralException(file, line)
{
	name_ = "WrongFileFormat";
	message_ = "Input data file '";
	message_ = message_ + input_file +"' does not have the correct format!!"; 
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}

ConfigurationReadingError::ConfigurationReadingError(const char* file, unsigned long line, const char* message) throw() : GeneralException(file, line)
{
	name_ = "ConfigurationReadingError";
	message_ = message;
	BALL::Exception::globalHandler.setName(String(name_));
	BALL::Exception::globalHandler.setMessage(String(message_));
}
