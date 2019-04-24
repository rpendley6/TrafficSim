import numpy as np
import random
import time

class Car():

    def __init__(self, velocity, exists, row, col, nextCar, count):

        #Probability there is a reckless driver
        if random.randint(0, 50) == 0:

            self.changeLanes = 8
            self.VMAX = 5

        else:

            self.changeLanes = 1
            self.VMAX = 3

        self.exists = exists
        self.velocity = velocity
        self.col = col
        self.row = row
        self.nextCar = nextCar
        self.count = count


    def __str__(self):

        if (self.exists == 1):
            return "X"
        elif (self.exists == -1):
            return "B"
        elif(self.exists == -2):
            return "S"
        else:
            return str(0);

    def __repr__(self):
        if (self.exists == 1):
            return "X"
        elif (self.exists == -1):
            return "B"
        elif(self.exists == -2):
            return "S"
        else:
            return str(0);


