import os

from read_data import read_data 
from pattern_searching import rabin_karp, kmp, bm_simplified, bm
from huffman import huffman

def search(file_name, pattern, search_type, huffman_coding, save_as):

    print('Searching...')

    text = read_data(file_name) # read data from file
    
    if huffman_coding:
        data = text.copy()
        data.append(pattern.lower())

        data_huff = huffman(data)
        text_ready = data_huff[:-1]
        pattern_ready = data_huff[-1]

    else:

        text_ready = text
        pattern_ready = pattern


    q = 109

    if os.path.exists(save_as):
        os.remove(save_as)

    num_of_lines_containing_pattern = 0

    for i in range(len(text)):
        line = text_ready[i]
        if search_type == 'rk': # rabin karp
            result = rabin_karp(pattern_ready, line, q, huffman_coding)
        elif search_type == 'kmp': # knuth morris pratt
            result = kmp(pattern_ready, line)
        elif search_type == 'bm_simplified': # boyer moore, simplified
            result = bm_simplified(pattern_ready, line)
        elif search_type == 'bm': # boyer moore 
            result = bm_simplified(pattern_ready, line)

        if result:
            with open(save_as, 'a') as file:
                num_of_lines_containing_pattern += 1
                file.write(text[i] + "\n")

    print('The lines in which the search word was found were saved to the file:', save_as)
    return num_of_lines_containing_pattern

