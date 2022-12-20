#include "lab_m1/tema1/Enemy.h"

#include <vector>
#include <iostream>

#include "lab_m1/tema1/Transform2D.h"

using namespace std;
using namespace m1;

// Default constructor.
// Shouldn't be called, but there you have it anyway...
Enemy::Enemy()
{
    x = -1;
    y = -1;
    h = 100;
    w = 100;
    toBeRemoved = false;
}

Enemy::~Enemy() {
    
}

// Constructor util.
// Un inamic poate fi spawnat intr-una dintre cele 4 regiuni ale ecranului,
// stanga/dreapta - sus/jos. Acest detaliu se seteaza prin setarea parametrilor
// right si up pe true sau false. Pozitia si viteza inamicului sunt generate
// aleator.
Enemy::Enemy(bool right, bool up)
{
    static bool first = true;
    if (first) {
        srand((int)time(0));
        first = false;
    }

    float x0, x1, y0, y1;

    // Identifica range-ul in care poate fi spawnat inamicul curent
    if (!right) {
        x0 = 0;
        x1 = MAP_WIDTH / 2;
    }
    else {
        x0 = MAP_WIDTH / 2;
        x1 = MAP_WIDTH;
    }

    if (!up) {
        y0 = 0;
        y1 = MAP_HEIGHT / 2;
    }
    else {
        y0 = MAP_HEIGHT / 2;
        y1 = MAP_HEIGHT;
    }

    // Generaza coordonatele in range-ul dat
    x = x0 + rand() % (int)((x1 + 1) - x0);
    y = y0 + rand() % (int)((y1 + 1) - y0);
    cout << "EnemySpawned @ " << x << ", " << y << endl;
    // Asigura-te ca nu se spawneaza in afara hartii(extra-safety)
    if (x > MAP_WIDTH - w) {
        x = MAP_WIDTH - w;
    }

    if (y > MAP_HEIGHT - h) {
        y = MAP_HEIGHT - h;
    }

    if (x < 0) {
        x = 0;
    }

    if (y < 0) {
        y = 0;
    }

    // Seteaza dimensiunea inamicului
    h = ENEMY_HEIGHT;
    w = ENEMY_WIDTH;
    toBeRemoved = false;
    // Viteza aleatorie, cf. cu cerinta
    speed = 50 + rand() % 100;
}

// Actualizeaza pozitia unui inamic. Inamicii se vor deplasa
// mereu catre jucator, asadar, avem nevoie sa cunoastem pozitia pe harta
// a jucatorului.
void Enemy::Move(float plX, float plY, float deltaTime) {
    
    // Calculam unghiul dintre jucator si inamic pentru a-i determina inamicului
    // directia de deplasare
    float dX = plX - x;
    float dY = plY - y;
    float tetha = atan2(dY, dX);

    // Actualizeaza pozitia inamicului
    x += cos(tetha) * speed * deltaTime;
    y += sin(tetha) * speed * deltaTime;

    // Coliziune inamic - perete harta (2.5p)
    if (x > MAP_WIDTH - w) {
        x = MAP_WIDTH - w;
    }

    if (y > MAP_HEIGHT - h) {
        y = MAP_HEIGHT - h;
    }

    if (x < 0) {
        x = 0;
    }

    if (y < 0) {
        y = 0;
    }

}
// AABB Colision Detection Algorithm
// Coliziune intre jucator(cerc) si inamic(patrat)
// Intoarce true daca avem coliziune, false altfel
// Credits: https://stackoverflow.com/a/10289749
bool Enemy::PlayerCollision(float playerX, float playerY) {
    float playerR = 50;
    float testX = playerX;
    float testY = playerY;

    if (testX < x)
        testX = x;
    if (testX > (x + w))
        testX = (x + w);
    if (testY < y)
        testY = y;
    if (testY > (y + h))
        testY = (y + h);

    return ((playerX - testX) * (playerX - testX) + (playerY - testY) * (playerY - testY)) < playerR * playerR;
}