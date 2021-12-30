#include "Texture.h"
#include <stb_image.h>
#include <iostream>;
#include <glad\glad.h>


Texture::~Texture()
{
	glDeleteTextures(1, &m_textureID);
}

bool Texture::Load(std::string a_inFilename, int skyboxOffset)
{
	int width = 0; 
	int height = 0;
	int channels = 0; 
	if (skyboxOffset >= 0)
	{
		stbi_set_flip_vertically_on_load(false);
	}
	else
	{
		stbi_set_flip_vertically_on_load(true);
	}	
	unsigned char* imageData = stbi_load(a_inFilename.c_str(), &width, &height, &channels, 4); //not sure what this 4 means?
	if (imageData != nullptr) 
	{
		m_filename = a_inFilename;
		m_width = width; 
		m_height = height;
		glGenTextures(1, &m_textureID);
		if (skyboxOffset >= 0)
		{
			//skybox data
			glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + skyboxOffset, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
		}
		else 
		{
			//texture data
			glBindTexture(GL_TEXTURE_2D, m_textureID);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
			glGenerateMipmap(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		stbi_image_free(imageData);
		std::cout << "Loaded Image: " << a_inFilename << std::endl;
		return true;
	}

	return false;

}

void Texture::GetDimensions(unsigned int& a_uiWidth, unsigned int& a_uiHeight) const
{
	a_uiWidth = m_width;
	a_uiHeight = m_height;
}
