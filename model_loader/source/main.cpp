#include "FileManager.h"
#include "OBJLoader.h"
#include <string>
#include <iostream>
#include <fstream>
int main(int argc, char* argv[])
{
	std::string inputPath;
	std::cout << "Enter Filename: ";
	std::getline(std::cin,inputPath);
	std::fstream filestream;
	FileManager fileManager(inputPath, filestream);
	if (fileManager.initialized) {
		if (fileManager.GetType() == "obj")
		{
			OBJLoader objLoader;
			char charIn = ' ';
			std::cout << "Print comments? (Y / any other key): ";
			std::cin >> charIn;
			bool showComments = std::tolower(charIn) == 'y';
			objLoader.OBJLoad(fileManager, showComments);
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

