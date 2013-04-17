#ifndef MODEL__H__
#define MODEL__H__

#include <stdexcept>
#include <fstream>
#include <string>
#include <cassert>
#include <sstream>
#include <memory>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ModelAsset.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "IObject.hpp"

class Model : public IObject
{
public:
	Model(void);
	~Model(void);

	void Initialize(std::shared_ptr<ModelAsset> pAsset, Toolbox* pToolbox);
	void Destroy(void);
	void Update(float deltaTime);
	void Render(Camera* pCamera);

	void SetPosition(const glm::vec3& rPos);
	void SetRotation(const glm::vec3& rRot);
	void SetScale(const glm::vec3& rScale);

	const glm::vec3& GetPosition(void);
	const glm::vec3& GetRotation(void);
	const glm::vec3& GetScale(void);

	std::shared_ptr<ModelAsset> GetAsset(void) { return m_pAsset; }

private:

	void UpdateTransform(void);

	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;

	std::shared_ptr<ModelAsset> m_pAsset;
};

#endif