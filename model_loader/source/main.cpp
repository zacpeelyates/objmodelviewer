//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// File: main.cpp
// Author: Zac Peel-Yates (s1703955)
// Date Created: 27/09/21
// Last Edited:  30/09/21
// Brief: Main entry point of application. Handles user input and calling correct loader class for supplied file.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#include "DisplayModel.h"
#include "FileManager.h"
#include "ShaderManager.h"
#include "OBJLoader.h"
#include "Utilities.h"
#include <string>
#include <iostream>
#include <fstream>

int main(int argc, char* argv[])
{
	//Main entry point of application
	std::string inputPath;
	std::cout << "Enter Filename: ";
	std::getline(std::cin,inputPath);
	if (FileManager::LoadFile(inputPath)) {
		//Get correct parsing class via filetype
		if (Utilities::GetFileType(inputPath) == "obj")
		{
			OBJLoader objLoader;
			char charIn = ' ';
			std::cout << "Print comments? (Y / any other key): ";
			std::cin >> charIn;
			bool showComments = std::tolower(charIn) == 'y';
			OBJData loaded;
			Utilities::TimerReset();
			if (objLoader.OBJLoad(inputPath,showComments, loaded)) //Attempt to load OBJ file (OBJLoader.cpp)
			{
				//draw();
				std::cout << "Processed in: " << Utilities::TimerTick() << std::endl;
			}
			else
			{
				std::cout << "failed to load file" << std::endl;
			}
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

