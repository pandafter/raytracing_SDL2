#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "physics.h"
#include "animation.h"

// Constantes
#define WIDTH 1200                  // Ancho de la ventana
#define HEIGHT 600                  // Alto de la ventana
#define COLOR_WHITE 0xffffffff      // Color blanco en formato ARGB
#define COLOR_BLACK 0x00000000      // Color negro en formato ARGB
#define COLOR_RAY 0x80ffff00        // Color del rayo (amarillo difuso)
#define RAY_DIFFUSION_RADIUS 5      // Radio de difusión del rayo


// Funciones de renderizado
void FillCircle(SDL_Renderer* renderer, struct Circle circle, Uint32 color);


//Dibuja un rayo difuminado en el renderizador.
void DrawDiffusedRay(SDL_Renderer* renderer, struct Ray ray, Uint32 color, double collision_distance, struct Circle circles[], int num_circles);


//Reflejo rayo
void reflect_ray(struct Ray* ray, struct Circle circles[], int num_circles, double collision_distance);

//Aplica un efecto de motion blur al renderizador.
void ApplyMotionBlur(SDL_Renderer* renderer);


//Dibuja un círculo iluminado por una fuente de luz.

void DrawIlluminatedCircle(SDL_Renderer* renderer, struct Circle circle, struct Circle light_source);

// Función de colisión
bool check_rays_collision(struct Ray* ray, struct Circle circles[], int num_circles, double* collision_distance);

#endif // RENDER_H