#include "IObject.hpp"


IObject::IObject(void)
{
	m_pParent		= NULL;
	m_currentFrame	= 0;
	m_pToolbox		= NULL;
}


IObject::~IObject(void)
{
}

void
IObject::ChildTo(IObject* pObject)
{
	if(pObject) {
		m_pParent = pObject;
		m_transform.SetParent(m_pParent->GetTransform());
	}
}

bool 
IObject::IsUpToDate(void) {
	if(m_currentFrame == m_pToolbox->frameNumber) {
		// has been updated this frame, no need to update again
		return true;
	}
	return false;
}

void
IObject::Initialize(Toolbox* pToolbox) {
	if(pToolbox) {
		m_pToolbox = pToolbox;
	}
}

void 
IObject::Update(float deltaTime)
{
	if(m_pParent) {
		m_pParent->Update(deltaTime);
	}

	m_transform.Update();

	m_currentFrame = m_pToolbox->frameNumber;
}

void
IObject::Render(Camera* pCamera)
{
}