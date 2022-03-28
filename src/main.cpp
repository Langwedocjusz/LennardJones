#include "Renderer.h"

#include <random>
#include <vector>

int main() {
    constexpr int num_of_points = 3000;

    std::random_device rd;
    std::mt19937 e2(rd());
    std::uniform_real_distribution<> dist(-1.0, 1.0);

    std::vector<float> instance_pos;
    instance_pos.reserve(4 * num_of_points);

    for (int i = 0; i < num_of_points; i++) {
        instance_pos.emplace_back(dist(e2)); //pos_x
        instance_pos.emplace_back(dist(e2)); //pox_y
        instance_pos.emplace_back(0.0f);     //vel_x
        instance_pos.emplace_back(0.0f);     //vel_y
    }

    Renderer renderer(&instance_pos);

    while (!renderer.ShouldClose()) {
        renderer.OnRender();
    }

    return 0;
} 