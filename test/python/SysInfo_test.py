import unittest
from BALLCore import SysInfo

class TestSysInfo(unittest.TestCase):

	def test_getFreeMemory(self):
		self.assertTrue(SysInfo.getFreeMemory() > 0)

def suite():
	return unittest.TestLoader().loadTestsFromTestCase(TestSysInfo)
