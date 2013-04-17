#include "Transform.hpp"


Transform::Transform(void)
{
	m_Local = glm::mat4();
	m_World = glm::mat4();

	m_pParent = NULL;

	m_isDirty = false;
}


Transform::~Transform(void)
{
}

void Transform::Update(void)
{
	// we need to make sure that the parent is updated if we do it like this.

	if(m_pParent) {
		if(m_pParent->isDirty()) {
			m_pParent->Update();
		}
		m_World = m_pParent->WorldMatrix() * m_Local;
	} else {
		m_World = m_Local;
	}

	m_isDirty = false;
}

glm::mat4& 
Transform::LocalMatrix(void)	
{
	MakeChainDirty();
	return m_Local; 
}

glm::mat4& 
Transform::WorldMatrix(void)	
{
	return m_World; 
}

bool	   
Transform::isDirty(void)		
{ 
	return m_isDirty;
}

Transform*  
Transform::Parent(void)	
{
	return m_pParent;
}

void			
Transform::SetParent(Transform* pParent)	
{ 
	if(pParent) {
		m_pParent = pParent; 
		pParent->AddChild(this);
	}
}

void	
Transform::AddChild(Transform* pChild)		
{
	m_pChilds.push_back(pChild); 
}

void		
Transform::MakeChainDirty(void)
{
	m_isDirty = true;

	for(size_t i = 0; i < m_pChilds.size(); ++i) {
		m_pChilds[i]->MakeChainDirty();
	}
}