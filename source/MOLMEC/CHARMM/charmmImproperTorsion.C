// $Id: charmmImproperTorsion.C,v 1.7 2000/06/30 05:56:14 oliver Exp $

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
	}


	// constructor
	CharmmImproperTorsion::CharmmImproperTorsion(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "CHARMM ImproperTorsion" );
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

		// iterate over all residues and try to assign the impropers
		Residue* last_residue = 0;
		ResidueIterator res_it = getForceField()->getSystem()->beginResidue();
		ResidueIterator next_it = res_it;

		for (; +res_it; last_residue = &*res_it, ++res_it)
		{
			// we keep an iterator that precedes res_it by one
			// this usually points to the next residue
			if (+next_it)
			{
				++next_it;
			}

			// retrieve the residue name
			String res_name = res_it->getFullName();

			// if we have impropers for this residue, iterate over all
			ResidueTorsions::Data torsion;
			for (Size i = 0; i < improper_atoms_.getNumberOfResidueTorsions(res_name); i++)
			{
				// retrieve the names for the four atoms
				improper_atoms_.assignTorsion(res_name, i, torsion);
				
				// create pointers to the atoms
				Atom* a1 = 0;
				Atom* a2 = 0;
				Atom* a3 = 0;
				Atom* a4 = 0;
				
				// iterate over all atoms of this residue and try
				// to identify the atoms we're looking for
				AtomIterator atom_it = res_it->beginAtom();
				for (; +atom_it; ++atom_it)
				{
					if ((a1 == 0) && (torsion.atom_name_A == atom_it->getName()))
					{
						a1 = &*atom_it;
					}
					if ((a2 == 0) && (torsion.atom_name_B == atom_it->getName()))
					{
						a2 = &*atom_it;
					}
					if ((a3 == 0) && (torsion.atom_name_C == atom_it->getName()))
					{
						a3 = &*atom_it;
					}
					if ((a4 == 0) && (torsion.atom_name_D == atom_it->getName()))
					{
						a4 = &*atom_it;
					}
				}

				// if some of the atom pointers are 0, we have to look in the
				// neighbouring residues for their names 
				// FIRST: next residue, if next_it is a valid iterator
				// and both iterators are on the same chain
				if (((a1 == 0) || (a2 == 0) || (a3 == 0) || (a4 == 0)) 
						&& +next_it
						&& (next_it->getChain() == res_it->getChain()))
				{
					for (atom_it = next_it->beginAtom(); +atom_it; ++atom_it)
					{
						if ((a1 == 0) && (torsion.atom_name_A == "+" + atom_it->getName()))
						{
							a1 = &*atom_it;
						}
						if ((a2 == 0) && (torsion.atom_name_B == "+" + atom_it->getName()))
						{
							a2 = &*atom_it;
						}
						if ((a3 == 0) && (torsion.atom_name_C == "+" + atom_it->getName()))
						{
							a3 = &*atom_it;
						}
						if ((a4 == 0) && (torsion.atom_name_D == "+" + atom_it->getName()))
						{
							a4 = &*atom_it;
						}
					}
				}

				// if some of the atom pointers are 0, we have to look in the
				// neighbouring residues for their names 
				// SECOND: last residue, if the last residue is not a null pointer
				//         and the two residues have to be on the same chain
				if (((a1 == 0) || (a2 == 0) || (a3 == 0) || (a4 == 0)) 
						&& (last_residue != 0)
						&& (last_residue->getChain() == res_it->getChain()))
				{
					for (atom_it = last_residue->beginAtom(); +atom_it; ++atom_it)
					{
						if ((a1 == 0) && (torsion.atom_name_A == "-" + atom_it->getName()))
						{
							a1 = &*atom_it;
						}
						if ((a2 == 0) && (torsion.atom_name_B == "-" + atom_it->getName()))
						{
							a2 = &*atom_it;
						}
						if ((a3 == 0) && (torsion.atom_name_C == "-" + atom_it->getName()))
						{
							a3 = &*atom_it;
						}
						if ((a4 == 0) && (torsion.atom_name_D == "-" + atom_it->getName()))
						{
							a4 = &*atom_it;
						}
					}
				}
	
				// now we might have to look for CYS disulphide bridges
				if (((a1 == 0) || (a2 == 0) || (a3 == 0) || (a4 == 0))
						&& res_it->hasProperty(Residue::PROPERTY__HAS_SSBOND))
				{
					// identify the residue the disulphide bridge goes to
					Residue* cys_res = 0;
					for (atom_it = res_it->beginAtom(); (cys_res != 0) && +atom_it; ++atom_it)
					{
						if (atom_it->getName() == "SG")
						{
							Atom::BondIterator bond_it = atom_it->beginBond();
							for (; +bond_it; ++bond_it)
							{
								if (bond_it->getPartner(*atom_it)->getName() == "SG")
								{
									cys_res = bond_it->getPartner(*atom_it)->getAncestor(RTTI::getDefault<Residue>());
									break;
								}
							}
						}
					}
					
					// if we found anothe CYS, look for atoms there
					// do this only once for each pair (use the residue handles to decide)
					if (cys_res != 0) 
					{
						// if we had this CYS already, we continue...
						//
						if ((cys_res->getHandle() < res_it->getHandle()))
						{
							continue;
						}

						// look for names
						for (atom_it = last_residue->beginAtom(); +atom_it; ++atom_it)
						{
							if ((a1 == 0) && (torsion.atom_name_A == "=" + atom_it->getName()))
							{
								a1 = &*atom_it;
							}
							if ((a2 == 0) && (torsion.atom_name_B == "=" + atom_it->getName()))
							{
								a2 = &*atom_it;
							}
							if ((a3 == 0) && (torsion.atom_name_C == "=" + atom_it->getName()))
							{
								a3 = &*atom_it;
							}
							if ((a4 == 0) && (torsion.atom_name_D == "=" + atom_it->getName()))
							{
								a4 = &*atom_it;
							}
						}
					}
				}

				// warn the user 
				if ((a1 == 0) || (a2 == 0) || (a3 == 0) || (a4 == 0))
				{
					Log.warn() << "CharmmImproperTorsion::setup: could not find improper torsion for " << torsion.residue_name 
										 << ":" << torsion.atom_name_A << "-" << torsion.atom_name_B << "-" << torsion.atom_name_C
										 << "-" << torsion.atom_name_D << endl;
					Log.warn() << "  Atoms found: ";

					if (a1 != 0) Log.warn() << " a1 = " << a1->getFullName();
					if (a2 != 0) Log.warn() << " a2 = " << a2->getFullName();
					if (a3 != 0) Log.warn() << " a3 = " << a3->getFullName();
					if (a4 != 0) Log.warn() << " a4 = " << a4->getFullName();
					Log.warn() << endl;

				} else {
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

						QuadraticImproperTorsion::Data values;
						values.atom1 = a1;
						values.atom2 = a2;
						values.atom3 = a3;
						values.atom4 = a4;

						// retrieve the parameters
						// permute the three atoms B, C, and D (atom1 is fixed)
						// and try wildcards for atoms C and D (permuting atom4)
						bool found = false;
						if (improper_parameters_.assignParameters(values.values, type_a1, type_a2, type_a3, type_a4)) 
						{
							found = true;
						}
						else if (improper_parameters_.assignParameters(values.values, type_a1, type_a2, type_a4, type_a3)) 
						{
							found = true;
						} 
						if (improper_parameters_.assignParameters(values.values, type_a1, type_a3, type_a2, type_a4)) 
						{
							found = true;
						}
						else if (improper_parameters_.assignParameters(values.values, type_a1, type_a3, type_a4, type_a2)) 
						{
							found = true;
						} 
						if (improper_parameters_.assignParameters(values.values, type_a1, type_a4, type_a2, type_a3)) 
						{
							found = true;
						}
						else if (improper_parameters_.assignParameters(values.values, type_a1, type_a4, type_a3, type_a2)) 
						{
							found = true;
						} 
						else if (improper_parameters_.assignParameters(values.values, type_a1, Atom::ANY_TYPE, Atom::ANY_TYPE, type_a2))
						{
							found = true;
						} 
						else if (improper_parameters_.assignParameters(values.values, type_a1, Atom::ANY_TYPE, Atom::ANY_TYPE, type_a3))
						{
							found = true;
						} 
						else if (improper_parameters_.assignParameters(values.values, type_a1, Atom::ANY_TYPE, Atom::ANY_TYPE, type_a4))
						{
							found = true;
						} 

						if (found)	
						{
							impropers_.push_back(values);
						} else {
							Log.warn() << "CharmmImproperTorsion::setup: no parameters for improper " 
												 << torsion.residue_name << "/" << torsion.atom_name_A << "/" 
												 << torsion.atom_name_B << "/" << torsion.atom_name_C << "/" 
												 << torsion.atom_name_D << " (types are " 
												 << a1->getTypeName() << "/" << a2->getTypeName() << "/"
												 << a3->getTypeName() << "/" << a4->getTypeName()												
												 << ")" << endl;
						}
					}
				}
			}
		}

		return true;
	}

	// calculates the current energy of this component
	double CharmmImproperTorsion::updateEnergy() 
	{
		double cosphi;

		Vector3	bc;
		Vector3 ba;
		Vector3 bd;
		Vector3 bcxbd;
		Vector3 bcxba;

		energy_ = 0;

		vector<QuadraticImproperTorsion::Data>::const_iterator it = impropers_.begin(); 

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

				double length_bcxba = bcxba.getLength();
				double length_bcxbd = bcxbd.getLength();


				if (length_bcxba != 0 && length_bcxbd != 0) 
				{
					bcxba /= length_bcxba;
					bcxbd /= length_bcxbd;

					cosphi = bcxba * bcxbd;

					// avoid problems with doubleing point precision
					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

					double tmp = (acos(cosphi) - it->values.phase);
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

		vector<QuadraticImproperTorsion::Data>::const_iterator it = impropers_.begin(); 

		for (; it != impropers_.end(); it++) 
		{
			if (getForceField()->getUseSelection() == false ||
					( getForceField()->getUseSelection() == true &&
					(it->atom1->isSelected() || it->atom2->isSelected() || it->atom3->isSelected() || it->atom4->isSelected())))
			{

				// A is the central atom
				// phi is the angle between the two plane defined by ABC and BCD

				// we define the difference vectors ba, bc, and bd
				ba = it->atom2->getPosition() - it->atom1->getPosition();
				bc = it->atom2->getPosition() - it->atom3->getPosition();
				bd = it->atom2->getPosition() - it->atom4->getPosition();

				// calculate the cross products
				// (plane normals)
				bcxba = bc % ba;
				bcxbd = bc % bd; 

				float length_bcxba = bcxba.getLength();
				float length_bcxbd = bcxbd.getLength();


				if (length_bcxba != 0 && length_bcxbd != 0) 
				{
					// normalize the plane normals
					float ilength_bcxba = 1/length_bcxba;
					float ilength_bcxbd = 1/length_bcxbd;
					bcxba *= ilength_bcxba;
					bcxbd *= ilength_bcxbd;

					// cos(phi) is the scalar product of
					// the normalized plane normals
					cosphi = bcxba * bcxbd;

					// to avoid problems with floating point accuracy
					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}
					
					float cosphi2 = cosphi * cosphi;
					float sinphi;
					if (cosphi2 <= 1.0)
					{
						sinphi = sqrt(1 - cosphi2);
					} else {
						sinphi = -2;	
					}

					if (sinphi > 0)
					{
						// conversion from kJ/mol -> J (forces are returned in units of 1N = 1 J/m)
						//   1e10			: Angstrom->m
						//   1e3			: kJ -> j
						//   AVOGADRO : mol -> 1
						float factor = -2.0 * (1e13 / Constants::AVOGADRO) 
														* it->values.k * (acos(cosphi) - it->values.phase) / sinphi;

						float ilength_bcxba_2 = ilength_bcxba * ilength_bcxba;
						float ilength_bcxbd_2 = ilength_bcxbd * ilength_bcxbd;
						float bcba = bc * ba;
						float bcbd = bc * bd;
						float babd = ba * bd;
						float ba2  = ba * ba;
						float bc2  = bc * bc;
						float bd2  = bd * bd;

						Vector3 dcosphi_dbc = cosphi * (ilength_bcxbd_2 * (bcbd * bd - bd2 * bc) 
																						+ (ilength_bcxba_2 *(bcba * ba - ba2 * bc)))
																	+	((ilength_bcxba * ilength_bcxbd) * ((2 * babd) * bc - bcba * bd - bcbd * ba));
						Vector3 dcosphi_dbd = (cosphi * (ilength_bcxbd_2) * (bcbd * bc - bc2 * bd) 
																	 + ((ilength_bcxba * ilength_bcxbd) * (bc2 * ba - bcba * bc)));
						Vector3 dcosphi_dba = (cosphi * (ilength_bcxba_2) * (bcba * bc - bc2 * ba) 
																	 +	((ilength_bcxba * ilength_bcxbd) * (bc2 * bd - bcbd * bc)));

						// add the forces to the atoms' force vectors
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
