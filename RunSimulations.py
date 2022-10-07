import os

ITERATIONS = 60
PARTICLE_NUMBERS = [128, 256, 512, 1024]

#Cpu simulations
for i in range(len(PARTICLE_NUMBERS)):
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " cpu " + str(ITERATIONS))

ITERATIONS = 60
PARTICLE_NUMBERS = [128, 256, 512, 1024, 2048, 4096]

#CL compute simulations
for i in range(len(PARTICLE_NUMBERS)):
    os.system("./build/bin/LennardJones " + str(PARTICLE_NUMBERS[i]) + " gl_compute " + str(ITERATIONS))
