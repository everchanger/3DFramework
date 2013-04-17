#ifndef MODELASSET__H__
#define MODELASSET__H__

#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include "Program.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Resource.hpp"

struct Vertex {
	Vertex() : x(0.0f), y(0.0f), z(0.0f), u(0.0f), v(0.0f) {};
	Vertex(glm::vec3& pos, glm::vec2& tex, glm::vec3& norm) : x(pos.x), y(pos.y), z(pos.z), u(tex.x), v(tex.y), nx(norm.x), ny(norm.y), nz(norm.z) {};
	Vertex(GLfloat X, GLfloat Y, GLfloat Z, GLfloat U, GLfloat V) : x(X), y(Y), z(Z), u(U), v(V) {};

	bool compare(Vertex& rV) {
		float epsilon = 0.001f;

		if(fabs(rV.x - x) < epsilon) {
			if(fabs(rV.y - y) < epsilon) {
				if(fabs(rV.z - z) < epsilon) {
					if(fabs(rV.u - u) < epsilon) {
						if(fabs(rV.v - v) < epsilon) {
							// ADD NORMALS
							return true;
						}	
					}	
				}	
			}	
		}

		return false;
	}

	GLfloat x, y, z;
	GLfloat u, v;
	GLfloat nx, ny, nz;
};

class ModelAsset : public Resource
{
public:
	ModelAsset(void);
	~ModelAsset(void);

	void Load(Toolbox* pToolbox, const char* pFileName);
	void Destroy(void);

	void loadObjModel(std::vector<Vertex>& rModel, const char* pFileName);

	Program* GetShaders(void)		{ return m_pShaders; }
	Texture* GetTexture(void)		{ return m_pTexture; }
	GLuint	 GetVAO(void)			{ return m_VAO; }
	GLuint	 GetNumVertices(void)	{ return m_numVertices; }

private:

	GLuint m_VAO;
	GLuint m_VBO;
	GLuint m_IBO;
	GLuint m_numVertices;

	GLenum m_drawType;
    GLint m_drawStart;
    GLint m_refCount;

	Texture* m_pTexture;
	Program* m_pShaders;
};

#endif