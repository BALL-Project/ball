#include <BALL/MOLMEC/PARAMETER/forceFieldEquivalences.h>
#include <BALL/FORMAT/INIFile.h>

namespace BALL 
{

	ForceFieldEquivalences::ForceFieldEquivalences(ForceField* force_field)
		: Parameters(),
			atom_type_equivalences_(force_field),
			force_field_(force_field)
	{
		valid_ = false;
	}


	ForceFieldEquivalences::ForceFieldEquivalences(const String& filename, ForceField* force_field)
		:	Parameters(),
			atom_type_equivalences_(force_field),
			force_field_(force_field)
	{
		// try to read the parameter file
		INI_file_.setFilename(filename);
		init();
	}


	ForceFieldEquivalences::ForceFieldEquivalences
		(const ForceFieldEquivalences& force_field_parameter)
		: Parameters(force_field_parameter),
			atom_type_equivalences_(force_field_parameter.atom_type_equivalences_),
			force_field_(force_field_parameter.force_field_)
	{
	}


	ForceFieldEquivalences::~ForceFieldEquivalences()
	{
		clear();
	}


	void ForceFieldEquivalences::clear()
	{
		// clean up everything
		INI_file_.clear();
		valid_ = false;
		force_field_ = 0;
	}


	const ForceFieldEquivalences& ForceFieldEquivalences::operator = 
		(const ForceFieldEquivalences& param)
	{
		Parameters::operator = (param);
		atom_type_equivalences_ = param.atom_type_equivalences_;
		force_field_ = param.force_field_;
		return *this;
	}


	AtomEquivalences&	ForceFieldEquivalences::getAtomEquivalences()
	{
		return atom_type_equivalences_;
	}
	
	ForceField* ForceFieldEquivalences::getForceField()
	{
		return force_field_;
	}


	bool ForceFieldEquivalences::exportParmFile(File& outfile) const
	{
		return atom_type_equivalences_.exportParmFile(outfile);
	}
	
	bool ForceFieldEquivalences::init()
	{
		// call the init() method of Parameters
		Parameters::init();
		
		// extract the AtomEquivalences section
		// set valid_ as extractSection checks for valid parameters!
		valid_ &= atom_type_equivalences_.extractSection(*this, "AtomEquivalences");

		return valid_;
	}


	bool ForceFieldEquivalences::isValid() const
	{	
		//TODO should the force_field pointer be considered?
		return (valid_ && atom_type_equivalences_.isValid() && INI_file_.isValid());
	}
	

	bool ForceFieldEquivalences::operator == (const ForceFieldEquivalences& param) const
	{
		// TODO: Do we need equality of the mother class Parameters?
		return (    (param.atom_type_equivalences_ == atom_type_equivalences_)
				     && (param.force_field_ == force_field_));
	}

} // namespace BALL

