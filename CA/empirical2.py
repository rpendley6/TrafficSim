import csv


data = {}
count = 0

with open('trajectories.csv') as csv_file:

    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:

        if (count != 0):

            if ((int(row[2]) == 101) and (int(row[3]) == 214)):

                if row[0] not in data:

                    data[row[0]] = (row[1], 0)
                else:
                    data[row[0]] = (data[row[0]][0], row[1])

        count = count + 1

times = []


for key in data:

    a,b = data[key]
    times.append(int(b) - int(a))

print(times)
print(sum(times) / len(times))