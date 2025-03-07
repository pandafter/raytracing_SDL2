#ifndef PHYSICS_H
#define PHYSICS_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "constants.h"  // Incluir constants.h para usar RAYS_NUMBER

// Estructuras
struct Circle
{
    double x;
    double y;
    double r;
    Uint32 base_color;
};

struct Ray
{
    double x_start, y_start;
    double angle;
    double length;
};

// Funciones de física
void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER]);
bool check_rays_collision(struct Ray* ray, struct Circle circles[], int num_circles, double* collision_distance);

#endif // PHYSICS_H