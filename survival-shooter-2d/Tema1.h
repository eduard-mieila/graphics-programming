#pragma once

#include <vector>
#include "components/simple_scene.h"
#include "Player.h"
#include "MyMap.h"
#include "Obstacle.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Macros.h"

namespace m1
{
    class Tema1 : public gfxc::SimpleScene
    {
     public:
        Tema1();
        ~Tema1();

        void Init() override;

        // Credits: EGC, Lab3
        struct LogicSpace
        {
            LogicSpace() : x(0), y(0), width(1), height(1) {}
            LogicSpace(float x, float y, float width, float height)
                : x(x), y(y), width(width), height(height) {}
            float x;
            float y;
            float width;
            float height;
        };

        // Credits: EGC, Lab3
        struct ViewSpace
        {
            ViewSpace() : x(0), y(0), width(1), height(1) {}
            ViewSpace(int x, int y, int width, int height)
                : x(x), y(y), width(width), height(height) {}
            int x;
            int y;
            int width;
            int height;
        };


     private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        glm::mat3 VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewSpace& viewSpace);
        void SetViewportArea(const ViewSpace& viewSpace, glm::vec3 colorColor, bool clear);

        void renderBody(glm::mat3 visMat);
        void renderEyes(glm::mat3 visMat);
        void renderBullets(glm::mat3 visMat, float deltaTime);
        void renderEnemies(glm::mat3 visMat, float deltaTime);
        void renderHealthBar(glm::mat3 visMat);
        
        glm::vec2 Logic2View(glm::vec2 logicCor);

     protected:
         Player *player;
         MyMap* map;
         LogicSpace logicSpace;
         ViewSpace viewSpace;

         float oldPlayerX, oldPlayerY;
         float playerX, playerY;
         float crsX, crsY;
         float dX, dY, tetha;
         gfxc::Camera *camera;

         bool gameEnded;

         std::vector<Obstacle> obstacles;
         std::vector<Bullet> bullets;
         std::vector<Enemy> enemies;
         std::clock_t startupTime;
         std::clock_t lastShootTime;
         std::clock_t lastEnemySpawnTime;
         std::clock_t lastEnemyCollisionCheckTime;
    };
}   // namespace m1
