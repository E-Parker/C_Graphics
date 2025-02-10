#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>

#include "glUtilities.h"
#include "vectorMath.h"
#include "texture.h"
#include "material.h"
#include "camera.h"
#include "mesh.h"
#include "font.h"
#include "gl_types.h"
#include "gl_shader_uniform.h"

constexpr int SCREEN_WIDTH = 1280;
constexpr int SCREEN_HEIGHT = 720;


int main(void) {

    // Initialize the window to the starting size and set the header.
    GLFWwindow* window = Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Delta Render");
    InitShaders();
    InitTextures();

    // Add termination functions to be executed at the end of the program.
    glUtilAddTerminationFunction(DereferenceFonts);
    glUtilAddTerminationFunction(DereferenceTextures);
    glUtilAddTerminationFunction(DereferenceShaders);
    glUtilAddTerminationFunction(glfwTerminate);

    // Load Missing Textures:
    CreateTexture("./assets/defaultAssets/defaultTexture.png", "defaultTexture", GL_RGBA, false, false, false, GL_LINEAR);
    CreateTexture("./assets/defaultAssets/missingTexture.png", "MissingTexture", GL_RGBA, false, false, false, GL_NEAREST);
    CreateTexture("./assets/defaultAssets/missingNormal.png", "missingNormal", GL_RGB, false, false, false, GL_LINEAR);

    // Load Textures:
    CreateTexture("./assets/textures/Mushroom.png", "mushroomBody", GL_RGBA, false, false, false, GL_LINEAR);
    CreateTexture("./assets/textures/MushroomGlow.png", "mushroomGlow", GL_RGB, false, false, false, GL_LINEAR);
    CreateTexture("./assets/defaultAssets/missingSpecular.png", "Specular", GL_RGB, false, false, false, GL_LINEAR);

    // Load Materials:
    Material* DefaultTextMaterial = new Material("./assets/shaders/defaultText.vert", "./assets/shaders/defaultText.frag", 1, GL_BACK, GL_ALWAYS);
    //Material* NormalMaterial = new Material("./assets/shaders/default.vert", "./assets/shaders/normal_color.frag", 0, GL_BACK, GL_LESS);
    //Material* TChoodColorMaterial = new Material("./assets/shaders/default.vert", "./assets/shaders/tcoord_color.frag", 0, GL_BACK, GL_LESS);
    Material* Mat0 = new Material("./assets/shaders/default.vert", "./assets/shaders/default.frag", 4, GL_BACK, GL_LESS);
    Material* Mat1 = new Material("./assets/shaders/default.vert", "./assets/shaders/default.frag", 4, GL_BACK, GL_LESS);

    // Set Material Textures:
    SetTextureFromAlias(Mat1, "Specular", 0);
    SetTextureFromAlias(Mat1, "Specular", 1);
    SetTextureFromAlias(Mat1, "missingNormal", 2);
    SetTextureFromAlias(Mat1, "Specular", 3);

    SetTextureFromAlias(Mat0, "mushroomBody", 0);
    SetTextureFromAlias(Mat0, "defaultTexture", 1);
    SetTextureFromAlias(Mat0, "missingNormal", 2);
    SetTextureFromAlias(Mat0, "Specular", 3);

    // Load Font:
    //Font* defautFont = CreateFont("./assets/defaultAssets/IBMPlexMono-Regular.ttf", "IBM", DefaultTextMaterial, 22.0f);

    // There is a known issue with fonts right now. Something is getting deleted when it isn't supposed to. Will run fine on a first pass.  
    //TextRender* testText = new TextRender();
    //SetFont(testText, "IBM", defautFont);

    StaticMesh* mesh = CreateStaticMeshFromWavefront("./assets/meshes/mushroom.obj");
    StaticMesh* lightVis = CreateStaticMeshFromWavefront("./assets/meshes/icosphere.obj");
    mesh->SetMaterial(Mat0, 0);
    lightVis->SetMaterial(Mat1, 0);

    Camera* mainCamera = new Camera(NoClipCameraUpdate);
    *GET_ASSET_TRANSFORM(mainCamera) = Translate(0.0f, -1.0f, -1.0f);

    Shader* testShader = Shader_create(Mat0->Program, "TestShader");

    //Uniform* mvpUniform;
    //Uniform_set_data(mvpUniform, transform);

    int x = 0;
    int y = 0;

    GLfloat time = 0.0f;
    
    GLuint activeLights = 2;

    vec3 lightPos { 0.0f, 5.0f, 0.0f };
    vec3 lightDir { 1.0f, 0.0f, 0.0f };
    vec3 lightColor { 2.0f, 2.0f, 2.0f };
    vec3 AmbientColor { 0.5f, 0.5f, 0.5f };
    
    glUtilSetAmbientColor(AmbientColor[0], AmbientColor[1], AmbientColor[2]);
    
    float lightRadius = 15.0f;

    UniformBuffer_set_Global("LightData", "u_activeLights", &activeLights);
    UniformBuffer_set_Global("LightData", "u_ambientColor", &AmbientColor);

    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, &lightPos);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "direction", 0, &lightDir);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "color", 0, &lightColor);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "attenuation", 0, &lightRadius);

    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 1, &lightPos);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "direction", 1, &lightDir);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "color", 1, &lightColor);
    UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "attenuation", 1, &lightRadius);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window)) {
        
        // FRAME STARTS HERE
        glUtilInitializeFrame(window);
        time = (GLfloat)Time();

        if (IsKeyPressed(GLFW_KEY_UP)) {
            y++;
        }
        
        if (IsKeyPressed(GLFW_KEY_DOWN)) {
            y--;
        }
        
        if (IsKeyPressed(GLFW_KEY_RIGHT)) {
            x++;
        }
        
        if (IsKeyPressed(GLFW_KEY_LEFT)) {
            x--;
        }

        *GET_ASSET_TRANSFORM(mesh) = Translate(static_cast<float>(x), static_cast<float>(y), 0.0f);
        
        lightPos[0] = sinf(Time() * 1.3f) * 2.0f;
        lightPos[1] = (sinf(Time() * 0.7f) * 0.2f) + 1.0f;
        lightPos[2] = cosf(Time() * 1.3f) * 2.0f;
        *GET_ASSET_TRANSFORM(lightVis) = Translate(lightPos[0], lightPos[1], lightPos[2]);        
        UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, &lightPos);

        mainCamera->Update(mainCamera, DeltaTime(), AspectRatio());
        float16 cameraView = ToFloat16(mainCamera->ViewMatrix);
        float cameraPos[3] = { mainCamera->Transform.m12, mainCamera->Transform.m13, mainCamera->Transform.m14 };

        float* cameraDir = ToFloat3(Forward(ToMatrix(mainCamera->Rotation))).v;

        UniformBuffer* buffer = UniformBuffer_get_self("FrameData");
        void* data = UniformBuffer_get_shared(buffer);
        //UniformBuffer* buffer1 = UniformBuffer_get_self("LightData");

        //UniformStruct* u_lights;
        //UniformBuffer_get_Struct(buffer1, "u_lights", &u_lights);

        UniformBuffer_set_Global("FrameData", "u_time", &time);
        UniformBuffer_set_Global("FrameData", "u_view", &cameraView.v);
        UniformBuffer_set_Global("FrameData", "u_position", &cameraPos);
        UniformBuffer_set_Global("FrameData", "u_direction", cameraDir);

        UniformBuffer_update_all();

        //Shader_use(testShader);

        mesh->Draw();
        lightVis->Draw();
       
        //SetText(testText,"This is a test.", x, y, static_cast<float>(WindowWidth()), static_cast<float>(WindowHeight()), 2.0f);
        //DrawTextMesh(testText, mainCamera, AspectRatio());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll and process events */
        glUtilPollEvents();
    }

    delete mainCamera;
    delete mesh;
    delete DefaultTextMaterial;
    delete Mat0;
    //delete NormalMaterial;
    //delete TChoodColorMaterial;
    //delete testText;

    //Shader_destroy(&testShader);
    glUtilTerminate();
    return 0;
}

