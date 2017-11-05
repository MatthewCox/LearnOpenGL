#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>

#include "shader.h"
#include "texture.h"
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

	// set up vertex data (and buffers) and configure vertex attributes
	const int sizeOfPosition = 3;
	const int sizeOfColour = 3;
	const int sizeOfTexCoord = 2;
	const int sizeOfOneVertex = sizeOfPosition + sizeOfColour + sizeOfTexCoord;
#if 0
	// triangle
	float vertices[] = {
		// positions         // colours         // texture coords
		 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  1.0f, 1.0f,  // top
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left
	};
	unsigned int indices[] = {
		0, 1, 2,
	};
#else
	// rectangle
	float vertices[] = {
		// positions         // colours         // texture coords
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f,  1.0f, 1.0f,  // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 0.0f,  0.0f, 1.0f,  // top left
	};
	unsigned int indices[] = {
		0, 1, 3, // first triangle
		1, 2, 3, // second triangle
	};
#endif
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	// bind VAO first to store all the following config
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
	// position attrib
	glVertexAttribPointer(0, sizeOfPosition, GL_FLOAT, GL_FALSE, sizeOfOneVertex * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// colour attrib
	glVertexAttribPointer(1, sizeOfColour, GL_FLOAT, GL_FALSE, sizeOfOneVertex * sizeof(float), (void*)(sizeOfPosition * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texcoord attrib
	glVertexAttribPointer(2, sizeOfTexCoord, GL_FLOAT, GL_FALSE, sizeOfOneVertex * sizeof(float), (void*)((sizeOfPosition + sizeOfColour) * sizeof(float)));
	glEnableVertexAttribArray(2);

	// load texture
	Texture texture = Texture("textures/container.jpg");

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

		// set up model-view-projection matrices
		glm::mat4 model = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 projection = glm::mat4(1.0f);
		model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		view = camera.getViewMatrix();
		projection = glm::perspective(glm::radians(45.0f), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
		shader.setMat4("model", model);
		shader.setMat4("view", view);
		shader.setMat4("projection", projection);

		// draw things
		shader.use();
		texture.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);

		// swap buffers and poll inputs
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// clean up GPU resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

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