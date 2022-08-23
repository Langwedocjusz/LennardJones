#version 460 core

struct Particle {
    vec2 position;
    vec2 velocity;
};

layout(std430, binding = 1) buffer particleBuffer
{
    Particle particles[];
};

layout(local_size_x = 1024, local_size_y = 1, local_size_z = 1) in;

uniform uint uFirst;
uniform uint uCount;

uniform float uDT;
uniform float uEpsilon;
uniform float uSigma;
uniform float uL;
uniform float uInvMass;

float safediv(float x, float y) {
    float div = x / y;

    if (isnan(div) || isinf(div))
        return 0.0;
    else
        return div;
}

float sgn(float x) {
    return float(x>0.0) - float(x<0.0);
}

void CalcForce(inout vec2 F, uint i) {
    F = vec2(0.0);
    vec2 c = vec2(0.0);

    for (uint j=0; j<uCount; j++) {
        if (j!=i) {
            vec2 d = particles[j].position - particles[i].position;
            //Boundary conditions on differences:
            d.x = (abs(d.x) < 0.5 * uL) ? d.x : -1.0 * sgn(d.x) * (uL - abs(d.x));
            d.x = (abs(d.y) < 0.5 * uL) ? d.y : -1.0 * sgn(d.y) * (uL - abs(d.y));

            float r = length(d);
            float x = safediv(uSigma, r);

            float f = 48.0 * safediv(uEpsilon, uSigma*uSigma) * pow(x,8.0) * (pow(x,6.0) - 0.5);

            vec2 ff = -f*d;

            F += ff;
        }
    }
}

void main() {
    uint i = gl_GlobalInvocationID.x;

    vec2 F;

    if (uFirst == 0) {
        //Initial velocities half-offset for the leapfrog scheme
        CalcForce(F, i);
        particles[i].velocity += uInvMass * F * 0.5 * uDT;
    }

    //1. Update positions
    particles[i].position += particles[i].velocity * uDT;
    //2. Enforce boundary conditions
    particles[i].position = uL*mod(particles[i].position/uL, 1.0);
    //3. Update velocities
    CalcForce(F, i);
    particles[i].velocity += uInvMass * F * uDT;
}