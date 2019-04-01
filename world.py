import car as car
import numpy as np
import random
import time
from matplotlib import pyplot as plt
from matplotlib import colors

class World():

    #SPACE IS EMPTY
    EMPTY = 0
    #SPACE IS FULL
    FILLED = 1
    #MAX CAR VELOCITY IS 3
    VMAX = 3

    def __init__(self, lanes, roadLength, new_cars, density):

        self.lanes = lanes
        self.roadLength = roadLength
        self.new_cars = new_cars
        self.cars = []


        #Create a world full of empty cars of size lanes x roadlength
        self.world = np.full((lanes, roadLength), car.Car(0, self.EMPTY, 0, 0, 0), dtype=car.Car)

        for i in range(0, self.lanes):
            for j in range(0, self.roadLength):

                if random.randint(0, 100) <= density:
                    new_car = car.Car(0, self.FILLED, i, j, self.roadLength)
                    self.world[i][j] = new_car
                    self.cars.append(new_car)

        #crate an empty array of cars in the system
        self.points = {}
        self.time = 0
        self.blocked_cars = []
        self.traffic = [(40,3, 0), (80, 0, 0)]

    def generateCars(self):

        new_lanes = list(range(0, self.lanes))

        for i in range(0, self.new_cars):

            #randomly generate a lane for the car to enter
            new_lane = random.randint(0, len(new_lanes) - 1)
            new_lane = new_lanes[new_lane]
            new_lanes.remove(new_lane)
            #create new car
            new_car = car.Car(1, self.FILLED, new_lane, 0, self.roadLength)
            #add car to world
            #

            if (self.world[new_lane][0].exists == 0):
                self.world[new_lane][0] = new_car
                self.cars.append(new_car)


    def findNextCar(self, car):

        #current car position
        cur_row = car.row
        cur_col = car.col
        #distance to next car
        length = 0

        #find next position which occupies a car
        for x in range(cur_col + 1, self.roadLength):

            if self.world[cur_row][x].exists != 0:
                return length
            else:
                length = length + 1

        #arbitary large number to return as set as nextcar.Car distance
        return 1000

    def backward_length(self, car):

        #find the backwards length until hitting a next car and return the car

        length = 0
        for x in list(reversed(range(0, car.col - 1))):

            if (self.world[car.row][x].exists != 0):
                return (self.world[car.row][x], length)
            else:
                length = length + 1

        return (None, 0)

    def propogateForward(self):


        for i, x in enumerate(self.traffic):
            loc, cur_time, on = x

            if (cur_time == 5):

                if on == 0:
                    self.traffic[i] = (loc, 0, 1)
                    print("traffic light at " + str(loc) + " is on")

                else:
                    self.traffic[i] = (loc, 0, 0)
                    print("traffic light at " + str(loc) + " is off")


            else:
                self.traffic[i] = (loc, cur_time + 1, on)




        print(self.blocked_cars)


        remove = []

        for i in range(len(self.blocked_cars)):
            blocked, cur_time = self.blocked_cars[i]
            cur_time = cur_time + 1

            if cur_time == 5:
                self.unblock(blocked)
                remove.append(i)
            else:
                self.blocked_cars[i] = (blocked, cur_time)

        for x in remove:
            del self.blocked_cars[x]


        #every iteration with probability () a car breaks down
        if random.randint(0,500) == 1:
            self.blockedCar(self.lanes, self.roadLength)


        remove = []
        moved = True

        for i, xcar in enumerate(self.cars):
            new_points = self.points.get(xcar, [self.time])
            new_points.append(xcar.col)
            self.points[xcar] = new_points

        #for every car in the cars array
        for i, xcar in enumerate(self.cars):

            length = self.findNextCar(xcar)
            #check for traffic lights
            #
            for x in self.traffic:
                if x[2] == 1:

                    new_length = x[0] - xcar.col

                    if (new_length > 0) & (new_length < length):
                        length = new_length - 1
                        print(length)

            xcar.nextCar = length


            #if car velocity is below maximum and the next car is far enough to allow the car to speed up
            if (xcar.velocity < self.VMAX) & (xcar.nextCar >= (xcar.velocity + 1)):
                xcar.velocity = xcar.velocity + 1

            #if current velocity can't be maintained look to change lanes
            elif (xcar.nextCar < xcar.velocity):
                print("1")
                moved, new_car = self.changeLanes(xcar)

                if (xcar.velocity > (xcar.nextCar - 1)) & moved:
                    xcar.velocity = xcar.nextCar

            #if current velocity can't be mainted just slow down
            elif (xcar.velocity > (xcar.nextCar - 1)):
                xcar.velocity = xcar.nextCar


            if moved == False:

                self.world[xcar.row][xcar.col] = car.Car(0, self.EMPTY, 0, 0, 0)
                xcar.col = new_car.col
                xcar.row = new_car.row
                xcar.velocity = new_car.velocity
                self.world[xcar.row][xcar.col] = xcar
                self.cars[i] = xcar


            else:

                #new position, is current coloumn plus velocity
                new_col = xcar.col + xcar.velocity
                #add an empty car in old position
                self.world[xcar.row][xcar.col] = car.Car(0, self.EMPTY, 0, 0, 0)
                xcar.col = new_col

                if new_col >= self.roadLength:
                    #remove car if it has left road
                    remove.append(xcar)
                else:
                    #else add new position to car
                    self.world[xcar.row][xcar.col] = xcar
                    self.cars[i] = xcar

        for i in remove:
            self.cars.remove(i)

        self.time = self.time + 1


    def changeLanes(self, xcar):
        #changing lane

        #car only change lanes with probability of 0.1
        if random.randint(0, 0) == 9:

            #check if it can change lanes, look at car to the right and left
            car_left = car.Car(1, xcar.exists, xcar.row - 1, xcar.col, 0)
            car_right = car.Car(1, xcar.exists, xcar.row + 1, xcar.col, 0)

            if (car_left.row >= 0):

                print(car_left.row, car_left.col)


                if self.world[car_left.row][car_left.col].exists == 0:

                    forward_length = self.findNextCar(car_left)
                    prev_car, backward_length = self.backward_length(car_left)

                    if prev_car is None:

                        if (car_left.velocity <= forward_length):

                            car_left.velocity = 1
                            print("changing1")
                            return (False, car_left)

                    elif (car_left.velocity <= forward_length) & (prev_car.velocity <= backward_length):

                        car_left.velocity = 1
                        print("changing")
                        return (False, car_left)

            if (car_right.row <= (self.lanes - 1)):

                print(car_right.row, car_right.col)

                if self.world[car_right.row][car_right.col] == 0:

                    forward_length = self.findNextCar(car_right)
                    prev_car, backward_length = self.backward_length(car_right)

                    if prev_car is None:

                        if (car_right.velocity <= forward_length):

                            car_right.velocity = 1
                            print("changing")
                            return (False, car_right)

                    elif (car_right.velocity <= forward_length) & (prev_car.velocity <= backward_length):

                        car_right.velocity = 1
                        print("changing")
                        return (False, car_right)

            return (True, xcar)

        else:

            return (True, xcar)


    def blockedCar(self, lanes, roadLength):

        if (len(self.cars) != 0):

            index = random.randint(0, (len(self.cars) - 1))

            print("blocking")

            blocked = self.cars[index]
            blocked.velocity = 0
            blocked.exists = -1

            self.cars[index].velocity = 0
            print(self.cars[index].col)
            print(self.cars[index].row)
            self.cars.remove(self.cars[index])

            self.blocked_cars.append((blocked, 0))
            print(self.blocked_cars)

    def unblock(self, blocked):

        blocked.exists = 1
        self.cars.append(blocked)
        print("unblocking")

    def trafficOn(self, loc):

        if (self.world[0][loc].exists == 0) & (self.world[1][loc] == 0):

            self.world[0][loc] = car.Car(0, -2, 0, loc, self.roadLength)
            self.world[0][loc] = car.Car(0, -2, 1, loc, self.roadLength)

    def trafficOff(self, loc):
        self.world[0][loc] = car.Car(0, self.EMPTY, 0, 0, 0)
        self.world[1][loc] = car.Car(0, self.EMPTY, 0, 0, 0)


    # def plot(self):

    #     self.num_rep = np.full((self.lanes, self.roadLength), 0, dtype=int)

    #     for x in range(self.roadLength):
    #         for y in range(self.lanes):
    #             if self.world[y][x].exists == 1:
    #                 self.num_rep[y][x] = 1
    #             else:
    #                 self.num_rep[y][x] = 0


    #     cmap = colors.ListedColormap(['white', 'black'])
    #     plt.figure(figsize=(12,1))
    #     plt.pcolor(self.num_rep[::-1], cmap=cmap, edgecolors='k', linewidths=0.5)
    #     # plt.ion()
    #     plt.show()
    #     plt.pause(0.001)