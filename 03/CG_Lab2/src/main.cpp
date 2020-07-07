// Standard headers
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>

#include "shader.h"
#include "model.h"
#include "basis.h"

/* For testing propose use the local (not installed) ui file */
/* #define RES_DIR PACKAGE_DATA_DIR"/res/" */
#define RES_DIR "resources/"

void reset();
void processInput(GLFWwindow *window);
void framebuffer_size_callback(GLFWwindow* window, GLint width,
                               GLint height);
void scroll_callback(GLFWwindow* window, GLdouble xoffset,
                     GLdouble yoffset);
void key_callback(GLFWwindow* window, int key, int scancode,
                     int action, int mods);

// settings
unsigned int SCR_WIDTH = 1366;
unsigned int SCR_HEIGHT = 700;

// camera
Basis basis[3] = {Basis(true)};
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

// active element
int active = 0;

// states
bool waterframe = 0;
bool light = 0;
bool dirLightOn = 1;
bool pointLight1On = 1;
bool pointLight2On = 1;
bool spotLightOn = 1;

float ZOOM =  45.0f;

int main() {
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
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT,
                                          "Egor Panasenko",
                                          NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, key_callback);

    // load glew
    // ---------
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // -------------------------------------
    Shader lightingShader(RES_DIR"lighting.vs", RES_DIR"lighting.fs");
    Shader lightlessShader(RES_DIR"lightless.vs",RES_DIR"lightless.fs");
    Shader * sh;

    // load models
    // -----------
    Model EModel(RES_DIR"E.bo");
    Model PModel(RES_DIR"P.bo");
    Model CubeModel(RES_DIR"Cube.bo");

    // lamp positions
    // --------------
    vec3 lampPositions[] = {
        vec3(5.0f, 0.0f, 5.0f),
        vec3(-1.0f, 0.0f, 1.0f)
    };

    reset();

    // render loop
    // -----------
    while(!glfwWindowShouldClose(window)) {
        // per-frame time logic
        // --------------------
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.8f, 0.9f, 0.8f, 1.0f);
        glClearDepth(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        mat4& view = basis[0].view;
        mat4 camera_rotation = toMat4(basis[0].rotation);
        vec3 campos = basis[0].get_camera_position();
        vec3 camdir = basis[0].get_camera_direction();

        // activate shader
        if (light) {
            sh = &lightingShader;
            sh->use();
            sh->setVec3("viewPos", campos);
            sh->setVec3("objectColor", 0.4f, 0.8f, 0.4f);
            sh->setFloat("materialShininess", 32.0f);
            sh->setBool("dirLightOn", dirLightOn);
            sh->setBool("pointLight1On", pointLight1On);
            sh->setBool("pointLight2On", pointLight2On);
            sh->setBool("spotLightOn", spotLightOn);

            // directional light
            sh->setVec3("dirLight.direction", -1.0f, -1.0f, -1.0f);
            sh->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
            sh->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
            sh->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
            // point light 1
            sh->setVec3("pointLight1.position", lampPositions[0]);
            sh->setVec3("pointLight1.ambient",  0.00f, 0.00f, 0.00f);
            sh->setVec3("pointLight1.diffuse",  1.0f, 1.0f, 1.0f);
            sh->setVec3("pointLight1.specular", 0.0f, 0.0f, 0.0f);
            sh->setFloat("pointLight1.constant", 1.0f);
            sh->setFloat("pointLight1.linear", 0.0f);
            sh->setFloat("pointLight1.quadratic", 0.0f);
            // point light 2
            sh->setVec3("pointLight2.position", lampPositions[1]);
            sh->setVec3("pointLight2.ambient", 0.05f, 0.05f, 0.05f);
            sh->setVec3("pointLight2.diffuse", 0.8f, 0.8f, 0.8f);
            sh->setVec3("pointLight2.specular", 1.0f, 1.0f, 1.0f);
            sh->setFloat("pointLight2.constant", 1.0f);
            sh->setFloat("pointLight2.linear", 0.09f);
            sh->setFloat("pointLight2.quadratic", 0.032f);
            // spotLight
            sh->setVec3("spotLight.position", campos);
            sh->setVec3("spotLight.direction", camdir);
            sh->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            sh->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
            sh->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            sh->setFloat("spotLight.constant", 1.0f);
            sh->setFloat("spotLight.linear", 0.09);
            sh->setFloat("spotLight.quadratic", 0.032);
            sh->setFloat("spotLight.cutOff",
                         cos(radians(12.5f)));
            sh->setFloat("spotLight.outerCutOff",
                         cos(radians(15.0f)));
        } else {
            sh = &lightlessShader;
            sh->use();
        }

        // pass projection matrix to shader
        mat4 projection = perspective
            (radians(ZOOM),
             (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        sh->setMat4("projection", projection);

        // camera/view transformation
        sh->setMat4("view", view);
        sh->setMat4("camera_rotation", camera_rotation);

        // render the loaded model
        basis[1].animate(deltaTime);
        sh->setMat4("model", basis[1].view);
        sh->setMat4("rotation", toMat4(basis[1].rotation));
        EModel.Draw(*sh);

        // render the loaded model
        basis[2].animate(deltaTime);
        sh->setMat4("model", basis[2].view);
        sh->setMat4("rotation", toMat4(basis[2].rotation));
        PModel.Draw(*sh);

        lightlessShader.use();
        lightlessShader.setMat4("projection", projection);
        lightlessShader.setMat4("camera_rotation", camera_rotation);
        lightlessShader.setMat4("view", view);
        lightlessShader.setMat4("rotation", mat4(1.0));
        lightlessShader.setMat4
            ("model", translate(mat4(1.0), lampPositions[0]));
        CubeModel.Draw(lightlessShader);
        lightlessShader.setMat4
            ("model", translate(mat4(1.0), lampPositions[1]));
        CubeModel.Draw(lightlessShader);

        // glfw: swap buffers and poll IO events
        // -------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources
    // -----------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void reset() {
    vec3 empty(0.0f, 0.0f, 0.0f);
    basis[0].animation = empty;
    basis[1].animation = empty;
    basis[2].animation = empty;

    basis[0].view = translate(mat4(1.0),
                                   vec3(0.0f, 0.0f, -3.0f));

    /*basis[1].view = rotate(
                translate(mat4(1.0), vec3(-0.8f, 0.0f, 0.0f)),
                 0.2f, vec3(0.0f, 0.0f, 1.0f));

    basis[2].view = rotate(
                translate(mat4(1.0), vec3( 0.8f, 0.0f, 0.0f)),
                -0.2f, vec3(0.0f, 0.0f, 1.0f));*/

    basis[1].view = translate(mat4(1.0), vec3(-0.8f, 0.0f, 0.0f));
    basis[1].rotation = quat(vec3(0.0f, 0.0f,  0.2f));

    basis[2].view = translate(mat4(1.0), vec3( 0.8f, 0.0f, 0.0f));
    basis[2].rotation = quat(vec3(0.0f, 0.0f, -0.2f));
}

// process all input
// -----------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    bool ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS;
    float s = (ctrl)? 10.0f : 1.0f;
    float a = active ? -1.0f : 1.0f;

    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        basis[active].move(Y * a * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        basis[active].move(X * a * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        basis[active].move(Z * a * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        basis[active].move(-Z * a * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        basis[active].move(-Y * a * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        basis[active].move(-X * a * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
        basis[active].rotate(-Z * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
        basis[active].rotate(-Y * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
        basis[active].rotate(-X * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        basis[active].rotate(X * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
        basis[active].rotate(Z * s * deltaTime);
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        basis[active].rotate(Y * s * deltaTime);

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
        basis[active].animation += -Z * s * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
        basis[active].animation += -Y * s * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
        basis[active].animation += -X * s * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
        basis[active].animation += X * s * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
        basis[active].animation += Z * s * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
        basis[active].animation += Y * s * deltaTime;

    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
        reset();
}

// glfw: when the window size changed this callback function executes
// ------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, GLint width,
                               GLint height) {
    // make sure the viewport matches the new window dimensions;
    // note that width and height will be significantly larger
    // than specified on retina displays.
    glViewport(0, 0, width, height);
    SCR_WIDTH = width; SCR_HEIGHT = height;
}

// glfw: when the mouse scroll wheel scrolls, this callback is called
// ------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, GLdouble xoffset,
                     GLdouble yoffset) {
    if (ZOOM >= 1.0f && ZOOM <= 45.0f)
        ZOOM -= yoffset;
    if (ZOOM <= 1.0f)
        ZOOM = 1.0f;
    if (ZOOM >= 45.0f)
        ZOOM = 45.0f;
}

// glfw: when the keyboard was used, this callback is called
// ------------------------------------------------------------------
void key_callback(GLFWwindow* window, int key, int scancode,
                     int action, int mods) {
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS)
        active++;
    if (active > 2) active = 0;
    if (key == GLFW_KEY_Z && action == GLFW_PRESS) {
        if (waterframe) {
            waterframe = 0;
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        } else {
            waterframe = 1;
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        }
    }
    if (key == GLFW_KEY_X && action == GLFW_PRESS)
        light = (light)? 0 : 1;
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
        dirLightOn = (dirLightOn)? 0 : 1;
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
        pointLight1On = (pointLight1On)? 0 : 1;
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
        pointLight2On = (pointLight2On)? 0 : 1;
    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
        spotLightOn = (spotLightOn)? 0 : 1;
    if (key == GLFW_KEY_0 && action == GLFW_PRESS) {
        dirLightOn = 0;
        pointLight1On = 0;
        pointLight2On = 0;
        spotLightOn = 0;
    }

}
