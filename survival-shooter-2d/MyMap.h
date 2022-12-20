#pragma once

#include "components/simple_scene.h"
#include "Macros.h"

namespace m1
{
    // Clasa MyMap contine date despre harta, alaturi de mesh-urile
    // pentru: harta, obstacole si healthBar.
    class MyMap
    {
    public:
        MyMap();
        ~MyMap();
        Mesh* getMapMesh();
        Mesh* getObsBoxMesh();
        Mesh* getWireBarMesh();
        Mesh* getBarMesh();
        const float height, width;
        const float boxDim = 100;
        
    protected:
        Mesh* map;
        Mesh* obsBox;
        Mesh* wireBar;
        Mesh* bar;

        const glm::vec3 obsBoxColor = glm::vec3(0.5f);
        const glm::vec3 mapColor = glm::vec3(1, 0.95f, 0.56f);
    };
}   // namespace m1

