#include "Renderer.h"
#include "simulations/NaiveCPU.h"
#include "simulations/GLCompute.h"

#include <random>
#include <sstream>

void read_int(int& target, char* input, std::string error_message) {
    std::istringstream ss(input);
    if (!(ss>>target)) {
        throw error_message;
    }
}

int main(int argc, char** argv) {
    //Input processing
    std::string sim_type = argv[2];
    int num_of_points = 0, num_of_iterations = 0;
    
    if (argc != 3 && argc != 4) {
        std::cerr <<  "Invalid arguments. Supported syntax is:" << '\n';
        std::cerr << "LennardJones <number of particles: int> <simulation type: string>" << '\n';
        std::cerr << "Or:" << '\n';
        std::cerr << "LennardJones <number of particles: int> <simulation type: string> <number of iterations: int>" << '\n';
        return -1;
    }

    if (sim_type != "cpu" && sim_type != "gl_compute") {
        std::cerr << "Invalid simulation type. Supported options are \"cpu\" and \"gl_compute\"" << '\n';
        return -1;
    }

    try { 
        read_int(num_of_points, argv[1], "Invalid number of particles");
        if (argc==4) read_int(num_of_iterations, argv[3], "Invalid number of iterations");
    }

    catch (std::string error_message) {
        std::cerr << error_message << '\n';
        return -1;
    }

    //Simulation parameters
    constexpr float mass = 100.0f, sigma = 0.1f, epsilon = 0.01f, DT = 0.001f, L = 10.0f;
    
    //Initial positions of the particles
    std::vector<Particle> instance_pos;
    instance_pos.reserve(num_of_points);

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> pos_dist(0.0f, 0.5f*L/sqrt(num_of_points));
    std::uniform_real_distribution<> vel_dist(-0.5f, 0.5f);


    for (int i = 0; i < num_of_points; i++) {
        instance_pos.emplace_back(
            L/sqrt(num_of_points) * (0.5f + std::floor(i/sqrt(num_of_points)) ) + pos_dist(e2), 
            L/sqrt(num_of_points) * (0.5f + std::fmod(i, sqrt(num_of_points)) ) + pos_dist(e2),
            vel_dist(e2), 
            vel_dist(e2) );
    }

    //Renderer initialization
    Renderer renderer(&instance_pos, L);
    if(argc == 4) renderer.setIterationsLimit(num_of_iterations);
    renderer.setQuadSize(10.0f);

    //Cpu sim
    if (sim_type == "cpu") { 
        NaiveCPUSim simulator(&instance_pos, DT, epsilon, sigma, mass, L);

        while(!renderer.ShouldClose()) {
            simulator.OnUpdate();

            renderer.UpdateFromCPU(&instance_pos);
            renderer.OnRender();
        }
    }

    //GL compute sim
    else if (sim_type == "gl_compute") {
        GLComputeSim simulator("shaders/ComputeSim.glsl");
        simulator.setL(L);
        simulator.setDT(DT);
        simulator.setEpsilon(epsilon);
        simulator.setSigma(sigma);
        simulator.setMass(mass);
        simulator.setNumber(num_of_points);
        simulator.setHandles(renderer.exposeHandles());

        while (!renderer.ShouldClose()) {
            simulator.OnUpdate();

            renderer.OnRender();
        }
    }

    return 0;
} 
