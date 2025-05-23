from collections import Counter # A Counter is a dict subclass for counting hashable objects. 
from queue import PriorityQueue # Constructor for a priority queue

class Node(): # nodes for huffman tree

    def __init__(self, value):
        self.value = value
        self.left = None
        self.right = None


def read_data(plik):  # read data from txt file

    text = []
    with open(plik, 'r') as file:
        for line in file:
            text.append(line.strip().lower()) # change to lowercase 
    return text

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
    hc_translate = str.maketrans(hc) # The maketrans() method returns a mapping table that can be used with the translate() method to replace specified characters

    encoded = []
    for line in text:
        encoded.append(line.translate(hc_translate))

    return encoded


if __name__ == "__main__":

    data = read_data('text_for_huffman_example.txt') # read data from txt file
    print(data)
    print(huffman(data))

