// $Id: forceFieldParameters.C,v 1.5 2000/02/14 22:42:46 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>

namespace BALL 
{

	ForceFieldParameters::ForceFieldParameters()
		: Parameters()
	{
		valid_ = false;
	}

	ForceFieldParameters::ForceFieldParameters(const String& filename)
		:	Parameters()
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

} // namespace BALL
