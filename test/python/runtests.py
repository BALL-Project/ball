import unittest
import Atom_test, RegularExpression_test, SysInfo_test, String_test

if __name__ == '__main__':
	suite = unittest.TestSuite([e.suite() for e in [
		Atom_test,
		RegularExpression_test,
		SysInfo_test,
		String_test
	]])
	unittest.TextTestRunner(verbosity=2).run(suite)
