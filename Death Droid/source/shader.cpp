#include "shader.h"

unsigned int compileShader(unsigned int type, const std::string& source)
{
	//create unique shader id, link that with source,
	//compile resulting shader, output errors, and return id

	unsigned int id = glCreateShader(type);
	const char* cSource = source.c_str();
	glShaderSource(id, 1, &cSource, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		int errorLength;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &errorLength);
		char error[200];
		glGetShaderInfoLog(id, 200, &errorLength, error);

		std::cout << "Failed to compile shader: " << error << std::endl;
	}

	return id;
}

unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	//create a program, compile vertex and fragment
	//shaders, add shaders to the program, delete
	//the two shaders, and return program

	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}

unsigned int createShader(const std::string& vertexShader, const std::string& geometryShader, const std::string& fragmentShader)
{
	//create a program, compile vertex, geometry, and fragment
	//shaders, add shaders to the program, delete
	//the two shaders, and return program

	unsigned int program = glCreateProgram();

	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int gs = compileShader(GL_GEOMETRY_SHADER, geometryShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, gs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);

	return program;
}

std::string loadShader(std::string path)
{
	//returns a string of the (shader) file dictated by path

	std::ifstream stream(path);
	std::stringstream shader;

	shader << stream.rdbuf();
	return shader.str();
}