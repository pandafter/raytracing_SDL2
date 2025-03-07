#include "animation.h"
#include "render.h"

#define BALL_SPEED 2

void ballAnimation(struct Circle* shadow_circle, int* direction)
{
    shadow_circle->y += *direction * BALL_SPEED;

    if (shadow_circle->y - shadow_circle->r <= 0 || shadow_circle->y + shadow_circle->r >= HEIGHT)
    {
        *direction *= -1;
    }
}