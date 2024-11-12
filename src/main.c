#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <linmath.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

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

bool should_quit = false;

vec3 cameraPos = {0.0f, 0.0f, 3.0f};
vec3 cameraFront = {0.0f, 0.0f, -1.0f};
vec3 cameraUp = {0.0f, 1.0f, 0.0f};

float fov = 45.0f;

float yaw = -90.0f;
float pitch = 0;

void input_handler() {
    const float cameraSpeed = 0.05f;

    const Uint8 *currentKeyStates = SDL_GetKeyboardState(NULL);
    if (currentKeyStates[SDL_SCANCODE_W]) {
        vec3 result;
        vec3_scale(result, cameraFront, cameraSpeed);
        vec3_add(cameraPos, cameraPos, result);
    } else if (currentKeyStates[SDL_SCANCODE_S]) {
        vec3 result;
        vec3_scale(result, cameraFront, cameraSpeed);
        vec3_sub(cameraPos, cameraPos, result);
    }

    if (currentKeyStates[SDL_SCANCODE_A]) {
        vec3 result;
        vec3_mul_cross(result, cameraFront, cameraUp);
        vec3 normalized;
        vec3_norm(normalized, result);
        vec3 final;
        vec3_scale(final, normalized, cameraSpeed);
        vec3_sub(cameraPos, cameraPos, final);
    } else if (currentKeyStates[SDL_SCANCODE_D]) {
        vec3 result;
        vec3_mul_cross(result, cameraFront, cameraUp);
        vec3 normalized;
        vec3_norm(normalized, result);
        vec3 final;
        vec3_scale(final, normalized, cameraSpeed);
        vec3_add(cameraPos, cameraPos, final);
    }

    const float sensitivity = 2.0f;
    if (currentKeyStates[SDL_SCANCODE_LEFT]) {
        yaw -= sensitivity;

    } else if (currentKeyStates[SDL_SCANCODE_RIGHT]) {
        yaw += sensitivity;
    }

    if (currentKeyStates[SDL_SCANCODE_UP]) {
        pitch += sensitivity;

        if (pitch > 89.0f)
            pitch = 89.0f;
    } else if (currentKeyStates[SDL_SCANCODE_DOWN]) {
        pitch -= sensitivity;

        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    vec3 direction;
    direction[0] = cos(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    direction[1] = sin(pitch * (M_PI / 180));
    direction[2] = sin(yaw * (M_PI / 180)) * cos(pitch * (M_PI / 180));
    vec3_norm(cameraFront, direction);
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

    SDL_ShowCursor(false);

    float vertices[] = {
        // positions          // normals           // texture coords
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
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
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normal
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    unsigned int shader_default = RenderShaderCreate("./shaders/default.vert", "./shaders/default.frag");
    unsigned int shader_light = RenderShaderCreate("./shaders/light.vert", "./shaders/light.frag");

    float rotTimer = 0.0f;

    glUseProgram(shader_default);

    unsigned int diffuse = glGetUniformLocation(shader_default, "material.diffuse");
    glUniform1i(diffuse, 0);

    unsigned int specular = glGetUniformLocation(shader_default, "material.specular");
    glUniform1i(specular, 1);

    unsigned int texture;

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, colorChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char *data = stbi_load("./assets/container2.png", &width, &height, &colorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture container2.png\n");
    }

    stbi_image_free(data);

    unsigned int specularMap;

    glGenTextures(1, &specularMap);
    glBindTexture(GL_TEXTURE_2D, specularMap);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    data = stbi_load("./assets/container2_specular.png", &width, &height, &colorChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        printf("Failed to load texture container2_specular.png\n");
    }

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

        input_handler();

        rotTimer++;

        // render begin
        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader_default);

        vec3 lightCoords = {sin(rotTimer / 100), 1.0f, cos(rotTimer / 100)};
        unsigned int lightPos = glGetUniformLocation(shader_default, "light.position");
        glUniform3f(lightPos, lightCoords[0], lightCoords[1], lightCoords[2]);

        unsigned int shininess = glGetUniformLocation(shader_default, "material.shininess");
        glUniform1f(shininess, 64.0f);

        vec3 lightColor;
        lightColor[0] = 1.0f;
        lightColor[1] = 1.0f;
        lightColor[2] = 1.0f;

        vec3 diffuseColor;
        vec3_scale(diffuseColor, lightColor, 0.5f);
        vec3 ambientColor;
        vec3_scale(ambientColor, diffuseColor, 0.2f);

        unsigned int ambientLight = glGetUniformLocation(shader_default, "light.ambient");
        glUniform3f(ambientLight, ambientColor[0], ambientColor[1], ambientColor[2]);

        unsigned int diffuseLight = glGetUniformLocation(shader_default, "light.diffuse");
        glUniform3f(diffuseLight, diffuseColor[0], diffuseColor[1], diffuseColor[2]);

        unsigned int specularLight = glGetUniformLocation(shader_default, "light.specular");
        glUniform3f(specularLight, lightColor[0], lightColor[1], lightColor[2]);

        unsigned int viewPos = glGetUniformLocation(shader_default, "viewPos");
        glUniform3f(viewPos, cameraPos[0], cameraPos[1], cameraPos[2]);

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

        mat4x4 model;
        mat4x4_identity(model);

        unsigned int modelLoc = glGetUniformLocation(shader_default, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        glUseProgram(shader_light);

        mat4x4 lightProjection;
        mat4x4_perspective(lightProjection, fov * (M_PI / 180), WIDTH/HEIGHT, 0.1f, 100.0f);

        unsigned int lightProjectionLoc = glGetUniformLocation(shader_light, "projection");
        glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, (const GLfloat*)lightProjection);

        mat4x4 lightView;
        vec3 lightCameraOrigin;
        vec3_add(lightCameraOrigin, cameraPos, cameraFront);
        mat4x4_look_at(lightView, cameraPos, cameraOrigin, cameraUp);

        unsigned int lightViewLoc = glGetUniformLocation(shader_light, "view");
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, (const GLfloat*)lightView);

        mat4x4 lightModel;
        mat4x4_identity(lightModel);
        mat4x4_translate(lightModel, lightCoords[0], lightCoords[1], lightCoords[2]);
        // for whatever reason only the aniso func works
        mat4x4_scale_aniso(lightModel, lightModel, 0.2f, 0.2f, 0.2f);

        unsigned int lightModelLoc = glGetUniformLocation(shader_light, "model");
        glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, (const GLfloat*)lightModel);

        unsigned int lampColor = glGetUniformLocation(shader_light, "lampColor");
        glUniform3f(lampColor, lightColor[0], lightColor[1], lightColor[2]);

        glBindVertexArray(lightVAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);

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
