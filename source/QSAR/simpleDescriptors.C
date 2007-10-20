// -*- Mode: C++; tab-width: 2; -*-
// vi: set ts=2:
//
// $Id: simpleDescriptors.C,v 1.9.20.1 2007/03/16 00:06:49 bertsch Exp $
//

#include <BALL/QSAR/simpleDescriptors.h>
#include <BALL/KERNEL/expression.h>
#include <BALL/KERNEL/forEach.h>

using namespace std;

namespace BALL
{
	/////////////////////
	// molecular weight
	/////////////////////
	MolecularWeight::MolecularWeight()
		:	SimpleBase("MolecularWeight", "g/mol")
	{
	}

	MolecularWeight::MolecularWeight(const MolecularWeight& mw)
		:	SimpleBase(mw)
	{
	}

	MolecularWeight::~MolecularWeight()
	{
	}

	MolecularWeight& MolecularWeight::operator = (const MolecularWeight& mw)
	{
		this->setName(mw.getName());
		this->setUnit(mw.getUnit());
		return *this;
	}

	
	/////////////////////
	// number of bonds
	/////////////////////
	NumberOfBonds::NumberOfBonds()
		:	SimpleBase("NumberOfBonds", "")
	{
	}

	NumberOfBonds::NumberOfBonds(const NumberOfBonds& nb)
		:	SimpleBase(nb)
	{
	}

	NumberOfBonds::~NumberOfBonds()
	{
	}

	NumberOfBonds& NumberOfBonds::operator = (const NumberOfBonds& nb)
	{
		this->setName(nb.getName());
		this->setUnit(nb.getUnit());
		return *this;
	}

	
	///////////////////////////
	// number of single bonds
	//////////////////////////
	NumberOfSingleBonds::NumberOfSingleBonds()
		:	SimpleBase("NumberOfSingleBonds", "")
	{
	}

	NumberOfSingleBonds::NumberOfSingleBonds(const NumberOfSingleBonds& nsb)
		:	SimpleBase(nsb)
	{
	}

	NumberOfSingleBonds::~NumberOfSingleBonds()
	{
	}

	NumberOfSingleBonds& NumberOfSingleBonds::operator = (const NumberOfSingleBonds& nsb)
	{
		this->setName(nsb.getName());
		this->setUnit(nsb.getUnit());
		return *this;
	}

	///////////////////////////
	// number of double bonds
	///////////////////////////
	NumberOfDoubleBonds::NumberOfDoubleBonds()
		:	SimpleBase("NumberOfDoubleBonds", "")
	{
	}

	NumberOfDoubleBonds::NumberOfDoubleBonds(const NumberOfDoubleBonds& ndb)
		:	SimpleBase(ndb)
	{
	}

	NumberOfDoubleBonds::~NumberOfDoubleBonds()
	{
	}

	NumberOfDoubleBonds& NumberOfDoubleBonds::operator = (const NumberOfDoubleBonds& ndb)
	{
		this->setName(ndb.getName());
		this->setUnit(ndb.getUnit());
		return *this;
	}

	///////////////////////////
	// number of triple bonds
	///////////////////////////
	NumberOfTripleBonds::NumberOfTripleBonds()
		:	SimpleBase("NumberOfTripleBonds", "")
	{
	}

	NumberOfTripleBonds::NumberOfTripleBonds(const NumberOfTripleBonds& ntb)
		:	SimpleBase(ntb)
	{
	}

	NumberOfTripleBonds::~NumberOfTripleBonds()
	{
	}

	NumberOfTripleBonds& NumberOfTripleBonds::operator = (const NumberOfTripleBonds& ntb)
	{
		this->setName(ntb.getName());
		this->setUnit(ntb.getUnit());
		return *this;
	}

	//////////////////////////////
	// number of aromatic bonds
	//////////////////////////////
	NumberOfAromaticBonds::NumberOfAromaticBonds()
		:	SimpleBase("NumberOfAromaticBonds", "")
	{
	}

	NumberOfAromaticBonds::NumberOfAromaticBonds(const NumberOfAromaticBonds& nab)
		:	SimpleBase(nab)
	{
	}

	NumberOfAromaticBonds::~NumberOfAromaticBonds()
	{
	}

	NumberOfAromaticBonds& NumberOfAromaticBonds::operator = (const NumberOfAromaticBonds& nab)
	{
		this->setName(nab.getName());
		this->setUnit(nab.getUnit());
		return *this;
	}
	
	/////////////////////
	// number of atoms
	/////////////////////
	NumberOfAtoms::NumberOfAtoms()
		:	SimpleBase("NumberOfAtoms", "")
	{
	}

	NumberOfAtoms::NumberOfAtoms(const NumberOfAtoms& na)
		:	SimpleBase(na)
	{
	}

	NumberOfAtoms::~NumberOfAtoms()
	{
	}

	NumberOfAtoms& NumberOfAtoms::operator = (const NumberOfAtoms& na)
	{
		this->setName(na.getName());
		this->setUnit(na.getUnit());
		return *this;
	}

	////////////////////
	// number of boron
	////////////////////
	NumberOfBoron::NumberOfBoron()
		: SimpleBase("NumberOfBoron", "")
	{
	}
	
	NumberOfBoron::NumberOfBoron(const NumberOfBoron& nb)
		:	SimpleBase(nb)
	{
	}

	NumberOfBoron::~NumberOfBoron()
	{
	}

	NumberOfBoron& NumberOfBoron::operator = (const NumberOfBoron& nb)
	{
		this->setName(nb.getName());
		this->setUnit(nb.getUnit());
		return *this;
	}
	
	/////////////////////
	// number of carbon
	/////////////////////
	NumberOfCarbon::NumberOfCarbon()
		:	SimpleBase("NumberOfCarbon", "")
	{
	}

	NumberOfCarbon::NumberOfCarbon(const NumberOfCarbon& nc)
		:	SimpleBase(nc)
	{
	}

	NumberOfCarbon::~NumberOfCarbon()
	{
	}

	NumberOfCarbon& NumberOfCarbon::operator = (const NumberOfCarbon& nc)
	{
		this->setName(nc.getName());
		this->setUnit(nc.getUnit());
		return *this;
	}
	
	////////////////////////
	// number of nitrogen
	////////////////////////
	NumberOfNitrogen::NumberOfNitrogen()
		:	SimpleBase("NumberOfNitrogen", "")
	{
	}

	NumberOfNitrogen::NumberOfNitrogen(const NumberOfNitrogen& nn)
		:	SimpleBase(nn)
	{
	}

	NumberOfNitrogen::~NumberOfNitrogen()
	{
	}

	NumberOfNitrogen& NumberOfNitrogen::operator = (const NumberOfNitrogen& nn)
	{
		this->setName(nn.getName());
		this->setUnit(nn.getUnit());
		return *this;
	}

	//////////////////////
	// number of oxygen
	//////////////////////
	NumberOfOxygen::NumberOfOxygen()
		:	SimpleBase("NumberOfOxygen", "")
	{
	}
	
	NumberOfOxygen::NumberOfOxygen(const NumberOfOxygen& no)
		:	SimpleBase(no)
	{
	}

	NumberOfOxygen::~NumberOfOxygen()
	{
	}

	NumberOfOxygen& NumberOfOxygen::operator = (const NumberOfOxygen& no)
	{
		this->setName(no.getName());
		this->setUnit(no.getUnit());
		return *this;
	}
	
	////////////////////////
	// number of flourine
	////////////////////////
	NumberOfFlourine::NumberOfFlourine()
		:	SimpleBase("NumberOfFlourine", "")
	{
	}

	NumberOfFlourine::NumberOfFlourine(const NumberOfFlourine& nf)
		:	SimpleBase(nf)
	{
	}

	NumberOfFlourine::~NumberOfFlourine()
	{
	}

	NumberOfFlourine& NumberOfFlourine::operator = (const NumberOfFlourine& nf)
	{
		this->setName(nf.getName());
		this->setUnit(nf.getUnit());
		return *this;
	}

	/////////////////////////
	// number of phophorus
	/////////////////////////
	NumberOfPhosphorus::NumberOfPhosphorus()
		:	SimpleBase("NumberOfPhosphorus", "")
	{
	}

	NumberOfPhosphorus::NumberOfPhosphorus(const NumberOfPhosphorus& np)
		:	SimpleBase(np)
	{
	}

	NumberOfPhosphorus::~NumberOfPhosphorus()
	{
	}

	NumberOfPhosphorus& NumberOfPhosphorus::operator = (const NumberOfPhosphorus& np)
	{
		this->setName(np.getName());
		this->setUnit(np.getUnit());
		return *this;
	}

	//////////////////////
	// number of sulfur
	//////////////////////
	NumberOfSulfur::NumberOfSulfur()
		:	SimpleBase("NumberOfSulfur", "")
	{
	}

	NumberOfSulfur::NumberOfSulfur(const NumberOfSulfur& ns)
		:	SimpleBase(ns)
	{
	}

	NumberOfSulfur::~NumberOfSulfur()
	{
	}

	NumberOfSulfur& NumberOfSulfur::operator = (const NumberOfSulfur& ns)
	{
		this->setName(ns.getName());
		this->setUnit(ns.getUnit());
		return *this;
	}

	/////////////////////////
	// number of chlorine
	/////////////////////////
	NumberOfChlorine::NumberOfChlorine()
		:	SimpleBase("NumberOfChlorine", "")
	{
	}

	NumberOfChlorine::NumberOfChlorine(const NumberOfChlorine& nc)
		:	SimpleBase(nc)
	{
	}

	NumberOfChlorine::~NumberOfChlorine()
	{
	}

	NumberOfChlorine& NumberOfChlorine::operator = (const NumberOfChlorine& nc)
	{
		this->setName(nc.getName());
		this->setUnit(nc.getUnit());
		return *this;
	}

	///////////////////////
	// number of bromine
	///////////////////////
	NumberOfBromine::NumberOfBromine()
		:	SimpleBase("NumberOfBromine", "")
	{
	}

	NumberOfBromine::NumberOfBromine(const NumberOfBromine& nb)
		:	SimpleBase(nb)
	{
	}

	NumberOfBromine::~NumberOfBromine()
	{
	}

	NumberOfBromine& NumberOfBromine::operator = (const NumberOfBromine& nb)
	{
		this->setName(nb.getName());
		this->setUnit(nb.getUnit());
		return *this;
	}

	//////////////////////
	// number of iodine
	//////////////////////
	NumberOfIodine::NumberOfIodine()
		:	SimpleBase("NumberOfIodine", "")
	{
	}

	NumberOfIodine::NumberOfIodine(const NumberOfIodine& ni)
		:	SimpleBase(ni)
	{
	}

	NumberOfIodine::~NumberOfIodine()
	{
	}

	NumberOfIodine& NumberOfIodine::operator = (const NumberOfIodine& ni)
	{
		this->setName(ni.getName());
		this->setUnit(ni.getUnit());
		return *this;
	}

	////////////////////////
	// number of hydrogen
	////////////////////////
	NumberOfHydrogen::NumberOfHydrogen()
		:	SimpleBase("NumberOfHydrogen", "")
	{
	}

	NumberOfHydrogen::NumberOfHydrogen(const NumberOfHydrogen& nh)
		:	SimpleBase(nh)
	{
	}

	NumberOfHydrogen::~NumberOfHydrogen()
	{
	}

	NumberOfHydrogen& NumberOfHydrogen::operator = (const NumberOfHydrogen& nh)
	{
		this->setName(nh.getName());
		this->setUnit(nh.getUnit());
		return *this;
	}

	///////////////////////
	// formal charge
	///////////////////////
	FormalCharge::FormalCharge()
		:	SimpleBase("FormalCharge", "eV")
	{
	}

	FormalCharge::FormalCharge(const FormalCharge& fc)
		:	SimpleBase(fc)
	{
	}

	FormalCharge::~FormalCharge()
	{
	}

	FormalCharge& FormalCharge::operator = (const FormalCharge& fc)
	{
		this->setName(fc.getName());
		this->setUnit(fc.getUnit());
		return *this;
	}

	/////////////////////////////
	// number of aromatic atoms
	/////////////////////////////
	NumberOfAromaticAtoms::NumberOfAromaticAtoms()
		:	SimpleBase("NumberOfAromaticAtoms", "")
	{
	}

	NumberOfAromaticAtoms::NumberOfAromaticAtoms(const NumberOfAromaticAtoms& naa)
		:	SimpleBase(naa)
	{
	}

	NumberOfAromaticAtoms::~NumberOfAromaticAtoms()
	{
	}

	NumberOfAromaticAtoms& NumberOfAromaticAtoms::operator = (const NumberOfAromaticAtoms& naa)
	{
		this->setName(naa.getName());
		this->setUnit(naa.getUnit());
		return *this;
	}

	///////////////////////////
	// number of heavy bonds
	///////////////////////////
	NumberOfHeavyBonds::NumberOfHeavyBonds()
		:	SimpleBase("NumberOfHeavyBonds", "")
	{
	}

	NumberOfHeavyBonds::NumberOfHeavyBonds(const NumberOfHeavyBonds& nhb)
		:	SimpleBase(nhb)
	{
	}

	NumberOfHeavyBonds::~NumberOfHeavyBonds()
	{
	}

	NumberOfHeavyBonds& NumberOfHeavyBonds::operator = (const NumberOfHeavyBonds& nhb)
	{
		this->setName(nhb.getName());
		this->setUnit(nhb.getUnit());
		return *this;
	}

	///////////////////////////
	// number of heavy atoms
	///////////////////////////
	NumberOfHeavyAtoms::NumberOfHeavyAtoms()
		:	SimpleBase("NumberOfHeavyAtoms", "")
	{
	}

	NumberOfHeavyAtoms::NumberOfHeavyAtoms(const NumberOfHeavyAtoms& nha)
		:	SimpleBase(nha)
	{
	}

	NumberOfHeavyAtoms::~NumberOfHeavyAtoms()
	{
	}

	NumberOfHeavyAtoms& NumberOfHeavyAtoms::operator = (const NumberOfHeavyAtoms& nha)
	{
		this->setName(nha.getName());
		this->setUnit(nha.getUnit());
		return *this;
	}

	//////////////////////
	// vertex adjacency
	//////////////////////
	VertexAdjacency::VertexAdjacency()
		:	SimpleBase("VertexAdjacency", "")
	{
	}

	VertexAdjacency::VertexAdjacency(const VertexAdjacency& va)
		:	SimpleBase(va)
	{
	}

	VertexAdjacency::~VertexAdjacency()
	{
	}

	VertexAdjacency& VertexAdjacency::operator = (const VertexAdjacency& va)
	{
		this->setName(va.getName());
		this->setUnit(va.getUnit());
		return *this;
	}

	double VertexAdjacency::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		double m = ac.getProperty("NumberOfHeavyBonds").getDouble();
		return m!=0 ? 1 + 3.3219280948873626 * log10(m) : 0.0;
	}

	///////////////////////////////
	// vertex adjacency equaility 
	//////////////////////////////
	VertexAdjacencyEquality::VertexAdjacencyEquality()
		:	SimpleBase("VertexAdjacencyEquality", "")
	{
	}

	VertexAdjacencyEquality::VertexAdjacencyEquality(const VertexAdjacencyEquality& vae)
		:	SimpleBase(vae)
	{
	}

	VertexAdjacencyEquality::~VertexAdjacencyEquality()
	{
	}

	VertexAdjacencyEquality& VertexAdjacencyEquality::operator = ( const VertexAdjacencyEquality& vae)
	{
		this->setName(vae.getName());
		this->setUnit(vae.getUnit());
		return *this;
	}

	double VertexAdjacencyEquality::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}	
		double n = ac.getProperty("NumberOfHeavyAtoms").getDouble();
		double m = ac.getProperty("NumberOfHeavyBonds").getDouble();

		if (n!=0.0 && m!=0) 
		{
			double f = 1.0 - 2*m/n/n;
			if (f > 0.0 && f < 1.0) 
			{
				return -(1.0-f) * 3.3219280948873626 * log10(1.0-f) - f * 3.3219280948873626 * log10(f);
			}
			else 
			{
				return 0.0;
			}
		}
		return 0.0;
	}
	
	/////////////////////////////////
	// number of hydrophobic atoms
	/////////////////////////////////
	NumberOfHydrophobicAtoms::NumberOfHydrophobicAtoms()
		:	SimpleBase("NumberOfHydrophobicAtoms", ""),
		expression_("")
	{
	}

	NumberOfHydrophobicAtoms::NumberOfHydrophobicAtoms(const NumberOfHydrophobicAtoms& nha)
		:	SimpleBase(nha),
		expression_(nha.getExpression())
	{
	}

	NumberOfHydrophobicAtoms::NumberOfHydrophobicAtoms(const String& expression)
		:	SimpleBase("NumberOfHydrophobicAtoms", ""),
		expression_(expression)
	{
	}

	NumberOfHydrophobicAtoms::~NumberOfHydrophobicAtoms()
	{
	}

	NumberOfHydrophobicAtoms& NumberOfHydrophobicAtoms::operator = (const NumberOfHydrophobicAtoms& nha)
	{
		this->setName(nha.getName());
		this->setUnit(nha.getUnit());
		this->setExpression(nha.getExpression());
		return *this;
	}

	void NumberOfHydrophobicAtoms::setExpression(const String& expression)
	{
		this->expression_ = expression;
	}

	const String& NumberOfHydrophobicAtoms::getExpression() const
	{
		return this->expression_;
	}

	double NumberOfHydrophobicAtoms::compute(AtomContainer& ac)
	{
		if (expression_ != "")
		{
			Size num_hyd = 0;
			Expression ex_hyd(expression_);
			AtomIterator a_it = ac.beginAtom();
			BALL_FOREACH_ATOM (ac, a_it)
			{
				if (ex_hyd(*a_it))
				{
					++num_hyd;
				}
			}
			ac.setProperty(this->getName(), (double)num_hyd);
		}
		else
		{
			if (!isValid_(ac))
			{
				calculate_(ac);
			}
		}
		return ac.getProperty(this->getName()).getDouble();
	}

	/////////////////////////////////////
	// number of hydrogen bond donors
	/////////////////////////////////////
	NumberOfHydrogenBondDonors::NumberOfHydrogenBondDonors()
		:	SimpleBase("NumberOfHydrogenBondDonors", ""),
		expression_("")
	{
	}

	NumberOfHydrogenBondDonors::NumberOfHydrogenBondDonors(const NumberOfHydrogenBondDonors& nhbd)
		:	SimpleBase(nhbd),
			expression_(nhbd.getExpression())
	{
	}

	NumberOfHydrogenBondDonors::NumberOfHydrogenBondDonors(const String& expression)
		:	SimpleBase("NumberOfHydrogenBondDonors", ""),
		expression_(expression)
	{
	}

	NumberOfHydrogenBondDonors::~NumberOfHydrogenBondDonors()
	{
	}

	NumberOfHydrogenBondDonors& NumberOfHydrogenBondDonors::operator = (const NumberOfHydrogenBondDonors& nhbd)
	{
		this->setName(nhbd.getName());
		this->setUnit(nhbd.getUnit());
		this->setExpression(nhbd.getExpression());
		return *this;
	}
	
	void NumberOfHydrogenBondDonors::setExpression(const String& expression)
	{
		this->expression_ = expression;
	}

	const String& NumberOfHydrogenBondDonors::getExpression() const
	{
		return this->expression_;
	}

	double NumberOfHydrogenBondDonors::compute(AtomContainer& ac)
	{
		if (expression_ != "")
		{
			Expression ex_don(expression_);
			Size num_don = 0;
			AtomIterator a_it = ac.beginAtom();
			BALL_FOREACH_ATOM (ac, a_it)
			{
				if (ex_don(*a_it))
				{
					++num_don;
				}
			}
			ac.setProperty(this->getName(), (double)num_don);
		}
		else
		{
			if (!isValid_(ac))
			{
				calculate_(ac);
			}
		}
		return ac.getProperty(this->getName()).getDouble();
	}

	////////////////////////////////////////
	// number of hydrogen bond acceptors
	////////////////////////////////////////
	NumberOfHydrogenBondAcceptors::NumberOfHydrogenBondAcceptors()
		:	SimpleBase("NumberOfHydrogenBondAcceptors", ""),
		expression_("")
	{
	}

	NumberOfHydrogenBondAcceptors::NumberOfHydrogenBondAcceptors(const NumberOfHydrogenBondAcceptors& nhba)
		:	SimpleBase(nhba),
		expression_(nhba.getExpression())
	{
	}

	NumberOfHydrogenBondAcceptors::NumberOfHydrogenBondAcceptors(const String& expression)
		:	SimpleBase("NumberOfHydrogenBondAcceptors", ""),
		expression_(expression)
	{
	}

	NumberOfHydrogenBondAcceptors::~NumberOfHydrogenBondAcceptors()
	{
	}

	NumberOfHydrogenBondAcceptors& NumberOfHydrogenBondAcceptors::operator = (const NumberOfHydrogenBondAcceptors& nhba)
	{
		this->setName(nhba.getName());
		this->setUnit(nhba.getUnit());
		this->setExpression(nhba.getExpression());
		return *this;
	}

	void NumberOfHydrogenBondAcceptors::setExpression(const String& expression)
	{
		this->expression_ = expression;
	}

	const String& NumberOfHydrogenBondAcceptors::getExpression() const
	{
		return this->expression_;
	}

	double NumberOfHydrogenBondAcceptors::compute(AtomContainer& ac)
	{
		if (expression_ != "")
		{
			Expression ex_acc(expression_);
			Size num_acc = 0;
			AtomIterator a_it = ac.beginAtom();
			BALL_FOREACH_ATOM (ac, a_it)
			{
				if (ex_acc(*a_it))
				{
					++num_acc;
				}
			}
			ac.setProperty(this->getName(), (double)num_acc);
		}
		else
		{
			if (!isValid_(ac))
			{
				calculate_(ac);
			}
		}
		return ac.getProperty(this->getName()).getDouble();
	}

	///////////////////////////
	// bond polarizabilities
	///////////////////////////
	BondPolarizabilities::BondPolarizabilities()
		:	SimpleBase("BondPolarizabilities", "A^3")
	{
	}

	BondPolarizabilities::BondPolarizabilities(const BondPolarizabilities& bp)
		:	SimpleBase(bp)
	{
	}

	BondPolarizabilities::~BondPolarizabilities()
	{
	}

	BondPolarizabilities& BondPolarizabilities::operator = (const BondPolarizabilities& bp)
	{
		this->setName(bp.getName());
		this->setUnit(bp.getUnit());
		return *this;
	}

	///////////////////////////
	// atomic polarizabilities
	///////////////////////////
	AtomicPolarizabilities::AtomicPolarizabilities()
		:	SimpleBase("AtomicPolarizabilities", "A^3")
	{
	}

	AtomicPolarizabilities::AtomicPolarizabilities(const AtomicPolarizabilities& ap)
		:	SimpleBase(ap)
	{
	}

	AtomicPolarizabilities::~AtomicPolarizabilities()
	{
	}

	AtomicPolarizabilities& AtomicPolarizabilities::operator = (const AtomicPolarizabilities& ap)
	{
		this->setName(ap.getName());
		this->setUnit(ap.getUnit());
		return *this;
	}

	////////////////////////////////
	// number of rotatable bonds
	///////////////////////////////
	NumberOfRotatableBonds::NumberOfRotatableBonds()
		:	SimpleBase("NumberOfRotatableBonds", "")
	{
	}
	
	NumberOfRotatableBonds::NumberOfRotatableBonds(const NumberOfRotatableBonds& nrb)
		:	SimpleBase(nrb)
	{
	}
	
	NumberOfRotatableBonds::~NumberOfRotatableBonds()
	{
	}

	NumberOfRotatableBonds& NumberOfRotatableBonds::operator = (const NumberOfRotatableBonds& nrb)
	{
		this->setName(nrb.getName());
		this->setUnit(nrb.getUnit());
		return *this;
	}
	
	//////////////////////////////////////
	// number of rotatable single bonds
	//////////////////////////////////////
	NumberOfRotatableSingleBonds::NumberOfRotatableSingleBonds()
		:	SimpleBase("NumberOfRotatableSingleBonds", "")
	{
	}

	NumberOfRotatableSingleBonds::NumberOfRotatableSingleBonds(const NumberOfRotatableSingleBonds& nrsb)
		:	SimpleBase(nrsb)
	{
	}
	
	NumberOfRotatableSingleBonds::~NumberOfRotatableSingleBonds()
	{
	}

	NumberOfRotatableSingleBonds& NumberOfRotatableSingleBonds::operator = (const NumberOfRotatableSingleBonds& nrsb)
	{
		this->setName(nrsb.getName());
		this->setUnit(nrsb.getUnit());
		return *this;
	}

	///////////////////////////////////
	// relative number of rotatable bonds
	///////////////////////////////////
	RelNumberOfRotatableBonds::RelNumberOfRotatableBonds()
		:	SimpleBase("RelNumberOfRotatableBonds", "")
	{
	}

	RelNumberOfRotatableBonds::RelNumberOfRotatableBonds(const RelNumberOfRotatableBonds& rnrb)
		:	SimpleBase(rnrb)
	{
	}

	RelNumberOfRotatableBonds::~RelNumberOfRotatableBonds()
	{
	}

	RelNumberOfRotatableBonds& RelNumberOfRotatableBonds::operator = (const RelNumberOfRotatableBonds& rnrb)
	{
		this->setName(rnrb.getName());
		this->setUnit(rnrb.getUnit());
		return *this;
	}

	double RelNumberOfRotatableBonds::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{	
			calculate_(ac);
		}
		double num_bonds =  ac.getProperty("NumberOfBonds").getDouble();
		double num_rot = ac.getProperty("NumberOfRotatableBonds").getDouble();
		return num_bonds != 0 ? num_rot/num_bonds : -1.0;
	}

	//////////////////////////////////////////////////////
	// relative number number of rotatable single bonds
	//////////////////////////////////////////////////////
	RelNumberOfRotatableSingleBonds::RelNumberOfRotatableSingleBonds()
		:	SimpleBase("RelNumberOfRotatableSingleBonds", "")
	{
	}

	RelNumberOfRotatableSingleBonds::RelNumberOfRotatableSingleBonds(const RelNumberOfRotatableSingleBonds& rnrs)
		:	SimpleBase(rnrs)
	{
	}

	RelNumberOfRotatableSingleBonds::~RelNumberOfRotatableSingleBonds()
	{
	}

	RelNumberOfRotatableSingleBonds& RelNumberOfRotatableSingleBonds::operator = (const RelNumberOfRotatableSingleBonds& rnrs)
	{
		this->setName(rnrs.getName());
		this->setUnit(rnrs.getUnit());
		return *this;
	}

	double RelNumberOfRotatableSingleBonds::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		double num_bonds = ac.getProperty("NumberOfBonds").getDouble();
		double num_rot = ac.getProperty("NumberOfRotatableSingleBonds").getDouble();
		return num_bonds != 0 ? num_rot/num_bonds : -1.0;
	}

	//////////////////////////////////////
	// mean atom information content
	//////////////////////////////////////
	MeanAtomInformationContent::MeanAtomInformationContent()
		:	SimpleBase("MeanAtomInformationContent", "")
	{
	}

	MeanAtomInformationContent::MeanAtomInformationContent(const MeanAtomInformationContent& maic)
		:	SimpleBase(maic)
	{
	}

	MeanAtomInformationContent::~MeanAtomInformationContent()
	{
	}

	MeanAtomInformationContent& MeanAtomInformationContent::operator = (const MeanAtomInformationContent& maic)
	{
		this->setName(maic.getName());
		this->setUnit(maic.getUnit());
		return *this;
	}

	//////////////////////////////////////
	// atom information content
	//////////////////////////////////////
	AtomInformationContent::AtomInformationContent()
		:	SimpleBase("AtomInformationContent", "")
	{
	}

	AtomInformationContent::AtomInformationContent(const AtomInformationContent& aic)
		:	SimpleBase(aic)
	{
	}

	AtomInformationContent::~AtomInformationContent()
	{
	}

	AtomInformationContent& AtomInformationContent::operator = (const AtomInformationContent& aic)
	{
		this->setName(aic.getName());
		this->setUnit(aic.getUnit());
		return *this;
	}

	double AtomInformationContent::compute(AtomContainer& ac)
	{
		if (!isValid_(ac))
		{
			calculate_(ac);
		}
		double num_atoms = ac.getProperty("NumberOfAtoms").getDouble();
		double info_mean = ac.getProperty("MeanAtomInformationContent").getDouble();
		return num_atoms * info_mean;
	}

	//////////////////
	// size of sssr
	//////////////////
	SizeOfSSSR::SizeOfSSSR()
		:	SimpleBase("SizeOfSSSR", "")
	{
	}

	SizeOfSSSR::SizeOfSSSR(const SizeOfSSSR& sssr)
		:	SimpleBase(sssr)
	{
	}

	SizeOfSSSR::~SizeOfSSSR()
	{
	}

	SizeOfSSSR& SizeOfSSSR::operator = (const SizeOfSSSR& sssr)
	{
		this->setName(sssr.getName());
		this->setUnit(sssr.getUnit());
		return *this;
	}

	/////////////////////////////////
	// principal moment of inertia
	/////////////////////////////////
	PrincipalMomentOfInertia::PrincipalMomentOfInertia()
		:	SimpleBase("PrincipalMomentOfInertia", "uŲ")
	{
	}

	PrincipalMomentOfInertia::PrincipalMomentOfInertia(const PrincipalMomentOfInertia& pmi)
		:	SimpleBase(pmi)
	{
	}

	PrincipalMomentOfInertia::~PrincipalMomentOfInertia()
	{
	}

	PrincipalMomentOfInertia& PrincipalMomentOfInertia::operator = (const PrincipalMomentOfInertia& pmi)
	{
		this->setName(pmi.getName());
		this->setUnit(pmi.getUnit());
		return *this;
	}

	///////////////////////////////////
	// principal moment of inertia x
	///////////////////////////////////
	PrincipalMomentOfInertiaX::PrincipalMomentOfInertiaX()
		:	SimpleBase("PrincipalMomentOfInertiaX", "uA^2")
	{
	}

	PrincipalMomentOfInertiaX::PrincipalMomentOfInertiaX(const PrincipalMomentOfInertiaX& pmix)
		:	SimpleBase(pmix)
	{
	}

	PrincipalMomentOfInertiaX::~PrincipalMomentOfInertiaX()
	{
	}

	PrincipalMomentOfInertiaX& PrincipalMomentOfInertiaX::operator = (const PrincipalMomentOfInertiaX& pmix)
	{
		this->setName(pmix.getName());
		this->setUnit(pmix.getUnit());
		return *this;
	}

	/////////////////////////////////////
	// principal moment of inertia y
	/////////////////////////////////////
	PrincipalMomentOfInertiaY::PrincipalMomentOfInertiaY()
		:	SimpleBase("PrincipalMomentOfInertiaY", "uA^2")
	{
	}

	PrincipalMomentOfInertiaY::PrincipalMomentOfInertiaY(const PrincipalMomentOfInertiaY& pmiy)
		:	SimpleBase(pmiy)
	{
	}

	PrincipalMomentOfInertiaY::~PrincipalMomentOfInertiaY()
	{
	}

	PrincipalMomentOfInertiaY& PrincipalMomentOfInertiaY::operator = (const PrincipalMomentOfInertiaY& pmiy)
	{
		this->setName(pmiy.getName());
		this->setUnit(pmiy.getUnit());
		return *this;
	}

	/////////////////////////////////////
	// principal moment of inertia
	/////////////////////////////////////
	PrincipalMomentOfInertiaZ::PrincipalMomentOfInertiaZ()
		:	SimpleBase("PrincipalMomentOfInertiaZ", "uA^2")
	{
	}

	PrincipalMomentOfInertiaZ::PrincipalMomentOfInertiaZ(const PrincipalMomentOfInertiaZ& pmiz)
		:	SimpleBase(pmiz)
	{
	}

	PrincipalMomentOfInertiaZ::~PrincipalMomentOfInertiaZ()
	{
	}

	PrincipalMomentOfInertiaZ& PrincipalMomentOfInertiaZ::operator = (const PrincipalMomentOfInertiaZ& pmiz)
	{
		this->setName(pmiz.getName());
		this->setUnit(pmiz.getUnit());
		return *this;
	}

} // namespace BALL



