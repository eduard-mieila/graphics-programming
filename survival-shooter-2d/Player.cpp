#include "lab_m1/tema1/Player.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/Transform2D.h"

using namespace std;
using namespace m1;


Player::Player() :
    eyesOffsetX(PLAYER_EYES_OFFSET_X),
    eyesOffsetY(PLAYER_EYES_OFFSET_Y),
    speed(PLAYER_SPEED),
    radius(PLAYER_BODY_RADIUS)
{

    // Create head mesh
    {
        vector<VertexFormat> vertices;
        vector<unsigned int> indices;

        glm::vec3 color = glm::vec3(0.25f, 0, 1);
        vertices.emplace_back(VertexFormat(glm::vec3(0, 0, 0), color));
        for (int i = 1; i <= 360; i++) {
            float rad = i * PI / 180;
            vertices.emplace_back(glm::vec3(cos(rad) * radius, sin(rad) * radius, 0), color);
            indices.push_back(i);
        }

        body = new Mesh("circle");
        body->InitFromData(vertices, indices);
        body->SetDrawMode(GL_TRIANGLE_FAN);
    }

    // Create eye mesh
    {
        vector<VertexFormat> vertices
        {
           VertexFormat(glm::vec3(10, 15, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(40, 15, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(25, 35, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(60, 15, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(90, 15, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(75, 35, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(35, 50, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(65, 50, 0), glm::vec3(1, 1, 1)),
           VertexFormat(glm::vec3(50, 70, 0), glm::vec3(1, 1, 1)),
        };

        vector<unsigned int> indices
        { 
            0, 2, 1,
            3, 5, 4,
            6, 8, 7,
        };

        eye = new Mesh("triangle eye");
        eye->InitFromData(vertices, indices);
        eye->SetDrawMode(GL_TRIANGLES);
    }

    // Create bullet mesh
    {
        vector<VertexFormat> verticesBullet
        {
           VertexFormat(glm::vec3(-10, 0, 0), glm::vec3(0)),
           VertexFormat(glm::vec3(-10, 30, 0), glm::vec3(0)),
           VertexFormat(glm::vec3(10, 30, 0), glm::vec3(0)),
           VertexFormat(glm::vec3(10, 0, 0), glm::vec3(0)),

        };

        vector<unsigned int> indicesBullet
        {
            0, 2, 1,
            0, 3, 2,
        };

        bullet = new Mesh("circle bullet");
        bullet->InitFromData(verticesBullet, indicesBullet);
        bullet->SetDrawMode(GL_TRIANGLE_FAN);
    }

    // Create enemy mesh
    {
        glm::vec3 enemyCol1 = glm::vec3(1.0f, 0.01f, 0.35f);
        glm::vec3 enemyCol2 = glm::vec3(1.0f, 0.85f, 0.01f);
        vector<VertexFormat> verticesEnemy
        {
            VertexFormat(glm::vec3(25, 50, 0), enemyCol2),
            VertexFormat(glm::vec3(50, 75, 0), enemyCol2),
            VertexFormat(glm::vec3(75, 50, 0), enemyCol2),
            VertexFormat(glm::vec3(50, 25, 0), enemyCol2),


            VertexFormat(glm::vec3(0, 0, 0), enemyCol1),
            VertexFormat(glm::vec3(0, 100, 0), enemyCol1),
            VertexFormat(glm::vec3(100, 100, 0), enemyCol1),
            VertexFormat(glm::vec3(100, 0, 0), enemyCol1),

        };

        vector<unsigned int> indicesEnemy
        {
            0, 2, 1,
            0, 3, 2,
            4, 6, 5,
            4, 7, 6,
        };

        enemy = new Mesh("enemy sq");
        enemy->InitFromData(verticesEnemy, indicesEnemy);
        enemy->SetDrawMode(GL_TRIANGLES);
    }

    // Initializeaza HP-ul si scor-ul jucatorului
    hp = 100;
    score = 0;
}


Player::~Player()
{
    delete body;
    delete eye;
    delete bullet;
    delete enemy;
}

Mesh* Player::getBodyMesh() {
    return body;
}

Mesh* Player::getEyeMesh() {
    return eye;
}

Mesh* Player::getBulletMesh() {
    return bullet;
}

Mesh* Player::getEnemyMesh() {
    return enemy;
}
