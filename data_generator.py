import numpy as np
import pandas as pd

def paths_from_s_to_fields(how_many_fields, barley=[], choosen_seed=10):
    np.random.seed(choosen_seed)
    #s_to_field = [[0, np.random.randint(1, how_many_fields+1), np.random.randint(10, 50)] for i in range(np.random.randint(how_many_fields-5, how_many_fields+10))]
    if len(barley) == 0:
        barley = [np.random.randint(10, 50) for i in range(how_many_fields)]
    s_to_field = [[0, i+1, barley[i]] for i in range(how_many_fields)]
    df = pd.DataFrame(s_to_field, columns=['from', 'to', 'flow'])
    return df

def paths_from_fileds_to_breweries(how_many_fields, how_many_breweries, choosen_seed=11):
    np.random.seed(choosen_seed)
    field_to_brewery = [[np.random.randint(1, how_many_fields+1), np.random.randint(how_many_fields+1, how_many_fields+how_many_breweries+1), np.random.randint(5, 35)] for i in range(np.random.randint(how_many_breweries-int(0.2*how_many_breweries), how_many_breweries+int(0.5*how_many_breweries)))]
    df = pd.DataFrame(field_to_brewery, columns=['from', 'to', 'flow'])
    return df

def paths_from_breweries_to_inns(how_many_fields, how_many_breweries, how_many_inns, choosen_seed=12):
    np.random.seed(choosen_seed)
    brewery_to_inn = [[np.random.randint(how_many_fields+1, how_many_fields+how_many_breweries+1), np.random.randint(how_many_fields+how_many_breweries+1, how_many_fields+how_many_breweries+how_many_inns+1), np.random.randint(10, 25)] for i in range(np.random.randint(how_many_inns-int(0.2*how_many_inns), how_many_inns+int(0.5*how_many_inns)))]
    df = pd.DataFrame(brewery_to_inn, columns=['from', 'to', 'flow'])
    return df

def paths_from_inns_to_t(how_many_fields, how_many_breweries, how_many_inns, choosen_seed=13):
    np.random.seed(choosen_seed)
    #inn_to_t = [[np.random.randint(how_many_fields+how_many_breweries+1, how_many_fields+how_many_breweries+how_many_inns+1), how_many_fields+how_many_breweries+how_many_inns+1, np.random.randint(15, 30)] for i in range(np.random.randint(how_many_inns-5, how_many_inns+10))]
    inn_to_t = [[how_many_fields+how_many_breweries+i+1, how_many_fields+how_many_breweries+how_many_inns+1, np.random.randint(15, 30)] for i in range(how_many_inns)]
    df = pd.DataFrame(inn_to_t, columns=['from', 'to', 'flow'])
    return df


def generate_data(fields, breweries, inns, barley=[], choosen_seed=10):
    df1 = paths_from_s_to_fields(how_many_fields=fields, barley=barley, choosen_seed=choosen_seed)
    df2 = paths_from_fileds_to_breweries(how_many_fields=fields, how_many_breweries=breweries, choosen_seed=choosen_seed+1)
    df3 = paths_from_breweries_to_inns(how_many_fields=fields, how_many_breweries=breweries, how_many_inns=inns, choosen_seed=choosen_seed+2)
    df4 = paths_from_inns_to_t(how_many_fields=fields, how_many_breweries=breweries, how_many_inns=inns, choosen_seed=choosen_seed+3)
    df = pd.concat([df1, df2, df3, df4], ignore_index=True)
    print(df)
    return df

if __name__ == "__main__":
    fields = int(input('Number of fields: ')) # 12 
    answer = input('Would you like to specify how much barley is growing in each field? (y/n) ')
    if answer == 'y':
        how_much_barley = [input('Field %s: ' %(i+1)) for i in range(fields)]
    else: 
        how_much_barley = []
    breweries = int(input('Number of breweries: ')) # 9
    inns = int(input('Number of inns: ')) # 12
    print('Saving data to csv...')
    generate_data(fields=fields, breweries=breweries, inns=inns, barley=how_much_barley).to_csv("output.csv", index=False)  
    print('Done!')

