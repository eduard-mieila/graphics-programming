#pragma once

#include <chrono>

namespace m1
{
    // Clasa Enemy retine date despre un inamic
    class Enemy3D
    {
    public:
        Enemy3D();
        ~Enemy3D();
        
        Enemy3D(float x, float z);

        // x si z - coordonatele curente
        // sinRot, cosRot - folosite pentru deplasare
        // toBeRemoved - obiectul urmeaza sa fie eliminat la urmatoarea iteratie/urmatorul cadru randat

        float x, z;
        double sinRot, cosRot;
        bool toBeRemoved;
        
    };
    
}   // namespace m1
