#include "r_physics.h"

// PLANET

vec3_t r_physics_orbit_to_local(planet_t *planet, vec3_t position) {
    float ci = cos(planet -> inclination);
    float si = sin(planet -> inclination);

    float y = position.y * ci - position.z * si;
    float z = position.y * si + position.z * ci;
    position.y = y;
    position.z = z;

    float cn = cos(planet -> node);
    float sn = sin(planet -> node);

    float x = position.x * cn + position.z * sn;
    z = -position.x * sn + position.z * cn;
    position.x = x;
    position.z = z;

    return vec3_add(position, planet -> parent -> state.position);
}

void r_physics_planet_state_update(planet_t *planet, double time) {
    if (planet -> parent != NULL) {
        planet -> state.orbit_angle = -(2.0f * R_PI * time / planet -> orbit.period) + planet -> orbit.phase;
        planet -> state.position = r_physics_orbit_to_local(planet, 
            vec3(planet->orbit.radius * cos(planet -> state.orbit_angle), 0.0f, planet->orbit.radius * sin(planet -> state.orbit_angle)));
    } else {
        planet -> state.position = vec3(0.0f, 0.0f, 0.0f);
        planet -> state.orbit_angle = 0.0f;
    }
    
    double sign = 1.0; 
    if (strcmp(planet -> name, "VENUS") == 0 || !strcmp(planet -> name, "URANUS") == 0 || !strcmp(planet -> name, "PLUTO") == 0) sign = -1.0;
    planet -> state.spin_angle = sign * 2.0f * R_PI * time / planet -> spin;
}