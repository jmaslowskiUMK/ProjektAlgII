
# Rabin-Karp Algorithm

def plus_minus(word, huff=False): # word_ascii[0] - word_ascii[1] + word_ascii[2] - word_ascii[3] + ... 
    
    if not huff:
        total = 0
        for i, el in zip(range(len(word)), word):
            total += (-1)**i * el
    else: # if huffman coding then just sum 
        total = sum([sum([int(n) for n in num]) for num in word]) # sum of 'word' = sum(sum of each character)

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
        # splitting text and pattern by ' '
        text_ready = text.split(' ')
        pattern_ready = pattern.split(' ')
        pattern_ready = pattern_ready[:-1] # del the last el ('')


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
            #print('Found!')
            return True # pattern found in text
            #same = False # change to false to continue searching  

    #return found_patterns_id # patterns positions
    return False



# Knuth–Morris–Pratt Algorithm

def pref_suf(p):

    m = len(p)
    
    pi = [-1 for i in range(m)]
    pi[1 -1] = 0
    k = 0 

    for q in range(2, m+1):
        # -1 because the indexes start from 0 (pseudocode from the presentation - start from 1)
        while(k>0 and p[k+1 -1] != p[q -1]): 
            k = pi[k -1]
        if (p[k+1 -1] == p[q -1]):
            k = k + 1
        pi[q -1] = k

    return pi


def kmp(pattern, text):

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    n = len(text)
    m = len(pattern)

    pi = pref_suf(pattern)
    q = 0

    for i in range(1, n+1):
        # -1 because the indexes start from 0 (pseudocode from the presentation - start from 1)
        while (q>0 and pattern[q+1 -1] != text[i -1]):
            q = pi[q -1]
        if (pattern[q+1 -1] == text[i -1]):
            q += 1
        if (q == m):
            #print('Found!') # i-m
            return True
            #q = pi[q -1] 

    return False



# Boyer-Moore Algorithm

def create_last(text, pattern):
    
    # because the indexes start from 1 
    n = len(text) -1
    m = len(pattern) -1

    t_al = set([letter for letter in text])
    p_al = set([letter for letter in pattern])
    al = t_al.union(p_al)
    al = al.difference({'^'})
    al = list(al)
    al.sort() # alphabet
    
    last = {}
    for x in al:
        last[x] = 0

    for i in reversed(range(m +1)): # reverse to go through it less times
        if i>0: # because the indexes start from 1 
            if last[pattern[i]] == 0:
                last[pattern[i]] = i 

    return last

def bm_simplified(pattern, text):

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    # so that the indexes start from 1 
    text = '^' + text
    pattern = '^' + pattern
    
    last = create_last(text, pattern)
    
    # because the indexes start from 1 
    n = len(text) -1
    m = len(pattern) -1
    
    pp = 0
    i = 1

    while(i <= n-m +1):
        j = m
        while (j>0 and pattern[j] == text[i+j-1]):
            j = j-1
        if (j>0):
            i = i+max(1, j-last[text[i+j-1]])
        else: 
            pp = i
            #print('Found!') # i-1
            return True
            #i = i+1
    if pp == 0:
        #print('Pattern not found')
        return False




def bmnext(pattern):
    
    # because the indexes start from 1 
    m = len(pattern) -1
    
    bmn = {}
    for i in range(1, m+1 +1): # icluding m+1
        bmn[i] = 0
        
    i = m+1
    b = m+2
    pi = {}

    pi[i] = b
    
    while i>1:
        while(b<=m+1 and pattern[i-1] != pattern[b-1]):
            if bmn[b] == 0:
                bmn[b] = b-i
            b = pi[b]
        b = b-1
        i = i-1
        pi[i] = b
    
    b = pi[1]
    
    for i in range(1, m+1 +1):
        if bmn[i] == 0:
            bmn[i] = b-1
        if i == b:
            b = pi[b]
            
    return bmn


def bm(pattern, text):

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    # so that the indexes start from 1 
    text = '^' + text
    pattern = '^' + pattern

    last = create_last(text, pattern)
    bmn = bmnext(pattern)
    
    # because the indexes start from 1 
    n = len(text) -1
    m = len(pattern) -1
        
    pp = 0
    i = 1

    while i <= n-m +1:
        j = m 
        
        while j>0 and pattern[j] == text[i+j-1]:
            j = j-1

        if j>0:
            i = i+max(bmn[j+1], j-last[text[i+j-1]])

        else:
            pp = i
            #print('Found!') # i-1
            return True
            #i = i+bmn[1]
            

    if pp == 0:
        #print('Pattern not found')
        return False


