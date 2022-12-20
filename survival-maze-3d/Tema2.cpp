#include "lab_m1/tema2/Tema2.h"

using namespace std;
using namespace m1;

/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
    
}


void Tema2::Init()
{
    // Alegere configuratie labirint
    srand((unsigned)time(NULL));
    int n = rand() % 3;
    cout << "Lab config = " << n << endl;
    if (n == 0) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                map[i][j] = map1[i][j];
            }
        }
    } else if (n == 1) {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                map[i][j] = map2[i][j];
            }
        }
    } else {
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                map[i][j] = map3[i][j];
            }
        }
    }

    // Determinare pozitie start jucator
    // Creare inamici
    int x = 0, z = 0;
    while (map[x][z] != 0) {
        x = rand() % LABYRINTH_DIM;
        z = rand() % LABYRINTH_DIM;
    }
    PlayerX = x;
    PlayerY = 0.2f;
    PlayerZ = z;

    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {

            if (map[i][j] == 2) {
                enemies.push_back(Enemy3D(i, j));
            }

            if (map[i][j] == 4) {
                TargetX = i;
                TargetZ = j;
            }
        }
    }

    // Initializare variabile utile
    playerHP = 100;
    playerRotation = 1.67f;
    attackMode = false;
    gameOver = false;
    tenSecWarning = false;
    lastShootTime = clock();
    startUpTime = clock();

    // Setare initiala camera
    camera = new implemented::CameraTema2();
    camera->Set(glm::vec3(PlayerX + 0.2f * sin(playerRotation), PlayerY + 0.35f, PlayerZ + 0.2f * cos(playerRotation)), glm::vec3(PlayerX, PlayerY + 0.3f, PlayerZ), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    
    // Mesh bullet
    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }


    // Mesh player
    {
        glm::vec3 headColor = glm::vec3(0.9f, 1.0f, 0.42f);
        glm::vec3 bodyColor = glm::vec3(0.f, 1.0f, 0.9f);
        glm::vec3 armColor = glm::vec3(0.f, 0.f, 1.f);
        vector<VertexFormat> vertices
        {
            // Body Vertices(0 - 7)
            VertexFormat(glm::vec3(-BODY_DIM_X, -BODY_DIM_Y, BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(BODY_DIM_X, -BODY_DIM_Y,  BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(-BODY_DIM_X,  BODY_DIM_Y,  BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(BODY_DIM_X,  BODY_DIM_Y,  BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(-BODY_DIM_X, -BODY_DIM_Y, -BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(BODY_DIM_X, -BODY_DIM_Y, -BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(-BODY_DIM_X,  BODY_DIM_Y, -BODY_DIM_Z), bodyColor),
            VertexFormat(glm::vec3(BODY_DIM_X,  BODY_DIM_Y, -BODY_DIM_Z), bodyColor),

            // Head Vertices(8 - 15)
            VertexFormat(glm::vec3(-HEAD_DIM_XYZ, -HEAD_DIM_XYZ + HEAD_OFFSET_Y, HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(HEAD_DIM_XYZ, -HEAD_DIM_XYZ + HEAD_OFFSET_Y,  HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(-HEAD_DIM_XYZ,  HEAD_DIM_XYZ + HEAD_OFFSET_Y,  HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(HEAD_DIM_XYZ,  HEAD_DIM_XYZ + HEAD_OFFSET_Y,  HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(-HEAD_DIM_XYZ, -HEAD_DIM_XYZ + HEAD_OFFSET_Y, -HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(HEAD_DIM_XYZ, -HEAD_DIM_XYZ + HEAD_OFFSET_Y, -HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(-HEAD_DIM_XYZ,  HEAD_DIM_XYZ + HEAD_OFFSET_Y, -HEAD_DIM_XYZ), headColor),
            VertexFormat(glm::vec3(HEAD_DIM_XYZ,  HEAD_DIM_XYZ + HEAD_OFFSET_Y, -HEAD_DIM_XYZ), headColor),

            // Left Arm Vertices(16 - 23)
            VertexFormat(glm::vec3(-ARM_DIM_X + ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X + ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X + ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X + ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X + ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X + ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X + ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X + ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),

            // Right Arm Vertices(24 - 31)
            VertexFormat(glm::vec3(-ARM_DIM_X - ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X - ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X - ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X - ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y,  ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X - ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X - ARM_OFFSET_X, -ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(-ARM_DIM_X - ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),
            VertexFormat(glm::vec3(ARM_DIM_X - ARM_OFFSET_X,  ARM_DIM_Y + ARM_OFFSET_Y, -ARM_DIM_Z), armColor),

        };

        vector<unsigned int> indices =
        {
            // Body indices
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,

            // Head Indices
             8,  9, 10,      9, 11, 10,
            10, 11, 15,     10, 15, 13,
             9, 15, 11,      9, 13, 15,
            14, 15, 12,     15, 13, 12,
             8, 12,  9,      9, 12, 13,
            10, 14, 12,      8, 10, 12,

            // Left Arm Indices
            16, 17, 18,        17, 19, 18,
            18, 19, 23,        18, 23, 21,
            17, 23, 19,        17, 21, 23,
            22, 23, 20,        23, 21, 20,
            16, 20, 17,        17, 20, 21,
            18, 22, 20,        16, 18, 20,

            // Right Arm Indices
            24, 25, 26,        25, 27, 26,
            26, 27, 31,        26, 31, 29,
            25, 31, 27,        25, 29, 31,
            30, 31, 28,        31, 29, 28,
            26, 28, 25,        25, 28, 29,
            26, 30, 28,        24, 26, 28,
        };

        CreateMesh("player", vertices, indices);
    }


    // Create a simple orange cube for walls
    {
        glm::vec3 wallColor = glm::vec3(1.f, 0.53f, 0.0f);
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1,  1), wallColor),
            VertexFormat(glm::vec3(1, -1,  1), wallColor),
            VertexFormat(glm::vec3(-1,  1,  1), wallColor),
            VertexFormat(glm::vec3(1,  1,  1), wallColor),
            VertexFormat(glm::vec3(-1, -1, -1), wallColor),
            VertexFormat(glm::vec3(1, -1, -1), wallColor),
            VertexFormat(glm::vec3(-1,  1, -1), wallColor),
            VertexFormat(glm::vec3(1,  1, -1), wallColor),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("wall", vertices, indices);
    }

    // Enemy Mesh
    {
        glm::vec3 enemyColor = glm::vec3(1, 0, 0);
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-ENEMY_DIM_X, -ENEMY_DIM_Y,  ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(ENEMY_DIM_X, -ENEMY_DIM_Y,  ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(-ENEMY_DIM_X,  ENEMY_DIM_Y,  ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(ENEMY_DIM_X,  ENEMY_DIM_Y,  ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(-ENEMY_DIM_X, -ENEMY_DIM_Y, -ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(ENEMY_DIM_X, -ENEMY_DIM_Y, -ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(-ENEMY_DIM_X,  ENEMY_DIM_Y, -ENEMY_DIM_Z), enemyColor),
            VertexFormat(glm::vec3(ENEMY_DIM_X,  ENEMY_DIM_Y, -ENEMY_DIM_Z), enemyColor),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,        1, 3, 2,
            2, 3, 7,        2, 7, 6,
            1, 7, 3,        1, 5, 7,
            6, 7, 4,        7, 5, 4,
            0, 4, 1,        1, 4, 5,
            2, 6, 4,        0, 2, 4,
        };

        CreateMesh("enemy", vertices, indices);
    }


    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(40, 0, 40), glm::vec3(0.75f)),
            VertexFormat(glm::vec3(40, 0, -40), glm::vec3(0.75f)),
            VertexFormat(glm::vec3(-40, 0, -40), glm::vec3(0.75f)),
            VertexFormat(glm::vec3(-40, 0, 40), glm::vec3(0.75f)),
        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            0, 2, 3,
        };

        CreateMesh("myPlane", vertices, indices);
    }
    
    cout << "You have " << TIME_LIMIT << " seconds available to get out of the maze!\n";

}


Mesh* Tema2::CreateMesh(const char *name, const std::vector<VertexFormat> &vertices, const std::vector<unsigned int> &indices)
{
    unsigned int VAO = 0;
    // Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Create the VBO and bind it
    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    // Create the IBO and bind it
    unsigned int IBO;
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    CheckOpenGLError();

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
    meshes[name]->vertices = vertices;
    meshes[name]->indices = indices;
    return meshes[name];
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}


void Tema2::Update(float deltaTimeSeconds)
{
    if (!gameOver) {
        // Verificare timpi. Afisam un mesaj de avertizare cu 10 secunde inainte de final.
        std::clock_t currTime = clock();
        if ((currTime - startUpTime) / CLOCKS_PER_SEC >= TIME_LIMIT - 10 && !tenSecWarning) {
            tenSecWarning = true;
            cout << "10 seconds remaining! Hurry up!\n";
        }

        // Daca am depasit timpul limita, oprim jocul
        if ((currTime - startUpTime) / CLOCKS_PER_SEC >= TIME_LIMIT) {
            gameOver = true;
            cout << "Time is up! Game over!\n";
        }

        // Camera Third-Person normal, First-Person in attackMode.
        if (attackMode) {
            camera->Set(glm::vec3(PlayerX + 0.2f * sin(playerRotation), PlayerY + 0.35f, PlayerZ + 0.2f * cos(playerRotation)), glm::vec3(PlayerX, PlayerY + 0.35f, PlayerZ), glm::vec3(0, 1, 0));
        }
        else {
            camera->Set(glm::vec3(PlayerX + 0.2f * sin(playerRotation), PlayerY + 0.35f, PlayerZ + 0.2f * cos(playerRotation)), glm::vec3(PlayerX, PlayerY + 0.3f, PlayerZ), glm::vec3(0, 1, 0));
        }

        // Daca s-a ajuns in casuta de iesire, jocul se termina.
        if (PlayerX >= TargetX - 0.5f && PlayerX <= TargetX + 0.5f &&
            PlayerZ >= TargetZ - 0.5f && PlayerZ <= TargetZ + 0.5f) {
            gameOver = true;
            cout << "You won!" << endl;
        }


        // Randam playerul. Daca suntem in modul atack(First Person), nu il mai randam
        if (!attackMode)
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(PlayerX, PlayerY, PlayerZ));
            modelMatrix = glm::rotate(modelMatrix, playerRotation, glm::vec3(0, 1, 0));
            RenderMesh(meshes["player"], shaders["VertexColor"], modelMatrix);
        }

        // Randam inamicii. Acestia se invart in jurul casutei lor.
        double rot = Engine::GetElapsedTime();
        double sinRot = sin(rot) / 4;
        double cosRot = cos(rot) / 4;
        for (int i = 0; i < enemies.size(); i++) {
            enemies[i].sinRot = sinRot;
            enemies[i].cosRot = cosRot;
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(enemies[i].x + enemies[i].sinRot, ENEMY_Y_LEVEL, enemies[i].z + enemies[i].cosRot));
            RenderMesh(meshes["enemy"], shaders["VertexColor"], modelMatrix);
        }


        // Walls render
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 10; j++) {
                if (map[i][j] == 1) {
                    glm::mat4 modelMatrix = glm::mat4(1);
                    modelMatrix = glm::translate(modelMatrix, glm::vec3(i, 0, j));
                    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5f, 1.5f, 0.5f));
                    RenderMesh(meshes["wall"], shaders["VertexColor"], modelMatrix);
                }
            }
        }

        int bulletSpeed = 10;
        for (int i = 0; i < bullets.size(); i++) {
            // Move bullets
            bullets[i].x += sin(bullets[i].rotation) * deltaTimeSeconds * bulletSpeed;
            bullets[i].z += cos(bullets[i].rotation) * deltaTimeSeconds * bulletSpeed;

            // Render bullets
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(bullets[i].x, 0.2f, bullets[i].z));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderMesh(meshes["sphere"], shaders["VertexColor"], modelMatrix);

            // Check bullet TTL. Mark for deletion if necessary.
            if ((float)(currTime - bullets[i].spawnTime) / CLOCKS_PER_SEC >= BULLET_TTL) {
                bullets[i].toBeRemoved = true;
            }

            // Check for Bullet-Wall collision
            if (AABBBulletWall(bullets[i])) {
                bullets[i].toBeRemoved = true;
            }

            // Check for Bullet-Enemy collision
            if (AABBBulletEnemy(bullets[i])) {
                bullets[i].toBeRemoved = true;
            }
        }

        // Remove marked bullets
        if (!bullets.empty()) {
            bullets.erase(std::remove_if(
                bullets.begin(),
                bullets.end(),
                [](Bullet3D const& b) {
                    return b.toBeRemoved;
                }),
                bullets.end());
        }

        // Remove marked enemies
        if (!enemies.empty()) {
            enemies.erase(std::remove_if(
                enemies.begin(),
                enemies.end(),
                [](Enemy3D const& e) {
                    return e.toBeRemoved;
                }),
                enemies.end());
        }

        // Floor render
        {
            glm::mat4 modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(5, 0, 5));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.25f));

            RenderMesh(meshes["myPlane"], shaders["VertexColor"], modelMatrix);
        }
    }

}

void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::FrameEnd()
{
    // DrawCoordinateSystem();
}


void Tema2::RenderSimpleMesh(Mesh *mesh, Shader *shader, const glm::mat4 & modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    // Get shader location for uniform mat4 "Model"
    int location = glGetUniformLocation(shader->program, "Model");

    // Set shader uniform "Model" to modelMatrix
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Get shader location for uniform mat4 "View"
    location = glGetUniformLocation(shader->program, "View");

    // Set shader uniform "View" to viewMatrix
    glm::mat4 viewMatrix = GetSceneCamera()->GetViewMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Get shader location for uniform mat4 "Projection"
    location = glGetUniformLocation(shader->program, "Projection");

    // Set shader uniform "Projection" to projectionMatrix
    glm::mat4 projectionMatrix = GetSceneCamera()->GetProjectionMatrix();
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{

    float oldPlayerX = PlayerX;
    float oldPlayerZ = PlayerZ;
    {
        float cameraSpeed = 2.0f;
        AABBPlayerEnemy();

        // Tragere proiectile (doar in modul attack) - space
        if (window->KeyHold(GLFW_KEY_SPACE) && attackMode) {
            std::clock_t currTime = clock();
            // T/O de 0.5 sec pentru fiecare bullet nou
            if ((float)(currTime - lastShootTime) / CLOCKS_PER_SEC >= BULLET_FIRE_RATE) {
                lastShootTime = currTime;
                bullets.push_back(Bullet3D(PlayerX, PlayerZ, playerRotation));
            }
        }

        // Deplasare inainte
        if (window->KeyHold(GLFW_KEY_W)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
            glm::vec3 offsetPos = dir * deltaTime * cameraSpeed;
            PlayerX += offsetPos.x;
            PlayerZ += offsetPos.z;
            if (!AABBPlayerWall() && !AABBPlayerEnemy()) {
                camera->MoveForward(deltaTime * cameraSpeed);
            }
            else
            {
                PlayerX = oldPlayerX;
                PlayerZ = oldPlayerZ;
            }
            
        }

        // Deplasare la stanga
        if (window->KeyHold(GLFW_KEY_A)) {
            glm::vec3 offsetPos = -deltaTime * cameraSpeed * glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));
            PlayerX += offsetPos.x;
            PlayerZ += offsetPos.z;
            if (!AABBPlayerWall() && !AABBPlayerEnemy()) {
                camera->TranslateRight(-deltaTime * cameraSpeed);
            }
            else
            {
                PlayerX = oldPlayerX;
                PlayerZ = oldPlayerZ;
            }
        }

        // Deplasare in spate
        if (window->KeyHold(GLFW_KEY_S)) {
            glm::vec3 dir = glm::normalize(glm::vec3(camera->forward.x, 0, camera->forward.z));
            glm::vec3 offsetPos = dir * - deltaTime * cameraSpeed;
            PlayerX += offsetPos.x;
            PlayerZ += offsetPos.z;
            if (!AABBPlayerWall() && !AABBPlayerEnemy()) {
                camera->MoveForward(-deltaTime * cameraSpeed);
            }
            else
            {
                PlayerX = oldPlayerX;
                PlayerZ = oldPlayerZ;
            }
        }

        // Deplasare in dreapta
        if (window->KeyHold(GLFW_KEY_D)) {
            glm::vec3 offsetPos = deltaTime * cameraSpeed * glm::normalize(glm::vec3(camera->right.x, 0, camera->right.z));
            PlayerX += offsetPos.x;
            PlayerZ += offsetPos.z;
            if (!AABBPlayerWall() && !AABBPlayerEnemy()) {
                camera->TranslateRight(deltaTime * cameraSpeed);
            }
            else
            {
                PlayerX = oldPlayerX;
                PlayerZ = oldPlayerZ;
            }
        }        
    }
}


// Coliziune Jucator - Obstacol
bool Tema2::AABBPlayerWall() {
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            if (map[i][j] == 1) {
                if (i - WALL_DIM <= PlayerX + PLAYER_HITBOX_DIM &&
                    i + WALL_DIM >= PlayerX - PLAYER_HITBOX_DIM &&
                    j - WALL_DIM <= PlayerZ + PLAYER_HITBOX_DIM &&
                    j + WALL_DIM >= PlayerZ - PLAYER_HITBOX_DIM) {
                    return true;
                }
            }
        }
    }
    return false;
}


// Coliziune Jucator - Inamic
bool Tema2::AABBPlayerEnemy() {
    for (int i = 0; i < enemies.size(); i++) {
        if (enemies[i].x + (float)enemies[i].sinRot - ENEMY_DIM_X <= PlayerX + PLAYER_HITBOX_DIM &&
            enemies[i].x + (float)enemies[i].sinRot + ENEMY_DIM_X >= PlayerX - PLAYER_HITBOX_DIM &&
            enemies[i].z + (float)enemies[i].cosRot - ENEMY_DIM_Z <= PlayerZ + PLAYER_HITBOX_DIM &&
            enemies[i].z + (float)enemies[i].cosRot + ENEMY_DIM_Z >= PlayerZ - PLAYER_HITBOX_DIM) {
            playerHP -= ENEMY_DAMAGE;
            cout << "PlayerHP = " << playerHP << endl;
            if (playerHP <= 0) {
                gameOver = true;
                cout << "Game Over!" << endl;
            }
            return true;
        }
    }

    return false;
}

// Coliziune Proiectil - Perete
bool Tema2::AABBBulletWall(Bullet3D b) {
    for (int i = 0; i < LABYRINTH_DIM; i++) {
        for (int j = 0; j < LABYRINTH_DIM; j++) {
            if (map[i][j] == 1) {
                float x = max(i - WALL_DIM, min(b.x, i + WALL_DIM));
                float y = max(-1.5f, min(BULLET_LEVEL, 1.5f));
                float z = max(j - WALL_DIM, min(b.z, j + WALL_DIM));

                float distance = sqrt((x - b.x) * (x - b.x) +
                                      (y - BULLET_LEVEL) * (y - BULLET_LEVEL) +
                                      (z - b.z) * (z - b.z));
                if (distance < SPHERE_RADIUS) {
                    return true;
                }
            }
        }
    }
    return false;
}


// Coliziune Proiectil - Inamic
bool Tema2::AABBBulletEnemy(Bullet3D b) {
    for (int i = 0; i < enemies.size(); i++) {
        float x = max(enemies[i].x + (float)enemies[i].sinRot - ENEMY_DIM_X, min(b.x, enemies[i].x + (float)enemies[i].sinRot + ENEMY_DIM_X));
        float y = max(ENEMY_Y_LEVEL - ENEMY_DIM_Y, min(0.2f, ENEMY_Y_LEVEL + ENEMY_DIM_Y));
        float z = max(enemies[i].z + (float)enemies[i].cosRot - ENEMY_DIM_Z - 0.5f, min(b.z, enemies[i].z + (float)enemies[i].cosRot - ENEMY_DIM_Z));

        float distance = sqrt((x - b.x) * (x - b.x) +
                              (y - BULLET_LEVEL) * (y - BULLET_LEVEL) +
                              (z - b.z) * (z - b.z));
        if (distance < SPHERE_RADIUS) {
            enemies[i].toBeRemoved = true;
            return true;
        }
    }
    return false;
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Rotatie jucator
    if (window->MouseHold(GLFW_MOUSE_BUTTON_LEFT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (!attackMode) 
        {
            camera->RotateThirdPerson_OY(sensivityOY * -deltaX);
        }
        else {
            camera->RotateFirstPerson_OY(sensivityOY * -deltaX);
        }
        playerRotation += sensivityOY * -deltaX;
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Comutare Mod normal - Mod atac
    if (button == GLFW_MOUSE_BUTTON_3) {
        attackMode = !attackMode;
    }
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}
