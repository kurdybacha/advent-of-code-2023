#!/bin/python3

file = open("input.txt")
sum = 0
for line in file:
    for c in line:
        if c.isdigit():
            sum += int(c)*10
            break;
    for c in reversed(line):
        if c.isdigit():
            sum += int(c)
            break;
print(sum)
