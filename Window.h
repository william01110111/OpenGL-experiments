#pragma once

#include "utils.h"
#include "ShaderProgram.h"
class GLFWwindow;

class Window
{
public:
	Window();
	~Window();
	
	void open(int w, int h, string name);
	void close();
	
	bool update(); //returns true while the window is open
	void drawRect();
	
private:
	
	void setupGL();
	void setupTexture();
	
	static int instanceCount;
	string name;
	ShaderProgram shaderProgram;
	GLuint squareVAO;
	
	GLFWwindow * windowObj=nullptr;
};
