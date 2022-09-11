#include "NaiveCPU.h"

#include <cmath>
#include <iostream>

NaiveCPUSim::NaiveCPUSim(std::vector<Particle>* user_pointer, float dt,
    float epsilon, float sigma, float m, float l)
    : m_Data(user_pointer), m_DT(dt), m_Epsilon(epsilon), m_Sigma(sigma)
    , m_InvMass(1.0f / m), m_L(l), m_InvL(1.0f / l)
{}

void NaiveCPUSim::CalcForce(float& F_x, float& F_y, size_t i, Particle* particle) {
    F_x = 0.0f, F_y = 0.0f;
    float c_x = 0.0f, c_y = 0.0f;

    for (size_t j = 0; j < m_Data->size(); j++) {
        if (i != j) {
            float d_x = (*m_Data)[j].pos_x - particle->pos_x;
            float d_y = (*m_Data)[j].pos_y - particle->pos_y;
            //Boundary conditions on differences:
            d_x = (abs(d_x) < 0.5f * m_L) ? d_x : -1.0f * utils::sgn(d_x) * (m_L - abs(d_x));
            d_y = (abs(d_y) < 0.5f * m_L) ? d_y : -1.0f * utils::sgn(d_y) * (m_L - abs(d_y));

            float r = sqrt(d_x * d_x + d_y * d_y);
            float x = utils::safediv(m_Sigma, r);

            float f = 48.0f * utils::safediv(m_Epsilon, m_Sigma*m_Sigma)
                            * std::pow(x, 8) * (std::pow(x, 6) - 0.5);

            float f_x = -f * d_x;
            float f_y = -f * d_y;

            utils::Kahan_summation(F_x, f_x, c_x);
            utils::Kahan_summation(F_y, f_y, c_y);
        }
    }
}

void NaiveCPUSim::OnUpdate() {
    float F_x, F_y;

    if (m_FirstIteration) {
        //Initial velocities half-offset for the leapfrog scheme
        for (size_t i = 0; i < m_Data->size(); i++) {
            Particle* particle = &m_Data->operator[](i);

            CalcForce(F_x, F_y, i, particle);

            particle->vel_x += m_InvMass * F_x * 0.5f * m_DT;
            particle->vel_y += m_InvMass * F_y * 0.5f * m_DT;
        }

        m_FirstIteration = false;
    }

    for (size_t i = 0; i < m_Data->size(); i++) {
        //For each particle
        Particle* particle = &m_Data->operator[](i);

        //1. Update positions
        particle->pos_x += particle->vel_x * m_DT;
        particle->pos_y += particle->vel_y * m_DT;

        //2. Enforce boundary conditions
        particle->pos_x -= m_L * std::floor(m_InvL * particle->pos_x);
        particle->pos_y -= m_L * std::floor(m_InvL * particle->pos_y);

        //3. Update velocities
        CalcForce(F_x, F_y, i, particle);

        particle->vel_x += m_InvMass * F_x * m_DT;
        particle->vel_y += m_InvMass * F_y * m_DT;
    }
}
