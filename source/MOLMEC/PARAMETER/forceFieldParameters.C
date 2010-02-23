// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: forceFieldParameters.C,v 1.13 2003/08/26 09:17:54 oliver Exp $
//

#include <BALL/MOLMEC/PARAMETER/forceFieldParameters.h>
#include <BALL/FORMAT/INIFile.h>

namespace BALL 
{

	ForceFieldParameters::ForceFieldParameters(ForceField* force_field)
		: Parameters(),
			atom_types_(),
			force_field_(force_field)
	{
		valid_ = false;
	}

	ForceFieldParameters::ForceFieldParameters(const String& filename, ForceField* force_field)
		:	Parameters(),
			atom_types_(force_field),
			force_field_(force_field)
	{
		// try to read the parameter file
		INI_file_.setFilename(filename);
		init();
	}

	ForceFieldParameters::ForceFieldParameters
		(const ForceFieldParameters& force_field_parameter)
		: Parameters(force_field_parameter),
			atom_types_(force_field_parameter.atom_types_),
			force_field_(force_field_parameter.force_field_)
	{
	}


	ForceFieldParameters::~ForceFieldParameters()
	{
		clear();
	}


	void ForceFieldParameters::clear()
	{
		// clean up everything
		INI_file_.clear();
		valid_       = false;
		force_field_ = 0;
	}


	const ForceFieldParameters& ForceFieldParameters::operator = 
		(const ForceFieldParameters& param)
	{
		Parameters::operator = (param);
		atom_types_ = param.atom_types_;
		force_field_ = param.force_field_;

		return *this;
	}


	AtomTypes& ForceFieldParameters::getAtomTypes() 
	{
		return atom_types_;
	}
		
	ForceField* ForceFieldParameters::getForceField() 
	{
		return force_field_;
	}
		

	bool ForceFieldParameters::exportParmFile(File& outfile) const
	{
		// first export the atom types
		bool value = atom_types_.exportParmFile(outfile);
		
		// then export the hydrophilic types
		INIFile* inifile = const_cast<INIFile*>(&INI_file_);
		INIFile::LineIterator it = inifile->getSectionFirstLine("HydrophilicTypes");
		++it;
		while (((*it)[0]==';') && +it)
		{
			++it;
		}
		outfile << *it << endl;;
		return value;
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
		// ?????: Equality of the mother class Parameters
		return (   (param.atom_types_ == atom_types_)
				    && (param.force_field_ == force_field_));
	}

} // namespace BALL
