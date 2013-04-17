#ifndef SHADER__H__
#define SHADER__H__

#include <GL/glew.h>
#include <string>


/**
Represents a compiled OpenGL shader.
*/
class Shader { 
	public:
	
		// tdogl::Shader objects can be copied and assigned because they are reference counted
		// like a shared pointer
		Shader(const Shader& other);
		Shader& operator =(const Shader& other);
		~Shader();
        
		/**
		Creates a shader from a text file.
         
		@param filePath    The path to the text file containing the shader source.
		@param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
						or GL_FRAGMENT_SHADER.
         
		@throws std::exception if an error occurs.
		*/
		static Shader shaderFromFile(const std::string& filePath, GLenum shaderType);
        
        
		/**
		Creates a shader from a string of shader source code.
         
		@param shaderCode  The source code for the shader.
		@param shaderType  Same as the argument to glCreateShader. For example GL_VERTEX_SHADER
						or GL_FRAGMENT_SHADER.
         
		@throws std::exception if an error occurs.
		*/
		Shader(const std::string& shaderCode, GLenum shaderType);
        
        
		/**
		@result The shader's object ID, as returned from glCreateShader
		*/
		GLuint object() const;
                
	private:
		GLuint m_object;
		unsigned* m_pRefCount;
        
		void _retain();
		void _release();
};

#endif