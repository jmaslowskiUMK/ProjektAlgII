# Data Generator Description (*DataGenerator.py*)

*DataGenerator.py* is a Python script that generates synthetic data in CSV format, 
which can then be used as input for other programs within the project for testing or demonstration purposes.

While running the programme you are required to enter: 
- the number of fields,
- the number of breweries,
- the number of pubs.

Total beer volume is defined as the sum of the processed barley amounts from each brewery, 
with each brewery's amount of processed barley being multiplied by a random factor between 3000 and 6400.

### Data generating functions
- *generate_barley_yield()* 
    - This function generates barley fields.
    - Each field has:
        - id,
        - a yield randomly chosen between 3 and 8 tons,
        - random (x, y) coordinates to simulate geographic placement on a map, 
        where x ranges from 0 to 50 (left side) and y ranges from 0 to 100.

- *generate_equal_barley_processed()*
    - This function generates brewery entities.
    - Each brewery has:
        - id,
        - an amount of processed barley randomly chosen between 0.9 and 4 tons,
        - random (x, y) coordinates to simulate geographic placement on a map, 
        where x ranges from 50 to 150 (middle) and y ranges from 0 to 100.

- *distribute_beer_to_pubs()*
    - This function generates pubs.
    - Each pub has:
        - id,
        - a share of the total beer volume randomly chosen between 0 and half of the remaining beer volume,
        - random (x, y) coordinates to simulate geographic placement on a map, 
        where x ranges from 150 to 200 (right side) and y ranges from 0 to 100.

- *generate_lanes_yield_to_brewery()*
    - Each field connects to one or more breweries, with a randomly assigned capacity (in tons).
    - Each lane has:
        - field id,
        - brewery id,
        - type – *"Field-to-Brewery"*,
        - capacity randomly chosen between 0.1 and 1.

- *generate_lanes_brewery_to_pub()*
    - Each brewery connects to one or more pubs, with a randomly assigned capacity (in liters).
    - Each lane has:
        - brewery id,
        - pub id,
        - type – *"Brewery-to-Pub"*,
        - capacity randomly chosen between 100 and 5000.


The output file, *data.csv*, is overwritten with each run.

