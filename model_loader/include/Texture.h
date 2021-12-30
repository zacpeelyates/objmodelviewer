#pragma once
#include <string>

class Texture 
{
public:
	Texture() {};
	~Texture();

	bool Load(std::string a_inFilename);
	const std::string& GetFileName() inline const {return m_filename; }
	unsigned int GetTextureID() inline const {return m_textureID; }
	void GetDimensions(unsigned int& a_uiWidth, unsigned int& a_uiHeight) const;

private:
	std::string m_filename;
	unsigned int m_width; 
	unsigned int m_height;
	unsigned int m_textureID;
};

