// $Id: shiftModule.C,v 1.8 2000/09/21 13:48:04 oliver Exp $

#include<BALL/NMR/shiftModule.h>

using namespace std;

namespace BALL
{

	const char* ShiftModule::PROPERTY__SHIFT = "ChemicalShift";

	ShiftModule::ShiftModule()
		throw()
		:	module_name_(""),
			parameters_(0),
			valid_(false)
	{
	}

	ShiftModule::~ShiftModule()
		throw()
	{
	}

	ShiftModule::ShiftModule(Parameters& parameters, const String& name)
		throw()
		:	module_name_(name),
			parameters_(&parameters),
			valid_(false)
	{
	}

	ShiftModule::ShiftModule(const ShiftModule& module)
		throw()
		:	UnaryProcessor<Composite>(module),
			module_name_(module.module_name_),
			parameters_(module.parameters_),
			valid_(module.valid_)
	{
	}

	void ShiftModule::clear()
		throw()
	{
		module_name_ = "";
		parameters_ = 0;
		valid_ = false;
	}


	void ShiftModule::init()
		throw()
	{
		// empty method
	}

	const ShiftModule& ShiftModule::operator = (const ShiftModule& module) 
		throw()
	{
		module_name_ = module.module_name_;
		parameters_ = module.parameters_;
		valid_ = module.valid_;

		return *this;
	}

	void ShiftModule::setName(const String& name)
		throw()
	{
		module_name_ = name;
	}

	const String& ShiftModule::getName() const
		throw()
	{
		return module_name_;
	}

	void ShiftModule::setParameters(Parameters& parameters)
		throw()
	{
		parameters_ = &parameters;
		valid_ = false;
	}

	const Parameters* ShiftModule::getParameters() const
		throw()
	{
		return parameters_;
	}

	bool ShiftModule::start()
		throw()
	{
		// abort if the module was not correctly initialized
		return valid_;
	}

	bool ShiftModule::finish()
		throw()
	{
		// abort if the module was not correctly initialized
		return valid_;
	}

	bool ShiftModule::isValid() const
		throw()
	{
		return valid_;
	}

}	// namespace BALL
