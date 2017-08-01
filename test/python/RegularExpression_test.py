import unittest
from BALLCore import RegularExpression, String, Substring

class TestRegularExpression(unittest.TestCase):

	def test_ctor(self):
		re = RegularExpression()
		self.assertIsNotNone(re)
		self.assertTrue(re.isValid())
		self.assertEquals(re.getPattern(), '')

	def test_ctorPattern(self):
		re = RegularExpression('abc')
		self.assertIsNotNone(re)
		self.assertTrue(re.isValid())
		self.assertEquals(re.getPattern(), 'abc')

	def test_copyCtor(self):
		re = RegularExpression(RegularExpression('abc'))
		self.assertIsNotNone(re)
		self.assertTrue(re.isValid())
		self.assertEquals(re.getPattern(), 'abc')

	def test_clear(self):
		re = RegularExpression('abc')
		self.assertEquals(re.getPattern(), 'abc')
		re.clear()
		self.assertEquals(re.getPattern(), '')

	def test_destroy(self):
		re = RegularExpression('abc')
		self.assertEquals(re.getPattern(), 'abc')
		re.destroy()
		self.assertEquals(re.getPattern(), '')

	def test_set(self):
		re = RegularExpression()
		self.assertEquals(re.getPattern(), '')
		re.set('abc')
		self.assertEquals(re.getPattern(), 'abc')
		re.set('abc*d')
		self.assertEquals(re.getPattern(), 'abc*d')
		re.set('abc*d', False)
		self.assertEquals(re.getPattern(), 'abc*d')
		re.set('abc*d', True)
		self.assertEquals(re.getPattern(), 'abc.*d')
		re.set(RegularExpression('def'))
		self.assertEquals(re.getPattern(), 'def')

	def test_get(self):
		re = RegularExpression()
		RegularExpression('abc').get(re)
		self.assertEquals(re.getPattern(), 'abc')

	def test_countSubexpressions(self):
		re = RegularExpression('abc')
		self.assertTrue(re.isValid())
		self.assertEquals(re.countSubexpressions(), 0)
		re.set('([A-Z]+[0-9]+).*([0-9])')
		self.assertTrue(re.isValid())
		self.assertEquals(re.countSubexpressions(), 2)

	def test_match(self):
		re = RegularExpression('a[bc]+d')
		s  = 'abbcbbd'
		self.assertTrue(re.isValid())
		self.assertTrue(re.match(s, 0, 0))
		self.assertFalse(re.match(s, 1, 0))
		self.assertTrue(re.match(s, 0))
		self.assertFalse(re.match(s, 1))
		self.assertTrue(re.match(s))
		re.set('.*bd')
		self.assertTrue(re.isValid())
		self.assertTrue(re.match(s))

	def test_matchString(self):
		re = RegularExpression('a[bc]+d')
		s  = String('abbcbbd')
		self.assertTrue(re.isValid())
		self.assertTrue(re.match(s, 0, 0))
		self.assertFalse(re.match(s, 1, 0))
		self.assertTrue(re.match(s, 0))
		self.assertFalse(re.match(s, 1))
		self.assertTrue(re.match(s))
		re.set('.*bd')
		self.assertTrue(re.isValid())
		self.assertTrue(re.match(s))

	def test_matchSubstring(self):
		re = RegularExpression('a[bc]+d')
		s  = String('abbcbbd')
		ss = Substring()
		ss.bind(s)
		self.assertTrue(re.match(ss))
		self.assertFalse(re.match(ss, 1))

	def test_matchStatic(self):
		re = RegularExpression()
		self.assertTrue(re.match('abbcbbd', '.*bd'))
		self.assertTrue(re.match('abbcbbd', 'bd'))
		self.assertTrue(re.match('abbcbbd', 'a*d'))
		self.assertTrue(re.match('abbcbbd', 'a[bc]+d'))
		self.assertFalse(re.match('abbcbbd', 'b.a'))
		self.assertFalse(re.match('abbcbbd', 'a..d'))
		self.assertFalse(re.match('abbcbbd', 'x'))
		self.assertFalse(re.match('abbcbbd', 'a[bc][bc]d'))
		self.assertFalse(re.match('abbcbbd', 'a[b]+d'))
		self.assertFalse(re.match('abbcbbd', 'a[BC]+d'))

	def test_find(self):
		re = RegularExpression('[A-Z]+')
		s  = String('1234ABC123')
		ss = Substring()
		self.assertTrue(re.find(s, ss))
		self.assertTrue(ss.isBound())
		self.assertEquals(ss.size(), 3)
		self.assertEquals(ss.getFirstIndex(), 4)
		ss.set('')
		self.assertEquals(s, '1234123')

	def test_isEmpty(self):
		re = RegularExpression('abc')
		self.assertFalse(re.isEmpty())
		re.set('')
		self.assertTrue(re.isEmpty())

	def test__eq__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertTrue(re2 == re1)
		re1.set('')
		self.assertFalse(re2 == re1)

	def test__ne__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertFalse(re2 != re1)
		re1.set('')
		self.assertTrue(re2 != re1)

	def test__lt__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertFalse(re2 < re1)
		re2.set('abb')
		self.assertTrue(re2 < re1)

	def test__le__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertTrue(re2 <= re1)
		re2.set('abb')
		self.assertTrue(re2 <= re1)
		re2.set('abd')
		self.assertFalse(re2 <= re1)

	def test__gt__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertFalse(re2 > re1)
		re2.set('abd')
		self.assertTrue(re2 > re1)

	def test__ge__(self):
		re1 = RegularExpression('abc')
		re2 = RegularExpression('abc')
		self.assertTrue(re2 >= re1)
		re2.set('abd')
		self.assertTrue(re2 >= re1)
		re2.set('abb')
		self.assertFalse(re2 >= re1)

def suite():
	return unittest.TestLoader().loadTestsFromTestCase(TestRegularExpression)
