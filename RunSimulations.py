import os
import time
import matplotlib.pyplot as plt

ITERATIONS = 60
PARTICLE_NUMBERS = [128, 256, 512, 1024, 2048, 4096]

#Cpu simulations
cpu_times = []

for i in range(len(PARTICLE_NUMBERS)):
    t1 = time.time()
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " cpu " + str(ITERATIONS))
    t2 = time.time()
    print("Cpu, " + str(PARTICLE_NUMBERS[i]) + " particles, " + str(ITERATIONS) + " iterations = %s seconds" % (t2-t1))
    cpu_times.append(t2-t1)


#GL compute simulations
gl_compute_times = []

for i in range(len(PARTICLE_NUMBERS)):
    t1 = time.time()
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " gl_compute " + str(ITERATIONS))
    t2 = time.time()
    print("GL Compute, " + str(PARTICLE_NUMBERS[i]) + " particles, " + str(ITERATIONS) + " iterations = %s seconds" % (t2-t1))
    gl_compute_times.append(t2-t1)

plt.xlabel("Number of particles")
plt.ylabel("Execution time (for " + str(ITERATIONS) + " iterations) [s]")
plt.plot(PARTICLE_NUMBERS, cpu_times, '-o')
plt.plot(PARTICLE_NUMBERS, gl_compute_times, '-o')
plt.show()
