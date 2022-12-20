#pragma once

#include <chrono>

namespace m1
{
    // Clasa Bullet retine date despre un proiectil
    class Bullet3D
    {
    public:
        Bullet3D();
        ~Bullet3D();
        
        Bullet3D(float x, float z, float rotation);

        // x si z - coordonatele curente
        // spawnTime - timpul la care a fost lansat proiectilul
        // rotation - directia in care glontul trebuie sa mearga
        // toBeRemoved - obiectul urmeaza sa fie eliminat la urmatoarea iteratie/urmatorul cadru randat

        float x, z;
        std::clock_t spawnTime;
        float rotation;
        bool toBeRemoved;
        
    };
    
}   // namespace m1
