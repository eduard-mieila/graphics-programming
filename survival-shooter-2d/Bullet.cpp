#include "lab_m1/tema1/Bullet.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/Transform2D.h"
#include "Macros.h"

using namespace std;
using namespace m1;

// Default constructor.
// Shouldn't be called, but there you have it anyway...
Bullet::Bullet()
{
    x = -1;
    xO = -1;
    y = -1;
    yO = -1;
    h = BULLET_HEIGHT;
    w = BULLET_WIDTH;
    rotation = 0;
    toBeRemoved = false;
}

Bullet::~Bullet() {
    
}

// Constructor util.
// Seteaza pozitia initiala si rotatia unui
// proiectil nou.
Bullet::Bullet(float x, float y, float rotation)
{
    this->x = x;
    this->xO = x;
    this->y = y;
    this->yO = y;
    h = BULLET_HEIGHT;
    w = BULLET_WIDTH;
    this->rotation = rotation;
    toBeRemoved = false;
}

// AABB Colision Detection Algortihm
// Pentru coliziuni proiectil-obstacol.
// Returneaza true pentru coliziune, false altfel
bool Bullet::ObstacleCollision(Obstacle o) {
    return (o.x < x + w &&
        o.x + o.width > x &&
        o.y < y + h &&
        o.height + o.y > y);
}

// AABB Colision Detection Algortihm
// Pentru coliziuni proiectil-inamic.
// Returneaza true pentru coliziune, false altfel.
bool Bullet::EnemyCollision(Enemy e) {
    return (e.x < x + w &&
        e.x + e.w > x &&
        e.y < y + h &&
        e.h + e.y > y);
}

// Verifica daca proiectilul a parcurs distanta maxima.
// Daca da, seteaz obiectul curent pentru stergere.
void Bullet::MaxDistanceReached() {
    if ((BULLET_MAX_DISTANCE * BULLET_MAX_DISTANCE) < ((x - xO) * (x - xO) + (y - yO) * (y - yO))) {
        toBeRemoved = true;
    }
}