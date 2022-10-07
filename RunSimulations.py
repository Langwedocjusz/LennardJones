import os
import time

ITERATIONS = 60
PARTICLE_NUMBERS = [128, 256, 512, 1024, 2048, 4096]

#Cpu simulations
for i in range(len(PARTICLE_NUMBERS)):
    t1 = time.time()
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " cpu " + str(ITERATIONS))
    t2 = time.time()
    print("Cpu, " + str(PARTICLE_NUMBERS[i]) + " particles, " + str(ITERATIONS) + " iterations = %s seconds" % (t2-t1))

ITERATIONS = 60
PARTICLE_NUMBERS = [128, 256, 512, 1024, 2048, 4096]

#CL compute simulations
for i in range(len(PARTICLE_NUMBERS)):
    t1 = time.time()
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " gl_compute " + str(ITERATIONS))
    t2 = time.time()
    print("GL Compute, " + str(PARTICLE_NUMBERS[i]) + " particles, " + str(ITERATIONS) + " iterations = %s seconds" % (t2-t1))
