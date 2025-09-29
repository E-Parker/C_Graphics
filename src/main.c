#include "glad/glad.h"
#include "GLFW/glfw3.h"

#include "stdio.h"
#include "assert.h"
#include "math.h"

#include "engine_core/engine_types.h"
#include "engine_core/engine_shader.h"

#include "engine/object.h"
#include "engine/object/camera.h"
#include "engine/object/mesh.h"
#include "engine/engine.h"


int main(void) {

    
    // Initialize the window to the starting size and set the header.
    if (!Engine_initialize(320, 200, "Delta Render"));
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
    Shader_create("DefaultShader",
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER}, 
        { .path = "./assets/shaders/default.frag", .type = GL_FRAGMENT_SHADER }
    );

    Shader_create("DitherShader",
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER },
        { .path = "./assets/shaders/dithered_alpha.frag", .type = GL_FRAGMENT_SHADER }
    );

    Shader_create("TCoordShader",
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER },
        { .path = "./assets/shaders/tcoord_color.frag", .type = GL_FRAGMENT_SHADER }
    );

    Shader_create("NormalShader",
        { .path = "./assets/shaders/default.vert", .type = GL_VERTEX_SHADER },
        { .path = "./assets/shaders/normal_color.frag", .type = GL_FRAGMENT_SHADER }
    );

    // Create Materials:
    Material* Mat0 = Material_create(Shader_get("DefaultShader"), 4, GL_BACK, GL_LESS);
    Material* Mat1 = Material_create(Shader_get("DefaultShader"), 4, GL_BACK, GL_LESS);
    Material* Dither = Material_create(Shader_get("DitherShader"), 1, GL_BACK, GL_LESS);
    Material* TCoords = Material_create(Shader_get("TCoordShader"), 0, GL_BACK, GL_LESS);
    Material* Normals = Material_create(Shader_get("NormalShader"), 0, GL_BACK, GL_LESS);

    // Set Material Textures:
    SetTextureFromAlias(Mat0, "Specular", 0);
    SetTextureFromAlias(Mat0, "defaultTexture", 1);
    SetTextureFromAlias(Mat0, "missingNormal", 2);
    SetTextureFromAlias(Mat0, "Specular", 3);

    SetTextureFromAlias(Mat1, "mushroomBody", 0);
    SetTextureFromAlias(Mat1, "mushroomGlow", 1);
    SetTextureFromAlias(Mat1, "missingNormal", 2);
    SetTextureFromAlias(Mat1, "Specular", 3);

    SetTextureFromAlias(Dither, "mushroomBody", 0);


    Camera* mainCamera = Object_Camera_create();
    StaticMesh* groundMesh = Object_StaticMesh_create("./assets/meshes/ground.bin", NULL);
    StaticMesh* mesh0 = Object_StaticMesh_create("./assets/meshes/mushroom.bin", NULL);
    StaticMesh* mesh1 = Object_StaticMesh_create("./assets/meshes/mushroom.bin", NULL);
    StaticMesh* mesh2 = Object_StaticMesh_create("./assets/meshes/mushroom.bin", NULL);
    StaticMesh* mesh3 = Object_StaticMesh_create("./assets/meshes/mushroom.bin", NULL);
    StaticMesh* lightVis = Object_StaticMesh_create("./assets/meshes/arrow.bin", NULL);
    
    vec3 cameraDefaultPos = { 0.0f, -1.0f, -1.0f };
    mat4_translate(cameraDefaultPos, mainCamera->Transform);

    Object_StaticMesh_set_Material(mesh0, 0, Mat1);
    Object_StaticMesh_set_Material(mesh1, 0, Normals);
    Object_StaticMesh_set_Material(mesh2, 0, TCoords);
    Object_StaticMesh_set_Material(mesh3, 0, Dither);
    Object_StaticMesh_set_Material(lightVis, 0, Mat0);
    Object_StaticMesh_set_Material(groundMesh, 0, Mat0);
    
    vec3 mesh0Translate = { 1.0f, 0.0f, 1.0f };
    vec3 mesh1Translate = { 1.0f, 0.0f, -1.0f };
    vec3 mesh2Translate = { -1.0f, 0.0f, 1.0f };
    vec3 mesh3Translate = { -1.0f, 0.0f, -1.0f };

    vec3 meshScale = { 0.5f, 0.5f, 0.5f };
    mat4 scale;
    mat4_scale(meshScale, scale);

    mat4_translate(mesh0Translate, mesh0->Transform);
    mat4_translate(mesh1Translate, mesh1->Transform);
    mat4_translate(mesh2Translate, mesh2->Transform);
    mat4_translate(mesh3Translate, mesh3->Transform);

    mat4_multiply(scale, mesh0->Transform, mesh0->Transform);
    mat4_multiply(scale, mesh1->Transform, mesh1->Transform);
    mat4_multiply(scale, mesh2->Transform, mesh2->Transform);
    mat4_multiply(scale, mesh3->Transform, mesh3->Transform);

    GLint activeLights = 2;

    vec3 lightPos = { 0.0f, 2.0f, 0.0f };
    vec3 lightDir = { 1.0f, 0.0f, 0.0f };
    vec3 lightColor = { 2.0f, 2.0f, 2.0f };
    vec3 AmbientColor = { 0.2f, 0.2f, 0.2f };
    float lightRadius = 25.0f;
    
    Engine_set_ambient_color(AmbientColor[0], AmbientColor[1], AmbientColor[2]);
    

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

    UniformBuffer_update_all();


    bool captureCursor = true;
    SetCaptureCursor(captureCursor);

    while (Engine_execute_tick()) {

        if (IsKeyPressed(GLFW_KEY_TAB)) {
            captureCursor = !captureCursor;
            SetCaptureCursor(captureCursor);
        }

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
 
        GLfloat time = Time();

        UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, lightPos);
        UniformBuffer_set_Global    ("FrameData", "u_time", &time);
        UniformBuffer_set_Global("FrameData", "u_view", mainCamera->ViewMatrix);
        UniformBuffer_set_Global("FrameData", "u_position", cameraPos);
        UniformBuffer_set_Global("FrameData", "u_direction", cameraDir);

        UniformBuffer_update_all();

        groundMesh->Draw(groundMesh);
        mesh0->Draw(mesh0);
        mesh1->Draw(mesh1);
        mesh2->Draw(mesh2);
        mesh3->Draw(mesh3);
        lightVis->Draw(lightVis);
       
        //SetText(testText,"This is a test.", x, y, static_cast<float>(WindowWidth()), static_cast<float>(WindowHeight()), 2.0f);
        //DrawTextMesh(testText, mainCamera, AspectRatio());
    }
    
    mainCamera->Destroy(mainCamera);
    mesh0->Destroy(mesh0);
    mesh1->Destroy(mesh1);
    mesh2->Destroy(mesh2);
    mesh3->Destroy(mesh3);
    lightVis->Destroy(lightVis);

    Material_destroy(&Mat0); 
    Material_destroy(&Mat1); 
    Material_destroy(&Dither);
    Material_destroy(&Normals);
    Material_destroy(&TCoords);
    
    Engine_terminate();
    return 0;
}

