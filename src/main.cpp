#include "Renderer.h"
#include "simulations/NaiveCPU.h"

#include <random>

/*std::random_device rd;
std::mt19937 e2(rd());
std::uniform_real_distribution<> pos_dist(0.0f, L);
std::uniform_real_distribution<> vel_dist(-0.5f, 0.5f);

for (int i = 0; i < num_of_points; i++) {
    instance_pos.emplace_back(
        L / sqrt(num_of_points) * fmod(i, sqrt(num_of_points)),
        L / sqrt(num_of_points) * floor(i / sqrt(num_of_points)),
        vel_dist(e2), vel_dist(e2));

    //instance_pos.emplace_back(pos_dist(e2), pos_dist(e2), vel_dist(e2), vel_dist(e2));
}*/

int main() {
    constexpr int num_of_points = 512;
    constexpr float mass = 100.0f, sigma = 0.0f, epsilon = 0.0f, DT = 0.001f, L = 10.0f;

    std::vector<Particle> instance_pos;
    instance_pos.reserve(num_of_points);

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> pos_dist(0.0f, 0.5f*L/sqrt(num_of_points));
    std::uniform_real_distribution<> vel_dist(-0.5f, 0.5f);


    for (int i = 0; i < num_of_points; i++) {
        instance_pos.emplace_back(
            L/sqrt(num_of_points) * (0.5f + std::floorf(i/sqrt(num_of_points)) ) + pos_dist(e2), 
            L/sqrt(num_of_points) * (0.5f + std::fmod(i, sqrt(num_of_points)) ) + pos_dist(e2),
            vel_dist(e2), 
            vel_dist(e2) );
    }

    NaiveCPUSim simulator(&instance_pos, DT, epsilon, sigma, mass, L);
    Renderer renderer(&instance_pos, L);

    while (!renderer.ShouldClose()) {
        simulator.OnUpdate();

        renderer.UpdateFromCPU(&instance_pos);

        renderer.OnRender();
    }

    return 0;
} 