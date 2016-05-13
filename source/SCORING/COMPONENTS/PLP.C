// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
// ----------------------------------------------------
// $Maintainer: Marcel Schumann $
// $Authors: Jan Fuhrmann, Marcel Schumann $
// ----------------------------------------------------


#include <BALL/SCORING/COMPONENTS/PLP.h>
#include <BALL/MOLMEC/COMMON/support.h>
#include <BALL/SYSTEM/path.h>
#include <BALL/KERNEL/PTE.h>
#include <BALL/KERNEL/standardPredicates.h>
#include <map>
#include <BALL/FORMAT/HINFile.h>

using namespace std;
using namespace BALL;


/// NOTE : Since forces are not needed for a normal ScoringFunction, their calculation is disabled at the moment ! (--> speed-increase by factor 2)


bool PLP::isNSp3(const Atom* at)
{
	if (at->countBonds() < 2)
	{
		return false;
	}

	if (at->countBonds() < 3) return false;


	Atom* p1 = at->getBond(0)->getPartner(*at);
	Atom* p2 = at->getBond(1)->getPartner(*at);
	Atom* p3 = at->getBond(2)->getPartner(*at);

	Vector3 v1 = p1->getPosition() - at->getPosition();
	Vector3 v2 = p2->getPosition() - at->getPosition();
	Vector3 v3 = p3->getPosition() - at->getPosition();

	v1 = v1 % v2;

	double angle = (v1.getAngle(v3)).toDegree();
	bool b = true;

	if (angle > 85 && angle < 95) b = false;

	return b;
}


bool PLP::isSp3(const Atom* at)
{
	if (at->getElement().getName() == "Nitrogen")
		return isNSp3(at);

	if (at->getElement().getName() == "Oxygen")
	{
		return at->countBonds() != 1;
	}

	if (at->getElement().getName() == "Carbon")
	{
		return at->countBonds() == 4;
	}

	return true;
}


// Conversion from kJ / (mol A) into Newton
double FORCES_FACTOR = 1000 * 1E10 / Constants::AVOGADRO;


// ES_CONSTANT
double ES_CONSTANT = 332.0716 * Constants::JOULE_PER_CAL;


PLP::PLP()
	throw()
	:	cut_off_(20),
		vdw_cut_on_(13),
		vdw_cut_off_(15),
		es_cut_on_(13),
		es_cut_off_(15),
		dc_(1),
		n_(1),
		es_enabled_(true),
		vdw_enabled_(true)
{
	setName("Gehlhaar Energy");
}


PLP::PLP(ForceField& force_field)
	throw()
	:	ForceFieldComponent(force_field),
		cut_off_(20),
		vdw_cut_on_(13),
		vdw_cut_off_(15),
		es_cut_on_(13),
		es_cut_off_(15),
		dc_(1),
		n_(1),
		es_enabled_(true),
		vdw_enabled_(true)
{
	setName("Gehlhaar Energy");
}


PLP::PLP(const PLP& component)
	throw()
	:	ForceFieldComponent(*component.force_field_),
		cut_off_(component.cut_off_),
		vdw_cut_on_(component.vdw_cut_on_),
		vdw_cut_off_(component.vdw_cut_off_),
		es_cut_on_(component.es_cut_on_),
		es_cut_off_(component.es_cut_off_),
		dc_(component.dc_),
		n_(component.n_),
		es_enabled_(component.es_enabled_),
		vdw_enabled_(component.vdw_enabled_)
{
	type_name_ = "PLP";
	/* Why not the following ??????
	setName(component.getNamae());
	*/
}


PLP::~PLP()
	throw()
{
	clear();
}


const PLP& PLP::operator = (const PLP& anb)
	throw()
{
	// catch self assignment
	if (&anb == this) return *this;

	// call the base class operator
	ForceFieldComponent::operator = (anb);
	return *this;
}


void PLP::clear()
	throw()
{

}



bool PLP::operator == (const PLP& anb)
	throw()
{
	dc_ = anb.dc_;
	n_  = anb.n_;
	return (this == &anb);
}


void PLP::setRotatableBonds(vector<Bond*>& bonds)
{
	ghtorsions_.clear();

	for (Size x = 0; x < bonds.size(); x++)
	{
		Bond& b = *bonds[x];

		GhTorsion ght;

		ght.at1 = const_cast<Atom*>(b.getFirstAtom());

		ght.at2 = const_cast<Atom*>(b.getSecondAtom());

		if (ght.at1->countBonds() < 2 || ght.at2->countBonds() < 2)
		{
			cerr << "too few bonds for a torsion" << endl;
		}

		for (Atom::BondIterator bi = ght.at1->beginBond(); +bi; ++bi)
		{
			Atom* at = bi->getPartner(*ght.at1);
			if (at != ght.at2) ght.at3 = at;
		}

		for (Atom::BondIterator bi = ght.at2->beginBond(); +bi; ++bi)
		{
			Atom* at = bi->getPartner(*ght.at2);
			if (at != ght.at1) ght.at4 = at;
		}

		if (isSp3(ght.at1) && isSp3(ght.at2))
		{
			ght.type = 0;
		}
		else
		{
			if ( (isSp3(ght.at1) && !isSp3(ght.at2)) || (!isSp3(ght.at1) && isSp3(ght.at2)) )
			{
				ght.type = 1;
			}
			else
			{
				ght.type = 2;
			}
		}

		ghtorsions_.push_back(ght);
	}
}



Size PLP::getAtomType(const Atom* atom)
{
	String name = atom->getElement().getName();

	if (name == "Bromine") return 2;

	if (name == "Magnesium" || name == "Zinc" ||
		name == "Vanadium" || 	name == "Calcium" ||
		name == "Sodium" || name == "Potassium" ||
		name == "Manganese" || name == "Iron")
	{
		return 4;
	}

	if (name == "Oxygen")
	{
		for (Atom::BondConstIterator bi = atom->beginBond(); +bi; ++bi)
		{
			if (bi->getPartner(*atom)->getElement().getName() == "Hydrogen")
			{
				return 3;
			}
		}
		return 2;
	}


	if (atom->getElement().getName() == "Nitrogen")
	{
		Size type = 0;
		for (Atom::BondConstIterator bi = atom->beginBond(); +bi; ++bi)
		{
			if (bi->getPartner(*atom)->getElement().getName() == "Hydrogen")
			{
				type = 1;
				break;
			}
		}

		bool sp3 = isNSp3(atom);
		Size bonds = atom->countBonds();
		if (sp3 && bonds == 4 && type) type = 1;
		if (sp3 && bonds == 4 && !type) type = 0;
		if (sp3 && bonds == 3 && type) type = 3;
		if (sp3 && bonds == 3 && !type) type = 2;
		if (!sp3 && bonds == 3 && type)	type = 1;
		if (!sp3 && bonds == 3 && !type) type = 0;
		if (!sp3 && bonds == 2 && type)  type = 3;
		if (!sp3 && bonds == 2 && !type)  type = 2;

		return type;
	}

	return 0;
}



// assign interaction-types (0 or 1) for the given atom-pairs
void PLP::update(const vector<pair<Atom*, Atom*> >& atom_vector)
{
	atom_pair_vector_ = atom_vector;
	non_bonded_data_.resize(atom_pair_vector_.size());

	for (Position p = 0; p < atom_pair_vector_.size(); p++)
	{
		Atom* const atom1 = atom_pair_vector_[p].first;
		Atom* const atom2 = atom_pair_vector_[p].second;


		NonBondedPairData& data = non_bonded_data_[p];

		if (ligand_intra_molecular_)
		{
			data.type = NonBondedPairData::conformation;
			continue;
		}

		// t1 = 0 steric 1 donor 2 acceptor 3 both 4 metal
		int t1 = atom1->getType();
		int t2 = atom2->getType();

		if (t1 < 0 || t1 > 4 || t2 < 0 || t2 > 4)
		{
			String t = String(t1);
			if (t2 < 0 || t2 > 4) t = String(t2);
			String mess = "'"+t+"' is no valid Gehlhaar AtomType!";
			throw BALL::Exception::GeneralException(__FILE__, __LINE__, "Gehlhaar AtomType error", mess);
		}

		data.type = NonBondedPairData::steric;

		// t1 = 0 steric 1 donor 2 acceptor 3 both
		if ( (t1 == 1 && t2 == 2) ||
			(t1 == 1 && t2 == 3) || (t1 == 2 && t2 == 1) ||
			(t1 == 3 && t2 == 1) || (t1 == 2 && t2 == 3) ||
			(t1 == 3 && t2 == 2) || (t1 == 3 && t2 == 3) )
		{
			data.type = NonBondedPairData::hydrogen_bond;
		}

		// metal always HB
		if (t1 == 4 || t2 == 4) data.type = NonBondedPairData::hydrogen_bond;
	}
	//cout << "atom_pair_vector_.size() = "<<atom_pair_vector_.size() << endl;
}


void PLP::update()
	throw(Exception::TooManyErrors)
{
	cout<<"PLP::update() !!"<<endl;
	cout<<"Use only PLP::update(const AtomPairVector&)"<<endl;
	exit(1);
}


bool PLP::setup(Options& /*options*/)
{
	// clear vector of non-bonded atom pairs
	clear();

	es_energy_ = 0;
	vdw_energy_ = 0;

	setEnabled(true);

	if (es_enabled_ && es_cut_off_ > 0)
	{
		bool ok = es_cut_off_ > es_cut_on_ &&
				cut_off_ >= es_cut_off_;
		if (!ok )
		{
			Log.error() << "Invalid ES cuton/cutoff values!" << std::endl;
		}
		else
		{
			es_d_on_ = es_cut_on_ + 0.05;
			es_d_on2_ = es_d_on_ * es_d_on_;
			es_d_off_ = es_cut_off_ + 0.05;
			es_d_off2_ = es_d_off_ * es_d_off_;
			es_denom_ = pow((es_d_off2_ - es_d_on2_), 3);
			es_cover3_ = -(es_d_on2_ + es_d_off2_) / es_denom_;
			es_ac_ = es_d_off2_ *es_d_off2_ * (es_d_off2_ - 3. * es_d_on2_ )
				/ es_denom_;
			es_bc_ = 6. * es_d_on2_ * es_d_off2_ / es_denom_;
			es_cc_ = 3. * es_cover3_;
			es_dc_ = 2. / es_denom_;
			es_dover5_ = es_dc_ / 5.;
			es_eadd_ = (es_d_off2_ * es_d_on2_ * (es_d_off_ - es_d_on_) -
					(pow(es_d_off_, 5.) - pow(es_d_on_, 5.)) / 5.) * 8. / es_denom_;
			es_const_ = es_bc_ * es_d_off_ -
					es_ac_ / es_d_off_ +
					es_cover3_ * pow(es_d_off_, 3.) +
					es_dover5_ * pow(es_d_off_, 5.);
			// CONSTR = TWOB*LOG(CTOFM)-ACOEF/C2OFM+CCOEF*C2OFM+OFF4M*DENOM
			es_constr_ = 2. * es_bc_ * log(es_d_off_) - es_ac_ / es_d_off2_ +
					es_cc_ * es_d_off2_ + pow(es_d_off2_, 2.) / es_denom_;
			// EADDR  = (TWELVE*ONOFF2M*LOG(CTOFM/CTONM) - THREE*(OFF4M-C2ONM*C2ONM))*DENOM
			es_eaddr_ = (12. * es_d_on2_ * es_d_off2_ * log(es_d_off_ / es_d_on_) -
					3. * (pow(es_d_off2_, 2.) - pow(es_d_on2_, 2.))) / es_denom_;
		}

		enable_es_switch_ = ok;
	}
	else
	{
		enable_es_switch_ = false;
	}

	if (vdw_enabled_ && vdw_cut_off_ > 0)
	{
		bool ok = vdw_cut_off_ > vdw_cut_on_ && cut_off_ >= vdw_cut_off_;
		if (!ok )
		{
			Log.error() << "Invalid vdw cuton/cutoff values!" << std::endl;
		}
		enable_vdw_switch_ = ok;
	}
	else
	{
		enable_vdw_switch_ = false;
	}

	return true;
}


void PLP::e1(double& d, double& e, double& delta)
{
	e = 0.;

	delta = 0.;

	double A = 3.4;
	double B = 3.6;
	double C = 4.5;
	double D = 5.5;
	double F = 20.0;
	double E = -0.4;

	if (d < A)
	{
		e =  F*(A - d)/A;
		//delta =  -F/A;
	}
	else if (d < B)
	{
		e = E - E * (B - d)/(B - A);
		//delta = E/(B-A);
	}
	else if (d < C)
	{
		e = E;
		//delta = 0.;
	}
	else if (d < D)
	{
		e = E * (D - d)/(D - C);
		//delta = -E/(D-C);
	}

	if (d >= 3.39 && d <= 3.41)
	{
		e = (1650./17. * d - 11287./17.)*d + 3860393./3400.;
		//delta = 3300./17.* d - 11287./17.;
	}
	else if ( d >= 3.59 && d <= 3.61)
	{
		e =  (50.*d - 361.) * d + 130241./200.;
		//delta = 100.*d - 361.;
	}
	else if ( d >= 4.49 && d <= 4.51)
	{
		e =  (10.*d-449./5.) * d + 201201./1000.;
		//delta = 20.*d - 449./5.;
	}
	else if ( d >= 5.49 && d <= 5.51)
	{
		e =  (-10.*d + 551./5.) * d - 303601./1000.;
		//delta = -20.*d + 551./5.;
	}
}


void PLP::e2(double& d, double& e, double& delta)
{
	e = 0.;

	delta = 0.;

	double A = 2.3;
	double B = 2.6;
	double C = 3.1;
	double D = 3.4;
	double F = 20.0;
	double E = -2.0;

	if (d < A)
	{
		e =  F*(A - d)/A;
		//delta =  -F/A;
	}
	else if (d < B)
	{
		e = E - E * (B - d)/(B - A);
		//delta = E/(B-A);
	}
	else if (d < C)
	{
		e = E;
		//delta = 0.;
	}
	else if (d < D)
	{
		e = E * (D - d)/(D - C);
		//delta = -E/(D-C);
	}

	if (d >= 2.29 && d <= 2.31)
	{
		e = (50.724 * d - 241.014)*d + 286.005;
		//delta = 101.448 * d - 241.014;
	}
	else if ( d >= 2.59 && d <= 2.61)
	{
		e =  (166.6666*d - 870.) * d + 1133.35;
		//delta = 333.3333*d - 870.;
	}
	else if ( d >= 3.09 && d <= 3.11)
	{
		e =  (166.66666*d-1029.999) * d + 1589.3499;
		//delta = 333.3333*d - 1029.999;
	}
	else if ( d >= 3.39 && d <= 3.41)
	{
		e =  (-166.66666*d + 1136.666) * d - 1938.0166;
		//delta = -333.3333*d + 1136;
	}
}



double PLP::updateEnergy()
	throw()
{
	double ght_energy = 0.;

	//rotgrad_.resize(ghtorsions_.size());

	/// Calculation of ligand torsion score.
	/// ghtorsions_ need to have been set by setRotatableBonds() for each new ligand!
	for (Size x = 0; x < ghtorsions_.size(); x++)
	{
		Atom& at1 = *ghtorsions_[x].at1;
		Atom& at2 = *ghtorsions_[x].at2;
		Atom& at3 = *ghtorsions_[x].at3;
		Atom& at4 = *ghtorsions_[x].at4;

		Vector3	a13 = at3.getPosition() - at1.getPosition();
		Vector3 a12 = at2.getPosition() - at1.getPosition();
		Vector3 a42 = at2.getPosition() - at4.getPosition();
		Vector3 cross1312;
		Vector3 cross1242;

		cross1312 = a13 % a12;
		cross1242 = a12 % a42;

		double length_cross1312 = cross1312.getLength();
		double length_cross1242 = cross1242.getLength();

		if (length_cross1312 != 0 && length_cross1242 != 0)
		{
			if (ghtorsions_[x].type == 2)
			{
			//	rotgrad_[x] = 0.;
				continue;
			}

			cross1312 /= length_cross1312;
			cross1242 /= length_cross1242;

			double cosphi = cross1312 * cross1242;

			if (cosphi > 1.0)
			{
				cosphi = 1.0;
			}
			if (cosphi < -1.0)
			{
				cosphi = -1.0;
			}

			double phi = acos(cosphi);

			// s definiert + oder - 180 Grad
//			double s = a12.x * (cross1312.z * cross1242.y - cross1312.y * cross1242.z)
//				+ a12.y * (cross1312.x * cross1242.z - cross1312.z * cross1242.x)
//				+ a12.z * (cross1312.y * cross1242.x - cross1312.x * cross1242.y);

			if (ghtorsions_[x].type == 0)
			{

				ght_energy += 1.0 * (1. + cos(3. * phi));

// 				if (s < 0)
// 					rotgrad_[x] = -3.*sin(3. * phi) * 6.283;
// 				else
// 					rotgrad_[x] = 3.*sin(3. * phi) * 6.283;
			}
			else
			{
				ght_energy += 0.5 * (1. + cos(6. * phi));

// 				if (s < 0)
// 					rotgrad_[x] = -3.*sin(6. * phi) * 6.283;
// 				else
// 					rotgrad_[x] = 3.*sin(6. * phi) * 6.283;
			}

		}
		else
		{
			ght_energy += 100.;
		}
	}

	energy_ = 0;
	vdw_energy_ = 0;
	es_energy_  = 0;

	/// Calculation of nonbonded score (receptor-ligand and ligand-ligand atoms).
	/// atom_pair_vector_ needs to have been set by update()
	for (Position p = 0; p < atom_pair_vector_.size(); p++)
	{

		Vector3 p1 = atom_pair_vector_[p].first->getPosition();
		Vector3 p2 = atom_pair_vector_[p].second->getPosition();

		double d = (p1 - p2).getLength();

		NonBondedPairData& data = non_bonded_data_[p];

		double e = 0.;
		//double delta = 0.;

		if (data.type == NonBondedPairData::steric)
		{
			double A = 3.4;
			double B = 3.6;
			double C = 4.5;
			double D = 5.5;
			double F = 20.0;
			double E = -0.4;

			if (d < A)
			{
				e =  F*(A - d)/A;
				//delta =  -F/A;
			}
			else if (d < B)
			{
				e = E - E * (B - d)/(B - A);
				//delta = E/(B-A);
			}
			else if (d < C)
			{
				e = E;
				//delta = 0.;
			}
			else if (d < D)
			{
				e = E * (D - d)/(D - C);
				//delta = -E/(D-C);
			}

			if (d >= 3.39 && d <= 3.41)
			{
				e = (1650./17. * d - 11287./17.)*d + 3860393./3400.;
				//delta = 3300./17.* d - 11287./17.;
			}
			else if ( d >= 3.59 && d <= 3.61)
			{
				e =  (50.*d - 361.) * d + 130241./200.;
				//delta = 100.*d - 361.;
			}
			else if ( d >= 4.49 && d <= 4.51)
			{
				e =  (10.*d-449./5.) * d + 201201./1000.;
				//delta = 20.*d - 449./5.;
			}
			else if ( d >= 5.49 && d <= 5.51)
			{
				e =  (-10.*d + 551./5.) * d - 303601./1000.;
				//delta = -20.*d + 551./5.;
			}
		}
		else if (data.type == NonBondedPairData::hydrogen_bond)
		{
			double A = 2.3;
			double B = 2.6;
			double C = 3.1;
			double D = 3.4;
			double F = 20.0;
			double E = -2.0;

			if (d < A)
			{
				e =  F*(A - d)/A;
				//delta =  -F/A;
			}
			else if (d < B)
			{
				e = E - E * (B - d)/(B - A);
				//delta = E/(B-A);
			}
			else if (d < C)
			{
				e = E;
				//delta = 0.;
			}
			else if (d < D)
			{
				e = E * (D - d)/(D - C);
				//delta = -E/(D-C);
			}

			if (d >= 2.29 && d <= 2.31)
			{
				e = (50.724 * d - 241.014)*d + 286.005;
				//delta = 101.448 * d - 241.014;
			}
			else if ( d >= 2.59 && d <= 2.61)
			{
				e =  (166.6666*d - 870.) * d + 1133.35;
				//delta = 333.3333*d - 870.;
			}
			else if ( d >= 3.09 && d <= 3.11)
			{
				e =  (166.66666*d-1029.999) * d + 1589.3499;
				//delta = 333.3333*d - 1029.999;
			}
			else if ( d >= 3.39 && d <= 3.41)
			{
				e =  (-166.66666*d + 1136.666) * d - 1938.0166;
				//delta = -333.3333*d + 1136;
			}
		}
		else if (data.type == NonBondedPairData::conformation)
		{
			if (d < 2.35 && atom_pair_vector_[p].first->getElement().getSymbol() != "H" && atom_pair_vector_[p].second->getElement().getSymbol() != "H")
			{
				e = 10000;
				//cout<<atom_pair_vector_[p].first->getFullName()<<" "<<atom_pair_vector_[p].second->getFullName()<<endl;
				//cout<<non_bonded_data_[p].type<<"  "<<d<<"  : "<<e<<endl;
			}
		}
		else
		{
			cerr << "PLP error" << endl;
			exit(0);
		}

//		double gg = 0.;
//
// 		if (data.type == 0)
// 		{
// 			double d1,d2;
//
// 			double d3 = d + 0.001;
//
// 			e1(d3,d1,d2);
//
// 			gg = (e - d1)/0.001;
//
// 			d2 = -d2;
//
// 			if (fabs(gg-d2) > 0.01)
// 				counter++;
// 		}
// 		else if (data.type == 1)
// 		{
// 			double d1,d2;
//
// 			double d3 = d + 0.001;
//
// 			e2(d3,d1,d2);
//
// 			gg = (e - d1)/0.001;
//
// 			d2 = -d2;
//
// 			if (fabs(gg-d2) > 0.01)
// 				counter++;
// 		}

		vdw_energy_ += e;

		//Vector3 g = (p1-p2)*(delta/d);

		//atom_pair_vector_[p].first->setForce(atom_pair_vector_[p].first->getForce()+ g);
		//atom_pair_vector_[p].second->setForce(atom_pair_vector_[p].second->getForce() - g);
	}

	energy_ = vdw_energy_ + ght_energy;

	//cout<<"PLP vdw_energy_ = "<<vdw_energy_<<"  ght_energy = "<<ght_energy<<endl<<flush;

	return energy_;
}


void PLP::updateForces()throw()
{

}

double PLP::getVDWEnergy() const
{
	if (!vdw_enabled_) return 0;
	return vdw_energy_;
}

double PLP::getESEnergy() const
{
	if (!es_enabled_) return 0;
	return es_energy_;
}
