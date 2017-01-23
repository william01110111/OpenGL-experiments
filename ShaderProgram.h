#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "utils.h"

class ShaderProgram
{
public:
	
	ShaderProgram(string vertFile, string fragFile, bool debug);
	inline operator GLuint() {return data;}
	~ShaderProgram();
	
private:
	
	class Shader
	{
	public:
		Shader(string file, GLenum type, bool debug);
		inline operator GLuint() {return data;}
		~Shader();
		
	private:
		string getInfoLog();
		GLuint data;
	};
	
	string getInfoLog();
	
	GLuint data;
};

