import sys

file = open(sys.argv[1])
limits = {
    'red': 12,
    'green': 13,
    'blue': 14
}
sum = 0
for no, line in enumerate(file):
    cub_sets = line.split(':')[1].split(';')
    ok = True
    for cub_set in cub_sets:
        cubes = cub_set.split(',')
        for cube in cubes:
            number, color = cube.strip().split(' ')
            if int(number) > limits[color]:
                ok = False
                break
        if not ok:
            break
    if ok:
        sum += no + 1
print(sum)
