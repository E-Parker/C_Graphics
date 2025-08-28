#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <assert.h>
#include <math.h>

#include "gl_utilities.h"
#include "gl_math.h"
#include "gl_shader_uniform.h"

#include "texture.h"
#include "material.h"
#include "camera.h"
#include "mesh.h"
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

int main(void) {

    // Initialize the window to the starting size and set the header.
    GLFWwindow* window = Initialize(SCREEN_WIDTH, SCREEN_HEIGHT, "Delta Render");
    InitShaders();
    InitTextures();

    // Add termination functions to be executed at the end of the program.
    //glUtilAddTerminationFunction(DereferenceFonts);
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
    //Material* DefaultTextMaterial = new Material("./assets/shaders/defaultText.vert", "./assets/shaders/defaultText.frag", 1, GL_BACK, GL_ALWAYS);
    //Material* NormalMaterial = new Material("./assets/shaders/default.vert", "./assets/shaders/normal_color.frag", 0, GL_BACK, GL_LESS);
    //Material* TChoodColorMaterial = new Material("./assets/shaders/default.vert", "./assets/shaders/tcoord_color.frag", 0, GL_BACK, GL_LESS);
    Material* Mat0 = Material_create("./assets/shaders/default.vert", "./assets/shaders/default.frag", 4, GL_BACK, GL_LESS);
    Material* Mat1 = Material_create("./assets/shaders/default.vert", "./assets/shaders/default.frag", 4, GL_BACK, GL_LESS);

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

    //TextRender* testText = new TextRender();
    //SetFont(testText, "IBM", defautFont);

    //StaticMesh* mesh = CreateStaticMeshFromWavefront("./assets/meshes/mushroom.obj");
    //StaticMesh* lightVis = CreateStaticMeshFromWavefront("./assets/meshes/icosphere.obj");
    //mesh->SetMaterial(Mat0, 0);
    //lightVis->SetMaterial(Mat1, 0);

    Camera* mainCamera = Object_Camera_create();

    Shader* testShader = Shader_create(Mat0->Program, "TestShader");

    //Uniform* mvpUniform;
    //Uniform_set_data(mvpUniform, transform);

    int x = 0;
    int y = 0;

    GLfloat time = 0.0f;
    
    GLuint activeLights = 2;

    vec3 lightPos = { 0.0f, 5.0f, 0.0f };
    vec3 lightDir = { 1.0f, 0.0f, 0.0f };
    vec3 lightColor = { 2.0f, 2.0f, 2.0f };
    vec3 AmbientColor = { 0.5f, 0.5f, 0.5f };
    
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

        vec3 meshTranslate = { (float)x, (float)y, 0.0f };

        //mat4_translate(meshTranslate, &mesh->Transform);

        lightPos[0] = sinf(Time() * 1.3f) * 2.0f;
        lightPos[1] = (sinf(Time() * 0.7f) * 0.2f) + 1.0f;
        lightPos[2] = cosf(Time() * 1.3f) * 2.0f;

        //mat4_translate(lightPos, lightVis->Transform);

        UniformBuffer_set_Struct_at_Global("LightData", "u_Lights", "position", 0, &lightPos);
        
        Camera_NoClip_Update(mainCamera, DeltaTime(), AspectRatio());
        
        vec3 cameraPos; 
        vec3 cameraDir;

        mat4_get_translation(mainCamera->Transform, cameraPos);
        mat4_get_forward(mainCamera->Transform, cameraDir);

        mat4 cameraView;

        mat4_lookat(cameraPos, cameraDir, V3_UP, cameraView);

        UniformBuffer* buffer = UniformBuffer_get_self("FrameData");
        void* data = UniformBuffer_get_shared(buffer);
        //UniformBuffer* buffer1 = UniformBuffer_get_self("LightData");

        //UniformStruct* u_lights;
        //UniformBuffer_get_Struct(buffer1, "u_lights", &u_lights);

        UniformBuffer_set_Global("FrameData", "u_time", &time);
        UniformBuffer_set_Global("FrameData", "u_view", &cameraView);
        UniformBuffer_set_Global("FrameData", "u_position", &cameraPos);
        UniformBuffer_set_Global("FrameData", "u_direction", &cameraDir);

        UniformBuffer_update_all();

        //Shader_use(testShader);

        //mesh->Draw();
        //lightVis->Draw();
       
        //SetText(testText,"This is a test.", x, y, static_cast<float>(WindowWidth()), static_cast<float>(WindowHeight()), 2.0f);
        //DrawTextMesh(testText, mainCamera, AspectRatio());

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll and process events */
        glUtilPollEvents();
    }
    
    Object_Camera_destroy(mainCamera);
    //Object_Mesh_destroy(mesh);
    //Object_Mesh_destroy(lightVis);

    Material_destroy(&Mat0); 
    Material_destroy(&Mat1); 
    
    Shader_destroy(&testShader);
    
    glUtilTerminate();
    return 0;
}

