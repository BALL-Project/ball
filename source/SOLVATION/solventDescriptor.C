// $Id: solventDescriptor.C,v 1.1 2000/08/31 16:47:23 anker Exp $

#include <BALL/SOLVATION/solventDescriptor.h>

namespace BALL
{
	SolventDescriptor::SolventDescriptor()
		:	name_(""),
			number_density_(0),
			solvent_atoms_()
	{
	}

	SolventDescriptor::SolventDescriptor(const SolventDescriptor& solvent)
		:	name_(solvent.name_),
			number_density_(solvent.number_density_),
			solvent_atoms_(solvent.solvent_atoms_)
	{
	}

	SolventDescriptor::SolventDescriptor(const String& name, 
			float number_density, 
			const std::vector<SolventAtomDescriptor>& atom_list)
		:	name_(name),
			number_density_(number_density),
			solvent_atoms_(atom_list)
	{
	}

	void SolventDescriptor::clear()
	{
		name_ = "";
		number_density_ = 0.0;
		solvent_atoms_.clear();
	}

	void SolventDescriptor::destroy()
	{
		clear();
	}

	SolventDescriptor::~SolventDescriptor()
	{
		destroy();
	}


	void SolventDescriptor::set(const SolventDescriptor& descriptor)
	{
		name_ = descriptor.name_;
		number_density_ = descriptor.number_density_;
		solvent_atoms_ = descriptor.solvent_atoms_;
	}


	const SolventDescriptor& SolventDescriptor::operator = 
		(const SolventDescriptor& descriptor)
	{
		set(descriptor);
		return *this;
	}

	void SolventDescriptor::setName(const String& name)
	{
		name_ = name;
	}

	String SolventDescriptor::getName() const
	{
		return name_;
	}

	void SolventDescriptor::setNumberDensity(float number_density)
	{
		number_density_ = number_density;
	}

	float SolventDescriptor::getNumberDensity() const
	{
		return number_density_;
	}

	void SolventDescriptor::setSolventAtomDescriptorList(const std::vector<SolventAtomDescriptor>& solvent_atoms)
	{
		solvent_atoms_ = solvent_atoms;
	}

	std::vector<SolventAtomDescriptor> SolventDescriptor::getSolventAtomDescriptorList() const
	{
		return solvent_atoms_;
	}

	Size SolventDescriptor::getNumberOfAtomTypes() const
	{
		return solvent_atoms_.size();
	}

	SolventAtomDescriptor SolventDescriptor::getAtomDescriptor(Position index) const
	{
		return solvent_atoms_[index];
	}

}
