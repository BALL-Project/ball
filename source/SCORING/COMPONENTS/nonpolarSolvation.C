// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: nonpolarSolvation.C,v 1.4 2006/05/27 09:05:23 anker Exp $

#include <BALL/SCORING/COMPONENTS/nonpolarSolvation.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/STRUCTURE/defaultProcessors.h>
#include <BALL/STRUCTURE/geometricProperties.h>
#include <BALL/DATATYPE/hashMap.h>
#include <BALL/KERNEL/atomIterator.h>
#include <BALL/MOLMEC/COMMON/radiusRuleProcessor.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/DATATYPE/string.h>

#include <BALL/SYSTEM/timer.h>

using namespace std;

namespace BALL
{

	// Define the option keywords
	const char* NonpolarSolvation::Option::PROBE_RADIUS
		= "probe_radius";
	const char* NonpolarSolvation::Option::SURFACE_TENSION
		= "surface_tension";
	const char* NonpolarSolvation::Option::UHLIG_CONSTANT
		= "uhlig_constant";
	const char* NonpolarSolvation::Option::SOLVENT_NUMBER_DENSITY
		= "solvent_number_density";
	const char* NonpolarSolvation::Option::ABSOLUTE_TEMPERATURE
		= "absolute_temperature";
	const char* NonpolarSolvation::Option::NONPOLAR_METHOD
		= "nonpolar_method";
	const char* NonpolarSolvation::Option::SOLVENT_DESCRIPTOR_FILE
		= "solvent_descriptor_file";
	const char* NonpolarSolvation::Option::LJ_PARAM_FILE
		= "lj_param_file";
	const char* NonpolarSolvation::Option::ATOM_TYPE_FILE
		= "atom_types_file";
	const char* NonpolarSolvation::Option::NONPOLAR_OVERWRITE_RADII
		= "nonpolar_overwrite_radii";
	const char* NonpolarSolvation::Option::NONPOLAR_RADIUS_RULES
		= "nonpolar_radius_rules";
	const char* NonpolarSolvation::Option::NONPOLAR_RADIUS_SCALING
		= "nonpolar_radius_scaling";

	// Define the default values
	const float NonpolarSolvation::Default::PROBE_RADIUS
		= 0;
	const float NonpolarSolvation::Default::SURFACE_TENSION
		= 0;
	const float NonpolarSolvation::Default::UHLIG_CONSTANT
		= 0;
	const float NonpolarSolvation::Default::SOLVENT_NUMBER_DENSITY
		= 0;
	const float NonpolarSolvation::Default::ABSOLUTE_TEMPERATURE
		= 0;
	const Size NonpolarSolvation::Default::NONPOLAR_METHOD
		= 0;
	const String NonpolarSolvation::Default::SOLVENT_DESCRIPTOR_FILE
		= "solvents/PCM-water.ini";
	const String NonpolarSolvation::Default::LJ_PARAM_FILE
		= "Amber/amber94.ini";
	const String NonpolarSolvation::Default::ATOM_TYPE_FILE
		= "Amber/amber94.types";
	const bool NonpolarSolvation::Default::NONPOLAR_OVERWRITE_RADII
		= false;
	const String NonpolarSolvation::Default::NONPOLAR_RADIUS_RULES
		= "solvation/bondi.rul";
	const float NonpolarSolvation::Default::NONPOLAR_RADIUS_SCALING
		= 1.0f;


	NonpolarSolvation::NonpolarSolvation()

		:	ScoringComponent(),
			verbosity_(0),
			factor_(0.0),
			calculation_method_(0),
			probe_radius_(0.0),
			surface_tension_(0.0),
			constant_(0.0),
			solvent_number_density_(0.0),
			absolute_temperature_(0.0),
			uhlig_(),
			pcm_(),
			vdw_solvent_()
	{
		// set component name
		setName("Nonpolar Solvation");
	}


	NonpolarSolvation::NonpolarSolvation(ScoringFunction& sf)

		:	ScoringComponent(sf),
			verbosity_(0),
			factor_(0.0),
			calculation_method_(0),
			probe_radius_(0.0),
			surface_tension_(0.0),
			constant_(0.0),
			solvent_number_density_(0.0),
			absolute_temperature_(0.0),
			uhlig_(),
			pcm_(),
			vdw_solvent_()
	{
		// set component name
		setName("Nonpolar Solvation");
	}


	NonpolarSolvation::NonpolarSolvation(const NonpolarSolvation& fd)

		:	ScoringComponent(fd),
			verbosity_(fd.verbosity_),
			factor_(fd.factor_),
			calculation_method_(fd.calculation_method_),
			probe_radius_(fd.probe_radius_),
			surface_tension_(fd.surface_tension_),
			constant_(fd.constant_),
			solvent_number_density_(fd.solvent_number_density_),
			absolute_temperature_(fd.absolute_temperature_),
			uhlig_(fd.uhlig_),
			pcm_(fd.pcm_),
			vdw_solvent_(fd.vdw_solvent_)
	{
	}


	NonpolarSolvation::~NonpolarSolvation()
	{
		clear();
	}


	void NonpolarSolvation::clear()
	{
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


	bool NonpolarSolvation::setup()

	{

		Timer timer;
		timer.start();

		if (getScoringFunction() == 0)
		{
			Log.error() << "NonpolarSolvation::setup(): "
				<< "component not bound to scoring function." << endl;
			return false;
		}

		Options* options = getScoringFunction()->getOptionsToModify();

		probe_radius_
			= options->setDefaultReal(NonpolarSolvation::Option::PROBE_RADIUS,
					NonpolarSolvation::Default::PROBE_RADIUS);

		surface_tension_
			= options->setDefaultReal(NonpolarSolvation::Option::SURFACE_TENSION,
					NonpolarSolvation::Default::SURFACE_TENSION);

		constant_
			= options->setDefaultReal(NonpolarSolvation::Option::UHLIG_CONSTANT,
					NonpolarSolvation::Default::UHLIG_CONSTANT);

		solvent_number_density_
			= options->setDefaultReal(NonpolarSolvation::Option::SOLVENT_NUMBER_DENSITY,
					NonpolarSolvation::Default::SOLVENT_NUMBER_DENSITY);

		absolute_temperature_
			= options->setDefaultReal(NonpolarSolvation::Option::ABSOLUTE_TEMPERATURE,
					NonpolarSolvation::Default::ABSOLUTE_TEMPERATURE);

		calculation_method_
			= options->setDefaultInteger(NonpolarSolvation::Option::NONPOLAR_METHOD,
					NonpolarSolvation::Default::NONPOLAR_METHOD);

		verbosity_
			= options->setDefaultInteger(ScoringFunction::Option::VERBOSITY,
					ScoringFunction::Default::VERBOSITY);

		String solvent_descriptor_file
			= options->setDefault(NonpolarSolvation::Option::SOLVENT_DESCRIPTOR_FILE,
					NonpolarSolvation::Default::SOLVENT_DESCRIPTOR_FILE);

		String lj_param_file
			= options->setDefault(NonpolarSolvation::Option::LJ_PARAM_FILE,
					NonpolarSolvation::Default::LJ_PARAM_FILE);

		String atom_type_file
			= options->setDefault(NonpolarSolvation::Option::ATOM_TYPE_FILE,
					NonpolarSolvation::Default::ATOM_TYPE_FILE);

		bool overwrite_radii
			= options->setDefaultBool(NonpolarSolvation::Option::NONPOLAR_OVERWRITE_RADII,
					NonpolarSolvation::Default::NONPOLAR_OVERWRITE_RADII);

		String tmp;
		if (overwrite_radii)
		{
			String filename
				= options->setDefault(NonpolarSolvation::Option::NONPOLAR_RADIUS_RULES,
						NonpolarSolvation::Default::NONPOLAR_RADIUS_RULES);
			cout << filename << endl;
			Path path;
			tmp = path.find(filename);
			if (tmp == "") tmp = filename;
			if (verbosity_ > 0)
			{
				Log.info() << "NonpolarSolvation: using radius rule file " << tmp
					<< std::endl;
			}
		}
		INIFile radius_rule_ini(tmp);
		radius_rule_ini.read();
		RadiusRuleProcessor radius_rules(radius_rule_ini);

		System protein_system;
		Molecule* protein = new Molecule(*(getScoringFunction()->getReceptor()), true);
		if (overwrite_radii)
		{
			protein->apply(radius_rules);
		}

		float scaling_factor = options->setDefaultReal(NonpolarSolvation::Option::NONPOLAR_RADIUS_SCALING, NonpolarSolvation::Default::NONPOLAR_RADIUS_SCALING);

		if (scaling_factor != 1.0f)
		{
			AtomIterator scale_it = protein->beginAtom();
			for (; +scale_it; ++scale_it)
			{
				scale_it->setRadius(scaling_factor * scale_it->getRadius());
				// PARANOIA
				if (scale_it->getRadius() < 0.1)
				{
					std::cout << "Found radius < 0.1: " << scale_it->getFullName()
						<< std::endl;
				}
				// PARANOIA
			}
		}

		Molecule* protein_copy = new Molecule(*protein, true);
		receptor_.insert(*protein);
		complex_.insert(*protein_copy);

		System ligand_system;
		Molecule* ligand = new Molecule(*(getScoringFunction()->getLigand()), true);
		if (overwrite_radii)
		{
			ligand->apply(radius_rules);
		}

		if (scaling_factor != 1.0f)
		{
			cout << "Scaling nonpolar radii by " << scaling_factor << endl;
			AtomIterator scale_it = ligand->beginAtom();
			for (; +scale_it; ++scale_it)
			{
				scale_it->setRadius(scaling_factor * scale_it->getRadius());
				// PARANOIA
				if (scale_it->getRadius() < 0.1)
				{
					std::cout << "Found radius < 0.1: " << scale_it->getFullName()
						<< std::endl;
				}
				// PARANOIA
			}
		}

		Molecule* ligand_copy = new Molecule(*ligand, true);
		ligand_.insert(*ligand);
		complex_.insert(*ligand_copy);

		if (verbosity_ > 1)
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

		score_ = 0.0;

		if (calculation_method_ == CALCULATION__UHLIG)
		{
			if (probe_radius_ != 0.0)
			{
				uhlig_.options.setReal(UhligCavFreeEnergyProcessor::Option::PROBE_RADIUS, probe_radius_);
			}
			if (surface_tension_ != 0.0)
			{
				uhlig_.options.setReal(UhligCavFreeEnergyProcessor::Option::SURFACE_TENSION, surface_tension_);
			}
			if (constant_ != 0.0)
			{
				uhlig_.options.setReal(UhligCavFreeEnergyProcessor::Option::CONSTANT, constant_);
			}
			processor_ = &uhlig_;
		}
		else
		{
			if (calculation_method_ == CALCULATION__PCM)
			{
				if (probe_radius_ != 0.0)
				{
					pcm_.options.setReal(PCMCavFreeEnergyProcessor::Option::PROBE_RADIUS, probe_radius_);
				}
				if (absolute_temperature_ != 0.0)
				{
					pcm_.options.setReal(PCMCavFreeEnergyProcessor::Option::ABSOLUTE_TEMPERATURE, absolute_temperature_);
				}
				if (solvent_number_density_ != 0.0)
				{
					pcm_.options.setReal(PCMCavFreeEnergyProcessor::Option::SOLVENT_NUMBER_DENSITY, solvent_number_density_);
				}
				processor_ = &pcm_;
			}
			else
			{
				if (calculation_method_ == CALCULATION__VDW_SOLVENT)
				{
					if (solvent_descriptor_file != "")
					{
						vdw_solvent_.options.set(Pair6_12InteractionEnergyProcessor::Option::VERBOSITY, verbosity_);
						vdw_solvent_.options.set(Pair6_12InteractionEnergyProcessor::Option::SOLVENT_FILENAME, solvent_descriptor_file);
						vdw_solvent_.options.set(Pair6_12InteractionEnergyProcessor::Option::LJ_FILENAME, lj_param_file);
					}
					processor_ = &vdw_solvent_;
				}
				else
				{
					Log.error() << "NonpolarSolvation::setup(): "
						<< "unknown model" << endl;
					return false;
				}
			}
		}

		timer.stop();
		if (verbosity_ > 1)
		{
			Log.info() << "NonpolarSolvation::setup() "
				<< timer.getCPUTime() << " s" << std::endl;
		}

		return(true);
	}


	void NonpolarSolvation::update(const vector<std::pair<Atom*, Atom*> >& pair_vector)
	{
	}


	double NonpolarSolvation::updateScore()
	{
		Timer timer;
		timer.start();

		// declare variables for the different energies
		float dG_protein = 0.0;
		float dG_ligand = 0.0;
		float dG_complex = 0.0;

		// We need local copies of the molecules. The pointers returned by
		// ScoringFunction::get*() should be non-zero if setup() was
		// successful, so we don't check for NULL pointers here.

		// Because we have local copies, we need to update the atom postition
		// for our molecules.
		AtomConstIterator src = getScoringFunction()->getReceptor()->beginAtom();
		AtomIterator dst = receptor_.beginAtom();
		// This for-loop assumes that both systems are still of same size and
		// that atoms are still in the same order. No checking done on this!
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}

		src = getScoringFunction()->getLigand()->beginAtom();
		dst = ligand_.beginAtom();
		for (; +src && +dst; ++src, ++dst)
		{
			dst->setPosition(src->getPosition());
		}

		receptor_.apply(*processor_);
		dG_protein = processor_->getEnergy();

		ligand_.apply(*processor_);
		dG_ligand = processor_->getEnergy();

		complex_.apply(*processor_);
		dG_complex = processor_->getEnergy();

		score_ = dG_complex - (dG_protein + dG_ligand);

		if (verbosity_ > 8)
		{
			Log.info() << "dG_protein = " << dG_protein << endl;
			Log.info() << "dG_ligand = " << dG_ligand << endl;
			Log.info() << "dG_complex = " << dG_complex << endl;
			Log.info() << "NONPOLAR" << calculation_method_ << ": score is "
				<< score_ << endl;
		}

		timer.stop();
		if (verbosity_ > 1)
		{
			Log.info() << "NonpolarSolvation::calculateScore() "
				<< timer.getCPUTime() << " s" << std::endl;
		}

		if (verbosity_ > 1)
		{
			Log.info() << "NONPOLAR" << calculation_method_ << ": energy is "
				<< score_ << endl;
		}

		return score_;
	}
}
