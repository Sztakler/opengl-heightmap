from math import exp, floor
from numpy import linspace

xs = linspace(0.0, 0.188, 101)

velocities = []

print("{", end="")

a = 20.0
b = 20.0
c = 20.0
d = -3.5

for i in range(len(xs)):
    x = xs[i]
    velocities.append(exp(a*x + b*x**2 + c*x**3 + d))

    print(float(velocities[i]), end=", ")
    if (i % 10 == 0 and i > 0):
        print()

print("};", end="")

# h = linspace(10, 50, 1000)

# for i in range(len(h)):
#     height = floor((h[i] - 10) * 2.5)
#     print(height, (h[i] - 10) * 2.5, velocities[height])
#     # velocity = velocities[1]

# l = 10
# h = 50

# l -= 10
# h -= 10

