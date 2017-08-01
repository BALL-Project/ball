import unittest
from BALLCore import String, IndexOverflow, IndexUnderflow

class TestString(unittest.TestCase):

	def test_ctor(self):
		s = String()
		self.assertIsNotNone(s)

	def test_ctorString(self):
		s = String('ABC')
		self.assertEquals(s.size(), 3)
		self.assertEquals(s.c_str()[0], 'A')
		self.assertEquals(s.c_str()[1], 'B')
		self.assertEquals(s.c_str()[2], 'C')

	def test_ctorString3(self):
		s = 'halloh'
		self.assertEquals(String(s), 'halloh')
		self.assertEquals(String(s, 1), 'alloh')
		self.assertEquals(String(s, 1, 2), 'al')
		self.assertEquals(String(s, -1, 0), '')
		self.assertEquals(String(s, -5), 'alloh')
		self.assertEquals(String(s, 2, 3), 'llo')
		self.assertRaises(IndexOverflow, lambda: String(s, 0, 8))
		self.assertRaises(IndexUnderflow, lambda: String(s, -12))

	def test_copyCtor(self):
		s = String(String())
		self.assertEquals(s.size(), 0)
		s = String(String('ABC'))
		self.assertEquals(s.size(), 3)

	def test_copyCtor3(self):
		s = String('Halloh')
		self.assertEquals(String(s, 0, 0), '')
		self.assertEquals(String(s, 2, 4), 'lloh')
		self.assertEquals(String(s, 4, 2), 'oh')
		self.assertEquals(String(s, 3), 'loh')
		self.assertEquals(String(s, -2), 'oh')
		self.assertEquals(String(s, -1), 'h')
		self.assertRaises(IndexOverflow, lambda: String(s, 1, 7))

	def test_cstr(self):
		s = String()
		self.assertEquals(len(s.c_str()), 0)

	def test_size(self):
		s = String()
		self.assertEquals(s.size(), 0)

	def test__eq__(self):
		s1 = String()
		self.assertTrue(s1 == '')
		self.assertTrue(s1 == s1)
		self.assertTrue(s1 == s1.c_str())
		self.assertFalse(s1 == 'Hallo')
		self.assertFalse(s1 == 'Halla')
		self.assertFalse(s1 == 'Halloh')
		self.assertFalse(s1 == 'Hall')

		s2 = String('Hallo')
		self.assertTrue(s2 == 'Hallo')
		self.assertTrue(s2 == s2)
		self.assertTrue(s2 == s2.c_str())
		self.assertFalse(s2 == '')
		self.assertFalse(s2 == 'Halla')
		self.assertFalse(s2 == 'Halloh')
		self.assertFalse(s2 == 'Hall')

		self.assertFalse(s1 == s2)
		self.assertFalse(s2 == s1)
		self.assertFalse(s2 == String('Hall'))
		self.assertFalse(s2 == String('Halloh'))

def suite():
	return unittest.TestLoader().loadTestsFromTestCase(TestString)
