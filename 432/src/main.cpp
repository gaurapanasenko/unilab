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
#define RES_DIR "src/"

void framebuffer_size_callback(GLFWwindow* window, GLint width,
                               GLint height);
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos);
void scroll_callback(GLFWwindow* window, GLdouble xoffset,
                     GLdouble yoffset);
void processInput(GLFWwindow *window);

// settings
unsigned int SCR_WIDTH = 1366;
unsigned int SCR_HEIGHT = 700;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

// lighting
glm::vec3 lightPos(12.0f, 10.0f, 20.0f);

// letters data
glm::vec3 EPos(  0.6f,   0.0f,   0.0f);
glm::vec3 EDeg(  0.0f,   0.0f,  -5.0f);
glm::vec3 EAni(  0.0f,   0.0f,   0.0f);
glm::vec3 PPos( -0.6f,   0.0f,   0.0f);
glm::vec3 PDeg(  0.0f,   0.0f,   5.0f);
glm::vec3 PAni(  0.0f,   0.0f,   0.0f);


const float MOVE_SPEED =  0.5f;
const float ROTATE_SPEED =  64.0f;
const float ANIMATE_SPEED =  256.0f;

int main() {
    // glfw: initialize and configure
    // ------------------------------
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

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
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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
	Shader lampShader(RES_DIR"lamp.vs", RES_DIR"lamp.fs");

    // load models
    // -----------
    Model EModel(RES_DIR"E.obj");
    Model PModel(RES_DIR"P.obj");

    // draw in wireframe
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	glm::mat4 model;
	
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

        // activate shader
        lightingShader.use();
        lightingShader.setVec3("objectColor", 0.4f, 0.8f, 0.4f);
        lightingShader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);
        lightingShader.setVec3("lightPos", lightPos);

        //lightlessShader.use();
        // pass projection matrix to shader
        glm::mat4 projection = glm::perspective
			(glm::radians(camera.Zoom),
			 (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = camera.GetViewMatrix();
        lightingShader.setMat4("view", view);

		// render the loaded model
        model = glm::mat4();
		/*model = glm::rotate(model, (float)glfwGetTime(),
		//model = glm::rotate(model, (float)glm::radians(180),
		                    glm::vec3(0.0, 1.0, 0.0));*/
		model = glm::translate(model, EPos);
		model = glm::rotate(model,
		                    glm::radians(EDeg.x += deltaTime * EAni.x),
		                    glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(EDeg.y += deltaTime * EAni.y),
		                    glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(EDeg.z += deltaTime * EAni.z),
		                    glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
        EModel.Draw(lightingShader);
		
		// render the loaded model
        model = glm::mat4();
		/*model = glm::rotate(model, (float)glfwGetTime(),
		//model = glm::rotate(model, (float)glm::radians(180),
		                    glm::vec3(0.0, 1.0, 0.0));*/
		model = glm::translate(model, PPos);
		model = glm::rotate(model,
		                    glm::radians(PDeg.x += deltaTime * PAni.x),
		                    glm::vec3(1.0, 0.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(PDeg.y += deltaTime * PAni.y),
		                    glm::vec3(0.0, 1.0, 0.0));
		model = glm::rotate(model,
		                    glm::radians(PDeg.z += deltaTime * PAni.z),
		                    glm::vec3(0.0, 0.0, 1.0));
		lightingShader.setMat4("model", model);
        PModel.Draw(lightingShader);

        // also draw the lamp object
        lampShader.use();
        lampShader.setMat4("projection", projection);
        lampShader.setMat4("view", view);
        model = glm::mat4();
        model = glm::translate(model, lightPos);
        model = glm::scale(model, glm::vec3(0.2f)); // a smaller cube
        lampShader.setMat4("model", model);
		
        EModel.Draw(lampShader);

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

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        camera.ProcessKeyboard(DOWN, deltaTime);


	int a = (glfwGetKey(window,GLFW_KEY_C)==GLFW_PRESS)? 0 : 1;
	int s = 0;

	if (glfwGetKey(window,GLFW_KEY_Z) == GLFW_PRESS) s = -1;
	if (glfwGetKey(window,GLFW_KEY_X) == GLFW_PRESS) s = 1;
	
	float mv = s * MOVE_SPEED    * deltaTime;
	float rt = s * ROTATE_SPEED  * deltaTime;
	float an = s * ANIMATE_SPEED * deltaTime;

	if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
		*((a)?&EPos.x:&PPos.x) += mv;
	if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
		*((a)?&EPos.y:&PPos.y) += mv;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		*((a)?&EPos.z:&PPos.z) += mv;
		
	if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
		*((a)?&EDeg.x:&PDeg.x) += rt;
	if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
		*((a)?&EDeg.y:&PDeg.y) += rt;
	if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
		*((a)?&EDeg.z:&PDeg.z) += rt;
	
	if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS)
		*((a)?&EAni.x:&PAni.x) += an;
	if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
		*((a)?&EAni.y:&PAni.y) += an;
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		*((a)?&EAni.z:&PAni.z) += an;
	
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		EAni = glm::vec3(  0.0f,   0.0f,   0.0f);
		PAni = glm::vec3(  0.0f,   0.0f,   0.0f);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
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


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, GLdouble xpos, GLdouble ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
		// reversed since y-coordinates go
		// from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: when the mouse scroll wheel scrolls, this callback is called
// ------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, GLdouble xoffset,
                     GLdouble yoffset) {
    camera.ProcessMouseScroll(yoffset);
}
