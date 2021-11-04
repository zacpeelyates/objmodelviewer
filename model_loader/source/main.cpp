//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 27/09/21
// Last Edited:  30/09/21
// Brief: Main entry point of application. Handles user input and calling correct loader class for supplied file.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "RenderWindow.h"



int main(int argc, char* argv[])
{
	//Main entry point of application
	RenderWindow* modelApp = new RenderWindow();
	modelApp->Run("Model Loader",1920,1080,false);
	delete modelApp;
	return 0;
}



//std::string inputpath;
//std::cout << "enter filename: ";
//std::getline(std::cin, inputpath);
//if (filemanager::loadfile(inputpath)) {
//	//get correct parsing class via filetype
//	if (utilities::getfiletype(inputpath) == "obj")
//	{
//		objloader objloader;
//		char charin = ' ';
//		std::cout << "print comments? (y / any other key): ";
//		std::cin >> charin;
//		bool showcomments = std::tolower(charin) == 'y';
//		objdata loaded;
//		utilities::timerreset();
//		if (objloader.objload(inputpath, showcomments, loaded)) //attempt to load obj file (objloader.cpp)
//		{
//			std::cout << "processed in: " << utilities::timertick() << std::endl;
//		}
//		else
//		{
//			std::cout << "failed to load file" << std::endl;
//		}
//	}
//	else
//	{
//		std::cout << "unsupported filetype! exiting." << std::endl;
//		return false;
//	}
//}
//else
//{
//	std::cout << "file failed to initialize! exiting." << std::endl;
//	return false;
//}



