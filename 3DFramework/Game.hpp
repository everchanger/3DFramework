#ifndef GAME__H__
#define GAME__H__

// third-party libraries
#include <windows.h>
#include <GL/glew.h>
#include <GL/glfw.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// standard C++ libraries
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <cmath>

// tdogl classes
#include "ResourceManager.hpp"
#include "Program.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "ModelManager.hpp"
#include "Timer.hpp"
#include "Toolbox.hpp"

// constants
const glm::vec2 SCREEN_SIZE(800, 600);

class Game
{
public:
	Game(void);
	~Game(void);

	void Initialize(void);
	void Run(void);
	void Destroy(void);
	void Update(void);
	void Render(void);

private:
	
	ResourceManager*	m_pResourceManager;
	Program*			m_pProgram;
	Camera*				m_pCamera;
	Timer*				m_pTimer;
	ModelManager*		m_pModelManager;

	Toolbox			m_Toolbox;
};

#endif