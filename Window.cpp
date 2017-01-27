#include <unordered_map>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <SOIL/SOIL.h>

#include "Window.h"
//written using https://learnopengl.com/

std::unordered_map<GLFWwindow*, Window*> windowMap;

int Window::instanceCount=0;

Window* getWindow(GLFWwindow* window)
{
	return windowMap[window];
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (action==GLFW_PRESS)
	{
		if (key==GLFW_KEY_ESCAPE || key==GLFW_KEY_ENTER)
		{
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		
		if (key==GLFW_KEY_PERIOD)
		{
			getWindow(window)->blurRds*=1.1;
		}
		
		if (key==GLFW_KEY_COMMA)
		{
			getWindow(window)->blurRds*=0.9;
		}
	}
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
	windowMap[windowObj]=this;
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
		windowMap.erase(windowObj);
		glfwDestroyWindow(windowObj);
		windowObj=nullptr;
	}
}

void Window::setupGL()
{
	shaderProgram=ShaderProgram("shader0.vert", "shader0.frag", false);
	
	GLuint VBO, EBO;
	int width, height;
	
	GLfloat vertices[] = {
		//Positions				Colors					Texture Coords
		1.0f,	1.0f,	0.0f,	0.0f,	1.0f,	0.0f,	1.0f,	0.0f,	// Top Right
		1.0f,	-1.0f,	0.0f,	0.0f,	0.5f,	0.5f,	1.0f,	1.0f,	// Bottom Right
		-1.0f,	-1.0f,	0.0f,	0.0f,	0.0f,	1.0f,	0.0f,	1.0f,	// Bottom Left
		-1.0f,	1.0f,	0.0f,	0.0f,	0.0f,	0.5f,	0.0f,	0.0f,	// Top Left 
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
			
			glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
			
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
			glEnableVertexAttribArray(0);
			
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(1);
			
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6*sizeof(GL_FLOAT)));
			glEnableVertexAttribArray(2);
		
		glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
		//remember: do NOT unbind the EBO, keep it bound to this VAO
		
	glBindVertexArray(false); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)
	
	//deleting these objects appears to work, not sure if its supposed to though
	//glDeleteBuffers(1, &VBO);
	//glDeleteBuffers(1, &EBO);
	
	setupTexture();
}

void Window::setupTexture()
{
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	
	// Set the texture wrapping/filtering options (on the currently bound texture object)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	
	// Load and generate the texture
	int width, height;
    unsigned char* image = SOIL_load_image("/home/william/c++/GLFW_test/orange.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    if (!image)
	{
		cout << "image loading error: " << SOIL_last_result() << endl;
	}
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.
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
	cout << blurRds << endl;
	
	// Draw our first triangle
	glUseProgram(shaderProgram);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(squareVAO);
	blurRds*=1.1;
	glUniform1f(1, blurRds);
	
	//glDrawArrays(GL_TRIANGLES, 0, 6);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	glBindVertexArray(false);
}

//get any new input
//void Window::updateInput();

