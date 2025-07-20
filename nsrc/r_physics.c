#include "r_physics.h"

// void r_physics_state_update(planet_t *planet, double time) {
//     if (planet -> parent != NULL) {
//         planet -> state.orbit_angle = -(2.0f * R_PI * time / planet -> orbit.period) + planet -> orbit.phase;
//         planet -> state.position.x = planet -> orbit.radius * cos(planet -> state.orbit_angle);
//         planet -> state.position.z = planet -> orbit.radius * sin(planet -> state.orbit_angle);
//         // printf("%s=(%f, %f, %f)\n", planet -> name, planet -> state.position.x, planet -> state.position.y, planet -> state.position.z);
//     }
//     planet -> state.spin_angle = 2.0f * R_PI * time / planet -> spin;
// }

void r_physics_state_update(planet_t *planet, double time) {
    if (planet -> parent != NULL) {
        planet -> state.orbit_angle = -(2.0f * R_PI * time / planet -> orbit.period) + planet -> orbit.phase;

        // position in orbital plane (local space)
        vec3_t local = {
            planet->orbit.radius * cos(planet->state.orbit_angle),
            0.0f,
            planet->orbit.radius * sin(planet->state.orbit_angle)
        };

        // convert to world space
        planet->state.position = orbit_local_to_world(local, planet);
        
        // vec3_t position = {
        //     planet -> orbit.radius * cos(planet -> state.orbit_angle),
        //     0.0f,
        //     planet -> orbit.radius * sin(planet -> state.orbit_angle)
        // };

        // float cy = cos(planet -> inclination);
        // float sy = sin(planet -> inclination);

        // float iy = position.y * cy - position.z * sy;
        // float iz = position.y * sy + position.z * cy;

        // position.y = iy;
        // position.z = iz;

        // float cx = cos(planet -> node);
        // float sx = sin(planet -> node);

        // float nx = position.x * cx + position.z * sx;
        // float nz = -position.x * sx + position.z * cx;

        // position.x = nx;
        // position.z = nz;

        // planet -> state.position = vec3_add(position, planet -> parent -> state.position);
    } else {
        planet -> state.position = vec3(0.0f, 0.0f, 0.0f);
        planet -> state.orbit_angle = 0.0f;
    }
    
    double sign = -1.0; 
    if (planet -> name == "VENUS" || planet -> name == "URANUS" || planet -> name == "PLUTO") sign = 1.0;
    planet -> state.spin_angle = sign * 2.0f * R_PI * time / planet -> spin;
}