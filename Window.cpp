#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Window.h"
#include "Image.h"
//written using https://learnopengl.com/

int Window::instanceCount=0;

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

Window::Window()
{
	if (instanceCount==0)
	{
		// Init GLFW
		glfwInit();
		// Set all the required options for GLFW
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	}
	
	instanceCount++;
}

Window::~Window()
{
	close();
	
	instanceCount--;
	
	if (instanceCount==0)
	{
		glfwTerminate();
	}
}

//open the window with the given dimensions and name
void Window::open(int w, int h, string nameIn)
{
	name=nameIn;
	
	windowObj = glfwCreateWindow(w, h, name.c_str(), nullptr, nullptr);
	glfwMakeContextCurrent(windowObj);
	
	glfwSetKeyCallback(windowObj, key_callback);
	
	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();
	
	setupGL();
}

//close the window
void Window::close()
{
	if (windowObj)
	{
		glfwDestroyWindow(windowObj);
		windowObj=nullptr;
	}
}

void Window::setupGL()
{
	shaderProgram=ShaderProgram("shader0.vert", "shader0.frag", false);
	
	GLuint VBO, EBO;
	int width, height;
	
	GLfloat vertices[] =
	{
		0.5f,  0.5f, 0.0f,  // Top Right
		0.5f, -0.5f, 0.0f,  // Bottom Right
		-0.5f, -0.5f, 0.0f,  // Bottom Left
		-0.5f,  0.5f, 0.0f   // Top Left
	};
	
	GLuint indices[] =
	{
		0, 1, 3,  // First Triangle
		1, 2, 3   // Second Triangle
	};
	
	glfwGetFramebufferSize(windowObj, &width, &height);
	glViewport(0, 0, width, height);
	
	glGenVertexArrays(1, &squareVAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);
	
	// Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
	glBindVertexArray(squareVAO);
		
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
		//remember: do NOT unbind the EBO, keep it bound to this VAO
		
	glBindVertexArray(false); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	//deleting these objects appears to work, not sure if its supposed to though
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	
	setupTexture();
}

void Window::setupTexture()
{
	//GLuint texture;
	//glGenTextures(1, &texture);
	//glBindTexture(GL_TEXTURE_2D, texture);
	//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
}

//refresh the window
bool Window::update()
{
	if (!windowObj)
		return false;
		
	// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
	glfwPollEvents();
	
	// Swap the screen buffers
	glfwSwapBuffers(windowObj);
	
	// Render
	// Clear the colorbuffer
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	if (glfwWindowShouldClose(windowObj))
		close();
		
	return true;
}

void Window::drawRect()
{
	// Draw our first triangle
	glUseProgram(shaderProgram);
	glBindVertexArray(squareVAO);
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(false);
}

//get any new input
//void Window::updateInput();

