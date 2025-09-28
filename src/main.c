#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "assert.h"
#include "math.h"

#include "engine/engine.h"
#include "engine/math.h"
#include "engine/shader_uniform.h"
#include "engine/parse_shader.h"

#include "texture.h"
#include "material.h"
#include "camera.h"
#include "mesh.h"

#include "stdio.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int main(void) {

    
    // Initialize the window to the starting size and set the header.
    if (!Engine_initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Delta Render"));
    InitShaders();
    InitTextures();

    // Add termination functions to be executed at the end of the program.
    //Engine_add_termination_function(DereferenceFonts);
    Engine_add_termination_function(DereferenceTextures);
    Engine_add_termination_function(DereferenceShaders);

    // Load Missing Textures:
    CreateTexture("./assets/defaultAssets/defaultTexture.png", "defaultTexture", GL_RGBA, false, false, false, GL_LINEAR);
    CreateTexture("./assets/defaultAssets/test.png", "testTexture", GL_RGBA, false, false, false, GL_LINEAR);
    CreateTexture("./assets/defaultAssets/missingTexture.png", "MissingTexture", GL_RGBA, false, false, false, GL_NEAREST);
    CreateTexture("./assets/defaultAssets/missingNormal.png", "missingNormal", GL_RGB, false, false, false, GL_LINEAR);

    // Load Textures:
    CreateTexture("./assets/textures/Mushroom.png", "mushroomBody", GL_RGBA, false, false, false, GL_LINEAR);
    CreateTexture("./assets/textures/MushroomGlow.png", "mushroomGlow", GL_RGB, false, false, false, GL_LINEAR);
    CreateTexture("./assets/defaultAssets/missingSpecular.png", "Specular", GL_RGB, false, false, false, GL_LINEAR);


    // Create shaders:
    GLuint DefaultShaderProgram;
    GLuint NormalShaderProgram;

    Shader_CompileProgram(DefaultShaderProgram, 
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER}, 
        { .path = "./assets/shaders/default.frag", .type = GL_FRAGMENT_SHADER }
    );

    Shader_CompileProgram(NormalShaderProgram,
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER },
        { .path = "./assets/shaders/normal_color.frag", .type = GL_FRAGMENT_SHADER }
    );

    

    // Create Materials:
    Material* Mat0 = Material_create(Shader_create(DefaultShaderProgram, "DefaultShader"), 4, GL_BACK, GL_LESS);
    Material* Mat1 = Material_create(Shader_create(DefaultShaderProgram, "DefaultShader"), 4, GL_BACK, GL_LESS);
    Material* Normals = Material_create(Shader_create(NormalShaderProgram, "NormalShader"), 0, GL_BACK, GL_LESS);

    // Set Material Textures:

    SetTextureFromAlias(Mat0, "Specular", 0);
    SetTextureFromAlias(Mat0, "defaultTexture", 1);
    SetTextureFromAlias(Mat0, "missingNormal", 2);
    SetTextureFromAlias(Mat0, "Specular", 3);

    SetTextureFromAlias(Mat1, "mushroomBody", 0);
    SetTextureFromAlias(Mat1, "mushroomGlow", 1);
    SetTextureFromAlias(Mat1, "missingNormal", 2);
    SetTextureFromAlias(Mat1, "Specular", 3);

    // Load Font:
    //Font* defautFont = CreateFont("./assets/defaultAssets/IBMPlexMono-Regular.ttf", "IBM", DefaultTextMaterial, 22.0f);

    //TextRender* testText = new TextRender();
    //SetFont(testText, "IBM", defautFont);

    Camera* mainCamera = Object_Camera_create();
    StaticMesh* mesh = Object_StaticMesh_create("./assets/meshes/mesh0.bin", NULL);
    StaticMesh* lightVis = Object_StaticMesh_create("./assets/meshes/mesh1.bin", NULL);
    
    assert(mesh && lightVis);

    vec3 cameraDefaultPos = { 0.0f, -1.0f, -1.0f };
    mat4_translate(cameraDefaultPos, mainCamera->Transform);

    Object_StaticMesh_set_Material(mesh, 0, Normals);
    Object_StaticMesh_set_Material(lightVis, 0, Mat0);

    int x = 0;
    int y = 0;

    GLfloat time = 0.0f;
    
    GLuint activeLights = 2;

    vec3 lightPos = { 0.0f, 5.0f, 0.0f };
    vec3 lightDir = { 1.0f, 0.0f, 0.0f };
    vec3 lightColor = { 2.0f, 2.0f, 2.0f };
    vec3 AmbientColor = { 0.2f, 0.2f, 0.2f };
    
    Engine_set_ambient_color(AmbientColor[0], AmbientColor[1], AmbientColor[2]);
    
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

    bool captureCursor = true;
    SetCaptureCursor(captureCursor);

    while (Engine_execute_tick()) {

        if (IsKeyPressed(GLFW_KEY_TAB)) {
            captureCursor = !captureCursor;
            SetCaptureCursor(captureCursor);
        }

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

        vec3 meshTranslate = { (float)x, (float)y, 0.0f };
        vec3 meshScale = { 0.5f, 0.5f, 0.5f };
        
        mat4_translate(meshTranslate, mesh->Transform);
        mat4 scale;

        mat4_scale(meshScale, scale);
        mat4_multiply(scale, mesh->Transform, mesh->Transform);

        lightPos[0] = sinf(Time() * 1.3f) * 2.0f;
        lightPos[1] = (sinf(Time() * 0.7f) * 0.2f) + 1.0f;
        lightPos[2] = cosf(Time() * 1.3f) * 2.0f;

        mat4_lookat(lightPos, V3_ZERO, V3_UP, lightVis->Transform);
        mat4_inverse(lightVis->Transform, lightVis->Transform);

        UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, &lightPos);

        mainCamera->Tick(mainCamera, DeltaTime());

        vec3 cameraPos;
        vec3 cameraDir = { 0.0f, 0.0f, 1.0f };

        mat4_get_translation(mainCamera->Transform, cameraPos);
        vec3_rotate(cameraDir, mainCamera->Rotation, cameraDir);

 
        UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, lightPos);

        UniformBuffer_set_Global("FrameData", "u_time", &time);
        UniformBuffer_set_Global("FrameData", "u_view", mainCamera->ViewMatrix);
        UniformBuffer_set_Global("FrameData", "u_position", cameraPos);
        UniformBuffer_set_Global("FrameData", "u_direction", cameraDir);

        UniformBuffer_update_all();

        mesh->Draw(mesh);
        lightVis->Draw(lightVis);
       
        //SetText(testText,"This is a test.", x, y, static_cast<float>(WindowWidth()), static_cast<float>(WindowHeight()), 2.0f);
        //DrawTextMesh(testText, mainCamera, AspectRatio());
    }
    
    mainCamera->Destroy(mainCamera);
    mesh->Destroy(mesh);
    lightVis->Destroy(lightVis);

    Material_destroy(&Mat0); 
    Material_destroy(&Mat1); 
    
    Engine_terminate();
    return 0;
}

