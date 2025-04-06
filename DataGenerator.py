import random
import csv

def generate_barley_yield(number_of_fields):
    fields = []
    for i in range(number_of_fields):
        value = random.uniform(3, 8)  # random value between 3 and 8 tons
        x = random.uniform(0, 50)  # fields on the left (just to make it look like in an algorithm)
        y = random.uniform(0, 100)  # random y coord
        fields.append({"id": f"Field_{i+1}", "yield": round(value, 2), "x": round(x, 2), "y": round(y, 2)})
    return fields

def generate_equal_barley_processed(number_of_breweries):
    breweries = []
    processed_value = random.uniform(0.9, 4)  # random value between 0.9 and 4 tons
    for i in range(number_of_breweries):
        x = random.uniform(50, 150)  # Breweries in the middle (just to make it look like in an algorithm)
        y = random.uniform(0, 100)  # random y coord
        breweries.append({"id": f"Brewery_{i+1}", "processed": round(processed_value, 2), "x": round(x, 2), "y": round(y, 2)})
    return breweries

def distribute_beer_to_pubs(total_beer, number_of_pubs):
    pubs = []
    remaining_beer = total_beer
    for i in range(number_of_pubs - 1):  # distributing to all pubs except the last one
        share = round(random.uniform(0, remaining_beer / 2), 2)  # Random share
        x = random.uniform(150, 200)  # pubs on the right (just to make it look like in an algorithm)
        y = random.uniform(0, 100)  # random y coord
        pubs.append({"id": f"Pub_{i+1}", "beer": share, "x": round(x, 2), "y": round(y, 2)})
        remaining_beer -= share
    x = random.uniform(150, 200)  # last pub to close circle
    y = random.uniform(0, 100)  # random y coord
    pubs.append({"id": f"Pub_{number_of_pubs}", "beer": round(remaining_beer, 2), "x": round(x, 2), "y": round(y, 2)})
    return pubs

def generate_lanes_yield_to_brewery(fields, breweries):
    lanes = []
    for field in fields:
        number_of_lanes = random.randint(1, len(breweries))  # each field can have multiple lanes
        destinations = random.sample(breweries, number_of_lanes)  # randomly choose breweries
        for dest in destinations:
            capacity = random.uniform(0.1, 1)  # transport capacity in tons (0.1 to 7 ton per trip)
            lanes.append({
                "from": field["id"],
                "to": dest["id"],
                "type": "Field-to-Brewery",
                "capacity": round(capacity, 2)
            })
    return lanes

def generate_lanes_brewery_to_pub(breweries, pubs):
    lanes = []
    for brewery in breweries:
        number_of_lanes = random.randint(1, len(pubs))  # each and every brewery can have multiple lanes
        destinations = random.sample(pubs, number_of_lanes)  # randomly choosen pubs
        for dest in destinations:
            capacity = random.randint(100, 5000)  # capacity in liters
            lanes.append({
                "from": brewery["id"],
                "to": dest["id"],
                "type": "Brewery-to-Pub",
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
            "Category", "ID", "Yield (tons)", "Processed (tons)", "Beer (liters)", 
            "X Coordinate", "Y Coordinate", "Lane From", "Lane To", "Lane Type", "Capacity (tons/liters)"
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
                "", "", lane["from"], lane["to"], lane["type"], lane["capacity"]
            ])

number_of_fields = int(input("Enter the number of fields: "))
fields = generate_barley_yield(number_of_fields)

number_of_breweries = int(input("Enter the number of breweries: "))
breweries = generate_equal_barley_processed(number_of_breweries)

total_beer = round(sum([brewery["processed"] * random.uniform(3000, 6400) for brewery in breweries]), 2)

number_of_pubs = int(input("Enter the number of pubs: "))
pubs = distribute_beer_to_pubs(total_beer, number_of_pubs)

lanes_field_to_brewery = generate_lanes_yield_to_brewery(fields, breweries)
lanes_brewery_to_pub = generate_lanes_brewery_to_pub(breweries, pubs)

# lanes in one list 
combined_lanes = lanes_field_to_brewery + lanes_brewery_to_pub

save_all_to_csv("data.csv", fields, breweries, pubs, combined_lanes)
print("Data have been saved to 'data.csv'")
