//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 27/09/21
// Last Edited:  30/09/21
// Brief: Main entry point of application. 
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RenderWindow.h"

int main(int argc, char* argv[])
{
	//Main entry point of application
	RenderWindow* modelApp = new RenderWindow();
	modelApp->Run("Model Loader",1280,720,false);
	delete modelApp;
	return 0;
}




