#include "NaiveCPU.h"

NaiveCPUSim::NaiveCPUSim(std::vector<Particle>* user_pointer, float dt,
    float epsilon, float sigma, float l) 
    : m_Data(user_pointer), m_DT(dt), m_Epsilon(epsilon), m_Sigma(sigma), m_L(l)
{}

void NaiveCPUSim::OnUpdate() {
    for (size_t i = 0; i < m_Data->size(); i++) {
        //For each particle
        Particle* particle = &m_Data->operator[](i);
        //1. Calculate Forces
        float F_x = 0.0f, F_y = 0.0f;

        for (size_t j = 0; j < m_Data->size(); j++) {
            if (i != j) {
                //TO-DO: Lennard Jones force calculation
                F_x = 0.01f;
                F_y = 0.01f;
            }
        }
        //2. Update velocities
        particle->vel_x += m_DT * F_x;
        particle->vel_y += m_DT * F_y;

        //3. Update positions
        particle->pos_x += m_DT * particle->vel_x;
        particle->pos_y += m_DT * particle->vel_y;

        //4. Enforce boundary conditions
        particle->pos_x = fmod(particle->pos_x, m_L);
        particle->pos_y = fmod(particle->pos_y, m_L);
    }
}