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

	Shader objectShader = Shader("shaders/objectShader.vs", "shaders/objectShader.fs");
	Shader lightShader = Shader("shaders/lightShader.vs", "shaders/lightShader.fs");

	Model lightbulb = Model("models/bulb/bulb.obj");
	Model nanosuit = Model("models/nanosuit/nanosuit.obj");

	glm::vec3 lightbulbPos = glm::vec3(1.2f, 1.0f, 2.0f);
	glm::vec3 nanosuitPos = glm::vec3(0.0f, -1.75f, 0.0f);

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
		glClearColor(0.01f, 0.01f, 0.01f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		objectShader.use();
		objectShader.setVec3("viewPos", camera.getPosition());
		objectShader.setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
		objectShader.setVec3("lightPos", lightbulbPos);

		// set up model-view-projection matrices
		glm::mat4 view = camera.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);

		objectShader.setMat4("view", view);
		objectShader.setMat4("projection", projection);

		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, nanosuitPos);
		model = glm::scale(model, glm::vec3(0.2f));
		objectShader.setMat4("model", model);

		nanosuit.draw(objectShader);

		lightShader.use();
		lightShader.setVec3("lightColour", glm::vec3(1.0f, 1.0f, 1.0f));
		lightShader.setMat4("view", view);
		lightShader.setMat4("projection", projection);

		model = glm::mat4(1.0f);
		model = glm::translate(model, lightbulbPos);
		model = glm::scale(model, glm::vec3(0.002f));
		lightShader.setMat4("model", model);

		// draw things
		lightbulb.draw(lightShader);

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