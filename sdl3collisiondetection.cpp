// sdl3collisiondetection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <GL/gl.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

// Vertex shader source code
const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    void main() {
        gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    }
)";

// Fragment shader source code
const char* fragmentShaderSource = R"(
    #version 330 core
    out vec4 FragColor;
    void main() {
        FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
    }
)";

typedef struct {
    float x, y;
} Point2D;

void drawLineSegment(SDL_Renderer* renderer, Point2D A, Point2D B, int numSteps) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    float step = 1.0f / numSteps;
    for(float t = 0.0f; t <= 1.0f; t += step) {
        float x = (1.0f - t) * A.x + t * B.x;
        float y = (1.0f - t) * A.y + t * B.y;
        SDL_RenderPoint(renderer, x, y);
    }
}

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    SDL_Window* window = SDL_CreateWindow(
        "SDL3 OpenGL",
        WINDOW_WIDTH,
        WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
    );

    if (!window) {
        fprintf(stderr, "Window could not be created! SDL_Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    //SDL_GLContext glContext = SDL_GL_CreateContext(window);
    //if (!glContext) {
    //    fprintf(stderr, "OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
    //    SDL_DestroyWindow(window);
    //    SDL_Quit();
    //    return 1;
    //}

    //if (SDL_GL_SetSwapInterval(1) < 0) {
    //    fprintf(stderr, "Warning: Unable to set VSync! SDL_Error: %s\n", SDL_GetError());
    //}

    bool quit = false;
    SDL_Event e;

    Point2D A = { 100.0f, 200.0f };  // Start point
    Point2D B = { 200.0f, 300.0f };  // End point

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_EVENT_QUIT) {
                quit = true;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        SDL_RenderClear(renderer);
        drawLineSegment(renderer, A, B, 1000);
        SDL_RenderPresent(renderer);
    }
    
    //SDL_GL_DestroyContext(glContext);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
