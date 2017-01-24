
//written using https://learnopengl.com/

/// linker settings:
/*
-lGLEW
-lglfw
-lGL
*/

#include "Window.h"

int main()
{
	Window window;
	
	window.open(600, 800, "hello window");
	
	while (window.update()) {}
	
	return 0;
}
