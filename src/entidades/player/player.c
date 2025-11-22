#include "player.h"

// ============================================================================
// CONFIGURAÇÃO DA SALA (ÁREA JOGÁVEL)
// Ajuste estes valores olhando para as linhas vermelhas na tela!
// ============================================================================

// Retângulo 1: O chão principal (o "grosso" da sala)
// Dica: Tente ajustar para cobrir o chão central, ignorando as portas por enquanto.
static Rectangle roomMain = { 
    .x = 190,      // Distância da borda esquerda da tela até o começo do chão
    .y = 120,      // Distância do topo da tela até o começo do chão
    .width = 650,  // Largura total do chão
    .height = 540  // Altura total do chão
};

// Retângulo 2: Corredor Horizontal (Porta Esq <-> Porta Dir)
// Útil se as portas ficarem um pouco para fora do retângulo principal
static Rectangle roomHori = { 
    .x = 80,       // Começa um pouco antes do main
    .y = 340,       // Altura centralizada verticalmente (aprox)
    .width = 880,   // Mais largo que o main para pegar as portas
    .height = 90    // Altura do corredor da porta
};

// Retângulo 3: Corredor Vertical (Porta Cima <-> Porta Baixo)
// Útil se as portas de cima/baixo ficarem para fora
static Rectangle roomVert = { 
    .x = 470,       // Centralizado horizontalmente (aprox)
    .y = 80,        // Começa mais acima que o main
    .width = 80,    // Largura da porta
    .height = 620   // Mais alto que o main
};

// ============================================================================

static Texture2D LoadTex(const char *file)
{
    return LoadTexture(file);
}

// ----------------------------------------------------
// Inicializar player
// ----------------------------------------------------
void InitPlayer(Player *p)
{   
    p->hitboxRadius = 18.0f; 
    
    
    p->body.position = (Vector2){ 
        roomMain.x + roomMain.width/2, 
        roomMain.y + roomMain.height/2 
    };

    p->body.speed = 200;
    p->body.direction = DIR_DOWN;

    p->head.direction = DIR_DOWN;
    p->head.frame = 0;
    p->head.timer = 0;
    p->head.animSpeed = 0.15f;


    p->hp = 6.0f;       
    p->maxHp = 6.0f;
    
    p->fireRate = 0.4f;
    p->fireCooldown = 0;

    // Carregamento de Sprites (Mantido igual)
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

    p->spriteWidth  = p->bodySprites.frente[0].width;
    p->spriteHeight = p->bodySprites.frente[0].height;  

    for (int i = 0; i < MAX_PROJECTILES; i++)
        p->projectiles[i].active = false;
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
    // Usa os limites do retângulo principal para destruir projéteis
    if (!CheckCollisionPointRec(pr->position, roomMain))
    {
        // Dá uma margem extra antes de destruir
        float margin = 50.0f;
        Rectangle bounds = { 
            roomMain.x - margin, roomMain.y - margin, 
            roomMain.width + margin*2, roomMain.height + margin*2 
        };
        
        if (!CheckCollisionPointRec(pr->position, bounds))
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

        LimitProjectileInsideRoom(&p->projectiles[i]);
    }
}

// ====================================================
// NOVA LÓGICA DE COLISÃO
// ====================================================

// Verifica se uma posição futura é válida
static bool IsInsidePlayableArea(Vector2 pos, float radius)
{
    Rectangle* areas[] = { &roomMain, &roomHori, &roomVert };

    // Verifica se o círculo do player está TOTALMENTE dentro de ALGUM retângulo
    for (int i = 0; i < 3; i++)
    {
        Rectangle r = *areas[i];

        // Aritmética simples: Posição +/- raio deve estar dentro de (X, Y, W, H)
        if (pos.x - radius >= r.x &&
            pos.x + radius <= r.x + r.width &&
            pos.y - radius >= r.y &&
            pos.y + radius <= r.y + r.height)
        {
            return true;
        }
    }
    return false;
}


void DamagePlayer(Player *p, float dmg)
{
    p->hp -= dmg;
    if (p->hp < 0) p->hp = 0;
}
// Update principal do Player
void UpdatePlayer(Player *p)
{
    float dt = GetFrameTime();

    p->isMovingBody = false;
    p->isMovingHead = false;

    // MOVIMENTO (WASD)
    Vector2 move = (Vector2){0};
    if (IsKeyDown(KEY_W)) move.y -= 1;
    if (IsKeyDown(KEY_S)) move.y += 1;
    if (IsKeyDown(KEY_A)) move.x -= 1;
    if (IsKeyDown(KEY_D)) move.x += 1;

    if (move.x != 0 || move.y != 0)
    {
        p->isMovingBody = true;
        move = Vector2Normalize(move);
        Vector2 velocity = Vector2Scale(move, p->body.speed * dt);
        
        // 1. Tentar movimento completo
        Vector2 nextPos = Vector2Add(p->body.position, velocity);
        
        if (IsInsidePlayableArea(nextPos, p->hitboxRadius))
        {
            p->body.position = nextPos;
        }
        else 
        {
            // 2. Deslizar nas paredes (Wall Sliding)
            // Tenta mover só em X
            Vector2 nextPosX = { p->body.position.x + velocity.x, p->body.position.y };
            if (IsInsidePlayableArea(nextPosX, p->hitboxRadius)) p->body.position.x = nextPosX.x;

            // Tenta mover só em Y
            Vector2 nextPosY = { p->body.position.x, p->body.position.y + velocity.y };
            if (IsInsidePlayableArea(nextPosY, p->hitboxRadius)) p->body.position.y = nextPosY.y;
        }

        // Direção do corpo
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
        p->bodySprites.frame = 0;
    }

    // CABEÇA
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
            p->head.frame = 1;
            p->head.timer = 0.0f;
        }
    }
    else
    {
        p->head.frame = 0;
    }

    p->head.timer += dt;
    if (p->head.timer >= p->head.animSpeed) p->head.frame = 0;

    // Posição Cabeça
    p->head.position.x = p->body.position.x;
    p->head.position.y = p->body.position.y - 28.0f;

    // Projéteis
    if (p->fireCooldown > 0.0f) p->fireCooldown -= dt;
    UpdateProjectiles(p, dt);

}

// Player Desenho
void DrawPlayer(Player *p)
{
    float scale = 2.0f;

    for (int i = 0; i < p->hp; i++){
    DrawCircle(30 + i * 20, 30, 8, RED);
    }
    // ============================================================
    // DEBUG: DESENHAR RETÂNGULOS DE COLISÃO
    // Use isso para ajustar os valores lá em cima no código!
    // ============================================================
    DrawRectangleLinesEx(roomMain, 2.0f, RED);   // Retângulo Principal
    DrawRectangleLinesEx(roomHori, 2.0f, BLUE);  // Corredor Horizontal
    DrawRectangleLinesEx(roomVert, 2.0f, GREEN); // Corredor Vertical
    // ============================================================

    // Desenha Sombra (opcional, ajuda na profundidade)
    DrawEllipse(p->body.position.x, p->body.position.y, 10, 5, (Color){0,0,0,100});

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