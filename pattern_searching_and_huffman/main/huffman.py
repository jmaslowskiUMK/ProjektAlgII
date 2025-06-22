from collections import Counter # A Counter is a dict subclass for counting hashable objects. 
from queue import PriorityQueue # Constructor for a priority queue

class Node(): # nodes for huffman tree

    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None


def prepare_data(text):

    letters = []
    for line in text:
        for letter in line:
            letters.append(letter)  
    return letters


# preorder traversal visits the node in the order: root -> left -> right
# additionally saves huffman codes to the 'codes' dictionary 
def preorder(node, codes, code):

    if node.left is None and node.right is None:
        codes[node.value] = code
        return

    preorder(node.left[2], codes, code + "0")
    preorder(node.right[2], codes, code + "1")


def huffman_codes(freq):

    pq = PriorityQueue()

    i = 0
    for key, value in freq.items(): # for each character (key) and its frequency (value) in freq
        # put them into priority queue
        pq.put((value, i, Node(key))) # adding a unique index i to resolve conflicts (same value) in the priority queue
        i += 1
        
    while(pq.qsize() > 1): # as long as there is more than one element in the queue
        
        left = pq.get()
        right = pq.get()

        new = Node(left[0]+right[0])
        new.left = left
        new.right = right

        pq.put((new.value, i, new))
        i += 1

    parent = pq.get()[2] # get() <- EXTRACT-MIN(Q) 
    codes = {}
    code = ''

    preorder(parent, codes, code)

    return codes


def huffman(text):

    data_count = dict(Counter(prepare_data(text))) # count the frequency of each character 
    hc = huffman_codes(data_count) # huffman codes dict 

    encoded = []
    for line in text:
        encoded_line = ''
        for letter in line:
            encoded_line = encoded_line + hc[letter] + ' '
        encoded.append(encoded_line)

    return encoded

