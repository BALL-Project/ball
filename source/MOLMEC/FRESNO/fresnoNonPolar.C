// $Id: fresnoNonPolar.C,v 1.1.2.2 2002/11/12 16:26:02 anker Exp $

#include <BALL/MOLMEC/COMMON/forceField.h>

#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/PTE.h>

#include <BALL/MOLMEC/FRESNO/fresno.h>
#include <BALL/MOLMEC/FRESNO/fresnoNonPolar.h>

#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/geometricProperties.h>

#include <BALL/DATATYPE/hashMap.h>
#include <BALL/KERNEL/atomIterator.h>

using namespace std;

namespace BALL
{

	FresnoNonPolar::FresnoNonPolar()
		throw()
		:	ForceFieldComponent(),
			probe_radius_(0.0),
			surface_tension_(0.0),
			constant_(0.0),
			solvent_number_density_(0.0),
			absolute_temperature_(0.0),
			calculation_method_(0),
			verbosity_(0),
			uhlig_(),
			pcm_()
	{
		// set component name
		setName("Fresno NonPolar");
	}


	FresnoNonPolar::FresnoNonPolar(ForceField& force_field)
		throw()
		:	ForceFieldComponent(force_field),
			probe_radius_(0.0),
			surface_tension_(0.0),
			constant_(0.0),
			solvent_number_density_(0.0),
			absolute_temperature_(0.0),
			calculation_method_(0),
			verbosity_(0),
			uhlig_(),
			pcm_()
	{
		// set component name
		setName("Fresno NonPolar");
	}


	FresnoNonPolar::FresnoNonPolar(const FresnoNonPolar& fd, bool deep)
		throw()
		:	ForceFieldComponent(fd, deep),
			probe_radius_(0.0),
			surface_tension_(0.0),
			constant_(0.0),
			solvent_number_density_(0.0),
			absolute_temperature_(0.0),
			calculation_method_(0),
			verbosity_(0),
			uhlig_(),
			pcm_()
	{
	}


	FresnoNonPolar::~FresnoNonPolar()
		throw()
	{
		clear();
	}


	void FresnoNonPolar::clear()
		throw()
	{
		// ?????
		// ForceFieldComponent does not comply with the OCI
		// ForceFieldComponent::clear();
		probe_radius_ = 0.0;
		surface_tension_ = 0.0;
		constant_ = 0.0;
		solvent_number_density_ = 0.0;
		absolute_temperature_ = 0.0;
		calculation_method_ = 0;
		verbosity_ = 0;
		uhlig_.clear();
		pcm_.clear();
	}


	bool FresnoNonPolar::setup()
		throw()
	{
		ForceField* force_field = getForceField();
		if (force_field == 0)
		{
			Log.error() << "FresnoNonPolar::setup(): "
				<< "component not bound to force field." << endl;
			return false;
		}

    Options& options = force_field->options;

		probe_radius_ 
			= options.setDefaultReal(FresnoFF::Option::PROBE_RADIUS,
					FresnoFF::Default::PROBE_RADIUS);

		surface_tension_
			= options.setDefaultReal(FresnoFF::Option::SURFACE_TENSION,
					FresnoFF::Default::SURFACE_TENSION);

		constant_
			= options.setDefaultReal(FresnoFF::Option::UHLIG_CONSTANT,
					FresnoFF::Default::UHLIG_CONSTANT);

		solvent_number_density_
			= options.setDefaultReal(FresnoFF::Option::SOLVENT_NUMBER_DENSITY,
					FresnoFF::Default::SOLVENT_NUMBER_DENSITY);

		absolute_temperature_
			= options.setDefaultReal(FresnoFF::Option::ABSOLUTE_TEMPERATURE,
					FresnoFF::Default::ABSOLUTE_TEMPERATURE);

		calculation_method_ 
			= options.setDefaultInteger(FresnoFF::Option::NONPOLAR_METHOD,
					FresnoFF::Default::DESOLV_METHOD);

		verbosity_
			= options.setDefaultInteger(FresnoFF::Option::VERBOSITY,
					FresnoFF::Default::VERBOSITY);

		if (verbosity_ > 0)
		{
			switch(calculation_method_)
			{
				case CALCULATION__UHLIG:
					Log.info() << "Model " << calculation_method_ 
						<< ": Uhlig (surface tension)." << endl << endl;
					break;

				case CALCULATION__PCM:
					Log.info() << "Model " << calculation_method_ 
						<< ": PCM." << endl << endl;
					break;
			}
		}

		// we need local copies of the molecules

		System protein_system;
		Molecule protein = *(((FresnoFF*)force_field)->getProtein());
		protein_system.insert(*(new Molecule(protein, true)));

		System ligand_system;
		Molecule ligand = *(((FresnoFF*)force_field)->getLigand());
		ligand_system.insert(*(new Molecule(ligand, true)));

		System* complex_system = force_field->getSystem();

		energy_ = 0.0;

		// declare variables for the different energies
		float dG_protein = 0.0;
		float dG_ligand = 0.0;
		float dG_complex = 0.0;

		EnergyProcessor* processor;

		if (calculation_method_ == CALCULATION__UHLIG)
		{
			if (probe_radius_ != 0.0)
			{
				uhlig_.options.setInteger(UhligCavFreeEnergyProcessor::Option::PROBE_RADIUS, probe_radius_);
			}
			if (surface_tension_ != 0.0)
			{
				uhlig_.options.setInteger(UhligCavFreeEnergyProcessor::Option::SURFACE_TENSION, surface_tension_);
			}
			if (constant_ != 0.0)
			{
				uhlig_.options.setInteger(UhligCavFreeEnergyProcessor::Option::CONSTANT, constant_);
			}
			processor = &uhlig_;
		}
		else
		{
			if (calculation_method_ == CALCULATION__PCM)
			{
				if (probe_radius_ != 0.0)
				{
					pcm_.options.setInteger(PCMCavFreeEnergyProcessor::Option::PROBE_RADIUS, probe_radius_);
				}
				if (absolute_temperature_ != 0.0)
				{
					pcm_.options.setInteger(PCMCavFreeEnergyProcessor::Option::ABSOLUTE_TEMPERATURE, absolute_temperature_);
				}
				if (solvent_number_density_ != 0.0)
				{
					pcm_.options.setInteger(PCMCavFreeEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY, solvent_number_density_);
				}
				processor = &pcm_;
			}
			else
			{
				if (calculation_method_ == CALCULATION__VDW_SOLVENT)
				{
					processor = &vdw_solvent_;
				}
				else
				{
					if (calculation_method_ == CALCULATION__VDW_INTERACTION)
					{
						Log.error() << "FresnoNonPolar::setup(): "
							<< "CALCULATION__VDW_INTERACTION) not yet implemented." << endl;
						return false;
						// processor = &vdw_interaction_;
					}
					else
					{
						Log.error() << "FresnoNonPolar::setup(): "
							<< "unknown model" << endl;
						return false;
					}
				}
			}
		}

		protein_system.apply(*processor);
		dG_protein = processor->getEnergy();

		ligand_system.apply(*processor);
		dG_ligand = processor->getEnergy();

		complex_system->apply(*processor);
		dG_complex = processor->getEnergy();

		energy_ = dG_complex - (dG_protein + dG_ligand);

		// DEBUG
		Log.info() << "dG_protein = " << dG_protein << endl;
		Log.info() << "dG_ligand = " << dG_ligand << endl;
		Log.info() << "dG_complex = " << dG_complex << endl;
		Log.info() << "NONPOLAR" << calculation_method_ << ": score is " 
			<< energy_ << endl;
		// /DEBUG

		return true;

	}


	double FresnoNonPolar::updateEnergy()
		throw()
	{
		// DEBUG
		cout << "NONPOLAR: energy is " << energy_ << endl;
		// /DEBUG
		return energy_;
	}


	void FresnoNonPolar::updateForces()
		throw()
	{
	}


}
