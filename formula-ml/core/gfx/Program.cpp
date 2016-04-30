#include "Program.h"
#include <fstream>
#include <sstream>
#include <vector>
#include<iostream>

#include <core/util/Util.h>

std::string textFileRead(const std::string& filePath)
{
	//open file
	std::ifstream f;
	f.open(filePath.c_str(), std::ios::in | std::ios::binary);
	if (!f.is_open())
	{
		throw std::runtime_error(std::string("Failed to open file: ") + filePath);
	}

	//read whole file into stringstream buffer
	std::stringstream buffer;
	buffer << f.rdbuf();

	return buffer.str();
}


GLuint CreateShader(std::string vertex_source, std::string fragment_source) {
	GLuint pid;
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	std::string vs = textFileRead(vertex_source);
	std::string fs = textFileRead(fragment_source);

    std::vector<std::string> attributes;
    std::stringstream parser;
    parser << vs;
    
    std::string word;
    while (parser >> word) {
        if (word == "in") {
            parser >> word;
            parser >> word;
            if (word.back() == ';') {
                word.pop_back();
            }
            attributes.push_back(word);
        }
    }
    

	const char * vsp = vs.c_str();
	const char * fsp = fs.c_str();
	glShaderSource(vertexShader, 1, &vsp, NULL);
	glShaderSource(fragmentShader, 1, &fsp, NULL);

	// Compile the shader, translates into internal representation and checks for errors.
	glCompileShader(vertexShader);
	GLint status;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(vertexShader, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(vertexShader); vertexShader = 0;
		printf(msg.c_str());
		throw std::runtime_error(msg);
	}


	// Compile the shader, translates into internal representation and checks for errors.
	glCompileShader(fragmentShader);
	// check for compiler errors in fragment shader.
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		std::string msg("Compile failure in shader:\n");

		GLint infoLogLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* strInfoLog = new char[infoLogLength + 1];
		glGetShaderInfoLog(fragmentShader, infoLogLength, NULL, strInfoLog);
		msg += strInfoLog;
		delete[] strInfoLog;

		glDeleteShader(fragmentShader); fragmentShader = 0;
		printf(msg.c_str());
		throw std::runtime_error(msg);
	}
    
	// Create a program object and attach the two shaders we have compiled, the program object contains
	// both vertex and fragment shaders as well as information about uniforms and attributes common to both.
	pid = glCreateProgram();
	glAttachShader(pid, fragmentShader);
	glAttachShader(pid, vertexShader);
    
    // Now that the fragment and vertex shader has been attached, we no longer need these two separate objects and should delete them.
    // The attachment to the shader program will keep them alive, as long as we keep the shaderProgram.
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);


    for (int i = 0; i < attributes.size(); i++) {
        glBindAttribLocation(pid, i, attributes[i].c_str());

    }
    
    glLinkProgram(pid);

    
	// Check for linker errors, many errors, such as mismatched in and out variables between 
	// vertex/fragment shaders,  do not appear before linking.
	{
		GLint linkOk = 0;
		glGetProgramiv(pid, GL_LINK_STATUS, &linkOk);
        
		if (linkOk == GL_FALSE)
		{
			std::string msg("Compile failure in shader:\n");

			GLint infoLogLength;
			glGetShaderiv(pid, GL_INFO_LOG_LENGTH, &infoLogLength);
			char* strInfoLog = new char[infoLogLength + 1];
			glGetShaderInfoLog(pid, infoLogLength, NULL, strInfoLog);
			msg += strInfoLog;
			delete[] strInfoLog;

			glDeleteShader(vertexShader); vertexShader = 0;
			printf(msg.c_str());
			throw std::runtime_error(msg);
		}
        
	}

    util::gl_error_check("Shader loading");
	return pid;

}
