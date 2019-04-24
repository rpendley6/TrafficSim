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
        print("n: " + str(n))
        world.propogateForward()
        world.generateCars()
        print("\n")
        print(world.world)
        time.sleep(1)


#PEACHSTREET HAS TWO LANES
lanes = 2

#AVERAGAE LENGTH OF CAR IS 14.4FT

#DISTANCE IS 1800FT so approx 128 sqaures
roadlength = 128

#NUMBER OF NEW CARS PER TIME INTERVAL
new_cars = 2

new_world = world.World(lanes, roadlength, new_cars, 75)
new_world.generateCars()
print(new_world.world)

#TIME INTERVALS IS 15
time_interval = 584

simulate(new_world, time_interval)

# PEACHTREE STREET FROM 10th to 14th
# 10th to 11th: Approx 535FT which is around 37 Squares
# 11th to 12th: Approx 485FT which is around 35 Sqaures (37 + 35 = 72)
# 12th to 14th (No Traffic Light at 13th): Approx 800FT which is around 56 Squares ()