// $Id: charmmTorsion.C,v 1.6 2000/06/30 05:56:15 oliver Exp $

#include <BALL/MOLMEC/CHARMM/charmmTorsion.h>
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
	CharmmTorsion::CharmmTorsion()
		:	ForceFieldComponent()
	{	
		// set component name
		setName("CHARMM Torsion");
	}


	// constructor
	CharmmTorsion::CharmmTorsion(ForceField& force_field)
		:	ForceFieldComponent(force_field)
	{
		// set component name
		setName( "CHARMM Torsion" );
	}


	// copy constructor
	CharmmTorsion::CharmmTorsion(const CharmmTorsion&	component, bool clone_deep)
		:	ForceFieldComponent(component, clone_deep)
	{
		// assign the torsion array
		torsion_ = component.torsion_;

	}

	// destructor
	CharmmTorsion::~CharmmTorsion()
	{
		// clear the torsion array
		torsion_.clear();
	}


	// setup the internal datastructures for the component
	bool CharmmTorsion::setup()
	{
		if (getForceField() == 0) 
		{
			Log.error() << "CharmmTorsion::setup: component not bound to force field" << endl;
			return false;
		}

		// clear torsion array
		torsion_.clear();

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
			// extract the torsion parameters
			result = torsion_parameters_.extractSection(getForceField()->getParameters(), "Torsions");

			if (result == false) 
			{
				Log.error() << "cannot find section Torsions" << endl;
				return false;
			}

			// check whether the torsions are contructed from the connectivity or whether
			// we read them from the ResidueTorsions section in the parameter file
			if (getForceField()->getParameters().getParameterFile().hasSection("ResidueTorsions"))
			{
				result = residue_torsions_.extractSection(getForceField()->getParameters(), "ResidueTorsions");
				if (result == false)
				{
					Log.error() << "CharmmTorsion::setup: cannot parse section Torsions" << endl;
					return false;
				}
				
				use_residue_torsion_list_ = true;
			}
		}


		// calculate the torsions
		vector<Atom*>::const_iterator atom_it = getForceField()->getAtoms().begin();
		Atom::BondIterator it1;
		Atom::BondIterator it2;
		Atom::BondIterator it3;

		Atom*	a1;
		Atom*	a2;
		Atom*	a3;
		Atom*	a4;

		// proper torsion will be added to the torsion vector
		for (; atom_it != getForceField()->getAtoms().end(); ++atom_it) 
		{
			for (it1 = (*atom_it)->beginBond(); +it1 ; ++ it1) 
			{
				if (*atom_it == it1->getFirstAtom()) 
				{
					// central atoms
					a2 = *atom_it;
					a3 = it1->getSecondAtom();

					for (it2 = (*atom_it)->beginBond(); +it2 ; ++it2) 
					{
						if ((*it2).getSecondAtom() != (*it1).getSecondAtom()) 
						{
							// determine the first atom
							if ((*it2).getFirstAtom() == *atom_it) 
							{
								a1 = (*it2).getSecondAtom();
							} 
							else 
							{
								a1 = (*it2).getFirstAtom();
							}

							for (it3 = (*it1).getSecondAtom()->beginBond(); +it3 ; ++it3) 
							{
								if ((*it3).getFirstAtom() != a2 ) 
								{
									// determine the fourth atom a4
									if ((*it3).getFirstAtom() == a3)
									{
										a4 = (*it3).getSecondAtom();
									} 
									else 
									{
										a4 = (*it3).getFirstAtom();
									}

									if (getForceField()->getUseSelection() == false ||
										(getForceField()->getUseSelection() == true &&
										(a1->isSelected() && a2->isSelected() && a3->isSelected() && a4->isSelected())))
									{
										// if we use ResidueTorsions (i.e. a list of torsions for each
										// residue is specified in the parameter file), we have to check
										// if to consider this torsion
										if (use_residue_torsion_list_)
										{
											String atom_name_A = a1->getName();
											String atom_name_B = a2->getName();
											String atom_name_C = a3->getName();
											String atom_name_D = a4->getName();
											
											// the second atom is always in the residue in question
											// 											
											// now check for the other three atoms: if it is in a different residue,
											// add a "+" or "-"
											Residue* res = a2->getAncestor(RTTI::getDefault<Residue>());
											Chain* chain = a2->getAncestor(RTTI::getDefault<Chain>());
											if (res != 0 && chain != 0)
											{
												// check whether a1, a3, and a4 are in the same residue
												// or in the previous (marked as -) or the next residue (marked as +)
												ResidueIterator res_it;
												if (a1->getAncestor(RTTI::getDefault<Chain>()) == chain)
												{
													Residue* last_res = 0;
													for (res_it = chain->beginResidue(); +res_it; last_res = &*res_it, ++res_it)
													{
														if (&*res_it == res)
														{
															if (a1->getAncestor(RTTI::getDefault<Residue>()) == last_res)
															{
																atom_name_A = "-" + atom_name_A;
															}
															if (a3->getAncestor(RTTI::getDefault<Residue>()) == last_res)
															{
																atom_name_C = "-" + atom_name_C;
															}
															if (a4->getAncestor(RTTI::getDefault<Residue>()) == last_res)
															{
																atom_name_D = "-" + atom_name_D;
															}
														}
														if (last_res == res)
														{
															if (a1->getAncestor(RTTI::getDefault<Residue>()) == &*res_it)
															{
																atom_name_A = "+" + atom_name_A;
															}
															if (a3->getAncestor(RTTI::getDefault<Residue>()) == &*res_it)
															{
																atom_name_C = "+" + atom_name_C;
															}
															if (a4->getAncestor(RTTI::getDefault<Residue>()) == &*res_it)
															{
																atom_name_D = "+" + atom_name_D;
															}
														}
													}
												}
											}

											// if we are in a CYS-S residue, all atoms that are in another CYS-S
											// are prefixed by "="
											if ((res != 0) && res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
											{	
												Residue* other_res = a1->getAncestor(RTTI::getDefault<Residue>());
												if ((other_res != 0) && (other_res != res) 
														&& other_res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
												{
													atom_name_A = "=" + atom_name_A;
												}
												other_res = a3->getAncestor(RTTI::getDefault<Residue>());
												if ((other_res != 0) && (other_res != res)
														&& other_res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
												{
													atom_name_C = "=" + atom_name_C;
												}
												other_res = a4->getAncestor(RTTI::getDefault<Residue>());
												if ((other_res != 0) && (other_res != res)
														&& other_res->hasProperty(Residue::PROPERTY__HAS_SSBOND))
												{
													atom_name_D = "=" + atom_name_D;
												}
											}
													
											// check whether the names are known in ResidueTorsions
											if (res != 0)
											{
												if (!residue_torsions_.hasTorsion(res->getFullName(), atom_name_A, atom_name_B, atom_name_C, atom_name_D))
												{
													continue;
												}
											} 
											else 
											{
												Log.warn() << "Cannot identify torsion for (UNKNOWN)" << " " << atom_name_A 
													<< "/" << atom_name_B << "/" << atom_name_C << "/" << atom_name_D << endl;

												continue;
											}
										}
										
										// search torsion parameters for (a1,a2,a3,a4)
										Atom::Type type_a1 = a1->getType();
										Atom::Type type_a2 = a2->getType();
										Atom::Type type_a3 = a3->getType();
										Atom::Type type_a4 = a4->getType();
										CosineTorsion::Values values;
										CosineTorsion::SingleData tmp;
										tmp.atom1 = a1;
										tmp.atom2 = a2;
										tmp.atom3 = a3;
										tmp.atom4 = a4;

										// retrieve parameters for the torsion
										bool found = false;
										if (torsion_parameters_.assignParameters(values, type_a1, type_a2, type_a3, type_a4)) 
										{
											found = true;
										} 
										else if (torsion_parameters_.assignParameters(values, Atom::ANY_TYPE, type_a2, type_a3, Atom::ANY_TYPE)) 
										{
											found = true;
										}

										// complain about mising parameters or store this torsion
										// in the torsion_ array
										if (found) 
										{
											for (unsigned char j = 0; j < values.n; j++) 
											{
												tmp.values = values.values[j];
												torsion_.push_back(tmp);
											}
										} 
										else 
										{
											Log.warn() << "CharmmTorsion::setup: cannot find torsion parameters for:"
												<< force_field_->getParameters().getAtomTypes().getTypeName(type_a1) << "-"
												<< force_field_->getParameters().getAtomTypes().getTypeName(type_a2) << "-"
												<< force_field_->getParameters().getAtomTypes().getTypeName(type_a3) << "-"
												<< force_field_->getParameters().getAtomTypes().getTypeName(type_a4) << endl;
										}
									}
								} 
							}
						}
					}
				}
			}
		}

		return true;
	}

	// calculates the current energy of this component
	double CharmmTorsion::updateEnergy() 
	{
		double cosphi;

		Vector3	a21;
		Vector3 a23;
		Vector3 a34;
		Vector3 cross2321;
		Vector3 cross2334;

		energy_ = 0;

		vector<SingleCharmmTorsion>::const_iterator it = torsion_.begin(); 

		for ( ; it != torsion_.end(); it++) 
		{
			if ( getForceField()->getUseSelection() == false ||
					( getForceField()->getUseSelection() == true &&
					(it->atom1->isSelected() || it->atom2->isSelected() || it->atom3->isSelected() || it->atom4->isSelected())))
			{
				a21 = it->atom1->getPosition() - it->atom2->getPosition();
				a23 = it->atom3->getPosition() - it->atom2->getPosition();
				a34 = it->atom4->getPosition() - it->atom3->getPosition();

				cross2321 = a23 % a21;
				cross2334 = a23 % a34;

				double length_cross2321 = cross2321.getLength();
				double length_cross2334 = cross2334.getLength();

				if (length_cross2321 != 0 && length_cross2334 != 0) 
				{
					cross2321 /= length_cross2321;
					cross2334 /= length_cross2334;

					cosphi = cross2321 * cross2334;

					// avoid problem with doubleing point precision
					if (cosphi > 1.0)
					{
						cosphi = 1.0;
					}
					if (cosphi < -1.0)
					{
						cosphi = -1.0;
					}

					// 
					//  E = V (1 + cos(f * phi - phase) 
					//
					// BAUSTELLE: des geht schneller fuer phase = 0|PI
					energy_ += it->V * (1 + cos(it->f * acos(cosphi) - it->phase));
				}
			}
		}
		return energy_;
	}

	// calculates and adds its forces to the current forces of the force field
	void CharmmTorsion::updateForces()
	{
		double cosphi;
		double dEdphi;

		Vector3	ab;		// vector from atom2 to atom1
		Vector3 cb;		// vector from atom2 to atom3
		Vector3 dc;		// vector from atom3 to atom4

		vector<SingleCharmmTorsion>::iterator it = torsion_.begin(); 

		for (; it != torsion_.end(); it++) 
		{
			if (getForceField()->getUseSelection() == false ||
 					(getForceField()->getUseSelection() == true &&
					(it->atom1->isSelected() || it->atom2->isSelected() 
					 || it->atom3->isSelected() || it->atom4->isSelected())))
			{
				ab = it->atom1->getPosition() - it->atom2->getPosition();
				double length_ab = ab.getLength();
				Vector3 ba = it->atom2->getPosition() - it->atom1->getPosition();
				cb = it->atom3->getPosition() - it->atom2->getPosition();
				double length_cb = cb.getLength();
				dc = it->atom4->getPosition() - it->atom3->getPosition();
				double length_dc = dc.getLength();

				if (length_ab != 0 && length_cb != 0 && length_dc != 0) 
				{
					Vector3  t = ba % cb;   // cross product of cb and ba
					Vector3  u = cb % dc;   // cross product of cb and dc

					double length_t2 = t.getSquareLength();
					double length_u2 = u.getSquareLength();

					double length_t = sqrt(length_t2);
					double length_u = sqrt(length_u2);

					if (length_t != 0 && length_u != 0) 
					{
						cosphi = (t * u) / (length_t * length_u);

						// avoid problems due to doubleing point precision
						if (cosphi > 1.0)
						{
							cosphi = 1.0;
						}
						if (cosphi < -1.0)
						{
							cosphi = -1.0;
						}

						// multiply with the barrier height and a factor
						// for unit conversion: 1e13: kJ/(mol A) -> J/(mol m)
						//  AVOGADRO: J/mol -> J
						dEdphi = (-it->V) * (1e13 / Constants::AVOGADRO) * it->f * sin(it->f * acos(cosphi) - it->phase);

            double direction = (t % u) * cb;
            if (direction > 0.0)
            {
              dEdphi = -dEdphi;
						}

						Vector3 ca = it->atom3->getPosition() - it->atom1->getPosition();
						Vector3 db = it->atom4->getPosition() - it->atom2->getPosition();
						Vector3 dEdt =   (float)(dEdphi / (length_t2 * cb.getLength())) * (t % cb);
						Vector3 dEdu = - (float)(dEdphi / (length_u2 * cb.getLength())) * (u % cb);
	

						if (getForceField()->getUseSelection() == false)
						{
							it->atom1->getForce() += dEdt % cb;
							it->atom2->getForce() += ca % dEdt + dEdu % dc;
							it->atom3->getForce() += dEdt % ba + db % dEdu;
							it->atom4->getForce() += dEdu % cb; 
						} 
						else 
						{
							if (it->atom1->isSelected()) it->atom1->getForce() += dEdt % cb;
							if (it->atom2->isSelected()) it->atom2->getForce() += ca % dEdt + dEdu % dc;
							if (it->atom3->isSelected()) it->atom3->getForce() += dEdt % ba + db % dEdu;
							if (it->atom4->isSelected()) it->atom4->getForce() += dEdu % cb;
						}
					}
				}
			}
		}
	}

} // namespace BALL
