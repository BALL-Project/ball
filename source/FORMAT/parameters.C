// $Id: parameters.C,v 1.1 2000/02/14 22:47:10 oliver Exp $
//

#include <BALL/FORMAT/parameters.h>

namespace BALL 
{

	Parameters::Parameters()
	{
		valid_ = false;
	}

	Parameters::Parameters(const String& filename)
	{
		// try to read the parameter file
		INI_file_.setFilename(filename);
		init();
	}

	Parameters::~Parameters()
	{
		// clean up everything
		INI_file_.destroy();
		valid_ = false;
	}


	const String& Parameters::getFilename() const 
	{
		return INI_file_.getFilename();
	}

	void Parameters::setFilename(const String& filename) 
	{
		INI_file_.setFilename(filename);
	}

	INIFile& Parameters::getParameterFile() 
	{
		return INI_file_;
	}

	bool Parameters::init()
	{
		// read the parameter file
		if (!INI_file_.read())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, INI_file_.getFilename().c_str());
		}

		// extract the AtomTypes section
		// set valid_ as extractSection checks for valid parameters!
		valid_ = INI_file_.isValid();

		return valid_;
	}

	bool Parameters::isValid() const
	{
		return (valid_ && INI_file_.isValid());
	}

} // namespace BALL
