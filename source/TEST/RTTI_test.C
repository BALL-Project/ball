// $Id: RTTI_test.C,v 1.5 2000/06/05 01:14:19 amoll Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////
#include <BALL/KERNEL/protein.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/nucleotide.h>
#include <BALL/KERNEL/nucleicAcid.h>
#include <BALL/KERNEL/bond.h>
#include <BALL/KERNEL/secondaryStructure.h>
///////////////////////////

START_TEST(RTTI, "$Id: RTTI_test.C,v 1.5 2000/06/05 01:14:19 amoll Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;
using namespace BALL::RTTI;
CHECK(RTTI::isKindOf<>())
	Protein p;
	Protein*			p_ptr(&p);
	Molecule*			m_ptr(&p);
	BaseFragment*	b_ptr(&p);
	TEST_EQUAL(isKindOf<Molecule>(*p_ptr), true)
	TEST_EQUAL(isKindOf<Molecule>(*m_ptr), true)
	TEST_EQUAL(isKindOf<Molecule>(*b_ptr), true)
	TEST_EQUAL(isKindOf<Protein>(*p_ptr), true)
	TEST_EQUAL(isKindOf<Protein>(*m_ptr), true)
	TEST_EQUAL(isKindOf<Protein>(*b_ptr), true)
	TEST_EQUAL(isKindOf<BaseFragment>(*p_ptr), true)
	TEST_EQUAL(isKindOf<BaseFragment>(*m_ptr), true)
	TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
	TEST_EQUAL(isKindOf<Residue>(*p_ptr), false)
	TEST_EQUAL(isKindOf<Residue>(*m_ptr), false)
	TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)
	Molecule m;
	m_ptr = &m;
	b_ptr = &m;
	TEST_EQUAL(isKindOf<Molecule>(*m_ptr), true)
	TEST_EQUAL(isKindOf<Molecule>(*b_ptr), true)
	TEST_EQUAL(isKindOf<Protein>(*m_ptr), false)
	TEST_EQUAL(isKindOf<Protein>(*b_ptr), false)
	TEST_EQUAL(isKindOf<BaseFragment>(*m_ptr), true)
	TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
	TEST_EQUAL(isKindOf<Residue>(*m_ptr), false)
	TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)
	BaseFragment b;
	b_ptr = &b;
	TEST_EQUAL(isKindOf<Molecule>(*b_ptr), false)
	TEST_EQUAL(isKindOf<Protein>(*b_ptr), false)
	TEST_EQUAL(isKindOf<BaseFragment>(*b_ptr), true)
	TEST_EQUAL(isKindOf<Residue>(*b_ptr), false)

	Atom*					a_ptr;
	System*				s_ptr = new System();
	PDBAtom*		 pa_ptr;
	Bond*				 bo_ptr;
	NucleicAcid* na_ptr;
	Nucleotide*  nu_ptr;
	Chain*				c_ptr;
	SecondaryStructure* ss_ptr;
	TEST_EQUAL(isKindOf<Atom>(*a_ptr), true)
	TEST_EQUAL(isKindOf<System>(*s_ptr), true)
	TEST_EQUAL(isKindOf<PDBAtom>(*pa_ptr), true)
	TEST_EQUAL(isKindOf<Bond>(*bo_ptr), true)
	TEST_EQUAL(isKindOf<NucleicAcid>(*na_ptr), true)
	TEST_EQUAL(isKindOf<Nucleotide>(*nu_ptr), true)
	TEST_EQUAL(isKindOf<Chain>(*c_ptr), true)
	TEST_EQUAL(isKindOf<SecondaryStructure>(*ss_ptr), true)
RESULT											

CHECK(RTTI::isInstanceOf<>())
	Protein p;
	Protein*			p_ptr(&p);
	Molecule*			m_ptr(&p);
	BaseFragment*	b_ptr(&p);
	TEST_EQUAL(isInstanceOf<Molecule>(*p_ptr), false)
	TEST_EQUAL(isInstanceOf<Molecule>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<Protein>(*p_ptr), true)
	TEST_EQUAL(isInstanceOf<Protein>(*m_ptr), true)
	TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), true)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*p_ptr), false)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<Residue>(*p_ptr), false)
	TEST_EQUAL(isInstanceOf<Residue>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)
	Molecule m;
	m_ptr = &m;
	b_ptr = &m;
	TEST_EQUAL(isInstanceOf<Molecule>(*m_ptr), true)
	TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), true)
	TEST_EQUAL(isInstanceOf<Protein>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<Residue>(*m_ptr), false)
	TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)
	BaseFragment b;
	b_ptr = &b;
	TEST_EQUAL(isInstanceOf<Molecule>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<Protein>(*b_ptr), false)
	TEST_EQUAL(isInstanceOf<BaseFragment>(*b_ptr), true)
	TEST_EQUAL(isInstanceOf<Residue>(*b_ptr), false)

	Atom*					a_ptr = new Atom();
	System*				s_ptr = new System();
	PDBAtom*		 pa_ptr = new PDBAtom();
	Bond*				 bo_ptr = new Bond();
	NucleicAcid* na_ptr = new NucleicAcid();
	Nucleotide*  nu_ptr = new Nucleotide();
	Chain*				c_ptr = new Chain();
	SecondaryStructure* ss_ptr = new SecondaryStructure;
	TEST_EQUAL(isInstanceOf<Atom>(*a_ptr), true)
	TEST_EQUAL(isInstanceOf<System>(*s_ptr), true)
	TEST_EQUAL(isInstanceOf<PDBAtom>(*pa_ptr), true)
	TEST_EQUAL(isInstanceOf<Bond>(*bo_ptr), true)
	TEST_EQUAL(isInstanceOf<NucleicAcid>(*na_ptr), true)
	TEST_EQUAL(isInstanceOf<Nucleotide>(*nu_ptr), true)
	TEST_EQUAL(isInstanceOf<Chain>(*c_ptr), true)
	TEST_EQUAL(isInstanceOf<SecondaryStructure>(*ss_ptr), true)
RESULT											

CHECK(getDefault<>())
	Protein p;
	p = getDefault<Protein>();
	Atom a;
	a = getDefault<Atom>();
	System s;
	s = getDefault<System>();
	PDBAtom pa;
	pa = getDefault<PDBAtom>();
	Bond b;
	b = getDefault<Bond>();
	NucleicAcid na;
	na = getDefault<NucleicAcid>();
	Nucleotide n;
	n = getDefault<Nucleotide>();
	SecondaryStructure ss;
	ss = getDefault<SecondaryStructure>();
	Residue r;
	r = getDefault<Residue>();
	Chain c;
	c = getDefault<Chain>();
	Molecule m;
	m = getDefault<Molecule>();
	BaseFragment bf;
	bf = getDefault<BaseFragment>();
	Fragment f;
	f = getDefault<Fragment>();
RESULT

CHECK(getNew<>())
	Protein* p = (Protein*)getNew<Protein>();
	TEST_NOT_EQUAL(p, 0)
	Atom* a = (Atom*)getNew<Atom>();
	TEST_NOT_EQUAL(a, 0)
	System* s = (System*)getNew<System>();
	TEST_NOT_EQUAL(s, 0)
	PDBAtom* pa = (PDBAtom*)getNew<PDBAtom>();
	TEST_NOT_EQUAL(pa, 0)
	Bond* b = (Bond*)getNew<Bond>();
	TEST_NOT_EQUAL(b, 0)
	NucleicAcid* na = (NucleicAcid*)getNew<NucleicAcid>();
	TEST_NOT_EQUAL(na, 0)
	Nucleotide* n = (Nucleotide*)getNew<Nucleotide>();
	TEST_NOT_EQUAL(n, 0)
	SecondaryStructure* ss = (SecondaryStructure*)getNew<SecondaryStructure>();
	TEST_NOT_EQUAL(ss, 0)
	Residue* r = (Residue*)getNew<Residue>();
	TEST_NOT_EQUAL(r, 0)
	Chain* c = (Chain*)getNew<Chain>();
	TEST_NOT_EQUAL(c, 0)
	Molecule* m = (Molecule*)getNew<Molecule>();
	TEST_NOT_EQUAL(m, 0)
	BaseFragment* bf = (BaseFragment*)getNew<BaseFragment>();
	TEST_NOT_EQUAL(bf, 0)
	Fragment* f = (Fragment*)getNew<Fragment>();
	TEST_NOT_EQUAL(f, 0)
RESULT

CHECK(getName<>())
// there is not much to check - each damned compiler 
// tries his own demangling!
	TEST_EQUAL(String(getName<Protein>()).hasSubstring("Protein"), true)
	TEST_EQUAL(String(getName<Atom>()).hasSubstring("Atom"), true)
	TEST_EQUAL(String(getName<System>()).hasSubstring("System"), true)
	TEST_EQUAL(String(getName<PDBAtom>()).hasSubstring("PDBAtom"), true)
	TEST_EQUAL(String(getName<Bond>()).hasSubstring("Bond"), true)
	TEST_EQUAL(String(getName<NucleicAcid>()).hasSubstring("NucleicAcid"), true)
	TEST_EQUAL(String(getName<Nucleotide>()).hasSubstring("Nucleotide"), true)
	TEST_EQUAL(String(getName<SecondaryStructure>()).hasSubstring("SecondaryStructure"), true)
	TEST_EQUAL(String(getName<Residue>()).hasSubstring("Residue"), true)
	TEST_EQUAL(String(getName<Chain>()).hasSubstring("Chain"), true)
	TEST_EQUAL(String(getName<Molecule>()).hasSubstring("Molecule"), true)
	TEST_EQUAL(String(getName<BaseFragment>()).hasSubstring("BaseFragment"), true)
	TEST_EQUAL(String(getName<Fragment>()).hasSubstring("Fragment"), true)
RESULT

CHECK(getStreamName<>())
// there is not much to check - each damned compiler 
// tries his own demangling!
	TEST_EQUAL(getStreamName<Protein>(), String("BALL::Protein"))
	TEST_EQUAL(getStreamName<Atom>(), String("BALL::Atom"))
	TEST_EQUAL(getStreamName<System>(), String("BALL::System"))
	TEST_EQUAL(getStreamName<PDBAtom>(), String("BALL::PDBAtom"))
	TEST_EQUAL(getStreamName<Bond>(), String("BALL::Bond"))
	TEST_EQUAL(getStreamName<NucleicAcid>(), String("BALL::NucleicAcid"))
	TEST_EQUAL(getStreamName<Nucleotide>(), String("BALL::Nucleotide"))
	TEST_EQUAL(getStreamName<SecondaryStructure>(), String("BALL::SecondaryStructure"))
	TEST_EQUAL(getStreamName<Residue>(), String("BALL::Residue"))
	TEST_EQUAL(getStreamName<Chain>(), String("BALL::Chain"))
	TEST_EQUAL(getStreamName<Molecule>(), String("BALL::Molecule"))
	TEST_EQUAL(getStreamName<BaseFragment>(), String("BALL::BaseFragment"))
	TEST_EQUAL(getStreamName<Fragment>(), String("BALL::Fragment"))
RESULT

CHECK(castTo<>())/*
	Fragment* f1 = new Fragment();
	BaseFragment bf = castTo<BaseFragment) (*f);
	BaseFragment* bf = new BaseFragment();
	Fragment f = castTo<Fragment) (*f1);*/
RESULT

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
