#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <assert.h>

#include "linmath.h"
#include "shader.h"
#include "model.h"

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

const int WIDTH = 1920;
const int HEIGHT = 1080;

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

    glViewport(0, 0, WIDTH, HEIGHT);

    // wireframe rendering
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    glEnable(GL_DEPTH_TEST);

    SDL_ShowCursor(false);

    unsigned int shader_default = RenderShaderCreate("./shaders/default.vert", "./shaders/default.frag");
    unsigned int shader_light = RenderShaderCreate("./shaders/light.vert", "./shaders/light.frag");

    printf("Loading model...\n");
    Model model = loadModel("./assets/backpack/backpack.obj");
    printf("model loaded.\n");

    glUseProgram(shader_default);

    unsigned int VBO;
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    unsigned int lightVAO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    float rotTimer = 0.0f;

    // directional lighting
    unsigned int lightDirection = glGetUniformLocation(shader_default, "dirLight.direction");
    glUniform3f(lightDirection, -0.2f, -1.0f, -0.3f);

    float pointLightPositions[] = {
         0.7f,  0.2f,  2.0f,
         2.3f, -3.3f, -4.0f,
        -4.0f,  2.0f, -12.0f,
         0.0f,  0.0f, -3.0f
    };

    unsigned int pointLightPos0 = glGetUniformLocation(shader_default, "pointLights[0].position");
    glUniform3f(pointLightPos0, pointLightPositions[0], pointLightPositions[1], pointLightPositions[2]);

    unsigned int pointLightAmbient0 = glGetUniformLocation(shader_default, "pointLights[0].ambient");
    glUniform3f(pointLightAmbient0, 0.05f, 0.05f, 0.05f);

    unsigned int pointLightDiffuse0 = glGetUniformLocation(shader_default, "pointLights[0].diffuse");
    glUniform3f(pointLightDiffuse0, 0.8f, 0.8f, 0.8f);

    unsigned int pointLightSpecular0 = glGetUniformLocation(shader_default, "pointLights[0].specular");
    glUniform3f(pointLightSpecular0, 1.0f, 1.0f, 1.0f);

    unsigned int pointLightConstant0 = glGetUniformLocation(shader_default, "pointLights[0].constant");
    glUniform1f(pointLightConstant0, 1.0f);

    unsigned int pointLightLinear0 = glGetUniformLocation(shader_default, "pointLights[0].linear");
    glUniform1f(pointLightLinear0, 0.09f);

    unsigned int pointLightQuadratic0 = glGetUniformLocation(shader_default, "pointLights[0].quadratic");
    glUniform1f(pointLightQuadratic0, 0.032f);


    unsigned int pointLightPos1 = glGetUniformLocation(shader_default, "pointLights[1].position");
    glUniform3f(pointLightPos1, pointLightPositions[3], pointLightPositions[4], pointLightPositions[5]);

    unsigned int pointLightAmbient1 = glGetUniformLocation(shader_default, "pointLights[1].ambient");
    glUniform3f(pointLightAmbient1, 0.05f, 0.05f, 0.05f);

    unsigned int pointLightDiffuse1 = glGetUniformLocation(shader_default, "pointLights[1].diffuse");
    glUniform3f(pointLightDiffuse1, 0.8f, 0.8f, 0.8f);

    unsigned int pointLightSpecular1 = glGetUniformLocation(shader_default, "pointLights[1].specular");
    glUniform3f(pointLightSpecular1, 1.0f, 1.0f, 1.0f);

    unsigned int pointLightConstant1 = glGetUniformLocation(shader_default, "pointLights[1].constant");
    glUniform1f(pointLightConstant1, 1.0f);

    unsigned int pointLightLinear1 = glGetUniformLocation(shader_default, "pointLights[1].linear");
    glUniform1f(pointLightLinear1, 0.09f);

    unsigned int pointLightQuadratic1 = glGetUniformLocation(shader_default, "pointLights[1].quadratic");
    glUniform1f(pointLightQuadratic1, 0.032f);


    unsigned int pointLightPos2 = glGetUniformLocation(shader_default, "pointLights[2].position");
    glUniform3f(pointLightPos2, pointLightPositions[6], pointLightPositions[7], pointLightPositions[8]);

    unsigned int pointLightAmbient2 = glGetUniformLocation(shader_default, "pointLights[2].ambient");
    glUniform3f(pointLightAmbient2, 0.05f, 0.05f, 0.05f);

    unsigned int pointLightDiffuse2 = glGetUniformLocation(shader_default, "pointLights[2].diffuse");
    glUniform3f(pointLightDiffuse2, 0.8f, 0.8f, 0.8f);

    unsigned int pointLightSpecular2 = glGetUniformLocation(shader_default, "pointLights[2].specular");
    glUniform3f(pointLightSpecular2, 1.0f, 1.0f, 1.0f);

    unsigned int pointLightConstant2 = glGetUniformLocation(shader_default, "pointLights[2].constant");
    glUniform1f(pointLightConstant2, 1.0f);

    unsigned int pointLightLinear2 = glGetUniformLocation(shader_default, "pointLights[2].linear");
    glUniform1f(pointLightLinear2, 0.09f);

    unsigned int pointLightQuadratic2 = glGetUniformLocation(shader_default, "pointLights[2].quadratic");
    glUniform1f(pointLightQuadratic2, 0.032f);


    unsigned int pointLightPos3 = glGetUniformLocation(shader_default, "pointLights[3].position");
    glUniform3f(pointLightPos3, pointLightPositions[9], pointLightPositions[10], pointLightPositions[11]);

    unsigned int pointLightAmbient3 = glGetUniformLocation(shader_default, "pointLights[3].ambient");
    glUniform3f(pointLightAmbient3, 0.05f, 0.05f, 0.05f);

    unsigned int pointLightDiffuse3 = glGetUniformLocation(shader_default, "pointLights[3].diffuse");
    glUniform3f(pointLightDiffuse3, 0.8f, 0.8f, 0.8f);

    unsigned int pointLightSpecular3 = glGetUniformLocation(shader_default, "pointLights[3].specular");
    glUniform3f(pointLightSpecular3, 1.0f, 1.0f, 1.0f);

    unsigned int pointLightConstant3 = glGetUniformLocation(shader_default, "pointLights[3].constant");
    glUniform1f(pointLightConstant3, 1.0f);

    unsigned int pointLightLinear3 = glGetUniformLocation(shader_default, "pointLights[3].linear");
    glUniform1f(pointLightLinear3, 0.09f);

    unsigned int pointLightQuadratic3 = glGetUniformLocation(shader_default, "pointLights[3].quadratic");
    glUniform1f(pointLightQuadratic3, 0.032f);

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

        unsigned int spotLightPos = glGetUniformLocation(shader_default, "spotLight.position");
        glUniform3f(spotLightPos, cameraPos[0], cameraPos[1], cameraPos[2]);

        unsigned int spotLightDir = glGetUniformLocation(shader_default, "spotLight.direction");
        glUniform3f(spotLightDir, cameraFront[0], cameraFront[1], cameraFront[2]);

        unsigned int spotLightAmbient = glGetUniformLocation(shader_default, "spotLight.ambient");
        glUniform3f(spotLightAmbient, 0.0f, 0.0f, 0.0f);

        unsigned int spotLightDiffuse = glGetUniformLocation(shader_default, "spotLight.diffuse");
        glUniform3f(spotLightDiffuse, 1.0f, 1.0f, 1.0f);

        unsigned int spotLightSpecular = glGetUniformLocation(shader_default, "spotLight.specular");
        glUniform3f(spotLightSpecular, 1.0f, 1.0f, 1.0f);

        unsigned int spotLightConstant = glGetUniformLocation(shader_default, "spotLight.constant");
        glUniform1f(spotLightConstant, 1.0f);

        unsigned int spotLightLinear = glGetUniformLocation(shader_default, "spotLight.linear");
        glUniform1f(spotLightLinear, 0.09f);

        unsigned int spotLightQuadratic = glGetUniformLocation(shader_default, "spotLight.quadratic");
        glUniform1f(spotLightQuadratic, 0.032f);

        unsigned int spotLightCutOff = glGetUniformLocation(shader_default, "spotLight.cutOff");
        glUniform1f(spotLightCutOff, cos(12.5f * (M_PI / 180)));

        unsigned int spotLightOuterCutOff = glGetUniformLocation(shader_default, "spotLight.outerCutOff");
        glUniform1f(spotLightOuterCutOff, cos(17.5f * (M_PI / 180)));

        unsigned int shininess = glGetUniformLocation(shader_default, "material.shininess");
        glUniform1f(shininess, 64.0f);


        unsigned int viewPos = glGetUniformLocation(shader_default, "viewPos");
        glUniform3f(viewPos, cameraPos[0], cameraPos[1], cameraPos[2]);

        mat4x4 projection;
        mat4x4_perspective(projection, fov * (M_PI / 180), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        unsigned int projectionLoc = glGetUniformLocation(shader_default, "projection");
        glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, (const GLfloat*)projection);

        mat4x4 view;
        vec3 cameraOrigin;
        vec3_add(cameraOrigin, cameraPos, cameraFront);
        mat4x4_look_at(view, cameraPos, cameraOrigin, cameraUp);

        unsigned int viewLoc = glGetUniformLocation(shader_default, "view");
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, (const GLfloat*)view);

        mat4x4 objectModel;
        mat4x4_identity(objectModel);
        mat4x4_translate(objectModel, 0.0f, 0.0f, -5.0f);

        unsigned int modelLoc = glGetUniformLocation(shader_default, "model");
        glUniformMatrix4fv(modelLoc, 1, GL_FALSE, (const GLfloat*)objectModel);

        assert(glGetError() == GL_NO_ERROR);
        drawModel(&model, shader_default);
        assert(glGetError() == GL_NO_ERROR);

        glUseProgram(shader_light);

        unsigned int lightProjectionLoc = glGetUniformLocation(shader_light, "projection");
        glUniformMatrix4fv(lightProjectionLoc, 1, GL_FALSE, (const GLfloat*)projection);

        unsigned int lightViewLoc = glGetUniformLocation(shader_light, "view");
        glUniformMatrix4fv(lightViewLoc, 1, GL_FALSE, (const GLfloat*)view);

        glBindVertexArray(lightVAO);
        for (int i = 0; i < 4; i++) {

            mat4x4 lightModel;
            mat4x4_identity(lightModel);
            mat4x4_translate(lightModel, pointLightPositions[i * 3], pointLightPositions[i * 3 + 1], pointLightPositions[i * 3 +2]);
            // for whatever reason only the aniso func works
            mat4x4_scale_aniso(lightModel, lightModel, 0.2f, 0.2f, 0.2f);

            unsigned int lightModelLoc = glGetUniformLocation(shader_light, "model");
            glUniformMatrix4fv(lightModelLoc, 1, GL_FALSE, (const GLfloat*)lightModel);

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
