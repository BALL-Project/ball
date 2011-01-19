// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//

#include <BALL/NMR/shiftModel.h>
#include <BALL/CONCEPT/factory.h>
#include <BALL/FORMAT/parameterSection.h>
#include <BALL/NMR/johnsonBoveyShiftProcessor.h>
#include <BALL/NMR/haighMallionShiftProcessor.h>
#include <BALL/NMR/EFShiftProcessor.h>
#include <BALL/NMR/anisotropyShiftProcessor.h>
#include <BALL/NMR/randomCoilShiftProcessor.h>
#include <BALL/NMR/HBondShiftProcessor.h>
#include <BALL/NMR/empiricalHSShiftProcessor.h>

using namespace std;

namespace BALL
{	
	const char* ShiftModel::MODULE_LIST_SECTION = "ShiftModules";

	ShiftModel::ShiftModel()
		:	ShiftModule(),
			parameters_(),
			modules_(),
			valid_(false)
	{
		registerStandardModules_();
	}

	ShiftModel::ShiftModel(const String& filename)
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
		:	ShiftModule(),
			parameters_(model.parameters_),
			modules_(),
			registered_modules_(model.registered_modules_),
			valid_(false)
	{
		init_();
	}

	void ShiftModel::clear()
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
	{
		clear();
	}

	Parameters& ShiftModel::getParameters()
	{
		return parameters_;
	}

	ShiftModel::ModuleList& ShiftModel::getModuleList()	
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
	{
		return parameters_.getFilename();
	}

	bool ShiftModel::isValid() const
		
	{
		return valid_;
	}

	const ShiftModel& ShiftModel::operator = (const ShiftModel& model)	
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
				Position name_col = module_section.getColumnIndex("name");
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
	{
		registered_modules_.erase(name);
	}

	ShiftModule* ShiftModel::createModule_(const String& type, const String& name) const
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
	{
		registerModule("JohnsonBovey", Factory<JohnsonBoveyShiftProcessor>::createVoid);
		registerModule("HaighMallion", Factory<HaighMallionShiftProcessor>::createVoid);
		registerModule("ElectricField", Factory<EFShiftProcessor>::createVoid);
		registerModule("Anisotropy", Factory<AnisotropyShiftProcessor>::createVoid);
		registerModule("RandomCoil", Factory<RandomCoilShiftProcessor>::createVoid);
		registerModule("HBond", Factory<HBondShiftProcessor>::createVoid);
		registerModule("EmpiricalShiftHyperSurfaces", Factory<EmpiricalHSShiftProcessor>::createVoid);
	}

	Processor::Result ShiftModel::operator () (Composite& composite)
	{
		// Clear previsously assigned shifts and...
		Atom* atom = dynamic_cast<Atom*>(&composite);
		if (atom != 0)
		{
			atom->clearProperty(ShiftModule::PROPERTY__SHIFT);
		}
			
		// ...call operator () for every module.
		Processor::Result result = Processor::CONTINUE;
		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			// abort if any of the modules returns Processor::ABORT
			result = (*it)->operator () (composite);
			if (result == Processor::ABORT)
			{
				break;
			}
		}
		
		return result;
	}


	bool ShiftModel::start()
	{
		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			if (!(*it)->start()) return false;
		}
		
		return true;
	}


	bool ShiftModel::finish()
	{
		ModuleList::iterator it = modules_.begin();
		for (; it != modules_.end(); ++it)
		{
			if (!(*it)->finish()) return false;
		}
		
		return true;
	}
}


