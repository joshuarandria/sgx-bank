import csv
import pandas as pd

df = pd.read_csv('hashmap.csv')

# Select the 'Temperature' column
amounts = df['key']
print(amounts)
amounts0 = df[df['hash'] == 1]
amounts0=amounts0['value']

# Calculate the mean of the values in the 'Temperature' column
mean_temp = amounts0.mean()

print(mean_temp)