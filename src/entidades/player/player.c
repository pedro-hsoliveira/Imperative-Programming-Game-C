#include "player.h"

static Texture2D LoadTex(const char *file)
{
    return LoadTexture(file);
}

static bool IsInsideRect(Vector2 p, float rx, float ry, float rw, float rh);
static void AllowSideBlocks(Player *p);

// ----------------------------------------------------
// Inicializar player
// ----------------------------------------------------
void InitPlayer(Player *p)
{   
    p->hitboxRadius = 18.0f; 
    p->spriteWidth  = p->bodySprites.frente[0].width;
    p->spriteHeight = p->bodySprites.frente[0].height;    
    p->body.position = (Vector2){400, 260};
    p->body.speed = 200;
    p->body.direction = DIR_DOWN;

    p->head.direction = DIR_DOWN;
    p->head.frame = 0;
    p->head.timer = 0;
    p->head.animSpeed = 0.15f;

    p->fireRate = 0.4f;
    p->fireCooldown = 0;

    // SPRITES DO CORPO
    for (int i = 0; i < 6; i++)
    {
        p->bodySprites.frente[i]   = LoadTexture(TextFormat("../assets/sprites/player/tronco_perna/frente%d.png",   i+1));
        p->bodySprites.tras[i]     = LoadTexture(TextFormat("../assets/sprites/player/tronco_perna/tras%d.png",     i+1));
        p->bodySprites.esquerda[i] = LoadTexture(TextFormat("../assets/sprites/player/tronco_perna/direita%d.png", i+1));
        p->bodySprites.direita[i]  = LoadTexture(TextFormat("../assets/sprites/player/tronco_perna/esquerda%d.png",  i+1));
    }

    p->bodySprites.idle = LoadTexture("../assets/sprites/player/tronco_perna/parado1.png");
    p->bodySprites.frame = 0;
    p->bodySprites.timer = 0;
    p->bodySprites.animSpeed = 0.10f;

    // SPRITES DA CABEÇA
    p->headSprites.frente[0]   = LoadTexture("../assets/sprites/player/cabeca/frente1.png");
    p->headSprites.frente[1]   = LoadTexture("../assets/sprites/player/cabeca/frente2.png");

    p->headSprites.tras[0]     = LoadTexture("../assets/sprites/player/cabeca/tras1.png");
    p->headSprites.tras[1]     = LoadTexture("../assets/sprites/player/cabeca/tras2.png");

    p->headSprites.esquerda[0] = LoadTexture("../assets/sprites/player/cabeca/esquerda1.png");
    p->headSprites.esquerda[1] = LoadTexture("../assets/sprites/player/cabeca/esquerda2.png");

    p->headSprites.direita[0]  = LoadTexture("../assets/sprites/player/cabeca/direita1.png");
    p->headSprites.direita[1]  = LoadTexture("../assets/sprites/player/cabeca/direita2.png");

    p->headSprites.idle        = LoadTexture("../assets/sprites/player/cabeca/parado1.png");

    for (int i = 0; i < MAX_PROJECTILES; i++)
        p->projectiles[i].active = false;
}



// Base para futuro sistema de power-ups


void GerenciarPowerUps(Player *p)
{
    // FUTURO 
}

// Disparo 

static void Shoot(Player *p)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!p->projectiles[i].active)
        {
            p->projectiles[i].active = true;
            p->projectiles[i].position = p->head.position;
            p->projectiles[i].speed = 380;

            switch (p->head.direction)
            {
                case DIR_UP:    p->projectiles[i].direction = (Vector2){ 0, -1}; break;
                case DIR_DOWN:  p->projectiles[i].direction = (Vector2){ 0,  1}; break;
                case DIR_LEFT:  p->projectiles[i].direction = (Vector2){-1,  0}; break;
                case DIR_RIGHT: p->projectiles[i].direction = (Vector2){ 1,  0}; break;
            }
            return;
        }
    }
}


static void LimitProjectileInsideRoom(Projectile *pr)
{
    float roomOffsetX = 188;
    float roomOffsetY = 120;
    float roomWidth  = 611;
    float roomHeight = 516;

    float left   = roomOffsetX;
    float right  = roomOffsetX + roomWidth;
    float top    = roomOffsetY;
    float bottom = roomOffsetY + roomHeight;

    if (pr->position.x < left  ||
        pr->position.x > right ||
        pr->position.y < top   ||
        pr->position.y > bottom)
    {
        pr->active = false;
    }
}

// Atualiza projéteis

static void UpdateProjectiles(Player *p, float dt)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!p->projectiles[i].active) continue;

        p->projectiles[i].position.x += p->projectiles[i].direction.x * p->projectiles[i].speed * dt;
        p->projectiles[i].position.y += p->projectiles[i].direction.y * p->projectiles[i].speed * dt;

        // sai da tela
        LimitProjectileInsideRoom(&p->projectiles[i]);
    }
}


// LIMITES MAP

static void LimitPlayerInsideRoom(Player *p)
{
    float roomOffsetX = 188;
    float roomOffsetY = 120;

    float roomWidth  = 611;
    float roomHeight = 516;

    // Ajuste da hitbox (centro do corpo)
    float left   = roomOffsetX + p->hitboxRadius + 20;
    float right  = roomOffsetX + roomWidth  - p->hitboxRadius + 30;

    float top    = roomOffsetY + p->hitboxRadius - 5;
    float bottom = roomOffsetY + roomHeight - p->hitboxRadius + 20;

    // --- Se o player passar do limite, recoloca no limite ---

    if (p->body.position.x < left)
        p->body.position.x = left;

    if (p->body.position.x > right)
        p->body.position.x = right;

    if (p->body.position.y < top)
        p->body.position.y = top;

    if (p->body.position.y > bottom)
        p->body.position.y = bottom;
}




// Update principal do Player

void UpdatePlayer(Player *p)
{
    float dt = GetFrameTime();

    // ===============================
    // RESET DE ESTADOS
    // ===============================
    p->isMovingBody = false;
    p->isMovingHead = false;

    // ===============================
    // MOVIMENTO DO CORPO (WASD)
    // ===============================
    Vector2 move = (Vector2){0};

    if (IsKeyDown(KEY_W)) move.y -= 1;
    if (IsKeyDown(KEY_S)) move.y += 1;
    if (IsKeyDown(KEY_A)) move.x -= 1;
    if (IsKeyDown(KEY_D)) move.x += 1;

    if (move.x != 0 || move.y != 0)
    {
        p->isMovingBody = true;

        move = Vector2Normalize(move);

        p->body.position.x += move.x * p->body.speed * dt;
        p->body.position.y += move.y * p->body.speed * dt;

        // Direção do corpo (apenas WASD)
        if (fabs(move.y) > fabs(move.x))
            p->body.direction = (move.y < 0 ? DIR_UP : DIR_DOWN);
        else
            p->body.direction = (move.x < 0 ? DIR_LEFT : DIR_RIGHT);

        // Animação corpo
        p->bodySprites.timer += dt;
        if (p->bodySprites.timer >= p->bodySprites.animSpeed)
        {
            p->bodySprites.timer = 0;
            p->bodySprites.frame = (p->bodySprites.frame + 1) % 6;
        }
    }
    else
    {
        p->bodySprites.frame = 0;  // parado
    }

    // ===============================
    // CABEÇA (SETAS + TIRO)
    // ===============================
    bool headPressed = false;

    if (IsKeyDown(KEY_UP))    { p->head.direction = DIR_UP;    headPressed = true; }
    if (IsKeyDown(KEY_DOWN))  { p->head.direction = DIR_DOWN;  headPressed = true; }
    if (IsKeyDown(KEY_LEFT))  { p->head.direction = DIR_LEFT;  headPressed = true; }
    if (IsKeyDown(KEY_RIGHT)) { p->head.direction = DIR_RIGHT; headPressed = true; }

    if (headPressed)
    {
        p->isMovingHead = true;

        if (p->fireCooldown <= 0.0f)
        {
            Shoot(p);
            p->fireCooldown = p->fireRate;

            // Animação de tiro da cabeça
            p->head.frame = 1;
            p->head.timer = 0.0f;
        }
    }
    else
    {
        // nenhuma seta → sprite base
        p->head.frame = 0;
    }

    // animação da cabeça (frame 1 → frame 0)
    p->head.timer += dt;
    if (p->head.timer >= p->head.animSpeed)
    {
        p->head.frame = 0;
    }

    // ===============================
    // POSIÇÃO FIXA DA CABEÇA
    // ===============================
    // sem depender de spriteHeight por enquanto
    float headOffsetX = 0.0f;
    float headOffsetY = -28.0f;     // ajuste fino — valor mais estável que -30

    p->head.position.x = p->body.position.x + headOffsetX;
    p->head.position.y = p->body.position.y + headOffsetY;
    // ===============================
    // ATUALIZA PROJÉTEIS
    // ===============================
    if (p->fireCooldown > 0.0f)
        p->fireCooldown -= dt;

    UpdateProjectiles(p, dt);

    // ===============================
    // LIMITES DA SALA
    // ===============================
    LimitPlayerInsideRoom(p);
}





// Player Desenho


void DrawPlayer(Player *p)
{
    float scale = 2.0f;

    // ===================== CORPO ======================
    Texture2D bodyTex;

    if (!p->isMovingBody)
    {
        bodyTex = p->bodySprites.idle;
    }
    else
    {
        switch (p->body.direction)
        {
            case DIR_UP:    bodyTex = p->bodySprites.tras[p->bodySprites.frame]; break;
            case DIR_DOWN:  bodyTex = p->bodySprites.frente[p->bodySprites.frame]; break;
            case DIR_LEFT:  bodyTex = p->bodySprites.esquerda[p->bodySprites.frame]; break;
            case DIR_RIGHT: bodyTex = p->bodySprites.direita[p->bodySprites.frame]; break;
        }
    }

    Vector2 bodyDrawPos = {
        p->body.position.x - (bodyTex.width * scale) / 2,
        p->body.position.y - (bodyTex.height * scale) / 2
    };

    DrawTextureEx(bodyTex, bodyDrawPos, 0, scale, WHITE);


    // ===================== CABEÇA ======================
    Texture2D headTex;

    if (!p->isMovingHead)
    {
        headTex = p->headSprites.idle;
    }
    else
    {
        switch (p->head.direction)
        {
            case DIR_UP:    headTex = p->headSprites.tras[p->head.frame]; break;
            case DIR_DOWN:  headTex = p->headSprites.frente[p->head.frame]; break;
            case DIR_LEFT:  headTex = p->headSprites.esquerda[p->head.frame]; break;
            case DIR_RIGHT: headTex = p->headSprites.direita[p->head.frame]; break;
        }
    }

    Vector2 headDrawPos = {
        p->head.position.x - (headTex.width * scale) / 2,
        p->head.position.y - (headTex.height * scale) / 2
    };

    DrawTextureEx(headTex, headDrawPos, 0, scale, WHITE);

    // ===================== PROJÉTEIS ======================
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (p->projectiles[i].active)
            DrawCircleV(p->projectiles[i].position, 6, RED);
}
