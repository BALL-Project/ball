// $Id: shiftModel.C,v 1.11 2000/09/25 19:12:09 oliver Exp $

#include <BALL/NMR/shiftModel.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/NMR/EFShiftProcessor.h>
#include <BALL/NMR/anisotropyShiftProcessor.h>
#include <BALL/NMR/randomCoilShiftProcessor.h>

using namespace std;

namespace BALL
{	
	const char* ShiftModel::MODULE_LIST_SECTION = "ShiftModules";

	ShiftModel::ShiftModel()
		throw()
		:	ShiftModule(),
			parameters_(),
			modules_(),
			valid_(false)
	{
		registerStandardModules_();
	}

	ShiftModel::ShiftModel(const String& filename)
		throw()
		:	ShiftModule(),
			parameters_(filename),
			modules_(),
			registered_modules_(),
			valid_(false)
	{
		registerStandardModules_();
		init_();
	}

	ShiftModel::ShiftModel(const ShiftModel& model)
		throw()
		:	ShiftModule(),
			parameters_(model.parameters_),
			modules_(),
			registered_modules_(model.registered_modules_),
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

		// reset the list of registerd modules
		registered_modules_.clear();
		registerStandardModules_();

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

		// copy the registered modules
		registered_modules_ = model.registered_modules_;
		
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
		throw(Exception::FileNotFound)
	{
		// inivalidate object
		valid_ = false;

		parameters_.init();
		if (parameters_.isValid() && parameters_.getParameterFile().hasSection(MODULE_LIST_SECTION))
		{
			ParameterSection module_section;
			module_section.extractSection(parameters_, MODULE_LIST_SECTION);

			if (module_section.hasVariable("name"))
			{
				// the section contains the columns "name" and "type", let's construct
				// the corresponding modules 
				Position name_col = module_section.getColumnIndex("nmae");
				for (Position i = 0; i < module_section.getNumberOfKeys(); i++)
				{
					String type = module_section.getKey(i);
					String name = module_section.getValue(i, name_col);

					if (registered_modules_.has(type))
					{
						// call the corresponding create method to construct the ShiftModule
						ShiftModule* module = createModule_(type, name);

						// and insert the pointer into the module list
						modules_.push_back(module);
					}
					else
					{
						// complain!
						Log.error() << "ShiftModel::init_: could not create module of type " 
												<< type << " for module " << name << " for shift model " 
												<< parameters_.getFilename() << ". Please use the registerModule method"
												<< " to associate a create method for this module type!" 
												<< std::endl;
					}
				}
			}

			valid_ = true;
		}

		// return the current state
		return valid_;
	}
	

	bool ShiftModel::isRegistered(const String& name) const
		throw()
	{
		return registered_modules_.has(name);
	}

	void ShiftModel::registerModule(const String& name, CreateMethod create_method)
		throw(Exception::NullPointer)
	{	
		// check that we did not get something strange
		if (create_method == 0)
		{
			throw Exception::NullPointer(__FILE__, __LINE__);
		}

		// insert the module into the map
		registered_modules_[name] = create_method;
	}

	void ShiftModel::unregisterModule(const String& name)
		throw()
	{
		registered_modules_.erase(name);
	}

	ShiftModule* ShiftModel::createModule_(const String& type, const String& name) const
		throw()
	{
		ShiftModule* module = 0;
		if (registered_modules_.has(type))
		{
			// if the name is registered, call the
			// corresponding create method from the hash map
			module = (ShiftModule*)(registered_modules_[type])();
			if (module != 0)
			{
				// If we constructed a module, set its name and the parameters,
				// then initialize the shift module.
				module->setParameters(const_cast<Parameters&>(parameters_));
				module->setName(name);
				module->init();
			}
		}

		return module;
	}

	void ShiftModel::registerStandardModules_()
		throw()
	{
		using RTTI::getNew;
		registerModule("JohnsonBovey", getNew<JohnsonBoveyShiftProcessor>);
		registerModule("HaighMallion", getNew<HaighMallionShiftProcessor>);
		registerModule("ElectricField", getNew<EFShiftProcessor>);
		registerModule("Anisotropy", getNew<AnisotropyShiftProcessor>);
		registerModule("RandomCoil", getNew<RandomCoilShiftProcessor>);
	}

	Processor::Result ShiftModel::operator () (Composite& composite)
		throw()
	{
		// call every module
		Processor::Result result;

		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			result = (*it)->operator () (composite);
			if (result == Processor::BREAK)
			{
				break;
			}
		}				
		
		return result;
	}


	bool ShiftModel::start()
		throw()
	{
		// call every module
		bool result;

		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			result = (*it)->start();
			if (result == false)
			{
				break;
			}
		}				
		
		return result;
	}


	bool ShiftModel::finish()
		throw()
	{
		// call every module
		bool result;

		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			result = (*it)->finish();
			if (result == false)
			{
				break;
			}
		}				
		
		return result;
	}


}


