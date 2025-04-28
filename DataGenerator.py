import random
import csv

def generate_barley_yield(number_of_fields):
    fields = []
    for i in range(number_of_fields):
        value = random.randint(3000, 8000)  # random yield between 3000 and 8000 kg
        x = random.randint(0, 50)  # fields on the left (as integers)
        y = random.randint(0, 100)  # random y coordinate (as integers)
        fields.append({"id": i * 3, "yield": value, "x": x, "y": y})  # IDs as 0, 3, 6, etc.
    return fields

def generate_equal_barley_processed(number_of_breweries):
    breweries = []
    processed_value = random.randint(900, 4000)  # random processed yield between 900 and 4000 kg
    for i in range(number_of_breweries):
        x = random.randint(50, 150)  # Breweries in the middle (as integers)
        y = random.randint(0, 100)  # random y coordinate (as integers)
        breweries.append({"id": i * 3 + 1, "processed": processed_value, "x": x, "y": y})  # IDs as 1, 4, 7, etc.
    return breweries

def distribute_beer_to_pubs(total_beer, number_of_pubs):
    pubs = []
    remaining_beer = total_beer
    for i in range(number_of_pubs - 1):  # distributing to all pubs except the last one
        share = random.randint(0, remaining_beer // 2)  # Random share as integers
        x = random.randint(150, 200)  # pubs on the right (as integers)
        y = random.randint(0, 100)  # random y coordinate (as integers)
        pubs.append({"id": i * 3 + 2, "beer": share, "x": x, "y": y})  # IDs as 2, 5, 8, etc.
        remaining_beer -= share
    x = random.randint(150, 200)  # last pub to close circle
    y = random.randint(0, 100)  # random y coordinate (as integers)
    pubs.append({"id": (number_of_pubs - 1) * 3 + 2, "beer": remaining_beer, "x": x, "y": y})  # Last pub ID follows the pattern
    return pubs


def generate_lanes_yield_to_brewery(fields, breweries):
    lanes = []
    for field in fields:
        number_of_lanes = random.randint(1, len(breweries))  # each field can have multiple lanes
        destinations = random.sample(breweries, number_of_lanes)  # randomly choose breweries
        for dest in destinations:
            capacity = random.randint(100, 1000)  # transport capacity in kg (100 to 1000 kg per trip)
            lanes.append({
                "from": field["id"],
                "to": dest["id"],
                "capacity": capacity
            })
    return lanes

def generate_lanes_brewery_to_pub(breweries, pubs):
    lanes = []
    for brewery in breweries:
        number_of_lanes = random.randint(1, len(pubs))  # each brewery can have multiple lanes
        destinations = random.sample(pubs, number_of_lanes)  # randomly chosen pubs
        for dest in destinations:
            capacity = random.randint(100, 5000)  # capacity in liters
            lanes.append({
                "from": brewery["id"],
                "to": dest["id"],
                "capacity": capacity
            })
    return lanes


def save_all_to_csv(filename, fields, breweries, pubs, lanes):
    """
    Saves all generated data into one CSV file with combined lanes.
    """
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        
        # writing headers
        writer.writerow([
            "Category", "ID", "Yield (kg)", "Processed (kg)", "Beer (liters)", 
            "X Coordinate", "Y Coordinate", "Lane From", "Lane To", "Capacity (kg/liters)"
        ])
        
        # writing fields
        for field in fields:
            writer.writerow([
                "Field", field["id"], field["yield"], "", "",
                field["x"], field["y"], "", "", "", ""
            ])
        
        # writing breweries
        for brewery in breweries:
            writer.writerow([
                "Brewery", brewery["id"], "", brewery["processed"], "",
                brewery["x"], brewery["y"], "", "", "", ""
            ])
        
        # writing pubs
        for pub in pubs:
            writer.writerow([
                "Pub", pub["id"], "", "", pub["beer"],
                pub["x"], pub["y"], "", "", "", ""
            ])
        
        # writing all lanes
        for lane in lanes:
            writer.writerow([
                "Lane", "", "", "", "",
                "", "", lane["from"], lane["to"], lane["capacity"]
            ])

number_of_fields = int(input("Enter the number of fields: "))
fields = generate_barley_yield(number_of_fields)

number_of_breweries = int(input("Enter the number of breweries: "))
breweries = generate_equal_barley_processed(number_of_breweries)

total_beer = sum([brewery["processed"] * random.randint(3000, 6400) for brewery in breweries])

number_of_pubs = int(input("Enter the number of pubs: "))
pubs = distribute_beer_to_pubs(total_beer, number_of_pubs)

lanes_field_to_brewery = generate_lanes_yield_to_brewery(fields, breweries)
lanes_brewery_to_pub = generate_lanes_brewery_to_pub(breweries, pubs)

# lanes in one list 
combined_lanes = lanes_field_to_brewery + lanes_brewery_to_pub

save_all_to_csv("data.csv", fields, breweries, pubs, combined_lanes)
print("Data have been saved to 'data.csv'")