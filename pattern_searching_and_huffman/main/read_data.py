
def read_data(plik):  # read data from file

    text = []
    with open(plik, 'r') as file:
        for line in file:
            text.append(line.strip().lower()) # change to lowercase 
    return text
