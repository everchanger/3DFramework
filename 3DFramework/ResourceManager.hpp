#ifndef RESOURCEMANAGER__H__
#define RESOURCEMANAGER__H__

#include "Resource.hpp"
#include "ModelAsset.hpp"
#include "Toolbox.hpp"
#include <map> 
#include <memory> 
#include <string>

enum ResourceType {
	RT_TEXTURE = 0,
	RT_MODEL,
	RT_SOUND
};

class ResourceManager
{
public:
	ResourceManager(void);
	~ResourceManager(void);

	void Initialize(Toolbox* pToolbox);

	std::shared_ptr<Resource>	AddResource(const char* pFileName, const char* pResourceName, ResourceType resourceType);
	std::shared_ptr<Resource>	GetResource(const char* pResourceName);

private:

	std::map< std::string, std::shared_ptr<Resource> > m_Resources;

	Toolbox* m_pToolbox;
};

#endif