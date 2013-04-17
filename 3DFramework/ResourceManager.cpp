#include "ResourceManager.hpp"


ResourceManager::ResourceManager(void)
{
}


ResourceManager::~ResourceManager(void)
{
}

void 
ResourceManager::Initialize(Toolbox* pToolbox)
{
	m_pToolbox = pToolbox;
}

std::shared_ptr<Resource>					
ResourceManager::AddResource(const char* pFileName, const char* pResourceName, ResourceType resourceType) 
{
	auto it = m_Resources.find(pResourceName) ;
	if(it != m_Resources.end()) {
		return it->second;
	}  

	std::shared_ptr<Resource> pResource;

	switch(resourceType) {
	case RT_MODEL:
		pResource = std::make_shared<ModelAsset>();		
		break;
	case RT_TEXTURE:
		break;
	case RT_SOUND:
		break;
	};
	
	pResource->Load(m_pToolbox, pFileName);

	m_Resources[pResourceName] = pResource;

	return m_Resources[pResourceName];
}

std::shared_ptr<Resource>	
ResourceManager::GetResource(const char* pResourceName) 
{
	auto it = m_Resources.find(pResourceName) ;
	if(it != m_Resources.end()) {
		return it->second;
	}

	return NULL;
}