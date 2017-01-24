#pragma once

#include "utils.h"
class GLFWwindow;

class Window
{
public:
	Window();
	~Window();
	
	void open(int w, int h, string name);
	void close();
	
	bool update(); //returns true while the window is open
	
private:
	
	static int instanceCount;
	string name;
	
	GLFWwindow * windowObj=nullptr;
};
