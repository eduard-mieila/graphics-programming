#pragma once

#include "components/simple_scene.h"
#include "Macros.h"

namespace m1
{
    // Clasa Obstacle contine date despre un obstacol
    class Obstacle
    {
    public:
        Obstacle();
        ~Obstacle();
        Obstacle(float x, float y, float w, float h);
        
        glm::mat3 GetModelMatrix(glm::mat3 visMatrix);
        bool PlayerCollision(float playerX, float playerY);
        
        // x si y - coordonatele curente
        // h si w - dimensiunile obstacolului
        float x, y, width, height;
        
    };
}   // namespace m1

