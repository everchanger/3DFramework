#ifndef TOOLBOX__H__
#define TOOLBOX__H__

class Game;
class ResourceManager;
class ModelManager;
class Timer;
class Program;

class Toolbox
{
public:
	Toolbox(void) {
		pGame			 = 0;
		pResourceManager = 0;
		pModelManager	= 0;
		pTimer			= 0;
		pProgram		= 0;

		frameNumber = 0;
	};
	~Toolbox(void) {};
	
	Game*				pGame;
	ResourceManager*	pResourceManager;
	ModelManager*	pModelManager;
	Timer*			pTimer;
	Program*		pProgram;
	int				frameNumber;
};

#endif