// $Id: RuleEvaluator_test.C,v 1.5 2001/12/30 13:29:01 sturm Exp $
#include <BALL/CONCEPT/classTest.h>

///////////////////////////

#include <BALL/MOLMEC/COMMON/ruleEvaluator.h>
#include <BALL/FORMAT/INIFile.h>
#include <BALL/FORMAT/HINFile.h>
#include <BALL/KERNEL/system.h>
#include <BALL/KERNEL/molecule.h>

///////////////////////////

START_TEST(RuleEvaluator, "$Id: RuleEvaluator_test.C,v 1.5 2001/12/30 13:29:01 sturm Exp $")

/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////

using namespace BALL;

RuleEvaluator* eval;
CHECK(RuleEvaluator::RuleEvaluator())
  eval = new RuleEvaluator;
	TEST_NOT_EQUAL(eval, 0)
RESULT


CHECK(RuleEvaluator::~RuleEvaluator())
  delete eval;
RESULT


CHECK(RuleEvaluator::setPrefix() const )
	eval = new RuleEvaluator;
	eval->setPrefix("PREFIX");
RESULT


CHECK(RuleEvaluator::getPrefix() const )
  TEST_EQUAL(eval->getPrefix(), "PREFIX")
	eval->setPrefix("");
  TEST_EQUAL(eval->getPrefix(), "")
RESULT

INIFile ini("data/RuleEvaluator_test.ini");
ini.read();
CHECK(RuleEvaluator::RuleEvaluator(INIFile& file, const String& prefix))
	TEST_EQUAL(ini.isValid(), true)
	RuleEvaluator eval(ini, "TEST1");
	TEST_EQUAL(eval.getPrefix(), "TEST1")
	TEST_EQUAL(eval.isValid(), true)
RESULT


CHECK(RuleEvaluator::RuleEvaluator(const RuleEvaluator& evaluator))
	RuleEvaluator ev1(ini, "TEST1");
	RuleEvaluator ev2(ev1);
	TEST_EQUAL(ev2.getPrefix(), "TEST1");
	TEST_EQUAL(ev2.isValid(), true)	
RESULT


CHECK(RuleEvaluator::initialize(INIFile& file, const String& prefix))
  //?????
RESULT


CHECK(RuleEvaluator::clear() const)
	RuleEvaluator eval(ini, "TEST1");
	eval.clear();
	TEST_EQUAL(eval.getPrefix(), "")
	TEST_EQUAL(eval.isValid(), false)
RESULT


CHECK(RuleEvaluator::RuleEvaluator& operator = (const RuleEvaluator& evaluator))
  //?????
RESULT


CHECK(RuleEvaluator::set(const RuleEvaluator& evaluator))
  //?????
RESULT


CHECK(RuleEvaluator::String operator () (const Atom& atom) const )
	RuleEvaluator eval(ini, "TEST1");
	HINFile f("data/RuleEvaluator_test.hin");
	System S;
	f >> S;
	f.close();
	TEST_EQUAL(S.countAtoms(), 5)
	
	AtomIterator it = S.beginAtom();
	TEST_EQUAL(eval.operator()(*it), "F-def")
	++it;
	TEST_EQUAL(eval.operator()(*it), "CT")
	++it;
	TEST_EQUAL(eval.operator()(*it), "H-def")
	++it;
	TEST_EQUAL(eval.operator()(*it), "H-def")
	++it;
	TEST_EQUAL(eval.operator()(*it), "")
RESULT


CHECK(RuleEvaluator::isValid() const )
  //?????
RESULT


CHECK(RuleEvaluator::dump(std::ostream& s = std::cout, Size indent_depth = 0) const )
  //?????
RESULT



/////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////
END_TEST
