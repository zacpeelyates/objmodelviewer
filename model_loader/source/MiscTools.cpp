#include "MiscTools.h"
#include <sstream>
std::vector<std::string> SplitStringAtChar(std::string a_strData, char a_cDelimiter)
{
	std::vector<std::string> outVec;
	std::stringstream ss(a_strData);
	std::string segment;
	while (std::getline(ss, segment, a_cDelimiter))
	{
		outVec.push_back(segment);
	}
	return outVec;
}

bool ParseStringToInt(std::string& a_rStrIn)
{
	if (a_rStrIn.empty()) return false;
	return std::find_if(a_rStrIn.begin(), a_rStrIn.end(), [](unsigned char c) {return !std::isdigit(c); }) == a_rStrIn.end();
}

