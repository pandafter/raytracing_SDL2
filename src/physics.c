#include "physics.h"
#include "constants.h"  // Incluir constants.h para usar RAYS_NUMBER y LENGTH_MULT
#include <math.h>

void generate_rays(struct Circle circle, struct Ray rays[RAYS_NUMBER])
{
    double angle_increment = 2 * M_PI / RAYS_NUMBER;
    for (int i = 0; i < RAYS_NUMBER; i++)
    {
        rays[i].x_start = circle.x;
        rays[i].y_start = circle.y;
        rays[i].angle = i * angle_increment;
        rays[i].length = circle.r * LENGTH_MULT;
    }
}

bool check_rays_collision(struct Ray* ray, struct Circle circles[], int num_circles, double* collision_distance)
{
    bool collision = false;
    double min_length = ray->length;

    for (int i = 0; i < num_circles; i++)
    {
        double dx = ray->x_start - circles[i].x;
        double dy = ray->y_start - circles[i].y;

        double a = pow(cos(ray->angle), 2) + pow(sin(ray->angle), 2);
        double b = 2 * (dx * cos(ray->angle) + dy * sin(ray->angle));
        double c = pow(dx, 2) + pow(dy, 2) - pow(circles[i].r, 2);

        double discriminant = pow(b, 2) - 4 * a * c;

        if (discriminant >= 0)
        {
            double t1 = (-b - sqrt(discriminant)) / (2 * a);
            double t2 = (-b + sqrt(discriminant)) / (2 * a);

            if (t1 >= 0 && t1 < min_length)
            {
                min_length = t1;
                collision = true;
            }
            if (t2 >= 0 && t2 < min_length)
            {
                min_length = t2;
                collision = true;
            }
        }
    }

    if (collision)
    {
        *collision_distance = min_length; // Devolver la distancia de colisión
    }

    return collision;
}