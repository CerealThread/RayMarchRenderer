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

#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H


#include "GL/glew.h"
#include <string>
#include "glm/glm.hpp"
#include <map>

using namespace std;

class Shader {
public:
	Shader();
	~Shader();

	enum ShaderType {
		VERTEX,
		FRAGMENT,
		PROGRAM
	};
	
	bool loadShaders(const char* vsFilename, const char* fsFilename);
	void use();

	void setUniform(const std::string name, const glm::vec2& v);
	void setUniform(const std::string name, const glm::vec3& v);
	void setUniform(const std::string name, const glm::vec4& v);
	void setUniform(const std::string name, const glm::mat4& m);
	void setUniform(const std::string name, const GLfloat& f);
	void setUniform(const std::string name, const GLint v);
	void setUniform(const std::string name, const unsigned int v);
	void setUniformSampler(const std::string name, const GLint& slot);





	GLuint getProgram() const;

private:

	string fileToString(const string& filename);
	void checkCompileErrors(GLuint shader, ShaderType type);
	GLint getUniformLocation(const std::string name);

	GLuint mHandle;
	std::map<string, GLint> mUniformLocations;
};

#endif
