import unittest
from BALLCore import Atom, Element, PTE

class TestAtom(unittest.TestCase):

	def setUp(self):
		self.atom = Atom()

	def test_isValid(self):
		self.assertTrue(self.atom.isValid())

	def test_getCharge(self):
		self.assertAlmostEquals(self.atom.getCharge(), 0)

	def test_setCharge(self):
		self.atom.setCharge(1.23456)
		self.assertAlmostEquals(self.atom.getCharge(), 1.23456)

	def test_getName(self):
		self.assertEquals(self.atom.getName(), '')

	def test_setName(self):
		self.atom.setName('ATOMNAME')
		self.assertEquals(self.atom.getName(), 'ATOMNAME')
		self.atom.setName('')
		self.assertEquals(self.atom.getName(), '')

	def test_getElement(self):
		self.assertTrue(self.atom.getElement().isUnknown())

	def test_setElement(self):
		self.atom.setElement(PTE.getElement(1))
		self.assertEquals(self.atom.getElement(), PTE.getElement(1))

def suite():
	return unittest.TestLoader().loadTestsFromTestCase(TestAtom)
