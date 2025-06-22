import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import unittest
from main.pattern_searching import rabin_karp, kmp, bm_simplified, bm


class TestPatternSearching(unittest.TestCase):
    def setUp(self):
        self.text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit."
        self.pattern = "sit"
        self.pattern_not_found = "tis"
        self.q = 109
    
    def test_rabin_karp(self):
        self.assertTrue(rabin_karp(self.pattern, self.text, self.q))
        self.assertFalse(rabin_karp(self.pattern_not_found, self.text, self.q))
    
    def test_kmp(self):
        self.assertTrue(kmp(self.pattern, self.text))
        self.assertFalse(kmp(self.pattern_not_found, self.text))
    
    def test_bm_simplified(self):
        self.assertTrue(bm_simplified(self.pattern, self.text))
        self.assertFalse(bm_simplified(self.pattern_not_found, self.text))
    
    def test_bm(self):
        self.assertTrue(bm(self.pattern, self.text))
        self.assertFalse(bm(self.pattern_not_found, self.text))

if __name__ == '__main__':
    unittest.main()