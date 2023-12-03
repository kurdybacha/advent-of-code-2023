import sys
import re
import math

file = open(sys.argv[1])
limits = {"red": 12, "green": 13, "blue": 14}
product_sum = 0
for line in file:
    cubes = re.split(";|,", line.split(":")[1])
    minis = {"red": 0, "green": 0, "blue": 0}
    for cube in cubes:
        count, color = cube.strip().split(" ")
        count = int(count)
        if minis[color] < count:
            minis[color] = count
    product_sum += math.prod(minis.values())
print(product_sum)
