#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <Shader.h>
#include <CMakeProjectMacros.h>

#include "CameraController.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window, float DeltaTime);


// settings
unsigned int SCR_WIDTH  = 800;
unsigned int SCR_HEIGHT = 600;

std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector);
std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector);
std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix);
std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix);

struct ObjectBuffer
{
    uint32_t VAO{};
    uint32_t VBO{};
    uint32_t EBO{};
    
    ~ObjectBuffer()
    {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};
void PrepareCubeBuffer(ObjectBuffer& OutData);
void PrepareQuadBuffer(ObjectBuffer& OutData);

glm::vec3 CubeData[] = {
    //Vertex Positions               //Texture coordinates     //Normals
                       //Front plane
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3( 1.f, -1.f, -1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3( 1.f,  1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3( 1.f,  1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-1.f,  1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, -1.f),
                      //Back plane             
    glm::vec3(-1.f, -1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3( 1.f, -1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-1.f,  1.f,  1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
    glm::vec3(-1.f, -1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, 0.f, 1.f),
                      //Left plane          
    glm::vec3(-1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-1.f,  1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-1.f, -1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
    glm::vec3(-1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(-1.f, 0.f, 0.f),
                       //Right plane           
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3( 1.f,  1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3( 1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3( 1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3( 1.f, -1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(1.f, 0.f, 0.f),
                      //Bottom plane            
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
    glm::vec3( 1.f, -1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
    glm::vec3( 1.f, -1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
    glm::vec3( 1.f, -1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
    glm::vec3(-1.f, -1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
    glm::vec3(-1.f, -1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, -1.f, 0.f),
                      //Top plane            
    glm::vec3(-1.f,  1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3( 1.f,  1.f, -1.f),  glm::vec3(1.0f, 1.0f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3( 1.f,  1.f,  1.f),  glm::vec3(1.0f, 0.0f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3(-1.f,  1.f,  1.f),  glm::vec3(0.0f, 0.0f, 0.f), glm::vec3(0.f, 1.f, 0.f),
    glm::vec3(-1.f,  1.f, -1.f),  glm::vec3(0.0f, 1.0f, 0.f), glm::vec3(0.f, 1.f, 0.f)
};

glm::vec2 PlaneVertexData[] //2D plane
{
    //Positions             
    glm::vec2(-1.f, +1.f), glm::vec2(0.f, 1.f),
    glm::vec2(-1.f, -1.f), glm::vec2(0.f, 0.f),
    glm::vec2(+1.f, -1.f), glm::vec2(1.f, 0.f),
    glm::vec2(+1.f, +1.f), glm::vec2(1.f, 1.f)
};

unsigned int PlaneDrawIndices[]
{
    0, 1, 2,
    2, 3, 0
};

struct Material
{
	glm::vec3 Color{};
	float Shineness{};
	float Specularity{};
	float Diffuseness{};
};

#define SPOT_LIGHT        1
#define DIRECTIONAL_LIGHT 2
#define POINT_LIGHT       3
#define AMBIENT_LIGHT     4
#define NONE              0

struct Light
{
	int  Type;
	glm::vec3  Position;
	float Intensity;
	glm::vec3  Direction;

    //For spot light
    float ConeOpening;
	float PushToEdge;

    //Attenuation
	float Constant  = 1;
	float Linear    = 0.022f;
	float Quadratic = 0.0019f;
};

void UpdateLight(const Shader& Program, const std::vector<Light*>& SceneLights, const glm::vec3& CameraPosition);
void UpdateMaterial ( const Shader& Program, const Material& ObjectMaterial );
void AddLightToScene( std::vector<Light*>& SceneLights, Light& LightToAdd);

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const char* Title  = "Instancing";
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, Title, NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }   

    //Camera setup / definition
    Camera MainCamera(FCameraDefinition{ECameraType::Perspective, 45.f, 1.f, 0.1f, 1000.f} );
    Camera::LoadStateFromFile(MainCamera, PROJECT_DIR"/CameraSaves/MainCamera.txt");

    ACameraController CameraController;
    CameraController.MountACamera(&MainCamera);
    CameraController.Window = window; 
    CameraController.SetTranslationalSpeed(50);
    CameraController.SetMouseSensitivity(0.15f);

    glfwSetCursorPosCallback(window, &ACameraController::Statics_OnMousePositionChange);
    glfwSetScrollCallback(window, &ACameraController::Statics_OnMouseScrollWheel);

    float DeltaTime = 0;
    float LastTime  = 0;

    Shader CubeProgram(
        PROJECT_DIR"/ShaderPrograms/Cube.vert", 
        PROJECT_DIR"/ShaderPrograms/Cube.frag",
        PROJECT_DIR"/ShaderPrograms/LightCalculation.frag"
    );

    Light PointLight = {POINT_LIGHT};
    PointLight.Position = glm::vec3(0, 50, 0);
    PointLight.Intensity = 0.8f;
    PointLight.Quadratic = 0.f;
    //PointLight.Linear    = 0.09f;

    Light AmbientLight = {AMBIENT_LIGHT};
    AmbientLight.Intensity = 0.1f;

    Light SpotLight = {SPOT_LIGHT};
    SpotLight.Position    = glm::vec3(-10, 5, 50);
    SpotLight.Intensity   = 1.f;
    SpotLight.Direction   = glm::vec3(0, 0, -1);
    SpotLight.ConeOpening = 30.0;  // in degrees
    SpotLight.PushToEdge  = 10.0;  

    std::vector<Light*> SceneLights;
    AddLightToScene(SceneLights, PointLight); 
    AddLightToScene(SceneLights, AmbientLight);

    CubeProgram.Use();
    UpdateLight(CubeProgram, SceneLights, CameraController.GetCameraPosition() );

    //Object buffer
    ObjectBuffer CubeBuffer;
    ObjectBuffer QuadBuffer;
    PrepareCubeBuffer(CubeBuffer);
    PrepareQuadBuffer(QuadBuffer);

    //CUBE DEFINITION
    glm::vec3 Cube_Scale    = glm::vec3(10);
    glm::vec3 Cube_Position = glm::vec3(0.0);
    
    glm::mat4 Cube_RotationMatrix = glm::rotate( glm::mat4(1), glm::radians(0.f), glm::vec3(0, 1, 0) );
    
    glm::mat4 Cube_ModelMatrix = glm::scale( Cube_RotationMatrix, Cube_Scale );
    Cube_ModelMatrix[3][0] = Cube_Position.x;
    Cube_ModelMatrix[3][1] = Cube_Position.y;
    Cube_ModelMatrix[3][2] = Cube_Position.z;
    Cube_ModelMatrix[3][3] = 1.0f;
    
    // Create 'n' number of materials
    std::vector<Material> Materials; 
    Materials.reserve(3);

    Material TmpMaterial = { glm::vec3(1, 0, 0), 20.f };
    Materials.emplace_back(TmpMaterial);

    TmpMaterial.Color = glm::vec3(0, 1, 0); TmpMaterial.Shineness = 20.f;
    Materials.emplace_back(TmpMaterial);

    TmpMaterial.Color = glm::vec3(0, 0, 1); TmpMaterial.Shineness = 20.f;
    Materials.emplace_back(TmpMaterial);

    TmpMaterial.Color = glm::vec3(0, 0.5f, 1.f); TmpMaterial.Shineness = 20.f;
    Materials.emplace_back(TmpMaterial);
    
    CubeProgram.Use();
    for (size_t i = 0; i < Materials.size(); i++)
    {
        char buffer[64]{};

        sprintf(buffer, "UObjectMaterials[%i].Color", i);
        CubeProgram.SetVector3(buffer, Materials[i].Color);

        sprintf(buffer, "UObjectMaterials[%i].Diffuseness", i);
        CubeProgram.SetFloat(buffer, Materials[i].Diffuseness);

        sprintf(buffer, "UObjectMaterials[%i].Specularity", i);
        CubeProgram.SetFloat(buffer, Materials[i].Specularity);

        sprintf(buffer, "UObjectMaterials[%i].Shineness", i);
        CubeProgram.SetFloat(buffer, Materials[i].Shineness);
    }
    
    //Key binding
    glfwSetMouseButtonCallback(window, [](GLFWwindow* Window, int ButtonID, int keystate, int mods)
    {
        const int ThumbMouseButton1 = 3;
        const int ThumbMouseButton2 = 4;

        if(ButtonID == ThumbMouseButton2 && keystate == GLFW_PRESS)
        {
            
        }
    });

    while (!glfwWindowShouldClose(window))
    {
        processInput(window, DeltaTime);
        glClearColor(0.05f, 0.05f, 0.05f, 0.05f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        /*
        glEnable(GL_BLEND);
        glBlendFunc(GL_ONE, GL_ONE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        */
        CameraController.Tick();  

        CubeProgram.Use();
        CubeProgram.SetMat4("UModel", Cube_ModelMatrix);
        CubeProgram.SetMat4("URotationMatrix", Cube_RotationMatrix);
        CubeProgram.SetMat4("UView", CameraController.GetControlledCamera()->GetViewMatrix());
        CubeProgram.SetMat4("UProjection", CameraController.GetControlledCamera()->GetProjectionMatrix());
        CubeProgram.SetVector2("UWindowDimension", glm::vec2(SCR_WIDTH, SCR_HEIGHT) );

        glBindVertexArray(CubeBuffer.VAO);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 36, Materials.size());
        //glDrawArrays(GL_TRIANGLES, 0, 36);
        
        //TIME
        float CurrentTime = (float)glfwGetTime();
        DeltaTime = CurrentTime - LastTime;
        LastTime  = CurrentTime;

        
        //
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    

    Camera::SaveStateToFile(MainCamera, PROJECT_DIR"/CameraSaves/MainCamera.txt");

    glfwTerminate();
    return 0;
}

void AddLightToScene( std::vector<Light*>& SceneLights, Light& LightToAdd)
{
    SceneLights.emplace_back(&LightToAdd);
}

void UpdateLight(const Shader& Program, const std::vector<Light*>& SceneLights, const glm::vec3& CameraPosition)
{
    int i = 0;
    for (const auto Light : SceneLights)
    {
        char buffer[64]{};

        sprintf(buffer, "USceneLights[%i].Type", i);
        Program.SetInt(buffer, Light->Type );

        sprintf(buffer, "USceneLights[%i].Position", i);
        Program.SetVector3(buffer, Light->Position );

        sprintf(buffer, "USceneLights[%i].Intensity", i);
        Program.SetFloat(buffer, Light->Intensity );

        sprintf(buffer, "USceneLights[%i].Direction", i);
        Program.SetVector3(buffer, Light->Direction );

        sprintf(buffer, "USceneLights[%i].ConeOpening", i);
        Program.SetFloat(buffer, Light->ConeOpening );

        sprintf(buffer, "USceneLights[%i].PushToEdge", i);
        Program.SetFloat(buffer, Light->PushToEdge );

        //Attenuation
        sprintf(buffer, "USceneLights[%i].Constant", i);
        Program.SetFloat(buffer, Light->Constant );

        sprintf(buffer, "USceneLights[%i].Linear", i);
        Program.SetFloat(buffer, Light->Linear );

        sprintf(buffer, "USceneLights[%i].Quadratic", i);
        Program.SetFloat(buffer, Light->Quadratic );

        i++;
    }
    Program.SetInt("UTotalLights", SceneLights.size() );
    Program.SetVector3("UCameraPosition", CameraPosition );
}

void UpdateMaterial ( const Shader& Program, const Material& ObjectMaterial )
{
    Program.SetVector3("UObjectMaterial.Color",     ObjectMaterial.Color);
    Program.SetFloat("UObjectMaterial.Diffuseness", ObjectMaterial.Diffuseness);
    Program.SetFloat("UObjectMaterial.Specularity", ObjectMaterial.Specularity);
    Program.SetFloat("UObjectMaterial.Shineness",   ObjectMaterial.Shineness);
}


void processInput(GLFWwindow *window, float DeltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    //Camera Controls

    //forward 'W'
    ACameraController::Statics_MoveForwardBackward(
        glfwGetKey(window, GLFW_KEY_W) * 1.f,
        DeltaTime
    );

    //backwards 'S'
    ACameraController::Statics_MoveForwardBackward(
        glfwGetKey(window, GLFW_KEY_S) * -1.0f,
        DeltaTime
    );

    //left 'A'
    ACameraController::Statics_MoveLeftRight(
        glfwGetKey(window, GLFW_KEY_A) * -1.0f,
        DeltaTime
    );

    //right 'D'
    ACameraController::Statics_MoveLeftRight(
        glfwGetKey(window, GLFW_KEY_D) * 1.0f,
        DeltaTime
    );

    //up 'E'
    ACameraController::Statics_MoveUpDown(
        glfwGetKey(window, GLFW_KEY_E) * 1.0f,
        DeltaTime
    );

    //down 'Q'
    ACameraController::Statics_MoveUpDown(
        glfwGetKey(window, GLFW_KEY_Q) * -1.0f,
        DeltaTime
    );
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
    SCR_WIDTH  = width;
    SCR_HEIGHT = height;
}

void PrepareCubeBuffer(ObjectBuffer& OutData)
{
    //Cube
    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeData), CubeData, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3  * sizeof(CubeData[0]), (void*)0);
    // texture coord attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3  * sizeof(CubeData[0]), (void*)(sizeof(CubeData[0]) ) );
    // normals
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3  * sizeof(CubeData[0]), (void*)(sizeof(CubeData[0]) * 2 ) );

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    OutData.VAO = VAO;
    OutData.VBO = VBO;
}

void PrepareQuadBuffer(ObjectBuffer& OutData)
{
    //Quad
    unsigned int VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertexData), PlaneVertexData, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(PlaneDrawIndices), PlaneDrawIndices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2  * sizeof(PlaneVertexData[0]), (void*)0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(PlaneVertexData[0]), (void*)(sizeof(PlaneVertexData[0])));
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    OutData.VAO = VAO;
    OutData.VBO = VBO;
    OutData.EBO = EBO;
}


std::ostream& operator<<(std::ostream& cout, const glm::vec3& vector)
{
    printf("< %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z);
    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::vec4& vector)
{
    printf("< %3.2f, %3.2f, %3.2f, %3.2f >", vector.x, vector.y, vector.z, vector.w);

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat3& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|  \n");
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1] );
    printf("%3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    std::cout << "=======================================================================================\n";

    return cout;
}

std::ostream& operator<<(std::ostream& cout, const glm::mat4& Matrix)
{
    std::cout << "========================================MATRIX==========================================\n";
    printf("   X|   Y|    Z|     W|  \n");
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][0], Matrix[1][0], Matrix[2][0], Matrix[3][0] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][1], Matrix[1][1], Matrix[2][1], Matrix[3][1] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][2], Matrix[1][2], Matrix[2][2], Matrix[3][2] );
    printf("%3.2f, %3.2f, %3.2f, %3.2f\n", Matrix[0][3], Matrix[1][3], Matrix[2][3], Matrix[3][3] );
    cout << "-------------------------------\n";
    cout << "X = " << Matrix[0] << "\n";
    cout << "Y = " << Matrix[1] << "\n";
    cout << "Z = " << Matrix[2] << "\n";
    cout << "W = " << Matrix[3] << "\n";
    std::cout << "=======================================================================================\n";


    return cout;
}
