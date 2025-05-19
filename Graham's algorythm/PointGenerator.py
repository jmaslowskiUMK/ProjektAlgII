import random
import csv

def generate_points(min_x, max_x, min_y, max_y, number_of_points):
    return [(random.randint(min_x, max_x), random.randint(min_y, max_y)) for _ in range(number_of_points)]

def save_points_to_csv(points, filename):
    with open(filename, mode='w', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["X", "Y"]) # Nagłówki CSV
        writer.writerows(points)

min_x, max_x = -1000, 1000
min_y, max_y = -1000, 1000
number_of_points = 10000

points = generate_points(min_x, max_x, min_y, max_y, number_of_points)

# Zapis do pliku
filename = "Points.csv"
save_points_to_csv(points, filename)

print(f"Punkty zostały zapisane w {filename}")
