import csv


data = []
keys = []
count = 0

with open('trajectories.csv') as csv_file:

    csv_reader = csv.reader(csv_file, delimiter=',')
    for row in csv_reader:

        if (count != 0):


            if row[0] not in keys:

                if ((int(row[2]) == 101) and (int(row[3]) == 214)):

                    data.append((row[0], row[1]))
                    keys.append(row[0])



        count = count + 1

times = []

data.sort(key=lambda x: x[1])



for i in range(0, len(data) - 1):

    cur1 = data[i][1]
    cur2 = data[i + 1][1];

    diff = int(cur2) - int(cur1)
    times.append(diff)


with open('employee_file.csv', mode='w', newline="") as employee_file:
    employee_writer = csv.writer(employee_file, delimiter=',', quotechar='"', quoting=csv.QUOTE_MINIMAL)

    for i,j in enumerate(times):

        employee_writer.writerow([i, j])