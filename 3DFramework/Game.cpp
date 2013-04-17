#include "Game.hpp"

Game::Game(void)
{
	m_pResourceManager	= NULL;
	m_pProgram			= NULL;
	m_pCamera			= NULL;
	m_pTimer			= NULL;
	m_pModelManager		= NULL;
}

Game::~Game(void)
{
}

void Game::Initialize(void)
{
	 // initialise GLFW
    if(!glfwInit()) {
        throw std::runtime_error("glfwInit failed");
	}

    // open a window with GLFW
    glfwOpenWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MAJOR, 3);
    glfwOpenWindowHint(GLFW_OPENGL_VERSION_MINOR, 2);
    glfwOpenWindowHint(GLFW_WINDOW_NO_RESIZE, GL_TRUE);
    if(!glfwOpenWindow((int)SCREEN_SIZE.x, (int)SCREEN_SIZE.y, 8, 8, 8, 8, 16, 0, GLFW_WINDOW)) {
        throw std::runtime_error("glfwOpenWindow failed. Can your hardware handle OpenGL 3.2?");
	}
	glfwSetWindowTitle("Test App!");


	glfwSwapInterval(0);

    // GLFW settings
    glfwDisable(GLFW_MOUSE_CURSOR);
    glfwSetMousePos(0, 0);
    glfwSetMouseWheel(0);

    // initialise GLEW
    glewExperimental = GL_TRUE; //stops glew crashing on OSX :-/
    if(glewInit() != GLEW_OK) {
        throw std::runtime_error("glewInit failed");
	}
    
    // GLEW throws some errors, so discard all the errors so far
    while(glGetError() != GL_NO_ERROR) {
	}

    // print out some info about the graphics drivers
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
    std::cout << "Vendor: " << glGetString(GL_VENDOR) << std::endl;
    std::cout << "Renderer: " << glGetString(GL_RENDERER) << std::endl;

    // make sure OpenGL version 3.2 API is available
    if(!GLEW_VERSION_3_2) {
        throw std::runtime_error("OpenGL 3.2 API is not available.");
	}

    // OpenGL settings
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // load vertex and fragment shaders into opengl
    std::vector<Shader> shaders;

	shaders.push_back(Shader::shaderFromFile("data/shaders/vertex-shader.sdr", GL_VERTEX_SHADER));
	shaders.push_back(Shader::shaderFromFile("data/shaders/fragment-shader.sdr", GL_FRAGMENT_SHADER));

	m_pProgram = new Program(shaders);
	m_Toolbox.pProgram = m_pProgram;

	m_pResourceManager = new ResourceManager();
	m_pResourceManager->Initialize(&m_Toolbox);
	m_Toolbox.pResourceManager = m_pResourceManager;

	m_pResourceManager->AddResource("data/models/hammer.obj", "hammer", RT_MODEL);


    // setup gCamera
	m_pCamera = new Camera();

	m_pCamera->Initialize(&m_Toolbox);
    m_pCamera->setPosition(glm::vec3(0,0,4));
    m_pCamera->setViewportAspectRatio(SCREEN_SIZE.x / SCREEN_SIZE.y);


	m_pModelManager = new ModelManager();
	m_pModelManager->Initialize(&m_Toolbox);

	m_Toolbox.pModelManager = m_pModelManager;

	Model* pModel = m_pModelManager->CreateModel("hammer");
	pModel->SetPosition(glm::vec3(0.0f,0.0f,-1.0f));

	pModel->ChildTo(m_pCamera);

	// Create a clock for measuring time elapsed
	m_pTimer = new Timer();

	m_Toolbox.pTimer = m_pTimer;

	m_pTimer->reset();

	m_Toolbox.pGame = this;
}

void Game::Run(void) 
{
	while(glfwGetWindowParam(GLFW_OPENED)){

		// Do all the other updates
		m_pTimer->update();

        Update();
        Render();

		m_Toolbox.frameNumber++;
		printf("\n\n");

		int fps = m_pTimer->getFPS();
		printf("%d\n",fps);

        // check for errors
        GLenum error = glGetError();
        if(error != GL_NO_ERROR) {
            std::cerr << "OpenGL Error " << error << ": " << /*(const char*)gluErrorString(error) <<*/ std::endl;
		}
    }

	// clean up and exit
    glfwTerminate();
}

void Game::Destroy(void)
{
	if(m_pModelManager) {
		m_pModelManager->Destroy();
		delete m_pModelManager;
		m_pModelManager = NULL;
	}
}

void Game::Update(void)
{
	float deltaTime = (float)m_pTimer->getDeltaTime();

	m_pCamera->Update(deltaTime);

	static int tt = 0;
	if(tt > 1000) {
		if(glfwGetKey('1')) {
			Model* pModel = m_pModelManager->CreateModel("hammer");

			pModel->SetPosition(m_pCamera->GetPosition()+glm::vec3(0.0f,0.0f,-2.0f));
			//pModel->ChildTo(m_pModelManager->m_Models[0]->GetTransform());
			
			tt = 0;
		} 
	} else {
		tt++;
	}

	m_pModelManager->Update(deltaTime);

	//exit program if escape key is pressed
    if(glfwGetKey(GLFW_KEY_ESC)) {
        glfwCloseWindow();
	}
}

void Game::Render(void)
{
	// clear everything
    glClearColor(0, 0, 0, 1); // black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	m_pModelManager->Render(m_pCamera);
    
    // swap the display buffers (displays what was just drawn)
    glfwSwapBuffers();
}
