#include "Utils.h"

#include "Renderer.h"
#include "simulations/NaiveCPU.h"

#include <random>
#include <vector>
#include <cmath>

int main() {
    constexpr int num_of_points = 10000;
    constexpr float sigma = 0.1f, epsilon = 0.1f, DT = 0.01f, L = 0.5f;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(0.0f, L);

    std::vector<Particle> instance_pos;
    instance_pos.reserve(num_of_points);

    for (int i = 0; i < num_of_points; i++) {
        instance_pos.emplace_back(dist(e2), dist(e2), 0.0f, 0.0f);
    }

    NaiveCPUSim simulator(&instance_pos, DT, epsilon, sigma, L);
    Renderer renderer(&instance_pos, L);

    while (!renderer.ShouldClose()) {
        simulator.OnUpdate();

        renderer.UpdateFromCPU(&instance_pos);

        renderer.OnRender();
    }

    return 0;
} 