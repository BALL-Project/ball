// $Id: parameters.C,v 1.9 2001/04/23 22:00:32 amoll Exp $
//

#include <BALL/FORMAT/parameters.h>

namespace BALL 
{

	Parameters::Parameters()
		:	valid_(false)
	{
	}

	Parameters::Parameters(const Parameters& parameters)
		:	valid_(parameters.valid_),
			INI_file_(parameters.INI_file_)
	{
		init();
	}

	Parameters::Parameters(const String& filename)
	{
		// try to read the parameter file and initialize
		INI_file_.setFilename(filename);
		init();
	}

	Parameters::~Parameters()
	{
		clear();
	}


	void Parameters::clear()
		throw()
	{
		// clean up everything
		INI_file_.destroy();
		valid_ = false;
	}

	const Parameters& Parameters::operator = (const Parameters& parameters)
	{
		INI_file_ = parameters.INI_file_;
		valid_ = parameters.valid_;
		
		return *this;
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
			return false;
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


	bool Parameters::operator == (const Parameters& parameters) const
	{
		return ((valid_ == parameters.valid_) 
				&& (INI_file_ == parameters.INI_file_));
	}


} // namespace BALL
