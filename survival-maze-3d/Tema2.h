#pragma once

#include <vector>
#include <string>
#include <iostream>

#include "components/simple_scene.h"
#include "lab_m1/Tema2/my_camera.h"
#include "lab_m1/tema2/Bullet3D.h"
#include "lab_m1/tema2/Enemy3D.h"

#define LABYRINTH_DIM 10

#define BODY_DIM_X 0.09f
#define BODY_DIM_Y 0.165f
#define BODY_DIM_Z 0.06f

#define HEAD_DIM_XYZ 0.06f
#define HEAD_OFFSET_Y 0.25f

#define ARM_DIM_X 0.05f
#define ARM_DIM_Y 0.1f
#define ARM_DIM_Z 0.06f
#define ARM_OFFSET_X 0.16f
#define ARM_OFFSET_Y 0.06f

#define BULLET_TTL 1
#define BULLET_FIRE_RATE 0.5f
#define BULLET_LEVEL 0.2f
#define SPHERE_RADIUS 0.1f

#define ENEMY_DIM_X 0.25f
#define ENEMY_DIM_Y 0.6f
#define ENEMY_DIM_Z 0.25f
#define ENEMY_Y_LEVEL 0.4f
#define ENEMY_DAMAGE 10

#define WALL_DIM 0.5f

#define PLAYER_HITBOX_DIM 0.25f

#define TIME_LIMIT 30


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
     public:
        Tema2();
        ~Tema2();

        void Init() override;

        Mesh *CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices);

     protected:
         implemented::CameraTema2 *camera;

     private:
        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix);
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        bool AABBPlayerWall();
        bool AABBPlayerEnemy();
        bool AABBBulletWall(Bullet3D b);
        bool AABBBulletEnemy(Bullet3D b);

        void RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        
        std::vector<Bullet3D> bullets;
        std::vector<Enemy3D> enemies;
        std::clock_t lastShootTime;
        std::clock_t startUpTime;

        int playerHP;

        float PlayerX, PlayerY, PlayerZ;
        float TargetX, TargetZ;
        float playerRotation;
        glm::mat4 projectionMatrix;
        bool attackMode;
        bool gameOver;
        bool tenSecWarning;
        char map[10][10];

        // Se alege una dintre cele 3 configuratii de labirint de mai jos
        // 0 - Casuta libera
        // 1 - obstacol
        // 2 - inamic
        // 3 - jucator(unused)
        // 4 - casuta iesire
        char map1[10][10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             4, 0, 0, 0, 0, 1, 0, 1, 0, 1,
                             1, 1, 1, 2, 1, 1, 0, 1, 0, 1,
                             1, 0, 1, 0, 0, 0, 0, 0, 0, 1,
                             1, 0, 1, 1, 0, 1, 1, 0, 1, 1,
                             1, 0, 0, 0, 2, 0, 0, 0, 0, 1,
                             1, 1, 1, 0, 1, 1, 1, 2, 0, 1,
                             1, 0, 1, 0, 1, 0, 0, 1, 0, 1,
                             1, 0, 2, 0, 2, 0, 0, 1, 0, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

        char map2[10][10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             4, 0, 1, 1, 0, 0, 2, 0, 0, 1,
                             1, 0, 2, 0, 0, 1, 1, 1, 0, 1,
                             1, 0, 1, 1, 0, 0, 1, 1, 0, 1,
                             1, 0, 0, 1, 1, 2, 0, 0, 0, 1,
                             1, 1, 1, 1, 1, 0, 1, 1, 1, 1,
                             1, 0, 0, 2, 0, 0, 0, 0, 0, 1,
                             1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
                             1, 0, 2, 0, 1, 0, 0, 1, 0, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };

        char map3[10][10] = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
                             4, 0, 0, 2, 1, 0, 0, 0, 0, 1,
                             1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
                             1, 0, 1, 0, 0, 0, 2, 0, 0, 1,
                             1, 2, 1, 2, 1, 1, 1, 1, 0, 1,
                             1, 0, 1, 0, 0, 1, 0, 1, 0, 1,
                             1, 0, 2, 0, 0, 1, 0, 1, 1, 1,
                             1, 0, 1, 1, 0, 0, 0, 0, 0, 1,
                             1, 0, 1, 0, 0, 0, 1, 1, 0, 1,
                             1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
    };
}   // namespace m1
