#include <iostream>
#include <fstream>
#include "GLProgram.h"

#pragma warning(disable : 4996)

char *GLProgram::readShaderSource(string shaderFile)
{
	FILE *fp = fopen(shaderFile.c_str(), "rb");

	if (fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char *buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLProgramInstance *GLProgram::createProgram(std::string programName, string vertexfilename, string fragmentfilename)
{
	GLenum ErrorCheckValue = glGetError();
	int success;
	char infoLog[512];

	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Creiamo gli eseguibili degli shader
	// Leggiamo il codice del Vertex Shader
	GLchar *VertexShader = readShaderSource(vertexfilename);
	// Visualizzo sulla console il CODICE VERTEX SHADER
	// cout << VertexShader;

	// Generiamo un identificativo per il vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// Associamo all'identificativo il codice del vertex shader
	glShaderSource(vertexShaderId, 1, (const char **)&VertexShader, NULL);
	// Compiliamo il Vertex SHader
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Leggiamo il codice del Fragment Shader
	const GLchar *FragmentShader = readShaderSource(fragmentfilename);
	// Visualizzo sulla console il CODICE FRAGMENT SHADER
	// cout << FragmentShader;

	// Generiamo un identificativo per il FRAGMENT shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char **)&FragmentShader, NULL);
	// Compiliamo il FRAGMENT SHader
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	ErrorCheckValue = glGetError();
	cout << "Compilation result:" << ErrorCheckValue << endl;

	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue));

		exit(-1);
	}
	// Creiamo un identificativo di un eseguibile e gli colleghiamo i due shader compilati
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	//////////////

	GLProgramInstance *instance = new GLProgramInstance(programId);

	this->programs.insert({programName, instance});

	return instance;
}

GLProgramInstance *GLProgram::getProgram(std::string programName)
{
	return programs.find(programName)->second;
}

size_t GLProgram::programsCount()
{
	return programs.size();
}

map<string, GLProgramInstance *>::iterator GLProgram::getIterator()
{
	return programs.begin();
}

void GLProgram::useProgram(GLProgramInstance *instance)
{
	usedProgram = instance;
	instance->useProgram();
}

void GLProgram::useProgram(string programName)
{
	useProgram(getProgram(programName));
}

GLProgramInstance *GLProgram::getUsedProgram()
{
	return usedProgram;
}

GLProgramInstance::GLProgramInstance(GLuint id)
{
	this->id = id;
}

void GLProgramInstance::useProgram()
{
	glUseProgram(id);
	glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, value_ptr(projection));
	glUniform2f(getUniformLocation("resolution"), resolution.x, resolution.y);
}

void GLProgramInstance::setProjectionMatrix(mat4 matrix)
{
	projection = matrix;
}

void GLProgramInstance::setResolution(vec2 res)
{
	resolution = res;
}

GLuint GLProgramInstance::getID()
{
	return id;
}

GLint GLProgramInstance::getUniformLocation(string varName)
{
	return glGetUniformLocation(id, varName.c_str());
}