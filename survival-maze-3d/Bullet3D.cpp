#include "lab_m1/tema2/Bullet3D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

// Default constructor.
// Shouldn't be called, but there you have it anyway...
Bullet3D::Bullet3D()
{
    x = -1;
    z = -1;
    rotation = 3.14f;
    spawnTime = clock();
    toBeRemoved = false;
}

Bullet3D::~Bullet3D() {
    
}

// Constructor util.
// Seteaza pozitia initiala si rotatia unui
// proiectil nou.
Bullet3D::Bullet3D(float x, float z, float rotation)
{
    this->x = x;
    this->z = z;
    this->rotation = rotation + 3.14f;
    spawnTime = clock();
    toBeRemoved = false;
}
