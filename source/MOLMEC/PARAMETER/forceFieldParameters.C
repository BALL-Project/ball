// $Id: forceFieldParameters.C,v 1.4 2000/02/11 17:56:17 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

namespace BALL 
{

	ForceFieldParameters::ForceFieldParameters()
	{
		valid_ = false;
	}

	ForceFieldParameters::ForceFieldParameters(const String& filename)
	{
		// try to read the parameter file
		INI_file_.setFilename(filename);
		init();
	}

	ForceFieldParameters::~ForceFieldParameters()
	{
		// clean up everything
		INI_file_.destroy();
		valid_ = false;
	}


	const String& ForceFieldParameters::getFilename() const 
	{
		return INI_file_.getFilename();
	}

	void ForceFieldParameters::setFilename(const String& filename) 
	{
		INI_file_.setFilename(filename);
	}

	INIFile& ForceFieldParameters::getParameterFile() 
	{
		return INI_file_;
	}

	FFPSAtomTypes& ForceFieldParameters::getAtomTypes() 
	{
		return atom_types_;
	}

	bool ForceFieldParameters::init()
	{
		// read the parameter file
		if (!INI_file_.read())
		{
			throw Exception::FileNotFound(__FILE__, __LINE__, INI_file_.getFilename().c_str());
		}

		// extract the AtomTypes section
		// set valid_ as extractSection checks for valid parameters!
		valid_ = INI_file_.isValid();
		valid_ =  atom_types_.extractSection(*this, "AtomTypes");

		return valid_;
	}

	bool ForceFieldParameters::isValid() const
	{
		return (valid_ && atom_types_.isValid() && INI_file_.isValid());
	}

} // namespace BALL
