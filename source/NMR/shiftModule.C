// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include<BALL/NMR/shiftModule.h>

using namespace std;

namespace BALL
{

	const char* ShiftModule::PROPERTY__SHIFT = "ChemicalShift";
	const char* ShiftModule::PROPERTY__EXPERIMENTAL__SHIFT = "ExperimentalChemicalShift";

	ShiftModule::ShiftModule()
		:	module_name_(""),
			parameters_(0),
			valid_(false)
	{
	}

	ShiftModule::~ShiftModule()
	{
	}

	ShiftModule::ShiftModule(Parameters& parameters, const String& name)
		:	module_name_(name),
			parameters_(&parameters),
			valid_(false)
	{
	}

	ShiftModule::ShiftModule(const ShiftModule& module)
		:	UnaryProcessor<Composite>(module),
			module_name_(module.module_name_),
			parameters_(module.parameters_),
			valid_(module.valid_)
	{
	}

	void ShiftModule::clear()	
	{
		module_name_ = "";
		parameters_ = 0;
		valid_ = false;
	}


	void ShiftModule::init()
	{
		// empty method
	}

	const ShiftModule& ShiftModule::operator = (const ShiftModule& module)
	{
		module_name_ = module.module_name_;
		parameters_ = module.parameters_;
		valid_ = module.valid_;

		return *this;
	}

	void ShiftModule::setName(const String& name)	
	{
		module_name_ = name;
	}

	const String& ShiftModule::getName() const	
	{
		return module_name_;
	}

	void ShiftModule::setParameters(Parameters& parameters)	
	{
		parameters_ = &parameters;
		valid_ = false;
	}

	const Parameters* ShiftModule::getParameters() const	
	{
		return parameters_;
	}

	bool ShiftModule::start()	
	{
		// abort if the module was not correctly initialized
		return valid_;
	}

	bool ShiftModule::finish()	
	{
		// abort if the module was not correctly initialized
		return valid_;
	}

	bool ShiftModule::isValid() const	
	{
		return valid_;
	}

}	// namespace BALL
