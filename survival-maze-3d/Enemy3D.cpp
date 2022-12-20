#include "lab_m1/tema2/Enemy3D.h"

#include <vector>
#include <iostream>

using namespace std;
using namespace m1;

// Default constructor.
// Shouldn't be called, but there you have it anyway...
Enemy3D::Enemy3D()
{
    x = -1;
    z = -1;
    sinRot = 0;
    cosRot = 0;
    toBeRemoved = false;
}

Enemy3D::~Enemy3D() {
    
}

// Constructor util.
// Seteaza pozitia initiala si rotatia unui
// inamic nou.
Enemy3D::Enemy3D(float x, float z)
{
    this->x = x;
    this->z = z;
    sinRot = 0;
    cosRot = 0;
    toBeRemoved = false;
}
