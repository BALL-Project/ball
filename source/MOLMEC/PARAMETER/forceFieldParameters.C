// $Id: forceFieldParameters.C,v 1.8 2000/10/18 10:58:13 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

namespace BALL 
{

	ForceFieldParameters::ForceFieldParameters()
		: Parameters(),
			atom_types_()
	{
		valid_ = false;
	}


	ForceFieldParameters::ForceFieldParameters(const String& filename)
		:	Parameters(),
			atom_types_()
	{
		// try to read the parameter file
		INI_file_.setFilename(filename);
		init();
	}


	ForceFieldParameters::ForceFieldParameters
		(const ForceFieldParameters& force_field_parameter)
		: Parameters(force_field_parameter),
			atom_types_(force_field_parameter.atom_types_)
	{
	}


	ForceFieldParameters::~ForceFieldParameters()
		throw()
	{
		clear();
	}


	void ForceFieldParameters::clear()
		throw()
	{
		// clean up everything
		// BAUSTELLE: Sollte INIFile::clear() aufrufen
		INI_file_.destroy();
		valid_ = false;
	}


	const ForceFieldParameters& ForceFieldParameters::operator = 
		(const ForceFieldParameters& param)
	{
		Parameters::operator = (param);
		atom_types_ = param.atom_types_;

		return *this;
	}


	AtomTypes& ForceFieldParameters::getAtomTypes() 
	{
		return atom_types_;
	}


	bool ForceFieldParameters::init()
	{
		// call the init() method of Parameters
		Parameters::init();

		// extract the AtomTypes section
		// set valid_ as extractSection checks for valid parameters!
		valid_ &= atom_types_.extractSection(*this, "AtomTypes");

		return valid_;
	}


	bool ForceFieldParameters::isValid() const
	{
		return (valid_ && atom_types_.isValid() && INI_file_.isValid());
	}


	bool ForceFieldParameters::operator == (const ForceFieldParameters& param) const
	{
		// BAUSTELLE: Gleichheit der Mutterklasse Parameters
		return (param.atom_types_ == atom_types_);
	}

} // namespace BALL
