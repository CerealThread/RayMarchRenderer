// MIT License

// Copyright (c) 2019 Packt

// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:

// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.

// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "Shader.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include "glm/gtc/type_ptr.hpp"

Shader::Shader() : mHandle(0) {}

Shader::~Shader() {
    glDeleteProgram(mHandle);
}

bool Shader::loadShaders(const char* vsFilename, const char* fsFilename) {

    string vsString = fileToString(vsFilename);
    string fsString = fileToString(fsFilename);
    const GLchar* vsSourcePtr = vsString.c_str();
    const GLchar* fsSourcePtr = fsString.c_str();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vs, 1, &vsSourcePtr, NULL);
    glShaderSource(fs, 1, &fsSourcePtr, NULL);

    glCompileShader(vs);
    checkCompileErrors(vs, VERTEX);

    glCompileShader(fs);
    checkCompileErrors(fs, FRAGMENT);


    mHandle = glCreateProgram();
    glAttachShader(mHandle, vs);
    glAttachShader(mHandle, fs);
    glLinkProgram(mHandle);

    checkCompileErrors(mHandle, PROGRAM);

    glDeleteShader(vs);
    glDeleteShader(fs);

    mUniformLocations.clear();
	return true;
}

void Shader::use() {
	if (mHandle > 0) {
		glUseProgram(mHandle);
	}
}

string Shader::fileToString(const string& filename) {
	std::stringstream ss;
	std::ifstream file;

	try {
		file.open(filename, std::ios::in);

		if (!file.fail()) {
			ss << file.rdbuf();
		}

		file.close();
	}
	catch (std::exception ex) {
		std::cerr << "Error reading shader filename" << std::endl;
	}

	return ss.str();
}

void Shader::checkCompileErrors(GLuint shader, ShaderType type) {

    int status = 0;

    if (type == PROGRAM) {

        glGetProgramiv(mHandle, GL_LINK_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetProgramiv(mHandle, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetProgramInfoLog(mHandle, length, &length, &errorLog[0]);
            std::cerr << "Error! Program failed to link." << errorLog << std::endl;
        }
    }
    else //VERTEX or FRAGMENT
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
        if (status == GL_FALSE) {
            GLint length = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);

            string errorLog(length, ' ');
            glGetShaderInfoLog(shader, length, &length, &errorLog[0]);
            std::cerr << "Error! Shader failed to compile." << errorLog << std::endl;
        }
    }
}

GLuint Shader::getProgram() const {
    return mHandle;
}


GLint Shader::getUniformLocation(const std::string name) {
    std::map <string, GLint>::iterator it = mUniformLocations.find(name.c_str());

    if (it == mUniformLocations.end()) {
        mUniformLocations[name] = glGetUniformLocation(mHandle, name.c_str());
    }

    return mUniformLocations[name];
}

void Shader::setUniform(const std::string name, const glm::vec2& v) {
    GLint loc = getUniformLocation(name.c_str());
    glUniform2f(loc, v.x, v.y);
}
void Shader::setUniform(const std::string name, const glm::vec3& v) {
    GLint loc = getUniformLocation(name.c_str());
    glUniform3f(loc, v.x, v.y, v.z);
}
void Shader::setUniform(const std::string name, const glm::vec4& v) {
    GLint loc = getUniformLocation(name.c_str());
    glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const std::string name, const glm::mat4& m) {
    GLint loc = getUniformLocation(name.c_str());
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(m));
}



void Shader::setUniform(const std::string name, const GLfloat& f)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1f(loc, f);
}

void Shader::setUniform(const std::string name, const GLint v)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1i(loc, v);
}

void Shader::setUniform(const std::string name, const unsigned int v)
{
    GLint loc = getUniformLocation(name.c_str());
    glUniform1ui(loc, v);
}


void Shader::setUniformSampler(const std::string name, const GLint& slot)
{
    glActiveTexture(GL_TEXTURE0 + slot);
    GLint loc = getUniformLocation(name.c_str());
    glUniform1i(loc, slot);
}

