#include <vector>
#include <iostream>

#include "lab_m1/tema1/Transform2D.h"
#include "Obstacle.h"

using namespace std;
using namespace m1;

Obstacle::Obstacle()
{

}

Obstacle::~Obstacle()
{

}

// Constructor util.
// Seteaza pozitia si dimensiunea noului obstacol
Obstacle::Obstacle(float x, float y, float w, float h) {
	this->x = x;
	this->y = y;
	width = w;
	height = h;
}

// Intoarce modelMatrix-ul necesar randarii obstacolului.
// *** Deoarece vom calcula factorii de scalare pe loc, ***
// ********** vom genera aici modelMatrix-ul. *************
glm::mat3 Obstacle::GetModelMatrix(glm::mat3 visMatrix) {
	glm::mat3 modelMatrix = visMatrix;

    modelMatrix *= transform2D::Translate(x, y);
    modelMatrix *= transform2D::Scale(width / OBSTACLE_DEFAULT_WIDTH, height / OBSTACLE_DEFAULT_HEIGHT);

	return modelMatrix;
}

// AABB Colision Detection Algorithm
// Coliziune intre jucator(cerc) si obstacol(patrat)
// Intoarce true daca avem coliziune, false altfel
// Credits: https://stackoverflow.com/a/10289749
bool Obstacle::PlayerCollision(float playerX, float playerY) {
	float playerR = 50;
	float testX = playerX;
    float testY = playerY;

    if (testX < x)
        testX = x;
    if (testX > (x + width))
        testX = (x + width);
    if (testY < y)
        testY = y;
    if (testY > (y + height))
        testY = (y + height);

    return ((playerX - testX) * (playerX - testX) + (playerY - testY) * (playerY - testY)) < playerR * playerR;
}
