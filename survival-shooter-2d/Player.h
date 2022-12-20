#pragma once

#include "components/simple_scene.h"
#include "Obstacle.h"
#include "Macros.h"

namespace m1
{   // Clasa Player contine data despre jucator, dar si mesh-urile
    // pentru: corpul si ochii jucatorului, proiectile si inamici.
    class Player
    {
     public:
        Player();
        ~Player();
        Mesh* getBodyMesh();
        Mesh* getEyeMesh();
        Mesh* getBulletMesh();
        Mesh* getEnemyMesh();
        
        
        const float eyesOffsetX, eyesOffsetY;
        const float speed, radius;
        int hp;
        int score;

     protected:
         Mesh* body;
         Mesh* eye;
         Mesh* bullet;
         Mesh* enemy;
         float playerX, playerY;
         float camX, camY;
         
    };
}   // namespace m1
