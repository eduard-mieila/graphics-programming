#include <vector>
#include <iostream>

#include "lab_m1/tema1/Transform2D.h"
#include "MyMap.h"

using namespace std;
using namespace m1;

MyMap::MyMap() : 
    height(MAP_HEIGHT),
    width(MAP_WIDTH)
{

    // Create obstacle box mesh
    {
        vector<VertexFormat> verticesBox
        {
           VertexFormat(glm::vec3(0, 0, 0), obsBoxColor),
           VertexFormat(glm::vec3(OBSTACLE_DEFAULT_WIDTH, 0, 0), obsBoxColor),
           VertexFormat(glm::vec3(OBSTACLE_DEFAULT_WIDTH, OBSTACLE_DEFAULT_HEIGHT, 0), obsBoxColor),
           VertexFormat(glm::vec3(0, OBSTACLE_DEFAULT_HEIGHT, 0), obsBoxColor),
        };

        vector<unsigned int> indicesBox
        {
            0, 2, 1,
            2, 3, 0,
        };

        obsBox = new Mesh("obstacle box");
        obsBox->InitFromData(verticesBox, indicesBox);
        obsBox->SetDrawMode(GL_TRIANGLES);
    }

    // Create map box mesh
    {
        vector<VertexFormat> vertices
        {
           VertexFormat(glm::vec3(0, 0, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(0, height, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(width, height, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(width, 0, 0), glm::vec3(1, 1, 1)),
        };

        vector<unsigned int> indices
        {
            0, 2, 1,
            2, 3, 0,
        };

        map = new Mesh("square map");
        map->InitFromData(vertices, indices);
        map->SetDrawMode(GL_TRIANGLES);
    }

    // Create wireframe HealthBar mesh
    {
        vector<VertexFormat> verticesWireBar
        {
           VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(0, 25, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(150, 25, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(150, 0, 0), glm::vec3(0, 1, 0)),
        };

        vector<unsigned int> indicesWireBar
        {
            0, 1,
            1, 2,
            2, 3,
            3, 0,
        };

        wireBar = new Mesh("wireBar");
        wireBar->InitFromData(verticesWireBar, indicesWireBar);
        wireBar->SetDrawMode(GL_LINES);
    }

    // Create HealthBar mesh
    {
        vector<VertexFormat> verticesBar
        {
           VertexFormat(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(0, 25, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(150, 25, 0), glm::vec3(0, 1, 0)),
           VertexFormat(glm::vec3(150, 0, 0), glm::vec3(0, 1, 0)),
        };

        vector<unsigned int> indicesBar
        {
            0, 1, 2, 3,
        };

        bar = new Mesh("wireBar");
        bar->InitFromData(verticesBar, indicesBar);
        bar->SetDrawMode(GL_TRIANGLE_FAN);
    }
}

MyMap::~MyMap()
{
    delete map;
    delete obsBox;
    delete bar;
    delete wireBar;
}

Mesh* MyMap::getMapMesh()
{
    return map;
}

Mesh* MyMap::getObsBoxMesh()
{
    return obsBox;
}

Mesh* MyMap::getWireBarMesh()
{
    return wireBar;
}

Mesh* MyMap::getBarMesh()
{
    return bar;
}

