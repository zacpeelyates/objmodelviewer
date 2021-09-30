//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 27/09/21
// Last Edited:  30/09/21
// Brief: Main entry point of application. Handles user input and calling correct loader class for supplied file.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "FileManager.h"
#include "OBJLoader.h"
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	//Main entry point of application
	std::string inputPath;
	std::cout << "Enter Filename: ";
	std::getline(std::cin,inputPath);
	std::fstream filestream;
	FileManager fileManager(inputPath, filestream); //Handles file operations (FileManager.cpp)
	if (fileManager.initialized) {
		//Get correct parsing class via filetype
		if (fileManager.GetType() == "obj")
		{
			OBJLoader objLoader;
			char charIn = ' ';
			std::cout << "Print comments? (Y / any other key): ";
			std::cin >> charIn;
			bool showComments = std::tolower(charIn) == 'y';
			objLoader.OBJLoad(fileManager, showComments); //Attempt to laod OBJ file (OBJLoader.cpp)
		}
		else
		{
			std::cout << "Unsupported Filetype! Exiting." << std::endl;
			return 1;
		}
	}
	else
	{
		std::cout << "File failed to initialize! Exiting." << std::endl;
		return 1;
	}
	return 0;
}

