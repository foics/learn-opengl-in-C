#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>

#include "shader.h"

#define TICK_INTERVAL 30

static Uint32 next_time;

Uint32 time_left(void) {
    Uint32 now;

    now = SDL_GetTicks();
    if (next_time <= now) {
        return 0;
    } else {
        return next_time - now;
    }
}

const int WIDTH = 1280;
const int HEIGHT = 720;

float greenValue = 0.0f;
bool greenValueInc = true;

bool should_quit = false;

int main(int argc, char *argv[]) {
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_Window *window = SDL_CreateWindow(
            "Learn openGL",
            SDL_WINDOWPOS_CENTERED,
            SDL_WINDOWPOS_CENTERED,
            WIDTH,
            HEIGHT,
            SDL_WINDOW_OPENGL);

    if (!window) {
        printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    SDL_GL_CreateContext(window);
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        printf("Failed to load GL: %s\n", SDL_GetError());
        return -1;
    }

    puts("OpenGL Loaded");
    printf("Vendor:  %s\n", glGetString(GL_VENDOR));
    printf("Renderer:  %s\n", glGetString(GL_RENDERER));
    printf("Version:  %s\n", glGetString(GL_VERSION));
    // stuff below causes seg fault for some reason
    //int numAttributes;
    //glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &numAttributes);
    //printf("Maximum number of vertex attribs supports: %s\n", numAttributes);

    glViewport(0, 0, WIDTH, HEIGHT);

    // wireframe rendering
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    float vertices[] = {
        // positions        //colors
        0.5f, -0.5f, 0.0f,    1.0f, 0.0f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   // bottom left
        0.0f, 0.5f, 0.0f,  0.0f, 0.0f, 1.0f  // top
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // xyz
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int shader_default = RenderShaderCreate("./shaders/default.vert", "./shaders/default.frag");
    glUseProgram(shader_default);

    next_time = SDL_GetTicks() + TICK_INTERVAL;
    while (!should_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                default:
                    break;
            }
        }

        // render begin
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Possibly the 2 ugliest if statements in the world
        // if (greenValue > 1) {
        //     greenValueInc = false;
        // } else if (greenValue < 0) {
        //     greenValueInc = true;
        // }

        // if (greenValueInc) {
        //     greenValue += 0.005;
        // } else {
        //     greenValue -= 0.005;
        // }

        // printf("Green Value: %f\n", greenValue);
        // int vertexColorLocation = glGetUniformLocation(shader_default, "ourColor");

        // if (vertexColorLocation == -1) {
        //     printf("No uniform location could be found for vertexColorLocation.\n");
        //     return -1;
        // }

        // // we use 4f to get the 4 floats of the uniform value
        // glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);

        // render end; swaps buffers aka renders changes
        SDL_GL_SwapWindow(window);

        SDL_Delay(time_left());
        next_time += TICK_INTERVAL;
    }

    SDL_DestroyWindow(window);

    SDL_Quit();

    return 0;
}
