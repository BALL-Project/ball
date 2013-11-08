// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Marcel Schumann $
// ----------------------------------------------------

#include <BALL/SCORING/COMMON/diffScoringFunction.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/SYSTEM/timer.h>


using namespace BALL;


DifferentiableScoringFunction::DifferentiableScoringFunction(AtomContainer& receptor, AtomContainer& ligand, Options& options)
	: ScoringFunction(receptor, ligand, options)
{


}


DifferentiableScoringFunction::DifferentiableScoringFunction(AtomContainer& receptor, Vector3& hashgrid_origin, Options& options)
	: ScoringFunction(receptor, hashgrid_origin, options)
{

}


// void DifferentiableScoringFunction::updateForces()
// {
// 	// make sure forces of receptor atoms are also reset to zero before starting calculation !!
// 	for (AtomIterator it = receptor_->beginAtom(); +it; ++it)
// 	{
// 		it->setForce(RTTI::getDefault<Vector3>());
// 	}
//
// 	force_field_->updateForces();
// }
//
//
// void DifferentiableScoringFunction::updateOnlyRecLigForces()
// {
// 	// make sure forces of receptor atoms are also reset to zero before starting calculation !!
// 	Vector3 v(0,0,0);
// 	for (AtomIterator it = receptor_->beginAtom(); +it; ++it)
// 	{
// 		it->setForce(v);
// 	}
// 	for (AtomIterator it = ligand_->beginAtom(); +it; ++it)
// 	{
// 		it->setForce(v);
// 	}
//
// 	int i=0;
// 	for (vector<ForceFieldComponent*>::iterator it = force_field_->getComponents().begin(); it != force_field_->getComponents().end(); ++it)
// 	{
// 		if(i!=rec_lig_component_)
// 		{
// 			(*it)->setEnabled(false);
// 		}
// 		else (*it)->setEnabled(true);
//
// 	}
//
// 	//force_field_->updateForces();
// 	cout<<"rec_lig_component_ = "<<rec_lig_component_<<endl;
// 	force_field_->getComponent(rec_lig_component_)->updateForces();
//
//
// 	i=0;
// 	for (vector<ForceFieldComponent*>::iterator it = force_field_->getComponents().begin(); it != force_field_->getComponents().end(); ++it)
// 	{
// 		(*it)->setEnabled(true);
// 	}
// }
