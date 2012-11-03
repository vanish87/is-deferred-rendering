#pragma once
#include "App.h"
#include "Model.h"

class MyApp :
	public MocapGE:: App
{
public:
	MyApp(void);
	~MyApp(void);
	void InitObjects();
	void ReleaseObjects();

private:

	MocapGE::Model* scene;
};

