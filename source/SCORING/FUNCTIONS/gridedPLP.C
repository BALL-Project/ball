// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/FUNCTIONS/gridedPLP.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/SCORING/COMPONENTS/PLP.h>


using namespace BALL;
using namespace std;


GridedPLP::GridedPLP(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: DiffGridBasedScoring(receptor, ligand, options)
{
	setup();
	// TODO: this used to be 'setLigand(ligand_)', which, imho does not make sense, since
	// we are getting a ligand via parameter...
	setLigand(&ligand); //sets PLP::rotatable_bonds_
}


GridedPLP::GridedPLP(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: DiffGridBasedScoring(receptor, hashgrid_origin, options)
{
	setup();
}


void GridedPLP::setup()
{
	ScoringFunction::setName("GridedPLP");

	PLP* gh_ligand = new PLP; // create new Piecewise Linear Potential component for ligand conformation evaluation
	PLP* gh_reclig = new PLP; // create new PLP for receptor-ligand interaction

	gh_ligand->setName("Conformation NonBonded");
	gh_ligand->setLigandIntraMolecular(1);
	gh_ligand->setup(options_);
	scoring_components_.push_back(gh_ligand);

	gh_reclig->setLigandIntraMolecular(0);
	gh_reclig->setName("Rec-Lig NonBonded");
	gh_reclig->setup(options_);
	scoring_components_.push_back(gh_reclig);

	set<String> types;
	types.insert("0");  // steric
	types.insert("1");  // donor
	types.insert("2");  // acceptor
	types.insert("3");  // both
	types.insert("4");  // metal
	setAtomTypeNames(types);

	// has to used because we need the StaticLigandFragment calculation in order to obtain the rotatable bonds for the PLP-components
	use_static_lig_fragments_ = 1;
}


void GridedPLP::setLigand(AtomContainer* sys)
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

BALL::AtomTypes& GridedPLP::getAtomTypes()
{
	throw BALL::Exception::GeneralException(__FILE__, __LINE__, "GridedPLP error", "Use GridedPLP::setAtomType() instead of GridedPLP::getAtomTypes()");
	return atom_types_;
}


double GridedPLP::getES()
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


void GridedPLP::setAtomType(Atom* atom, const String& type_name)
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
