#include <SDL2/SDL_hints.h>
#include <SDL2/SDL_mouse.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
#include <linmath.h>

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

const int WIDTH = 800;
const int HEIGHT = 600;

float lastX = WIDTH / 2, lastY = HEIGHT / 2;

bool should_quit = false;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

float fov = 45.0f;

float yaw = -90.0f;
float pitch = 0;

void input_handler(SDL_KeyboardEvent *e) {
    const float cameraSpeed = 0.05f;

    if (e->type == SDL_KEYDOWN) {
        if (e->keysym.sym == SDLK_w) {
            vec3 result;
            vec3_scale(result, cameraFront, cameraSpeed);
            vec3_add(cameraPos, cameraPos, result);
        } else if (e->keysym.sym == SDLK_s) {
            vec3 result;
            vec3_scale(result, cameraFront, cameraSpeed);
            vec3_sub(cameraPos, cameraPos, result);
        }

        if (e->keysym.sym == SDLK_a) {
            vec3 result;
            vec3_mul_cross(result, cameraFront, cameraUp);
            vec3 normalized;
            vec3_norm(normalized, result);
            vec3 final;
            vec3_scale(final, normalized, cameraSpeed);
            vec3_sub(cameraPos, cameraPos, final);
        } else if (e->keysym.sym == SDLK_d) {
            vec3 result;
            vec3_mul_cross(result, cameraFront, cameraUp);
            vec3 normalized;
            vec3_norm(normalized, result);
            vec3 final;
            vec3_scale(final, normalized, cameraSpeed);
            vec3_add(cameraPos, cameraPos, final);
        }
    } else if (e->state == SDL_RELEASED) {
        if (e->keysym.sym == SDLK_w) {
            return;
        } else if (e->keysym.sym == SDLK_s) {
            return;
        }

        if (e->keysym.sym == SDLK_a) {
            return;
        } else if (e->keysym.sym == SDLK_d) {
            return;
        }
    }
}

bool firstMouse = false;
void mouse_callback(SDL_Event *event) {
    float xPos = event->motion.x;
    float yPos = event->motion.y;

    if (firstMouse) {
        lastX = xPos;
        lastY = yPos;
        firstMouse = false;
    }

    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;

    const float sensitivity = 0.1f;
    yaw += xOffset * sensitivity;
    pitch += yOffset * sensitivity;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    vec3 direction;
    direction[0] = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction[1] = sin(pitch * (M_PI / 180));
    direction[2] = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    vec3_norm(cameraFront, direction);

    printf("Mouse Position: (%f, %f)\n", xPos, yPos);
}

void scroll_callback(SDL_Event *event) {
    fov -= (float)event->wheel.y;
    if (fov < 1.0f)
        fov = 1.0f;
    if (fov > 45.0f)
        fov = 45.0f;
}

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

    glEnable(GL_DEPTH_TEST);

    SDL_SetRelativeMouseMode(false);
    SDL_ShowCursor(false);

    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    unsigned int VAO;
    glGenVertexArrays(1, &VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    unsigned int EBO;
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // xyz
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // tex coords
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    unsigned int shader_default = RenderShaderCreate("./shaders/default.vert", "./shaders/default.frag");

    unsigned int texture1, texture2;
    // texture 1
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // or extends to give texture border
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float[]){1, 1, 0, 1});

    // textures scaled lower are more pixilated and scaled higher are kinda blurry instead
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, numColorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/container.png", &width, &height, &numColorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture container.png\n");
    }

    // we can free the image data after making the TexImage
    stbi_image_free(data);

    // texture 2
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    // or extends to give texture border
    // glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, (float[]){1, 1, 0, 1});

    // textures scaled lower are more pixilated and scaled higher are kinda blurry instead
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("./assets/awesomeface.png", &width, &height, &numColorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture: awesomeface.png");
    }

    stbi_image_free(data);

    glUseProgram(shader_default);

    glUniform1i(glGetUniformLocation(shader_default, "texture1"), 0);
    glUniform1i(glGetUniformLocation(shader_default, "texture2"), 1);

    // vec4 vec = {1.0f, 0.0f, 0.0f, 1.0f};
    // mat4x4 trans;
    // mat4x4_identity(trans);
    // mat4x4_translate(trans, 1.0f, 1.0f, 0.0f);
    // vec4 result;
    // mat4x4_mul_vec4(result, trans, vec);
    // for (int i = 0; i < 4; i++) {
    //     printf("%f ", result[i]);
    // }
    // printf("\n");

    // mat4x4 trans;
    // mat4x4_identity(trans);
    // mat4x4 rot;
    // mat4x4_rotate(rot, trans, 0.0f, 0.0f, 1.0f, (90 * (M_PI / 180)));
    // mat4x4 result;
    // mat4x4_scale_aniso(result, rot, 0.5f, 0.5f, 0.5f);

    float cubePositions[] = {
        0.0f, 0.0f, 0.0f,
        2.0f, 5.0f, -15.0f,
        -1.5f, -2.2f, -2.5f,
        -3.8f, -2.0f, -12.3f,
        2.4f, -0.4f, -3.5f,
        -1.7f, 3.0f, -7.5f,
        1.3f, -2.0f, -2.5f,
        1.5f, 2.0f, -2.5f,
        1.5f, 0.2f, -1.5f,
        -1.3f, 1.0f, -1.5f
    };

    float rotTimer = 0.0f;

    float xPos = 0.0;
    float yPos = 0.0;

    next_time = SDL_GetTicks() + TICK_INTERVAL;
    while (!should_quit) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    should_quit = true;
                    break;
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    input_handler(&event.key);
                    break;
                case SDL_MOUSEMOTION:
                    mouse_callback(&event);
                    break;
                case SDL_MOUSEWHEEL:
                    scroll_callback(&event);
                    break;
                default:
                    break;
            }
        }

        rotTimer++;

        // render begin
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        glUseProgram(shader_default);

        mat4x4 projection;
        mat4x4_perspective(projection, fov * (M_PI / 180), WIDTH/HEIGHT, 0.1f, 100.0f);

        unsigned int projectionLoc = glGetUniformLocation(shader_default, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const GLfloat*)projection);

        mat4x4 view;
        vec3 cameraOrigin;
        vec3_add(cameraOrigin, cameraPos, cameraFront);
        mat4x4_look_at(view, cameraPos, cameraOrigin, cameraUp);

        unsigned int viewLoc = glGetUniformLocation(shader_default, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);

        glBindVertexArray(VAO);
        for (int i = 0; i < 10; i++) {

            mat4x4 model;
            mat4x4_identity(model);
            mat4x4_translate(model, cubePositions[i * 3], cubePositions[i * 3 + 1], cubePositions[i * 3 + 2]);
            float angle = 20.0f * i + 15.0f;
            mat4x4 result;
            mat4x4_rotate(result, model, 1.0f, 0.3f, 0.5f, (rotTimer / 100) * angle * (M_PI / 180));

            unsigned int modelLoc = glGetUniformLocation(shader_default, "model");
            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)result);

            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
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
