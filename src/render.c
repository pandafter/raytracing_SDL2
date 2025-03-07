#include "render.h"


void FillCircle(SDL_Renderer* renderer, struct Circle circle, Uint32 color)
{
    double r_sqr = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_sqr = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_sqr <= r_sqr)
            {
                SDL_SetRenderDrawColor(renderer, (color >> 16) & 0xFF, (color >> 8) & 0xFF, color & 0xFF, (color >> 24) & 0xFF);
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
        }
    }
}

void reflect_ray(struct Ray* ray, struct Circle circles[], int num_circles, double collision_distance)
{
    // Buscar el círculo con el que colisionó el rayo
    for (int i = 0; i < num_circles; i++)
    {
        double dx = ray->x_start - circles[i].x;
        double dy = ray->y_start - circles[i].y;
        double distance = sqrt(dx * dx + dy * dy);

        // Verificar si el rayo colisionó con este círculo
        if (distance <= circles[i].r + collision_distance)
        {
            // Calcular el punto de colisión
            double collision_x = ray->x_start + cos(ray->angle) * collision_distance;
            double collision_y = ray->y_start + sin(ray->angle) * collision_distance;

            // Calcular la normal en el punto de colisión
            double normal_x = (collision_x - circles[i].x) / circles[i].r;
            double normal_y = (collision_y - circles[i].y) / circles[i].r;

            // Calcular el ángulo de incidencia (dirección del rayo incidente)
            double incident_angle = ray->angle;

            // Calcular el ángulo de reflexión usando la ley de reflexión
            double dot_product = cos(incident_angle) * normal_x + sin(incident_angle) * normal_y;
            double reflected_angle = incident_angle - 2 * dot_product * atan2(normal_y, normal_x);

            // Actualizar el ángulo del rayo para que "rebote" en la dirección correcta
            ray->angle = reflected_angle;

            // Ajustar el punto de inicio del rayo reflejado
            ray->x_start = collision_x;
            ray->y_start = collision_y;

            // Reducir la longitud del rayo reflejado (por ejemplo, a la mitad)
            ray->length *= 0.1; // Puedes ajustar este factor según lo que necesites

            break; // Salir del bucle después de reflejar el rayo
        }
    }
}


void DrawDiffusedRay(SDL_Renderer* renderer, struct Ray ray, Uint32 color, double collision_distance, struct Circle circles[], int num_circles)
{
    double step = 0.2; // Distancia entre puntos a lo largo del rayo
    int max_bounces = 1; // Número máximo de rebotes (puedes ajustarlo)

    for (int bounce = 0; bounce < max_bounces; bounce++)
    {
        // Dibujar el rayo desde el inicio hasta el punto de colisión
        for (double t = 0; t <= collision_distance; t += step)
        {
            double x = ray.x_start + cos(ray.angle) * t;
            double y = ray.y_start + sin(ray.angle) * t;

            // Calcular la intensidad del color en este punto
            double point_intensity = 1.0 - (t / ray.length); // Usar la longitud global del rayo

            // Ajustar el color en función de la intensidad
            Uint8 r = (Uint8)(((color >> 16) & 0xFF) * point_intensity);
            Uint8 g = (Uint8)(((color >> 8) & 0xFF) * point_intensity);
            Uint8 b = (Uint8)((color & 0xFF) * point_intensity);

            // Aplicar transparencia (alpha) para simular el efecto foggy
            Uint8 alpha = (Uint8)(255 * point_intensity); // Alpha disminuye con la distancia

            // Establecer el color con transparencia
            SDL_SetRenderDrawColor(renderer, r, g, b, alpha);
            SDL_RenderDrawPoint(renderer, (int)x, (int)y);
        }

        // Verificar si hay una colisión y reflejar el rayo
        bool collision = check_rays_collision(&ray, circles, num_circles, &collision_distance);
        if (collision)
        {
            // Reflejar el rayo en el círculo colisionado
            reflect_ray(&ray, circles, num_circles, collision_distance);
            ray.length -= collision_distance; // Ajustar la longitud restante del rayo
        }
        else
        {
            break; // No hay más colisiones, salir del bucle
        }
    }
}

void ApplyMotionBlur(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0x10, 0x00, 0x00, 0x10);
    SDL_RenderClear(renderer);
}

void DrawIlluminatedCircle(SDL_Renderer* renderer, struct Circle circle, struct Circle light_source)
{
    double r_sqr = pow(circle.r, 2);
    for (double x = circle.x - circle.r; x <= circle.x + circle.r; x++)
    {
        for (double y = circle.y - circle.r; y <= circle.y + circle.r; y++)
        {
            double distance_sqr = pow(x - circle.x, 2) + pow(y - circle.y, 2);
            if (distance_sqr <= r_sqr)
            {
                // Calcular la distancia a la fuente de luz
                double light_distance = sqrt(pow(x - light_source.x, 2) + pow(y - light_source.y, 2));

                // Calcular la intensidad de la luz en función de la distancia
                double light_intensity = 1.0 - (light_distance / (circle.r * 2)) * 2;
                light_intensity = fmax(0, fmin(1, light_intensity)); // Asegurar que esté en el rango [0, 1]

                // Obtener los componentes del color base del círculo
                Uint8 r = (circle.base_color >> 16) & 0xFF;
                Uint8 g = (circle.base_color >> 8) & 0xFF;
                Uint8 b = circle.base_color & 0xFF;
                Uint8 a = (circle.base_color >> 24) & 0xFF;

                // Mezclar el color base con el color de la luz (COLOR_RAY)
                r = (Uint8)(r * (1 - light_intensity) + ((COLOR_RAY >> 16) & 0xFF) * light_intensity);
                g = (Uint8)(g * (1 - light_intensity) + ((COLOR_RAY >> 8) & 0xFF) * light_intensity);
                b = (Uint8)(b * (1 - light_intensity) + (COLOR_RAY & 0xFF) * light_intensity);

                // Crear el color iluminado
                SDL_SetRenderDrawColor(renderer, r, g, b, a);
                SDL_RenderDrawPoint(renderer, (int)x, (int)y);
            }
        }
    }
}