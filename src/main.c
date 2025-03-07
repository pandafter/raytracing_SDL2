#include <SDL2/SDL.h>
#include <stdbool.h>
#include "render.h"
#include "physics.h"
#include "animation.h"
#include "constants.h"

int main()
{
    // Inicializar SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Error al inicializar SDL: %s\n", SDL_GetError());
        return 1;
    }

    // Crear ventana
    SDL_Window* window = SDL_CreateWindow("RayTracing", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, 0);
    if (!window) {
        printf("Error al crear la ventana: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Crear renderizador
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Error al crear el renderizador: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Definir círculos
    struct Circle circle = {200, 200, 40, 0xffffffff}; // Círculo emisor (blanco)
    struct Circle shadow_circles[MAX_CIRCLES] = {
        {800, 300, 140, 0xff0000ff}, // Círculo grande (rojo)
        {500, 200, 60, 0x00ff00ff}   // Círculo pequeño (verde)
    };
    int num_shadow_circles = 2;

    // Generar rayos
    struct Ray rays[RAYS_NUMBER];
    generate_rays(circle, rays);

    // Variables de control
    int direction = 1;
    int running = 1;
    SDL_Event event;

    // Bucle principal
    while (running)
    {
        // Manejar eventos
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                running = 0;
            }
            if (event.type == SDL_MOUSEMOTION && event.motion.state != 0)
            {
                circle.x = event.motion.x;
                circle.y = event.motion.y;
            }
        }

        // Limpiar la pantalla
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);

        // Aplicar motion blur
        ApplyMotionBlur(renderer);

        // Animar la bola estática
        ballAnimation(&shadow_circles[0], &direction);

        // Generar rayos
        generate_rays(circle, rays);

        // Dibujar rayos
        for (int i = 0; i < RAYS_NUMBER; i++)
        {
            double collision_distance = rays[i].length; // Longitud máxima por defecto

            // Verificar colisión y obtener la distancia de colisión
            check_rays_collision(&rays[i], shadow_circles, num_shadow_circles, &collision_distance);
        
            // Dibujar el rayo con la distancia de colisión y manejar rebotes
            DrawDiffusedRay(renderer, rays[i], COLOR_RAY, collision_distance, shadow_circles, num_shadow_circles);
        }

        // Dibujar los círculos con iluminación
        DrawIlluminatedCircle(renderer, circle, circle);
        for (int i = 0; i < num_shadow_circles; i++)
        {
            DrawIlluminatedCircle(renderer, shadow_circles[i], circle);
        }


        // Actualizar la pantalla
        SDL_RenderPresent(renderer);
        SDL_Delay(10);
    }

    // Liberar recursos y finalizar SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}