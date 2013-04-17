#ifndef OBJECT__H__
#define OBJECT__H__

#include "Toolbox.hpp"
#include "Transform.hpp"

class Camera;

class IObject
{
public:
	IObject(void);
	~IObject(void);

	bool IsUpToDate(void);

	virtual void Initialize(Toolbox* pToolbox);
	virtual void Update(float deltaTime);
	virtual void Render(Camera* pCamera);

	void ChildTo(IObject* pObject);
	Transform* GetTransform(void) { return &m_transform; }

protected:

	IObject* m_pParent;
	Transform m_transform;

	Toolbox* m_pToolbox;

	int m_currentFrame;
};

#endif