#pragma once
#include "Window.h"

class App
{
public:
	App();
	// main message - frame loop
	int Go();

private:
	void DoFrame();

	Window wnd;
};

