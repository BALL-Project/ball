// $Id: shiftModel.C,v 1.2 2000/09/18 11:03:17 oliver Exp $

#include <BALL/NMR/shiftModel.h>

namespace BALL
{

	ShiftModel::ShiftModel()
		throw()
		:	ShiftModule(),
			parameters_(),
			modules_(),
			valid_(false)
	{
	}

	ShiftModel::ShiftModel(const String& filename)
		throw()
		:	ShiftModule(),
			parameters_(filename),
			modules_(),
			valid_(false)
	{
		init_();
	}

	void ShiftModel::clear()
		throw()
	{
		// model is invalid
		valid_ = false;

		// delete modules
		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			delete *it;
		}		
		modules_.clear();

		// clear parameters
		parameters_.clear();

		// clear options
		options.clear();
	}

	ShiftModel::~ShiftModel()
		throw()
	{
		clear();
	}

	Parameters& ShiftModel::getParameters()
		throw()
	{
		return parameters_;
	}

	ShiftModel::ModuleList& ShiftModel::getModuleList()
		throw()
	{
		return modules_;
	}

	void ShiftModel::setFilename(const String& filename)
		throw(Exception::FileNotFound)
	{
		// set the parameter filename 
		parameters_.setFilename(filename);

		// ...and initialize!
		init_();
	}
	
	const String& ShiftModel::getFilename() const
		throw()
	{
		return parameters_.getFilename();
	}

	bool ShiftModel::isValid() const
		throw()
	{
		return valid_;
	}

	const ShiftModel& ShiftModel::operator = (const ShiftModel& model)
		throw()
	{
		// clear the old contents
		clear();

		// copy the parameters and options
		parameters_ = model.parameters_;
		options = model.options;
		
		// and try to set up the same model 
		init_();

		return *this;
	}

	const ShiftModel& ShiftModel::operator = (const String& filename)
		throw()
	{
		// clear the old contents,
		clear();

		// read the parameters, and initialize
		setFilename(filename);

		return *this;		
	}

	bool ShiftModel::init_()
		throw()
	{
		// inivalidate object
		valid_ = false;

		// BAUSTELLE

		// return the current state
		return valid_;
	}
	
}
