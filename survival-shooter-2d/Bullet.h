#pragma once

#include "components/simple_scene.h"
#include "Obstacle.h"
#include "Enemy.h"
#include "Macros.h"

namespace m1
{
    // Clasa Bullet retine date despre un proiectil
    class Bullet
    {
    public:
        Bullet();
        ~Bullet();
        
        Bullet(float x, float y, float rotation);
        bool ObstacleCollision(Obstacle o);
        bool EnemyCollision(Enemy e);
        void MaxDistanceReached();

        // x si y - coordonatele curente
        // x0 si y0 - coordonatele punctului de plecare
        // h si w - dimensiunile glontului(20x30 mereu, cf. cu macrourile)
        // rotation - directia in care glontul trebuie sa mearga
        // toBeRemoved - obiectul urmeaza sa fie eliminat la urmatoarea iteratie/urmatorul cadru randat

        float x, xO;
        float y, yO;
        float h;
        float w;
        float rotation;
        bool toBeRemoved;
        
    };
    
}   // namespace m1
