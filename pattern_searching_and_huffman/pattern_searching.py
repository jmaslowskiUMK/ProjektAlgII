
def plus_minus(word, huff=False): # word_ascii[0] - word_ascii[1] + word_ascii[2] - word_ascii[3] + ... 
    
    if not huff:
        total = 0
        for i, el in zip(range(len(word)), word):
            total += (-1)**i * el
    else: # if huffman coding then just sum 
        total = sum([int(num) for num in word])

    return total


def rabin_karp(pattern, text, q, huff=False): 

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    if not huff: # if we're not using huffman coding then we need to switch words into ascii codes
        # text to ascii
        text_ready = [ord(letter) for letter in text] # whole text
        pattern_ready= [ord(letter) for letter in pattern] # pattern
    else:
        text_ready = text
        pattern_ready = pattern

    p_sum_q = plus_minus(pattern_ready, huff) % q # hash function (pattern)

    n = len(pattern_ready) # pattern length
    found_patterns_id = []
    same = False # if pattern in text

    for i in range(len(text_ready)-n + 1): # for each text fragment (pattern length)

        if p_sum_q == plus_minus(text_ready[i:i+n], huff) % q: # if hash(pattern) equals hash(fragment)
            # check if pattern and text fragment are the same character by character
            same = True 
            for j in range(n):
                if same:
                    if pattern_ready[j] != text_ready[i+j]: 
                        same = False
        
        if same: # if pattern and text fragment are the same 
            found_patterns_id.append(i) # append the starting index of the found pattern 
            print('Found!')
            return True # pattern found in text
            #same = False # change to false to continue searching  

    #return found_patterns_id # patterns positions
    return False

def pref_suf(p):

    m = len(p)
    
    pi = [-1 for i in range(m)]
    pi[1 -1] = 0
    k = 0 

    for q in range(2, m+1):
        #print(k, q, p[q -1], p[k+1 -1]) # bo indeksy od 0, a w prezentacji od 1
        while(k>0 and p[k+1 -1] != p[q -1]):
            k = pi[k -1]
        if (p[k+1 -1] == p[q -1]):
            k = k + 1
        pi[q -1] = k

        #print(pi)

    return pi


def kmp(pattern, text):

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    n = len(text)
    m = len(pattern)

    pi = pref_suf(pattern)
    #print(pi)
    q = 0

    for i in range(1, n+1):
        #print('\n', i, q, text[i -1], pattern[q+1 -1])
        while (q>0 and pattern[q+1 -1] != text[i -1]):
            q = pi[q -1]
            #print('pi[q]: ', q)
        if (pattern[q+1 -1] == text[i -1]):
            q += 1
            #print('q+1: ', q)
        if (q == m):
            #print("wzorzec wystepuje z przesunieciem ", i-m)
            print('Found!')
            return True
            q = pi[q -1]
            #print('pi[q]: ', q)

    return False



