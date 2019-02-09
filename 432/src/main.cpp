// Standard headers
#include <iostream>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "model.h"

/* For testing propose use the local (not installed) ui file */
/* #define RES_DIR PACKAGE_DATA_DIR"/res/" */
#define RES_DIR "resources/"

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
Camera camera;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// letters data
glm::vec3 EPos(  0.6f,   0.0f,   0.0f);
glm::vec3 EDeg(  0.0f,   0.0f,  -5.0f);
glm::vec3 EAni(  0.0f,   0.0f,   0.0f);
glm::vec3 PPos( -0.6f,   0.0f,   0.0f);
glm::vec3 PDeg(  0.0f,   0.0f,   5.0f);
glm::vec3 PAni(  0.0f,   0.0f,   0.0f);

// active element
int active = 0;

// states
bool waterframe = 0;
bool light = 0;
bool dirLightOn = 1;
bool pointLight1On = 1;
bool pointLight2On = 1;
bool spotLightOn = 1;

const float MOVE_SPEED    = 0.5f;
const float ROTATE_SPEED  = 64.0f;
const float ANIMATE_SPEED = 256.0f;

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
	glm::vec3 lampPositions[] = {
		glm::vec3(5.0f, 0.0f, 5.0f),
		glm::vec3(-1.0f, 0.0f, 1.0f)
	};
	
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

		glm::mat4 model;
        glm::mat4 view = camera.GetViewMatrix();
		glm::vec3 campos = camera.get_position();
		glm::vec3 camdir = camera.get_direction();

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
			             glm::cos(glm::radians(12.5f)));
			sh->setFloat("spotLight.outerCutOff",
			             glm::cos(glm::radians(15.0f)));     
		} else {
			sh = &lightlessShader;
			sh->use();
		}

        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective
			(glm::radians(camera.Zoom),
			 (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        sh->setMat4("projection", projection);

        // camera/view transformation
        sh->setMat4("view", view);

		// render the loaded model
        model = glm::mat4(1.0);
		model = glm::translate(model, EPos);
		model = glm::rotate(model,
		                    glm::radians(EDeg.y += deltaTime * EAni.y),
		                    glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(EDeg.x += deltaTime * EAni.x),
		                    glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(EDeg.z += deltaTime * EAni.z),
		                    glm::vec3(0.0, 0.0, 1.0));
		sh->setMat4("model", model);
        EModel.Draw(*sh);
		
		// render the loaded model
        model = glm::mat4(1.0);
		model = glm::translate(model, PPos);
		model = glm::rotate(model,
		                    glm::radians(PDeg.y += deltaTime * PAni.y),
		                    glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(PDeg.x += deltaTime * PAni.x),
		                    glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(PDeg.z += deltaTime * PAni.z),
		                    glm::vec3(0.0, 0.0, 1.0));
		sh->setMat4("model", model);
        PModel.Draw(*sh);

		lightlessShader.use();
        lightlessShader.setMat4("projection", projection);
        lightlessShader.setMat4("view", view);
        lightlessShader.setMat4
			("model", glm::translate(glm::mat4(1.0), lampPositions[0]));
        CubeModel.Draw(lightlessShader);
        lightlessShader.setMat4
			("model", glm::translate(glm::mat4(1.0), lampPositions[1]));
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

// process all input
// -----------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

	if (active == 0) {
		bool ctrl = glfwGetKey(window, GLFW_KEY_LEFT_CONTROL)==GLFW_PRESS;
		float s = (ctrl)? 10.0f : 1.0f;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			camera.move(FORWARD, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			camera.move(BACKWARD, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			camera.move(LEFT, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			camera.rotate(LEFT, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			camera.move(RIGHT, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			camera.rotate(RIGHT, s * deltaTime);
		
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			camera.move(UP, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			camera.move(DOWN, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			camera.rotate(FORWARD, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			camera.rotate(BACKWARD, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			camera.rotate(UP, s * deltaTime);
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			camera.rotate(DOWN, s * deltaTime);
	} else {
		if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
			*((active == 1)?&EPos.x:&PPos.x) += MOVE_SPEED   *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
			*((active == 1)?&EPos.x:&PPos.x) -= MOVE_SPEED   *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			*((active == 1)?&EPos.y:&PPos.y) += MOVE_SPEED   *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			*((active == 1)?&EPos.y:&PPos.y) -= MOVE_SPEED   *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
			*((active == 1)?&EPos.z:&PPos.z) += MOVE_SPEED   *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
			*((active == 1)?&EPos.z:&PPos.z) -= MOVE_SPEED   *deltaTime;

		if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
			*((active == 1)?&EDeg.x:&PDeg.x) -= ROTATE_SPEED *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
			*((active == 1)?&EDeg.x:&PDeg.x) += ROTATE_SPEED *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
			*((active == 1)?&EDeg.y:&PDeg.y) -= ROTATE_SPEED *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
			*((active == 1)?&EDeg.y:&PDeg.y) += ROTATE_SPEED *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
			*((active == 1)?&EDeg.z:&PDeg.z) -= ROTATE_SPEED *deltaTime;
		if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
			*((active == 1)?&EDeg.z:&PDeg.z) += ROTATE_SPEED *deltaTime;
		
		if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
			*((active == 1)?&EAni.x:&PAni.x) -= ANIMATE_SPEED*deltaTime;
		if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
			*((active == 1)?&EAni.x:&PAni.x) += ANIMATE_SPEED*deltaTime;
		if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
			*((active == 1)?&EAni.y:&PAni.y) -= ANIMATE_SPEED*deltaTime;
		if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
			*((active == 1)?&EAni.y:&PAni.y) += ANIMATE_SPEED*deltaTime;
		if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
			*((active == 1)?&EAni.z:&PAni.z) -= ANIMATE_SPEED*deltaTime;
		if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
			*((active == 1)?&EAni.z:&PAni.z) += ANIMATE_SPEED*deltaTime;
	}
	
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
		camera.reset();
	}
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
		EAni = glm::vec3(  0.0f,   0.0f,   0.0f);
		PAni = glm::vec3(  0.0f,   0.0f,   0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
		EPos = glm::vec3(  0.6f,   0.0f,   0.0f);
		EDeg = glm::vec3(  0.0f,   0.0f,  -5.0f);
		EAni = glm::vec3(  0.0f,   0.0f,   0.0f);
		PPos = glm::vec3( -0.6f,   0.0f,   0.0f);
		PDeg = glm::vec3(  0.0f,   0.0f,   5.0f);
		PAni = glm::vec3(  0.0f,   0.0f,   0.0f);
	}
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
    camera.ProcessMouseScroll(yoffset);
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
