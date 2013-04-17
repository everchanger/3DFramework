#include "Shader.hpp"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

Shader::Shader(const std::string& shaderCode, GLenum shaderType) : m_object(0), m_pRefCount(NULL)
{
    //create the shader object
    m_object = glCreateShader(shaderType);
    if(m_object == 0) {
        throw std::runtime_error("glCreateShader failed");
	}
    
    //set the source code
    const char* code = shaderCode.c_str();
    glShaderSource(m_object, 1, (const GLchar**)&code, NULL);
    
    //compile
    glCompileShader(m_object);
    
    //throw exception if compile error occurred
    GLint status;
    glGetShaderiv(m_object, GL_COMPILE_STATUS, &status);
    if (status == GL_FALSE) {
        std::string msg("Compile failure in shader:\n");
        
        GLint infoLogLength;
        glGetShaderiv(m_object, GL_INFO_LOG_LENGTH, &infoLogLength);
        
		char* strInfoLog = new char[infoLogLength + 1];
        glGetShaderInfoLog(m_object, infoLogLength, NULL, strInfoLog);
        msg += strInfoLog;
        delete[] strInfoLog;
        
        glDeleteShader(m_object); 
		m_object = 0;
        throw std::runtime_error(msg);
    }
    
    m_pRefCount = new unsigned;
    *m_pRefCount = 1;
}

Shader::Shader(const Shader& other) : m_object(other.m_object), m_pRefCount(other.m_pRefCount)
{
    _retain();
}

Shader::~Shader() {
    //_refCount will be NULL if constructor failed and threw an exception
    if(m_pRefCount) {
		_release();
	}
}

GLuint Shader::object() const {
    return m_object;
}

Shader& Shader::operator = (const Shader& other) {
   _release();

    m_object = other.m_object;
    m_pRefCount = other.m_pRefCount;
    
	_retain();

    return *this;
}

Shader Shader::shaderFromFile(const std::string& filePath, GLenum shaderType) {
    //open file
    std::ifstream f;
    f.open(filePath.c_str(), std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + filePath);
    }

    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();

    //return new shader
    Shader shader(buffer.str(), shaderType);
    return shader;
}

void Shader::_retain() {
    assert(m_pRefCount);
    *m_pRefCount += 1;
}

void Shader::_release() {
    assert(m_pRefCount && *m_pRefCount > 0);
    
	*m_pRefCount -= 1;
    if(*m_pRefCount == 0){
        glDeleteShader(m_object); m_object = 0;

        delete m_pRefCount; 
		m_pRefCount = NULL;
    }
}

