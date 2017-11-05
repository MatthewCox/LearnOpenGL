#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "model.h"
#include "camera.h"

// forward declarations
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void mouseCallback(GLFWwindow* window, double xPos, double yPos);
void processInput(GLFWwindow *window);

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

float mouseLastX = static_cast<float>(WINDOW_WIDTH / 2);
float mouseLastY = static_cast<float>(WINDOW_HEIGHT / 2);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main(int argc, char** argv)
{
	// set up GLFW and the OpenGL version we are supporting
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // needed for Mac OS X
#endif

	// create our GLFW window
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// glfw window configuration
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// register callbacks
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback); // window resize
	glfwSetCursorPosCallback(window, mouseCallback); // mouse position

	// init OpenGL function pointers with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global OpenGL state
	glEnable(GL_DEPTH_TEST);

	Shader shader = Shader("shaders/shader.vs", "shaders/shader.fs");

	Model nanosuit = Model("models/nanosuit/nanosuit.obj");

	// uncomment for wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// frame time
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		// handle inputs
		processInput(window);

		// render
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f); // Cornflower Blue, of course
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		shader.use();

		// set up model-view-projection matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// draw things
		nanosuit.draw(shader);

		// swap buffers and poll inputs
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up glfw
	glfwTerminate();
	return 0;
}

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow * window, double xPos, double yPos)
{
	float fXPos = static_cast<float>(xPos);
	float fYPos = static_cast<float>(yPos);

	static bool firstMouseMovement = true;
	if (firstMouseMovement)
	{
		mouseLastX = fXPos;
		mouseLastY = fYPos;
		firstMouseMovement = false;
	}

	float xOffset = fXPos - mouseLastX;
	float yOffset = mouseLastY - fYPos;

	mouseLastX = fXPos;
	mouseLastY = fYPos;

	camera.processMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.processKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.processKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.processKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.processKeyboard(RIGHT, deltaTime);
}