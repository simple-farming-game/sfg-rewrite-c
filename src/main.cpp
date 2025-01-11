#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <NovaLib/shader_m.h>
#include <NovaLib/camera.h>
#include <NovaLib/model.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Simple Farming Game beta", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("src/shader.vs", "src/shader.fs");
    Shader lightCubeShader("src/light.vs", "src/light.fs");

    Model ourModel("assets/model/arona/arona.obj");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // 윗면 (Top Face)
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.375f,  0.25f,   // 좌하단
        0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.625f,  0.25f,  // 우하단
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.625f,  0.5f,   // 우상단
        0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.625f,  0.5f,   // 우상단
        -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.375f,  0.5f,   // 좌상단
        -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.375f,  0.25f,  // 좌하단

        // 아랫면 (Bottom Face)
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.125f,  0.125f, // 좌하단
        0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.375f,  0.125f, // 우하단
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.375f,  0.375f, // 우상단
        0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.375f,  0.375f, // 우상단
        -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.125f,  0.375f, // 좌상단
        -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.125f,  0.125f, // 좌하단

        // 앞면 (Front Face)
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.375f,  0.25f,  // 좌하단
        0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.625f,  0.25f,  // 우하단
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.625f,  0.5f,   // 우상단
        0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.625f,  0.5f,   // 우상단
        -0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.375f,  0.5f,   // 좌상단
        -0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.375f,  0.25f,  // 좌하단

        // 뒷면 (Back Face)
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.125f,  0.25f,  // 좌하단
        0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.375f,  0.25f,  // 우하단
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.375f,  0.5f,   // 우상단
        0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.375f,  0.5f,   // 우상단
        -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.125f,  0.5f,   // 좌상단
        -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.125f,  0.25f,  // 좌하단

        // 왼쪽면 (Left Face)
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.375f,  0.125f, // 좌하단
        -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.625f,  0.125f, // 우하단
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.625f,  0.375f, // 우상단
        -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.625f,  0.375f, // 우상단
        -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.375f,  0.375f, // 좌상단
        -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.375f,  0.125f, // 좌하단

        // 오른쪽면 (Right Face)
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.375f,  0.125f, // 좌하단
        0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.625f,  0.125f, // 우하단
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.625f,  0.375f, // 우상단
        0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.625f,  0.375f, // 우상단
        0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.375f,  0.375f, // 좌상단
        0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.375f,  0.125f  // 좌하단
    };



    // first, configure the cube's VAO (and VBO)
    unsigned int VBO, cubeVAO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(cubeVAO);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // load textures (we now use a utility function to keep the code more organized)
    // -----------------------------------------------------------------------------
    unsigned int diffuseMap = loadTexture("assets/image/grass.png");

    // shader configuration
    // --------------------
    lightingShader.use();
    lightingShader.setInt("material.diffuse", 0);
    lightingShader.setInt("material.specular", 1);


    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);
        lightingShader.setFloat("material.shininess", 32.0f);

        /*
           Here we set all the uniforms for the 5/6 types of lights we have. We have to set them manually and index 
           the proper PointLight struct in the array to set each uniform variable. This can be done more code-friendly
           by defining light types as classes and set their values in there, or by using a more efficient uniform approach
           by using 'Uniform buffer objects', but that is something we'll discuss in the 'Advanced GLSL' tutorial.
        */

        // use light
        lightingShader.setBool("useDirLight", false);
        lightingShader.setBool("usePointLight", false);
        lightingShader.setBool("useSpotLight", false);
        lightingShader.setBool("useAmbientLight", true);

        // ambient light
        lightingShader.setVec3("ambientLightColor", glm::vec3(0.2f, 0.2f, 0.2f)); // 약한 흰색 빛
        lightingShader.setFloat("ambientIntensity", 5.0f); // 약한 강도

        // dir light
        lightingShader.setVec3("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.5f));
        lightingShader.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        lightingShader.setVec3("dirLight.diffuse", glm::vec3(1.4f, 1.4f, 1.4f));
        lightingShader.setVec3("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

        // view/projection transformations
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("projection", projection);
        lightingShader.setMat4("view", view);

        // world transformation
        glm::mat4 model = glm::mat4(1.0f);
        lightingShader.setMat4("model", model);

        // bind diffuse map
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);

        // render containers
        glBindVertexArray(cubeVAO);
        // calculate the model matrix for each object and pass it to shader before drawing
        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3( 0.0f,  0.0f,  0.0f));
        lightingShader.setMat4("model", model);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(2.0f, 0.0f, 2.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        lightingShader.setMat4("model", model);
        ourModel.Draw(lightingShader);

        glDrawArrays(GL_TRIANGLES, 0, 36);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

// utility function for loading a 2D texture from file
// ---------------------------------------------------
unsigned int loadTexture(char const * path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        // 텍스처 래핑 설정: 픽셀 단위로 매핑되도록 GL_NEAREST 사용
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // 텍스처 필터링 설정: 픽셀 단위 매핑을 위해 GL_NEAREST 사용
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
