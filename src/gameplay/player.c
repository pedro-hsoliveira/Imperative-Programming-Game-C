#include "player.h"

static Texture2D LoadTex(const char *file)
{
    return LoadTexture(file);
}

// ----------------------------------------------------
// Inicializar player
// ----------------------------------------------------
void InitPlayer(Player *p)
{
    // Corpo
    p->body.position = (Vector2){400, 260};
    p->body.speed = 200;
    p->body.direction = DIR_DOWN;

    // Cabeça
    p->head.direction = DIR_DOWN;
    p->head.frame = 0;
    p->head.timer = 0;
    p->head.animSpeed = 0.15f;

    // Tiro
    p->fireRate = 0.18f;
    p->fireCooldown = 0;

    // -------------------------
    // CARREGAR SPRITES CORPO
    // -------------------------
    for (int i = 0; i < 6; i++)
    {
        p->bodySprites.frente[i]   = LoadTex(TextFormat("assets/sprites/player/tronco_perna/frente%d.png", i+1));
        p->bodySprites.tras[i]     = LoadTex(TextFormat("assets/sprites/player/tronco_perna/tras%d.png", i+1));
        p->bodySprites.esquerda[i] = LoadTex(TextFormat("assets/sprites/player/tronco_perna/esquerda%d.png", i+1));
        p->bodySprites.direita[i]  = LoadTex(TextFormat("assets/sprites/player/tronco_perna/direita%d.png", i+1));
    }

    p->bodySprites.idle = LoadTex("assets/sprites/player/tronco_perna/parado1.png");
    p->bodySprites.frame = 0;
    p->bodySprites.timer = 0;
    p->bodySprites.animSpeed = 0.10f;

    // -------------------------
    // CARREGAR SPRITES CABEÇA
    // -------------------------
    p->headSprites.frente[0]   = LoadTex("assets/sprites/player/cabeca/frente1.png");
    p->headSprites.frente[1]   = LoadTex("assets/sprites/player/cabeca/frente2.png");

    p->headSprites.tras[0]     = LoadTex("assets/sprites/player/cabeca/tras1.png");
    p->headSprites.tras[1]     = LoadTex("assets/sprites/player/cabeca/tras2.png");

    p->headSprites.esquerda[0] = LoadTex("assets/sprites/player/cabeca/esquerda1.png");
    p->headSprites.esquerda[1] = LoadTex("assets/sprites/player/cabeca/esquerda2.png");

    p->headSprites.direita[0]  = LoadTex("assets/sprites/player/cabeca/direita1.png");
    p->headSprites.direita[1]  = LoadTex("assets/sprites/player/cabeca/direita2.png");

    p->headSprites.idle        = LoadTex("assets/sprites/player/cabeca/parado1.png");

    // Desativar tiros
    for (int i = 0; i < MAX_PROJECTILES; i++)
        p->projectiles[i].active = false;
}



// --------------------------------------------------------------
// Base para futuro sistema de power-ups
// --------------------------------------------------------------
void GerenciarPowerUps(Player *p)
{
    // FUTURO 
}

// --------------------------------------------------------------
// Disparo 
// --------------------------------------------------------------
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


// --------------------------------------------------------------
// Atualiza projéteis
// --------------------------------------------------------------
static void UpdateProjectiles(Player *p, float dt)
{
    for (int i = 0; i < MAX_PROJECTILES; i++)
    {
        if (!p->projectiles[i].active) continue;

        p->projectiles[i].position.x += p->projectiles[i].direction.x * p->projectiles[i].speed * dt;
        p->projectiles[i].position.y += p->projectiles[i].direction.y * p->projectiles[i].speed * dt;

        // sai da tela
        if (p->projectiles[i].position.x < 0 || p->projectiles[i].position.x > 800 ||
            p->projectiles[i].position.y < 0 || p->projectiles[i].position.y > 450)
        {
            p->projectiles[i].active = false;
        }
    }
}

// --------------------------------------------------------------
// Update principal do Player
// --------------------------------------------------------------
void UpdatePlayer(Player *p)
{
    float dt = GetFrameTime();

    if (p->fireCooldown > 0)
        p->fireCooldown -= dt;

    // ------------------------------
    // MOVIMENTO DO CORPO
    // ------------------------------
    Vector2 move = {0};

    if (IsKeyDown(KEY_W)) move.y -= 1;
    if (IsKeyDown(KEY_S)) move.y += 1;
    if (IsKeyDown(KEY_A)) move.x -= 1;
    if (IsKeyDown(KEY_D)) move.x += 1;

    bool moving = (move.x != 0 || move.y != 0);

    if (moving)
    {
        move = Vector2Normalize(move);
        p->body.position.x += move.x * p->body.speed * dt;
        p->body.position.y += move.y * p->body.speed * dt;

        if (fabs(move.y) > fabs(move.x))
            p->body.direction = (move.y < 0 ? DIR_UP : DIR_DOWN);
        else
            p->body.direction = (move.x < 0 ? DIR_LEFT : DIR_RIGHT);

        // animação corpo
        p->bodySprites.timer += dt;
        if (p->bodySprites.timer >= p->bodySprites.animSpeed)
        {
            p->bodySprites.timer = 0;
            p->bodySprites.frame = (p->bodySprites.frame + 1) % 6;
        }
    }
    else
    {
        p->bodySprites.frame = 0;
    }

    // ------------------------------
    // CABEÇA — SÓ COM SETAS
    // ------------------------------
    bool headPressed = false;

    if (IsKeyDown(KEY_UP))    { p->head.direction = DIR_UP;    headPressed = true; }
    if (IsKeyDown(KEY_DOWN))  { p->head.direction = DIR_DOWN;  headPressed = true; }
    if (IsKeyDown(KEY_LEFT))  { p->head.direction = DIR_LEFT;  headPressed = true; }
    if (IsKeyDown(KEY_RIGHT)) { p->head.direction = DIR_RIGHT; headPressed = true; }

    if (headPressed)
    {
        if (p->fireCooldown <= 0)
        {
            Shoot(p);
            p->fireCooldown = p->fireRate;
        }

        p->head.frame = 1;
        p->head.timer = 0;
    }

    // animação da cabeça: 1 → 0
    p->head.timer += dt;
    if (p->head.timer >= p->head.animSpeed)
    {
        p->head.timer = 0;
        p->head.frame = 0;
    }

    // posição fixa da cabeça (offset manual)
    p->head.position = (Vector2){
        p->body.position.x,
        p->body.position.y - 22
    };

    UpdateProjectiles(p, dt);
}


// --------------------------------------------------------------
// Desenho final do Player
// --------------------------------------------------------------


void DrawPlayer(Player *p)
{
    Texture2D bodyTex;

    switch (p->body.direction)
    {
        case DIR_UP:    bodyTex = p->bodySprites.tras[p->bodySprites.frame]; break;
        case DIR_DOWN:  bodyTex = p->bodySprites.frente[p->bodySprites.frame]; break;
        case DIR_LEFT:  bodyTex = p->bodySprites.esquerda[p->bodySprites.frame]; break;
        case DIR_RIGHT: bodyTex = p->bodySprites.direita[p->bodySprites.frame]; break;
    }

    DrawTexture(bodyTex,
        p->body.position.x - bodyTex.width/2,
        p->body.position.y - bodyTex.height/2,
        WHITE);

    // CABEÇA
    Texture2D headTex;

    switch (p->head.direction)
    {
        case DIR_UP:    headTex = p->headSprites.tras[p->head.frame]; break;
        case DIR_DOWN:  headTex = p->headSprites.frente[p->head.frame]; break;
        case DIR_LEFT:  headTex = p->headSprites.esquerda[p->head.frame]; break;
        case DIR_RIGHT: headTex = p->headSprites.direita[p->head.frame]; break;
    }


    
   p->head.position.y = p->body.position.y - 18;

    DrawTexture(headTex,
        p->head.position.x - headTex.width/2,
        p->head.position.y - headTex.height/2,
        WHITE);

    // Tiros
    for (int i = 0; i < MAX_PROJECTILES; i++)
        if (p->projectiles[i].active)
            DrawCircleV(p->projectiles[i].position, 6, RED);
}