#include "lab_m1/tema1/Tema1.h"


#include <iostream>

#include "lab_m1/tema1/Transform2D.h"


using namespace std;
using namespace m1;


// Used as argument for std::remove_if
bool bulletMarkedToBeRemoved(const Bullet b) {
    return b.toBeRemoved;
}

// Used as argument for std::remove_if
bool enemyMarkedToBeRemoved(const Enemy e) {
    return e.toBeRemoved;
}


Tema1::Tema1()
{
    // Trash init
    crsX = 0;
    crsY = 0;

    // Initializeaza structuri MyMap si Player
    map = new MyMap();
    player = new Player();

    // Width si Height vor fi reactualizate in Update
    logicSpace.x = 0;
    logicSpace.y = 0;
    logicSpace.width = 1280;
    logicSpace.height = 720;
    
    // Plasam jucatorul in mijlocul hartii
    playerX = map->width / 2;
    oldPlayerX = map->width / 2;
    playerY = map->height / 2;
    oldPlayerY = map->height / 2;

    // Initializam ceasurile
    startupTime = clock();
    lastShootTime = clock();
    lastEnemySpawnTime = clock();

    // Jocul abia incepe, so it's not GameOver yet :)
    gameEnded = false;
}


Tema1::~Tema1()
{
}


void Tema1::Init()
{
    // Gamera init
    glm::ivec2 resolution = window->GetResolution();
    camera = GetSceneCamera();
    camera->SetOrthographic(0, (float)resolution.x, 0, (float)resolution.y, 0.01f, 400);
    camera->SetPosition(glm::vec3(0, 0, 50));
    camera->SetRotation(glm::vec3(0, 0, 0));
    camera->Update();
    GetCameraInput()->SetActive(false);

    // Add some obstacles
    obstacles.push_back(Obstacle(300, 200, 100, 400));
    obstacles.push_back(Obstacle(850, 300, 400, 200));
    obstacles.push_back(Obstacle(1050, 750, 200, 200));
    obstacles.push_back(Obstacle(200, 800, 100, 100));
    obstacles.push_back(Obstacle(200, 800, 100, 100));
    obstacles.push_back(Obstacle(600, 950, 150, 300));
    obstacles.push_back(Obstacle(1000, 1200, 200, 100));
}


void Tema1::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema1::Update(float deltaTimeSeconds)
{
    // Daca Player-ul are HP sub 0 sau egal, jocul s-a incheiat, nu mai randa nimic
    if (player->hp > 0) {
        glm::ivec2 resolution = window->GetResolution();
        glm::ivec2 cursor = window->GetCursorPosition();
        // Actualizeaza lugimea si latimea ferestrei
        logicSpace.width = resolution.x;
        logicSpace.height = resolution.y;

        // Calculam unghiul dintre centrul ferestrei(implicit si centrul jucatorului) si cursor
        // (util pentru orientarea jucatorului, orientarea proiectilelor, etc.)
        crsX = (float)cursor.x;
        crsY = (float)(resolution.y - cursor.y);
        dX = crsX - resolution.x / 2;
        dY = crsY - resolution.y / 2;
        tetha = atan2(dY, dX);

        // Seteaza aria de desenare a jocului
        viewSpace = ViewSpace(0, 0, resolution.x, resolution.y);
        SetViewportArea(viewSpace, glm::vec3(0), true);

        // Calculam matricea de transformare fereastra-poarta uniforma
        glm::mat3 visMatrix = glm::mat3(1);
        visMatrix *= VisualizationTransf2DUnif(logicSpace, viewSpace);


        // Verifica daca exista coliziune cu obstacole
        // Daca exista, nu modifica pozitia jucatorului
        for (Obstacle o : obstacles) {
            if (o.PlayerCollision(playerX, playerY)) {
                playerX = oldPlayerX;
                playerY = oldPlayerY;
                break;
            }
        }

        // Randeaza HealthBar
        renderHealthBar(visMatrix);

        // Randeaza jucator
        renderEyes(visMatrix);
        renderBody(visMatrix);

        // Randeaza inamici
        renderEnemies(visMatrix, deltaTimeSeconds);

        // Spawneaza inamici noi daca este cazul
        std::clock_t currTime = clock();
        if ((float)(currTime - lastEnemySpawnTime) / CLOCKS_PER_SEC >= ENEMY_SPAWN_TIME) {
            lastEnemySpawnTime = currTime;
            int packOfEnemies = 1;
            // La fiecare 30 secunde mareste numarul de inamici spawnati cu inca 4
            packOfEnemies += ((currTime - startupTime) / CLOCKS_PER_SEC) / ENEMY_INCREASE_TIME;
            for (int i = 0; i < packOfEnemies; i++) {
                enemies.push_back(Enemy(false, false));
                enemies.push_back(Enemy(true, false));
                enemies.push_back(Enemy(false, true));
                enemies.push_back(Enemy(true, true));
            }
        }


        // Randeaza obstacole
        for (Obstacle o : obstacles) {
            RenderMesh2D(map->getObsBoxMesh(), shaders["VertexColor"], o.GetModelMatrix(visMatrix));
        }


        // Randeaza proiectile
        renderBullets(visMatrix, deltaTimeSeconds);

        // Randare harta
        RenderMesh2D(map->getMapMesh(), visMatrix, glm::vec3(1, 0.95f, 0.56f));

        // Actualizeaza pozitie camera
        glm::vec2 coord = Logic2View(glm::vec2(playerX, playerY));
        camera->SetPosition(glm::vec3(coord.x - (resolution.x / 2), coord.y - (resolution.y / 2), 50));
        camera->Update();
    
    } else {
        // Daca jocul tocmai s-a terminat, afiseaza scorul final in consola
        if (!gameEnded) {
            cout << "Game over! Total score: " << player->score << endl;
            gameEnded = true;
        }
    }
    
}

void Tema1::renderHealthBar(glm::mat3 visMat) {
    glm::mat3 modelMatrix = glm::mat3(1);
    glm::vec2 res = window->GetResolution();

    float scaleFactor = (float)player->hp / 100.0;

    modelMatrix *= transform2D::Translate(playerX - HEALTHBAR_OFFSET, playerY + (res.y / 2) - HEALTHBAR_OFFSET);
    RenderMesh2D(map->getWireBarMesh(), shaders["VertexColor"], modelMatrix);
    modelMatrix *= transform2D::Scale(scaleFactor, 1.0f);
    RenderMesh2D(map->getBarMesh(), shaders["VertexColor"], modelMatrix);


}

void Tema1::renderEyes(glm::mat3 visMat) {
    glm::mat3 modelMatrix = visMat;

    modelMatrix *= transform2D::Translate(playerX, playerY);
    modelMatrix *= transform2D::Rotate(tetha - HALF_PI);
    modelMatrix *= transform2D::Translate(-player->eyesOffsetX, -player->eyesOffsetY);
    
    RenderMesh2D(player->getEyeMesh(), shaders["VertexColor"], modelMatrix);
}

void Tema1::renderBody(glm::mat3 visMat) {
    glm::mat3 modelMatrix = visMat;

    modelMatrix *= transform2D::Translate(playerX, playerY);
    modelMatrix *= transform2D::Rotate(tetha - HALF_PI);

    RenderMesh2D(player->getBodyMesh(), shaders["VertexColor"], modelMatrix);
}

void Tema1::renderEnemies(glm::mat3 visMat, float deltaTime) {
    // Elimina inamicii care au lovit jucatorul sau care au fost loviti de un poriectil(markedToBeRemoved)
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), enemyMarkedToBeRemoved), enemies.end());
    
    for (int i = 0; i < enemies.size(); i++) {
        // Randeaza inamicul la pozitia corespunzatoare
        enemies[i].Move(playerX, playerY, deltaTime);
        glm::mat3 modelMatrix = visMat * transform2D::Translate(enemies[i].x, enemies[i].y);
        RenderMesh2D(player->getEnemyMesh(), shaders["VertexColor"], modelMatrix);

        // Daca s-a produs o coliziune cu un inamic, marcheaza-l pentru eliminare
        // Actualizeaza HP si afiseaza in consola
        if (!enemies[i].toBeRemoved && enemies[i].PlayerCollision(playerX, playerY)) {
            player->hp -= 5;
            cout << "Player HP = " << player->hp << endl;
            enemies[i].toBeRemoved = true;
        }
    }
}


void Tema1::renderBullets(glm::mat3 visMat, float deltaTime) {
    float bulletSpeed = 1000;
    
    // Elimina proiectilele de sub obstacole sau care au atins distanta maxima de deplasare
    // Coliziune proiectil - obstacol (2.5p)
    bullets.erase(std::remove_if(bullets.begin(), bullets.end(), bulletMarkedToBeRemoved), bullets.end());

    // Coliziune proiectil - perete harta (2.5p)
    // Vom sterge din vectorul de proiectile toate elementele
    // care au depasit conturul hartii
    if (!bullets.empty()) {
        bullets.erase(std::remove_if(
            bullets.begin(),
            bullets.end(),
            [](Bullet const& b)
            {
                return (b.x < 0 || b.y < 0 || b.x > MAP_WIDTH || b.y > MAP_HEIGHT);
            }
        ),
        bullets.end());

    }

    // Proiectile (tragere si desenare) - 20p
    for (int i = 0; i < bullets.size(); i++) {
        // Calculam noile coordonate ale proiectilului
        bullets[i].x += deltaTime * cos(bullets[i].rotation) * bulletSpeed;
        bullets[i].y += deltaTime * sin(bullets[i].rotation) * bulletSpeed;
        
        // Randam proiectilele tinand cont de pozitia si roatatia acestora
        glm::mat3 bVisMatrix = visMat;
        bVisMatrix *= transform2D::Translate(bullets[i].x, bullets[i].y);
        bVisMatrix *= transform2D::Rotate(bullets[i].rotation - HALF_PI);
        RenderMesh2D(player->getBulletMesh(), shaders["VertexColor"], bVisMatrix);

        // Verificam daca obstacolul a atins distanta maxima de deplasare
        // si marcheaza pentru a fi sters de la randarile urmatoare
        bullets[i].MaxDistanceReached();

        // Verificam daca s-a produs o coliziune cu obstacole si marcheaza
        // pentru a fi sters de la randarile urmatoare
        for (Obstacle o : obstacles) {
            if (bullets[i].ObstacleCollision(o)) {
                bullets[i].toBeRemoved = true;
                break;
            }
        }
        // Verificam daca s-a produs o coliziune intre proiectil si inamici
        // In caz afirmativ, marcam atat proiectilul cat si inamicul pentru
        // a fi eliminat din vectorii lor si actualizam si afisam
        // scorul obtinut de jucator
        for (int j = 0; j < enemies.size(); j++) {
            if (!enemies[j].toBeRemoved && !bullets[i].toBeRemoved) {
                if (bullets[i].EnemyCollision(enemies[j])) {
                    bullets[i].toBeRemoved = true;
                    enemies[j].toBeRemoved = true;
                    player->score += ENEMY_DESTR_SCORE;
                    cout << "Score = " << player->score << endl;
                }
            }
        }
    }

}

void Tema1::FrameEnd()
{
}


// Functie ce primeste coordonatele jucaatorului si intoarce coordonatele camerei
glm::vec2 Tema1::Logic2View(glm::vec2 logicCor) {
    float sx = viewSpace.width / logicSpace.width;
    float sy = viewSpace.height / logicSpace.height;
    float scaleF = sy < sx ? sy : sx;

    return glm::vec2(scaleF * logicCor.x * viewSpace.width / logicSpace.width, scaleF * logicCor.y * viewSpace.height / logicSpace.height);

}


void Tema1::OnInputUpdate(float deltaTime, int mods)
{    
    // In cazul in care avem o coliziune cu un obstacol, aavem nevoie de vechile
    // coordonate in care nu aveam inca o coliziune
    oldPlayerX = playerX;
    oldPlayerY = playerY;

    // Deplasare din taste. Tot aici
    // vom trata coliziunea cu capatul hartii
    // Coliziune jucator - perete harta (5p)
    if (window->KeyHold(GLFW_KEY_W)) {
        playerY += player->speed * deltaTime;
        if (playerY > map->height - player->radius) {
            playerY = map->height - player->radius;
        }
        
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        playerY -= player->speed * deltaTime;
        if (playerY < player->radius) {
            playerY = player->radius;
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        playerX -= player->speed * deltaTime;
        if (playerX < player->radius) {
            playerX = player->radius;
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        playerX += player->speed * deltaTime;
        if (playerX > map->width - player->radius) {
            playerX = map->width - player->radius;
        }
    }
}


void Tema1::OnKeyPress(int key, int mods)
{

}


void Tema1::OnKeyRelease(int key, int mods)
{

}


void Tema1::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{

}


void Tema1::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_2) {
        std::clock_t currTime = clock();
        // T/O de 0.5 sec pentru fiecare bullet nou
        if ((float)(currTime - lastShootTime) / CLOCKS_PER_SEC >= BULLET_FIRE_RATE) {
            lastShootTime = currTime;
            // Creeaza un proiectil nou tinand cont de rotatia mouse-ului fata de mijlocul ecranului
            bullets.push_back(Bullet(playerX + (PLAYER_BODY_RADIUS * cos(tetha)), playerY + (PLAYER_BODY_RADIUS * sin(tetha)), tetha));
        }
    }
}


void Tema1::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema1::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema1::OnWindowResize(int width, int height)
{
}

// Uniform 2D visualization matrix (same scale factor on x and y axes)
// Credits: EGC, Lab3
glm::mat3 Tema1::VisualizationTransf2DUnif(const LogicSpace& logicSpace, const ViewSpace& viewSpace)
{
    float sx, sy, tx, ty, smin;
    sx = viewSpace.width / logicSpace.width;
    sy = viewSpace.height / logicSpace.height;
    if (sx < sy)
        smin = sx;
    else
        smin = sy;
    tx = viewSpace.x - smin * logicSpace.x + (viewSpace.width - smin * logicSpace.width) / 2;
    ty = viewSpace.y - smin * logicSpace.y + (viewSpace.height - smin * logicSpace.height) / 2;
    

    return glm::transpose(glm::mat3(
        smin, 0.0f, tx,
        0.0f, smin, ty,
        0.0f, 0.0f, 1.0f));
}

// Credits: EGC, Lab03
void Tema1::SetViewportArea(const ViewSpace& viewSpace, glm::vec3 colorColor, bool clear)
{
    glViewport(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    glEnable(GL_SCISSOR_TEST);
    glScissor(viewSpace.x, viewSpace.y, viewSpace.width, viewSpace.height);

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(colorColor.r, colorColor.g, colorColor.b, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);

    GetSceneCamera()->SetOrthographic((float)viewSpace.x, (float)(viewSpace.x + viewSpace.width), (float)viewSpace.y, (float)(viewSpace.y + viewSpace.height), 0.1f, 400);
    GetSceneCamera()->Update();
}
