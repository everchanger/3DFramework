#include "Model.hpp"

Model::Model(void)
{
	m_pAsset = NULL;
}


Model::~Model(void)
{
}

void Model::Initialize(std::shared_ptr<ModelAsset> pAsset, Toolbox* pToolbox)
{
	IObject::Initialize(pToolbox);

	m_pAsset = pAsset;

	m_Scale = glm::vec3(1.0f);
}

void Model::Destroy(void)
{
	if(m_pAsset) {
		m_pAsset->Destroy();
	}
}

void Model::Update(float deltaTime)	
{
	if(IsUpToDate()) {
		return;
	}

	UpdateTransform();

	IObject::Update(deltaTime);
}

void Model::Render(Camera* pCamera)
{
	Program* pShaders = m_pAsset->GetShaders();
	
	// bind the program (the shaders)
    pShaders->use();

    // set the "camera" uniform
	pShaders->setUniform("camera", pCamera->CameraMatrix());

    // set the "model" uniform in the vertex shader, based on the gDegreesRotated global
	pShaders->setUniform("model", m_transform.WorldMatrix());
        
    // bind the texture and set the "tex" uniform in the fragment shader
    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_pAsset->GetTexture()->object());
    pShaders->setUniform("tex", 0); //set to 0 because the texture is bound to GL_TEXTURE0

    // bind the VAO (the triangle)
	glBindVertexArray(m_pAsset->GetVAO());
    
    // draw the VAO
    glDrawArrays(GL_TRIANGLES, 0, m_pAsset->GetNumVertices());
    
    // unbind the VAO, the program and the texture
    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
    pShaders->stopUsing();

}

void Model::SetPosition(const glm::vec3& rPos)
{
	m_Position = rPos;
}

void Model::SetRotation(const glm::vec3& rRot)
{
	m_Rotation = rRot;
}

void Model::SetScale(const glm::vec3& rScale)
{
	m_Scale = rScale;
}

const glm::vec3& 
Model::GetPosition(void)
{
	return m_Position;
}

const glm::vec3& 
Model::GetRotation(void)
{
	return m_Rotation;
}

const glm::vec3&
Model::GetScale(void)
{
	return m_Scale;
}

void Model::UpdateTransform(void)
{
	glm::mat4 orientation;

	/*http://www.arcsynthesis.org/gltut/Positioning/Tut06%20Fun%20with%20Matrices.html*/

	// translate
 	orientation = glm::translate(orientation, m_Position);

	// rotate
	orientation = glm::rotate(orientation, m_Rotation.x, glm::vec3(1,0,0));
    orientation = glm::rotate(orientation, m_Rotation.y, glm::vec3(0,1,0));
	orientation = glm::rotate(orientation, m_Rotation.z, glm::vec3(0,0,1));

	// scale( if you add this, add it here )
	m_transform.LocalMatrix() = glm::scale(orientation, m_Scale); 
}