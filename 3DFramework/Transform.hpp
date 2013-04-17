#ifndef TRANSFORM__H__
#define TRANSFORM__H__

#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Transform
{
public:
	Transform(void);
	~Transform(void);

	glm::mat4& LocalMatrix(void);
	glm::mat4& WorldMatrix(void);

	bool	   isDirty(void);

	Transform*  Parent(void);
	void		SetParent(Transform* pParent);
	
	void		MakeChainDirty(void);

	void		Update(void);

private:
	
	void		AddChild(Transform* pChild);
	
	glm::mat4	m_World;
	glm::mat4	m_Local;
	bool		m_isDirty;

	Transform* m_pParent;
	std::vector<Transform*> m_pChilds;
};

#endif
