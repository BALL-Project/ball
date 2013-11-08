// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/PLPScoring.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/SCORING/COMPONENTS/PLP.h>

using namespace BALL;
using namespace std;

PLPScoring::PLPScoring(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: ScoringFunction(receptor, ligand, options)
{
	setup();
	// TODO: this used to be 'setLigand(ligand_)', which, imho does not make sense, since
	// we are getting a ligand via parameter...
	setLigand(&ligand); //sets PLP::rotatable_bonds_
}


PLPScoring::PLPScoring(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: ScoringFunction(receptor, hashgrid_origin, options)
{
	setup();
}


void PLPScoring::setup()
{
	ScoringFunction::setName("PLPScoring");

	PLP* gh_ligand = new PLP; // create new Piecewise Linear Potential component for ligand conformation evaluation
	PLP* gh_reclig = new PLP; // create new PLP for receptor-ligand interaction

	gh_ligand->ScoringComponent::setName("Conformation NonBonded");
	gh_ligand->setScoringFunction(*this);
	gh_ligand->setLigandIntraMolecular(1);
	gh_ligand->setup(options_);
	gh_ligand->setCoefficient(0.005);
	scoring_components_.push_back(gh_ligand);

	gh_reclig->setLigandIntraMolecular(0);
	gh_reclig->setScoringFunction(*this);
	gh_reclig->ScoringComponent::setName("Rec-Lig NonBonded");
	gh_reclig->setup(options_);
	gh_reclig->setNormalizationParameters(48.7759, 15.239);
	gh_reclig->setCoefficient(0.0991334);
	scoring_components_.push_back(gh_reclig);

	setNormalizationParameters(14.7216, -34.358);

	// has to used because we need the StaticLigandFragment calculation in order to obtain the rotatable bonds for the PLP-components
	use_static_lig_fragments_ = 1;
}


void PLPScoring::setLigand(AtomContainer* sys)
{
	// make sure that the calculation of static-fragment energy does not include score for rotatable ligand bonds
	vector<Bond*> empty(0);
	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		PLP* gh_component = dynamic_cast<PLP*>(*it);
		if (gh_component != NULL)
		{
			gh_component->setRotatableBonds(empty);
		}
	}

	// ScoringFunction::setLigand() will assign the new ligand, calculate the static-fragment energy and find all rotatable bonds of the new ligand
	ScoringFunction::setLigand(*sys);
	cout<<"static_ligand_energy_ = "<<static_ligand_energy_<<endl;
	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		PLP* gh_component = dynamic_cast<PLP*>(*it);
		if (gh_component != NULL)
		{
			gh_component->setRotatableBonds(rotatable_ligand_bonds_);
		}
	}
}

BALL::AtomTypes& PLPScoring::getAtomTypes()
{
	throw BALL::Exception::GeneralException(__FILE__, __LINE__, "PLPScoring error", "Use PLPScoring::setAtomType() instead of PLPScoring::getAtomTypes()");
	return atom_types_;
}


double PLPScoring::getES()
{
	double es = 0;

	for (vector<ScoringComponent*> ::iterator it = scoring_components_.begin(); it != scoring_components_.end(); ++it)
	{
		PLP* anb = dynamic_cast<PLP*>(*it);
		if ((*it)->isLigandIntraMolecular() && anb != NULL)
		{
			es += anb->getESEnergy();
		}
	}

	return es;
}


void PLPScoring::setAtomType(Atom* atom, const String& type_name)
{
	if (type_name != "0" && type_name != "1" && type_name != "2" &&
		  type_name!="3" && type_name!="4")
	{
		String mess = "'"+type_name+"' is no valid Gehlhaar AtomType!";
		throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Gehlhaar AtomType error", mess);
	}
	int type = type_name.toInt();
	atom->setType(type);
}
