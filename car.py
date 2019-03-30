import numpy as np
import random
import time
from matplotlib import pyplot as plt
from matplotlib import colors

class Car():

    def __init__(self, velocity, exists, row, col, nextCar):

        self.exists = exists
        self.velocity = velocity
        self.col = col
        self.row = row
        self.nextCar = nextCar

    def __str__(self):

        if (self.exists == 1):
            return "X"
        elif (self.exists == -1):
            return "B"
        else:
            return str(0);

    def __repr__(self):
        if (self.exists == 1):
            return "X"
        elif (self.exists == -1):
            return "B"
        else:
            return str(0);


