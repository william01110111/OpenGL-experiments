
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
	
	window.open(1200, 1200, "hello window");
	
	while (window.update())
	{
		window.drawRect();
	}
	
	return 0;
}
