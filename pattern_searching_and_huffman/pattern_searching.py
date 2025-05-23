
def plus_minus(word_ascii): # word_ascii[0] - word_ascii[1] + word_ascii[2] - word_ascii[3] + ... 
    total = 0
    for i, el in zip(range(len(word_ascii)), word_ascii):
        total += (-1)**i * el
    return total

def rabin_karp(pattern, text, q): 

    # change to lowercase 
    text = text.lower()
    pattern = pattern.lower()

    # text to ascii
    text_ascii = [ord(letter) for letter in text] # whole text
    pattern_ascii = [ord(letter) for letter in pattern] # pattern

    p_sum_q = plus_minus(pattern_ascii) % q # hash function (pattern)

    n = len(pattern_ascii) # pattern length
    found_patterns_id = []
    same = False # if pattern in text

    for i in range(len(text_ascii)-n + 1): # for each text fragment (pattern length)

        if p_sum_q == plus_minus(text_ascii[i:i+n]) % q: # if hash(pattern) equals hash(fragment)
            # check if pattern and text fragment are the same character by character
            same = True 
            for j in range(n):
                if same:
                    if pattern_ascii[j] != text_ascii[i+j]: 
                        same = False
        
        if same: # if pattern and text fragment are the same 
            found_patterns_id.append(i) # append the starting index of the found pattern 
            same = False # change to false to continue searching  

    return found_patterns_id # patterns positions


if __name__ == "__main__":

    text = "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Proin augue sapien, pulvinar sed vehicula ut, congue eget dolor. Pellentesque lorem urna, mattis sed nisl eget, cursus facilisis lorem. In vel ipsum lacinia, varius ante ac, maximus arcu. Pellentesque massa sapien, blandit at aliquet vel, semper eget ligula. Curabitur pretium finibus tempus. Nulla dictum bibendum nisi a imperdiet. Pellentesque non pharetra nibh, non tempor massa. Maecenas non imperdiet mauris, ut consequat ex. Praesent arcu arcu, tempor et efficitur ac, molestie eget velit. Suspendisse ac facilisis purus, sit amet convallis libero. Integer venenatis lobortis tortor, quis vestibulum metus mattis at. Vestibulum ante ipsum primis in faucibus orci luctus et ultrices posuere cubilia curae;"
    pattern = 'lorem'
    q = 37

    result = rabin_karp(pattern, text, q)
    print(result)

    n = len(pattern) 
    for nr in result:
        print(text[nr:nr+n])

