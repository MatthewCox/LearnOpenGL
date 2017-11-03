#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "shader.h"
#include "texture.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

// forward declarations
void framebufferSizeCallback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

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
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "ModernOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	// register window resize callback
	glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);

	// init GL function pointers with glad
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// set our viewport dimensions
	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	Shader shader = Shader("shaders/shader.vs", "shaders/shader.fs");

	// set up vertex data (and buffers) and configure vertex attributes
#if 1
	// triangle
	float vertices[] = {
		// positions         // colours         // texture coords
		 0.0f,  0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // top
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  // bottom left
	};
	unsigned int indices[] = {
		0, 1, 2,
	};
#else
	// rectangle
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  1.0f, 0.0f, 1.0f, // top right
		 0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f, // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f, // bottom left
		-0.5f,  0.5f, 0.0f,  1.0f, 1.0f, 1.0f, // top left
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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// colour attrib
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// uncomment for wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// handle inputs
		processInput(window);

		// render
		glClearColor(0.39f, 0.58f, 0.93f, 1.0f); // Cornflower Blue, of course
		glClear(GL_COLOR_BUFFER_BIT);

		// draw triangle
		shader.use();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

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

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}