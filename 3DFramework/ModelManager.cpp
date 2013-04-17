#include "ModelManager.hpp"


ModelManager::ModelManager(void)
{
	m_pToolbox = NULL;
}


ModelManager::~ModelManager(void)
{
}

void
ModelManager::Initialize(Toolbox* pToolbox) {
	if(pToolbox) {
		m_pToolbox = pToolbox;
	}
}

void ModelManager::Destroy(void)
{
	// start with destroying the models, then the assets
	for(int i=0; i<m_Models.size(); ++i) {
		if(m_Models[i]) {
			m_Models[i]->Destroy();
			delete m_Models[i];
			m_Models[i] = NULL;
		}
	}
	m_Models.clear();
}

void ModelManager::Update(float deltaTime)
{
	for(int i=0; i<m_Models.size(); ++i) {
		if(m_Models[i]) {
			m_Models[i]->Update(deltaTime);
		}
	}
}

void ModelManager::Render(Camera* pCamera)
{
	for(int i=0; i<m_Models.size(); ++i) {
		if(m_Models[i]) {
			m_Models[i]->Render(pCamera);
		}
	}
}

Model* ModelManager::CreateModel(const char* pAssetName)
{
	// create a new instance of a model asset
	Model* pModel = new Model();

	// get a asset
	std::shared_ptr<ModelAsset> pAsset = std::static_pointer_cast<ModelAsset>(m_pToolbox->pResourceManager->GetResource(pAssetName));
	if(pAsset == 0) {
		return 0;
	}

	pModel->Initialize(pAsset,m_pToolbox);
	m_Models.push_back(pModel);

	return pModel;
}