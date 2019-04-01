import car as car
import world as world
import numpy as np
import random
import time
from matplotlib import pyplot as plt
from matplotlib import colors


def simulate(world, num):
    #runs the simulation for num iterations
    for n in range(num):
        world.propogateForward()
        world.generateCars()
        print("\n")
        print(world.world)
        time.sleep(1)


#PEACHSTREET HAS TWO LANES
lanes = 2

#DISTANCE IS 1800FT so approx 125 sqaures
roadlength = 135

#NUMBER OF NEW CARS PER TIME INTERVAL
new_cars = 2

new_world = world.World(lanes, roadlength, new_cars, 50)
new_world.generateCars()
print(new_world.world)

#TIME INTERVALS IS 15
time_interval = 50

simulate(new_world, time_interval)

