// $Id: shiftModule.C,v 1.4 2000/09/18 11:03:17 oliver Exp $

#include<BALL/NMR/shiftModule.h>

using namespace std;

namespace BALL
{

	ShiftModule::ShiftModule()
		throw()
	{
	}

	ShiftModule::~ShiftModule()
		throw()
	{
	}

	ShiftModule::ShiftModule(Parameters& parameters, const String& name)
		throw()
		:	module_name_(name),
			parameters_(&parameters)
	{
	}

	ShiftModule::ShiftModule(const ShiftModule& module)
		throw()
		:	UnaryProcessor<Composite>(module),
			module_name_(module.module_name_),
			parameters_(module.parameters_)
	{
	}

	void ShiftModule::clear()
		throw()
	{
		module_name_ = "";
		parameters_ = 0;
	}

	const ShiftModule& ShiftModule::operator = (const ShiftModule& module) 
		throw()
	{
		module_name_ = module.module_name_;
		parameters_ = module.parameters_;

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
	}

	const Parameters* ShiftModule::getParameters() const
		throw()
	{
		return parameters_;
	}

}	// namespace BALL
