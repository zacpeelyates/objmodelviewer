//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 27/09/21
// Last Edited:  30/09/21
// Brief: Main entry point of application. Handles user input and calling correct loader class for supplied file.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "ModelLoader.h"

int main(int argc, char* argv[])
{
	//Main entry point of application
	ModelLoaderApp* modelApp = new ModelLoaderApp();
	modelApp->Run("Model Loader",1920,1080,false);
	delete modelApp;
	return 0;
}

