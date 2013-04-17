#ifndef MODELMANAGER__H__
#define MODELMANAGER__H__

#include "Model.hpp"
#include "ResourceManager.hpp"

class ModelManager
{
public:
	ModelManager(void);
	~ModelManager(void);

	void	Initialize(Toolbox* pToolbox);
	void	Destroy(void);
	void	Update(float deltaTime);
	void	Render(Camera* pCamera);

	Model*		CreateModel(const char* pAssetName);
	
	std::vector<Model*>		 m_Models;

private:

	Toolbox*					m_pToolbox;
};

#endif
