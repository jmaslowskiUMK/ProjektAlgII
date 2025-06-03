import os

from read_data import read_data 
from pattern_searching import rabin_karp, kmp
from huffman import huffman

if __name__ == "__main__":

    file_name = 'data.csv'
    search_type = 'rk' # rk - rabin karp, kmp - knuth morris pratt
    huffman_coding = True
    save_as = search_type + '_search.csv'

    text = read_data(file_name) # read data from file
    pattern = 'field'

    if huffman_coding:
        data = text.copy()
        data.append(pattern)

        data_huff = huffman(data)
        text_ready = data_huff[:-1]
        pattern_ready = data_huff[-1]

    else:

        text_ready = text
        pattern_ready = pattern


    q = 109

    if os.path.exists(save_as):
        os.remove(save_as)


    for i in range(len(text)):
        line = text_ready[i]
        if search_type == 'rk':
            result = rabin_karp(pattern_ready, line, q, huffman_coding)
        elif search_type == 'kmp':
            result = kmp(pattern_ready, line)

        if result:
            with open(save_as, 'a') as file:
                file.write(text[i] + "\n")

    print('The lines in which the search word was found were saved to the file:', save_as)


