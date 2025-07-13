#include "r_physics.h"

void r_physics_state_update(planet_t *planet, double time) {
    if (planet -> parent != NULL) {
        planet -> state.orbit_angle = (2.0f * R_PI * time / planet -> orbit_period) + planet -> orbit_phase;
        planet -> state.position.x = planet -> orbit_radius * cos(planet -> state.orbit_angle);
        planet -> state.position.z = planet -> orbit_radius * sin(planet -> state.orbit_angle);
        // printf("%s=(%f, %f, %f)\n", planet -> name, planet -> state.position.x, planet -> state.position.y, planet -> state.position.z);
    }
    planet -> state.spin_angle = 2.0f * R_PI * time / planet -> spin;
}