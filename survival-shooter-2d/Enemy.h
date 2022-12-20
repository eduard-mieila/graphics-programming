#pragma once

#include "components/simple_scene.h"
#include "Obstacle.h"
#include "Macros.h"

namespace m1
{
    // Clasa Enemy contine date despre un inamic
    class Enemy
    {
    public:
        Enemy();
        ~Enemy();
        Enemy(bool right, bool up);

        void Move(float plX, float plY, float deltaTime);
        bool PlayerCollision(float playerX, float playerY);


        // x si y - coordonatele curente
        // h si w - dimensiunile glontului(100x100 mereu, cf. cu macrourile)
        // speed - viteza de deplasare a unui inamic
        // toBeRemoved - obiectul urmeaza sa fie eliminat la urmatoarea iteratie/urmatorul cadru randat
        float x, y;
        float h, w;
        int speed;
        bool toBeRemoved;

    protected:
        float mapW, mapH;
        
    };
    
}   // namespace m1
