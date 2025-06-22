import sys
import os
sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), '..')))

import unittest
from main.huffman import huffman, prepare_data, huffman_codes

class TestHuffman(unittest.TestCase):

    def test_prepare_data(self):
        text = ["abc", "def"]
        expected = ['a', 'b', 'c', 'd', 'e', 'f']
        self.assertEqual(prepare_data(text), expected)
    
    def test_huffman_codes(self):
        freq = {'a': 5, 'b': 9, 'c': 12, 'd': 13, 'e': 16, 'f': 45}
        codes = huffman_codes(freq)
        expected = {'f': '0', 'c': '100', 'd': '101', 'a': '1100', 'b': '1101', 'e': '111'}
        self.assertEqual(codes, expected)
       
    def test_huffman(self):
        text = ['aaaaa',
                'bbbbbbbbb',
                'cccccccccccc', 
                'ddddddddddddd',
                'eeeeeeeeeeeeeeee',
                'fffffffffffffffffffffffffffffffffffffffffffff']
        encoded = huffman(text)
        expected = ['1100 1100 1100 1100 1100 ',
                    '1101 1101 1101 1101 1101 1101 1101 1101 1101 ',
                    '100 100 100 100 100 100 100 100 100 100 100 100 ',
                    '101 101 101 101 101 101 101 101 101 101 101 101 101 ', 
                    '111 111 111 111 111 111 111 111 111 111 111 111 111 111 111 111 ',
                    '0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 0 ']
        self.assertEqual(encoded, expected)

if __name__ == '__main__':
    unittest.main()
