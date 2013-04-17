#include "ModelAsset.hpp"
#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>

ModelAsset::ModelAsset(void)
{
	m_refCount = 0;
	m_drawType = 0;
	m_drawStart = 0;

	m_VAO = 0;
	m_VBO = 0;

	m_pTexture = NULL;
}


ModelAsset::~ModelAsset(void)
{
}


void ModelAsset::Load(Toolbox* pToolbox, const char* pFileName)
{
	m_pShaders = pToolbox->pProgram;

	// make and bind the VAO
    glGenVertexArrays(1, &m_VAO);
    glBindVertexArray(m_VAO);
    
    // make and bind the VBO
    glGenBuffers(1, &m_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	
	std::vector<Vertex> model;

	loadObjModel(model, pFileName);

	m_numVertices = model.size();

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * model.size(), &model[0], GL_STATIC_DRAW);

    // connect the xyz to the "vert" attribute of the vertex shader
    glEnableVertexAttribArray(m_pShaders->attrib("vert"));
    glVertexAttribPointer(m_pShaders->attrib("vert"), 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), NULL);
        
    // connect the uv coords to the "vertTexCoord" attribute of the vertex shader
    glEnableVertexAttribArray(m_pShaders->attrib("vertTexCoord"));
    glVertexAttribPointer(m_pShaders->attrib("vertTexCoord"), 2, GL_FLOAT, GL_TRUE,  sizeof(Vertex), (const GLvoid*)(3 * sizeof(GLfloat)));

	// connect the normals to the "vertNormal" attribute of the vertex shader
    glEnableVertexAttribArray(m_pShaders->attrib("vertNormal"));
    glVertexAttribPointer(m_pShaders->attrib("vertNormal"), 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex), (const GLvoid*)(5 * sizeof(GLfloat)));

    // unbind the VAO
    glBindVertexArray(0);

	// Load the texture
	Bitmap bmp = Bitmap::bitmapFromFile("data/textures/hammer.png");
    bmp.flipVertically();
    m_pTexture = new Texture(bmp);

	m_refCount++;
}

void ModelAsset::Destroy(void)
{
	if(m_refCount == 1) {
		// unbind the VAO, the program and the texture
		glBindVertexArray(0);
		glBindTexture(GL_TEXTURE_2D, 0);
		m_pShaders->stopUsing();
	}

	m_refCount--;
}

void ModelAsset::loadObjModel(std::vector<Vertex>& rModel, const char* pFileName)
{
	//open file
    std::ifstream f;
	f.open(pFileName, std::ios::in | std::ios::binary);
    if(!f.is_open()){
        throw std::runtime_error(std::string("Failed to open file: ") + pFileName);
    }

    //read whole file into stringstream buffer
    std::stringstream buffer;
    buffer << f.rdbuf();

	f.close();

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec3> normals;
	std::vector<std::string> faces;

	char buff[256];
	while(buffer.getline(buff,256)) {
		printf("%s\n", buff);

		char* pSpace = strchr(buff, ' ');

		if(!pSpace) { 
			continue;
		}

		int pos = pSpace - buff;
		if(pos < 0) {
			continue;
		}
			
		char* pPrefix = new char[pos+1];
		strncpy(pPrefix, buff, pos);
		pPrefix[pos] = '\0';

		if(strcmp(pPrefix, "v") == 0) {
			glm::vec3 pos;
			if(sscanf_s(pSpace, " %f %f %f",&pos.x,&pos.y,&pos.z)==3) {
				positions.push_back(pos);
			} else {
				printf("PROBLEM READING a POSITION\n");
			}
		} else if(strcmp(pPrefix, "vt") == 0) {
			glm::vec2 tex;
			if(sscanf_s(pSpace, " %f %f",&tex.x,&tex.y)==2) {
				texcoords.push_back(tex);
			} else {
				printf("PROBLEM READING a TEXCOORD\n");
			}
		} else if(strcmp(pPrefix, "vn") == 0) {
			glm::vec3 normal;
			if(sscanf_s(pSpace, " %f %f %f",&normal.x,&normal.y,&normal.z)==3) {
				normals.push_back(normal);
			} else {
				printf("PROBLEM READING a NORMAL\n");
			}
		} else if(strcmp(pPrefix, "f") == 0) {
			faces.push_back(std::string(pSpace));
		}
	}

	// loop over the face data and put together correct vertexes!
	for(size_t i = 0; i < faces.size(); ++i) {
		// every face contains 3 vertices in the format " v1/vt1/vn1 v2/vt2/vn2 v3/vt3/vn3"
		int v1, v2, v3;
		v1 = v2 = v3 = 0;
		int vt1, vt2, vt3;
		vt1 = vt2 = vt3 = 0;
		int vn1, vn2, vn3;
		vn1 = vn2 = vn3 = 0;

		sscanf_s(faces.at(i).c_str(), " %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3);

		// make three new vertices and put them into the buffer!
		rModel.push_back(Vertex(positions.at(v1-1), texcoords.at(vt1-1), normals.at(vn1-1)));
		rModel.push_back(Vertex(positions.at(v2-1), texcoords.at(vt2-1), normals.at(vn2-1)));
		rModel.push_back(Vertex(positions.at(v3-1), texcoords.at(vt3-1), normals.at(vn3-1)));

		printf("i:%d\n", i);
	}
	

	printf("DONE f: %d\n", faces.size());
}

