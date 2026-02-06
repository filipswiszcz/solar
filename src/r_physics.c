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

struct tm r_physics_clock_to_tm(double time) {
    struct tm tm_origin = {0};
    tm_origin.tm_year = 2000 - 1900;
    tm_origin.tm_mon = 0;
    tm_origin.tm_mday = 1;
    tm_origin.tm_hour = 0;
    tm_origin.tm_min = 0;
    tm_origin.tm_sec = 0;
    tm_origin.tm_isdst = -1;

    time_t time_origin = mktime(&tm_origin);
    time_t time_current = time_origin + (time_t)time;

    struct tm tm_current;
#if defined(_WIN32) || defined(_WIN64)
    localtime_s(&tm_current, &time_current);
#else
    localtime_r(&time_current, &tm_current);
#endif
    return tm_current;
}