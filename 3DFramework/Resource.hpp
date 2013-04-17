#ifndef RESOURCE__H__
#define RESOURCE__H__

class Toolbox;

class Resource {
public:
	Resource() {};
	~Resource() {};

	virtual void Load(Toolbox* pToolbox, const char* pFileName) = 0;
};

#endif