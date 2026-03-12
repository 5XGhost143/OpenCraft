#include <SDL2/SDL.h>
#include <GL/glew.h>
#include <iostream>

#include "Engine/Core/Types.hpp"
#include "Engine/Core/Math.hpp"
#include "Engine/Core/Physics.hpp"
#include "Engine/Input/Input.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Render/Shader.hpp"
#include "Engine/Render/ParticleRenderer.hpp"
#include "Engine/Render/OutlineRenderer.hpp"

#include "Game/World/World.hpp"
#include "Game/World/MeshBuilder.hpp"
#include "Game/World/Particle.hpp"
#include "Game/Player/Camera.hpp"
#include "Game/Player/Raycaster.hpp"
#include "Game/UI/HUD.hpp"

static void GLAPIENTRY glDebugCallback(GLenum, GLenum type, GLuint, GLenum severity,
    GLsizei, const GLchar* msg, const void*)
{
    if (severity == GL_DEBUG_SEVERITY_NOTIFICATION)
        return;

    std::cerr << "[GL] " << msg << '\n';

    if (type == GL_DEBUG_TYPE_ERROR)
        std::abort();
}

int main(int, char*[])
{
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
    {
        std::cerr << "SDL_Init: " << SDL_GetError() << '\n';
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,  SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,         SDL_GL_CONTEXT_DEBUG_FLAG);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   24);

    SDL_Window* window = SDL_CreateWindow("OpenCraft",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (!window)
    {
        std::cerr << SDL_GetError();
        SDL_Quit();
        return 1;
    }

    SDL_GLContext glCtx = SDL_GL_CreateContext(window);

    if (!glCtx)
    {
        std::cerr << SDL_GetError();
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetSwapInterval(1);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cerr << "glewInit failed\n";
        return 1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(glDebugCallback, nullptr);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    SDL_SetRelativeMouseMode(SDL_TRUE);

    Audio audio;
    audio.init();

    World world;
    world.generate();

    WorldMesh worldMesh;
    worldMesh.init();
    worldMesh.build(world);

    Shader worldShader;
    worldShader.load("gl_shaders/world.vert", "gl_shaders/world.frag");

    ParticleRenderer particleRenderer;
    particleRenderer.init();

    OutlineRenderer outlineRenderer;
    outlineRenderer.init();

    HUD hud;
    hud.init();

    ParticleSystem particles;
    Player         player;
    Camera         camera;
    Input          input;

    float  breakProgress     = 0.f;
    float  bedrockFlashTimer = 0.f;
    RayHit currentHit;

    BlockID heldBlock = BLOCK_GRASS;

    Uint32 lastTick = SDL_GetTicks();
    bool   running  = true;

    while (running)
    {
        Uint32 now = SDL_GetTicks();
        float  dt  = (now - lastTick) * 0.001f;
        if (dt > 0.05f) dt = 0.05f;
        lastTick = now;
        float t = now * 0.001f;

        input.beginFrame();

        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
                running = false;

            if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE)
            {
                bool rel = SDL_GetRelativeMouseMode();
                SDL_SetRelativeMouseMode(rel ? SDL_FALSE : SDL_TRUE);
            }

            if (e.type == SDL_WINDOWEVENT && e.window.event == SDL_WINDOWEVENT_RESIZED)
                glViewport(0, 0, e.window.data1, e.window.data2);

            if (e.type == SDL_KEYDOWN)
            {
                SDL_Keycode k = e.key.keysym.sym;
                if (k == SDLK_1) heldBlock = BLOCK_GRASS;
                if (k == SDLK_2) heldBlock = BLOCK_DIRT;
                if (k == SDLK_3) heldBlock = BLOCK_STONE;
            }

            input.processEvent(e);
        }

        if (SDL_GetRelativeMouseMode())
            camera.rotate((float)input.mouseRelX, (float)input.mouseRelY);

        Vec3 fwd   = camera.forward();
        Vec3 right = camera.right();
        fwd.y   = 0; fwd   = fwd.normalized();
        right.y = 0; right = right.normalized();

        Vec3 wishMove = {0, 0, 0};
        if (input.held(SDLK_w)) wishMove += fwd;
        if (input.held(SDLK_s)) wishMove -= fwd;
        if (input.held(SDLK_d)) wishMove += right;
        if (input.held(SDLK_a)) wishMove -= right;
        if (wishMove.length() > 0.01f) wishMove = wishMove.normalized();

        bool jumpPressed = input.held(SDLK_SPACE);

        player.update(dt, world, wishMove, jumpPressed);
        camera.pos = player.eyePos();

        currentHit = raycast(world, camera.pos, camera.forward());

        bool holdingBreak = false;
        if (SDL_GetRelativeMouseMode())
        {
            Uint32 mb  = SDL_GetMouseState(nullptr, nullptr);
            holdingBreak = (mb & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0;
        }

        if (currentHit.hit && holdingBreak)
        {
            BlockID target    = world.getBlock(currentHit.block.x, currentHit.block.y, currentHit.block.z);
            bool    isBedrock = (target == BLOCK_BEDROCK);
            float   breakSpeed = isBedrock ? 6.f : 1.8f;

            breakProgress += dt * breakSpeed;

            if (breakProgress >= 1.f)
            {
                IVec3           b   = currentHit.block;
                const BlockDef& def = getBlockDef(target);
                Vec3 center = {b.x + 0.5f, b.y + 0.5f, b.z + 0.5f};

                particles.spawnBreak(center, def.colorSide[0], def.colorSide[1], def.colorSide[2]);
                audio.playBreak();

                if (isBedrock)
                {
                    world.setBlock(b.x, b.y, b.z, BLOCK_BEDROCK);
                    bedrockFlashTimer = 0.25f;
                }
                else
                {
                    world.setBlock(b.x, b.y, b.z, BLOCK_AIR);
                }

                breakProgress  = 0.f;
                worldMesh.dirty = true;
            }
        }
        else
        {
            breakProgress = 0.f;
        }

        if (bedrockFlashTimer > 0)
            bedrockFlashTimer -= dt;

        if (currentHit.hit && input.rightClicked && SDL_GetRelativeMouseMode())
        {
            IVec3 placePos =
            {
                currentHit.block.x + currentHit.normal.x,
                currentHit.block.y + currentHit.normal.y,
                currentHit.block.z + currentHit.normal.z,
            };

            if (world.inBounds(placePos.x, placePos.y, placePos.z) &&
                world.getBlock(placePos.x, placePos.y, placePos.z) == BLOCK_AIR)
            {
                float px0 = player.pos.x - HALF_W;
                float px1 = player.pos.x + HALF_W;
                float py0 = player.pos.y;
                float py1 = player.pos.y + PLAYER_HEIGHT;
                float pz0 = player.pos.z - HALF_W;
                float pz1 = player.pos.z + HALF_W;

                bool wouldOverlapPlayer =
                    (placePos.x + 1 > px0) && (placePos.x < px1) &&
                    (placePos.y + 1 > py0) && (placePos.y < py1) &&
                    (placePos.z + 1 > pz0) && (placePos.z < pz1);

                if (!wouldOverlapPlayer)
                {
                    world.setBlock(placePos.x, placePos.y, placePos.z, heldBlock);
                    const BlockDef& def = getBlockDef(heldBlock);
                    Vec3 center = {placePos.x + 0.5f, placePos.y + 0.5f, placePos.z + 0.5f};
                    particles.spawnPlace(center, def.colorTop[0], def.colorTop[1], def.colorTop[2]);
                    audio.playPlace();
                    worldMesh.dirty = true;
                }
            }
        }

        if (worldMesh.dirty)
            worldMesh.build(world);

        particles.update(dt);

        int   w = 0, h = 0;
        SDL_GetWindowSize(window, &w, &h);
        float aspect = (float)w / (float)h;

        Mat4 proj = Mat4::perspective(1.0472f, aspect, 0.05f, 200.f);
        Mat4 view = camera.viewMatrix();
        Mat4 vp   = proj * view;

        glClearColor(0.45f, 0.68f, 0.95f, 1.f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        worldShader.use();
        worldShader.setMat4("u_vp", vp);
        worldShader.setVec3("u_camPos", camera.pos);
        worldShader.setFloat("u_time", t);

        float bx = currentHit.hit ? (float)currentHit.block.x : -9999.f;
        float by = currentHit.hit ? (float)currentHit.block.y : -9999.f;
        float bz = currentHit.hit ? (float)currentHit.block.z : -9999.f;
        worldShader.setVec3("u_breakPos", {bx, by, bz});
        worldShader.setFloat("u_breakAnim", breakProgress);

        worldMesh.draw();

        if (currentHit.hit)
        {
            Vec3  bp        = {(float)currentHit.block.x, (float)currentHit.block.y, (float)currentHit.block.z};
            float flashAnim = bedrockFlashTimer > 0 ? (std::sin(t * 60.f) * 0.5f + 0.5f) : breakProgress;
            outlineRenderer.draw(bp, vp, flashAnim, t);
        }

        particleRenderer.draw(particles, vp);

        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        hud.drawCrosshair(w, h);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);

        SDL_GL_SwapWindow(window);
    }

    audio.destroy();
    worldMesh.destroy();
    worldShader.destroy();
    particleRenderer.destroy();
    outlineRenderer.destroy();
    hud.destroy();

    SDL_GL_DeleteContext(glCtx);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}