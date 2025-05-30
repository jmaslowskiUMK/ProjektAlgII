import random
import csv
import math
import random
import matplotlib.pyplot as plt
import numpy as np
from math import atan2


class Point:
    def __init__(self, x, y):
        self.x = x
        self.y = y

def generate_points(min_x, max_x, min_y, max_y, number_of_points):
    return [Point(random.randint(min_x, max_x), random.randint(min_y, max_y)) for _ in range(number_of_points)]

def random_start_point(points):
    return random.choice(points)

def is_inside_circle(center, p, radius):
    return math.hypot(p.x - center.x, p.y - center.y) <= radius

def orientation(p, q, r):
    #all changed to float64 for consistency
    return (q[0] - p[0])*(r[1] - p[1]) - (q[1] - p[1])*(r[0] - p[0])

def graham_scan(points):
    if len(points) < 3:
        return points

    start = min(points, key=lambda p: (p[1], p[0]))

    def polar_angle(p):
        return atan2(p[1] - start[1], p[0] - start[0])

    def distance(p):
        return (p[0] - start[0])**2 + (p[1] - start[1])**2

    points = sorted(points, key=lambda p: (polar_angle(p), distance(p)))

    hull = []
    for point in points:
        while len(hull) >= 2 and orientation(hull[-2], hull[-1], point) < 1e-14:  #epsilon for floating point precision
            hull.pop()
        hull.append(point)
    return hull


def generate_unique_coordinates(existing_coordinates, x_min, x_max, y_min, y_max):
    while True:
        x = random.randint(x_min, x_max)
        y = random.randint(y_min, y_max)
        if (x, y) not in existing_coordinates:
            existing_coordinates.add((x, y))
            return x, y

def generate_barley_yield(number_of_fields):
    fields = []
    existing_coordinates = set()
    for i in range(number_of_fields):
        value = random.randint(3000, 8000)  # random yield
        x, y = generate_unique_coordinates(existing_coordinates, -1000, 1000, -1000, 1000)
        fields.append({"id": i * 3, "yield": value, "x": x, "y": y})
    return fields

def generate_equal_barley_processed(number_of_breweries):
    breweries = []
    existing_coordinates = set()
    processed_value = random.randint(900, 4000)  # random processed yield
    for i in range(number_of_breweries):
        x, y = generate_unique_coordinates(existing_coordinates, -1000, 1000, -1000, 1000)
        breweries.append({"id": i * 3 + 1, "processed": processed_value, "x": x, "y": y})
    return breweries

def distribute_beer_to_pubs(total_beer, number_of_pubs):
    pubs = []
    existing_coordinates = set()
    remaining_beer = total_beer
    for i in range(number_of_pubs - 1):
        share = random.randint(0, remaining_beer // 2)
        x, y = generate_unique_coordinates(existing_coordinates, -1000, 1000, -1000, 1000)
        pubs.append({"id": i * 3 + 2, "beer": share, "x": x, "y": y})
        remaining_beer -= share
    x, y = generate_unique_coordinates(existing_coordinates, -1000, 1000, -1000, 1000)
    pubs.append({"id": (number_of_pubs - 1) * 3 + 2, "beer": remaining_beer, "x": x, "y": y})
    return pubs

def generate_lanes_yield_to_brewery(fields, breweries, repair_probability):
    lanes = []
    for field in fields:
        number_of_lanes = random.randint(1, len(breweries))
        destinations = random.sample(breweries, number_of_lanes)
        for dest in destinations:
            capacity = random.randint(100, 1000)
            repair_cost = random.randint(1, 10) if random.random() < repair_probability else 0
            lanes.append({
                "from": field["id"],
                "to": dest["id"],
                "capacity": capacity,
                "repair_cost": repair_cost
            })
    return lanes

def generate_lanes_brewery_to_pub(breweries, pubs, repair_probability):
    lanes = []
    for brewery in breweries:
        number_of_lanes = random.randint(1, len(pubs))
        destinations = random.sample(pubs, number_of_lanes)
        for dest in destinations:
            capacity = random.randint(100, 5000)
            repair_cost = random.randint(1, 10) if random.random() < repair_probability else 0
            lanes.append({
                "from": brewery["id"],
                "to": dest["id"],
                "capacity": capacity,
                "repair_cost": repair_cost
            })
    return lanes

def save_all_to_csv(filename, fields, breweries, pubs, lanes, hulls=None, hull_numbers=None):
    with open(filename, mode='w', newline='', encoding='utf-8') as file:
        writer = csv.writer(file)
        writer.writerow([
            "Category", "ID", "Yield (kg)", "Processed (kg)", "Beer (liters)", 
            "X Coordinate", "Y Coordinate", "Lane From", "Lane To", 
            "Capacity (kg/liters)", "Repair Cost", "Ground Class"
        ])
        for field in fields:
            writer.writerow([
                "Field", field["id"], field["yield"], "", "",
                field["x"], field["y"], "", "", "", "", ""
            ])
        for brewery in breweries:
            writer.writerow([
                "Brewery", brewery["id"], "", brewery["processed"], "",
                brewery["x"], brewery["y"], "", "", "", "", ""
            ])
        for pub in pubs:
            writer.writerow([
                "Pub", pub["id"], "", "", pub["beer"],
                pub["x"], pub["y"], "", "", "", "", ""
            ])
        for lane in lanes:
            writer.writerow([
                "Lane", "", "", "", "",
                "", "", lane["from"], lane["to"], 
                lane["capacity"], lane["repair_cost"], ""
            ])
        # save hulls with numbers
        if hulls and hull_numbers:
            for hull_idx, (hull, hull_num) in enumerate(zip(hulls, hull_numbers)):
                writer.writerow([f"Hull_{hull_idx}", "Array of Points", "", "", "", "", "", "", "", "", "", hull_num])
                """for idx, p in enumerate(hull):
                    writer.writerow([f"HullPoint_{hull_idx}", idx, "", "", "", p.x, p.y, "", "", "", "", hull_num])"""
                for idx, (x, y) in enumerate(hull):
                    writer.writerow([f"HullPoint_{hull_idx}", idx, "", "", "", x, y, "", "", "", "", hull_num])


number_of_fields = int(input("Enter the number of fields: "))
fields = generate_barley_yield(number_of_fields)

number_of_breweries = int(input("Enter the number of breweries: "))
breweries = generate_equal_barley_processed(number_of_breweries)

total_beer = sum([brewery["processed"] * random.randint(1, 10) for brewery in breweries])

number_of_pubs = int(input("Enter the number of pubs: "))
pubs = distribute_beer_to_pubs(total_beer, number_of_pubs)

repair_probability = float(input("Enter probability (0 to 1) that a lane has repair cost > 0: "))

lanes_field_to_brewery = generate_lanes_yield_to_brewery(fields, breweries, repair_probability)
lanes_brewery_to_pub = generate_lanes_brewery_to_pub(breweries, pubs, repair_probability)

combined_lanes = lanes_field_to_brewery + lanes_brewery_to_pub

# generating points for convex hulls within canvas, for now input number of hulls (later add generating for all canvas????), possible with large radius
max_x = 1000
max_y = 1000
min_x = -1000
min_y = -1000
points = generate_points(min_x, max_x, min_y, max_y, 10000)
radius = int(input("Enter the radius of the circles (for visuals at least 150): "))
#number_of_hulls = int(input("Enter the number of hulls: "))

hulls = []

#for i in range(number_of_hulls):
 #   if len(points) < 3:
  #      print(f"Not enough points left to create hull {i+1}")
   #     break
    #center = random_start_point(points)
    #points_in_circle = [p for p in points if is_inside_circle(center, p, radius)]
    #if len(points_in_circle) < 3: # at least 3 points needed for a hull
     #   print(f"Not enough points in circle for hull {i+1}")
      #  continue
 #   hull = convex_hull(points_in_circle)
  #  hulls.append(hull)
   # points = [p for p in points if not is_inside_circle(center, p, radius)]
while len(points) >= 3:
    center = random_start_point(points)
    if len(points) < 3:
        print("Not enough points left to create hull")
        break
    points_in_circle = [p for p in points if is_inside_circle(center, p, radius)]
    if len(points_in_circle) < 3:
        print("Not enough points in circle for hull")
        points = [p for p in points if not is_inside_circle(center, p, radius)]
        continue
    # Zamiana Point na tuple (x, y)
    points_tuples = [(p.x, p.y) for p in points_in_circle]
    hull_tuples = graham_scan(points_tuples)
    #print(hull_tupls)
    # Zamiana z powrotem na Point
    #hull = [ Point(x, y) for x, y in hull_tuples ]
    #print(hull)
    hulls.append([hull_tuples[i] for i in range(len(hull_tuples))])
    hull_numbers = [random.randint(1, 6) for _ in range(len(hulls))]
    #print(hulls)
    points = [p for p in points if not is_inside_circle(center, p, radius)]
save_all_to_csv("input_data.csv", fields, breweries, pubs, combined_lanes, hulls, hull_numbers)
print("Data have been saved to 'input_data.csv'")

""""
plt.figure(figsize=(10, 10))
colors = plt.cm.get_cmap('tab20', len(hulls))

for idx, hull in enumerate(hulls):
    if len(hull) < 3:
        continue
    xs = [p[0] for p in hull] + [hull[0][0]]
    ys = [p[1] for p in hull] + [hull[0][1]]
    plt.plot(xs, ys, marker='o', label=f'Hull {idx+1}', color=colors(idx))

plt.title("All Convex Hulls")
plt.xlabel("X")
plt.ylabel("Y")
plt.legend()
plt.grid(True)
plt.show()
"""