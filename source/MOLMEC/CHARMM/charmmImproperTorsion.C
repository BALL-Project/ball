// $Id: charmmImproperTorsion.C,v 1.1 2000/02/06 19:57:52 oliver Exp $

#include <BALL/MOLMEC/CHARMM/charmmImproperTorsion.h>
#include <BALL/MOLMEC/CHARMM/charmm.h>
#include <BALL/MOLMEC/COMMON/forceFieldComponent.h>
#include <BALL/MOLMEC/COMMON/forceField.h>
#include <BALL/KERNEL/atom.h>
#include <BALL/KERNEL/bond.h>

#include <algorithm>

using namespace std;

namespace BALL 
{

	// default constructor
	CharmmImproperTorsion::CharmmImproperTorsion()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("CHARMM ImproperTorsion");

		setForceField(0);
	}


	// constructor
	CharmmImproperTorsion::CharmmImproperTorsion(ForceField* force_field)
		:	ForceFieldComponent()
	{
		// set component name
		setName( "CHARMM ImproperTorsion" );

		setForceField(force_field);
	}


	// copy constructor
	CharmmImproperTorsion::CharmmImproperTorsion(const CharmmImproperTorsion&	component, bool clone_deep)
		:	ForceFieldComponent(component, clone_deep)
	{
		// assign the torsion array
		impropers_ = component.impropers_;
	}

	// destructor
	CharmmImproperTorsion::~CharmmImproperTorsion()
	{
		// clear the torsion array
		impropers_.clear();
	}


	// setup the internal datastructures for the component
	bool CharmmImproperTorsion::setup()
	{
		if (getForceField() == 0) 
		{
			Log.error() << "CharmmImproperTorsion::setup: component not bound to force field" << endl;
			return false;
		}

		// clear torsion array
		impropers_.clear();

		// extract the torsion parameters from the parameter file
		bool result;
		CharmmFF* charmm_force_field = dynamic_cast<CharmmFF*>(force_field_);
		bool has_initialized_parameters = true;
		if ((charmm_force_field == 0) || !charmm_force_field->hasInitializedParameters())
		{
			has_initialized_parameters = false;
		}

		if (!has_initialized_parameters)
		{
			result = improper_parameters_.extractSection(getForceField()->getParameters(), "ImproperTorsions");

			if (result == false) 
			{
				Log.error() << "cannot find section ImproperTorsions in parameter file" << endl;
				return false;
			}
		}
	

		// Improper torsions will be added to the torsion array
		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();

		// find all improper torsion atoms: their names are stored in 
		// the section ResidueImproperTorsions
		if (!has_initialized_parameters)
		{
			result = improper_atoms_.extractSection(getForceField()->getParameters(), "ResidueImproperTorsions");
			if (result == false)
			{
				Log.error() << "cannot find section ResidueImproperTorsions" << endl;
				return false;
			}
		}
		
		// check for each potential improper torsion atom (every atom having three bonds)
		// whether it is contained in the list of impropers
		for ( ; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			if ((*atom_it)->countBonds() == 3)
			{
				String res_name;
				int terminal = 0;
				Residue* res = (*atom_it)->getAncestor(RTTI::getDefault<Residue>());
				if (res == 0)
				{
					if ((*atom_it)->getFragment() != 0)
					{
						res_name = (*atom_it)->getFragment()->getName();
						res_name.trim();
					}
				} else {
					res_name = res->getName();
					res_name.trim();

					String suffix = "-";
					if (res->isNTerminal())
					{
						suffix = "-N";
					}
					if (res->isCTerminal())
					{
						suffix = "-C";
					}
					if (res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
					{
						suffix += "S";
					}
					if (suffix != "-")
					{
						res_name += suffix;
					}
				}

				String key;
				if (res_name != "")
				{
					key = res_name + " " + (*atom_it)->getName();
					switch (terminal)
					{
						case 1:		key += " N"; break;
						case 2:		key += " C"; break;
						default:	key += " -";
					}
				}

				if (improper_atoms_.has(key))
				{
						
					// identify the three remaining atoms 
					// BAUSTELLE
					Atom* a1 = 0;
					Atom* a2 = 0;
					Atom* a3 = 0;
					Atom* a4 = 0;

					FFPSQuadraticImproperTorsion::Torsion values;
					values.atom1 = a1;
					values.atom2 = a2;
					values.atom3 = a3;
					values.atom4 = a4;

					// if we use selection and the atoms are selected,
					// add the torsion the the impropers_ vector
					if	(getForceField()->getUseSelection() == false ||
							 (getForceField()->getUseSelection() == true &&
								(a1->isSelected() && a2->isSelected() && a3->isSelected() && a4->isSelected())))
					{
						Atom::Type type_a1 = a1->getType();
						Atom::Type type_a2 = a2->getType();
						Atom::Type type_a3 = a3->getType();
						Atom::Type type_a4 = a4->getType();

						bool found = false;

						if (improper_parameters_.hasParameters(type_a1, type_a2, type_a3, type_a4)) 
						{
							improper_parameters_.assignParameters(values.values, type_a1, type_a2, type_a3, type_a4);

							found = true;
						} else if (improper_parameters_.hasParameters(Atom::ANY_TYPE, type_a2, type_a3, type_a4))
						{
							improper_parameters_.assignParameters(values.values, Atom::ANY_TYPE, type_a2, type_a3, type_a4);

							found = true;
						} else if (improper_parameters_.hasParameters(Atom::ANY_TYPE, Atom::ANY_TYPE, type_a3, type_a4)) 
						{
							improper_parameters_.assignParameters(values.values, Atom::ANY_TYPE, Atom::ANY_TYPE, type_a3, type_a4);

							found = true;
						} else if (improper_parameters_.hasParameters(Atom::ANY_TYPE, type_a2, type_a3, Atom::ANY_TYPE)) 
						{
							improper_parameters_.assignParameters(values.values, Atom::ANY_TYPE, type_a2, type_a3, Atom::ANY_TYPE);

							found = true;
						}

						if (found)	
						{
							impropers_.push_back(values);
						}
					}
				}
			}
		}

		return true;
	}

	// calculates the current energy of this component
	float CharmmImproperTorsion::updateEnergy() 
	{
		float cosphi;

		Vector3	bc;
		Vector3 ba;
		Vector3 bd;
		Vector3 bcxbd;
		Vector3 bcxba;

		energy_ = 0;

		vector<FFPSQuadraticImproperTorsion::Torsion>::const_iterator it = impropers_.begin(); 

		for ( ; it != impropers_.end(); it++) 
		{
			if ( getForceField()->getUseSelection() == false ||
					( getForceField()->getUseSelection() == true &&
					(it->atom1->isSelected() || it->atom2->isSelected() || it->atom3->isSelected() || it->atom4->isSelected())))
			{
				ba = it->atom2->getPosition() - it->atom1->getPosition();
				bc = it->atom2->getPosition() - it->atom3->getPosition();
				bd = it->atom2->getPosition() - it->atom4->getPosition();

				bcxba = bc % ba;
				bcxbd = bc % bd; 

				float length_bcxba = bcxba.getLength();
				float length_bcxbd = bcxbd.getLength();


				// QUESTION: Only two test for the cross products seem to be necessary
				if (length_bcxba != 0 && length_bcxbd != 0) 
				{
					bcxba /= length_bcxba;
					bcxbd /= length_bcxbd;

					cosphi = bcxba * bcxbd;

					// avoid problems with floating point precision
					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

					float tmp = (acos(cosphi) - it->values.phase);
					energy_ += it->values.k * tmp * tmp;
				}
			}
		}

		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void CharmmImproperTorsion::updateForces()
	{
		float cosphi;

		Vector3	bc;
		Vector3 ba;
		Vector3 bd;
		Vector3 bcxbd;
		Vector3 bcxba;

		vector<FFPSQuadraticImproperTorsion::Torsion>::const_iterator it = impropers_.begin(); 

		for (; it != impropers_.end(); it++) 
		{
			if (getForceField()->getUseSelection() == false ||
					( getForceField()->getUseSelection() == true &&
					(it->atom1->isSelected() || it->atom2->isSelected() || it->atom3->isSelected() || it->atom4->isSelected())))
			{
				ba = it->atom2->getPosition() - it->atom1->getPosition();
				bc = it->atom2->getPosition() - it->atom3->getPosition();
				bd = it->atom2->getPosition() - it->atom4->getPosition();

				bcxba = bc % ba;
				bcxbd = bc % bd; 

				float length_bcxba = bcxba.getLength();
				float length_bcxbd = bcxbd.getLength();


				if (length_bcxba != 0 && length_bcxbd != 0) 
				{
					float ilength_bcxba = 1/length_bcxba;
					float ilength_bcxbd = 1/length_bcxbd;
					bcxba *= ilength_bcxba;
					bcxbd *= ilength_bcxbd;

					cosphi = bcxba * bcxbd;

					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}
					float cosphi2 = cosphi * cosphi;
					float tmp;

					if (cosphi2 <= 1.0)
					{
						tmp = sqrt(1 - cosphi2);
					} else {
						tmp = -2;	
					}

					if (tmp > 0)
					{
						float factor = -2.0 * it->values.k * (acos(cosphi) - it->values.phase) /tmp;

						float ilength_bcxba_2 = ilength_bcxba * ilength_bcxba;
						float ilength_bcxbd_2 = ilength_bcxbd * ilength_bcxbd;
						float bcba = bc * ba;
						float bcbd = bc * bd;
						float babd = ba * bd;
						float ba2  = ba * ba;
						float bc2  = bc * bc;
						float bd2  = bd * bd;

						Vector3 dcosphi_dbc = cosphi * (ilength_bcxbd_2 *(bcbd * bd - bd2 * bc) +   
																			 (ilength_bcxba_2 *(bcba * ba - ba2 * bc)) +							
														 ((ilength_bcxba * ilength_bcxbd) * ((2 * babd) * bc - bcba * bd - bcbd * ba)));
						Vector3 dcosphi_dbd = cosphi * ((ilength_bcxbd_2) *(bcbd * bc - bc2 * bd) +   
														 ((ilength_bcxba * ilength_bcxbd) * (bc2 * ba - bcba * bc)));
						Vector3 dcosphi_dba = cosphi * ((ilength_bcxba_2) *(bcba * bc - bc2 * ba) +   
														 ((ilength_bcxba * ilength_bcxbd) * (bc2 * bd - bcbd * bc)));

						if (getForceField()->getUseSelection() == false)
						{
							it->atom1->getForce() += factor * dcosphi_dba;
							it->atom2->getForce() -= factor * (dcosphi_dbc + dcosphi_dbd + dcosphi_dba);
							it->atom3->getForce() += factor * dcosphi_dbc;
							it->atom4->getForce() += factor * dcosphi_dbd; 
						} else {
							if (it->atom1->isSelected()) it->atom1->getForce() += factor * dcosphi_dba;
							if (it->atom2->isSelected()) it->atom2->getForce() -= factor * (dcosphi_dbc + dcosphi_dbd + dcosphi_dba);
							if (it->atom3->isSelected()) it->atom3->getForce() += factor * dcosphi_dbc;
							if (it->atom4->isSelected()) it->atom4->getForce() += factor * dcosphi_dbd;
						}
					}
				}
			}
		}
	}

} // namespace BALL
